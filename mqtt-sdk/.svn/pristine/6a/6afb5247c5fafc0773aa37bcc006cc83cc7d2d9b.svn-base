/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __QCOM_UART_H__
#define __QCOM_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _uart_para
{
    int BaudRate; //The baud rate
    char number; //The number of data bits
    char parity; //The parity(0:?,1:?,2:?,default 0)
    char StopBits; //The number of stop bits
    char FlowControl; //support flow control is 1
} qcom_uart_para;

typedef enum
{
    UART_WAKEUP_DISABLE = 0x0,
    UART0_RX_WAKEUP_ENABLE = 0x00000001,
    UART1_RX_WAKEUP_ENABLE = 0x00000002,
    UART1_RTS_WAKEUP_ENABLE = 0x00000004,
    UART2_RX_WAKEUP_ENABLE = 0x00000008,
    UART2_RTS_WAKEUP_ENABLE = 0x00000010
}UART_WAKEUP_VALUE;


A_UINT32 qcom_uart_init(void);

/**
 * This function is used to init single UART.
 *
 * @param name	        UART0/UART1/UART2
 *
 * @return 			0: Success  else: Error
 */
A_INT32 qcom_single_uart_init(A_CHAR * name);

/**
 * This function is used to get the number of UART.
 *
 * @param uartname	available UART ports, like ¡°UART0/UART1/UART2
 *
 * @return 			number of UART ports
 */
int qca_get_uart_num(char* uartname);
/**
 * This function is used to get paramer of UART.
 *
 * @param name		UART port name, like'UART0'or'UART1' or 'UART2'
 * @param uart_para	pointer to UART configuration.
 * @return 			0 on success. else on error.
 */
A_STATUS qcom_get_uart_config(A_CHAR * name, qcom_uart_para* uartpara);

/**
 * This function is used to set paramer of UART.
 *
 * @param name		UART port name, like'UART0' or'UART1' or 'UART2'
 * @param uart_para	UART parameters, like baud-rate
 *
 * @return 			0 on success. else on error.
 */
A_STATUS  qcom_set_uart_config(A_CHAR *name, qcom_uart_para* uartpara);

/**
 * This function is used to open UART and get a file descriptor(fd).
 *
 * @param name		"UART1" or "UART0" or 'UART2'
 *
 * @return 			number of UARTs.
 */
A_INT32 qcom_uart_open(A_CHAR* name);

/**
 * This function is used to close UART.
 *
 * @param fd			file descriptor returned from Uart_Open.
 *
 * @return 			0 on success. else on error
 */
A_INT32 qcom_uart_close(A_INT32 fd);
/**
 * This function is used to read data from UART
 *
 * @param fd			file descriptor returned from Uart_Open.
 * @param buff		address of the buff, the buffer should big enough.
 * @param len			Input length of the buff, output length of bytes has been read.
 *
 * @return 			bytes remain in the RX buffer.
 */
A_INT32 qcom_uart_read(A_INT32 fd, A_CHAR * buff, A_UINT32* len);

/**
 * This function is used to write data of UART.
 *
 * @param fd			file descriptor returned from Uart_Open.
 * @param buff		address of the buff, the buffer should big enough.
 * @param len			Input length of the buff, output length of bytes has been written.
 *
 * @return 			bytes remain in the TX buffer.
 */
A_INT32 qcom_uart_write(A_INT32 fd, A_CHAR* buff, A_UINT32* len);

/**
 * This function is used to UART directly write without using DSR .
 *
 * @param fd			file descriptor returned from Uart_Open.
 * @param buff		address of the buff, the buffer should big enough.
 * @param len			Input length of the buff, output length of bytes has been written.
 *
 */
void qcom_uart_write_direct(A_INT32 fd, A_CHAR* buff, A_UINT32* len);

/**
 * This function is used to set UART rx pin.
 *
 * @param pin0		rx pin for UART0.
 * @param pin1		rx pin for UART1.
 *
 */
void qcom_uart_rx_pin_set(int pin0, int pin1);
/**
 * This function is used to set UART tx pin.
 *
 * @param pin0		tx pin for UART0.
 * @param pin1		tx pin for UART1.
 *
 */
void qcom_uart_tx_pin_set(int pin0, int pin1);

/**
 * This function is used to config UART wakeup function.
 *
 * @param name		"UART0" or "UART1" or "UART2" 
 * @param mode		0: rx and 1: rts
 * @param enable		0: disable and otherwise: enable
 *
 */
void qcom_uart_wakeup_config(A_CHAR *name, A_UINT8 mode, A_UINT8 enable);

/**
 * This function is used to set UART initial tx/rx buffer.
 *
 * @param name		"UART0" or "UART1" or "UART2" 
 * @param tx_size		tx buffer size.
 * @param rx_size		rx buffer size.
 * @return 			0 on success. else on error
 *
 */
A_INT32 qcom_uart_set_buffer_size(A_CHAR * name, A_UINT32 tx_size, A_UINT32 rx_size);

#ifdef __cplusplus
}
#endif

#endif

