#include <iostream>
#include <fstream>
#include <ctime>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include "fileSystem.h"
using namespace std;

char fileSystemName[255];


/*
	#ifdef DEBUG
		fprintf(stdout, "----------------AFTER READING FILE SYSTEM ---------------------------\n");
		fprintf(stdout, "inodeCount :%d inodeSize : %d blockCount : %d \nblockSize : %d iNodePosition: %d blockPosition : %d \n" 
								,sp.iNodeCount,sp.iNodeSize,sp.blockCount,sp.blockSize,sp.iNodePositions,sp.blockPositions );
	#endif
*/

SuperBlock sp;



// Verilen inode numarasında token var mı diye bakar 
// eşitse 1 değilse 0 döner 
int checkGivenInodesNum(int theInodeNum, char *token,FILE *fp){

	#ifdef DEBUG
		fprintf(stdout, "Verilen inode numarasında o token var mı : inode Number : %d token : %s \n",theInodeNum,token);
	#endif

	iNode theInode;
	fseek(fp, sp.iNodePositions + (theInodeNum * sizeof(iNode)) , SEEK_SET);	
	fread(&theInode, sizeof(theInode), 1, fp);

	return _strcmp(theInode.fileName,token);

}


// /usr/bin usr içersinde bin dosyası var mı ?   inodenumber = usr --- token = bin 
int checkBlock(int iNodeNumber, char *token){


	#ifdef DEBUG
		fprintf(stdout, "checkBlocks---- inode number : %d Token : %s\n",iNodeNumber,token );
	#endif 

	FILE *fp;
	iNode theInode;
	fp = fopen(fileSystemName , "r+");

	fseek(fp,sp.iNodePositions + (iNodeNumber * sizeof(iNode)), SEEK_SET);

	fread(&theInode, sizeof(theInode), 1, fp);
	
	#ifdef DEBUG 
		fprintf(stdout, "---------checkBlock inode infos ------\n");
		fprintf(stdout, "ParentInode : %d fileName : %s fileType : %d block num : %d \n",theInode.parentInode,theInode.fileName,theInode.fileType, theInode.blocksNum[0]);
	#endif
	// 3 5 10 
	char theInodeNum;
	char blockData[sp.blockSize];
	char *tokenInode;

	for (int i = 0; i < 12; ++i)
	{	
		
		fseek(fp, sp.blockPositions + (sp.blockSize * theInode.blocksNum[i]) , SEEK_SET);
		if(theInode.blocksNum[i] == 65535)
			break;
		fread(blockData, sizeof(blockData), 1, fp);
		int theInodeCount = _strCount(blockData,'-');
		

		if(theInodeCount == 0) // o block boş ise bakma mkdir yapamazsın 
			break;	

		tokenInode = strtok(blockData, "-");
    	while (tokenInode != NULL) {
    		int childInode = checkGivenInodesNum(atoi(tokenInode),token,fp);
			if(childInode == 1){
				fclose(fp);
				return atoi(tokenInode);
			}
	        tokenInode = strtok(NULL, "-");

	        if(tokenInode[0] == '.'){
	        	fclose(fp);
	        	return -1;
	        }
    	}
			
	}


	fclose(fp);

	return -1;

}

	
// /usr/bin/emre

//Tüm parentlara bakalır eğer doğru ise 1 yoksa 0

// inodeNumber = emrenin inodenumberı // tokencount emreden başlıyor
int rcrCheckParent(int inodeNumber,char *tokensArr[],int tokenCount){

	FILE *fp;
	iNode theInode;

	int lastTokenCount = tokenCount - 1 ;

	fp = fopen(fileSystemName , "r+");

	for (int i = 0; i  < tokenCount; i++)
	{
		fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);  // root 
		fread(&theInode, sizeof(iNode), 1, fp);
		//fprintf(stdout, "parentNumber: %d childToken : %s \n",inodeNumber,tokensArr[i] );

		int hasA = checkBlock(inodeNumber,tokensArr[i]);
		//printf(stdout, "hasA : %d i : %d lastTokenCount : %d \n",hasA,i,lastTokenCount );

		if(hasA == -1 && i == lastTokenCount){ // yazılabilir
			fclose(fp);
			return inodeNumber; // doğrudur yazılabilir 
		}else if(hasA != -1 && i == lastTokenCount){
			fprintf(stderr, "Folder already exist ! \n");
			break;
		}else if(hasA == -1  && i != lastTokenCount){
			fprintf(stderr, "Wrong path ! \n");
			break;
		}
		inodeNumber = hasA;
		
	}

	fclose(fp);

	return -1;
}


