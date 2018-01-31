/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#include <osapi.h>
#include "uartserp_internal.h"
#include "qcom/qcom_pwr.h"
#include "qcom/qcom_timer.h"
#include "qcom/qcom_uart.h"
#include "qcom/priv_uart.h"
#include "hw/rtc_soc_reg.h"
#include "hw/apb_athr_wlan_map.h"
#include "hw/analog_intf_athr_wlan_reg.h"
#include "hw/rtc_wlan_reg.h"
#include "hw/gpio_athr_wlan_reg.h"
#include <hw/uart_reg.h>
#include "uart_serphw_api.h"
#include "hw/gpio_reg.h"

extern A_UINT8 gNumDev ;
extern UART_DEVICE *g_pUartDevices[];
extern power_save_fn gPreSleepHandler[MAX_PWR_FN];
extern power_save_fn gPostAwakeHandler[MAX_PWR_FN];
extern A_UINT8 uart_wakeup_flag;

A_UINT8 needUartWakeup = 0;

void uart_set_wakeup_enable(int pin, A_BOOL enable);

gpio_interrupt_info_t RtsPinInterrupt;

/*Now the RTS pin of uart1 can't be used to wakeup Ruby, 
 * * when it's ready, please define this macro.
 * */
#undef RTS_WAKEUP

#ifdef RTS_WAKEUP
#if 0
void _UartRtsPinInterruptHandler(void *data)
{  
    UART_DEVICE  *pUartDevice = (UART_DEVICE *)data;
    
    gpio_deregister_pin_interrupt(&RtsPinInterrupt);
    gpio_configure_pin(pUartDevice->HwPins.PinList[UART_HW_RTS].GpioPin, 
                        pUartDevice->HwPins.PinList[UART_HW_RTS].ConfigValue, 
                        pUartDevice->PinIsInput[UART_HW_RTS],
                        GPIO_PIN_PULLNONE);

    A_UART_REG_WRITE(UART_RTSGATE_ADDRESS, 0);
    A_UART_REG_READ(UART_RTSGATE_ADDRESS);

}
#endif
#endif

void regist_power_save_handler(void (*pre_sleep_fn) (void), void (*post_awake_fn)(void))
{
    int i;
    for(i=0;i<MAX_PWR_FN;i++)
    {
        if( NULL == gPreSleepHandler[i] )
        {
            gPreSleepHandler[i] = pre_sleep_fn;
            gPostAwakeHandler[i] = post_awake_fn;
            return;
        }
    }
}

void uart_set_wakeup_enable(int pin, A_BOOL enable)
{

    unsigned int reg;
    reg = A_SOC_ADDR_READ(WLAN_GPIO_BASE_ADDRESS+WLAN_GPIO_PIN0_ADDRESS + pin*4);
    reg |= WLAN_GPIO_PIN0_WAKEUP_ENABLE_SET(enable);
    A_SOC_ADDR_WRITE(WLAN_GPIO_BASE_ADDRESS+WLAN_GPIO_PIN0_ADDRESS + pin*4, reg);
    reg = A_SOC_ADDR_READ(WLAN_GPIO_BASE_ADDRESS+WLAN_GPIO_PIN0_ADDRESS + pin*4);

    if(enable == TRUE)
    {
        //0x4114
        reg = A_RTC_SOC_REG_READ(SOC_GPIO_WAKEUP_CONTROL_ADDRESS);
        reg |= SOC_GPIO_WAKEUP_CONTROL_ENABLE_SET(1); 
        A_RTC_SOC_REG_WRITE(SOC_GPIO_WAKEUP_CONTROL_ADDRESS, reg);
        A_RTC_SOC_REG_READ(SOC_GPIO_WAKEUP_CONTROL_ADDRESS);
    }
    else
    {
        //0x4114
        reg = A_RTC_SOC_REG_READ(SOC_GPIO_WAKEUP_CONTROL_ADDRESS);
        reg &= SOC_GPIO_WAKEUP_CONTROL_ENABLE_SET(0);        
        A_RTC_SOC_REG_WRITE(SOC_GPIO_WAKEUP_CONTROL_ADDRESS, reg); 
        A_RTC_SOC_REG_READ(SOC_GPIO_WAKEUP_CONTROL_ADDRESS);
    }    
}


