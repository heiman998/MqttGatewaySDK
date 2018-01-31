/*
    qca_ssl_client test case

    Add the code below to you project to test

>    extern void test_softap(void);
>    test_softap();
 */
#include "sample_main.h"

#include "qca_softap.h"

#define AP_PASSWD "12345678"

void test_softap(void)
{
	int ret = 0;

	ret = qca_start_softap_mode(AP_PASSWD);
	if(ret == 0)
		PRT_T_SOFTAP("Start softAP mode succeed\n");
	else
		PRT_T_SOFTAP("Start softAP mode failed\n");
}
