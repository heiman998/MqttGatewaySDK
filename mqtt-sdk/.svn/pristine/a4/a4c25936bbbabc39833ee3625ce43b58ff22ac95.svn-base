/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __PRIV_UART_H__
#define __PRIV_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#define UART_PORT0_NAME   "UART0"
#define UART_PORT1_NAME   "UART1"

#define SER_PORT_CONFIG_RTS_CTS_FLOW  (1 << 0)
#define SER_PORT_CONFIG_PARITY_CHECK  (1 << 1)
#define SER_PORT_CONFIG_ODD_PARITY    (1 << 2)
#define SER_PORT_CONFIG_EVEN_PARITY   (0)

/* UART API */
typedef struct {
    unsigned int Baud;        /* in Hz , if set to zero the baud is set through a private IOCTL */
    unsigned char DataBits;    /* number of data bits */
    unsigned char StopBits;    /* number of stop bits */
    unsigned short ConfigFlags;    
} UART_CFG_T;

typedef enum
{
    UART_TERMINATE_MASK = 0x0,
    UART0_START_MASK = 0x01,
    UART1_START_MASK = 0x02,
    UART2_START_MASK = 0x04
}UART_START_FLAG;

/*configure uart ring buffer size*/
int uart_set_buffer_size(char *name, A_UINT32 tx_size, A_UINT32 rx_size);

/* Return number of UART */
int Uart_Enumerate(void);

/*init designated uart*/
int Uart_Init(char * name);

//for dual uart wakeup init
int Init_Multi_Uart_Wakeup(void);
int Console_Uart_Num(void);

/* Uart_Open - open UART and get a file descritor (fd) 
   name(input): UART0 or UART1
*/
int Uart_Open(char* name);

/* Set parameter of UART */
int Uart_SetPara(int fd, UART_CFG_T uart_cfg);

/* Get parameter of UART */
int Uart_GetPara(int fd, UART_CFG_T* uart_cfg);

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes has been   written.
   returns, bytes remain in the TX buffer.
*/
int Uart_Write(int fd, unsigned char* buff, int* len);

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes has been   written.
   returns, bytes remain in the TX buffer.
*/
void Uart_Write_Direct(int fd, unsigned char* buff, int* len);

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes read.
   returns, bytes remain in the RX buffer.
*/
int Uart_Read(int fd, unsigned char* buff, int* len);

/* Uart_Close -close UART with given fd */
int Uart_Close(int fd);

int Uart_GetFd(char* name);
#ifdef __cplusplus
}
#endif

#endif

