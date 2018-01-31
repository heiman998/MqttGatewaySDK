/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: WLAN UART (aka HCI UART) serial port implementation
//
// @Notes:
//
//

#include <osapi.h>
#include "uartserp_internal.h"
#include <hw/uart_reg.h>
#include "select_handler_api.h"
#include "qcom/select_api.h"
#include "wmi.h"
#include "qcom/qcom_pwr.h"
#include "hw/gpio_reg.h"

#define intr_disable_high athos_block_all_intrlvl_high
extern A_old_intr_t intr_disable_high(void);

// Call this function in ISR, DSR and others which will be impacted by uart2 interrupt.
#define  A_INTR_DISABLE_HIGH(pOld)                      \
    do {                                                \
        *(pOld) = intr_disable_high();                  \
    } while (0)

// enable this macro for polling mode of debug uart
#undef DBG_UART_POLLING_MODE
//#define DBG_UART_POLLING_MODE

#undef A_WUART_REG_READ
#undef A_WUART_REG_WRITE

#define DBG_UART_DELAY_US_FOR_POWER_SAVE 	(240) // DELAY: 240 us for power save mode (DBG UART prints messy code if no delay)

UART_HW_PIN_CONFIG g_Uart0DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART0_RX_PIN,
     .ConfigValue  = UART0_CFG,
     .ConfigAsGpio = UART0_GPIO_CFG},
    {.GpioPin      = UART0_CTS_PIN,
     .ConfigValue  = UART0_CFG, 
     .ConfigAsGpio = UART0_GPIO_CFG},
    {.GpioPin      = UART0_TX_PIN,
     .ConfigValue  = UART0_CFG, 
     .ConfigAsGpio = UART0_GPIO_CFG},
    {.GpioPin      = UART0_RTS_PIN,
     .ConfigValue  = UART0_CFG, 
     .ConfigAsGpio = UART0_GPIO_CFG},          
};

UART_HW_PIN_CONFIG g_Uart1DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART1_RX_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
    {.GpioPin      = UART1_CTS_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
    {.GpioPin      = UART1_TX_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG},
    {.GpioPin      = UART1_RTS_PIN,
     .ConfigValue  = UART1_CFG, 
     .ConfigAsGpio = UART1_GPIO_CFG}, 
};

#if defined(AR6002_REV6) || defined(AR6002_REV7)

UART_HW_PIN_CONFIG g_Uart2DefaultPins[UART_HW_PIN_MAX] =
{
    {.GpioPin      = UART2_RX_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_CTS_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_TX_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG},
    {.GpioPin      = UART2_RTS_PIN,
     .ConfigValue  = UART2_CFG, 
     .ConfigAsGpio = UART2_GPIO_CFG}, 
};

#endif

UART_DEVICE *g_pUartDevices[UART_SERP_MAX_INSTANCES];
extern int uart_init; 


void Uart_Rx_Pin_Set(int pin0, int pin1)
{
    int ConfigValue;
    /* Config RX pin for UART0 */
    ConfigValue = -1;

    A_PRINTF("set rx pin is not supported\n");
    return;
    
    switch(pin0) {
        case 6:
            ConfigValue = 4;
            break;
        case 10:
            ConfigValue = 8;
            break;
        case 15:
            ConfigValue = 2;
            break;
        case 19:
            ConfigValue = 2;
            break;
        case 21:
            ConfigValue = 8;
            break;
        case 25:
            ConfigValue = 3;
            break;
        default:
            break;
        }
    if(ConfigValue >= 0)
    {
        g_Uart0DefaultPins[0].GpioPin=pin0;
        g_Uart0DefaultPins[0].ConfigValue = ConfigValue;
    }
    /* Config RX pin for UART1 */
    ConfigValue = -1;
    switch(pin1) {
        case 2:
            ConfigValue = 1;
            break;
        case 6:
            ConfigValue = 3;
            break;
        case 10:
            ConfigValue = 4;
            break;
        case 15:
            ConfigValue = 5;
            break;
        case 19:
            ConfigValue = 11;
            break;
        case 21:
            ConfigValue = 9;
            break;
        case 25:
            ConfigValue = 4;
            break;
        default:
            break;
        }
    if(ConfigValue >= 0)
    {
        g_Uart1DefaultPins[0].GpioPin=pin1;
        g_Uart1DefaultPins[0].ConfigValue = ConfigValue;
    }
}

void Uart_Tx_Pin_Set(int pin0, int pin1)
{
    int ConfigValue;
    /* Config TX pin for UART0 */
    ConfigValue = -1;
 
    A_PRINTF("set tx pin is supported\n");
    return;

    switch(pin0) {
        case 7:
            ConfigValue = 3;
            break;
        case 11:
            ConfigValue = 8;
            break;
        case 16:
            ConfigValue = 2;
            break;
        case 20:
            ConfigValue = 2;
            break;
        case 24:
            ConfigValue = 3;
            break;
        default:
            break;
        }
    if(ConfigValue >= 0)
    {
        g_Uart0DefaultPins[2].GpioPin=pin0;
        g_Uart0DefaultPins[2].ConfigValue = ConfigValue;
    }
    /* Config TX pin for UART1 */
    ConfigValue = -1;
    switch(pin1) {
        case 4:
            ConfigValue = 1;
            break;
        case 7:
            ConfigValue = 1;
            break;
        case 11:
            ConfigValue = 4;
            break;
        case 16:
            ConfigValue = 5;
            break;
        case 20:
            ConfigValue = 5;
            break;
        case 24:
            ConfigValue = 4;
            break;
        default:
            break;
        }
    if(ConfigValue >= 0)
    {
        g_Uart1DefaultPins[2].GpioPin=pin1;
        g_Uart1DefaultPins[2].ConfigValue = ConfigValue;
    }
}