//find empty block into bitmap and change it 
int findEmptyBlock(){

	FILE *fp;
	fp = fopen(fileSystemName , "r+");

	char theBitmap[sp.blockSize];
	char one = '1';

	fseek(fp, sizeof(SuperBlock), SEEK_SET); // bitmapin olduğu yerdeyiz
	fread(theBitmap, sizeof(theBitmap), 1, fp);

	for (int i = 0; i < sp.blockSize; ++i){
		if(theBitmap[i] == '0'){
			fseek(fp, sizeof(SuperBlock) + i, SEEK_SET);
			fwrite(&one,sizeof(char), 1 , fp);	
			fclose(fp);	
			return i; // 
		}
	}

	
	fprintf(stdout, "Boş blok yoktur !\n");
	fclose(fp);	

	return -1; //Boş blok yok 

}


//sp + bitmap + inodes + block 

/*
- Eklenecek dosya için boş inode bul bilgilerini yaz ekle 
- Bitmap üzerinden boş blok bul 1 yap
- Eklenecek dosyanın parentının inode ve blok bilgilerini güncelle 
*/
int _writeMkdirInfo(char *dirName,int parentInodeNum){


	FILE *fp;
	iNode theInode;
	iNode parent;

	fp = fopen(fileSystemName , "r+");
	fseek(fp, sp.iNodePositions, SEEK_SET);

	int emptyInodeNum;

	for (int i = 0; i < sp.iNodeCount; ++i)
	{
		fread(&theInode, sizeof(theInode), 1, fp);
		if(theInode.parentInode == -2){
			emptyInodeNum = i;
			break;
		}
	}

	//fprintf(stdout, "empty inode number : %d -- inode Count : %d  \n",emptyInodeNum,sp.iNodeCount );

	if(emptyInodeNum == sp.iNodeCount) {// boş ınode yoktur 
		fprintf(stderr, "Boş inode yoktur.\n");
		return -1;
	}

	int emptyBlock = findEmptyBlock();

	if (emptyBlock == -1){
		fprintf(stderr, "Boş blok yoktur.\n");
		return -1;					
	}
	
	theInode.fileSize = sp.blockSize;
	theInode.parentInode = parentInodeNum;
	strcpy(theInode.fileName,dirName);
	theInode.fileType = 2;
	theInode.inodeNumber = emptyInodeNum;
	theInode.lastModification = time(0);
	theInode.blocksNum[0] = emptyBlock;

	// boş ınode için ilerle ve yaz 
	fseek(fp, sp.iNodePositions + (sp.iNodeSize * emptyInodeNum), SEEK_SET);
	fwrite(&theInode,sizeof(theInode),1,fp); 

	//parent bilgilerine git ve onları güncelle
	fseek(fp, sp.iNodePositions + (sp.iNodeSize * parentInodeNum), SEEK_SET);
	fread(&parent, sizeof(parent), 1, fp);
	
	parent.lastModification = time(0); // parentı directory 


	fseek(fp, sp.iNodePositions + (sp.iNodeSize * parentInodeNum), SEEK_SET);
	fwrite(&parent, sizeof(parent), 1, fp);

	
	// 243 -256 arası blocklara dosya inodelarını yazma
	fseek(fp, sp.blockPositions + (sp.blockSize * parent.blocksNum[0]), SEEK_SET); // eğer 1 doluysa komple 2-3-4 diye devam etmeli 
	char dataBlock[sp.blockSize];
	int blockStartPos = ftell(fp);

	fread(dataBlock,sizeof(dataBlock),1,fp);
	//printf("Strwrite: %s\n",dataBlock );


	char wrData[countDigit(theInode.inodeNumber) + 1];
	sprintf(wrData,"%d-",theInode.inodeNumber);

	char *str = NULL;
	str = strrchr(dataBlock,'-');
	
	if(str != NULL){
		//fprintf(stdout, " %d - %ld \n",sp.blockSize,strlen(str) - 2);
		int writePosition = str -dataBlock + 1;
		//fprintf(stdout, "writePosition : %d\n",writePosition );
		fseek(fp, blockStartPos + writePosition, SEEK_SET);
		fwrite(wrData,sizeof(wrData),1,fp);

	}else { // Null ise daha önce inode sayılmamıştır
		fseek(fp, blockStartPos , SEEK_SET);
		fwrite(wrData,sizeof(wrData),1,fp);
	} 
	
	/*
	for (int i = 0; i < sp.blockSize; ++i)
		printf("%c",arra[i] );
	printf("\n");
	*/
	
	//fprintf(stdout, "Yazılıcak inodeNumber : %d Yazılacak inode : %d\n",theInode.inodeNumber, parentInodeNum);


	//fprintf(stdout, "Write successful ! %d\n " , emptyInodeNum );

	fclose(fp);

	return 1;

}


