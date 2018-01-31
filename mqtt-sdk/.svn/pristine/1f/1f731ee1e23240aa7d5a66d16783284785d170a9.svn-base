/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: console implementation to support a simple CLI
//
// @Notes:
//
//
//#ifdef CONFIG_CLI    //ychen
#include <osapi.h>
#include <serialport_api.h>
#include <serialport_ioctl.h>
#include <uart_serphw_api.h>
#include <console_api.h>
#include "console_internal.h"

static int print_help(int argc, A_CHAR *argv[]);
static int mem_read(int argc, A_CHAR *argv[]);
static int mem_write(int argc, A_CHAR *argv[]);
static int console_suspend_rx(int argc, A_CHAR *argv[]);
LOCAL void console_putc_replacement(A_UINT8 ch);
LOCAL A_BOOL console_mod_register_commands(CONSOLE_COMMAND *cmds, int count);

CONSOLE_INFO *Console_info = NULL;
A_BOOL        Console_serial_restored = FALSE;


CONSOLE_COMMAND Default_console_commands[] = {
    {.name = (A_CHAR *)"?",
     .description = (A_CHAR *)"help",        
     .execute = print_help},
    {.name = (A_CHAR *)"rd",
     .description = (A_CHAR *)"read mem : memr -a [addr] {-c [count]} {-b|-h|-w}",        
     .execute = mem_read},
    {.name = (A_CHAR *)"wr",
     .description = (A_CHAR *)"write mem : memw -a [addr] -d [data] {-b|-h|-w}",        
     .execute = mem_write},   
};

CONSOLE_COMMAND Console_uart_control = {
    .name = (A_CHAR *)"sc",
    .description = (A_CHAR *)"suspend console rx",        
    .execute = console_suspend_rx  
};
    
#define NUM_DEFAULT_CONSOLE_CMDS ((sizeof(Default_console_commands))/(sizeof(CONSOLE_COMMAND)))

//#define USE_STANDALONE_CONSOLE      // UART0- debug uart; UART1 - HCL uart, used by console

#ifndef USE_STANDALONE_CONSOLE
    //#define CONSOLE_OUTPUT  A_PRINTF_ALWAYS
    extern int app_printf(const char * format, ...);
    #define CONSOLE_OUTPUT app_printf
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

extern int cmnos_vprintf(void (*)(char ), const char *, va_list );

LOCAL void console_putc_replacement(A_UINT8 ch);
//static void (*_putc)(char c) = (void (*)(char))console_putc_replacement;

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
            
LOCAL void _refresh_console_line(void)
{
    int backup;
    
    CONSOLE_OUTPUT(ANSI_VT100_CLEAR_LINE);
        /* back up max-line worth of characters which will reset the cursor to the front of the line */
    CONSOLE_OUTPUT("\x1b[%dD",Console_info->config.max_cmdline_length);
    CONSOLE_OUTPUT(CONSOLE_PROMPT);
    CONSOLE_OUTPUT("%s", (char *)Console_info->current_line);
    
    /* the cursor will be parked at the end of the current line, back it up to reflect the current
     * cursor position */
    backup = Console_info->chars_avail - Console_info->cursor_pos;
    if (backup > 0) {
        CONSOLE_OUTPUT("\x1b[%dD",backup);    
    }
    
}

LOCAL A_BOOL _insert_console_char(A_CHAR character)
{   
    A_BOOL refresh = FALSE;
    
    if (Console_info->chars_avail >= Console_info->config.max_cmdline_length) {
        return FALSE;
    }
    
    if (Console_info->cursor_pos < Console_info->chars_avail) {
            /* move chars */
        A_MEMMOVE(&Console_info->current_line[Console_info->cursor_pos + 1],
                  &Console_info->current_line[Console_info->cursor_pos],
                  Console_info->chars_avail - Console_info->cursor_pos);
        refresh = TRUE;
    }
    
    Console_info->current_line[Console_info->cursor_pos] = character;
    Console_info->chars_avail++;
    Console_info->cursor_pos++;
    
    Console_info->current_line[Console_info->chars_avail] = '\0';
    if (refresh) {
        refresh_console_line();  
    } else {
        CONSOLE_OUTPUT("%c",character);    
    }
    return TRUE;
}

