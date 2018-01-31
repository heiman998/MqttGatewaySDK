/*
    qca_sniffer test case

    Add the code below to you project to test

>    extern void test_sniffer(void);
>    test_sniffer();
 */
#include "sample_main.h"
#include "qca_sniffer.h"

void sniffer_success_cb(char *ssid, char *password, unsigned char response, tSuccessMode_Type tmode);

void test_sniffer(void)
{
    printf("sniffer testing......\r\n");

    qca_sniffer_start(SCAN_CHANNEL_MODE, sniffer_success_cb);

    qca_set_sniffer_en(EN_SNIFFER);

	while(1)
	{
		qcom_thread_msleep(1000);
	}
}

void sniffer_success_cb(char *ssid, char *password, unsigned char response, tSuccessMode_Type tmode)
{
	switch (tmode)
	{
	    case AIRKISS:
		    printf("airkiss recv success.\n");
		    break;

	    case SNIFFER:
		    printf("sniffer recv success.\n");
		    break;

	    default:break;
	}

	printf("ssid:%s\n", ssid);
	printf("password:%s\n", password);

	qcom_thread_msleep(200);	
	
	printf("sniffer test done.\r\n");
}