void _mkdir( char *path){

	#ifdef DEBUG
		fprintf(stdout, "------------------------------------------\n");
		fprintf(stdout, "----------MKDIR DEBUG----------------------\n");
		fprintf(stdout, "------------------------------------------\n");
	#endif

  	int tokenCount = _strCount(path,'/');
	
	char slash[] = "/";
	int i = 0;
	char str[strlen(path) + 1];
	strcpy(str,path);
    char *token = strtok(str, "/"); 
    char *tokensArr[tokenCount];
 	int hasA;

    while (token != NULL) {
        tokensArr[i++] = token; 
        token = strtok(NULL, "/");
    } 

    if(_strcmp(slash,path))
    	tokenCount = 0;

    #ifdef DEBUG 
    	printf("------------Tokens for path--------- Token count : %d \n",tokenCount);
	    for (i = 0; i < tokenCount; ++i)
	       	printf("Token[%d] : %s\n",i, tokensArr[i]);
	#endif

	int checkInode = rcrCheckParent(0,tokensArr,tokenCount); // checkInode yazılacak dosyanın parent inodedur.
	if(checkInode >= 0 )
	 	_writeMkdirInfo(tokensArr[tokenCount - 1],checkInode);

	#ifdef DEBUG
		fprintf(stdout, "------------------------------------------\n");
		fprintf(stdout, "----------END MKDIR DEBUG----------------------\n");
		fprintf(stdout, "------------------------------------------\n");
	#endif
	

}


void printTheInodeNumber(int inodeNumber,FILE *fp){
	iNode listInode;
	//fprintf(stdout, "---%d\n",inodeNumber );
	fseek(fp, sp.iNodePositions + (sp.iNodeSize * inodeNumber) , SEEK_SET);
	fread(&listInode,sizeof(iNode),1,fp);
	fprintf(stdout, "%s\t%d\t%s\n",listInode.fileName,listInode.fileSize ,ctime(&listInode.lastModification));


}


void printListInfo(int inodeNum){

	FILE *fp;
	iNode listInode;
	iNode childInode;

	fp = fopen(fileSystemName , "r+");

	fseek(fp, sp.iNodePositions + (sp.iNodeSize * inodeNum) , SEEK_SET);
	fread(&listInode,sizeof(iNode),1,fp);

	if(listInode.fileType != 2 && listInode.fileType != 0){
		fprintf(stderr, "It isn't folder ! File type : %d \n",listInode.fileType );
		return;
	}
	
	char tempBlock[sp.blockSize];  

	for (int i = 0; i < 12; ++i)
	{
		//fprintf(stdout, "listInode.blocksNum[i] : %d \n",listInode.blocksNum[i] );
		if(listInode.blocksNum[i] != 65535){
			fseek(fp,sp.blockPositions + (listInode.blocksNum[i] * sp.blockSize),SEEK_SET);
			fread(tempBlock,sizeof(tempBlock),1,fp);
		    char *token = strtok(tempBlock, "-"); 

		    while (token != NULL && token[0] != '.') {
		    	printTheInodeNumber(atoi(token),fp);
		        token = strtok(NULL, "-");
		    }
		}else{
			if(listInode.fileType == 0)
				fprintf(stdout, ". \n");
			else
				fprintf(stdout, ". ..\n");
			break;
		}

	}

	fclose(fp);


}


int rcrCheckParent2(int inodeNumber,char *tokensArr[],int tokenCount){

	FILE *fp;
	iNode theInode;

	int lastTokenCount = tokenCount - 1 ;

	fp = fopen(fileSystemName , "r+");
	for (int i = 0; i  < tokenCount; i++)
	{
		fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);  // root 
		fread(&theInode, sizeof(iNode), 1, fp);
		//fprintf(stdout, "parentNumber: %d childToken : %s \n",inodeNumber,tokensArr[i] );
		int hasA = checkBlock(inodeNumber,tokensArr[i]);
		//fprintf(stdout, "hasA : %d i : %d lastTokenCount : %d \n",hasA,i,lastTokenCount );
		if(hasA == -1 && i == lastTokenCount){ 
			fprintf(stderr, "Hatalı mkdir \n");
			break;
		}else if(hasA != -1 && i == lastTokenCount){
			fclose(fp);
			return hasA;
		}else if(hasA == -1  && i != lastTokenCount){
			fprintf(stderr, "Hatalı path \n");
			break;
		}
		inodeNumber = hasA;
		
	}

	fclose(fp);

	return -1;
}




