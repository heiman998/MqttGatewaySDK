/** 
* @file 	qca_softap_test.c
* @brief	
* @author	
* @version	V0.1.0
* @date		2017.3.01
* @details	
* @copyright 
* @note		
*/

/* Includes ------------------------------------------------------------------*/
#include "qcom_common.h"

#include "ltm3210.h"
/**
* @brief: device id
*/
#ifndef DEVICE_ID
#define DEVICE_ID	0
#endif

/**
* @brief: NULL
*/
#ifndef NULL
#define NULL            ((void *)0)
#endif



/**
* @fn int qca_start_softap_mode(char *ssid, char *password)
* @brief strat softap mode
* @param ssid softap ssid
* @param password softap password
* @return 
*/
int qca_start_softap_mode(char *ssid, char *password, char channel)
{
	printf("softap ssid: %s\n", ssid);
	

	//== set to softap mode
	qcom_op_set_mode(DEVICE_ID, QCOM_WLAN_DEV_MODE_AP);

	if (ssid == NULL)
	{
		return -1;
	}

	//== set encrypt mode and password
	if ((password != NULL) && strlen(password))
	{
		printf("softap password: %s\n", password);

		qcom_sec_set_auth_mode(DEVICE_ID, WLAN_AUTH_WPA2_PSK);
		qcom_sec_set_encrypt_mode(DEVICE_ID, WLAN_CRYPT_AES_CRYPT);
		qcom_sec_set_passphrase(DEVICE_ID, password);
	}
	else
	{
		printf("WEP.\n");
		// open/wep mode
		qcom_sec_set_wepkey(DEVICE_ID, 1, NULL);
		qcom_sec_set_wepkey_index(DEVICE_ID, 1);
		qcom_sec_set_wep_mode(DEVICE_ID, 1);
	}

	// set wifi module ip
	qcom_ip_address_set(DEVICE_ID, _inet_addr(SOFTAP_IP), _inet_addr(SOFTAP_MASK), _inet_addr(SOFTAP_GATEWAY));

	// hidden ap 
	//qcom_ap_hidden_mode_enable(DEVICE_ID, 1);

	//set ap channel
	qcom_set_channel(DEVICE_ID, channel);

	// create softap
	qcom_set_ssid(DEVICE_ID, ssid);
	qcom_commit(DEVICE_ID);

	//qcom_ap_set_max_station_number(DEVICE_ID, 10);

	// set ip address pool
 	qcom_dhcps_set_pool(DEVICE_ID, _inet_addr(SOFTAP_IP_POOL_START), _inet_addr(SOFTAP_IP_POOL_END), 259200);	// ×âÁÞÊ±¼ä3Ìì
 	
	// open DHCP mode
	qcom_dhcps_enable(DEVICE_ID, 1);



	return 0;
}

/***************** (C) COPYRIGHT 2017 longsys *****END OF FILE***************/
