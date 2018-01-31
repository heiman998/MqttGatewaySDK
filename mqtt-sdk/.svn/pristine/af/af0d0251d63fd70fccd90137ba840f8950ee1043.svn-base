/*
    qca_ltlink test case

    Add the code below to you project to test

>    extern void test_ltlink(void);
>    test_ltlink();
 */
#include "sample_main.h"
#include "qca_ltlink.h"

#define DEVICE_ID 0

void user_promiscuous_cb(unsigned char *buf, int length);

void qca_ltlink_success_cb(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt);

void test_ltlink(void)
{
    int channel;
    int ret;
    
    printf("ltlink testing......\r\n");

	qcom_set_promiscuous_rx_cb(user_promiscuous_cb);

    ret = qca_ltlink_start(qca_ltlink_success_cb);
    if(ret != 0){
        printf("ltlink start fail.\r\n");
        return;
    }

    while(1)
	{
		qcom_thread_msleep(100);

		if (qca_ltlink_is_lock_channel() == 1){
			continue;
		}

		channel = qca_ltlink_get_next_valid_channel();
		if (channel != 0){
			qcom_set_channel(DEVICE_ID, channel);
		}
	}
}

void user_promiscuous_cb(unsigned char *buf, int length)
{
	qca_ltlink_promiscuous_cb(buf, length);
}

void qca_ltlink_success_cb(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt)
{
	printf("ltlink success.\r\n");
	printf("ssid: %s\n", ssid);
	printf("password: %s\n", password);
	printf("auth = %d, crypt = %d\n", auth, crypt);

	qcom_thread_msleep(200);	

	printf("ltlink test done.\r\n");

	qcom_sys_reset();	
}
