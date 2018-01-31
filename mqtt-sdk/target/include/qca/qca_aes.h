#ifndef _QCA_AES_H_JKLASJFKLDSJFKLDS
#define _QCA_AES_H_JKLASJFKLDSJFKLDS

/**
* @fn void qca_aes_ecb_encode(unsigned char *dest, unsigned char *src, int len, unsigned char *key)
* @brief encode the src to aes ecb.
* @param dest the buffer that store encode result
* @param src the buffer that want to encode
* @param len the length of src
* @param key aes ecb key
* @return none
* @note: the len should be multiple of 16(len % 16 = 0), so you should padding with yourself
         the key must be 16bytes
*/
void qca_aes_ecb_encode(unsigned char *dest, unsigned char *src, int len, unsigned char *key);

/**
* @fn void qca_aes_ecb_decode(unsigned char *dest, unsigned char *src, int len, unsigned char *key)
* @brief decode the src to aes ecb.
* @param dest the buffer that store decode result
* @param src the buffer that want to decode
* @param len the length of src
* @param key aes ecb key
* @return none
* @note: the key must be 16bytes
*/
void qca_aes_ecb_decode(unsigned char *dest, unsigned char *src, int len, unsigned char *key);

#endif
