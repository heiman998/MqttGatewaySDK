/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: console implementation to support at cmd
//
// @Notes:
//
//
#include <osapi.h>
#include <serialport_api.h>
#include <serialport_ioctl.h>
#include <uart_serphw_api.h>
#include <console_api.h>
#include "at_console_internal.h"

static int console_suspend_rx(int argc, A_CHAR *argv[]);
LOCAL void console_putc_replacement(A_UINT8 ch);
LOCAL A_BOOL console_mod_register_commands(CONSOLE_COMMAND *cmds, int count);

CONSOLE_INFO *Console_info = NULL;
A_BOOL        Console_serial_restored = FALSE;
void *g_uart_port;


/* Here we define this macro: USE_STANDALONE_CONSOLE   */
/* then we can bypass the printf and debug info in rom */
/* we can use console_printf() now                     */
// #define USE_STANDALONE_CONSOLE      // UART0- debug uart; UART1 - HCL uart, used by console

#ifndef USE_STANDALONE_CONSOLE
    #define CONSOLE_OUTPUT  A_PRINTF_ALWAYS
#else
#if __XCC__ < 8000
    #include "stdarg.h"
    #define va_list __gnuc_va_list
#else
    #define va_list __builtin_va_list
    #define va_arg __builtin_va_arg
    #define va_start __builtin_va_start
    #define va_end __builtin_va_end
    #define va_copy __builtin_va_copy
#endif

extern int cmnos_vprintf(void (*putc)(char c), const char *fmt, va_list ap);

LOCAL void console_putc_replacement(A_UINT8 ch);


void console_putc(unsigned char ch) 
{
    if (ch == '\n') {
        console_putc_replacement('\r');
        console_putc_replacement('\n');
    } else if (ch == '\r') {
    } else {
        console_putc_replacement(ch);
    }
}

static void (*_putc)(char c) = (void (*)(char))console_putc;

int
console_printf(const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);

    ret = cmnos_vprintf(_putc, fmt, ap);

    va_end(ap);

    return (ret);
}

#define CONSOLE_OUTPUT  console_printf
#endif   //USE_STANDALONE_CONSOLE

LOCAL void schedule_console_idle_recv_timer(void)
{
    A_UNTIMEOUT(&Console_info->recv_idle_timer);
    A_TIMEOUT_MS(&Console_info->recv_idle_timer,Console_info->config.idle_timeout_ms,FALSE);
}
            
/*read a char from uart and call the at process function*/
void _at_process_char(void)
{
    A_UINT8  character;
    int      length;
    int      bytes_avail;
    
    while (TRUE) {
        
        length = 1;
        
        bytes_avail = SERP_Read(Console_info->port,(A_UINT8 *)&character,&length);

        if (length == 0) {
            break;    
        }
		
        switch (character) {
            
            case ASCII_CARRIAGE_RETURN :
                if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ) {
                    A_POWER_SYSSLEEP_CONTROL(1); /* Permit sleep while processing cmd */
                }
                extern void process_at_character(A_UINT8 character);
                process_at_character(character);
                if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ) {
                    A_POWER_SYSSLEEP_CONTROL(0); /* disallow sleep while waiting for keystrokes */
                }
                break;
            default :
                process_at_character(character);
                break;
        }
    }
}

static void at_console_serial_event_callback(void *context, A_UINT16 event, A_UINT32 arg0, A_UINT32 arg1)
{
#if 1 //#ifdef RTOS_TX_BLOCKING
    extern void wakeup_task_execute_at_cmd(void);          
#endif
    switch (event) {
        case SERIAL_EVENT_RX_AVAIL :
#if 0 //#ifndef RTOS_TX_BLOCKING
            console_process_char();   
#else
            wakeup_task_execute_at_cmd();
#endif
            break;
        case SERIAL_EVENT_RX_WAKE :
                /* this is a one-shot event, it will not occur until we
                 * re-enable RX wake in the uart driver  */
            Console_info->flags &= ~CONSOLE_FLAGS_RX_WAKE_ENABLED;
            schedule_console_idle_recv_timer();
#if 0 //#ifndef RTOS_TX_BLOCKING
            console_process_char();
#else
            wakeup_task_execute_at_cmd();
#endif
            break;
        default:
            break;    
    }
    
}