void _list(char *path){
	#ifdef DEBUG
		fprintf(stdout, "------------------------------------------\n");
		fprintf(stdout, "----------LIST DEBUG----------------------\n");
		fprintf(stdout, "------------------------------------------\n");
	#endif

	int tokenCount = _strCount(path,'/');
	
	char slash[] = "/";
	int i = 0;
	char str[strlen(path) + 1];
	strcpy(str,path);
    char *token = strtok(str, "/"); 
    char *tokensArr[tokenCount];
 	int hasA;

    while (token != NULL) {
        tokensArr[i++] = token; 
        token = strtok(NULL, "/");
    } 

    if(_strcmp(slash,path)){
    	 printListInfo(0);
    	 return;
    }

	int chc = rcrCheckParent2(0,tokensArr,tokenCount);
 	if(chc != -1){
 		printListInfo(chc);
 	}


	#ifdef DEBUG
		fprintf(stdout, "------------------------------------------\n");
		fprintf(stdout, "----------END LIST DEBUG----------------------\n");
		fprintf(stdout, "------------------------------------------\n");
	#endif
}



void removeInodeNumber (char *blockInfo, char *rmData) {

    
    char *cmp;
    int length = strlen(rmData);


    while ((cmp = strstr(blockInfo, rmData))) {
        *cmp = '\0';
        strcat(blockInfo, cmp + length);
    }
    
    char point[length - 1];
    for (int i = 0; i < length - 1; ++i)
    	point[i] = '.';
    

    #ifdef DEBUG 
	    for (int i = 0; i < length; ++i)
	    	fprintf(stdout, "point : %c\n",point[i] );
    	for (int i = 0; i < 1024; ++i)
   			printf("%c",blockInfo[i]);
    #endif 
    
	
    strcat(blockInfo, point);
    blockInfo[sp.blockSize - 1] = '\0';
   

}


void updateRmdirInfo(int inodeNumber){ 
	
	FILE *fp;
	iNode theInodeChild;
	iNode theParent; 
	iNode freeInode;
	//------------------FREE INODE -----------------------------
	freeInode.parentInode = -2; // kullanılmayan boş inode için
	freeInode.fileType = 5; // boş file type

	for (int j = 0; j < 12; ++j)
		freeInode.blocksNum[j] = 65535;
	//------------------FREE INODE -----------------------------
	

	char blockInfo[sp.blockSize];
	char rmData[countDigit(inodeNumber) + 1];
	char point;
	char zero = '0';

	fp = fopen(fileSystemName , "r+");
	fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);   
	fread(&theInodeChild, sizeof(iNode), 1, fp); // silinecek dosya inode bilgileri

	fseek(fp, sp.blockPositions + (theInodeChild.blocksNum[0] * sp.blockSize) , SEEK_SET);
	fread(&point,sizeof(char),1,fp);
	fseek(fp,-1,SEEK_CUR);

	if(point != '.'){ // blok boş değildir 
		fprintf(stderr, "rmdir : failed to remove '%s' : Directory not empty block numarası : %d \n",theInodeChild.fileName,theInodeChild.blocksNum[0] );
		return;
	}else{ // boştur sil
		fseek(fp, sp.iNodePositions + (theInodeChild.parentInode * sp.iNodeSize) , SEEK_SET);
		fread(&theParent, sizeof(iNode), 1, fp); // silinecek dosyanın parent Inodeu

		theParent.lastModification = time(0); // parentı directory 
		fseek(fp, sp.iNodePositions + (theInodeChild.parentInode * sp.iNodeSize) , SEEK_SET);
		fwrite(&theParent, sizeof(theParent), 1, fp);
		
		fseek(fp, sp.blockPositions + (theParent.blocksNum[0] * sp.blockSize) , SEEK_SET);
		fread(blockInfo,sp.blockSize,1,fp);
		sprintf(rmData,"%d-",inodeNumber);
		#ifdef DEBUG
			for (int i = 0; i < sp.blockSize; ++i)
				printf("%c",blockInfo[i] );
			printf("\n\n");
			//fprintf(stdout, "blockInfo sadas: %ld\n",sizeof(blockInfo) );
		#endif
		removeInodeNumber(blockInfo,rmData);

		#ifdef DEBUG
			for (int i = 0; i < sp.blockSize; ++i)
				fprintf(stdout, "%c",blockInfo[i]);
			printf("\n");
			//fprintf(stdout, "Remove parent block size : %ld\n",sizeof(blockInfo) );
		#endif 

		fseek(fp, sp.blockPositions + (theParent.blocksNum[0] * sp.blockSize) , SEEK_SET);
		fwrite(blockInfo,sp.blockSize,1,fp);

		fseek(fp, sizeof(SuperBlock) + theInodeChild.blocksNum[0], SEEK_SET); 
		fwrite(&zero,sizeof(zero),1,fp); //bitmap bloğunu 0'ladık

		fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);  
		fwrite(&freeInode,sizeof(freeInode),1,fp); //Inode'u 0'ladık
		
	}

	fclose(fp);

}




