
/* Includes ------------------------------------------------------------------*/
#include "qcom_common.h"

/**
* @brief: softap ip information
*/
#define SOFTAP_IP					"192.168.1.1"
#define SOFTAP_MASK					"255.255.255.0"
#define SOFTAP_GATEWAY				"192.168.1.1"
#define SOFTAP_IP_POOL_START		"192.168.1.100"
#define SOFTAP_IP_POOL_END			"192.168.1.255"

#define DEVICE_ID 0

extern unsigned int _inet_addr(char *str);
/**
***************************************************************************
*@brief		:	start softap mode
*@param[in]	:	password
*@return	:	0			success
*				-1			failed
*@warning	:
*@see		:
***************************************************************************
*/
int qca_start_softap_mode(char *password)
{
	A_UINT8		macAddr[6];
	char		ssid[33];

	//== set ssid name
	qcom_mac_get(DEVICE_ID, macAddr);
	//print_hex(macAddr, sizeof(macAddr));

	memset(ssid, 0, sizeof(ssid));
	strcpy(ssid, "4010_");
	sprintf(ssid + strlen(ssid), "%02x%02x%02x%02x", macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
	//sprintf(ssid, "%02x%02x%02x%02x", macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
	A_PRINTF("softap ssid: %s\n", ssid);

	//== set to softap mode
	qcom_op_set_mode(DEVICE_ID, QCOM_WLAN_DEV_MODE_AP);

	//== set encrypt mode and password
	if ((password != NULL) && strlen(password))
	{
		qcom_sec_set_auth_mode(DEVICE_ID, WLAN_AUTH_WPA2_PSK);
		qcom_sec_set_encrypt_mode(DEVICE_ID, WLAN_CRYPT_AES_CRYPT);
		qcom_sec_set_passphrase(DEVICE_ID, password);
	}
	else
	{
		// open/wep mode
		qcom_sec_set_wepkey(DEVICE_ID, 1, NULL);
		qcom_sec_set_wepkey_index(DEVICE_ID, 1);
		qcom_sec_set_wep_mode(DEVICE_ID, 1);
	}

	qcom_ip_address_set(DEVICE_ID, _inet_addr(SOFTAP_IP), _inet_addr(SOFTAP_MASK), _inet_addr(SOFTAP_GATEWAY));

	// hidden ap
	//qcom_ap_hidden_mode_enable(DEVICE_ID, 1);
	//	qcom_set_country_code(DEVICE_ID, "CN");
	//	qcom_dhcpc_enable(DEVICE_ID, 0);
	qcom_set_channel(DEVICE_ID, 1);

	qcom_set_ssid(DEVICE_ID, ssid);
	qcom_commit(DEVICE_ID);

	//qcom_ap_set_max_station_number(DEVICE_ID, 10);

	// set ip address pool
 	qcom_dhcps_set_pool(DEVICE_ID, _inet_addr(SOFTAP_IP_POOL_START), _inet_addr(SOFTAP_IP_POOL_END), 259200);	// 租赁时间3天
 	qcom_dhcps_enable(DEVICE_ID, 1);

	return 0;
}

/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/
