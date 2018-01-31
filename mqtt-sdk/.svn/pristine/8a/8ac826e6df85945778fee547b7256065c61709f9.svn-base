/** 
* @file 	qca_gpio_test.h	
* @brief	
* @author	
* @version	V0.1.0
* @date		2016.3.15
* @details	
* @copyright 
* @note	Add the code below to you project to test
* extern void test_qca_http_client(void);
* test_qca_http_client();
* or, you maybe create a thread to run qca_http_client_test()
*/

#include "qcom_utils.h"
#include "string.h"
#include "qca_http_client.h"

char buf[8];

/**
* @fn void get_cb(unsigned char *buf, unsigned int len)
* @brief callback
* @param 
* @return 
*/
void get_cb(unsigned char *buf, unsigned int len)
{
    unsigned int i;

	printf("<Test>get cb.\n");
    for(i = 0; i < len; i++){
        printf("%c", buf[i]);
    }
}

/**
* @fn void post_cb(unsigned char **send_buf, unsigned int *buf_len)
* @brief post callback
* @param **send_buf send buf
* @param *buf_len buf length
* @return none
*/
void post_cb(unsigned char **send_buf, unsigned int *buf_len)
{
    // Fill buffer
    strcpy(buf, "{asdfas}");

    // Mark buffer length
    *buf_len = strlen("{asdfas}");

    // Pointer to send buffer
    *send_buf = (unsigned char *)buf;
}

/**
* @fn void test_qca_http_client(void)
* @brief http client test
* @param 
* @return 
*/
void qca_http_client_test(void)
{
    int ret = 0;
    http_client_header_t header = {
        "application/json",
        KEEP_ALIVE,
    };

    ret = http_client_startup((unsigned char *)"http://httpbin.org", 80);
    if(ret == 0)
	{
        printf("Http client start success\r\n");

        ret = http_client_get("/get", NULL, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client GET success\r\n");
        }
		else
		{
            printf("Http client GET failed\r\n");
        }

        ret = http_client_get("/get", &header, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client GET success\r\n");
        }
		else
		{
            printf("Http client GET failed\r\n");
        }

        ret = http_client_get("/get1", &header, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client GET success\r\n");
        }
		else
		{
            printf("Http client GET failed(%d)\r\n", ret);
        }

        ret = http_client_post("/post", 64, NULL, post_cb, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client POST success\r\n");
        }else
		{
            printf("Http client POST failed\r\n");
        }

        ret = http_client_delete("/get", &header, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client DELETE success\r\n");
        }
		else
		{
            printf("Http client DELETE failed(%d)\r\n", ret);
        }

        ret = http_client_put("/get", 64, NULL, post_cb, get_cb);
        if(ret == 0)
		{
            printf("\r\nHttp client PUT success\r\n");
        }
		else
		{
            printf("Http client PUT failed(%d)\r\n", ret);
        }

        http_client_stop();
    }
	else
	{
        printf("Http client start failed\r\n");
    }
}
