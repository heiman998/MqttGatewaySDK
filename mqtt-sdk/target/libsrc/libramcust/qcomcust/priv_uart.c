/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#include <osapi.h>
#include <qcom/priv_uart.h>
#include <serialport_api.h>
#include <serialport_ioctl.h>
#include <uart_serphw_api.h>
#include <qcom/select_api.h>
#include <uartserp_internal.h>
#include <serial_internal.h>

#define DEBUG_UART                      0
#define CONSOLE_FLAGS_RX_SUSPENDED     (1 << 0)
#define CONSOLE_FLAGS_ALLOW_RX_SUSPEND (1 << 1)
#define CONSOLE_FLAGS_USE_HW_RX_SUSP   (1 << 2)
#define CONSOLE_FLAGS_RX_WAKE_ENABLED  (1 << 3)

#define CONSOLE_RECV_BUFFER_DEFAULT_SIZE        2048
#define CONSOLE_SEND_BUFFER_DEFAULT_SIZE        2048
#define CONSOLE_SLOW_POLL_INTERVAL_MS   1000
#define DEFAULT_BAUD                    115200

#define TX_SYS_DECL_PROTECT(flag)
#define TX_SYS_PROTECT(flag)
#define TX_SYS_UNPROTECT(flag)

typedef struct _CONSOLE_INFO_PRIV {
    void            *port;
    A_UINT32        flags;
} CONSOLE_INFO_PRIV;

typedef struct _CONSOLE_INFO {    
    CONSOLE_COMMAND *command_list;
    void            *port;
    A_CHAR          *history_buffer;
    int             history_length;
    int             history_write_index;
    int             history_read_index;
    CONSOLE_CONFIG  config;
    A_CHAR          *current_line;
    int             chars_avail;
    int             cursor_pos;
    A_CHAR          escape_buffer[4];
    int             current_escape_pos;
    A_CHAR          *argv_buffers[16];
    A_UINT32        flags;
    A_timer_t       recv_idle_timer;
} CONSOLE_INFO;


static CONSOLE_INFO_PRIV *Console_uart_info[MAX_UART_SUPPORT] = {NULL,NULL,NULL};
static A_BOOL        Console_serial_restored = FALSE;

static void* uart_handle[MAX_UART_SUPPORT];
static int uart_print_id=0;
static UART_CFG_T uart_data[MAX_UART_SUPPORT];
int uart_init = 0;
A_UINT8 sdk_shell_flag = 0;
UART_START_FLAG uart_start_number = UART_TERMINATE_MASK;
typedef struct _UART_BUFFER {
    A_INT32     tx_buffer;
    A_INT32     rx_buffer;
} UART_BUFFER;

static UART_BUFFER uart_buffer[MAX_UART_SUPPORT] = {
    {CONSOLE_SEND_BUFFER_DEFAULT_SIZE, CONSOLE_RECV_BUFFER_DEFAULT_SIZE}, 
    {CONSOLE_SEND_BUFFER_DEFAULT_SIZE, CONSOLE_RECV_BUFFER_DEFAULT_SIZE}, 
    {CONSOLE_SEND_BUFFER_DEFAULT_SIZE, CONSOLE_RECV_BUFFER_DEFAULT_SIZE} };


A_BOOL serial_port_start = FALSE;

int Uart_read_null(int fd);
extern UART_RX_DETECT uart_rx_detect;
extern CONSOLE_INFO *Console_info;


#if DEBUG_UART
static void console_putc_replacement(A_UINT8 ch);
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

static void console_putc_replacement(A_UINT8 ch);

static void console_putc(unsigned char ch) 
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
uart_dbg(const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);

    ret = cmnos_vprintf(_putc, fmt, ap);

    va_end(ap);

    return (ret);
}

static int console_getc_replacement(A_UINT8 *pch)
{
    return 0;
}