void uart_pre_sleep_handler(void)
{
    unsigned int powermode;

    if(needUartWakeup)
    {
        return;
    }

    if(uart_wakeup_flag > 0)
    {
        qcom_power_get_mode(0, &powermode);

        if( MAX_PERF_POWER != powermode)
        {
            needUartWakeup = 1; 

            //uart0 is debug uart, only support wake up by break signal
            if( (uart_wakeup_flag & UART0_RX_WAKEUP_ENABLE) && g_pUartDevices[0])
            {
                UartConfigureRxWake(g_pUartDevices[0] ,TRUE);
            }            
            //uart1 is HS-uart, support both break signal and legacy flow contral
            if(g_pUartDevices[1])
            {
                //config RTS pin as a GPIO input, set UART gating flag

#ifdef RTS_WAKEUP
#if 0
                if((uart_wakeup_flag & UART1_RTS_WAKEUP_ENABLE))
                {
                    gpio_configure_pin(g_pUartDevices[1]->HwPins.PinList[UART_HW_RTS].GpioPin, 
                                       g_pUartDevices[1]->HwPins.PinList[UART_HW_RTS].ConfigAsGpio, 
                                       GPIO_PIN_DIR_INPUT,
                                       GPIO_PIN_PULLNONE);

                    RtsPinInterrupt.pin = g_pUartDevices[1]->HwPins.PinList[UART_HW_RTS].GpioPin;
                    RtsPinInterrupt.gpio_pin_int_handler_fn = _UartRtsPinInterruptHandler;
                    RtsPinInterrupt.arg = g_pUartDevices[1];


                    gpio_register_pin_interrupt(&RtsPinInterrupt);
                    gpio_set_interrupt_mode(&RtsPinInterrupt,GPIO_PIN_INT_RISING_EDGE);
                    gpio_arm_interrupt(&RtsPinInterrupt);
                    A_UART_REG_WRITE(UART_RTSGATE_ADDRESS, UART_RTSGATE_EN_MASK);
                    A_UART_REG_READ(UART_RTSGATE_ADDRESS);
                }
#endif
#endif
                if(uart_wakeup_flag & UART1_RX_WAKEUP_ENABLE)
                {
                    UartConfigureRxWake(g_pUartDevices[1] ,TRUE);
                }
            } 
            if(g_pUartDevices[2])
            {
                if(uart_wakeup_flag & UART2_RX_WAKEUP_ENABLE)
                {
                    UartConfigureRxWake(g_pUartDevices[2] ,TRUE);
                }
            }

        }
    }
                            
}

void uart_post_awake_handler(void)
{
    if( !needUartWakeup )
    {
        return;
    }

    if(g_pUartDevices[0] 
        && (uart_wakeup_flag & UART0_RX_WAKEUP_ENABLE))
    {
        UartConfigureRxWake(g_pUartDevices[0] ,FALSE);
    }

     //uart1 is HS-uart, support both break signal and legacy flow contral
    if(g_pUartDevices[1] 
        && ((uart_wakeup_flag & UART1_RX_WAKEUP_ENABLE) 
            || (uart_wakeup_flag & UART1_RTS_WAKEUP_ENABLE)))
    {
        UartConfigureRxWake(g_pUartDevices[1] ,FALSE);
#ifdef RTS_WAKEUP 
#if 0
        //config RTS pin, clear UART gating flag
        gpio_deregister_pin_interrupt(&RtsPinInterrupt);
        gpio_configure_pin(g_pUartDevices[1]->HwPins.PinList[UART_HW_RTS].GpioPin, 
                           g_pUartDevices[1]->HwPins.PinList[UART_HW_RTS].ConfigValue, 
                           g_pUartDevices[1]->PinIsInput[UART_HW_RTS],
                           //TRUE,
                           GPIO_PIN_PULLNONE);
        A_UART_REG_WRITE(UART_RTSGATE_ADDRESS, 0);
        A_UART_REG_READ(UART_RTSGATE_ADDRESS);
#endif
#endif
    }

    if(g_pUartDevices[2] 
        && ((uart_wakeup_flag & UART2_RX_WAKEUP_ENABLE) 
            || (uart_wakeup_flag & UART2_RTS_WAKEUP_ENABLE)))
    {
        UartConfigureRxWake(g_pUartDevices[2] ,FALSE);
    }
    needUartWakeup=0;
}

