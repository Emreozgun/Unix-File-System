#include <iostream>
#include <fstream>
#include <ctime>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include "fileSystem.h"
using namespace std;


int main(int argc, char const *argv[])
{
	
	FILE *fp;
	char zero = '0';
    int i,k,j;
    char dataFileName[256]; 
	SuperBlock spBlock;
	
	#ifdef DEBUG
    	fprintf(stdout, "++ Sizeof iNode :%ld Sizeof SuperBlock :%ld \n",sizeof(iNode), sizeof(SuperBlock));
	#endif


	if(argc != 4)
		fprintf(stderr, "Eksik paramatre girdiniz !\n");


	// assing to spBlock 
    spBlock.blockSize =  1024 * atoi(argv[1]);
    spBlock.iNodeCount = atoi(argv[2]);
    strcpy(dataFileName,argv[3]);
    spBlock.iNodePositions = spBlock.blockSize + sizeof(SuperBlock); // starting address of iNode
    spBlock.iNodeSize = sizeof(iNode);

   	if((sizeof(SuperBlock) +  (spBlock.iNodeSize * spBlock.iNodeCount) + spBlock.blockSize) >= (1024 * 1024)){
    	fprintf(stderr, "File system is greater than 1 mb !\n");
    	return 0;
    }

	iNode theInode[spBlock.iNodeCount];




	for (int i = 0; i < spBlock.iNodeCount; ++i){
		theInode[i].parentInode = -2; // kullanılmayan boş inode için
		theInode[i].fileType = 5; // boş file type
	}
	
	for (int i = 0; i < spBlock.iNodeCount; ++i)
		for (int j = 0; j < 12; ++j)
			theInode[i].blocksNum[j] = 65535;




    #ifdef DEBUG
    	fprintf(stdout, "-- Block size : %d iNode Count : %d Data File Name : %s \n",spBlock.blockSize,spBlock.iNodeCount,dataFileName );
	#endif

    char bitMap[spBlock.blockSize]; 
    for (i = 0; i < spBlock.blockSize; ++i) // fill bitMap with 0 
		bitMap[i] = '0';
	bitMap[spBlock.blockSize - 1] = '\0';
    
    spBlock.blockCount = (1024 * 1024 - (sizeof(spBlock) + sizeof(theInode) + sizeof(bitMap))) / (spBlock.blockSize);

    char blocks[spBlock.blockSize]; // blocks 

    for (j = 0; j < spBlock.blockSize; ++j)
    	blocks[j] = '.';




	int unusedSize = 1024*1024 - (sizeof(spBlock) + sizeof(theInode) + sizeof(bitMap) + (sizeof(blocks) *spBlock.blockCount)  );

	char unusedText[unusedSize];
	for (int i = 0; i < unusedSize; ++i)
		unusedText[i] = '\0';

	spBlock.blockPositions = sizeof(spBlock) + sizeof(theInode) + sizeof(bitMap) + unusedSize; // SuperBlock + Inode + bitMap = starting of blocks address 

	#ifdef DEBUG
		fprintf(stdout, "++ %d = (%d - (e%ld + %ld + %ld)) / %d  \n",spBlock.blockCount ,1024 * 1024,sizeof(spBlock),sizeof(theInode),sizeof(bitMap), spBlock.blockSize );
	#endif



    // Fill data file with 0 -- 1mb 
    fp = fopen( dataFileName , "wr+" );



	fwrite(&spBlock,sizeof(struct SuperBlock), 1 , fp); // write superBlock to file system
	fwrite(bitMap,spBlock.blockSize,1, fp); // write bitMap to file system
	fwrite(theInode,sizeof(struct iNode),spBlock.iNodeCount,fp); // write inodes to file system
	fwrite(unusedText,sizeof(unusedText), 1, fp);


	//fprintf(stdout, "-----%d\n",spBlock.blockSize );

	#ifdef DEBUG
		long int writtenSize = ftell(fp);
		fprintf(stdout, "-- Unused size : %d\n",unusedSize);
		fseek(fp, 0, SEEK_END); // return back to starting of file 
		fprintf(stdout, "++ written block size : %ld\n",ftell(fp) - writtenSize );
	#endif

	for (i = 0; i < spBlock.blockCount; ++i) // write blocks to file system
		fwrite(blocks, spBlock.blockSize,1, fp);

	
	
	fseek(fp,  spBlock.iNodePositions, SEEK_SET);

	// Assings for root 
	iNode root;
	strcpy(root.fileName,"root");
	root.fileType = 0;
	root.inodeNumber = 0;
	root.lastModification = time(0);
	root.blocksNum[0] = 0;
	for (int j = 1; j < 12; ++j)
		root.blocksNum[j] = 65535;
	root.parentInode = -1;
	root.fileSize = 0;

	fwrite(&root,sizeof(iNode), 1, fp);

	//bitmap changed  
	fseek(fp, sizeof(spBlock), SEEK_SET);
	char one = '1';
	fwrite(&one,sizeof(char), 1 , fp);




	#ifdef DEBUG
		fseek(fp, -1, SEEK_CUR);
		fread(&one, sizeof(one), 1, fp);
		printf("char : %c\n",one );
	#endif


	#ifdef DEBUG
		char bloktemp[spBlock.blockSize];
		//fprintf(stdout, "block blockPositions : %d \n", spBlock.blockPositions );
		fseek(fp,spBlock.blockPositions, SEEK_SET);
		fread(bloktemp,sizeof(bloktemp), 1, fp);
		printf("xxx: ---%s\n",bloktemp );
		printf("unusedSize : %d\n", unusedSize);
	#endif

		//printf("%s",unusedText); 

	fclose(fp);


	/*#######################---PART-2---##########################*/

	/*#######################---PART-2---##########################*/	
	return 0;
}
