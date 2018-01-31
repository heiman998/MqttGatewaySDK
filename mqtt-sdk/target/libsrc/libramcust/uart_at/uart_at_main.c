/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include "qcom_common.h"
#include "qcom_uart.h"
#include "qcom_internal.h"
#include "qcom/qcom_cli.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"
#include "qcom/qcom_wps.h"
#include "htc_buf.h"
#include "htc.h"
#define HTC_RAM_TEST_PATCH
#include "htc_api.h"
#include "wmix.h"

#define AT_WMI_COMMAND       "ATWMI="
#define AT_WMI_COMMAND_SIZE (sizeof(AT_WMI_COMMAND) - 1)
#define AT_WMI_END_POINT     0x01
#define AT_WMI_TIMEOUT       2000
#define HTC_HEADER_PAD      ((AT_WMI_COMMAND_SIZE & 0x03) % 4)
#define WMI_EXTENSION_CMDID  0x002E
static void ARdumpData(void *pData, char *pPrefix, unsigned int dataSize,unsigned int maxSize);
extern void _WMI_Dispatch(unsigned short commandID,unsigned char *pDataBuffer,int dataSize,unsigned short dataInfo);
static int  s_bDumpData = FALSE;

A_UINT8 currentDeviceId = 0;

/* uart at mode */
typedef enum {
    UART_AT_CMD_MODE =0,
    UART_AT_DATA_MODE
} UART_AT_OPERATING_MODE_T;

/* AT CMD type */
typedef struct {
    char*       cmd_str;
    A_UINT16    cmd_str_len;
    A_UINT8     min_args;
    A_UINT8     can_exec_in_unconn_state;
    A_UINT8     can_exec_in_conn_state;
    void (* process_cmd)(char* cmd_str, A_UINT16 cmd_len); 
    char*       help_str;
} AT_CMD_ENTRY;

UART_AT_OPERATING_MODE_T g_operating_mode;

/* important buffer and offset */
char *at_buffer;    /* buffer for cmd from uart */
char *data_buffer;  /* buffer for data from uart */
int cmd_offset = 0; /* offset in cmd buffer */
int data_offset= 0;/* offset in data buffer */
int g_data_buffer_size = 512; //data buffer size for uart data rx, default 512.
#define AT_CMD_BUFFER_LEN 1664

/* data of tcp/udp socket */
int tcp_socket =0;
int udp_socket =0;
int sudp_client =0;
int stcp_client_fd   =0;
int g_tx_timeout =300;  //timeout in tx
int socket_state_connected;
struct sockaddr_in server_udp_addr;
struct sockaddr_in remote_udp_addr ;
struct sockaddr_in fromAddr ;
int  remote_udp_size;
A_INT32 recvBuffSize = 1024; //data buffer size for socket data rx, default 1024.
A_UINT8 *pDataBuffer = NULL;

A_UINT32 g_Baud = 115200; // store the baud for query

/* params of mode switch */
A_UINT8 plus_received =0;
unsigned long plus_last_time;
#define PLUS_TIME_INTERVAL 1000

/* fucntion for uart read, write and ioctl */
extern int at_console_read(A_UINT8 * pBuffer, int *pWriteLength);
extern int at_console_write(A_UINT8 * pBuffer, int *pWriteLength);
extern int at_console_ioctl_baud(A_UINT32 Baud, A_UINT8 DataBits, A_UINT8 StopBits, A_UINT16 ConfigFlags);


/* fuction of cmd printf */
//extern int console_printf(const char *fmt, ...);
#define AT_PTF printf

/* function of tasks */
extern int qcom_task_start(void (*fn) (unsigned int), unsigned int arg, int stk_size, int tk_ms);
extern void qcom_task_del(void);
extern void qcom_task_exit(void);

/* specific cmd processing function */
void process_WS(char* cmd_str, A_UINT16 cmd_len);
void process_W(char* cmd_str, A_UINT16 cmd_len);
void process_WMI(char* cmd_str, A_UINT16 cmd_len);
void process_WREV(char* cmd_str, A_UINT16 cmd_len);
void process_WA(char* cmd_str, A_UINT16 cmd_len);
void process_WAWPS(char* cmd_str, A_UINT16 cmd_len);
void process_WAWPA(char* cmd_str, A_UINT16 cmd_len);
void process_WD(char* cmd_str, A_UINT16 cmd_len);
void process_WRSSI(char* cmd_str, A_UINT16 cmd_len);
void process_WPM(char* cmd_str, A_UINT16 cmd_len);
void process_PM(char* cmd_str, A_UINT16 cmd_len);
void process_WPHYMODE(char* cmd_str, A_UINT16 cmd_len);
void process_WWEPKEY(char* cmd_str, A_UINT16 cmd_len);
void process_WAWEP(char* cmd_str, A_UINT16 cmd_len);
void process_NPING(char* cmd_str, A_UINT16 cmd_len);
void process_NSET(char* cmd_str, A_UINT16 cmd_len);
void process_NDHCP(char* cmd_str, A_UINT16 cmd_len);
void process_NSOCK(char* cmd_str, A_UINT16 cmd_len);
void process_NCLOSE(char* cmd_str, A_UINT16 cmd_len);
void process_NCTCP(char* cmd_str, A_UINT16 cmd_len);
void process_O(char* cmd_str, A_UINT16 cmd_len);
void process_HELP(char* cmd_str, A_UINT16 cmd_len);
void process_BSIZE(char* cmd_str,A_UINT16 cmd_len);
void process_TO(char* cmd_str, A_UINT16 cmd_len);
void process_NCUDP(char* cmd_str,A_UINT16 cmd_len);
void process_NSUDP(char* cmd_str, A_UINT16 cmd_len);
void process_NSTCP(char* cmd_str, A_UINT16 cmd_len);
void process_baud(char* cmd_str, A_UINT16 cmd_len);
void process_PSUSE(char* cmd_str,A_UINT16 cmd_len);
void process_PSUS(char* cmd_str,A_UINT16 cmd_len);

/* help infos */
#define W_HELP                 "Gets WLAN status\n\r"
#define WMI_HELP               "Send raw HTC/WMI packet\n\r"
#define WREV_HELP              "Returns WLAN firmware version\n\r"
#define WS_HELP                "Start WLAN Scan\n\r"
#define PM_HELP                "Set power mode =<enable>,<mode>,<wake period>,<sleep period>\n\r\tenable-1:enable, 0:disable\n\r\tmode-0:one shot, 1:periodic\n\r\twake period- wake time in mseconds\n\r\tsleep period- sleep time in mseconds \n\r"
#define WPM_HELP               "Set power mode =<mode>\n\r\tmode- 1:IEEE Power Save, 0:Max Perf \n\r"
#define PSUSE_HELP             "Enable power suspend\n\r"
#define PSUS_HELP              "Start power suspend and configure wakeup options =<sleep time in ms>,<GPIO31 mode>,<GPIO30 mode>\n\r\tsleep time-0:disable, >0:wakeup when the timer expired\n\r\tGPIO31 mode-0:disable, 1:high, 2:low\n\r\tGPIO30 mode-0:disable, 1:high, 2:low\n\r"
#define WA_HELP                "Associate with SSID, no security, open mode =<ssid>\n\r"
#define WAWPS_HELP             "Associate using WPS =<1>,<PUSH/PIN>\n\r"
#define WAWPA_HELP             "Associate using WPA =<ssid>,<wpa ver>,<ucipher>,<mcipher>,<passphrase>\n\r\twpa version-1:wpa, 2:wpa2\n\r\tcipher-TKIP/CCMP\n\r"
#define WD_HELP                "Dissociate from connected AP =\n\r"
#define WPHYMODE_HELP          "Set WLAN PHY mode to =<a/b/g/n>\n\r" //KNR:todo na
#define WRSSI_HELP             "Get RSSI\n\r"
#define WWEPKEY_HELP           "Set WLAN WEP Key = <key index>,<hex key>\n\r\tkey index-1 to 4\n\r" 
#define WAWEP_HELP             "Associate using WEP mode =<key index>,<ssid>\n\r"
#define NPING_HELP             "Ping to specified host=<IP addr>,<ping_size(optional)>\n\r\tIP addr- a.b.c.d\n\r"
#define NSET_HELP              "Set/Get Network Parameters=? or <IP>,<Mask>,<GW>\n\r"
#define NDHCP_HELP             "Run DHCP client =<1>\n\r"
#define NSOCK_HELP             "Open Socket =<TCP/UDP>\n\r"
#define NCLOSE_HELP            "Close Socket \n\r"
#define NCTCP_HELP             "Attempt TCP client connection to destination=<IP>,<PORT>\n\r"
#define NCUDP_HELP             "Attempt UDP client connection to destination=<IP>,<PORT>\n\r"
#define NSUDP_HELP             "Attempt UDP server connection =<CLIENT IP>,<PORT>\n\r"
#define NSTCP_HELP             "Attempt TCP server connection =<PORT>\n\r"
#define BSIZE_HELP             "Enter the buffer size <1-1200> \n\r"
#define TO_HELP                "Enter the timeout (in millseconds not less than 200) \n\r"
#define O_HELP                 "Enter transparent mode\n\r"
#define HELP_HELP              "Enter an AT command followed by =arguments. Example ATWS=\n\r"
#define WBAUD_HELP             "Enter the baud rate ATB=<baud rate>,<data bits>,<parity bit>,<stop bits>\n\r \
    data bits = 8 \n\r \
Parity is n - no parity, e - even parity and o - odd parity \n\r \
1 stop bit Ex: ATB=9600,8,n,1\n\r"

