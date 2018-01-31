/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File: uart_serphw_api.h
//
// @Abstract: uart serial port hardware module
//
// @Notes:
//
//

#ifndef UART_SERPHW_API_H_
#define UART_SERPHW_API_H_

/* This module exports only init and start entry points, the actual interface to the uart hardware
 * is through the serial port apis at a higher layer.
 * This module provide a hardware implementation for the HCI UART hardware block */

    /* pins defined so far for the UART hardware */
typedef enum {
    UART_HW_RX = 0,
    UART_HW_CTS = 1,
    UART_HW_TX,
    UART_HW_RTS,
    UART_HW_PIN_MAX    
} UART_HW_PINS;

typedef struct {
    int     GpioPin;        /* GPIO pin corresponding to this UART pin
                              if set to 0, use defaults, if set to -1,
                              this pins is not used and does not need to be configured */
    A_UINT8 ConfigValue;    /* Required config value for the pin */
    A_UINT8 ConfigAsGpio;   /* Config value used to revert pins as GPIO for manual control */
} UART_HW_PIN_CONFIG;

typedef struct {
    UART_HW_PIN_CONFIG PinList[UART_HW_PIN_MAX]; /* pin setting overrides */
    int                RxBufferSize;    /* size of additional RX buffering, if 0, defaults are used */
    int                TxBufferSize;    /* size of additional TX buffering, if 0, defaults are used */
    A_CHAR             *pPortName;      /* port name override */
} UART_HW_CONFIG;
 
struct uart_serphw_apis {
        /* initialize instance */
    void     *(*_Init)(int InstanceId);
        /* start an instance */
    void     (*_Start)(void *pInstance, UART_HW_CONFIG *pConfig); 
    void     *pReserved;                    
};

void UARTSERP_HW_module_install(struct uart_serphw_apis *pAPIs);
void uart_pre_sleep_handler(void);
void uart_post_awake_handler(void);
void uart_wakeup_test(void);
void uart_set_wakeup_enable(int pin, A_BOOL enable);
int uart_check_connection_status(int uart_number, int delay_flag);

    /* default logical port names for each instance */
#define UART_PORT0_NAME   "UART0"
#define UART_PORT1_NAME   "UART1"
#define UART_PORT2_NAME   "UART2"

typedef enum {  
    UART_SERP_UART0_INSTANCE_ID = 0,  /* DBG_UART, UART0 */
    UART_SERP_UART1_INSTANCE_ID = 1,  /* UART1 */
    UART_SERP_UART2_INSTANCE_ID,      /* UART2 */
    UART_SERP_MAX_INSTANCES
} UART_SERP_INSTANCE_IDS;
 
#endif /*UART_SERPHW_API_H_*/
