
struct SuperBlock // 24 byte 
{
	int iNodeCount; // +
	int iNodeSize; //+
 	int blockCount; 
	int blockSize; // +  
	int iNodePositions; // +  
	int blockPositions; // + 
}; 



struct iNode // 104 byte 
{
	int fileSize;
	int parentInode;
	char fileName[64]; 
	int fileType; // 0 = root 1 = file, 2 = folder,3 = softlink, 4 = hardlink
	int inodeNumber; // 0-> root
	time_t lastModification; // 8 byte 
	int blocksNum[12];// assigned to 65535 as unused 
};

typedef iNode iNode;
typedef SuperBlock SuperBlock;


int _tokenCount(char *pathName){

	int tokenCount = 0;

    char *token = strtok(pathName, "/"); 

    while (token != NULL) {
    	tokenCount++;
        token = strtok(NULL, "/");
    } 




    return tokenCount;
}



int _strCount( char *str,char ch){

  int tokenCount = 0;
  for (int i = 0; str[i] != '\0'; ++i){
  	if(str[i] == ch){	
  		tokenCount++;
  		++str;  		
  	}
  }

  return tokenCount;
}



int _strcmp(char str[],char *str2){
	
	#ifdef DEBUG
		fprintf(stdout, "Strcmp---- Token 1 : %s == Token 2 %s \n",str,str2);
	#endif 
	

	int lenFirst = strlen(str);
	int lenSec = strlen(str2);

	if (lenFirst != lenSec)
		return 0;

	for (int i = 0; i < lenFirst; ++i)
	{
		if(str[i] != str2[i])
			return 0;
	}


	//otherwise 
	return 1;

}


int countDigit(int num ){

	int count = 0;

	while (num != 0) {
        num /= 10;     // n = n/10
        ++count;
    }
    return count;
}