/* cmd Engine */
AT_CMD_ENTRY at_cmds[] = {
    /* CMD              LEN                     args exec u?, exec c? process_cmd                     help    */   
    { "ATW=",           sizeof("ATW=")-1,        0,      1,    1,     process_W,                      W_HELP},
    { AT_WMI_COMMAND,   sizeof(AT_WMI_COMMAND)-1,0,      1,    1,     process_WMI,                    WMI_HELP},
    { "ATB=",           sizeof("ATB=")-1,        0,      1,    1,     process_baud,                   WBAUD_HELP},
    { "ATWREV=",        sizeof("ATWREV=")-1,     0,      1,    1,     process_WREV,                   WREV_HELP},
    { "ATWS=",          sizeof("ATWS=")-1,       0,      1,    0,     process_WS,                     WS_HELP},
    { "ATPM=",          sizeof("ATPM=")-1,       0,      1,    1,     process_PM,                     PM_HELP},
    { "ATWPM=",         sizeof("ATWPM=")-1,      0,      1,    1,     process_WPM,                    WPM_HELP},
    { "ATPSUSE=",       sizeof("ATPSUSE=")-1,    0,      1,    1,     process_PSUSE,                  PSUSE_HELP},  
    { "ATPSUS=",        sizeof("ATPSUS=")-1,     0,      1,    1,     process_PSUS,                   PSUS_HELP},  
    { "ATWA=",          sizeof("ATWA=")-1,       0,      1,    0,     process_WA,                     WA_HELP},
    { "ATWAWPS=",       sizeof("ATWAWPS=")-1,    0,      1,    0,     process_WAWPS,                  WAWPS_HELP},
    { "ATWAWPA=",       sizeof("ATWAWPA=")-1,    0,      1,    0,     process_WAWPA,                  WAWPA_HELP},
    { "ATWD=",          sizeof("ATWD=")-1,       0,      0,    1,     process_WD,                     WD_HELP},
    { "ATWPHYMODE=",    sizeof("ATWPHYMODE=")-1, 0,      1,    0,     process_WPHYMODE,               WPHYMODE_HELP}, //KNR:todo na
    { "ATWRSSI=",       sizeof("ATWRSSI=")-1,    0,      0,    1,     process_WRSSI,                  WRSSI_HELP},
    { "ATWWEPKEY=",     sizeof("ATWWEPKEY=")-1,  0,      1,    0,     process_WWEPKEY,                WWEPKEY_HELP},
    { "ATWAWEP=",       sizeof("ATWAWEP=")-1,    0,      1,    0,     process_WAWEP,                  WAWEP_HELP},
    { "ATNPING=",       sizeof("ATNPING=")-1,    0,      0,    1,     process_NPING,                  NPING_HELP},
    { "ATNSET=",        sizeof("ATNSET=")-1,     0,      1,    1,     process_NSET,                   NSET_HELP},
    { "ATNDHCP=",       sizeof("ATNDHCP=")-1,    0,      0,    1,     process_NDHCP,                  NDHCP_HELP},
    { "ATNSOCK=",       sizeof("ATNSOCK=")-1,    0,      0,    1,     process_NSOCK,                  NSOCK_HELP},
    { "ATNCLOSE=",      sizeof("ATNCLOSE=")-1,   0,      0,    1,     process_NCLOSE,                 NCLOSE_HELP},
    { "ATNCTCP=",       sizeof("ATNCTCP=")-1,    0,      0,    1,     process_NCTCP,                  NCTCP_HELP},
    { "ATNCUDP=",       sizeof("ATNCUDP=")-1,    0,      0,    1,     process_NCUDP,                  NCUDP_HELP},
    { "ATNSUDP=",       sizeof("ATNSUDP=")-1,    0,      0,    1,     process_NSUDP,                  NSUDP_HELP},
    { "ATNSTCP=",       sizeof("ATNSTCP=")-1,    0,      0,    1,     process_NSTCP,                  NSTCP_HELP},
    { "ATO=",           sizeof("ATO=")-1,        0,      0,    1,     process_O,                      O_HELP},
    { "ATBSIZE=",       sizeof("ATBSIZE=")-1,    0,      1,    1,     process_BSIZE,                  BSIZE_HELP},
    { "ATTO=",          sizeof("ATTO=")-1,       0,      1,    1,     process_TO,                     TO_HELP},
    { "ATHELP=",        sizeof("ATHELP=")-1,     0,      1,    1,     process_HELP,                   HELP_HELP},
    { NULL,             0,                       0,      0,    0,     NULL,                           NULL},
};

/*
 * Util Fuctions
 */
int my_strncmpi(char *str1, char *str2, int length)
{
    char temp_str1, temp_str2;
    if (length <= 0)
        return 0;

    while (--length >= 0) 
    {

        //convert both characters to uppercase
        temp_str1 = *str1;
        if((*str1 >= 0x61) && (*str1 <= 0x7a))
        {
            temp_str1 = *str1 - 0x20;
        }

        temp_str2 = *str2;
        if((*str2 >= 0x61) && (*str2 <= 0x7a))
        {
            temp_str2 = *str2 - 0x20;
        }

        if((temp_str1 != temp_str2) || (temp_str1 == '\0'))
        {
            break;
        }

        ++str1;
        ++str2;
    }

    return (temp_str1 - temp_str2);
}

int parse_ipv4_ad(unsigned int * ip_address,   /* pointer to IP address returned */
        unsigned *  sbits,      /* default subnet bit number */
        char *   stringin)
{
    int error = -1;
    char *   cp;
    int   dots  =  0; /* periods imbedded in input string */
    int   number;
    union
    {
        unsigned char   c[4];
        unsigned long   l;
    } retval;

    cp = stringin;
    while (*cp == ' ')
      ++cp;
    while (*cp)
    {
        if (*cp > '9' || *cp < '.' || *cp == '/')
            return(error);
        if (*cp == '.')dots++;
        cp++;
    }

    if ( dots < 1 || dots > 3 )
        return(error);

    cp = stringin;
    if ((number = atoi(cp)) > 255)
        return(error);

    retval.c[0] = (unsigned char)number;

    while (*cp != '.')cp++; /* find dot (end of number) */
    cp++;             /* point past dot */

    if (dots == 1 || dots == 2) retval.c[1] = 0;
    else
    {
        number = atoi(cp);
        while (*cp != '.')cp++; /* find dot (end of number) */
        cp++;             /* point past dot */
        if (number > 255) return(error);
        retval.c[1] = (unsigned char)number;
    }

    if (dots == 1) retval.c[2] = 0;
    else
    {
        number = atoi(cp);
        while (*cp != '.')cp++; /* find dot (end of number) */
        cp++;             /* point past dot */
        if (number > 255) return(error);
        retval.c[2] = (unsigned char)number;
    }

    if ((number = atoi(cp)) > 255)
        return(error);
    retval.c[3] = (unsigned char)number;

    if (retval.c[0] < 128) *sbits = 8;
    else if(retval.c[0] < 192) *sbits = 16;
    else *sbits = 24;

    *ip_address = retval.l;
    return(0);
}

int copy_data_upto(void* from_buf, void* to_buf, A_UINT16 * cmd_sz_read, A_UINT16 * tSz, A_UINT8 remove_leading_zeros, char term)
{
    A_UINT8* vpBuffer = (A_UINT8*) from_buf;
    A_UINT8* rpBuffer = (A_UINT8*) to_buf;
    //A_UINT8 leadingZero = 1;
    A_UINT8 leadingZero = remove_leading_zeros;
    A_UINT16 tMax = *tSz;
    *cmd_sz_read = 0;
    *tSz = 0;

    while(vpBuffer[*cmd_sz_read] != term)
    {
        if(vpBuffer[*cmd_sz_read] == '\r') //Unexpected '\r' before reading upto term char
        {
            return 0;
        }

        if(leadingZero && (vpBuffer[*cmd_sz_read] == '0'))
        {
            (*cmd_sz_read)++;
            continue;
        }
        else
        {
            leadingZero = 0;
            rpBuffer[*tSz] = vpBuffer[*cmd_sz_read];
            (*tSz)++;
            (*cmd_sz_read)++;
        }

        if((*tSz) >= tMax)
            return 0;
    }
    /* The argument was zero. Just send it */
    if(!(*tSz) && leadingZero)
    {
        rpBuffer[0] = '0';
        *tSz =1;
        return 0;
    }
    return 1;
}

/* function to switch mode : data <-->cmd */
void switch_uart_mode(int cmdMode)
{
    /* data --> cmd */
    if(cmdMode==1){
        mem_free(pDataBuffer);  //free socket receive data buffer
        mem_free(data_buffer);  //free tx data buffer
        g_operating_mode = UART_AT_CMD_MODE;
        AT_PTF("Entering cmd mode \n");

    /* cmd -->data */
    }else{
        // malloc rx data buffer
        if(!pDataBuffer){
            pDataBuffer = mem_alloc(recvBuffSize);
            /* AT_PTF("malloc receive buffer!\r\n"); */
        }
        if(!data_buffer){
            data_buffer = mem_alloc(g_data_buffer_size );
            memset(data_buffer,0,g_data_buffer_size);
            /* AT_PTF("malloc data buffer! \n"); */
        }    

        g_operating_mode = UART_AT_DATA_MODE;
        AT_PTF("Entering data mode \n");
    }
}

