#ifndef _QCA_BASE64_H
#define _QCA_BASE64_H


int base64_encode(unsigned char *in, int length, char *out);
int base64_decode(char* in, unsigned char *out);


#endif
