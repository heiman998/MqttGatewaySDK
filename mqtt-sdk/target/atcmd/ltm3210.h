/** 
* @file 	ltm3210.h	
* @brief	
* @author	
* @version	V0.1.0
* @date		2017.3.01
* @details	
* @copyright 
* @note		
*/

#ifndef _LTM3210_H_ATCMD_
#define _LTM3210_H_ATCMD_

#include "qcom/qcom_common.h"
#include "qcom/qcom_internal.h"
#include "qcom/qcom_gpio.h"
#include "qcom/socket.h"
#include "qcom/select_api.h"
#include "qcom/socket_api.h"
#include "qcom_nvram.h"


#include "qca_task_manage.h"
#include "qca_uart.h"
#include "qca_gpio.h"
#include "qca_flash_param.h"
#include "qca_psram_param.h"
#include "qca_string_ext.h"
#include "qca_ap_connect.h"
// #include "qca_calendar.h"
#include "qca_param.h"
// #include "qca_sniffer.h"
#include "qca_softap.h"
#include "qca_tcp_client.h"
#include "qca_tcp_server.h"
#include "qca_udp_server.h"
#include "qca_uart_handle.h"
// #include "qca_sniffer_config.h"

/**
* @brief: device id
*/
#ifndef DEVICE_ID
#define DEVICE_ID	0
#endif

#ifndef NULL
#define NULL            ((void *)0)
#endif


#define PARAM_START_ADDR	0x00FF000

/**
* @brief: working mode
*/
typedef enum{
	NONE_MODE	= -1,
	SOFTAP_MODE = 0,
	STATION_MODE,
	SNIFFER_MODE,
	AIRKISS_MODE,
	CONFIG_MODE,
	LTLINK_MODE,
}ework_mode_type;

typedef enum
{
	IDEL_STATE = 0,
	CONFIG_STATE,
	CONNECTTING_STATE,
	CONNECTED_STATE,
}ework_state_type;

typedef enum{
	UART_Channel = 0,
	TCP_Server_Channel,
	TCP_Client_Channel,
	UDP_Server_Channel,
}data_channel_type;

typedef enum{
	AT_COMMAND = 0,
	NONE_AT_COMMAND,
	AT_COMMAND_ERROR,
}command_type;



typedef struct {
	unsigned char connect_state;
	unsigned char remote_state;


}tGlobal_param_type;

extern tGlobal_param_type tGlobal_param;

/**
* @brief: correction time
*/
#define qca_thread_msleep(time)		qcom_thread_msleep(time*1024/1000)
#define qca_time_ms()				(time_ms()*1000/1024)


/**
* @brief: printf enable
*/

#define PRINT_HEX					print_hex

#define IPV4_ADDR(addr) (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr)& 0xFF
#define IPV4_FMT "%d.%d.%d.%d"

#define VERSION(ver) (ver) >> 12 & 0xF, (ver) >> 8 & 0xF, (ver)& 0xFF
#define VERSION_FMT "%d.%d.%02x"

/**
* @brief: check null pointer
*/
#define QCA_PTR_NULL_CHK(para)		do {if (NULL == (para)) return;} while(0)

/**
* @fn unsigned int qca_inet_addr(char *str)
* @brief inet_addr
* @param 
* @return 
*/
unsigned int qca_inet_addr(char *str);

/**
* @fn void print_hex(unsigned char *buf, int len)
* @brief printf hex data
* @param buf data buf
* @param len data size
* @return none
*/
void print_hex(unsigned char *buf, int len);

/**
* @fn void LTP1218_run(void)
* @brief 
* @param 
* @return 
*/
void LTM3210_run(void);


/**
* @fn int qca_handle_cmd(int channel, unsigned char *pdata)
* @brief resolve data
* @param 
* @return 
*/
int qca_handle_cmd(int channel, unsigned char *pdata, int len);

/**
* @fn int qca_http_ota(char *url, unsigned short port)
* @brief ota test
* @param url ota file url
* @param port http server port
* @return none
*/

int qca_http_ota(char *url, unsigned short port);
/**
* @fn void qca_gpio_test(void)
* @brief i2c test
* @param none
* @return none
*/
void qca_gpio_test(void);

#endif