/* process the cmd here */
void process_cmd()
{

    char* cmd_str ;
    int   cmd_index = 0;
    int   cmd_found = 0;
    int   len = cmd_offset;
    if(!at_buffer) //this should not happen
    {
        AT_PTF("%s %d serious error\n", __FUNCTION__, __LINE__);
        return;
    }
    ARdumpData(at_buffer, "+Input", cmd_offset, 0);
    cmd_str =  (char*)(at_buffer);

    //Process the command here
    if(1)
    {
        while((at_cmds[cmd_index].cmd_str) != NULL)
        {
            if(my_strncmpi(cmd_str, at_cmds[cmd_index].cmd_str, at_cmds[cmd_index].cmd_str_len) == 0)
            {
                cmd_str+=at_cmds[cmd_index].cmd_str_len;
                len-= at_cmds[cmd_index].cmd_str_len;
                AT_PTF("\n\r");
                cmd_found++;
                at_cmds[cmd_index].process_cmd(cmd_str, len);
                break;
            }
            cmd_index++;
        }
        if(!cmd_found)
        {
            AT_PTF("\n\rERROR: Invalid command.\n\rFor help enter athelp= \n\r");
        }
    }
}

//entry of cmd character
/* insert the char into cmd buffer */
static unsigned int s_currentTime;
static int          s_bCurrentTimeSet = FALSE;

void CMD_MODE_append_char(char c)
{
  unsigned short commandSize;
  char          *pError;
  char          *pHtcFrameHeader;

  /*-----------------------------------------------------------------
  ; Intercept "ATWMI=" command here, it is kind of special
  ;----------------------------------------------------------------*/
  if (cmd_offset >= AT_WMI_COMMAND_SIZE &&
      my_strncmpi(at_buffer, AT_WMI_COMMAND, AT_WMI_COMMAND_SIZE) == 0) {
    pError = "*Timeout waiting for HTC and WMI header";
    if (cmd_offset == AT_WMI_COMMAND_SIZE) {
      cmd_offset += HTC_HEADER_PAD;         /* HTC header on 4-byte boundary */

      /*-----------------------------------------------------------------
      ; 'D' ==> Special character to turn ON  debug
      ; 'X' ==> Special character to turn OFF debug
      ;----------------------------------------------------------------*/
      if (c == 'D' || c == 'X') {
        pError = NULL;
        s_bDumpData = TRUE;
        if (c == 'X')
          s_bDumpData = FALSE;
      }
    }
    if(cmd_offset < AT_CMD_BUFFER_LEN - 1) {
        *((char *)at_buffer + cmd_offset) = c;
        ++cmd_offset;
    }
    if (s_bCurrentTimeSet == FALSE) {
      s_bCurrentTimeSet = TRUE;
      s_currentTime = time_ms();
    }
    commandSize = 0;
    if (pError != NULL &&
        cmd_offset >= AT_WMI_COMMAND_SIZE + sizeof(HTC_FRAME_HDR) + sizeof(WMI_CMD_HDR) + HTC_HEADER_PAD) {
      pHtcFrameHeader = at_buffer + AT_WMI_COMMAND_SIZE + HTC_HEADER_PAD;
      commandSize = ((HTC_FRAME_HDR *)pHtcFrameHeader)->PayloadLen;
      if (commandSize >= sizeof(WMI_CMD_HDR) &&
          commandSize < AT_CMD_BUFFER_LEN - HTC_HEADER_PAD - AT_WMI_COMMAND_SIZE - sizeof(HTC_FRAME_HDR) &&
          ((HTC_FRAME_HDR *)pHtcFrameHeader)->EndpointID == AT_WMI_END_POINT) {
        pError = "*Timeout waiting for WMI data";
        if (cmd_offset == HTC_HEADER_PAD + AT_WMI_COMMAND_SIZE + sizeof(HTC_FRAME_HDR) + commandSize) {
          pError = NULL;
          ARdumpData(at_buffer, "+Input", cmd_offset, 0);
          process_WMI(at_buffer + AT_WMI_COMMAND_SIZE + HTC_HEADER_PAD + sizeof(HTC_FRAME_HDR), commandSize);
        }
      }
      else {
        ARdumpData(at_buffer + AT_WMI_COMMAND_SIZE + HTC_HEADER_PAD, "*Invalid HTC header",
                   cmd_offset - AT_WMI_COMMAND_SIZE - HTC_HEADER_PAD, 0);
        pError = NULL;
      }
    }
    if (pError == NULL || time_ms() - s_currentTime > AT_WMI_TIMEOUT) {
      if (pError != NULL) {
        ARdumpData(at_buffer + AT_WMI_COMMAND_SIZE + HTC_HEADER_PAD, pError,
                   cmd_offset - AT_WMI_COMMAND_SIZE - HTC_HEADER_PAD, 0);
      }
      memset(at_buffer, 0, AT_CMD_BUFFER_LEN);
      cmd_offset = 0;
      s_bCurrentTimeSet = FALSE;
      AT_PTF("\nshell>");
    }
    return;
  }

  /*-----------------------------------------------------------------
  ; NOT "ATWMI=" command
  ;----------------------------------------------------------------*/
    if((c =='\n') || (c=='\r') || (cmd_offset >= (AT_CMD_BUFFER_LEN -1)))
    {
        if(cmd_offset) 
        {
            //terminate command string with NULL character
            *( (char*)at_buffer + cmd_offset) ='\0';
            /* AT_PTF("new command : %s\n", at_buffer); */
            process_cmd();
        }
        else
        {
            AT_PTF("\nshell>");
        }

        memset(at_buffer,0,AT_CMD_BUFFER_LEN);
        cmd_offset = 0;

    }
    else
    {
        if(cmd_offset < AT_CMD_BUFFER_LEN-1)
        {
            *((char *)at_buffer + cmd_offset) = c;
            cmd_offset++;
        }
    }
}

/*************************************************************
 * Function     : at_send_packet 
 * Description  : this function is sending the data in the buffer by socket.
 *************************************************************/
void at_send_packet(){

    /* run in data mode only */
    if(g_operating_mode == UART_AT_DATA_MODE){
        /* if there is data in buffer, send it */
        if(data_offset){
            if(!(stcp_client_fd || tcp_socket || udp_socket || sudp_client)){
                return;
            }
            if(stcp_client_fd){
                if ( qcom_send(stcp_client_fd, data_buffer, data_offset, 0) <0 ){
                    AT_PTF("\nFailed to send data through tcp handle: %x, client handle: %x.\n", tcp_socket, stcp_client_fd);
                }
            }else if(tcp_socket){
                if ( qcom_send(tcp_socket, data_buffer, data_offset, 0) <0 ){
                    AT_PTF("\nFailed to send data through tcp handle %x.\n", tcp_socket);
                }
            }else if(sudp_client){//udp server to other client 
                if ( qcom_sendto(udp_socket, data_buffer, data_offset, 0, (struct sockaddr *) &fromAddr, sizeof (struct sockaddr_in)) < 0){
                    AT_PTF("\nFailed to send data through udp handle %x.\n", udp_socket);
                }
            }else{ //udp client to a server
                if ( qcom_sendto(udp_socket, data_buffer, data_offset, 0, (struct sockaddr *) &remote_udp_addr, sizeof (struct sockaddr_in)) < 0){
                    AT_PTF("\nFailed to send data through udp handle %x.\n", udp_socket);
                }
            }
            /* reset the buffer and offset */
            memset(data_buffer,0,g_data_buffer_size);
            data_offset = 0;
        }
    }
}

/* insert char or send data or receive + */
void insert_data_into_buffer(char c, A_UINT8 plus){
    switch(plus){
        case 0:
            if((data_offset > (g_data_buffer_size -1)))
            {
                at_send_packet();
                insert_data_into_buffer(c,0);
            }else
            {
                *((char *)data_buffer + data_offset) = c;
                data_offset++;
            }
            break;
        case 1: case 2:
            at_send_packet();
            break;
        case 3:
            while(plus_received--){
                *((char *)data_buffer + data_offset) = '+';
                data_offset++;
            }
            break;
        default:
            break;
    }
}

/* entry of data character */
/* insert data character into data buffer to tx */
void DATA_MODE_append_char(char c, A_UINT8 timeout)
{
    unsigned long current_time;

    AT_PTF("DATA_MODE_append_char in... %d\n", plus_received);

    /*  check if it is mode switch */
    /* to switch to cmd mode from data mode , sequence of +++ is expected with a minimum pause period of 1sec between the + */

    /* +++ or +* */
    if(plus_received){
        if(c=='+' && (( current_time=time_ms() ) - plus_last_time) >= PLUS_TIME_INTERVAL){
            /* AT_PTF("time of  second and ... + is: %d.\n", current_time); */
            plus_last_time = current_time;
            plus_received++;
            if(plus_received ==3){
                /* switch to cmd mode */
                AT_PTF("switch to cmd mode.\n");
                switch_uart_mode(1); 
            }
        }else{
            insert_data_into_buffer('+',3);
            plus_received =0;
            plus_last_time =0;
            goto insert_data;
        }
    }

    /* here we process the fisrt '+' */
    /* record the time the first '+' */
    if( (c =='+') && (!plus_received)){
        current_time = time_ms();
        /* AT_PTF("time of + is: %d.\n", current_time); */
        plus_last_time = current_time;
        plus_received++;
    }

insert_data:
    /*  fill the character into the buffer */
    /* user can set this buffer size value */
    insert_data_into_buffer(c,plus_received);

}

/* entry of character from uart*/
void process_at_character(A_UINT8 character){

    // open this code can see the input in the console
    /* Can't do this since we drop characters when echo is enabled */
    /*    AT_PTF("%c",character); */

    //molloc the cmd buffer 
    if(!at_buffer){
        at_buffer = mem_alloc(AT_CMD_BUFFER_LEN);
        memset(at_buffer,0,AT_CMD_BUFFER_LEN);
        /* AT_PTF("malloc at cmd buffer! \n"); */
    }

    if(g_operating_mode == UART_AT_DATA_MODE){
        DATA_MODE_append_char(character, 0);
    }else{
        CMD_MODE_append_char(character);
    }
}



