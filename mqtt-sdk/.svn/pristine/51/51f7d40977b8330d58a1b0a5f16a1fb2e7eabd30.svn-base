/** 
* @file 	qca_ltlink.c
* @brief	
* @author	
* @version	V0.1.0
* @date		2016.8.15
* @details	
* @copyright 
* @note		
*/
#include "ltm3210.h"
// #include "qcom_system.h"
// #include "base.h"
// #include "qcom_utils.h"
// #include "qcom_misc.h"
// #include "qcom_wlan.h"
// #include "qcom_misc.h"
#include "qca_json.h"
#include "qca_ltlink.h"
#include "qca_sniffer.h"
#include "qca_smart_config.h"

/**
* @brief: device id
*/
#ifndef DEVICE_ID
#define DEVICE_ID	0
#endif


void ltlink_json(char *pdata, char *mac)
{
	json_t			*root;
	char			*pjson;

	root =  json_create_object();
	json_add_item_to_object(root, "t", json_create_string("configok"));
	json_add_item_to_object(root, "r", json_create_number(200));
	json_add_item_to_object(root, "mac", json_create_string(mac));
	pjson = json_encode(root);

	strcpy(pdata, pjson);
	json_delete(root);
	json_free(pjson);
}

/**
* @fn void ltlink_success_respond(void)
* @brief 
* @param 
* @return 
*/
void ltlink_success_respond(void)
{
	char *pData = NULL;
	char *pMac = NULL;
	char *pMacStr = NULL;

	pData = mem_alloc(64);
	pMac = mem_alloc(8);
	pMacStr = mem_alloc(16);

	memset(pData, 0, 64);
	memset(pMac, 0, 8);

	qcom_mac_get(DEVICE_ID, (unsigned char *)pMac);	

	sprintf(pMacStr, "%02x%02x%02x%02x%02x%02x", 
		pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]);

	ltlink_json(pData, pMacStr);

	qca_ltlink_response(8001, pData, strlen(pData), 10, 50);

	mem_free(pData);
	mem_free(pMac);
	mem_free(pMacStr);
}


/**
* @fn qca_ltlink_success_cb(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt)
* @brief ltlink success callback function
* @param ssid		AP's ssid
* @param password	AP's password
* @param respond    link response
* @param auth		refer to WLAN_AUTH_MODE
* @param crypt		refer to WLAN_CRYPT_TYPE
*/
void qca_ltlink_success_cb(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt)
{
	t_save_param_type	*pParam = &t_save_param;
	char				*p;

	printf("------------Ltink success----------------\n");
	printf("ssid: %s\n", ssid);
	printf("password: %s\n", password);


	p = strchr(password, '+');

	if (p != NULL)
	{
		*p = 0x0;
	}

	strcpy(pParam->sta_ssid, ssid);
	strcpy(pParam->sta_password, password);

	pParam->work_mode = STATION_MODE;
	pParam->sta_mode = WITH_SCAN;

	qca_save_flash(pParam);

	qcom_thread_msleep(200);
	qcom_sys_reset();			// ����������AP�ɹ��󣬻ظ�response
}

/**
* @fn void user_promiscuous_cb(unsigned char *buf, int length)
* @brief register promiscuous callback
* @param buf Data received in promiscuous mode
* @param length the length of data
* @return none
*/
void user_promiscuous_cb(unsigned char *buf, int length)
{
	qca_ltlink_promiscuous_cb(buf, length);
}

/**
* @fn void qca_ltlink_mode(void)
* @brief start ltlink mode
* @param 
* @return 
*/
void qca_ltlink_mode(void)
{
	int channel;
	int ret;

	printf("wk lib start...\n");

	//== register callback function
	qcom_set_promiscuous_rx_cb(user_promiscuous_cb);

	//== start ltlink
	ret = qca_ltlink_start(qca_ltlink_success_cb);
	if(ret < 0)
	{
		return;
	}

	while(1)
	{
		qcom_thread_msleep(100);

		//== Check if the channel has been locked 
		if (qca_ltlink_is_lock_channel() == 1)
		{
			continue;
		}

		channel = qca_ltlink_get_next_valid_channel();
		if (channel != 0)
		{
			qcom_set_channel(DEVICE_ID, channel);
		}
	}
}


/**
* @fn void sniffer_success_cb(char *ssid, char *password, unsigned char response, tSuccessMode_Type tmode)
* @brief sniffer success callback
* @param *ssid recive ssid
* @param *password recive password
* @param response recive response
* @param mode smartlink mode
* @return none
*/
void sniffer_success_cb(char *ssid, char *password, unsigned char response, tSuccessMode_Type tmode)
{
	t_save_param_type	*pParam = &t_save_param;

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

	strcpy(pParam->sta_ssid, ssid);
	strcpy(pParam->sta_password, password);
	pParam->sta_respond = response;
	pParam->work_mode = STATION_MODE;
	pParam->sta_mode = WITH_SCAN;

	qca_save_flash(pParam);

	qcom_thread_msleep(200);
	qcom_sys_reset();	

}

/**
* @fn void qca_start_smartlink_mode(int mode)
* @brief start sniffer mode
* @param 
* @return 
*/
void qca_start_smartlink_mode(int mode)
{

	switch (mode)
	{
	case AIRKISS_MODE:
		qca_sniffer_start(SCAN_CHANNEL_MODE, sniffer_success_cb);
		qca_set_sniffer_en(EN_AIRKISS);
		break;
	case SNIFFER_MODE:  
		qca_sniffer_start(SCAN_CHANNEL_MODE, sniffer_success_cb);
		qca_set_sniffer_en(EN_SNIFFER);
		break;
	case CONFIG_MODE:   
		qca_sniffer_start(SCAN_CHANNEL_MODE, sniffer_success_cb);
		qca_set_sniffer_en(EN_ALL);
		break;
	case LTLINK_MODE:
		qca_ltlink_mode();
		break;
	default:

		break;
	}
}








