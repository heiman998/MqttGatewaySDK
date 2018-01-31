
#ifndef _QCA_UART_H_
#define _QCA_UART_H_


/** 
* @file 	qca_uart.h	
* @brief	Uart driver.
* @author	
* @version	V0.1.0
* @date		2016.3.15
* @details	
* @copyright 
* @note		
*/

#include "base.h"
#include "qcom/qcom_uart.h"


/**
* @fn uart_resolve_func
* @brief uart packet resolve function
* @param *buf receive cache buffer
* @param buf_len receive cache buffer size
* @param *data_start receive data start address in cache buffer
* @param *data_end receive data end address in cache buffer
* @param **packet_start packet start address
* @param **packet_end packet end address
* @return 0£ºsuccessful, -1£ºfailed
*/
typedef int(*uart_resolve_func)(unsigned char **packet_start, unsigned char **packet_end, \
	unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end);

/**
* @fn int qca_uart_get_buf_full_status(void)
* @brief check buf if full
* @param none
* @return		1		full
*				0		not full
*/
int qca_uart_get_buf_full_status(void);

/**
* @fn unsigned char* qca_uart_get_next_addr(unsigned char *addr, int step)
* @brief get next address
* @param addr the base address in uart receive cache buffer
* @param step the number of step from the base address
* @return new address
*/
unsigned char* qca_uart_get_next_addr(unsigned char *addr, int step);


/**
* @fn int qca_uart_task_start(qcom_uart_para *param, int recv_cache_buf_size, uart_resolve_func recv_cb)
* @brief create uart task
* @param *param uart parameters
* @param recv_cache_buf_size uart receive cache buffer size
* @param recv_cb resolve uart receive data function
* @return 0£ºsuccessful, -1£ºfailed
*/
int qca_uart_task_start(qcom_uart_para *param, int recv_cache_buf_size, uart_resolve_func recv_cb);

/**
* @fn int qca_uart_send(unsigned char *data, int len)
* @brief send uart data
* @param *data user data
* @param len data size
* @return 
*/
int qca_uart_send(unsigned char *data, int len);

/**
* @fn int qca_uart_get_one_recv_packet(unsigned char *packet, int *packet_len)
* @brief get packet from uart cache
* @param *packet receive buffer start address
* @param *packet_len receive buffer length
* @return 0£ºsuccessful, -1£ºfailed
*/
int qca_uart_get_one_recv_packet(unsigned char *packet, int *packet_len);

/**
* @fn int qca_uart_stop(void)
* @brief stop uart
* @param none
* @return 0£ºsuccessful, -1£ºfailed
*/
int qca_uart_stop(void);



#endif