/*
 * AT CMD Fuctions
 */

/* needs to add version api in driver */
void process_WREV(char* cmd_str, A_UINT16 cmd_len)
{
    A_CHAR date[20];
    A_CHAR time[20];
    A_CHAR ver[20];
    A_CHAR cl[20];
    qcom_firmware_version_get(date, time, ver, cl);


    AT_PTF("Host version        : Hostless\n");
    AT_PTF("Target version      : QCM\n");
    AT_PTF("Firmware version    : %s\n", ver);
    AT_PTF("Firmware changelist : %s\n", cl);
    AT_PTF("Interface version   : EBS\n");
    AT_PTF(" - built on %s %s\n", date, time);
}

void process_baud(char* cmd_str, A_UINT16 cmd_len)
{
#if 1
    char token_buf[32];
    A_UINT16  token_len;
    A_UINT32 data_bits,stop_bit;
    A_UINT32 Baud;
    A_UINT8 DataBits;
    A_UINT8 StopBits;
    A_UINT16 ConfigFlags;
    unsigned int input; 
    do 
    {
        if((*cmd_str == '?') || (cmd_len <= 1))
        {
            AT_PTF("Baud rate is %d \n\r",g_Baud);
            break;
        }

        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len < 32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if(token_len==0)
        {
            AT_PTF("ERROR: Baud rate not supplied\n");
            break;
        }
        A_SSCANF(token_buf, "%d", &input);
        if((input != 9600) && (input != 19200) && (input != 38400) && (input != 57600) && (input != 115200))
        {
            AT_PTF(" ERROR:Please enter a valid baud rate(9600, 19200, 38400, 57600, 115200)\n\r"); 
            break;
        }
        Baud   =input;
        g_Baud =input;

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len < 32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if(token_len==0)
        {
            AT_PTF("ERROR: Data bits not supplied\n\r");
            break;
        }
        A_SSCANF(token_buf, "%d", &data_bits);
        /* data_bits = strtol(token_buf, NULL, 10); */
        AT_PTF("Data bits = %d\n\r",data_bits);
        if(data_bits != 8)
        {
            AT_PTF("Only 8 bit data transmission is possible \n\r");
            break;
        }
        DataBits = data_bits;

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len < 32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if(token_len==0)
        {
            AT_PTF("ERROR: Parity(no parity/even/odd)  not supplied\n\r");
            break;
        }
        if((*token_buf != 'n') && (*token_buf != 'o') && (*token_buf != 'e'))
        {
            AT_PTF("Parity has to be 'n','o', or 'e'(no/odd/even parity)\n\r");
            break;
        }
        if(*token_buf == 'n')
            ConfigFlags=0;
        else if(*token_buf == 'o')
            ConfigFlags=1;
        else if(*token_buf == 'e')
            ConfigFlags=2;

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len < 32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if(token_len==0)
        {
            AT_PTF("ERROR: Stop bit not supplied\n");
            break;
        }
        A_SSCANF(token_buf, "%d", &stop_bit);
        /* stop_bit = strtol(token_buf, NULL,10); */
        AT_PTF("Stop bit = %d\n\r",stop_bit);
        if(stop_bit != 1)
        {
            AT_PTF("ERROR: Only one stop bit is supported \n\r");
            break;
        }
        StopBits = stop_bit;

    	if( at_console_ioctl_baud(Baud, DataBits, StopBits, ConfigFlags) == A_OK )
            AT_PTF("OK\n");
        else
            AT_PTF("ERROR\n");
    } while(0);
#endif
}

void process_HELP(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT8 cmd_index = 0;
    while((at_cmds[cmd_index].cmd_str) != NULL)
    {
        AT_PTF("%s\t%s", at_cmds[cmd_index].cmd_str, at_cmds[cmd_index].help_str);
        cmd_index++;
    }

}

#define DEFAULT_PING_SIZE 64
A_UCHAR ping_str[32];
A_UINT16 ping_size;

void process_NPING(char* cmd_str, A_UINT16 cmd_len)
{
    unsigned short token_len;
    A_UINT32 addr;
    char token_buf[32];
    A_UCHAR local_index=0;
    unsigned int sbits;
    A_UINT32 i;
    do{

        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 31; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if( (token_len<7) || (parse_ipv4_ad(&addr, &sbits, (char*)token_buf) !=0))
        {
            AT_PTF("ERROR: Invalid IP address %s\n", token_buf);
            break;
        }
        addr = ntohl(addr);

        while(token_buf[local_index]!= '\0')
        {
            ping_str[local_index] = token_buf[local_index];
            local_index++;
        }
        ping_str[local_index] = '\0';
        if(cmd_str[local_index]!= ',')
        {
            ping_size = DEFAULT_PING_SIZE;
            /* change the qcom_ip_ping to qcom_ping instead.*/
            //qcom_ip_ping(addr, ping_size, 5, 100);
            for(i=0;i<5;i++){
            	  if(qcom_ping(addr, ping_size) == A_OK)
            	      AT_PTF("Ping reply from %d.%d.%d.%d: time<1ms\r\n",
                        addr >> 24 & 0xFF, addr >> 16 & 0xFF, addr >> 8 & 0xFF, addr & 0xFF);
                else
            		    AT_PTF("Request timed out\r\n");
            	  if(i<4)
            		    qcom_thread_msleep(100);
            }
            break;
        }
        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;
        //get the ping size
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
        int size;
        A_SSCANF(token_buf, "%d", &size);
        ping_size= (A_UINT16)size;
        /* AT_PTF("ping size:%d\n",ping_size); */
        if(token_len==0)
        {
            AT_PTF("ERROR: Ping size not supplied\n");
            break;
        }
        if((ping_size < 32) || (ping_size > 1200))
        {
            AT_PTF("ERROR: Valid Ping size is bet 32 and 1200\n\r");
            break;
        }

				/* change the qcom_ip_ping to qcom_ping instead.
				 *if( qcom_ip_ping(addr, ping_size, 5, 100) == A_OK)
         *    AT_PTF("OK\n");
         *else
         *    AT_PTF("ERROR\n");
         */
        for(i=0;i<5;i++){
            if(qcom_ping(addr, ping_size) == A_OK)
                AT_PTF("Ping reply from %d.%d.%d.%d: time<1ms\r\n",
                        addr >> 24 & 0xFF, addr >> 16 & 0xFF, addr >> 8 & 0xFF, addr & 0xFF);
            else
            		AT_PTF("Request timed out\r\n");
            if(i<4)
            		qcom_thread_msleep(100);
        }

    }while(0);
}

//{ "Associate using WPA <ssid>,<ver>,<ucipher>,<mcipher>,<passphrase>\n\r"},
void process_WAWPA(char* cmd_str, A_UINT16 cmd_len)
{
    char token_buf[32];
    char ssid_buf[64];
    A_UINT16  token_len;
    /* A_INT32 result = -1; */
    A_UINT32 wpa_ver_num=0;

    do
    {
        //ssid
        A_MEMSET(ssid_buf, 0, 64);
        token_len = (cmd_len<token_len) ? cmd_len : 64; //maximum len for this token
        copy_data_upto(cmd_str, ssid_buf, &cmd_len, &token_len, 0, ',');

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        if(cmd_len <1)
            break;

        //Get version
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
        A_SSCANF(token_buf, "%d", &wpa_ver_num);
        /* wpa_ver_num = strtoul(token_buf, NULL, 10); */
        if(wpa_ver_num==1)
        {
            /* strcpy(wpa_ver,"wpa"); */
            qcom_sec_set_auth_mode(currentDeviceId, WLAN_AUTH_WPA_PSK); //WLAN_AUTH_MODE
        }
        else if(wpa_ver_num==2)
        {
            /* strcpy(wpa_ver,"wpa2"); */
            qcom_sec_set_auth_mode(currentDeviceId, WLAN_AUTH_WPA2_PSK); //WLAN_AUTH_MODE
        }
        else
        {
            AT_PTF("ERROR: version can be 1/2\n");
            break;
        }

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        if(cmd_len <1)
            break;

        //Get ucipher
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');
        if(my_strncmpi(token_buf, "TKIP", sizeof("TKIP")-1) == 0)
        {
            qcom_sec_set_encrypt_mode(currentDeviceId, WLAN_CRYPT_TKIP_CRYPT); 

        } 
        else if(my_strncmpi(token_buf, "CCMP", sizeof("CCMP")-1) == 0)
        {
            qcom_sec_set_encrypt_mode(currentDeviceId, WLAN_CRYPT_AES_CRYPT); 
        }
        else
        {
            AT_PTF("ERROR: Invalid ucipher\n");
            break;
        }

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        if(cmd_len <1)
            break;

        //Get mcipher
        // mcipher should be the same with ucipher !!!
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');
        if(my_strncmpi(token_buf, "TKIP", sizeof("TKIP")-1) == 0)
        {
            /* cipher.mcipher = 0x04; */
        } 
        else if(my_strncmpi(token_buf, "CCMP", sizeof("CCMP")-1) == 0)
        {
            /* cipher.mcipher = 0x08; */
        }
        else
        {
            AT_PTF("ERROR: Invalid mcipher\n");
            break;
        }

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        if(cmd_len <1)
            break;

        //Get the passphrase
        qcom_sec_set_passphrase(currentDeviceId, cmd_str);
        {
            char* argv[3] ;
            A_INT32 argc;
            argv[0] = NULL;
            argv[1] = NULL;
            argv[2] = (char*)ssid_buf;
            argc = 3;
            AT_PTF("Trying to connect to %s\n", ssid_buf);
            qcom_sta_connect_without_scan(currentDeviceId, ssid_buf);
            AT_PTF("OK\n");
        }
    } while(0);
}

