#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memcpy and strlen *///#include <conio.h>

unsigned char ascii_to_hex(char val)
{
	if('0' <= val && '9' >= val){
		return (unsigned char)(val - '0');
	}else if('a' <= val && 'f' >= val){
		return (unsigned char)(val - 'a') + 0x0a;	
	}else if('A' <= val && 'F' >= val){
		return (unsigned char)(val - 'A') + 0x0a;	
	}
	
	return 0xff;/* error */
}

unsigned char pattern[32];
unsigned char* buffer;
unsigned long lSize;
 int length = 0;
  char filename[200];


int findOffset()
{
	FILE * pFile;		
	int result = 0;
	int i,j;
	int offset = -1;

	pFile = fopen( filename , "r+b");
	if (pFile==NULL) 
	{
		//fputs ("File error",stderr); 
		return -1;
	}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  
  //printf("File size %d bytes\n",lSize);


  // allocate memory to contain the whole file:
  buffer = (unsigned char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) 
  {
	  //fputs ("Memory error",stderr); 
	  return -1;
  }

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) 
  {
	  //printf("Reading error"); 
	  return -1;
  }

  for(i=0;i<lSize-length;i++)
  {
	  for(j=0;j<length;j++)
	  {
		  if(pattern[j] != buffer[i+j])
		  {
			  result = 0;
			  break;
		  }
		  result = 1;
	  }

	  if(result == 1)
	  {
		  offset = i;
		  break;
	  }

  }

  fclose(pFile);
  return offset;
}




int main(int argc, char *argv[])
{
	 char str[256];
	 int i=0;
	 unsigned char val;
	 int offset;

	while(argc != 3){
    	    return 0;
	}	

	memcpy(str, argv[1], strlen(argv[1]));
	str[strlen(argv[1])] = '\0';
	memcpy(filename, argv[2], strlen(argv[2]));
	filename[strlen(argv[2])] = '\0';
	 //printf("Please enter file name\n");
	 //scanf("%s",filename);

	//printf("Please enter byte pattern to match\n");
	//scanf("%s",str);

	length = strlen(str);
	if(length %2 != 0)
	{
		//printf("invalid pattern length\n");
		goto DONE;
	}
	/*Convert ascii to hex, check for invalid characters*/
	for(i=0;i<length;i++)
	{
		val = ascii_to_hex(str[i]);
		if(val == 0xff)
		{
			//printf("Invalid character\n");
			goto DONE;
		}
		else
		{
			if((i&1) == 0)
				val <<= 4;

			pattern[i>>1] |= val;
		}
	}

	length = length/2;
	offset = findOffset();

	if(offset != -1)		
		printf("%d\n",offset);

DONE:
	if(buffer)
		free(buffer);

	return 0;
}