LOCAL A_BOOL _process_console_backspace(void)
{
    int     remove_index = Console_info->cursor_pos - 1;
    
    if ((Console_info->chars_avail <= 0) || (remove_index < 0)) {
        return FALSE;
    }
   
    Console_info->chars_avail--;
    
    if (Console_info->chars_avail) {
        A_MEMMOVE(&Console_info->current_line[remove_index],
                  &Console_info->current_line[remove_index + 1],
                  Console_info->chars_avail);
    }
    Console_info->cursor_pos--;
    Console_info->current_line[Console_info->chars_avail] = '\0';          
    refresh_console_line();    
    return TRUE;
}

LOCAL A_BOOL _process_console_delete(void)
{
    int     remove_index = Console_info->cursor_pos;
    
    if ((Console_info->chars_avail <= 0) || (remove_index < 0)) {
        return FALSE;
    }
   
    Console_info->chars_avail--;
    
    if (Console_info->cursor_pos < Console_info->chars_avail) {
        A_MEMMOVE(&Console_info->current_line[remove_index],
                  &Console_info->current_line[remove_index + 1],
                  Console_info->chars_avail);
    }
    Console_info->cursor_pos--;
    if (Console_info->cursor_pos < 0) {
        Console_info->cursor_pos = 0;    
    }
    Console_info->current_line[Console_info->chars_avail] = '\0';
    refresh_console_line();    
    return TRUE;
}

LOCAL A_BOOL _move_console_cursor(A_BOOL forward)
{
    if (forward) {
        if (Console_info->cursor_pos >= Console_info->chars_avail) {
            return FALSE;
        }    
        Console_info->cursor_pos++;
    } else {
        if ((Console_info->cursor_pos - 1) < 0) {
            return FALSE;
        }         
        Console_info->cursor_pos--;   
    }
    
    return TRUE;
}

LOCAL A_CHAR *_get_console_history_buf(int index)
{
    return Console_info->history_buffer + Console_info->history_length*index;
}

static void execute_console_cmd(int argc, A_CHAR *argv[])
{
    A_BOOL          found = FALSE;
    CONSOLE_COMMAND *cmd;
    
    cmd = Console_info->command_list;
    
    while (cmd != NULL) {
        if (A_STRCMP(cmd->name,(const char *)argv[0]) == 0) {
			if (0 != A_STRNCMP(cmd->name, "iwconfig ", 8))
			{
            	CONSOLE_OUTPUT("\n");
			}
            cmd->execute(argc,argv);
            found = TRUE;
            break;  
        }
        cmd = cmd->next;    
    }
    
    if (!found) {
        CONSOLE_OUTPUT("\n\n** cmd : %s - not found\n",argv[0]);   
    }
    
}

