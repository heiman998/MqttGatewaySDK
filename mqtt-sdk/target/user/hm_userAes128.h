//*********************************************************
//	AES128 encrypt&decrypt

// out buf len = dwDataLen + 16 return encrypt data len
unsigned int EncryptBuf(char *lpInBuf, unsigned int dwDataLen, char *lpOutBuf, char *lpKey16Byte); 

// return decrypt data len
unsigned int DecryptBuf(char *lpInBuf, unsigned int dwDataLen, char *lpOutBuf, char *lpKey16Byte);

void HMMakeKey(char *lpRandom256, char *lpKey16);
