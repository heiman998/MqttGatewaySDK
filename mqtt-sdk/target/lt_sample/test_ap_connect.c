/*
    qca_ssl_client test case

    Add the code below to you project to test

>    extern void test_ap_connect(void);
>    test_ap_connect();
 */

#include "sample_main.h"
#include "qca_ap_connect.h"

#define START_STATION_WITH_SCAN

#define WIFI_TIMEOUT 30000

#define WIFI_SSID "test_gree"
#define WIFI_PASSWD "test12345678"


void test_ap_connect(void)
{
	int ret = 0;

#ifdef START_STATION_WITH_SCAN
	ret = qca_start_station_mode(WITH_SCAN, WIFI_SSID, WIFI_PASSWD, WIFI_TIMEOUT);
#else
	/* Set auth and encrypt first if start station mode without scan */
	qca_ap_set_auth_encrypt(WLAN_AUTH_WPA2_PSK, WLAN_CRYPT_AES_CRYPT);
	ret = qca_start_station_mode(WITHOUT_SCAN, WIFI_SSID, WIFI_PASSWD, WIFI_TIMEOUT);
#endif
	if(ret == 0){
		PRT_T_AP_CONNECT("Start station succeed\n");
	}else{
		PRT_T_AP_CONNECT("Start station failed\n");
	}
}