USAPI void _StartStopInterruptPolling(UART_DEVICE *pUartDevice, A_BOOL Start)
{
    if (Start) {
        if (!pUartDevice->TimerQueued) {
            pUartDevice->TimerQueued = TRUE;
            A_TIMEOUT_MS(&pUartDevice->PollingTimer,pUartDevice->PollTimeoutMS,0);        
        }   
    } else {
        if (pUartDevice->TimerQueued) {
            pUartDevice->TimerQueued = FALSE;
            A_UNTIMEOUT(&pUartDevice->PollingTimer);    
        } 
    }
}

USAPI void _InterruptPollCallback(A_HANDLE alarm, void *data)
{ 
    UART_DEVICE *pUartDevice = (UART_DEVICE *)data;
    
    pUartDevice->TimerQueued = FALSE;
 
    UartDevDsrHandler(pUartDevice);
    
    if (A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS) != 0) {
            /* keep polling */    
        StartStopInterruptPolling(pUartDevice,TRUE);
    }
}
    
static void DisableUartInterrupts(UART_DEVICE *pUartDevice, A_UINT32 Ints)
{
    A_UINT32 val = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
    val &= ~Ints;
    A_IX_UART_REG_WRITE(pUartDevice,UART_INT_EN_ADDRESS,val);
    (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
}

static void EnableUartInterrupts(UART_DEVICE *pUartDevice, A_UINT32 Ints)
{
    A_UINT32 val = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS); 
    val |= Ints;
    A_IX_UART_REG_WRITE(pUartDevice,UART_INT_EN_ADDRESS,val);
    (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);  
}

USAPI void _UartTryTransmit(UART_DEVICE *pUartDevice)
{
    A_UINT8     byte;
    A_UINT32    data;
    int         bytesPopped,bytesTransmitted;
    A_BOOL      noRoom = FALSE;
	A_old_intr_t old_intr;
    
    bytesTransmitted = 0;
    
    while  (TRUE) {
        
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
        
            /* check if there is room in the TX FIFO */
        data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);             
        if (!(data & UART_DATA_TX_CSR_MASK)) {
            noRoom = TRUE;
            break;    
        }        
            /* TODO, this will be a bit slow */
        bytesPopped = SERPHW_PopTXBytes(pUartDevice->hPortDev,&byte,1);
        if (!bytesPopped) {
                /* ring buffer in upper layer is now drained */
            break;    
        }           
        
            /* push byte onto the FIFO */
        data = byte | UART_DATA_TX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS, data);
            /* flush before looping around */
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
          
        bytesTransmitted++;
        
    }   
    
	A_INTR_DISABLE_HIGH(&old_intr);
    if (bytesTransmitted || noRoom) {
        UART_STAT_INCR(pUartDevice,TxBytes,(A_UINT32)bytesTransmitted); 
    	pUartDevice->EnableInterrupts |= UART_INT_EN_TX_EMPTY_INT_EN_MASK;
		if (!pUartDevice->RxIsr)
		{
			EnableUartInterrupts(pUartDevice, UART_INT_EN_TX_EMPTY_INT_EN_MASK);
		} 
    } else {
            /* transmitter is now idle */
        pUartDevice->EnableInterrupts &= ~UART_INT_EN_TX_EMPTY_INT_EN_MASK;
		if (!pUartDevice->RxIsr)
		{
			DisableUartInterrupts(pUartDevice,UART_INT_EN_TX_EMPTY_INT_EN_MASK);
		}
    }

	A_INTR_RESTORE(old_intr);
}

void SERHWAPI_PushRXBytes_Notify(SER_PORT_INSTANCE Instance);
int SERHWAPI_PushRXBytes_Push(SER_PORT_INSTANCE Instance, A_UINT8 *pBuffer, int Length);
A_BOOL SERHWAPI_RXBuffer_Lock(SER_PORT_INSTANCE Instance);

USAPI void _UartReceive(UART_DEVICE *pUartDevice)
{
    A_UINT32 data;
    A_UINT8  byte;
    int      bytesPushed;

    while (TRUE) {
     
            /* read the FIFO port, this will not pop the actual byte out of the FIFO */    
        data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);        
            /* check if the byte is valid */
        if (!(data & UART_DATA_RX_CSR_MASK)) {
            break;    
        }
        
        byte = (A_UINT8)(data & 0xff);

            /* advance the RX FIFO */
        data = UART_DATA_RX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS, data);
            /* flush */
        (void)A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
         
            /* TODO : this will be slow */
        bytesPushed = SERPHW_PushRXBytes(pUartDevice->hPortDev,&byte,1);
        
        if (!bytesPushed) {
			#if 0
                /* upper layer ring buffer can't take anymore data */
            pUartDevice->SavedByte = (A_UINT16)byte | SAVED_BYTE_VALID;
                /* disable interrupt and wait */
            DisableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK); 
			#endif
            //break; 
            pUartDevice->RxDsrDiscard++;
        }   
        
        UART_STAT_INCR(pUartDevice,RxBytes,(A_UINT32)bytesPushed);                                    
    }
    
}

    /* callback from serial layer when the application writes bytes to the TX buffer,
     * this is to kick start the transmitter */
