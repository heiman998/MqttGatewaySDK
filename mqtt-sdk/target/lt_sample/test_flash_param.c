/*
    qca_flash_param test case

    Add the code below to you project to test

>    extern void test_flash_param(void);
>    test_flash_param();
 */
#include "sample_main.h"
#include "qca_flash_param.h"

#define FLASHADDRESS 0x000C0100
#define BUFFERSIZE 10

unsigned char flash_test_buf[BUFFERSIZE] = "0123456789";

void test_flash_param(void)
{
    int ret;
    
    printf("flash param testing......\r\n");

    printf("flash param save : %s\r\n", flash_test_buf);
    ret = qca_save_flash_params(FLASHADDRESS, flash_test_buf, BUFFERSIZE);
    if(ret != 0){
        printf("flash param save fail.\r\n");
        return;
    }

    memset(flash_test_buf, 0, BUFFERSIZE);
    ret = qca_load_flash_params(FLASHADDRESS, flash_test_buf, BUFFERSIZE);
    if(ret != 0){
        printf("flash param load fail.\r\n");
        return;
    }
    printf("flash param load : %s\r\n", flash_test_buf);

    printf("flash param test done.\r\n");
}