int uart_check_connection_status(int uart_number, int delay_flag)
{
    //A_UINT8 uart0_input_status = 0;   //not check uart0
    A_UINT8 uart1_input_status = 0;
    A_UINT8 uart2_input_status = 0;
    unsigned int reg;

    if(delay_flag == 1)
    {
        //A_DELAY_USECS(10000000); 
    }

    reg = A_GPIO_REG_READ(GPIO_IN_ADDRESS);     //0x14018

    if(uart_number == 0)
    {
        #if 0
        if(reg & (1<<UART0_RX_PIN))
        {
            uart0_input_status = 1;
        }
        return uart0_input_status;
        #endif
        return 1;
    }    
    else if(uart_number == 1)
    {
        if(reg & (1<<UART1_RX_PIN))
        {
            uart1_input_status = 1;
        }
        return uart1_input_status;
    }
    else if(uart_number == 2)
    {
        if(reg & (1<<UART2_RX_PIN))
        {
            uart2_input_status = 1;
        }
        return uart2_input_status;
    }
    else
    {
        return 0;
    }

}

void uart_wakeup_test(void)
{
    unsigned int reg;	
    unsigned int reg_pre;

    A_DELAY_USECS(1000);
    
    //pre-sleep handler
    uart_pre_sleep_handler();

    //0x5030
    reg_pre = A_RTC_WLAN_REG_READ(WLAN_MAC_SLEEP_CONTROL_ADDRESS); 
    if(!WLAN_MAC_SLEEP_CONTROL_ENABLE_GET(reg_pre))
    {
        reg = A_RTC_WLAN_REG_READ(WLAN_MAC_SLEEP_CONTROL_ADDRESS); 
        reg &= ~WLAN_MAC_SLEEP_CONTROL_ENABLE_MASK;
        reg |= WLAN_MAC_SLEEP_CONTROL_ENABLE_SET(1);  // 1: forced sleep, 0: forced wakeup
        A_RTC_WLAN_REG_WRITE(WLAN_MAC_SLEEP_CONTROL_ADDRESS, reg); 
    }

    gpio_pin_dir_set(g_pUartDevices[2]->HwPins.PinList[UART_HW_RTS].GpioPin, TRUE);

    //0x524C
    /*it is uncertain whether configure 0x524c to be 2 before sleep, need more tests to verify*/
    //A_RTC_WLAN_REG_WRITE(RTC_SYNC_FORCE_WAKE_ADDRESS, 0x2);
    //A_RTC_WLAN_REG_READ(RTC_SYNC_FORCE_WAKE_ADDRESS);
    
    asm("waiti 0");	
    asm("waiti 0");	
    asm("nop");	
    asm("nop");    

    //post-sleep handler
    uart_post_awake_handler();

    gpio_pin_dir_set(g_pUartDevices[2]->HwPins.PinList[UART_HW_RTS].GpioPin, FALSE);

    if(!WLAN_MAC_SLEEP_CONTROL_ENABLE_GET(reg_pre))
    {
        reg = A_RTC_WLAN_REG_READ(WLAN_MAC_SLEEP_CONTROL_ADDRESS); 
        reg &= ~WLAN_MAC_SLEEP_CONTROL_ENABLE_MASK;
        reg |= WLAN_MAC_SLEEP_CONTROL_ENABLE_SET(0);  // 1: forced sleep, 0: forced wakeup
        A_RTC_WLAN_REG_WRITE(WLAN_MAC_SLEEP_CONTROL_ADDRESS, reg); 
    }
}