static void UartDevHWTransmit(void *pPortContext, int TxBufferDepth)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    UartTryTransmit(pUartDevice); 
}


    /* drain handler, called by serial port layer when there is room in receiver to queue more bytes*/
static void UartDevDrainRX(void *pPortContext)
{
#if 0
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    if (pUartDevice->SavedByte) {
        A_UINT8 byte = (A_UINT8)(pUartDevice->SavedByte & 0xFF);
        pUartDevice->SavedByte = 0;
        SERPHW_PushRXBytes(pUartDevice->hPortDev,&byte,1);    
    }    
        /* re-enable recv interrupts */
    EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);  
#endif
}

USAPI void _UartDevSetBaud(UART_DEVICE *pUartDevice, A_UINT32 Baud, A_UINT32 Scale, A_UINT32 Step)
{
    int      scale;
    int      step;
    A_UINT64 step_temp;    
    
    if (Baud != 0) 
    {
        //for lower baudrate
        if(Baud < 4800)
        {
            /* this is a rough calculation that works for lower baud rates */
            int divisor = A_UART_FREQUENCY() / 16 / Baud;
            step = 8192;
            while (divisor > 0xff) 
            {
                divisor = divisor >> 1;
                step = step >> 1;
            }
            scale = divisor;
        }
        else
        {
            //scale =truncate(((1310 * serialClockFreq) / (131072 * baudClockFreq)))
            //step = round((131072 * baudClockFreq) / (serialClockFreq/(UartClockScale+1)))
            scale = A_UART_FREQUENCY()/100/Baud;
            step_temp = ((A_UINT64)Baud << 17) *10/(A_UART_FREQUENCY()/(scale+1));
            step = (step_temp + 5) / 10;
        }        
    }
    else  
    {
            /* use explicit values */
        scale = Scale;
        step =  Step;  
    }     
    
        /* write baud value */
    A_IX_UART_REG_WRITE(pUartDevice,UART_CLKDIV_ADDRESS, 
                      (UART_CLKDIV_CLK_SCALE_SET(scale) | UART_CLKDIV_CLK_STEP_SET(step)));
    
}

void UartRxFifo(UART_DEVICE *pUartDevice, A_BOOL force)
{
    A_UINT32 data;
    A_UINT8  byte;
    
    while (TRUE) {
     
        /* read the FIFO port, this will not pop the actual byte out of the FIFO */    
        data = A_IX_UART_REG_READ(pUartDevice, UART_DATA_ADDRESS);        
        /* check if the byte is valid */
        if (!(data & UART_DATA_RX_CSR_MASK)) {
            break;    
        }
		
        byte = (A_UINT8)(data & 0xff);
        /* advance the RX FIFO */
        data = UART_DATA_RX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice, UART_DATA_ADDRESS, data);

		if (pUartDevice->BufferLength < UART_RX_BUFFER_SIZE_INTERNAL)
		{
			pUartDevice->Buffer[pUartDevice->BufferLength++] = byte;
		}
		else
		{
			pUartDevice->RxIsrDiscard++;
		}
    }

	if (force || (pUartDevice->BufferLength >= (UART_RX_BUFFER_SIZE_INTERNAL - UART_RX_BUFFER_SIZE_MARGIN)
				   && !SERHWAPI_RXBuffer_Lock(pUartDevice->hPortDev)))
	{
		A_UINT32 bytesPushed = 0;
		A_UINT32 length = pUartDevice->BufferLength;
		
		bytesPushed = SERHWAPI_PushRXBytes_Push(pUartDevice->hPortDev, pUartDevice->Buffer, length);
		
		pUartDevice->RxDsrDiscard += length - bytesPushed;
		pUartDevice->BufferLength = 0;

		if (pUartDevice->RxMax < length)
		{
			pUartDevice->RxMax = length;
		}
		
		if (bytesPushed)
		{
			pUartDevice->RxWakeup = TRUE;
			UART_STAT_INCR(pUartDevice,RxBytes,bytesPushed);
		}
	}
}

static A_UINT32 UartDevIsrHandler(void *unused)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)unused;
    
	{
		A_UINT32 interrupts;
		A_UINT32 enable_interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);
        interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS);    
		DisableUartInterrupts(pUartDevice, ~UART_INT_RX_FULL_INT_MASK);
		
        if (interrupts & UART_INT_RX_FULL_INT_MASK) 
        {
        	UartRxFifo(pUartDevice, FALSE);
			pUartDevice->RxFullCount++;
			A_IX_UART_REG_WRITE(pUartDevice,UART_INT_ADDRESS,UART_INT_RX_FULL_INT_MASK);
        }

		if (!pUartDevice->RxIsr)
		{
			pUartDevice->RxIsr = TRUE;
			pUartDevice->EnableInterrupts = enable_interrupts;
		}
	}
    return A_ISR_CALL_DSR;
}

