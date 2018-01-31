/** 
* @file 	qca_at_command.h	
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
#include "string.h"
#include "qca_uart.h"

char qca_at_send(data_channel_type ch, unsigned char *buf, int length);

extern char AT_buf[512];
#define AT_PRINTF(ch, fmt,args...)						\
	do {                                            \
	memset(AT_buf, 0, sizeof(AT_buf));				\
	sprintf(AT_buf, fmt, ##args);					\
	qca_at_send(ch, (unsigned char *)AT_buf, strlen(AT_buf));		\
	qcom_thread_msleep(20);							\
	} while (0);

/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/