LOCAL int console_getc_replacement(A_UINT8 *pch)
{
    return 0;
}

LOCAL void console_putc_replacement(A_UINT8 ch)
{
    int len = 1;
    
    if (Console_serial_restored || (Console_info->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
            /* direct UART method */
        SERP_Ioctl(Console_info->port,
                   SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT,
                   &ch,
                   1); 
    } else {
            /* buffered UART */
        SERP_Write(Console_info->port,&ch,&len);
    }
}

LOCAL void console_recv_idle_timeout(A_HANDLE alarm, void *data)
{
    if (!Console_info->chars_avail) {
        /* console went idle */    
        console_suspend_rx(0,NULL);
    }   
}

LOCAL void at_console_mod_init(CONSOLE_CONFIG *config)
{
    UART_HW_CONFIG  uart_config;
    void            *phys_port;
    
    if (Console_info != NULL) {
        return;    
    }
    
    Console_info = (CONSOLE_INFO *)A_ALLOCRAM(sizeof(CONSOLE_INFO));
    
    if (config != NULL) {    
        Console_info->config = *config;
    }
    
    if (0 == Console_info->config.phys_uart_id) {
        Console_info->config.phys_uart_id = UART_SERP_UART0_INSTANCE_ID;
        if (NULL == Console_info->config.logical_port_name) {
            Console_info->config.logical_port_name = (A_CHAR *)UART_PORT0_NAME;
        }        
        if (0 == Console_info->config.baud) {
            Console_info->config.baud = HOST_INTEREST->hi_desired_baud_rate;    
        }
    }    
    if (0 == Console_info->config.baud) {
        Console_info->config.baud = CONSOLE_DEF_BAUDRATE;      
    } 
    if (0 == Console_info->config.max_argv_len) {
        Console_info->config.max_argv_len = CONSOLE_DEF_MAX_ARGV_LENGTH;    
    }
    if (0 == Console_info->config.output_buffer_size) {
        Console_info->config.output_buffer_size = CONSOLE_TRANSMIT_BUFFER_SIZE;    
    }   
    if (0 == Console_info->config.idle_timeout_ms) {
        Console_info->config.idle_timeout_ms = CONSOLE_RECV_IDLE_TIMEOUT_MS;
    }
    
    Console_info->history_length = Console_info->config.max_cmdline_length + 1;   
    Console_info->history_buffer = (A_CHAR *)A_ALLOCRAM(Console_info->history_length * 
                                                        Console_info->config.max_history);   
    Console_info->current_line = (A_CHAR *)A_ALLOCRAM(Console_info->config.max_cmdline_length + 1);
    
        /* setup the physical modem */
    phys_port = UARTSERP_HW_Init(Console_info->config.phys_uart_id);
    A_ASSERT(phys_port != NULL);  
    A_MEMZERO(&uart_config,sizeof(uart_config));        
        /* for the console, set UART buffering parameters
         * for recv we can use a fixed value to accommodate very fast typists */
    uart_config.RxBufferSize = CONSOLE_RECV_BUFFER_SIZE;
        /* use a value that is an override */
    uart_config.TxBufferSize = Console_info->config.output_buffer_size;
    UARTSERP_HW_Start(phys_port,&uart_config);
    
    A_INIT_TIMER(&Console_info->recv_idle_timer,
                 console_recv_idle_timeout,
                 NULL);
                 
    A_TIMER_SET_FLAGS(&Console_info->recv_idle_timer,TIMER_FLAGS_NO_FORCE_DISARM);  
}

LOCAL void console_restore_replacement(void)
{
        /* system encountered an exception and restored the serial port,
         * set the flag to handle A_PRINTF outputs */
    Console_serial_restored = TRUE;    
}

LOCAL void at_console_mod_start(void)
{   
    SERIAL_PORT_PROPERTIES  port_properties;
    SER_PORT_UART_CONFIG    port_config;
    A_STATUS                status;
       
        /* open the logical port */
    Console_info->port = SERP_Open(Console_info->config.logical_port_name,
                                   NULL,
                                   at_console_serial_event_callback,
                                   &port_properties);
   
    g_uart_port = Console_info->port;

    A_MEMZERO(&port_config,sizeof(port_config));

    port_config.Baud = Console_info->config.baud;
    port_config.DataBits = 8;
    port_config.StopBits = 1;
    
        /* configure port */
    status = SERP_Ioctl(Console_info->port,
                        SER_PORT_IOCTL_CONFIG_UART_PORT,
                        &port_config,
                        sizeof(port_config));
    A_ASSERT(A_SUCCESS(status));
    
        /* set receiver threshold to 1 character */
    SERP_Ioctl(Console_info->port,SER_PORT_IOCTL_SET_RX_THRESH,NULL,1);
          
    if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_ALLOW_RX_SUSPEND) {
        Console_info->flags |= CONSOLE_FLAGS_ALLOW_RX_SUSPEND; 
    }
        
    if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_USE_HW_RX_WAKE) {
            /* try using hardware for RX wake */
        Console_info->flags |= CONSOLE_FLAGS_USE_HW_RX_SUSP;       
    }  

    if ((UART_SERP_UART0_INSTANCE_ID == Console_info->config.phys_uart_id) ||
        (Console_info->flags & CONSOLE_FLAGS_ALLOW_RX_SUSPEND)) {  
        console_suspend_rx(0,NULL);
            /* install uart control commands */           
    }
    