USAPI void _CatchTxOverFlowError(UART_DEVICE *pUartDevice)
{
    A_ASSERT(FALSE);    
}

USAPI void _CatchRxOverFlowError(UART_DEVICE *pUartDevice)
{
    /* TODO */
    
}

extern unsigned int sleep_disable_refcnt;
A_BOOL IsSystemSleepAllowed()
{
	return (sleep_disable_refcnt == 0); // power save
}

USAPI int _UartDevDsrHandler(void *data)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)data;
    A_UINT32 interrupts;
	A_old_intr_t old_intr;

	// Critical code
	A_INTR_DISABLE_HIGH(&old_intr);
	if (!pUartDevice->RxIsr)
	{
		A_UINT32 enable_interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);  
		DisableUartInterrupts(pUartDevice, ~UART_INT_RX_FULL_INT_MASK);
		pUartDevice->EnableInterrupts = enable_interrupts;
		pUartDevice->RxIsr = TRUE;
	}
	A_INTR_RESTORE(old_intr);
	// Add code after here.
	
    while (TRUE) {
    
        interrupts = A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS);    
            /* check only those enabled */
        interrupts &= pUartDevice->EnableInterrupts; //A_IX_UART_REG_READ(pUartDevice,UART_INT_EN_ADDRESS);
                
        if (!interrupts) {
            break;    
        }     
      
        if (interrupts & UART_INT_RX_VALID_INT_MASK) {
			A_BOOL wakeup = FALSE;
            UART_STAT_INCR(pUartDevice,RxIrqs,1);
			
			DisableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);
    		
        	UartRxFifo(pUartDevice, TRUE);
			
			wakeup = pUartDevice->RxWakeup;
			pUartDevice->RxWakeup = FALSE;
			
			EnableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);

			if (wakeup)
			{
				SERHWAPI_PushRXBytes_Notify(pUartDevice->hPortDev);
			}
			
            if (uart_init) {
                uart_wakeup_select((char* )pUartDevice->SerialDev.pPortName);
			}
        }
        
        if (interrupts & UART_INT_TX_EMPTY_INT_MASK) {
            UART_STAT_INCR(pUartDevice,TxIrqs,1); 
            UartTryTransmit(pUartDevice); 
			
			if (pUartDevice->InstanceId == UART_SERP_UART0_INSTANCE_ID) {
				
				if (IsSystemSleepAllowed()) {
					// Power Save Mode
					A_DELAY_USECS(DBG_UART_DELAY_US_FOR_POWER_SAVE);
 				}
			}
        }
        
        if (interrupts & UART_INT_RX_OFLOW_ERR_INT_MASK) {
			pUartDevice->RxOverFlow++;
            CatchRxOverFlowError(pUartDevice);  
        }
        
        if (interrupts & UART_INT_RX_FRAMING_ERR_INT_MASK) {
            /* TODO */   
            UART_STAT_INCR(pUartDevice,RxFramingErrors,1);
        }
        
        if (interrupts & UART_INT_RX_PARITY_ERR_INT_MASK) {
            /* TODO */
            UART_STAT_INCR(pUartDevice,RxParityErrors,1); 
        }
        
        if (interrupts & UART_INT_TX_OFLOW_ERR_INT_MASK) {
                /* this should never happen since software always checks if the FIFO
                 * has room for a byte */
            CatchTxOverFlowError(pUartDevice);
        }

        /* According to the HW datasheet:
           1.RX_VALID interrupt can be cleared if there is no more data in the receive FIFO. 
           2.TX_READY interrupt, setting this bit will clear this interrupt
           if there is room for more data in the transmit FIFO. 
        */
        /* ack interrupts */
        A_IX_UART_REG_WRITE(pUartDevice,UART_INT_ADDRESS,interrupts); 
        (void)A_IX_UART_REG_READ(pUartDevice,UART_INT_ADDRESS); 
        
    }
	
	DisableUartInterrupts(pUartDevice, UART_INT_RX_FULL_INT_MASK);
	pUartDevice->RxIsr = FALSE;
	EnableUartInterrupts(pUartDevice, (pUartDevice->EnableInterrupts | UART_INT_RX_FULL_INT_MASK));
	    
    return A_HANDLER_DONE; 
}