LOCAL void _process_console_line(void)
{
    A_CHAR  *history;
    int     argc;
    A_CHAR  *ptr;
    A_BOOL  found_start;
    A_BOOL  found_end;
	A_BOOL  prompt_flg = 0;
    
    if (Console_info->chars_avail) {
        Console_info->current_line[Console_info->chars_avail] = '\0';
            /* save current line to history buffer */
        history = get_console_history_buf(Console_info->history_write_index);
        A_STRCPY(history,Console_info->current_line);
		
		if ((0 == A_STRNCMP(history, "iwconfig ", 8)) || (0 == A_STRNCMP(history, "wmiconfig --connect", 19))
		 || (0 == A_STRNCMP(history, "wmiconfig --disc", 16)))
		{
		  prompt_flg = 1;
		}
		
        Console_info->history_write_index++;
        if (Console_info->history_write_index >= Console_info->config.max_history) {
            Console_info->history_write_index = 0;    
        }
        
        argc = 0;
        ptr = Console_info->current_line;
        found_start = FALSE; 
        found_end = TRUE;         
        A_MEMZERO(&Console_info->argv_buffers,sizeof(Console_info->argv_buffers));
           
        while ((*ptr != '\0') && (argc < CONSOLE_DEF_MAX_ARGC)) {
            if (!found_start) {
                if (*ptr != ' ') {
                    found_start = TRUE; 
                    found_end = FALSE;
                    Console_info->argv_buffers[argc] = ptr;      
                } 
            } else {
                if (*ptr == ' ') {
                    *ptr = '\0';
                    argc++;   
                    found_start = FALSE;
                    found_end = TRUE;
                }
            }
            ptr++;    
        }    
        
        if ((argc < CONSOLE_DEF_MAX_ARGC) && found_start && !found_end) {
            argc++;    
        }
        
        if (argc) {
            execute_console_cmd(argc,Console_info->argv_buffers);
        }
    }
    
    Console_info->chars_avail = 0;
    Console_info->cursor_pos = 0;
	
	if (prompt_flg == 1)
	{
		return;
	}
	
    if (!(Console_info->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
        CONSOLE_OUTPUT(CONSOLE_PROMPT_LINE);
    }
}

LOCAL void move_next_history_read_index(A_BOOL Up)
{
    if (Up) {
        Console_info->history_read_index++;
        if (Console_info->history_read_index >= Console_info->config.max_history) {
            Console_info->history_read_index = 0;    
        }
    } else { 
        Console_info->history_read_index--; 
        if (Console_info->history_read_index < 0) {
            Console_info->history_read_index = (Console_info->config.max_history - 1);  
        }   
    }
}

LOCAL void _show_console_history(A_BOOL Up)
{
    int    len;
    A_CHAR *history;
    int    i; 
    
        /* find the next non-empty command-line history buffer, the direction of
         * searching is indicated */
    for (i = 0; i < Console_info->config.max_history; i++) {
        history = get_console_history_buf(Console_info->history_read_index);
        len = A_STRLEN(history);
        if (len > 0) {
            A_STRCPY(Console_info->current_line,history);
            Console_info->chars_avail = len;
            Console_info->cursor_pos = Console_info->chars_avail;
            refresh_console_line();
            move_next_history_read_index(Up);
            break;    
        }  
            /* keep looking until we wrap back around */
        move_next_history_read_index(Up);
    }

    if (i == Console_info->config.max_history) {
        CONSOLE_OUTPUT(ASCII_BELL);        
    }
}

void _console_process_char(void)
{
    A_UINT8  character;
    int      length;
    int      bytes_avail;
    A_BOOL   extended = FALSE;
    
    while (TRUE) {
        
        length = 1;
        
        bytes_avail = SERP_Read(Console_info->port,(A_UINT8 *)&character,&length);

        if (length == 0) {
            break;    
        }
        
        //CONSOLE_OUTPUT("%2.2X\n",character);
        
        if (Console_info->flags & CONSOLE_FLAGS_RX_SUSPENDED) {
            if (character == ASCII_CARRIAGE_RETURN) {
                    /* activate the console when the first enter key is pressed */
                Console_info->flags &= ~CONSOLE_FLAGS_RX_SUSPENDED;
                if (!(Console_info->flags & CONSOLE_FLAGS_USE_HW_RX_SUSP)) {              
                        /* switch to fast polling in the port */
                    SERP_Ioctl(Console_info->port,
                               SER_PORT_HW_IOCTL_SET_POLL_INTERVAL,
                               NULL,
                               CONSOLE_FAST_POLL_INTERVAL_MS);
                }                         
            } else {
                continue;    
            }
        } 
        
        if (Console_info->current_escape_pos > 0) {
            /* in an escape sequence */  
            Console_info->escape_buffer[Console_info->current_escape_pos] = character;
            Console_info->current_escape_pos++;  
            if (Console_info->current_escape_pos >= MIN_ESCAPE_BYTES) {                              
                if (Console_info->escape_buffer[1] == '[') {
                    switch (Console_info->escape_buffer[2]) {
                        case 'A' :
                            /* cursor up */
                            show_console_history(TRUE);
                            break;
                        case 'B' :
                            /* cursor down */
                            show_console_history(FALSE);
                            break;                           
                        case 'C' :
                            /* cursor FORWARD */
                            if (!move_console_cursor(CURSOR_FORWARD)){
                                CONSOLE_OUTPUT(ASCII_BELL);    
                            } else {
                                CONSOLE_OUTPUT(ANSI_VT100_CURSOR_FWD);   
                            }
                            break;
                        case 'D' :
                            /* cursor BACKWARDS */
                            if (!move_console_cursor(CURSOR_BACKWARD)) {
                                CONSOLE_OUTPUT(ASCII_BELL);      
                            } else {
                                CONSOLE_OUTPUT(ANSI_VT100_CURSOR_BACK);
                            }
                            break;
                        case '4' :
                            if (Console_info->current_escape_pos <= MIN_ESCAPE_BYTES) {
                                extended = TRUE;
                            } else {
                                if (Console_info->escape_buffer[3] == 0x7e) {
                                        /* delete key */
                                    if (!process_console_delete()) {
                                        CONSOLE_OUTPUT(ASCII_BELL);      
                                    }          
                                }    
                            }
                            break;    
                        default:
                            break;    
                    }
                }
                
                if (!extended) {                
                    Console_info->current_escape_pos = 0;
                }    
            }
            continue;
        }
        
        if ((character >= 0x20) && (character <= 0x7E)) {            
            if (!insert_console_char(character)) {
                CONSOLE_OUTPUT(ASCII_BELL);       
            }                      
            continue;   
        }
        
        switch (character) {
            
            case ASCII_CARRIAGE_RETURN :
                if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ) {
                    A_POWER_SYSSLEEP_CONTROL(1); /* Permit sleep while processing cmd */
                }
                process_console_line();
                if (Console_info->config.flags & CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ) {
                    A_POWER_SYSSLEEP_CONTROL(0); /* disallow sleep while waiting for keystrokes */
                }
                if (Console_info->flags & CONSOLE_FLAGS_ALLOW_RX_SUSPEND) {
                    schedule_console_idle_recv_timer();                
                }
                break;
            case ASCII_BACK_SPACE :
                if (!process_console_backspace()) {
                    CONSOLE_OUTPUT(ASCII_BELL);    
                }  
                break;
            case ASCII_ESCAPE :
                Console_info->escape_buffer[0] = character;
                Console_info->current_escape_pos = 1;
                break;
            default :
                break;
        }
       
    }  
}

