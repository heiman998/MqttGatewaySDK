#ifndef _QCA_PARAM_H_ATCMD_
#define _QCA_PARAM_H_ATCMD_
/**
* @file 	
* @brief
* @author
* @version	V0.1.0
* @date		2016.9.20
* @details
* @copyright
* @note
*/

#include "qcom_uart.h"


#define  TEST_SSID			"Test"
#define	 TEST_PASSWORD		"test12345678"

#define SAVE_PARAM_FLAG		0x55AA55AA

/**
* @brief: socket addr
*/
typedef struct{
	unsigned int ip;
	unsigned int port;
	unsigned char ip6_str[48];
}tsocket_addr_type;

/** 
* @struct t_save_param_type
* @brief param type
*/
typedef struct  
{
	int					flag;			// SAVE_PARAM_FLAG
	char				sta_ssid[33];
	char				sta_password[65];
	char				sta_auth;
	char				sta_cryte;
	char				sta_mode;
	char				sta_respond;
	char				sap_ssid[33];
	char				sap_password[65];
	char 				sap_channel;
	char				work_mode;
	char				uid[32];        //user id
	char				remotesw;
	char 				savepower;
	qcom_uart_para		uart_para;
	tsocket_addr_type	tcp_client;
	tsocket_addr_type	tcp_server;
	tsocket_addr_type	udp_server;
// 	tsocket_addr_type	udp_client;
}t_save_param_type;

extern t_save_param_type	t_save_param;

/**
* @fn 
* @brief 
* @param 
* @return 
*/
void qca_save_flash(t_save_param_type *param);

/**
* @fn void qca_flash_param_init(void)
* @brief check flash param
* @param none
* @return none
*/
void qca_flash_param_init(void);

/**
* @fn void qca_factory_setting(void)
* @brief 
* @param 
* @return 
*/
void qca_factory_setting(void);

#endif
/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/