USAPI void _UartDevPortPinsEnableDisable(UART_DEVICE *pUartDevice, A_BOOL Enable)
{
    int i;
    
    if (Enable) {
        /* disable pin hold circuits and reconfigure GPIOs */
        for (i = 0; i < UART_HW_PIN_MAX; i++) {
            if (pUartDevice->HwPins.PinList[i].GpioPin > 0) {
                gpio_configure_pin(pUartDevice->HwPins.PinList[i].GpioPin, 
                                   pUartDevice->HwPins.PinList[i].ConfigValue, 
                                   pUartDevice->PinIsInput[i],
                                   GPIO_PIN_PULLNONE);
            }              
        }               
    } else {

        /* when port is disabled, we pulldown all pins and set them as inputs */               
        for (i = 0; i < UART_HW_PIN_MAX; i++) {
            if (pUartDevice->HwPins.PinList[i].GpioPin > 0) {
                if (i == UART_HW_TX) {
                    /* TODO-FIX ME, there is an issue in the FPGA where converting 
                     * the TX output as a GPIO input takes down debug UART as well */
                } else {
                    gpio_configure_pin(pUartDevice->HwPins.PinList[i].GpioPin, 
                                       pUartDevice->HwPins.PinList[i].ConfigAsGpio, /* revert to gpio */
                                       TRUE,  /* configure as an input */
                                       GPIO_PIN_PULLDOWN);
                }
            }              
        }         
    }
}

USAPI void _UartDevShutDownUart(UART_DEVICE *pUartDevice)
{   
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,FALSE);    
    }
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS); 
    UartDevPortPinsEnableDisable(pUartDevice,FALSE);  
}

USAPI void _UartDevSetupUart(UART_DEVICE *pUartDevice, SER_PORT_UART_CONFIG *pConfig)
{
    A_UINT32 regVal,baud;

     if(pUartDevice->BaseAddress ==WLAN_UART2_BASE_ADDRESS)  {
	  A_GPIO_REG_WRITE(SDIO_ADDRESS,0x0);
     }

        /* reset UART block */
    regVal = A_RTC_SOC_REG_READ(SOC_RESET_CONTROL_ADDRESS);
    if(pUartDevice->BaseAddress == WLAN_UART_BASE_ADDRESS) {
        regVal |= RESET_CONTROL_UART_RST_SET(1);
    }
    else if(pUartDevice->BaseAddress ==WLAN_DBG_UART_BASE_ADDRESS)  {
        regVal |= RESET_CONTROL_DEBUG_UART_RST_SET(1);
    } else if(pUartDevice->BaseAddress ==WLAN_UART2_BASE_ADDRESS)  {
	regVal |= RESET_CONTROL_UART2_RST_SET(1);
    } else{
        A_ASSERT(FALSE);
    }
    A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS,regVal);
    if(pUartDevice->BaseAddress == WLAN_UART_BASE_ADDRESS) {
        regVal &= ~RESET_CONTROL_UART_RST_MASK;
    }
    else if(pUartDevice->BaseAddress ==WLAN_DBG_UART_BASE_ADDRESS)    {
        regVal &= ~RESET_CONTROL_DEBUG_UART_RST_MASK;
    } else if(pUartDevice->BaseAddress == WLAN_UART2_BASE_ADDRESS) {
       regVal &= ~RESET_CONTROL_UART2_RST_MASK;
    } else{
        A_ASSERT(FALSE);
    }
    A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS,regVal);

        /* Disable interrupts */
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS);   
    
        /* we can enable the port pins now */
    UartDevPortPinsEnableDisable(pUartDevice,TRUE);
    
        /* enable the interface, set for default DTE operation */
    regVal = UART_CONTROL_IFC_ENABLE_SET(1) | UART_CONTROL_HOST_INT_ENABLE_SET(1);
    
    if (pConfig->ConfigFlags & SER_PORT_CONFIG_RTS_CTS_FLOW) {
            /* flow control enabled */
        A_ASSERT(pUartDevice->HwPins.PinList[UART_HW_CTS].GpioPin >= 0);
        A_ASSERT(pUartDevice->HwPins.PinList[UART_HW_RTS].GpioPin >= 0);
        regVal |= UART_CONTROL_FLOW_ENABLE_SET(1);
    } else {
        regVal |= UART_CONTROL_RX_READY_ORIDE_SET(1);
        regVal |= UART_CONTROL_TX_READY_ORIDE_SET(1);     
    }
    
    if (pConfig->ConfigFlags & SER_PORT_CONFIG_PARITY_CHECK) {
        regVal |= UART_CONTROL_PARITY_ENABLE_SET(1);
        if (!(pConfig->ConfigFlags & SER_PORT_CONFIG_ODD_PARITY)) {
                /* even parity */
            regVal |= UART_CONTROL_PARITY_EVEN_SET(1);    
        }    
    }
   
    A_IX_UART_REG_WRITE(pUartDevice,UART_CONTROL_ADDRESS, regVal); 

    baud = pConfig->Baud;
    if (0 == baud) {
            /* caller will set a more precise setting for the baud rate, for now
             * just use a default */
        baud = DEFAULT_UART_BAUD_RATE; 
    }
    
    UartDevSetBaud(pUartDevice,baud,0,0);  
        
        /* enable interrupts */
    EnableUartInterrupts(pUartDevice,
                         UART_INT_EN_TX_OFLOW_ERR_INT_EN_MASK   | 
                         UART_INT_EN_RX_FRAMING_ERR_INT_EN_MASK | 
                         UART_INT_EN_RX_OFLOW_ERR_INT_EN_MASK);  

    if (pConfig->ConfigFlags & SER_PORT_CONFIG_PARITY_CHECK) {
        EnableUartInterrupts(pUartDevice,UART_INT_RX_PARITY_ERR_INT_MASK);    
    }
    
    UART_RESET_STATS(pUartDevice);
    
    EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK | UART_INT_EN_RX_FULL_INT_EN_MASK); 
    