static void console_putc_replacement(A_UINT8 ch)
{
    int len = 1;
    
    if (Console_serial_restored || (Console_uart_info[1]->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
            /* direct UART method */
        SERP_Ioctl(Console_uart_info[1]->port,
                   SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT,
                   &ch,
                   1); 
    } else {
            /* buffered UART */
        SERP_Write(Console_uart_info[1]->port,&ch,&len);
    }
}

static void console_restore_replacement(void)
{
        /* system encountered an exception and restored the serial port,
         * set the flag to handle A_PRINTF outputs */
    Console_serial_restored = TRUE;    
}
#else
static void console_putc_replacement(A_UINT8 ch);
static void console_putc_replacement(A_UINT8 ch)
{
#if 0
    int len = 1;
    if (Console_serial_restored || (Console_uart_info[1]->flags & CONSOLE_FLAGS_RX_SUSPENDED)) {
            /* direct UART method */
        SERP_Ioctl(Console_uart_info[1]->port,
                   SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT,
                   &ch,
                   1); 
    } else {
            /* buffered UART */
        SERP_Write(Console_uart_info[1]->port,&ch,&len);
    }
#else
    if (!A_SERIAL_ENABLED()) 
        return;

    Console_serial_restored = TRUE;
    SERP_Ioctl(Console_uart_info[uart_print_id]->port, SER_PORT_HW_IOCTL_WRITE_BYTES_DIRECT, &ch, 1); 
#endif
}

#endif  //DEBUG_UART

static void _console_process_char(void)
{
#if 0
    A_UINT8  character;
    int      length;
    int      bytes_avail;
    
    while (TRUE) {
        
        bytes_avail = SERP_Read(Console_uart_info->port,(A_UINT8 *)&character,&length);
                
        if (length == 0) {
            break;    
        } else {
            uart_dbg("recv -- %d\n", length);
	    break;
	}
        
    }  
#endif    
}

static void console_serial_event_callback(void *context, A_UINT16 event, A_UINT32 arg0, A_UINT32 arg1)
{
    switch (event) {
        case SERIAL_EVENT_RX_AVAIL :
            _console_process_char();   
            break;
        case SERIAL_EVENT_RX_WAKE :
                /* this is a one-shot event, it will not occur until we
                 * re-enable RX wake in the uart driver  */
            Console_uart_info[1]->flags &= ~CONSOLE_FLAGS_RX_WAKE_ENABLED;
            _console_process_char();
            break;
        default:
            break;    
    }
    
}

int uart_set_buffer_size(char *name, A_UINT32 tx_size, A_UINT32 rx_size)
{
    int phys_uart_id = -1;

    if(tx_size == 0) 
    {
        tx_size = CONSOLE_SEND_BUFFER_DEFAULT_SIZE;        
    }
    if(rx_size == 0)
    {
        rx_size = CONSOLE_RECV_BUFFER_DEFAULT_SIZE;
    }
    
    if (A_STRCMP(name, "UART0") == 0) 
    {
        phys_uart_id = 0;
        //if uart0 was initialized, buffer set failed
        if(uart_start_number & UART0_START_MASK)
        {
            return -1;
        }
    } 
    else if (A_STRCMP(name, "UART1") == 0) 
    {
        phys_uart_id = 1;
        //if uart1 was initialized, buffer set failed
        if(uart_start_number & UART1_START_MASK)
        {
            return -1;
        }
    } 
    else if (A_STRCMP(name, "UART2") == 0) 
    {
        //if uart2 was initialized, buffer set failed
        if(uart_start_number & UART2_START_MASK)
        {
            return -1;
        }
        phys_uart_id = 2;
    }
    else 
    {
         return -1;
    }
    uart_buffer[phys_uart_id].tx_buffer = tx_size;
    uart_buffer[phys_uart_id].rx_buffer = rx_size;
    
    return 0;
    
}

static void uart_mod_init(int phys_uart_id)
{
    UART_HW_CONFIG  uart_config;
    void            *phys_port;
    
    if (Console_uart_info[phys_uart_id] != NULL) {
        return;    
    }
    
    Console_uart_info[phys_uart_id] = (CONSOLE_INFO_PRIV *)A_ALLOCRAM(sizeof(CONSOLE_INFO_PRIV));
    
        /* setup the physical modem */
    phys_port = UARTSERP_HW_Init(phys_uart_id);
    A_ASSERT(phys_port != NULL);  
    A_MEMZERO(&uart_config,sizeof(uart_config));        
        /* for the console, set UART buffering parameters
         * for recv we can use a fixed value to accommodate very fast typists */
    uart_config.RxBufferSize = uart_buffer[phys_uart_id].rx_buffer;
        /* use a value that is an override */
    uart_config.TxBufferSize = uart_buffer[phys_uart_id].tx_buffer;
    
    UARTSERP_HW_Start(phys_port,&uart_config);
}

LOCAL void uart_mod_start(char* name, int phys_uart_id)
{   
    SERIAL_PORT_PROPERTIES  port_properties;
    SER_PORT_UART_CONFIG    port_config;
    A_STATUS                status;
    
        /* open the logical port */
    Console_uart_info[phys_uart_id]->port = SERP_Open((A_CHAR*)name,
                                   NULL,
                                   console_serial_event_callback,
                                   &port_properties);
   
    A_MEMZERO(&port_config,sizeof(port_config));

    uart_handle[phys_uart_id] = Console_uart_info[phys_uart_id]->port;
    port_config.Baud = DEFAULT_BAUD;
    port_config.DataBits = 8;
    port_config.StopBits = 1;
    
        /* configure port */
    status = SERP_Ioctl(Console_uart_info[phys_uart_id]->port,
                        SER_PORT_IOCTL_CONFIG_UART_PORT,
                        &port_config,
                        sizeof(port_config));
    A_ASSERT(A_SUCCESS(status));
    
        /* set receiver threshold to 1 character */
    SERP_Ioctl(Console_uart_info[phys_uart_id]->port,SER_PORT_IOCTL_SET_RX_THRESH,NULL,128);
          
#if DEBUG_UART
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_getc = console_getc_replacement;
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_restore_funcs = console_restore_replacement;
#endif
}

void uart_rx_start(int phys_uart_id)
{
    extern void UartConfigureRxPolling(int id, int en);
    UartConfigureRxPolling(phys_uart_id, 1);
}

int Uart_Init(char *name)
{
    int phys_uart_id = -1;
    
    if (A_STRCMP(name, "UART0") == 0) 
    {
        phys_uart_id = 0;
    } 
    else if (A_STRCMP(name, "UART1") == 0) 
    {
        phys_uart_id = 1;
    } 
    else if (A_STRCMP(name, "UART2") == 0) 
    {
        phys_uart_id = 2;
    }
    else 
    {
        return -1;
    }
    
    if(serial_port_start == FALSE)
    {
        SERP_MODULE_INSTALL();
        SERP_Init();
        UARTSERP_HW_MODULE_INSTALL();
    }
    if((phys_uart_id == 0) && !(uart_start_number & UART0_START_MASK))
    {
        uart_mod_init(0);
        uart_mod_start((char*)"UART0", 0);
        uart_init = 1; 
        uart_start_number |= UART0_START_MASK;
        return 0;
    }
    else if((phys_uart_id == 1) && !(uart_start_number & UART1_START_MASK))
    {
        uart_mod_init(1);
        uart_mod_start((char*)"UART1", 1);
        uart_init = 1; 
        uart_start_number |= UART1_START_MASK;
        return 0;
    }
    else if((phys_uart_id == 2) && !(uart_start_number & UART2_START_MASK))
    {
        uart_mod_init(2);
        uart_mod_start((char*)"UART2", 2);
        uart_init = 1; 
        uart_start_number |= UART2_START_MASK;
        return 0;
    }
    return -1;
}

/* Return number of UART */
int Uart_Enumerate(void)
{
    //int uart_num = 1;
    int uart_num = 3;
    if (uart_init != 0) {
    	return uart_num;
    }
    
    uart_init = 1; 
    
    SERP_MODULE_INSTALL();
    SERP_Init();
    UARTSERP_HW_MODULE_INSTALL();
    uart_mod_init(0);
    uart_mod_init(1);
    uart_mod_init(2);
    uart_mod_start((char*)"UART0", 0);
    uart_mod_start((char*)"UART1", 1);
    uart_mod_start((char*)"UART2", 2);
    uart_start_number |= UART0_START_MASK | UART1_START_MASK | UART2_START_MASK;
    return uart_num;
}

//for dual uart wakeup init
int Init_Multi_Uart_Wakeup()
{
    int phys_uart_id = -1;
    phys_uart_id = Console_info->config.phys_uart_id;

    if(phys_uart_id == 0)
    {
        if(!(uart_start_number & UART1_START_MASK))
        {
            uart_mod_init(1);
            uart_mod_start((char*)"UART1", 1);
            uart_start_number |= UART1_START_MASK;
        }
        if(!(uart_start_number & UART2_START_MASK))
        {
            uart_mod_init(2);
            uart_mod_start((char*)"UART2", 2);
            uart_start_number |= UART2_START_MASK;
        }
    }
    else if(phys_uart_id == 1)
    {
        if(!(uart_start_number & UART0_START_MASK))
        {
            uart_mod_init(0);
            uart_mod_start((char*)"UART0", 0);
            uart_start_number |= UART0_START_MASK;
        }
        if(!(uart_start_number & UART2_START_MASK))
        {
            uart_mod_init(2);
            uart_mod_start((char*)"UART2", 2);
            uart_start_number |= UART2_START_MASK;
        }
    }
    else if(phys_uart_id == 2)
    {
        if(!(uart_start_number & UART0_START_MASK))
        {
            uart_mod_init(0);
            uart_mod_start((char*)"UART0", 0);
            uart_start_number |= UART0_START_MASK;
        }
        if(!(uart_start_number & UART1_START_MASK))
        {
            uart_mod_init(1);
            uart_mod_start((char*)"UART1", 1);
            uart_start_number |= UART1_START_MASK;
        }
    }
    else
    {
        return -1;
    }
    
    return 0;
}

int Console_Uart_Num(void)
{
    if(Console_info->config.phys_uart_id == 0)
        return 0;
    else if(Console_info->config.phys_uart_id == 1)
        return 1;
    else if(Console_info->config.phys_uart_id == 2)
        return 2;
    else
        return -1;
}

/* Uart_Open - open UART and get a file descritor (fd) 
   name(input): UART0 or UART1
*/
int Uart_Open(char* name)
{
    int phys_uart_id;

    if (A_STRCMP(name, "UART0") == 0) {
	phys_uart_id = 0;
       //uart_rx_start(0);
    } else if (A_STRCMP(name, "UART1") == 0) {
	phys_uart_id = 1;
       //uart_print_id = 0;
        //uart_rx_start(1);
    } else if (A_STRCMP(name, "UART2") == 0) {
        phys_uart_id = 2;
    }else {
         //A_ASSERT(FALSE);  
         return -1;
    }

    uart_rx_detect = Uart_read_null;
//    uart_mod_init(phys_uart_id);
//    uart_mod_start(name, phys_uart_id);
    _A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    

    return (phys_uart_id + 1);
}


int Uart_Open_UTFBT(char* name)
{
    int phys_uart_id;

    if (A_STRCMP(name, "UART0") == 0) {
	phys_uart_id = 0;
       //uart_rx_start(0);
    } else if (A_STRCMP(name, "UART1") == 0) {
	phys_uart_id = 1;
       //uart_print_id = 0;
        //uart_rx_start(1);
    } else if (A_STRCMP(name, "UART2") == 0) {
        phys_uart_id = 2;
    }else {
         //A_ASSERT(FALSE);  
         return -1;
    }

    uart_rx_detect = Uart_read_null;
//    uart_mod_init(phys_uart_id);
//    uart_mod_start(name, phys_uart_id);
    //_A_OS_INDIRECTION_TABLE->cmnos.serial._serial_putc = console_putc_replacement;    

    return (phys_uart_id + 1);
}

/* Set parameter of UART */
int Uart_SetPara(int fd, UART_CFG_T uart_cfg) 
{
    A_STATUS                status;
    SER_PORT_UART_CONFIG   port_config;

    A_MEMCPY(&port_config, &uart_cfg, sizeof(port_config));

    if(sdk_shell_flag == 0)
    {
         void *handle = (void *)uart_handle[fd-1];

        A_MEMCPY(&(uart_data[fd-1]), &uart_cfg, sizeof(UART_CFG_T));
        
            /* configure port */
        status = SERP_Ioctl(handle,
                            SER_PORT_IOCTL_CONFIG_UART_PORT,
                            &port_config,
                            sizeof(port_config));
    }
    else
    {
        status = SERP_Ioctl(Console_info->port,
                            SER_PORT_IOCTL_CONFIG_UART_PORT,
                            &port_config,
                            sizeof(port_config));
        sdk_shell_flag = 0;
    }
   
    return status;
}

/* Get parameter of UART */
int Uart_GetPara(int fd, UART_CFG_T* uart_cfg)
{
    if(fd == 1 || fd == 2 || fd ==3)
    {
        A_MEMCPY(uart_cfg, &(uart_data[fd-1]), sizeof(UART_CFG_T));
        return 0;
    }
    else
        return -1;
}

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes has been   written.
   returns, bytes remain in the TX buffer.
*/
int Uart_Write(int fd, unsigned char* buff, int* len)
{
    void *handle = (void *)uart_handle[fd-1];
    int retVal = -1;
    //A_PRINTF("Uart_Write, fd=%d, handle %p\n", fd, handle);
    TX_SYS_DECL_PROTECT(prot_flag);

    TX_SYS_PROTECT(prot_flag);
    retVal =  SERP_Write(handle,buff,len);
    TX_SYS_UNPROTECT(prot_flag);
    return retVal;
}

extern void CallUartDeviceDirectByteWrite(UART_DEVICE *pUartDevice, A_UINT8 *pBuffer, int Count);
void Uart_Write_Direct(int fd, unsigned char* buff, int* len)
{
    void *handle = (void *)uart_handle[fd-1];
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)handle;
    UART_DEVICE *pUartDevice = (UART_DEVICE *)(pInfo->pHWPort->pPortContext);

    TX_SYS_DECL_PROTECT(prot_flag);
    TX_SYS_PROTECT(prot_flag);
    CallUartDeviceDirectByteWrite(pUartDevice, buff, *len);
    TX_SYS_UNPROTECT(prot_flag);
}