static void console_serial_event_callback(void *context, A_UINT16 event, A_UINT32 arg0, A_UINT32 arg1)
{
#if 1 //#ifdef RTOS_TX_BLOCKING
    extern void wakeup_task_execute_cli_cmd(void);          
#endif
    switch (event) {
        case SERIAL_EVENT_RX_AVAIL :
#if 0 //#ifndef RTOS_TX_BLOCKING
            console_process_char();   
#else
            wakeup_task_execute_cli_cmd();
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
            wakeup_task_execute_cli_cmd();
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

LOCAL void console_mod_init(CONSOLE_CONFIG *config)
{
    UART_HW_CONFIG  uart_config;
    void            *phys_port;
    int             i;
    
    if (Console_info != NULL) {
        return;    
    }
    
    Console_info = (CONSOLE_INFO *)A_ALLOCRAM(sizeof(CONSOLE_INFO));
    
    if (config != NULL) {    
        Console_info->config = *config;
    }
    
    if (0 == Console_info->config.max_cmdline_length) {
        Console_info->config.max_cmdline_length = CONSOLE_DEF_MAX_LINE_CHARS;   
    }    
    if (0 == Console_info->config.max_history) {
        Console_info->config.max_history = CONSOLE_DEF_MAX_HISTORY;    
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
    
    for (i = 0; i < CONSOLE_DEF_MAX_ARGC; i++) {
        Console_info->argv_buffers[i] = (A_CHAR *)A_ALLOCRAM(Console_info->config.max_argv_len + 1);
    }
    
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

LOCAL void console_mod_start(void)
{   
    SERIAL_PORT_PROPERTIES  port_properties;
    SER_PORT_UART_CONFIG    port_config;
    A_STATUS                status;
    
#ifdef CONFIG_CLI
    console_register_commands(Default_console_commands,NUM_DEFAULT_CONSOLE_CMDS);
#else
    console_mod_register_commands(Default_console_commands,NUM_DEFAULT_CONSOLE_CMDS);
#endif
       
        /* open the logical port */
    Console_info->port = SERP_Open(Console_info->config.logical_port_name,
                                   NULL,
                                   console_serial_event_callback,
                                   &port_properties);
   
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
#ifdef CONFIG_CLI
        console_register_commands(&Console_uart_control,1);
#else
        console_mod_register_commands(&Console_uart_control,1);  //ychen
#endif
    }
    
#ifndef USE_STANDALONE_CONSOLE
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    
#endif
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_getc = console_getc_replacement;
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_restore_funcs = console_restore_replacement;
    CONSOLE_OUTPUT("\nCLI:");

	Console_info->flags = 0;
    CONSOLE_OUTPUT(CONSOLE_PROMPT_LINE);
}

LOCAL void insert_console_command(CONSOLE_COMMAND *cmd)
{
    cmd->next = Console_info->command_list;
    Console_info->command_list = cmd;    
}

LOCAL A_BOOL console_mod_register_commands(CONSOLE_COMMAND *cmds, int count)
{
    int i;
    
    if (Console_info == NULL) {
        return FALSE;    
    }
    
    for (i = 0; i < count; i++) {
        insert_console_command(&cmds[i]);
    }
    
    return TRUE;
}

LOCAL void console_module_install(struct console_apis *pAPIs)
{
    pAPIs->_init = console_mod_init;
    pAPIs->_register_commands = console_mod_register_commands;
    pAPIs->_start = console_mod_start;
}


CONSOLE_INTERNAL_APIS ConsoleIndirectionTable = {
    _refresh_console_line,
    _insert_console_char,
    _process_console_backspace,
    _process_console_delete,
    _move_console_cursor,
    _get_console_history_buf,
    _process_console_line,
    _show_console_history,
    _console_process_char,
    _do_memory_op,
    _get_mem_access_options
};

/****** built-in console commands ********/

static int print_help(int argc, A_CHAR *argv[])
{
    CONSOLE_COMMAND *cmd;
    
    cmd = Console_info->command_list;
    
    CONSOLE_OUTPUT("\nCommand List:\n\n");
    
    while (cmd != NULL) {
        CONSOLE_OUTPUT("%s - %s\n",cmd->name,cmd->description);  
        cmd = cmd->next;    
    }
    
    return 0;
}

LOCAL int _do_memory_op(void *pAddress, A_BOOL read, A_UINT32 write_value, MEMORY_ACCESS_WIDTH width)
{
    A_UINT32 read_value = 0;
    int      incr = sizeof(A_UINT32);
    
    switch (width) {
        case MEMORY_ACCESS_BYTE :
            if (read) {
                read_value = *(volatile A_UINT8 *)pAddress;    
            } else {
                *(volatile A_UINT8 *)pAddress = (A_UINT8)write_value;   
            }
            incr = sizeof(A_UINT8);
            break;
        case MEMORY_ACCESS_SHORT :
            if (read) {
                read_value = *(volatile A_UINT16 *)pAddress;    
            } else {
                *(volatile A_UINT16 *)pAddress = (A_UINT16)write_value;   
            }
            incr = sizeof(A_UINT16);
            break;
        case MEMORY_ACCESS_WORD :
            if (read) {
                read_value = *(volatile A_UINT32 *)pAddress;    
            } else {
                *(volatile A_UINT32 *)pAddress = (A_UINT32)write_value;   
            }
            break;
        default :
            break;     
    }
    
    if (read) {
        switch (width) {
            case MEMORY_ACCESS_BYTE :
                CONSOLE_OUTPUT("0x%X : 0x%2.2X\n",(A_UINT32)pAddress,read_value);  
                break;
            case MEMORY_ACCESS_SHORT :
                CONSOLE_OUTPUT("0x%X : 0x%4.4X\n",(A_UINT32)pAddress,read_value); 
                break;
            case MEMORY_ACCESS_WORD :
                CONSOLE_OUTPUT("0x%X : 0x%8.8X\n",(A_UINT32)pAddress,read_value); 
                break;
            default :
                break;     
        }    
    }
    
    return incr;
}

LOCAL int _get_mem_access_options(int argc, A_CHAR *argv[], MEMORY_ACCESS_OPTIONS *options)
{
    int         i;
    
    options->width = MEMORY_ACCESS_WORD;     
    options->count = 1;
       
    for (i = 1; i < argc; i++) {
        if ((A_STRCMP(argv[i],"-a") == 0) && ((i + 1) < argc)) {
            options->address = A_ATOUL(argv[i + 1]);
            options->flags |= ADDRESS_OPTION;         
        }  
        
        if (A_STRCMP(argv[i],"-b") == 0) {
            options->width = MEMORY_ACCESS_BYTE;        
        }
        
        if (A_STRCMP(argv[i],"-h") == 0) {
            options->width = MEMORY_ACCESS_SHORT;        
        }
        
        if (A_STRCMP(argv[i],"-w") == 0) {
            options->width = MEMORY_ACCESS_WORD;        
        }
        
        if ((A_STRCMP(argv[i],"-c") == 0) && ((i + 1) < argc)) {
             options->count = A_ATOUL(argv[i + 1]);         
        }  
        
        if ((A_STRCMP(argv[i],"-d") == 0) && ((i + 1) < argc)) {
             options->write_val = A_ATOUL(argv[i + 1]);
             options->flags |= WRITE_VALUE_OPTION;          
        }  
    }
    
    if (!(options->flags & ADDRESS_OPTION)) {
        CONSOLE_OUTPUT("Missing Address\n");
        return -1;    
    }
    return 0;
}


static int mem_read(int argc, A_CHAR *argv[])
{
    MEMORY_ACCESS_OPTIONS options;
    int                   i;
    int                   incr;
    
    A_MEMZERO(&options,sizeof(options));
    
    if (get_mem_access_options(argc,argv,&options) < 0) {
        return -1;    
    }
    
    for (i = 0; i < options.count; i++) {
        incr = do_memory_op((void *)(options.address),TRUE,0,options.width);
        options.address += (A_UINT32)incr;
    }
    
    return 0;   
}

static int mem_write(int argc, A_CHAR *argv[])
{
    MEMORY_ACCESS_OPTIONS options;
    
    A_MEMZERO(&options,sizeof(options));
    
    if (get_mem_access_options(argc,argv,&options) < 0) {
        return -1;    
    }
    
    if (!(options.flags & WRITE_VALUE_OPTION)) {
        CONSOLE_OUTPUT("Missing Write Data\n");
        return -1;    
    }
    
    do_memory_op((void *)options.address,FALSE,options.write_val,options.width);
    
    return 0;
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
                   3); //3 ms is default PollTimeoutMS, better than 10ms
                   //CONSOLE_SLOW_POLL_INTERVAL_MS);
    }
                   
    return 0;    
}

//#endif