#ifdef DBG_UART_POLLING_MODE   //For DBG UART polling mode
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,TRUE);
    }       
#endif    
}

void UartConfigureRxPolling(int id, int en)
{
    UART_DEVICE * pUartDevice = g_pUartDevices[id];
    if (pUartDevice->Polling) {
        StartStopInterruptPolling(pUartDevice,en);
    }       
}

USAPI void _UartConfigureRxWake(UART_DEVICE *pUartDevice, A_BOOL Enable)
{
    
    if (Enable) {
            /* switch RX as a GPIO input */
        gpio_configure_pin(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin, 
                           pUartDevice->HwPins.PinList[UART_HW_RX].ConfigValue, //ConfigAsGpio
                           GPIO_PIN_DIR_INPUT,
                           GPIO_PIN_PULLNONE);
        gpio_register_pin_interrupt(&pUartDevice->RxPinInterrupt);
        gpio_set_interrupt_mode(&pUartDevice->RxPinInterrupt,GPIO_PIN_INT_LEVEL_LOW);//GPIO_PIN_INT_FALLING_EDGE
        gpio_arm_interrupt(&pUartDevice->RxPinInterrupt);
        if (pUartDevice->Polling) {
                /* stop any polling */
            StartStopInterruptPolling(pUartDevice,FALSE);
        } else {
                /* disable RX interrupts */
            DisableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);
        }
        uart_set_wakeup_enable(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin, TRUE);
    } else {
        gpio_deregister_pin_interrupt(&pUartDevice->RxPinInterrupt);
            /* return RX pin back to normal */
        gpio_configure_pin(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin, 
                           pUartDevice->HwPins.PinList[UART_HW_RX].ConfigValue, 
                           GPIO_PIN_DIR_INPUT,
                           GPIO_PIN_PULLNONE);
        if (pUartDevice->Polling) {
                /* start up polling */
            StartStopInterruptPolling(pUartDevice,TRUE);
        } else {
                /* re-enable RX interrupts */
            EnableUartInterrupts(pUartDevice,UART_INT_EN_RX_VALID_INT_EN_MASK);
        }
        uart_set_wakeup_enable(pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin, FALSE);
    }
    
}

    /* direct write without using interrupts, this is to accommodate console operation 
     * that needs to allow exception printing through the current console port */
static void UartDeviceDirectByteWrite(UART_DEVICE *pUartDevice, A_UINT8 *pBuffer, int Count)
{
    int      i;
    A_UINT32 data;
        
    for (i = 0; i < Count; i++) {
        while (TRUE) {
            data = A_IX_UART_REG_READ(pUartDevice,UART_DATA_ADDRESS);
            if (data & UART_DATA_TX_CSR_MASK) {
                break;
            }
        } 

        data = *pBuffer | UART_DATA_TX_CSR_SET(1);
        A_IX_UART_REG_WRITE(pUartDevice,UART_DATA_ADDRESS,data);
        
        pBuffer++;
        
        while (TRUE) {
            data = A_IX_UART_REG_READ(pUartDevice,UART_CONTROL_ADDRESS);
            if (!(data & UART_CONTROL_TX_BUSY_MASK)) {
                break;
            }
        }
    }    
        
}

void CallUartDeviceDirectByteWrite(UART_DEVICE *pUartDevice, A_UINT8 *pBuffer, int Count)
{
    UartDeviceDirectByteWrite(pUartDevice, pBuffer, Count);
}

    
static A_STATUS UartDevIoctl(void       *pPortContext, 
                             A_UINT16   Code, 
                             void       *pBuffer,
                             int        BufferLengthOrArg)
{
    UART_DEVICE *pUartDevice = (UART_DEVICE *)pPortContext;
    A_STATUS status = A_OK;
    
    switch (Code) {
        case SER_PORT_IOCTL_FLUSH :
        
            if ((BufferLengthOrArg == SER_PORT_FLUSH_ALL) ||
                (BufferLengthOrArg == SER_PORT_FLUSH_RX)) {
                    
            } 
                
            /* TODO: flush pending TX? */
            break;  
        case SER_PORT_IOCTL_CONFIG_UART_PORT :
            {
                SER_PORT_UART_CONFIG *pConfig = (SER_PORT_UART_CONFIG *)pBuffer;
                if ((pConfig->DataBits != 8) || (pConfig->StopBits != 1)) {
                    status = A_ENOTSUP;
                    break;    
                }
                UartDevSetupUart(pUartDevice,pConfig);
            }
            break;   
        case SER_PORT_HW_IOCTL_DIRECT_BAUD :
            UartDevSetBaud(pUartDevice,
                           0, 
                           ((SER_PORT_HW_BAUD_CONFIG *)pBuffer)->Scale,
                           ((SER_PORT_HW_BAUD_CONFIG *)pBuffer)->Step);
            break;
        case SER_PORT_HW_IOCTL_STOP :
            UartDevShutDownUart(pUartDevice);
            break;
        case SER_PORT_HW_IOCTL_SET_POLL_INTERVAL :
            pUartDevice->PollTimeoutMS = (A_UINT32)BufferLengthOrArg;
            break;
        case SER_PORT_HW_IOCTL_CONFIG_RX_WAKEUP :
            if (BufferLengthOrArg) {
                UartConfigureRxWake(pUartDevice,TRUE);   
            } else {
                UartConfigureRxWake(pUartDevice,FALSE);         
            }
            break;
        case SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT :
            UartDeviceDirectByteWrite(pUartDevice,(A_UINT8 *)pBuffer,(int)BufferLengthOrArg);
	    break;
        default:
            break;    
    }
    
    return status;   
}