void process_WS(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT16 count;
    QCOM_BSS_SCAN_INFO* pOut;
    qcom_start_scan_params_t scanParams;
    scanParams.forceFgScan = 1; 
    scanParams.numChannels = 0;
    scanParams.homeDwellTimeInMs = 0;
    scanParams.forceScanIntervalInMs = 1; 
    scanParams.scanType  = WMI_LONG_SCAN;
    qcom_scan_all_bss_start(currentDeviceId, &scanParams);

    if(qcom_get_scan(currentDeviceId, &pOut, &count) == A_OK)
    {
        QCOM_BSS_SCAN_INFO* info = pOut;
        int i,j;

        tx_thread_sleep(1000);
        for (i = 0; i < count; i++)
        {
            AT_PTF("ssid = ");
            {
                for(j = 0;j < info[i].ssid_len;j++)
                {
                    AT_PTF("%c",info[i].ssid[j]);
                }
                AT_PTF("\n");
            }
            AT_PTF("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",info[i].bssid[0],info[i].bssid[1],info[i].bssid[2],info[i].bssid[3],info[i].bssid[4],info[i].bssid[5]);
            AT_PTF("channel = %d\n",info[i].channel);
            AT_PTF("indicator = %d\n",info[i].rssi);
            AT_PTF("security = ");
            if(info[i].security_enabled)
            {
                AT_PTF("\n");
                if(info[i].rsn_auth || info[i].rsn_cipher)
                {
                    AT_PTF("RSN/WPA2= ");

                    if(info[i].rsn_auth)
                    {
                        AT_PTF("{");
                        if(info[i].rsn_auth & SECURITY_AUTH_1X)
                        {
                            AT_PTF("802.1X ");
                        }
                        if(info[i].rsn_auth & SECURITY_AUTH_PSK)
                        {
                            AT_PTF("PSK ");
                        }
                        AT_PTF("}");
                    }
                    if(info[i].rsn_cipher)
                    {
                        AT_PTF("{");
                        if(info[i].rsn_cipher & ATH_CIPHER_TYPE_WEP)
                        {
                            AT_PTF("WEP ");
                        }
                        if(info[i].rsn_cipher & ATH_CIPHER_TYPE_TKIP)
                        {
                            AT_PTF("TKIP ");
                        }
                        if(info[i].rsn_cipher & ATH_CIPHER_TYPE_CCMP)
                        {
                            AT_PTF("AES ");
                        }
                        AT_PTF("}");
                    }
                    AT_PTF("\n");
                }
                if(info[i].wpa_auth || info[i].wpa_cipher)
                {
                    AT_PTF("WPA= ");

                    if(info[i].wpa_auth)
                    {
                        AT_PTF("{");
                        if(info[i].wpa_auth & SECURITY_AUTH_1X)
                        {
                            AT_PTF("802.1X ");
                        }
                        if(info[i].wpa_auth & SECURITY_AUTH_PSK)
                        {
                            AT_PTF("PSK ");
                        } 
                        AT_PTF("}");
                    }

                    if(info[i].wpa_cipher)
                    {
                        AT_PTF("{");
                        if(info[i].wpa_cipher & ATH_CIPHER_TYPE_WEP)
                        {
                            AT_PTF("WEP ");
                        }
                        if(info[i].wpa_cipher & ATH_CIPHER_TYPE_TKIP)
                        {
                            AT_PTF("TKIP ");
                        }
                        if(info[i].wpa_cipher & ATH_CIPHER_TYPE_CCMP)
                        {
                            AT_PTF("AES ");
                        }
                        AT_PTF("}");
                    }
                    AT_PTF("\n");
                }
                if(info[i].rsn_cipher == 0 && info[i].wpa_cipher == 0)
                {
                    AT_PTF("WEP \n");
                }
            }
            else
            {
                AT_PTF("NONE! \n");
            }
            if (i != (count-1))
                AT_PTF("\n");    
        } 
        AT_PTF("shell> ");
    }

}

//"Set power mode =<enable>,<mode>,<wake period>,<sleep period>\n\r\tenable-1:enable, 0:disable\n\r\tmode-0:one shot, 1:periodic\n\r\twake period- wake time in mseconds\n\r\tsleep period- sleep time in mseconds \n\r"
void process_PM(char* cmd_str, A_UINT16 cmd_len)
{
	// add later
}

void process_WPM(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT32 mode;

    do 
    {
        if((*cmd_str == '?') || (cmd_len < 1))
        {
            qcom_power_get_mode(currentDeviceId, &mode);

            if( mode == REC_POWER)        //REC_POWER
            {
                AT_PTF("Power Mode   =   Power Save\n");
            }
        	else if (mode == MAX_PERF_POWER)  //MAX_PERF_POWER
            {
                AT_PTF("Power Mode   =   Max Perf\n");
            }
        	else {
                AT_PTF("Power Mode   =   Power Save\n");
            }
            
            break;
        }

        if(*cmd_str == '0')
        {
            mode = MAX_PERF_POWER;
        }
        else
        {
            mode = REC_POWER;
        }
        if (mode != REC_POWER && mode != MAX_PERF_POWER)
        {
            AT_PTF("ERROR: Invalid power mode\n");
            break;
        }

        qcom_power_set_mode(currentDeviceId, mode);

    } while(0);
}

void process_WAWEP(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT16 token_len = 64;
    char token_buf[64];
    char* argv[3] ;
    A_INT32 argc;

    if(cmd_len == 0)
        AT_PTF("ERROR, KEY index not supplied\n");

    do {
        //Key index
        token_len = (cmd_len<token_len) ? cmd_len : 32; //maximum len for this token
        A_MEMSET(token_buf, 0, 32);
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');

        int key_index;
        A_SSCANF(token_buf, "%d", &key_index);
        /* AT_PTF("KEY index: %d\n",key_index); */
        qcom_sec_set_wepkey_index(currentDeviceId, key_index);

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;
        //Get SSID here
        token_len = (cmd_len<token_len) ? cmd_len : 64; //maximum len for this token
        A_MEMSET(token_buf, 0, 64);
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');
        if(token_len != 0) 
        {
            argv[0] = NULL;
            argv[1] = NULL;
            argv[2] = (char*)token_buf;
            argc = 3;
            /* AT_PTF("KEY : %s\n",token_buf); */
            qcom_sec_set_encrypt_mode(currentDeviceId, WLAN_CRYPT_WEP_CRYPT); //WLAN_CRYPT_TYPE
            qcom_sta_connect_without_scan(currentDeviceId, token_buf);
        }
    } while(0);
}

//extern void qcom_wps_enable(A_UINT8 device_id, int enable);
//extern void qcom_wps_start(A_UINT8 device_id, int connect, int mode, char *pin);
void process_WAWPS(char* cmd_str, A_UINT16 cmd_len)
{
    char token_buf[32];
    A_UINT16  token_len;
    A_UINT8 wps_mode = 0;
    A_UINT32 connect_flag = 0;

    do
    {
        //Is connect after WPS <0/1>
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<token_len) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        A_SSCANF(token_buf, "%d", &connect_flag);
        if((connect_flag != 0) && (connect_flag != 1))
        {
            AT_PTF("ERROR: connect flag can be 0/1\n");
            break;
        }

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        //Get mode PIN/PUSH 
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');

        if(my_strncmpi(token_buf, "PUSH", sizeof("PUSH")-1) == 0)
        {
            wps_mode = 1;
        } 
        else if(my_strncmpi(token_buf, "PIN", sizeof("PIN")-1) == 0)
        {
            wps_mode = 0;
        }
        else
        {
            AT_PTF("ERROR: mode can be either PUSH/PIN\n");
            break;
        }

        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        qcom_wps_enable(currentDeviceId, 1);
        qcom_wps_start(currentDeviceId, connect_flag, wps_mode, cmd_str);        
        /* qcom_wps_start(1, 1, 0);         */

    } while(0);
}

void process_WWEPKEY(char* cmd_str, A_UINT16 cmd_len)
{
    char token_buf[32];
    A_UINT32 input;
    A_UINT16 index_len, token_len;
    
    do {        

        //get index
        index_len = 1;
        A_SSCANF(cmd_str, "%d", &input);
        
        index_len++; //skip ','
        cmd_str +=index_len;
        cmd_len -=index_len;
        //get key
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<token_len) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
        
        qcom_sec_set_wepkey(currentDeviceId, input, token_buf);
    } while(0);
} 

#define WAKE_MGR_WAKE_EVENT_TIMER       0x00000001
#define WAKE_MGR_WAKE_EVENT_GPIO30_HIGH 0x00000002
#define WAKE_MGR_WAKE_EVENT_GPIO30_LOW  0x00000004
#define WAKE_MGR_WAKE_EVENT_GPIO31_HIGH 0x00000008
#define WAKE_MGR_WAKE_EVENT_GPIO31_LOW  0x00000010

void process_PSUSE(char* cmd_str,A_UINT16 cmd_len){
    AT_PTF("Enable  power suspend\n");
    qcom_wlan_suspend_enable();
}

void process_PSUS(char* cmd_str,A_UINT16 cmd_len){
  
   A_UINT32  sleep_time;
   A_UINT32  flag;
   A_UINT32  gpio31_config;
   A_UINT32  gpio30_config;


   A_SSCANF(cmd_str, "%d,%d,%d", &sleep_time, &gpio31_config, &gpio30_config);

   if (sleep_time)
   	flag = WAKE_MGR_WAKE_EVENT_TIMER;
   else
	flag = 0;
   switch(gpio31_config)
   {
   case    1:
     flag |= WAKE_MGR_WAKE_EVENT_GPIO31_HIGH;
     break;
   case    2:
     flag |= WAKE_MGR_WAKE_EVENT_GPIO31_LOW;
     break;
   }

   switch(gpio30_config)
   {
   case    1:
     flag |= WAKE_MGR_WAKE_EVENT_GPIO30_HIGH;
     break;
   case    2:
     flag |= WAKE_MGR_WAKE_EVENT_GPIO30_LOW;
     break;
   }

   AT_PTF("flag: 0x%x\n", flag);
   if (flag == 0)
   {
     AT_PTF("ERROR: Please enter a valid wakeup options\n");
     return;
   }
   qcom_wm_suspend(sleep_time, flag);

}

