/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: UART port internal definitions
//
// @Notes:
//
//

#ifndef UARTSERP_INTERNAL_H_
#define UARTSERP_INTERNAL_H_

#include <serialport_api.h>
#include <serialport_ioctl.h>
#include <uart_serphw_api.h>

#define SERIAL_LAYER_RING_BUFFER_SIZE  (2*1024)

#define DEFAULT_UART_BAUD_RATE          9600
#define UART_TX_FIFO_SIZE               4
#define UART_RX_FIFO_SIZE               4
#define INT_POLL_INTERVAL_MS            3

//#define ENABLE_UART_STATS

#define SAVED_BYTE_VALID 0x8000

#ifdef ENABLE_UART_STATS

typedef struct {
    A_UINT32    RxIrqs;
    A_UINT32    RxBytes;
    A_UINT32    TxIrqs;
    A_UINT32    TxBytes;
    A_UINT32    RxDropped;
    A_UINT32    TxDropped;
    A_UINT32    RxFramingErrors;
    A_UINT32    RxParityErrors;
} UART_HW_STATS;

#define UART_STAT_INCR(p,stat,count) (p)->Stats.stat += (count)
#define UART_RESET_STATS(p)          A_MEMZERO(&(p)->Stats,sizeof(UART_HW_STATS))

#else
#define UART_STAT_INCR(p,stat,count)
#define UART_RESET_STATS(p)

#endif

#define UART_RX_BUFFER_SIZE_INTERNAL 320
#define UART_RX_BUFFER_SIZE_MARGIN   40

typedef struct {
    SER_PORT_HW_DEV       SerialDev;                  /* registered serial device structure */
    SER_PORT_INSTANCE     hPortDev;                   /* our serial port instance in SERP layer */
    UART_HW_CONFIG        HwPins;                       /* saved hardware pins for run time configuration */
    A_BOOL                PinIsInput[UART_HW_PIN_MAX];  /* pin is an input pin */
    A_UINT16              SavedByte;
    int                   InstanceId;
    A_UINT32              BaseAddress;
#ifdef ENABLE_UART_STATS
    UART_HW_STATS         Stats;
#endif
    UART_HW_PIN_CONFIG    *pDefPinConfigTable;        /* table to use for default pin config */
    A_timer_t             PollingTimer;
    A_BOOL                TimerQueued;
    A_BOOL                Polling;
    A_UINT32              PollTimeoutMS;
    gpio_interrupt_info_t RxPinInterrupt;
	A_UINT32	  		  EnableInterrupts;
	volatile A_BOOL		  RxIsr; /* UART interrupt (except Rx Full) was disabled if TRUE */
	A_BOOL				  RxWakeup; /* need to notify in DSR if TRUE */
	A_UINT32			  RxMax;
	A_UINT32			  RxIsrDiscard;
	A_UINT32              RxFullCount;
	A_UINT32			  RxDsrDiscard;
	A_UINT32			  RxOverFlow;
	A_UINT32              BufferLength;
	A_UINT8			  	  Buffer[UART_RX_BUFFER_SIZE_INTERNAL];
} UART_DEVICE;

#define ALL_UART_INTS  0xFFFFFFFF

#if defined(A_SIMOS_DEVHOST)        // { A_SIMOS_DEVHOST
// FIX? UART REG WRITE NOT SUPPORTED YET FOR A_SIMOS_DEVHOST
#define A_IX_UART_REG_WRITE(p,addr,val) do {;} while (0)
#else                               // }{ A_SIMOS_DEVHOST
#define A_IX_UART_REG_WRITE(p,addr,val) \
    A_SOC_ADDR_WRITE((p)->BaseAddress + (A_UINT32)(addr), (val))
#endif                              // } A_SIMOS_DEVHOST

#if defined(A_SIMOS_DEVHOST)        // { A_SIMOS_DEVHOST
// FIX? UART REG READ NOT SUPPORTED YET FOR A_SIMOS_DEVHOST
#define A_IX_UART_REG_READ(p,addr) (0)
#else                               // }{ A_SIMOS_DEVHOST
#define A_IX_UART_REG_READ(p,addr) \
    A_SOC_ADDR_READ((p)->BaseAddress + (A_UINT32)(addr))
#endif                              // } A_SIMOS_DEVHOST


void _StartStopInterruptPolling(UART_DEVICE *pUartDevice, A_BOOL Start);
void _InterruptPollCallback(A_HANDLE alarm, void *data);
void _UartTryTransmit(UART_DEVICE *pUartDevice);
void _UartReceive(UART_DEVICE *pUartDevice);
void _UartDevSetBaud(UART_DEVICE *pUartDevice, A_UINT32 Baud, A_UINT32 Scale, A_UINT32 Step);
void _CatchTxOverFlowError(UART_DEVICE *pUartDevice);
void _CatchRxOverFlowError(UART_DEVICE *pUartDevice);
int  _UartDevDsrHandler(void *data);
void _UartDevPortPinsEnableDisable(UART_DEVICE *pUartDevice, A_BOOL Enable);
void _UartDevShutDownUart(UART_DEVICE *pUartDevice);
void _UartDevSetupUart(UART_DEVICE *pUartDevice, SER_PORT_UART_CONFIG *pConfig);
void _UartConfigureRxWake(UART_DEVICE *pUartDevice, A_BOOL Enable);

