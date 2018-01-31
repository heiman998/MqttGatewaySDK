/*
    qca_aes test case

    Add the code below to you project to test

>    extern void test_aes(void);
>    test_aes();
 */
#include "sample_main.h"
#include "qca_aes.h"

void test_aes(void)
{
    static unsigned char src[16] = {0x01, 0x04, 0xae, 0x94, 0x31, 0x45, 0x29, 0x56
                          , 0x52, 0xae, 0x94, 0x31, 0x45, 0x29, 0x56, 0x43};

    static unsigned char dest[16];

    static unsigned char tmp[16];
    int i;

    memset(dest, 0, 16);

    printf("Test aes encode\n");

    qca_aes_ecb_encode(dest, src, 16, "1234567890abcdef");

    printf("encode data:\n");
    for(i = 0; i < 16; i++){
        printf("0x%02x, ", dest[i]);
    }
    printf("\n");

    printf("Test aes decode\n");

    qca_aes_ecb_decode(tmp, dest, 16, "1234567890abcdef");

    printf("decode data:\n");
    for(i = 0; i < 16; i++){
        printf("0x%02x, ", tmp[i]);
    }
    printf("\n");

    if(memcmp(src, tmp, 16) == 0){
        printf("Aes test succeed\n");
    }
}