struct cmnos_intr_service_s {
    A_handler_t dsr;            /* Delayed Service Routine */
    void       *dsr_arg;

    A_isr_t     isr;            /* Interrupt Service Routine */
    void       *isr_arg;
    A_UINT32    maskoff;        /* Which intrs to mask after ISR runs and before DSR completes */
};
extern struct cmnos_intr_service_s cmnos_intr_service[NUM_DIRECT_INTR];

void SetInterruptEnable_AfterIsr(A_UINT32 inum)
{
    A_ASSERT(inum < NUM_DIRECT_INTR);
	
	cmnos_intr_service[inum].maskoff |= 1 << inum;
}

static void UartDev_Start(void *pInstance, UART_HW_CONFIG *pConfig)
{
    UART_DEVICE         *pUartDevice;
    UART_HW_PIN_CONFIG  *pDefConfig;
    
    A_ASSERT(pInstance != NULL);
    
    pUartDevice = (UART_DEVICE *)pInstance; 
    
    pDefConfig = pUartDevice->pDefPinConfigTable;
    
    A_MEMCPY(&pUartDevice->HwPins,pConfig,sizeof(UART_HW_CONFIG));
    
    if (pUartDevice->HwPins.PinList[UART_HW_TX].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_TX] = pDefConfig[UART_HW_TX];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_RX] = pDefConfig[UART_HW_RX];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_CTS].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_CTS] = pDefConfig[UART_HW_CTS];
    }
    
    if (pUartDevice->HwPins.PinList[UART_HW_RTS].GpioPin == 0) {
        pUartDevice->HwPins.PinList[UART_HW_RTS] = pDefConfig[UART_HW_RTS];
    }
  
        /* set directions */
    pUartDevice->PinIsInput[UART_HW_RX] = TRUE;
    pUartDevice->PinIsInput[UART_HW_CTS] = TRUE;
    pUartDevice->PinIsInput[UART_HW_TX] = FALSE;
    pUartDevice->PinIsInput[UART_HW_RTS] = FALSE;
    
    if (pConfig->pPortName != NULL) {
            /* user caller supplied name instead */
        pUartDevice->SerialDev.pPortName = pConfig->pPortName;   
    }
        
        /* disable port pins for now to prevent floating inputs */
    UartDevPortPinsEnableDisable(pUartDevice,FALSE);
    
        /* setup the serial device */                                     
    pUartDevice->SerialDev.Properties.Flags = SER_PORT_PROP_FLAGS_RX_NO_DROP;
        /* the serial layer provides ring buffers for us */     
    if (pConfig->TxBufferSize != 0) {                                                                                          
        pUartDevice->SerialDev.Properties.TXBufferSize = pConfig->TxBufferSize;
    } else {
        pUartDevice->SerialDev.Properties.TXBufferSize = SERIAL_LAYER_RING_BUFFER_SIZE;    
    }
    
    if (pConfig->RxBufferSize != 0) { 
        pUartDevice->SerialDev.Properties.RXBufferSize = pConfig->RxBufferSize;
    } else {
        pUartDevice->SerialDev.Properties.RXBufferSize = SERIAL_LAYER_RING_BUFFER_SIZE;    
    }

	//////////////////////////////////////
	pUartDevice->BufferLength = 0;
	pUartDevice->RxIsr = FALSE;
	pUartDevice->RxWakeup = FALSE;
	pUartDevice->RxIsrDiscard = FALSE;
	pUartDevice->RxMax = 0;
	pUartDevice->RxFullCount = 0;
	pUartDevice->RxDsrDiscard = 0;
	pUartDevice->RxOverFlow = 0;
	//////////////////////////////////////
    pUartDevice->SerialDev.HWTransmit = UartDevHWTransmit;
    pUartDevice->SerialDev.DrainRX = UartDevDrainRX;
    pUartDevice->SerialDev.Ioctl = UartDevIoctl;
    pUartDevice->SerialDev.pPortContext = pUartDevice;
    
    DisableUartInterrupts(pUartDevice,ALL_UART_INTS);
    
    switch (pUartDevice->InstanceId) {   
        case UART_SERP_UART0_INSTANCE_ID :
            #ifdef DBG_UART_POLLING_MODE // enable polling of uart0
            pUartDevice->Polling = TRUE;
            #else
                /* setup dsr service routine */
            A_RXINCOMING_INTR_ATTACH(GET_UART_SERP_INDIR_FN(_UartDevDsrHandler), pUartDevice);
                /* register ISR handler */
            A_RXINCOMING_ISR_ATTACH(UartDevIsrHandler, pUartDevice);
			SetInterruptEnable_AfterIsr(A_INUM_RXINCOMING);
            A_RXINCOMING_INTR_UNMASK(); 
            break;
            #endif /*DBG_UART_POLLING_MODE*/
        case UART_SERP_UART1_INSTANCE_ID:          
                /* setup dsr service routine */
            A_WUART_INTR_ATTACH(GET_UART_SERP_INDIR_FN(_UartDevDsrHandler),pUartDevice); 
                /* register ISR handler */
            A_WUART_ISR_ATTACH(UartDevIsrHandler,pUartDevice);
			SetInterruptEnable_AfterIsr(A_INUM_HCI_UART);
            A_WUART_INTR_UNMASK();     
            break;
#if defined(AR6002_REV6) || defined(AR6002_REV7)
        case UART_SERP_UART2_INSTANCE_ID :
           #ifdef DBG_UART_POLLING_MODE // enable polling of uart0
            pUartDevice->Polling = TRUE;
            #else
               /* setup dsr service routine */
            A_UART2_INTR_ATTACH(GET_UART_SERP_INDIR_FN(_UartDevDsrHandler), pUartDevice);
                /* register ISR handler */
            A_UART2_ISR_ATTACH(UartDevIsrHandler, pUartDevice);
			SetInterruptEnable_AfterIsr(A_INUM_UART2);
            A_UART2_INTR_UNMASK();
	   #endif        
            break;  
#endif              
        default:
            A_ASSERT(FALSE);
            break;    
        
    }

    if (pUartDevice->Polling) {
            /* setup polling timer */
        A_INIT_TIMER(&pUartDevice->PollingTimer,
                     GET_UART_SERP_INDIR_FN(_InterruptPollCallback),
                     pUartDevice);  
        A_TIMER_SET_FLAGS(&pUartDevice->PollingTimer,TIMER_FLAGS_NO_FORCE_DISARM);
        pUartDevice->PollTimeoutMS = INT_POLL_INTERVAL_MS;
    }

    pUartDevice->RxPinInterrupt.pin = pUartDevice->HwPins.PinList[UART_HW_RX].GpioPin;
    pUartDevice->RxPinInterrupt.gpio_pin_int_handler_fn = GET_UART_SERP_INDIR_FN(_UartRxPinInterruptHandler);
    pUartDevice->RxPinInterrupt.arg = pUartDevice;
    
        /* register the port */
    pUartDevice->hPortDev = SERPHW_RegisterPort(&pUartDevice->SerialDev);
    A_ASSERT(pUartDevice->hPortDev != NULL);

}

