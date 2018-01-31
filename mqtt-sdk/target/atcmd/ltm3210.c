/** 
* @file 	ltm3210.c	
* @brief	
* @author	
* @version	V0.1.0
* @date		2017.3.01
* @details	
* @copyright 
* @note		
*/


/* Includes ----------------------------------------------------------------*/

#include "ltm3210.h"
#include "qca_uart_handle.h"

#include "qca_smart_config.h"
#include "qca_at_command.h"

#include "user_version.h"

tGlobal_param_type tGlobal_param;

static unsigned int ip;
static unsigned int mask;
static unsigned int gateway;


/**
* @fn void qca_socket_init(void)
* @brief start TCP Client/TCP Server/UDP server
* @param none
* @return none
*/
void qca_socket_init(void)
{
	qca_tcp_client();
	qca_tcp_server();
	qca_udp_server();
}

/**
* @fn void LTM3210_run(void)
* @brief 
* @param 
* @return 
*/
void LTM3210_run(void)
{
	t_save_param_type	*pParam = &t_save_param;
	int		ret;

	// load flash param
	qca_flash_param_init();

	// uart0 init and create UART data handle task
	qca_start_uart_task();

	AT_PRINTF(UART_Channel, "NT:0001,V"VERSION_FMT"\r\n", VERSION(USER_VERSION_SOFTWARE));

	// GPIO indicator lamp
	//qca_gpio_test();
	
	// work mode
	switch(pParam->work_mode)
	{
	case SOFTAP_MODE:
		AT_PRINTF(UART_Channel, "NT:0017,%s,%s,%d\r\n", pParam->sap_ssid, pParam->sap_password, pParam->sap_channel);
		// start softAP mode
		qca_start_softap_mode(pParam->sap_ssid, pParam->sap_password, pParam->sap_channel);

		AT_PRINTF(UART_Channel, "NT:0009,%s,%s,%s\r\n", SOFTAP_IP, SOFTAP_MASK, SOFTAP_GATEWAY);

		// create UDP/TCP Server/TCP Client task
		qca_socket_init();

		break;
	case STATION_MODE:
		AT_PRINTF(UART_Channel, "NT:0002\r\n");
		while (1)
		{
			tGlobal_param.connect_state = CONNECTTING_STATE;
			if (WITHOUT_SCAN == pParam->sta_mode)
			{
				qca_ap_set_auth_encrypt(pParam->sta_auth, pParam->sta_cryte);
				ret = qca_start_station_mode(WITHOUT_SCAN, pParam->sta_ssid, pParam->sta_password, 20000);
			}
			else
			{
				ret = qca_start_station_mode(WITH_SCAN, pParam->sta_ssid, pParam->sta_password, 20000);
			}
			
			if (ret == CONNECT_OK)
			{
				//ltlink_success_respond();
//				qca_airkiss(AIRKISS_V2, "gh_a1add65ebcdf", "4kCdtL0og", pParam->sta_respond);
				qcom_ip_address_get(DEVICE_ID, &ip, &mask, &gateway);

				AT_PRINTF(UART_Channel, "NT:0009" "," IPV4_FMT "," IPV4_FMT "," IPV4_FMT "\r\n", IPV4_ADDR(ip), IPV4_ADDR(mask), IPV4_ADDR(gateway));

				tGlobal_param.connect_state = CONNECTED_STATE;
				qca_socket_init();

				if(pParam->savepower == 1){
					qcom_set_power_mode(DEVICE_ID, REC_POWER);
            		cmnos_power_syssleep_control(1);
				}
				break;
			}
			else if (ret == SSID_ERROR)
			{
				break;
			}
			else if (ret == PASSWORD_ERROR)
			{
				qcom_sys_reset();
			}
			else
			{
				break;
			}
		}

		break;
	case SNIFFER_MODE:
		AT_PRINTF(UART_Channel, "NT:0014\r\n");
		tGlobal_param.connect_state = CONFIG_STATE;
		qca_start_smartlink_mode(SNIFFER_MODE);
		break;
	case AIRKISS_MODE:
		AT_PRINTF(UART_Channel, "NT:0004\r\n");
		tGlobal_param.connect_state = CONFIG_STATE;
		qca_start_smartlink_mode(AIRKISS_MODE);
		break;
	case LTLINK_MODE:
		AT_PRINTF(UART_Channel, "NT:0018\r\n");
		tGlobal_param.connect_state = CONFIG_STATE;
		qca_start_smartlink_mode(LTLINK_MODE);
		break;
	default:
		break;
	}

	while (1)
	{
//		printf("UART1 test...\n");
		qcom_thread_msleep(1000);
	}
}



/**
* @fn void print_hex(unsigned char *buf, int len)
* @brief printf hex data
* @param buf data buf
* @param len data size
* @return none
*/
void print_hex(unsigned char *buf, int len)
{
	int			i;

	for (i = 0; i < len; i++)
	{
		A_PRINTF("%02x ", *buf++);
	}
}


/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/