void process_TO(char* cmd_str,A_UINT16 cmd_len)
{
    A_UINT32 input;

    do 
    {
        //query
        if((*cmd_str == '?') || (cmd_len <= 1))
        {
            AT_PTF("ERROR: Transmit Time Out is %d \n",g_tx_timeout);
            break;
        }
        A_SSCANF(cmd_str, "%d", &input);
        if((input > 100) || (input < 1))
        {
            AT_PTF("ERROR: Please enter a time out 1 to 100\n");
            break;
        }
        g_tx_timeout = input;
    } while(0);
}

void process_O(char* cmd_str, A_UINT16 cmd_len)
{
    if( tcp_socket || udp_socket )
    {
        switch_uart_mode(0);
    }
    else
    {
        AT_PTF("ERROR: No socket in connected state\n");
    }
}

void process_WPHYMODE(char* cmd_str, A_UINT16 cmd_len)
{
    A_UCHAR token_buf[32];
    A_UINT16  token_len;
    A_MEMSET(token_buf, 0, 32);
    token_len = (cmd_len<token_len) ? cmd_len : 32; //maximum len for this token
    copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, ',');
    AT_PTF("%s\n", cmd_str);
    AT_PTF("%s\n", token_buf);
    if (0 == A_STRCMP(cmd_str, "a")) {
        AT_PTF("Not support a\n");
        return;
    } else if (0 == A_STRCMP(cmd_str, "b")) {
        qcom_set_phy_mode(currentDeviceId, (A_UINT8)QCOM_11B_MODE);
    } else if (0 == A_STRCMP(cmd_str, "g")) {
        qcom_set_phy_mode(currentDeviceId, (A_UINT8)QCOM_11G_MODE);
    } else if (0 == A_STRCMP(cmd_str, "n")) {
        qcom_set_phy_mode(currentDeviceId, (A_UINT8)QCOM_11N_MODE);
    } else if (0 == A_STRCMP(cmd_str, "ht40")) {
        qcom_set_phy_mode(currentDeviceId, (A_UINT8)QCOM_HT40_MODE);
    } else {
        AT_PTF("Unknown wmode, only support b/g/n/ht40\n");
        return;
    }
}

#define IP_PTF(addr) AT_PTF("%d.%d.%d.%d", (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr) & 0xFF)
void process_NSET(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT32 addr, mask, gw;
    A_UINT16  token_len;
    A_UCHAR token_buf[32];
    unsigned int sbits;

    //We are querying Network information here
    if((*cmd_str == '?') || (cmd_len <= 1))
    {
        qcom_ip_address_get(currentDeviceId, &addr, &mask, &gw);
        AT_PTF("IP:");
        IP_PTF(addr);
        AT_PTF(", Mask:");
        IP_PTF(mask);
        AT_PTF(", Gateway:");
        IP_PTF(gw);
    }
    //We are setting Network information here
    else
    {
        do 
        {
            //Get the IP address
            A_MEMSET(token_buf, 0, 32);
            token_len = (cmd_len<token_len) ? cmd_len : 32; //maximum len for this token
            copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

            if( (token_len<7) || (parse_ipv4_ad(&addr, &sbits, (char*)token_buf) !=0))
            {
                AT_PTF("ERROR: Invalid IP address %s\n", token_buf);
                break;
            }
            addr = ntohl(addr);

            //Get the Mask address
            A_MEMSET(token_buf, 0, 32);
            token_len++; //skip ','
            cmd_str+=token_len;
            cmd_len-=token_len;

            token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
            copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
            if( (token_len<7) || (parse_ipv4_ad(&mask, &sbits, (char*)token_buf) != 0))
            {
                AT_PTF("ERROR: Invalid Mask address %s\n", token_buf);
                break;
            }
            mask = ntohl(mask);

            //Get the GW address
            A_MEMSET(token_buf, 0, 32);
            token_len++; //skip ','
            cmd_str+=token_len;
            cmd_len-=token_len;

            token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
            copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
            if((token_len<7) || (parse_ipv4_ad(&gw, &sbits, (char*)token_buf) !=0))
            {
                AT_PTF("ERROR: Invalid Gateway address %s\n", token_buf);
                break;
            }
            gw = ntohl(gw);
            if ( qcom_ip_address_set(currentDeviceId, addr, mask, gw) != A_OK)
                AT_PTF("ERROR\n");
            else     
                AT_PTF("OK\n");

        } while(0);
    }
}

/**********************************************************************
; AR dump Data(...) - Dump hexadecimal data
;*********************************************************************/
void ARdumpData(void *pData, char *pPrefix, unsigned int dataSize,unsigned int maxSize)
{
  unsigned int   asciiOK;
  unsigned char  ascii[32];
  unsigned int   ch;
  unsigned int   dumpSize;
  unsigned int   offset;
  unsigned int   offsetOK;
  unsigned char *pAscii;
  unsigned char *pByte;

  /*-----------------------------------------------------------------
  ; *pPrefix == '*' ==> Override 's_bDumpData' boolean MUST be BEFORE any'+' or '-')
  ; *pPrefix == '-' ==> Exclude ASCII trailer          MUST be BEFORE any'+')
  ; *pPrefix == '+' ==> Include offset prefix
  ;----------------------------------------------------------------*/
  if (s_bDumpData != FALSE || (pPrefix != NULL && *pPrefix == '*')) {
    asciiOK  = 1;
    offsetOK = 0;
    if (pPrefix != NULL) {
      if (*pPrefix == '*')
        ++pPrefix;
      if (*pPrefix == '-') {
        asciiOK = 0;
        ++pPrefix;
      }
      if (*pPrefix == '+') {
        offsetOK = 1;
        ++pPrefix;
      }
      AT_PTF("\n"AT_WMI_COMMAND": %s(Size[0x%p] = %u):\n",pPrefix,pData,dataSize);
    }
    if (dataSize != 0 && pData != NULL) {
      if (maxSize != 0 && dataSize > maxSize)
        dataSize = maxSize;
      offset    = 0;
      pByte     = (unsigned char *) pData;
      do {
        dumpSize = 0;
        if (offsetOK != 0) {
          AT_PTF("%04X...", offset);
          offset += 16;
        }
        pAscii = &ascii[0];
        do {
          ch = *pByte++;
          if (dumpSize == 7)
            AT_PTF("%02X-", ch);
          else
            AT_PTF("%02X ", ch);
          if (ch < ' ' || ch > 0x7E)
            ch = 183;
          *pAscii = ch;
          pAscii += asciiOK;
        } while (++dumpSize < dataSize && dumpSize < 16);
        dataSize -= dumpSize;
        *pAscii++ = '\r';
        *pAscii++ = '\n';
        *pAscii   = 0;
        while (asciiOK != 0 && dumpSize++ < 16)
          AT_PTF("   ");
        AT_PTF("%s",&ascii[0]);
      } while (dataSize != 0);
    }
  }
}

/*-----------------------------------------------------------------
; pCommand = WMI Header | WMI Payload
;----------------------------------------------------------------*/
void process_WMI(char *pCommand, unsigned short commandSize)
{
  unsigned short commandId;
  unsigned short commandInfo;

  commandId    = ((WMI_CMD_HDR *)pCommand)->commandId;
  commandInfo  = ((WMI_CMD_HDR *)pCommand)->info1;
  commandSize -= sizeof(WMI_CMD_HDR);
  pCommand     += sizeof(WMI_CMD_HDR);
  if (commandId == WMI_EXTENSION_CMDID && commandSize >= sizeof(WMIX_CMD_HDR)) {
    commandSize -= sizeof(WMIX_CMD_HDR);
    commandId    = ((WMIX_CMD_HDR *)pCommand)->commandId;
    pCommand     += sizeof(WMIX_CMD_HDR);
  }
  ARdumpData(pCommand,"+WMI Data",commandSize,0);
  _WMI_Dispatch(commandId,(unsigned char *)pCommand,commandSize,commandInfo);
  AT_PTF("\nOK\n");
}