USAPI void _UartRxPinInterruptHandler(void *data)
{  
    UART_DEVICE  *pUartDevice = (UART_DEVICE *)data;
    
        /* disable UART wake */
    UartConfigureRxWake(pUartDevice,FALSE);   
        /* notify application, application will decide to re-enable RX wake */
    SERPHW_IndicatePortEvent(pUartDevice->hPortDev,SERIAL_EVENT_RX_WAKE,0,0);
}


static void *UartDev_Init(int InstanceId)
{
    UART_DEVICE *pUartDevice;
    
    pUartDevice = (UART_DEVICE *)A_ALLOCRAM(sizeof(UART_DEVICE));
    pUartDevice->InstanceId = InstanceId;
    
    switch (InstanceId) {   
        case UART_SERP_UART0_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_DBG_UART_BASE_ADDRESS; 
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT0_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart0DefaultPins;
            break;         
        case UART_SERP_UART1_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_UART_BASE_ADDRESS;
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT1_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart1DefaultPins;
            break;
#if defined(AR6002_REV6) || defined(AR6002_REV7)
        case UART_SERP_UART2_INSTANCE_ID :
            pUartDevice->BaseAddress = WLAN_UART2_BASE_ADDRESS;
            pUartDevice->SerialDev.pPortName = (A_CHAR *)UART_PORT2_NAME;
            pUartDevice->pDefPinConfigTable = g_Uart2DefaultPins;
            break;
#endif            
        default:
            A_ASSERT(FALSE);
            break;    
        
    }
    
//    A_ASSERT(g_pUartDevices[InstanceId] == NULL);
    g_pUartDevices[InstanceId] = pUartDevice;
    
    return (void *)pUartDevice;  
}

void UARTSERP_HW_module_install(struct uart_serphw_apis *pAPIs)
{
    pAPIs->_Init = UartDev_Init;
    pAPIs->_Start = UartDev_Start;  
    pAPIs->pReserved = &g_pUartDevices; 
}

UART_SERP_INTERNAL_APIS g_UartSerpIndirectionTable = {
    _StartStopInterruptPolling,
    _InterruptPollCallback,
    _UartTryTransmit,
    _UartReceive,
    _UartDevSetBaud,
    _CatchTxOverFlowError,
    _CatchRxOverFlowError,
    _UartDevDsrHandler,
    _UartDevPortPinsEnableDisable,
    _UartDevShutDownUart,
    _UartDevSetupUart,
    _UartRxPinInterruptHandler,
    _UartConfigureRxWake
};