void _rmdir(char **tokensArr,int tokenCount){
	
	int i;
	#ifdef DEBUG 
		printf("------------Tokens for path--------- Token count : %d \n",tokenCount);
		for (i = 0; i < tokenCount; ++i)
			printf("Token[%d] : %s\n",i, tokensArr[i]);
	#endif

	if(tokenCount == 0){
		fprintf(stderr, "Operation is not available !\n");
		return;
	}

	int chc = rcrCheckParent2(0,tokensArr,tokenCount);
	if(chc != -1) 
 		updateRmdirInfo(chc);

}

int getEmptyInodeNum(){

	iNode theInode;
	FILE *fp;
	fp = fopen(fileSystemName , "r+");

	if (fp == NULL)
    		return  -1;

	fseek(fp, sp.iNodePositions, SEEK_SET);

	for (int i = 0; i < sp.iNodeCount; ++i)
	{
		fread(&theInode, sizeof(theInode), 1, fp);
		if(theInode.parentInode == -2){
			return i;
		}
	}
}

void updateParentInfo(int parentInodeNum,FILE *fp,struct iNode theInode){


	iNode parent;

	//parent bilgilerine git ve onları güncelle
	fseek(fp, sp.iNodePositions + (sp.iNodeSize * parentInodeNum), SEEK_SET);
	fread(&parent, sizeof(parent), 1, fp);
	
	parent.lastModification = time(0); // parentı directory 


	fseek(fp, sp.iNodePositions + (sp.iNodeSize * parentInodeNum), SEEK_SET);
	fwrite(&parent, sizeof(parent), 1, fp);

	
	// 243 -256 arası blocklara dosya inodelarını yazma
	fseek(fp, sp.blockPositions + (sp.blockSize * parent.blocksNum[0]), SEEK_SET); // eğer 1 doluysa komple 2-3-4 diye devam etmeli 
	char dataBlock[sp.blockSize];
	int blockStartPos = ftell(fp);

	fread(dataBlock,sizeof(dataBlock),1,fp);


	char wrData[countDigit(theInode.inodeNumber) + 1];
	sprintf(wrData,"%d-",theInode.inodeNumber);

	char *str = NULL;
	str = strrchr(dataBlock,'-');
	
	if(str != NULL){
		//fprintf(stdout, " %d - %ld \n",sp.blockSize,strlen(str) - 2);
		int writePosition = str -dataBlock + 1;
		fseek(fp, blockStartPos + writePosition, SEEK_SET);
		fwrite(wrData,sizeof(wrData),1,fp);

	}else { // Null ise daha önce inode sayılmamıştır
		fseek(fp, blockStartPos , SEEK_SET);
		fwrite(wrData,sizeof(wrData),1,fp);
	} 


}