#ifndef USE_STANDALONE_CONSOLE
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    
#endif
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_getc = console_getc_replacement;
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_restore_funcs = console_restore_replacement;
    CONSOLE_OUTPUT("\nAT CMD:<ENTER>\n");
}

LOCAL void at_console_module_install(struct console_apis *pAPIs)
{
    pAPIs->_init = at_console_mod_init;
    pAPIs->_start = at_console_mod_start;
}

static int console_suspend_rx(int argc, A_CHAR *argv[])
{
    Console_info->flags |= CONSOLE_FLAGS_RX_SUSPENDED;
    
    A_UNTIMEOUT(&Console_info->recv_idle_timer);
     
    if (Console_info->flags & CONSOLE_FLAGS_USE_HW_RX_SUSP) {
            /* use hardware based wake mechanism */
        if (!(Console_info->flags & CONSOLE_FLAGS_RX_WAKE_ENABLED)) {   
            Console_info->flags |= CONSOLE_FLAGS_RX_WAKE_ENABLED;
            SERP_Ioctl(Console_info->port,
                       SER_PORT_HW_IOCTL_CONFIG_RX_WAKEUP,
                       NULL,
                       1);
        }
    } else { 
            /* use slow polling mechanism to simulate the rx suspending */
        SERP_Ioctl(Console_info->port,
                   SER_PORT_HW_IOCTL_SET_POLL_INTERVAL,
                   NULL,
                   CONSOLE_SLOW_POLL_INTERVAL_MS);
    }
                   
    return 0;    
}

int at_console_write(A_UINT8 * pBuffer, int *pWriteLength){
	 return SERP_Write(Console_info->port, pBuffer, pWriteLength );
}

int at_console_read(A_UINT8 * pBuffer, int *pReadLength){
	 return SERP_Read(Console_info->port, pBuffer, pReadLength );
}

int at_console_ioctl_baud(A_UINT32 Baud, A_UINT8 DataBits, A_UINT8 StopBits, A_UINT16 ConfigFlags){
	
	SER_PORT_UART_CONFIG port_config;
        port_config.Baud = Baud;
        port_config.DataBits = DataBits;
        port_config.ConfigFlags= ConfigFlags;
        port_config.StopBits = StopBits;

	return SERP_Ioctl(Console_info->port, SER_PORT_IOCTL_CONFIG_UART_PORT, &port_config, sizeof (port_config));
}
