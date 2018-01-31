/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//#include <os/cmnos_api.h>
#include <osapi.h>
#include <qcom_uart.h>
#include <priv_uart.h>

A_UINT8 uart_wakeup_flag = 0;

A_UINT32 qcom_uart_init(void)
{
    int retVal = 0;
    retVal = Uart_Enumerate();
    return retVal;
}

A_INT32 qcom_single_uart_init(A_CHAR * name)
{
    int retVal = 0;
    retVal = Uart_Init((char *)name);
    return retVal;
}

A_STATUS qcom_get_uart_config(A_CHAR * name, qcom_uart_para* uartpara)
{
    int fd = 0, retVal = 0;
    UART_CFG_T uart_para;

    fd = Uart_GetFd((char *)name);
    if(fd != 0)
    {
        A_MEMSET(&uart_para, 0, sizeof(UART_CFG_T));
        retVal = Uart_GetPara(fd, &uart_para);
        uartpara->BaudRate = uart_para.Baud;
        uartpara->number = uart_para.DataBits;
        uartpara->StopBits = uart_para.StopBits;
        if((uart_para.ConfigFlags & SER_PORT_CONFIG_RTS_CTS_FLOW) != 0)
        {
            uartpara->FlowControl = 1;
        }
        else
        {
            uartpara->FlowControl = 0;
        }
        if((uart_para.ConfigFlags & SER_PORT_CONFIG_PARITY_CHECK) == 0)
        {
            uartpara->parity = 0;
        }
        else if((uart_para.ConfigFlags & SER_PORT_CONFIG_ODD_PARITY) != 0)
        {
            uartpara->parity = 1;
        }
        else
        {
            uartpara->parity = 2;
        }
        return retVal;
    }
    else
    {
        return -1;
    }
}

A_STATUS  qcom_set_uart_config(A_CHAR *name, qcom_uart_para* uartpara)
{
    UART_CFG_T uart_para;
    int fd = 0;
    fd = Uart_GetFd((char *)name);
    if(fd == 0)
    {
        return -1;
    }
    A_MEMSET(&uart_para, 0, sizeof(UART_CFG_T));
    uart_para.Baud = uartpara->BaudRate;
    uart_para.DataBits = uartpara->number;
    uart_para.StopBits = uartpara->StopBits;
    if(uartpara->FlowControl)
    {
    	uart_para.ConfigFlags |= SER_PORT_CONFIG_RTS_CTS_FLOW;
    }
    if(uartpara->parity == 1)
    {
       uart_para.ConfigFlags |= SER_PORT_CONFIG_PARITY_CHECK;
       uart_para.ConfigFlags |= SER_PORT_CONFIG_ODD_PARITY;
    }
    else if(uartpara->parity == 2)
    {
       uart_para.ConfigFlags |= SER_PORT_CONFIG_PARITY_CHECK;
    }
    return Uart_SetPara(fd, uart_para);
}

A_INT32 qcom_uart_open(A_CHAR* name)
{
    return Uart_Open((char *)name);
}
int qcom_uart_close(A_INT32 fd)
{
    return Uart_Close(fd);
}
A_INT32 qcom_uart_read(A_INT32 fd, A_CHAR * buff, A_UINT32* len)
{
    return Uart_Read(fd, (unsigned char *)buff, (int *)len);
}
A_INT32 qcom_uart_write(A_INT32 fd, A_CHAR* buff, A_UINT32* len)
{
    return Uart_Write(fd, (unsigned char *)buff, (int *)len);
}

void qcom_uart_write_direct(A_INT32 fd, A_CHAR* buff, A_UINT32* len)
{
    Uart_Write_Direct(fd, (unsigned char *)buff, (int *)len);
}

void qcom_uart_rx_pin_set(int pin0, int pin1)
{
    extern void Uart_Rx_Pin_Set(int pin0, int pin1);
    Uart_Rx_Pin_Set(pin0,pin1);
	
}
void qcom_uart_tx_pin_set(int pin0, int pin1)
{
    extern void Uart_Tx_Pin_Set(int pin0, int pin1); 
    Uart_Tx_Pin_Set(pin0,pin1);
}

void qcom_uart_wakeup_config(A_CHAR *name, A_UINT8 mode, A_UINT8 enable)
{
    if(!A_STRCMP(name,"UART0"))
    {
        if(mode == 0)
        {
            if(enable == 0)
            {
                uart_wakeup_flag &= ~UART0_RX_WAKEUP_ENABLE;
            }
            else
            {
                uart_wakeup_flag |= UART0_RX_WAKEUP_ENABLE;
            }
        }
    }
    else if(!A_STRCMP(name,"UART1"))
    {
        if(mode == 0)
        {
            if(enable == 0)
            {
                uart_wakeup_flag &= ~UART1_RX_WAKEUP_ENABLE;
            }
            else
            {
                uart_wakeup_flag |= UART1_RX_WAKEUP_ENABLE;
            }
        }
        else if(mode == 1)
        {
            if(enable == 0)
            {
                uart_wakeup_flag &= ~UART1_RTS_WAKEUP_ENABLE;
            }
            else
            {
                uart_wakeup_flag |= UART1_RTS_WAKEUP_ENABLE;
            }
        }
    }
    else if(!A_STRCMP(name,"UART2"))
    {
        if(mode == 0)
        {
            if(enable == 0)
            {
                uart_wakeup_flag &= ~UART2_RX_WAKEUP_ENABLE;
            }
            else
            {
                uart_wakeup_flag |= UART2_RX_WAKEUP_ENABLE;
            }
        }
        else if(mode == 1)
        {
            if(enable == 0)
            {
                uart_wakeup_flag &= ~UART2_RTS_WAKEUP_ENABLE;
            }
            else
            {
                uart_wakeup_flag |= UART2_RTS_WAKEUP_ENABLE;
            }
        }
    }
    
    if(uart_wakeup_flag != UART_WAKEUP_DISABLE)
    {
        Init_Multi_Uart_Wakeup();
    }
}

A_INT32 qcom_uart_set_buffer_size(A_CHAR * name, A_UINT32 tx_size, A_UINT32 rx_size)
{
    return uart_set_buffer_size((char *)name, tx_size, rx_size);
}