void _writeFileInfo(char *destinationFile,int parentInodeNum,char *sourceFile){


	iNode theInode;
	// Öncelikle veriyi okuyalım 
	FILE *sourceFp;
	sourceFp = fopen(sourceFile, "r+");

	if (sourceFp == NULL) {
    		perror("Failed open file: ");
    		return;
	}

	fseek(sourceFp, 0, SEEK_END);
	int sourceFileSize = ftell(sourceFp);
	#ifdef DEBUG
		//fprintf(stdout, "%d\n",sourceFileSize );  
	#endif
	fseek(sourceFp, 0, SEEK_SET);
	// Dosyanın size bilgisi

	FILE *systemFp;
	systemFp = fopen(fileSystemName , "r+");

	if (systemFp == NULL) {
		fclose(sourceFp);
		perror("Failed open file: ");
		return;
	}

	int emptyInode = getEmptyInodeNum();
	if(emptyInode == -1){
		fclose(sourceFp);
		fclose(systemFp);
		perror("İnode yok \n ");
		return;
	}

	fseek(systemFp, sp.iNodePositions + (sp.iNodeSize * emptyInode) , SEEK_SET); // yeni inode 

	theInode.fileSize = sourceFileSize;
	theInode.parentInode = parentInodeNum;
	strcpy(theInode.fileName,destinationFile);
	theInode.fileType = 1; // file
	theInode.inodeNumber = emptyInode;
	theInode.lastModification = time(0);


	if(sourceFileSize == 0){ // boş dosya sadece inode al 

		fwrite(&theInode,sizeof(iNode), 1 ,systemFp);
		updateParentInfo(parentInodeNum,systemFp,theInode);
		//fprintf(stdout, "Write to file finished \n");

	}else if(sourceFileSize > 0){
		int blockCount;
		
		if (sourceFileSize % sp.blockSize == 0)
			blockCount = sourceFileSize / sp.blockSize;
		else 
			blockCount = (sourceFileSize / sp.blockSize) + 1;
		//int tempBlockNum[blockCount];
		for (int i = 0; i < blockCount; ++i)
		{
			int blockNum = findEmptyBlock();
			if(blockNum == -1){ // yeterli boş blok yok yazılamıyor 
				fprintf(stderr, "Boş blok yok işlem yapılamadı !\n");
				fseek(systemFp, sp.iNodePositions + (sp.iNodeSize * emptyInode) , SEEK_SET); // yeni inode 
				iNode freeInode; 

				freeInode.parentInode = -2; // kullanılmayan boş inode için
				freeInode.fileType = 5; // boş file type
				for (int k = 0; k < 12; ++k)
					freeInode.blocksNum[k] = 65535;

				fwrite(&freeInode,sizeof(iNode), 1 ,systemFp);
				
				fclose(sourceFp);
				fclose(systemFp);
				return;
			}	
			theInode.blocksNum[i] = blockNum; 
			//tempBlockNum[i] = blockNum;
		}

		for (int i = blockCount; i < 12; ++i)
			theInode.blocksNum[i] = 65535; 

		fwrite(&theInode,sizeof(iNode), 1 ,systemFp);

		updateParentInfo(parentInodeNum,systemFp,theInode);
		char ch;
		int remainingSize = 0;
		for (int i = 0; i < blockCount; ++i)
		{
			fseek(systemFp,sp.blockPositions + (theInode.blocksNum[i] * sp.blockSize),SEEK_SET);
			//fprintf(stdout, "remainingSize : %d blockNum : %d \n",remainingSize,theInode.blocksNum[i] );
			int blockSizeRemain = 0;
			for(int j = remainingSize; j < sourceFileSize; j++){
				if(blockSizeRemain == sp.blockSize)
					break;
				fread(&ch,sizeof(ch), 1 ,sourceFp);
				fwrite(&ch,sizeof(ch), 1 ,systemFp);
				remainingSize++;
				blockSizeRemain++;
			}

		}
		#ifdef DEBUG 
			/*
			fprintf(stdout, "block num : %d \n",theInode.blocksNum[1]  );
			char readBlock[sp.blockSize];
			fseek(systemFp,sp.blockPositions + (theInode.blocksNum[1] * sp.blockSize),SEEK_SET);
			fread(&readBlock,sizeof(readBlock),1,systemFp);

			
			for (int i = 0; i < sp.blockSize; ++i)
				printf("%c",readBlock[i] );
			fprintf(stdout, "\n");*/
		#endif 
	}else{ 
		perror("Error about negative size: ");
		fclose(sourceFp);
		fclose(systemFp);
		return;
	}

	fclose(sourceFp);
	fclose(systemFp);


}


void _write(char **tokensArr,int tokenCount,char *sourceFile){

	//printf("Token count : %d == sourceFile : %s\n",tokenCount, sourceFile);


	int parentInode = rcrCheckParent(0,tokensArr,tokenCount); // checkInode yazılacak dosyanın parent inodedur.
	if(parentInode >= 0 )
	 	_writeFileInfo(tokensArr[tokenCount - 1],parentInode,sourceFile);
	else 
	 	fprintf(stderr, "Hata \n");

}


void writeToDesFile(int inodeFile, char *destFile){


	iNode theInode;
	// Öncelikle veriyi okuyalım 
	FILE *destFp;
	destFp = fopen(destFile, "w+");

	if (destFp == NULL) {
    		perror("Failed open file: ---");
    		return;
	}

	FILE *systemFp;
	systemFp = fopen(fileSystemName , "r+");

	if (systemFp == NULL) {
    		perror("Failed open file: ");
    		return;
	}

	iNode sourceInode;

	fseek(systemFp,sp.iNodePositions + (inodeFile * sp.iNodeSize),SEEK_SET);
	fread(&sourceInode,sizeof(iNode),1,systemFp);

	if(sourceInode.fileType == 0 || sourceInode.fileType == 2){
		fprintf(stderr, "Filetype is not appropriate ! : %d \n",sourceInode.fileType );
		return;
	}


	int sourceFileSize = sourceInode.fileSize;

	char ch;
	int remainingSize = 0;	
	//printf("sourceFileSize : %d \n",sourceFileSize );

		
	for(int i = 0; sourceInode.blocksNum[i] != 65535; i++){
		
		if(inodeFile != 0 && sourceInode.blocksNum[i] == 0)
			continue;
		fseek(systemFp,sp.blockPositions + (sourceInode.blocksNum[i] * sp.blockSize),SEEK_SET);
		int blockSizeRemain = 0;
		for(int j = remainingSize; j < sourceFileSize; j++){
			if(blockSizeRemain == sp.blockSize)
				break;

			fread(&ch,sizeof(ch), 1 ,systemFp);
			fwrite(&ch,sizeof(ch), 1 ,destFp);
			remainingSize++;
			blockSizeRemain++;
		}
	}

	fclose(destFp);
	fclose(systemFp);

}


