/*
    qca_i2s test case

    Add the code below to you project to test

>    extern void test_i2s(void);
>    test_i2s();
 */
#include "sample_main.h"
#include "qca_i2s.h"

#define I2S_BUFFER_SIZE_1 (160 * 2 * 2)
#define I2S_BUFFER_SIZE_2 (320 * 2 * 2)
#define I2S_BUFFER_CNT 64

unsigned char i2s_send_finish = 0;
unsigned char i2s_recv_finish = 0;

unsigned char recv_buf[I2S_BUFFER_SIZE_2] = {0};

qca_i2s_config_t config;
qca_i2s_t port;

//void i2s_callback_send(void *param);
//void i2s_callback_recv(void *param);

void test_i2s(void)
{
    int ret = 0;
    int recv_len = 0;

    i2s_send_finish = 0;

    config.freq = QCA_I2S_FREQ_16K;
    config.word_size = QCA_I2S_WORD_SIZE_16;
    config.buf_size = I2S_BUFFER_SIZE_1;
    config.dma_buf_count = I2S_BUFFER_CNT;
    config.mode = QCA_I2S_MODE_MASTER;
    config.cb = NULL;

    printf("i2s function testing......\r\n");
    
    ret = qca_i2s_init(QCA_I2S_1, &config);
    if(ret != 0){
        printf("i2s bus 1 init fail.\r\n");
        return;
    }

    config.buf_size = I2S_BUFFER_SIZE_2;
    config.mode = QCA_I2S_MODE_SLAVE;
    config.cb = NULL;

    ret = qca_i2s_init(QCA_I2S_0, &config);
    if(ret != 0){
        printf("i2s bus 0 init fail.\r\n");
        return;
    }

    ret = qca_i2s_recv_start(QCA_I2S_0);
    if(ret != 0){
        printf("i2s recv start fail.\r\n");
        return;
    }
    printf("i2s recv start......\r\n");
    
    do{
        qcom_thread_msleep(2);
        ret = qca_i2s_recv(QCA_I2S_0, recv_buf, &recv_len);
    }
    while(ret == 0);
    printf("i2s recv finish.\r\n");
    //qca_i2s_recv(QCA_I2S_0, recv_buf, &recv_len);
    //qcom_thread_msleep(3000);

    ret = qca_i2s_recv_stop(QCA_I2S_0);
    if(ret != 0){
        printf("i2s recv stop fail.\r\n");
        return;
    }
    printf("i2s recv stop.\r\n");

    qcom_thread_msleep(1000);

    printf("i2s send...\r\n");
    
    //qca_i2s_send(QCA_I2S_1, recv_buf, recv_len);
    //qcom_thread_msleep(4000);

    do
    {
        qcom_thread_msleep(2);
        ret = qca_i2s_send(QCA_I2S_1, recv_buf, recv_len);
    }
    while(ret != 0);

    printf("i2s send finish.\r\n");

    if((i2s_send_finish == 1) && (i2s_recv_finish == 1)){
        printf("i2s callback function has been executed.\r\n");
    }
    else
    {
        if(i2s_send_finish != 1)
        {
            printf("i2s send callback function has not been executed.\r\n");
        }
        if(i2s_recv_finish != 1)
        {
            printf("i2s recv callback function has not been executed.\r\n");
        }
    }

    printf("i2s function test done.\r\n");
}

// void i2s_callback_send(void *param)
// {
//     i2s_send_finish = 1;
// }

// void i2s_callback_recv(void *param)
// {
//     i2s_recv_finish = 1;
// }
