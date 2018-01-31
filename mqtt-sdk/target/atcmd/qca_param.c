/** 
* @file 	qca_flash_param_test.c
* @brief	
* @author	
* @version	V0.1.0
* @date		2016.3.15
* @details	
* @copyright 
* @note		
*/

#include "qcom_common.h"
#include "ltm3210.h"
#include "qca_flash_param.h"
#include "qca_param.h"

t_save_param_type	t_save_param;

/* Printf ------------------------------------------------------------------*/

/**
* @fn 
* @brief 
* @param 
* @return 
*/
void qca_save_flash(t_save_param_type *param)
{
	param->flag = SAVE_PARAM_FLAG; 
	qca_save_flash_params(PARAM_START_ADDR, (unsigned char *)param, sizeof(t_save_param_type));
}

/**
* @fn 
* @brief 
* @param 
* @return 
*/
int qca_load_flash(t_save_param_type *param)
{
	qca_load_flash_params(PARAM_START_ADDR, (unsigned char *)param, sizeof(t_save_param_type));

	if (param->flag == SAVE_PARAM_FLAG)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
* @fn void qca_stainfo_default(t_save_param_type *param)
* @brief The default station param
* @param 
* @return 
*/
void qca_stainfo_default(t_save_param_type *param)
{
	strcpy(param->sta_ssid, TEST_SSID);
	strcpy(param->sta_password, TEST_PASSWORD);
}

void qca_sapinfo_default(t_save_param_type *param)
{
	A_UINT8		macAddr[6];

	qcom_mac_get(DEVICE_ID, macAddr);

	memset(param->sap_ssid, 0, sizeof(param->sap_ssid));
	strcpy(param->sap_ssid, "LT_");
	sprintf(param->sap_ssid + strlen(param->sap_ssid), "%02x%02x%02x%02x", macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

/**
* @fn void qca_factory_setting(void)
* @brief 
* @param 
* @return 
*/
void qca_factory_setting(void)
{
	t_save_param_type	*param = &t_save_param;

	memset(param, 0, sizeof(t_save_param_type));
	qca_sapinfo_default(param);
	//param->work_mode = STATION_MODE;
	param->work_mode = SOFTAP_MODE;
	//param->work_mode = SNIFFER_MODE;
	// param->work_mode = LTLINK_MODE;
	//param->work_mode = AIRKISS_MODE;

	param->sta_mode = WITH_SCAN;
	// strcpy(param->sta_ssid, "ioe-test");
	// strcpy(param->sta_password, "test12345678");

	param->tcp_server.port = 8848;
	param->udp_server.port = 8849;

	param->sap_channel = 1;

	param->savepower = 0;

	//= uart
	param->uart_para.BaudRate = 115200;
	param->uart_para.number = 8;
	param->uart_para.StopBits = 1;
	param->uart_para.parity = 0;
	param->uart_para.FlowControl = 0;

	qca_save_flash(param);
}

/**
* @fn void qca_param_init(void)
* @brief check flash param
* @param none
* @return none
*/
void qca_flash_param_init(void)
{
	int			ret;
	t_save_param_type	*param = &t_save_param;

	ret = qca_load_flash(param);
	if (!ret)
	{
		printf("factory setting.\n");

		qca_factory_setting();
	}
}



/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/