/* fd (Input), file descriptor 
   buff (Input/output), address of the buff, the buffer should big enough.
   len (Input/output), Input length of the buff, output length of bytes read.
   returns, bytes remain in the RX buffer.
*/
int Uart_Read(int fd, unsigned char* buff, int* len)
{
    void *handle = (void *)uart_handle[fd-1];
    int retVal = -1;
    //uart_dbg("Uart_Read,port %p,handle %p fd=%d\n", Console_info->port, handle, fd);
    TX_SYS_DECL_PROTECT(prot_flag);

    TX_SYS_PROTECT(prot_flag);
    retVal = SERP_Read(handle, buff, len);
    TX_SYS_UNPROTECT(prot_flag);
    return retVal;
}

int Uart_read_null(int fd)
{
    int lenth = 0;
    return Uart_Read(fd, (unsigned char*)NULL, &lenth);
}

/* Uart_Close -close UART with given fd */
int Uart_Close(int fd)
{
//    void *handle = (void *)uart_handle[fd-1];
    
//    SERP_Close(handle);
    return 0;
}

int Uart_GetFd(char* name)
{
   int fd = 0;
   if (A_STRCMP(name, "UART0") == 0) 
   {
	fd = 1;
    } 
   else if (A_STRCMP(name, "UART1") == 0)
   {
	fd = 2;
    }
  else if (A_STRCMP(name, "UART2") == 0)
   {
        fd = 3;
    }    

    return fd;
}