void process_W(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT32 dev_mode;
    A_UINT16 channel = 0;
    A_UINT8 phymode;
    A_UINT8 chipState;
    A_UINT32 powermode = 2;
    A_UINT8 macAddr[6]; 

    /* ssid */
    A_UINT32 ssid_len = 33;
    A_CHAR ssid[ssid_len]; 
    A_MEMSET(ssid, 0, ssid_len);
    qcom_get_state(currentDeviceId, &chipState);
    if(chipState == 4)     
     {
       qcom_get_ssid(currentDeviceId, ssid);
     }
    AT_PTF("ssid         =   %s\n",ssid);

    /* PHY MODE */
    qcom_get_phy_mode(currentDeviceId, &phymode);
    if (phymode == 1) {
        AT_PTF("Phy Mode     =   a\n");
    } else if (phymode == 2) {
        AT_PTF("Phy Mode     =   g\n");
    } else if (phymode == 4) {
        AT_PTF("Phy Mode     =   b\n");
    } else {
        AT_PTF("Phy Mode     =   mixed\n");
    }

    /* Power Mode */
    /* default not in sleep mode */
  	/* Fix for IR?84238 : Information display is inconsistent when configure power save mode. 	*/
	//
	// Default value of power mode is 0 and it is Power Save
	//
    qcom_power_get_mode(currentDeviceId,&powermode); 
	// This is before Fix for IR?84238 // qcom_power_get_mode(&powermode);
    if( powermode == REC_POWER)        //REC_POWER
    {
        AT_PTF("Power Mode   =   Power Save\n");
    }
	//Fix for IR?84238 :  Added validation for checking the power mode to defaultvalue "0" of Maxperf
	else if (powermode == MAX_PERF_POWER )  //MAX_PERF_POWER
    {
        AT_PTF("Power Mode   =   Max Perf\n");
    }
	else {
        AT_PTF("Power Mode   =   Power Save\n");
    }

    /* MAC */
    A_MEMSET(&macAddr, 0, sizeof (macAddr));
    qcom_mac_get(currentDeviceId, (A_UINT8 *) & macAddr);
    AT_PTF("Mac Addr     =   %x:%x:%x:%x:%x:%x\n",
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);

    /* Mode */
    qcom_op_get_mode(currentDeviceId, &dev_mode);
    char *modeDesc[] = { "Station", "AP", "Unknown" };
    AT_PTF("Mode         =   %s\n", modeDesc[dev_mode]);

    /* Channel */
    if(chipState ==QCOM_WLAN_LINK_STATE_CONNECTED_STATE)
       {
          qcom_get_channel(currentDeviceId, &channel);
          if (channel >= 2412) {
            channel = (((channel - 2412) / 5) + 1);
           }
       }
    AT_PTF("Channel      =   %d\n", channel);

}

void process_WRSSI(char* cmd_str, A_UINT16 cmd_len)
{
    A_UINT8 rssi;
    A_UINT8 chipState;
    /* QCOM_WLAN_DEV_MODE devMode; */
    A_UINT32 devMode;

    qcom_op_get_mode(currentDeviceId, &devMode);
    if(devMode == 0){
        qcom_get_state(currentDeviceId, &chipState);
        if(chipState == 4){
            qcom_sta_get_rssi(currentDeviceId, &rssi);
            AT_PTF("indicator = %d dB\n", rssi);
        }else{
            AT_PTF("Not associate\n");
        }
    }else{
        AT_PTF("RSSI not in AP mode\n");
    }
}

void process_WA(char* cmd_str, A_UINT16 cmd_len)
{
    A_UCHAR token_buf[64];
    A_UINT16  token_len;

    do 
    {
        //get the SSID to connect to
        A_MEMSET(token_buf, 0, 64);
        token_len = 64;

        token_len = (cmd_len<token_len) ? cmd_len : 64; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 0, '\r');

        if(token_len != 0) 
        {
            // we need to add action in driver to judge whether success of fail
            qcom_sta_connect_with_scan(currentDeviceId, (A_CHAR *)token_buf);
        }
    } while(0);
}

void process_NDHCP(char* cmd_str, A_UINT16 cmd_len)
{
    if(*cmd_str == '2')         //DHCP release here
    {
        // no action now
    }
    else                        //Start DHCP client if not running already
    {
        /* 1: enbale; 0:disable */
        qcom_dhcpc_enable(currentDeviceId, 1);
    }
}

void process_NCLOSE(char* cmd_str, A_UINT16 cmd_len)
{
    if( !(tcp_socket || udp_socket) )
    {
        AT_PTF("ERROR : Socket is not opened \n\r");
    }
    else{
        if(tcp_socket){
            AT_PTF(" Close TCP Socket handle = 0x%x \n\r",tcp_socket);
            /* if(socket_state_connected) */
            {
                socket_state_connected  = 0;
                qcom_socket_close(tcp_socket);
                stcp_client_fd =0; //empty client fd in stcp
                tcp_socket = 0;
            }
        }
        if(udp_socket){
            AT_PTF(" Close UDP Socket handle = 0x%x \n\r",udp_socket);
            /* if(socket_state_connected) */
            {
                socket_state_connected  = 0;
                qcom_socket_close(udp_socket);
                sudp_client =0; //empty client in sudp
                udp_socket = 0;
            }
        }

        AT_PTF("OK \n\r");
    }
}


void process_NSOCK(char* cmd_str, A_UINT16 cmd_len)
{
    do 
    {
        if( tcp_socket || udp_socket)
        {
            AT_PTF("ERROR: A socket is already open \n");
            break;
        }
        if(my_strncmpi(cmd_str, "TCP", sizeof("TCP")-1) == 0)
        {
            tcp_socket = qcom_socket(AF_INET, SOCK_STREAM, 0);
            if (tcp_socket < 0) {
                AT_PTF("TCP socket open error\n");
                return;
            }
            AT_PTF("TCP handle: 0x%x\n",tcp_socket);
            break;
        }
        if(my_strncmpi(cmd_str, "UDP", sizeof("UDP")-1) == 0)
        {
            udp_socket = qcom_socket(PF_INET, SOCK_DGRAM, 0);
            if (udp_socket < 0) {
                AT_PTF("UDP socket open error\n");
                return;
            }
            break;
            AT_PTF("UDP handle: 0x%x\n",udp_socket);
        }
        AT_PTF("ERROR: invalid arguments \n");
    } while(0);
}

void process_BSIZE(char* cmd_str,A_UINT16 cmd_len)
{
    A_UINT32 input; 
    do 
    {
        //query
        if((*cmd_str == '?') || (cmd_len <= 1))
        {
            AT_PTF("Buffer size is %d \n\r",g_data_buffer_size);
            break;
        }

        A_SSCANF(cmd_str, "%d", &input);
        if((input < 1) ||( input > 1200))
        {
            AT_PTF(" ERROR:Please enter a valid size (1 to 1200)\n"); 
            break;
        }
        g_data_buffer_size = input;
        mem_free(data_buffer);
        data_buffer = mem_alloc(g_data_buffer_size );
        memset(data_buffer,0,g_data_buffer_size);
        AT_PTF("malloc data buffer! \n");
    } while(0);
}

void at_tcp_client_receive(A_UINT32 param_unused)
{
    A_INT32 recvBytes = 0;
    while (1) {
        recvBytes = qcom_recv(tcp_socket, (char *) pDataBuffer, recvBuffSize, 0);
        if (recvBytes <= 0) {
        } else {
           
            at_console_write( pDataBuffer, &recvBytes);
        }

        /* qcom_thread_msleep(2000); */
        if(g_operating_mode==UART_AT_CMD_MODE){
            break;
        }
    }
    qcom_task_exit();        //delete the rx thread for data

}

void process_WD(char* cmd_str, A_UINT16 cmd_len)
{
    qcom_disconnect(currentDeviceId);
}

//
//  TCP
//
void process_NCTCP(char* cmd_str, A_UINT16 cmd_len)
{
    struct sockaddr_in sock_addr;
    int ret;
    char token_buf[32];
    A_UINT16  token_len;
    A_UINT32 addr;
    unsigned int sbits;

    memset((char*)&sock_addr, 0, sizeof (struct sockaddr_in));
    do {
        if(!tcp_socket)
        {
            AT_PTF("ERROR: No socket is opened. Open a socket using ATNSOCK first\n");
            break;
        }
        if((tcp_socket) && (socket_state_connected ==1))
        {
            AT_PTF("ERROR: A socket is already open and connected \n");
            break;
        }
        if(udp_socket)
        {
            AT_PTF("Error: Attempting a tcp connection with udp socket handle\n");
            break;
        }

        //get the ip address
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 31; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if( (token_len<7) || (parse_ipv4_ad(&addr, &sbits, (char*)token_buf) !=0))
        {
            AT_PTF("ERROR: Invalid IP address %s\n", token_buf);
            break;
        }
        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        //get the port number 
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
        if(token_len==0)
        {
            AT_PTF("ERROR: Port number not supplied\n");
            break;
        }

        sock_addr.sin_addr.s_addr = addr;
        int port;
        A_SSCANF(token_buf, "%d", &port);
        sock_addr.sin_port = htons(port);
        sock_addr.sin_family = AF_INET;// or ATH_AF_INET ?
        ret = qcom_connect(tcp_socket, (struct sockaddr *) &sock_addr, sizeof (struct sockaddr_in));
        if (ret < 0) {
            AT_PTF("Failed to connect socket %d.\n", tcp_socket);
            return ; 
        }
        switch_uart_mode(0); //switch to data mode
        qcom_task_start(at_tcp_client_receive, 0, 2048, 50);
    } while(0);

}

void at_tcp_server_receive(A_UINT32 param_unused)
{
    A_INT32 recvBytes = 0;
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet;

    tmo.tv_sec = 600;
    tmo.tv_usec=0;
    FD_ZERO(&sockSet);
    FD_SET(stcp_client_fd, &sockSet);
    while (1) {
        /* Wait for Input */
        AT_PTF("[s]tcp select start...0x%x\n", stcp_client_fd);
        fdAct = qcom_select(stcp_client_fd + 1, &sockSet, NULL, NULL, &tmo);  //k_select()
        if (0 != fdAct) {
            if (FD_ISSET(stcp_client_fd, &sockSet)) {
                memset((char*)pDataBuffer, 0, 1024);
                recvBytes =
                    qcom_recv(stcp_client_fd, (char *) pDataBuffer, recvBuffSize,
                            0);
                if (recvBytes <= 0) {
                    AT_PTF("tcp server rx error!\n");
                    break;
                } else {
                    AT_PTF("[s]qcom_recvfrom %d\n", recvBytes);
                    at_console_write( pDataBuffer, &recvBytes);
                }

            }
        } else {
            AT_PTF("[s]tcp select tmo...[%d] %d\n", fdAct, g_operating_mode);
            FD_ZERO(&sockSet);
            FD_SET(stcp_client_fd, &sockSet);
		}

        if(g_operating_mode==UART_AT_CMD_MODE){
            break;
        }
    }
    qcom_power_set_mode(0, REC_POWER);
    qcom_task_exit();
}
    void