void _read(char **tokensArr,int tokenCount,char *destFile){


	//printf("Token count : %d == sourceFile : %s\n",tokenCount, destFile);

	int inodeFile = rcrCheckParent2(0,tokensArr,tokenCount);

	if(inodeFile >= 0 )
	 	writeToDesFile(inodeFile,destFile);
	else 
	 	fprintf(stderr, "Hata \n");

}



int updateDel(int inodeNumber){ 
	
	FILE *fp;
	iNode theInodeChild;
	iNode theParent; 
	iNode freeInode;

	//fprintf(stdout, "inodenumber : %d\n",inodeNumber );
	

	char blockInfo[sp.blockSize];
	char rmData[countDigit(inodeNumber) + 1];
	char point;
	char zero = '0';

	fp = fopen(fileSystemName , "r+");
	fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);   
	fread(&theInodeChild, sizeof(iNode), 1, fp); // silinecek dosya inode bilgileri

	//sadece inode temizle ve parent bilgilerini temizle
	// PARENT BİLGİLERİ GUNCEL 
	fseek(fp, sp.iNodePositions + (theInodeChild.parentInode * sp.iNodeSize) , SEEK_SET);
	fread(&theParent, sizeof(iNode), 1, fp); // silinecek dosyanın parent Inodeu

	theParent.lastModification = time(0); 
	fseek(fp, sp.iNodePositions + (theInodeChild.parentInode * sp.iNodeSize) , SEEK_SET);
	fwrite(&theParent, sizeof(theParent), 1, fp);


	fseek(fp, sp.blockPositions + (theParent.blocksNum[0] * sp.blockSize) , SEEK_SET);
	fread(blockInfo,sp.blockSize,1,fp);
	sprintf(rmData,"%d-",inodeNumber);

	removeInodeNumber(blockInfo,rmData);

	fseek(fp, sp.blockPositions + (theParent.blocksNum[0] * sp.blockSize) , SEEK_SET);
	fwrite(blockInfo,sp.blockSize,1,fp);

	fseek(fp, sp.iNodePositions + (inodeNumber * sp.iNodeSize) , SEEK_SET);  
	fwrite(&freeInode,sizeof(iNode),1,fp); //Inode'u 0'ladık

	
	char freeBlock[sp.blockSize]; // blocks 

    for (int j = 0; j < sp.blockSize; ++j)
    	freeBlock[j] = '.';

	if(theInodeChild.fileSize > 0){ // + bitMap temizle

		for(int i = 0; i < theInodeChild.blocksNum[i] != 65535 && i > 12; i++){
			fseek(fp, sizeof(SuperBlock) + theInodeChild.blocksNum[i], SEEK_SET); 
			fwrite(&zero,sizeof(char),1,fp); //bitmap bloğunu 0'ladık

			fseek(fp, sp.blockPositions + (theInodeChild.blocksNum[i] * sp.blockSize), SEEK_SET);
			fwrite(freeBlock,sizeof(freeBlock),1,fp);

		}

	}
		
	fclose(fp);

}


void _del(char **tokensArr,int tokenCount){



	if(tokenCount == 0){
		fprintf(stderr, "Operation is not available !\n");
		return;
	}

	int chc = rcrCheckParent2(0,tokensArr,tokenCount);

	if(chc != -1)
 		updateDel(chc);


}