typedef struct UART_SERP_INTERNAL_APIS {
    void (*_StartStopInterruptPolling)(UART_DEVICE *pUartDevice, A_BOOL Start);
    void (*_InterruptPollCallback)(A_HANDLE alarm, void *data);
    void (*_UartTryTransmit)(UART_DEVICE *pUartDevice);
    void (*_UartReceive)(UART_DEVICE *pUartDevice);
    void (*_UartDevSetBaud)(UART_DEVICE *pUartDevice, A_UINT32 Baud, A_UINT32 Scale, A_UINT32 Step);
    void (*_CatchTxOverFlowError)(UART_DEVICE *pUartDevice);
    void (*_CatchRxOverFlowError)(UART_DEVICE *pUartDevice);
    int  (*_UartDevDsrHandler)(void *data);
    void (*_UartDevPortPinsEnableDisable)(UART_DEVICE *pUartDevice, A_BOOL Enable);
    void (*_UartDevShutDownUart)(UART_DEVICE *pUartDevice);
    void (*_UartDevSetupUart)(UART_DEVICE *pUartDevice, SER_PORT_UART_CONFIG *pConfig);
    void (*_UartRxPinInterruptHandler)(void *data);
    void (*_UartConfigureRxWake)(UART_DEVICE *pUartDevice, A_BOOL Enable);
} UART_SERP_INTERNAL_APIS;

extern UART_SERP_INTERNAL_APIS g_UartSerpIndirectionTable;

#define CALL_UART_SERP_INDIR_FN(fn) g_UartSerpIndirectionTable.fn
#define GET_UART_SERP_INDIR_FN(fn)  g_UartSerpIndirectionTable.fn

#define USAPI

#define StartStopInterruptPolling(p,S) CALL_UART_SERP_INDIR_FN(_StartStopInterruptPolling(p,S))
#define InterruptPollCallback(a,d)     CALL_UART_SERP_INDIR_FN(_InterruptPollCallback(a,d))
#define UartTryTransmit(p)             CALL_UART_SERP_INDIR_FN(_UartTryTransmit(p))
#define UartReceive(p)                 CALL_UART_SERP_INDIR_FN(_UartReceive(p))
#define UartDevSetBaud(p,B,Sc,St)      CALL_UART_SERP_INDIR_FN(_UartDevSetBaud(p,B,Sc,St))
#define CatchTxOverFlowError(p)        CALL_UART_SERP_INDIR_FN(_CatchTxOverFlowError(p))
#define CatchRxOverFlowError(p)        CALL_UART_SERP_INDIR_FN(_CatchRxOverFlowError(p))
#define UartDevDsrHandler(d)           CALL_UART_SERP_INDIR_FN(_UartDevDsrHandler(d))
#define UartDevPortPinsEnableDisable(p,E) CALL_UART_SERP_INDIR_FN(_UartDevPortPinsEnableDisable(p,E))
#define UartDevShutDownUart(p)         CALL_UART_SERP_INDIR_FN(_UartDevShutDownUart(p))
#define UartDevSetupUart(p,C)          CALL_UART_SERP_INDIR_FN(_UartDevSetupUart(p,C))
#define UartConfigureRxWake(p,e)       CALL_UART_SERP_INDIR_FN(_UartConfigureRxWake(p,e))

#if defined(AR6002_REV4)
    /* pin mux value for all GPIO pins to switch to UART pins */

#define UART0_CFG       0x4
#define UART0_GPIO_CFG  0x2
#define UART0_TX_PIN    8
#define UART0_RX_PIN    17
#define UART0_RTS_PIN   0
#define UART0_CTS_PIN   0

#define UART1_CFG       0x4
#define UART1_GPIO_CFG  0x2
#define UART1_TX_PIN    15
#define UART1_RX_PIN    17
#define UART1_RTS_PIN   16
#define UART1_CTS_PIN   18

#elif defined(AR6002_REV6) || defined(AR6002_REV7)

#if defined(AR6002_REV75) || defined(AR6002_REV76)

#define UART0_CFG       0x5
#define UART0_GPIO_CFG  0xf
#define UART0_TX_PIN    29
#define UART0_RX_PIN    28
#define UART0_RTS_PIN   -1
#define UART0_CTS_PIN   -1

#define UART1_CFG       0x5
#define UART1_GPIO_CFG  0xf

#define UART1_TX_PIN    23
#define UART1_RX_PIN    24

#define UART1_RTS_PIN   22
#define UART1_CTS_PIN   21 

#define UART2_CFG       0x5
#define UART2_GPIO_CFG  0xf
#define UART2_TX_PIN    2
#define UART2_RX_PIN    3  
#define UART2_RTS_PIN   4 
#define UART2_CTS_PIN   5 

#else

#define UART0_CFG       0x3
#define UART0_GPIO_CFG  0x6
#define UART0_TX_PIN    7
#define UART0_RX_PIN    2
#define UART0_RTS_PIN   -1
#define UART0_CTS_PIN   -1

#define UART1_CFG       0x4
#define UART1_GPIO_CFG  0x6
#if defined(FPGA)
#define UART1_TX_PIN    24
#define UART1_RX_PIN    25
#else
#define UART1_TX_PIN    11
#define UART1_RX_PIN    10
#endif
#define UART1_RTS_PIN   -1 //8
#define UART1_CTS_PIN   -1  //9

#define UART2_CFG       0x4
#define UART2_GPIO_CFG  0x2
#define UART2_TX_PIN    -1
#define UART2_RX_PIN    -1
#define UART2_RTS_PIN   -1
#define UART2_CTS_PIN   -1

#endif /*AR6002_REV75*/
#else

#error "must define default GPIOs for UARTs"

#endif

#endif /*UARTSERP_INTERNAL_H_*/