at_tcp_rx_handle()
{
    A_INT32 ret = 0;
    A_UINT32 clientIp;
    A_UINT16 clientPort;
    A_UINT32 isFirst = 0;
    struct sockaddr_in clientAddr;
    A_INT32 len = sizeof (struct sockaddr_in);
    struct timeval tmo;
    A_INT32 fdAct = 0;
    q_fd_set sockSet;
    tmo.tv_sec = 2;
    tmo.tv_usec = 0;

    FD_ZERO(&sockSet);
    FD_SET(tcp_socket, &sockSet);
    if (0 == isFirst) {
        ret = qcom_listen(tcp_socket, 10);
        if (ret < 0) {
            /* Close Socket */
            /*AT_PTF("Failed to listen socket \n");*/
        }
        isFirst = 1;
    }
    AT_PTF("[s]listen socket 0x%x\n", tcp_socket);
    /* AT_PTF("tcp socket %d\n",tcp_socket); */
    while (1) {
        fdAct = qcom_select(tcp_socket + 1, &sockSet, NULL, NULL, &tmo);  
        if (fdAct != 0) {
            AT_PTF("server select socket! \n");
            break;
        } else {
            AT_PTF("[s]select tmo...[%d]\n", fdAct);
			FD_ZERO(&sockSet);
            FD_SET(tcp_socket, &sockSet);
    	}
    }
    stcp_client_fd = qcom_accept(tcp_socket, (struct sockaddr *) &clientAddr, &len);
    if (stcp_client_fd < 0) {
        /* Close Socket */
        AT_PTF("Failed to accept socket %d.\n", stcp_client_fd);
    }

    AT_PTF("[s]accept socket 0x%x 0x%x\n", stcp_client_fd, tcp_socket);
    clientIp = ntohl(clientAddr.sin_addr.s_addr);
    clientPort = ntohs(clientAddr.sin_port);
    AT_PTF("Receiving from 0x%x Remote port:%d \r\n",
            clientIp, clientPort);

    switch_uart_mode(0); //switch to data mode
    qcom_task_start(at_tcp_server_receive, 0, 2048, 50);

}

void process_NSTCP(char* cmd_str, A_UINT16 cmd_len)
{
    A_INT32 ret = 0;
    struct sockaddr_in sock_addr;
    memset((char*)&sock_addr, 0, sizeof (struct sockaddr_in));

    do{
        if(tcp_socket == 0)
        {
            AT_PTF("ERROR: No socket is opened. Open a socket using ATNSOCK first\n");
            break;
        }

        if(udp_socket)
        {
            AT_PTF("ERROR: Attempting a tcp connection with udp socket handle\n");
            break;
        }

        //Get the parameters here. Port number
        int port;
        A_SSCANF(cmd_str, "%d", &port);
        sock_addr.sin_port= htons(port);
        if((port < 1024 )|| (port> 65534))
        {
            AT_PTF("Invalid Port number! Please supply a port number > 1024\n");
            break;
        }
        sock_addr.sin_family = AF_INET;
        ret = qcom_bind(tcp_socket, (struct sockaddr *) &sock_addr, sizeof (struct sockaddr_in));
        if (ret < 0) {
            AT_PTF("Failed to bind socket %d.\n", tcp_socket);
            return ; 
        }

        AT_PTF("[s]Socket Binding successful 0x%x %d\n", tcp_socket, sock_addr.sin_port);
        qcom_power_set_mode(0, MAX_PERF_POWER);
        at_tcp_rx_handle();
    }while(0);

}


//
//  UDP
//
void at_udp_client_receive(A_UINT32 param_unused){

    A_INT32 fdAct = 0;
    A_INT32 recvBytes;
    q_fd_set sockSet, master;

    /* Init fd_set */
    FD_ZERO(&master);
    FD_SET(udp_socket, &master);
    struct timeval tmo;
    tmo.tv_sec = 30;
    tmo.tv_usec=0;
    remote_udp_size = sizeof(remote_udp_addr);
    while (1) {
        /* Wait for Input */
	    sockSet = master; 
        fdAct = qcom_select(udp_socket + 1, &sockSet, NULL, NULL, &tmo);  
        if (0 != fdAct) {
            if (FD_ISSET(udp_socket, &sockSet)) {
                memset((char*)pDataBuffer, 0, 1024);
                recvBytes = qcom_recvfrom(udp_socket, (char*) pDataBuffer,
                        recvBuffSize, 0,
                        (struct sockaddr *) &remote_udp_addr, &remote_udp_size);
                if (recvBytes <= 0) {
                    AT_PTF("UDP Client receive error\r\n");
                } else {
                    at_console_write( pDataBuffer, &recvBytes);
                }
            }
        }
        if(g_operating_mode==UART_AT_CMD_MODE){
            break;
        }
    }
    qcom_task_exit();
}


void at_udp_server_receive(A_UINT32 param_unused){
    A_INT32 recvBytes = 0;
    A_INT32 fromSize = 0;
    q_fd_set sockSet,master;
    struct timeval tmo;
    A_INT32 fdAct = 0;

    /* Prepare IP address & port */
    memset(&fromAddr, 0, sizeof (struct sockaddr_in));
    fromSize = sizeof (struct sockaddr_in);

    /* Init fd_set */
    FD_ZERO(&master);
    FD_SET(udp_socket, &master);
    tmo.tv_sec = 300;
    tmo.tv_usec = 0;
    while (1) {
        /* Wait for Input */
	    sockSet = master;
        fdAct = qcom_select(udp_socket + 1, &sockSet, NULL, NULL, &tmo);   //k_select()
        if (0 != fdAct) {
            if (FD_ISSET(udp_socket, &sockSet)) {
                if(sudp_client ==0){
                    sudp_client=1;
                    switch_uart_mode(0); //switch to data mode
                }
                memset((char*)pDataBuffer, 0, 1024);
                recvBytes = qcom_recvfrom(udp_socket, (char*) pDataBuffer,
                        recvBuffSize, 0,
                        (struct sockaddr *) &fromAddr, &fromSize);
                if (recvBytes <= 0) {
                    AT_PTF("udp server rx error!\n");
                }
                else
                {
                    AT_PTF("[s]udp recv %d bytes\n", recvBytes);                    
                }
                at_console_write( pDataBuffer, &recvBytes);
            }
        }
        /* qcom_thread_msleep(2000); */
        if(g_operating_mode==UART_AT_CMD_MODE){
            break;
        }
    }

    qcom_task_exit();        //delete the rx thread for data
}

void process_NCUDP(char* cmd_str,A_UINT16 cmd_len)
{
    /* int ret; */
    char token_buf[32];
    A_UINT16  token_len;
    A_UINT32 addr;
    unsigned int sbits;

    memset((char*)&remote_udp_addr, 0, sizeof (struct sockaddr_in));
    /* AT_PTF("string:  %s\n", cmd_str); */
    do {
        if(!udp_socket)
        {
            AT_PTF("ERROR: No socket is opened. Open a socket using ATNSOCK first\n");
            break;
        }
        if((udp_socket) && (socket_state_connected ==1))
        {
            AT_PTF("ERROR: A socket is already open and connected \n");
            break;
        }
        if(tcp_socket)
        {
            AT_PTF("Error: Attempting a udp connection with tcp socket handle\n");
            break;
        }

        //get the ip address
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 31; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');

        if( (token_len<7) || (parse_ipv4_ad(&addr, &sbits, (char*)token_buf) !=0))
        {
            AT_PTF("ERROR: Invalid IP address %s\n", token_buf);
            break;
        }
        token_len++; //skip ','
        cmd_str+=token_len;
        cmd_len-=token_len;

        //get the port number 
        A_MEMSET(token_buf, 0, 32);
        token_len = (cmd_len<32) ? cmd_len : 32; //maximum len for this token
        copy_data_upto(cmd_str, token_buf, &cmd_len, &token_len, 1, ',');
        if(token_len==0)
        {
            AT_PTF("ERROR: Port number not supplied\n");
            break;
        }

        remote_udp_addr.sin_addr.s_addr = addr;
        int port;
        A_SSCANF(token_buf, "%d", &port);
        remote_udp_addr.sin_port = htons(port);
        remote_udp_addr.sin_family = AF_INET;// or ATH_AF_INET ?
        switch_uart_mode(0); //switch to data mode
        qcom_task_start(at_udp_client_receive, 0, 2048, 50);
    } while(0);

}
void process_NSUDP(char* cmd_str, A_UINT16 cmd_len)
{
    int ret;

    do{    
        if( udp_socket == 0)
        {
            AT_PTF("ERROR: No socket is opened. Open a socket using ATNSOCK first\n");
            break;
        }

        int port;
        A_SSCANF(cmd_str, "%d", &port);
        if((port < 1024 )|| (port > 65534))
        {
            AT_PTF("Invalid Port number! Please enter a port number > 1024\n");
            break;
        }

        memset(&server_udp_addr, 0, sizeof (struct sockaddr_in));
        server_udp_addr.sin_port = htons(port);
        server_udp_addr.sin_family = AF_INET;

        ret = qcom_bind(udp_socket, (struct sockaddr *)&server_udp_addr, sizeof (struct sockaddr_in));
        if (ret < 0) {
            AT_PTF("Failed to bind socket %d.\n", udp_socket);
            return ; 
        }

        AT_PTF("Socket Binding successful\n");
        qcom_task_start(at_udp_server_receive, 0, 2048, 50);

    }while(0);
}