/*
It will list block count, i-node count, free
block and i-nodes, number of files and directories, and
block size.
*/
void _dumpe2fs(){
	FILE *fp;
	iNode dmpInode;
	fp = fopen(fileSystemName, "r+");
	int freeInodeCount = 0;
	int freeBlockCount = 0;
	int fileCount = 0;
	int dirCount = 0;

	fprintf(stdout, "--Block Size:\t %d\n",sp.blockSize );
	fprintf(stdout, "--İnode count :\t %d\n",sp.iNodeCount );
	fprintf(stdout, "--Block count :\t %d\n",sp.blockCount );

	// freeInode count 
	for (int i = 0; i < sp.iNodeCount; ++i)
	{
		fseek(fp, sp.iNodePositions + (i * sp.iNodeSize) , SEEK_SET);
		fread(&dmpInode,sizeof(dmpInode),1,fp);
		if(dmpInode.parentInode == -2)
			freeInodeCount++;	

		if(dmpInode.fileType == 1)
			fileCount++;
		else if(dmpInode.fileType == 2 || dmpInode.fileType == 0)	
			dirCount++;

	}

	// free block count 
	fseek(fp, sizeof(struct SuperBlock), SEEK_SET);
	char ch;
	for (int i = 0; i < sp.blockCount; ++i)
	{
		fread(&ch,sizeof(ch),1,fp);
		//fprintf(stdout, "%c ",ch );
		if(ch == '0')
			freeBlockCount++;
	}



	fprintf(stdout, "--Free block count :\t %d\n",freeBlockCount);
	fprintf(stdout, "--Free inode count :\t %d\n",freeInodeCount);
	fprintf(stdout, "--Number of files :\t %d\n",fileCount );
	fprintf(stdout, "--Number of directory :\t %d\n",dirCount );
	char bitCh;
	
	for (int i = 0; i < sp.iNodeCount; ++i)
	{
		fseek(fp, sp.iNodePositions + (i * sp.iNodeSize) , SEEK_SET);
		fread(&dmpInode,sizeof(dmpInode),1,fp);
		
		if(dmpInode.parentInode != -2){
			fprintf(stdout, "--İnode count : %d \t", i);
			fprintf(stdout, "Blocks : ");
			for (int j = 0; j < 12 && dmpInode.blocksNum[j] != 65535; ++j)
			{	
				fprintf(stdout, "%d ",dmpInode.blocksNum[j]);
			}
			fprintf(stdout, "\t");
			fprintf(stdout, "Filename: %s \t",dmpInode.fileName);
			if(dmpInode.fileType == 0)
				fprintf(stdout, "File-Type : Folder - ROOT");
			else if(dmpInode.fileType == 2)
				fprintf(stdout, "File-Type : Folder");
			else if(dmpInode.fileType == 1)
				fprintf(stdout, "File-Type : File");
			else
				fprintf(stdout, "File-Type : Unknown %d",dmpInode.fileType);
			fprintf(stdout, "\n");

		}
	}


}


int main(int argc, char const *argv[])
{
	char operationName[255],pathName[255],sourceFile[255];
	FILE *fp;



	if(argc == 3){
		strcpy(fileSystemName,argv[1]);
		strcpy(operationName,argv[2]);
	}else if(argc == 4){
		strcpy(fileSystemName,argv[1]);
		strcpy(operationName,argv[2]);	
		strcpy(pathName,argv[3]);
	}else if(argc == 5){
		strcpy(fileSystemName,argv[1]);
		strcpy(operationName,argv[2]);	
		strcpy(pathName,argv[3]);
		strcpy(sourceFile,argv[4]);
	}else{	
		fprintf(stderr, "Eksik paramatre girdiniz !\n");
	}

	#ifdef DEBUG 
		if(argc == 4)
			fprintf(stdout, "File system name : %s operation Name: %s pathName : %s \n",fileSystemName,operationName,pathName );
		else 
			fprintf(stdout, "File system name : %s operation Name: %s pathName : %s dataName : %s \n",fileSystemName,operationName,pathName,sourceFile );
	#endif 


	fp = fopen(fileSystemName , "r+");
	fread(&sp, sizeof(sp), 1, fp);
	fclose(fp);
	char cpPathname[255]; 

	strcpy(cpPathname,pathName);

	// 493 - 506 aralığını mkdir ve list içinden sil parametre olarak yolla 
	int tokenCount = _tokenCount(cpPathname);
	
	char slash[] = "/";
	int i = 0;
	char str[strlen(pathName) + 1];
	strcpy(str,pathName);
    char *token = strtok(str, "/"); 
    char *tokensArr[tokenCount];

    while (token != NULL) {
        tokensArr[i++] = token; 
        token = strtok(NULL, "/");
    } 


	if(strcmp(operationName,"mkdir") == 0)
		_mkdir(pathName);
	else if(strcmp(operationName,"list") == 0)
		_list(pathName);
	else if(strcmp(operationName,"rmdir") == 0)
		_rmdir(tokensArr,tokenCount);
	else if(strcmp(operationName,"write") == 0)
		_write(tokensArr,tokenCount,sourceFile);
	else if(strcmp(operationName,"read") == 0)
		_read(tokensArr,tokenCount,sourceFile);
	else if(strcmp(operationName,"del") == 0)
		_del(tokensArr,tokenCount);
	else if(strcmp(operationName,"dumpe2fs") == 0)
		_dumpe2fs();


	return 0;
}

