/*
    qca_psram_param test case

    Add the code below to you project to test

>    extern void test_param_param(void);
>    test_psram_param();
 */
#include "sample_main.h"
#include "qca_psram_param.h"

#define BUFFERSIZE 10

unsigned char psram_test_buf[BUFFERSIZE] = "0123456789";

void test_psram_param(void)
{
    int ret;
    
    printf("psram param testing......\r\n");

    printf("psram write: %s\r\n", psram_test_buf);
    ret = qca_psram_dset_write(psram_test_buf, BUFFERSIZE);
    if(ret != 0){
        if(ret == -1){
            printf("psram write fail(write).\r\n");
        }
        else{
            printf("psram write fail(create error).\r\n");
        }
        return;
    }

    memset(psram_test_buf, 0, BUFFERSIZE);
    ret = qca_psram_dset_read(psram_test_buf, BUFFERSIZE);
    if(ret < 0){
        if(ret == -1){
            printf("psram read fail(read).\r\n");
        }
        else{
            printf("psram read fail(create error).\r\n");
        }
        return;
    }
    printf("psram read: %s\r\n", psram_test_buf);

    printf("psram param test done.\r\n");
}

