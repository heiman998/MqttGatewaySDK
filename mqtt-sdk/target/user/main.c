//#include "StackTrace.h"
//#include "MQTTPacket.h"
//#include "MQTTPacket.h"

#include "math.h"
#include "qcom_common.h"
#include "qca_json.h"
#include "hm_app.h"
#include "hm_keyAndSmartLink.h"
#include "qca_ap_connect.h"
#include "qca_flash_param.h"
#include "hm_net_data_manage.h"

//#include "MQTTPacket.h"
//#include <string.h>
#include "MQTTinterface.h"
//#include "hmlink_system.h"
/* Includes ----------------------------------------------------------------*/

#include "../lt_sample/ltm3210.h"
//#include "xlink_sdk.h"
#include "xlink_All_Device.h"
#include "Xlink_Head_Adaptation.h"
//#include "xlink_tcp_client.h"
#include "xsdk_config.h"
#include "xlink_data.h" //ctype.h
#include "ServerDataHandle.h"

#include "t_log.h"
#include "hm_Head_adaptation.h"


//#include <iostream>  
//#include <string>  
//#include <string.h> 

//#include "string.h"

//#include "#include <string.h>.h" 





/* Private define ------------------------------------------------------------*/
#define TCP_CLIENT_STACK_SIZE		1024	
#define TCP_CLIENT_BUF_SIZE			2048

#define TCP_SERVER_PORT				1883 //

#define TCP_SERVER_IP_ADDR         "119.29.224.84"//

#ifdef HEIMAN_APP
#define TCP_SERVER_ADDR				"io.heiman.cn"
#else
#ifdef HM_TEST
#define TCP_SERVER_ADDR				"q.emqtt.com"//
#else
#ifdef YUNZHIYI_APP
#define TCP_SERVER_ADDR				"123.cn"
#endif
#endif
#endif

#define UDP_SERVER_PORT					5987
#define UDP_SERVER_BUF_SIZE				1024

#define START_STATION_WITH_SCAN

#define WIFI_TIMEOUT 5000

#define WIFI_SSID "WIFI_8A05"//"chen wifi"//HiWiFi_50FE5A"
#define WIFI_PASSWD "heiman2017.."//"qwertyuiop"//"123456789"


int wifi_connect_flag=0;

#define  UDP_SENDBUFFER_LEN  (1074)
char  xlink_udp_buffer[UDP_SENDBUFFER_LEN];
short xlink_udp_bufflen = 0;
extern tGlobal_net_type tGlobal_net;
uint16_t sysTimeZone;
extern NETSENDINFO netSendInfo;
extern NETRCVINFO   netRcvInfo;
extern DEV_MYSELF_INFO hmIotsDevInfo;
extern GWSTATFORTIME gwStateForRealTime;
char *alarm_report_data;
uint16_t alarm_report_len;
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int tcp_fd = -1;
int udp_fd = -1;
/********************v3**************************/
#define TICKS_DIFF(cur, prev) ((cur) >= (prev)) ? ((cur)-(prev)) : ((0xFFFFFFFF-(prev))+(cur)+1)

#define  XLINK_DEBUG_ON 1
#define XLINK_CONFIG_BUFFER_SIZE__ 180

typedef union {
	unsigned char byte;
	struct {
		unsigned char isConnectedServer :1;
		unsigned char isConnectWIFI :1;
		unsigned char islogined :1;
		unsigned char isTCPReInit :1;
		unsigned char isDisConnect :1;
		unsigned char isNeedSendSta :1;
		unsigned char res :2;
	} bit;
} XLINK_LOCAL_STA;
typedef struct {
	unsigned char xsdk_config[XLINK_CONFIG_BUFFER_SIZE__];
	unsigned char pid_key[66];
}xlink_sdk_para_t;


xlink_sdk_para_t   xlink_sdk_para_cfg;

static  unsigned int   PreTime = 0;
static  unsigned int   CurTime = 0;

XLINK_USER_CONFIG  user_config_call;
volatile XLINK_LOCAL_STA  g_AllSta;
volatile static unsigned char sdk_inner_buffer[TCP_CLIENT_BUF_SIZE];
volatile static unsigned char xlink_tcp_rcv_buffer[1024];

HM_USER_CONFIG  hm_config_call;//add sdk


#define qca_time_ms()				(time_ms()*1000/1024)

//hardy add
NETWORK_USER_CONFIG  network_config_call;
//extern void hmlink_process_udp_data(unsigned char *data, unsigned int datalength, _sockaddr_in *addr);
//int hmlink_tcp_PushData(unsigned char * data, short datalen);

/********************v3**************************/

extern NETRCVINFO   netRcvInfo;

//test
#define UART_PACKET_MAX_LEN	256
#define BUFFSIZE 512

unsigned char recv_uart_buf1[BUFFSIZE];

static qcom_uart_para uart_paramer1;

void uart_recv1(void);

int uart_reslove_packet1(unsigned char **packet_start, unsigned char **packet_end, 
    unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end);


//extern  MQTTPacket_connectData data1;


void hmlink_process_udp_data(unsigned char *data, unsigned int datalength, xlink_addr *addr) {
	char cpdata[1024];
	memcpy(cpdata,data,datalength);
	cpdata[datalength] = '\0';
	printf("li test for udp :%d,data:%s\r\n",datalength,cpdata);
	//user_config_call.process_udp_data(data, datalength, addr);
}
#if 0
int hmlink_tcp_PushData(unsigned char * data, short datalen){
}
#endif

void cp_to_rcv_buf(char *data,unsigned short len,uint8_t rcvtype,uint32_t appDevId)
{
	static uint8_t full;
	if((netRcvInfo.netrcvnum < NET_RCV_PK_NUM) && (len < 1024))
	{
		full = 0;//接收缓存满的标志位清�?
		netRcvInfo.netrcvflag = 1;
		memcpy(&netRcvInfo.netrcvbuf[netRcvInfo.netrcvnum * NET_RCV_BUF_BASIC],data,len);
		netRcvInfo.netrcvbuf[netRcvInfo.netrcvnum * NET_RCV_BUF_BASIC + len] = '\0';
		netRcvInfo.netappinfo[netRcvInfo.netrcvnum].dealType = rcvtype;
		netRcvInfo.netappinfo[netRcvInfo.netrcvnum].appDevId = appDevId;
		netRcvInfo.netrcvnum ++;
	}else if(netRcvInfo.netrcvnum >= NET_RCV_PK_NUM){
		if(full == 0)//第一次进入，接收缓存满的标志位置1，第二次进入如果缓存满标志位还为1，则清除接收缓存
		{
			full = 1;
		}else{
			memset(netRcvInfo.netrcvbuf,'\0',netRcvInfo.netrcvnum * NET_RCV_BUF_BASIC);
			netRcvInfo.netrcvnum = 0;
			netRcvInfo.netrcvflag = 0;
			memset(netRcvInfo.netappinfo,0,sizeof(APPINFO) * NET_RCV_PK_NUM);
		}
		printf( "the rcv buf is not valid,the buf num:%d\r\n",netRcvInfo.netrcvnum);
	}
}


int xlink_tcp_send_data(unsigned char* data,unsigned  int datalength)
{
	printf ("xlink_tcp_send_data:datalength=%d.\n",datalength);
   	return qcom_send(tcp_fd,(char *)data,datalength,0);
}
void xlink_uip_udp_send(xlink_addr *addr, char *data, int datalen) {

     printf ("xlink_uip_udp_send:datalength=%d\n",datalen);
     //printf("udp (fd = %d %d)port %d  addr : %x\n", addr->socket,datalen,addr->port, addr->ip);

	 struct sockaddr_in fromAddr; 
	 fromAddr.sin_addr.s_addr = addr->sin_addr.s_addr;
	 fromAddr.sin_family = AF_INET;
	 fromAddr.sin_port = addr->sin_port;

     printf("udp (fd = %d %d)port %d  addr : %x\n", addr->socket,datalen,fromAddr.sin_port, fromAddr.sin_addr.s_addr);

	 memset(xlink_udp_buffer,0,sizeof(xlink_udp_buffer));
	 if(datalen <= UDP_SENDBUFFER_LEN){
		memcpy(xlink_udp_buffer,data,datalen);
		xlink_udp_bufflen = datalen;
		qcom_sendto(addr->socket, xlink_udp_buffer, xlink_udp_bufflen, 0, (struct sockaddr*)&fromAddr, sizeof (struct sockaddr_in));
                  //qcom_sendto(udp_fd, (char *)recvBuff, recvBytes, 0, (struct sockaddr*)&fromAddr, sizeof (struct sockaddr_in));
	 }
}

int xlink_write_flash_cb(char* data, unsigned int datalength)
{
	
	int ret;
	printf("flash param save : %s\r\n", data);
	memcpy(xlink_sdk_para_cfg.xsdk_config,data,datalength);
    	ret = qca_save_flash_params(START_ADDR_FLASH_XLINK_INFO_ADDR,(unsigned char*)&xlink_sdk_para_cfg, sizeof(xlink_sdk_para_t));

	if(ret != 0){
		printf("flash param save fail.\r\n");
		return -1;
	}

	return datalength;
}
int xlink_read_flash_cb( char* data,unsigned int datalength)
{
	int ret = -1;
	memset(&xlink_sdk_para_cfg,0,sizeof(xlink_sdk_para_t));
	ret =qca_load_flash_params(START_ADDR_FLASH_XLINK_INFO_ADDR, (unsigned char*)&xlink_sdk_para_cfg, sizeof(xlink_sdk_para_t));
    if(ret != 0){
        printf("flash param load fail.\r\n");
        return;
    }
    memcpy(data, xlink_sdk_para_cfg.xsdk_config, datalength);
    printf("flash param load datalength: %d\r\n", datalength);
    return datalength;
}



int ConnectTCPSever(char *domain,int port)
{

                struct sockaddr_in	remoteAddr;
                int server_ip = 0;;
                int tcpc_fd = -1;
                int ret = 0;
                qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);	
                  tcpc_fd = qcom_socket(PF_INET, SOCK_STREAM, 0);
		if (tcpc_fd < 0)
		{
			printf("open tcp client socket error.\r\n");
			return -1;
		}
		#if 0
	        int ipAddress = 0;
                	if (qcom_dnsc_get_host_by_name(domain, &ipAddress) == 0) {
                		//server_ip = LE_BE_32(ipAddress);
                		server_ip = ((((unsigned int) (ipAddress) & 0xff000000) >> 24) | ((((unsigned int) (ipAddress) & 0x00ff0000) >> 8)) \
			| (((unsigned int) (ipAddress) & 0x0000ff00) << 8) | (((unsigned int) (ipAddress) & 0x000000ff) << 24));
                	}
                printf(" get host ip %d.%d.%d.%d\r\n", (server_ip & 0xff000000) >> 24, (server_ip & 0x00ff0000) >> 16, (server_ip & 0x0000ff00) >> 8, server_ip & 0x000000ff);
                #else
                server_ip = qca_inet_addr(TCP_SERVER_ADDR);
            		A_PRINTF("Server Addr:\n");
            		printf("%d.%d.%d.%d\n", 
            		(server_ip) & 0xFF, (server_ip) >> 8 & 0xFF,
            		(server_ip) >> 16 & 0xFF, (server_ip) >> 24 & 0xFF);

                #endif
		//== ipv4 connect 
		memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_addr.s_addr = htonl(server_ip);
		remoteAddr.sin_port = htons(port);

		ret = qcom_connect(tcpc_fd, (struct sockaddr *) &remoteAddr, sizeof (struct sockaddr_in));
		if (ret < 0)
		{
			printf("Connect Failed\r\n");
			qcom_socket_close(tcpc_fd);
			return -1;
		}

		printf("Connect %lu.%lu.%lu.%lu OK\r\n",
			(remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
			(remoteAddr.sin_addr.s_addr) >> 16 & 0xFF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);
			
			return tcpc_fd;
}

extern char TCPSocket_connectd;

int tcp_init(void)
{
    struct sockaddr_in	remoteAddr;
	int ret = 0;
	int server_ip = 0;
	uint8_t SERVER_IP_buff[40]={0x31,0x31,0x39,0x2e,0x32,0x39,0x2e,0x32,0x32,0x34,0x2e,0x38,0x34};//119.29.224.84"
             tcp_fd = qcom_socket(PF_INET, SOCK_STREAM, 0);
            if (tcp_fd < 0)
            {
                printf("open tcp client socket error.\r\n");
                return 0;
            }
            
           /* server_ip = qca_inet_addr(TCP_SERVER_ADDR);
            A_PRINTF("Server Addr:\n");
            printf("%d.%d.%d.%d fd:%d\n", 
            (server_ip) & 0xFF, (server_ip) >> 8 & 0xFF,
            (server_ip) >> 16 & 0xFF, (server_ip) >> 24 & 0xFF,tcp_fd);*/
       //     qcom_dnsc_get_host_by_name(TCP_SERVER_ADDR, &server_ip);  //DNS
       //IP
       		
			if(strlen(hmIotsDevInfo.ipaddr)>9)
			{
			server_ip = qca_inet_addr(hmIotsDevInfo.ipaddr);
			printf("1 current mqtt server ip:%s\n",hmIotsDevInfo.ipaddr);
			}
			else{
       		server_ip = qca_inet_addr(TCP_SERVER_IP_ADDR);//server_ip = qca_inet_addr(SERVER_IP_buff);//
       		printf("2 current mqtt server ip:%s\n",SERVER_IP_buff);
			}
			printf("Server Addr:%d.%d.%d.%d\n", 
			(server_ip) & 0xFF, (server_ip) >> 8 & 0xFF,
			(server_ip) >> 16 & 0xFF, (server_ip) >> 24 & 0xFF);
    
            //== ipv4 connect 
            memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
            remoteAddr.sin_family = AF_INET;
            remoteAddr.sin_addr.s_addr = htonl(server_ip);
            remoteAddr.sin_port = htons(TCP_SERVER_PORT);
    
            ret = qcom_connect(tcp_fd, (struct sockaddr *) &remoteAddr, sizeof (struct sockaddr_in));
            if (ret < 0)
            {
                printf("Connect Failed\r\n");
                qcom_socket_close(tcp_fd);
                 printf("Connect Failed %d\r\n",tcp_fd);
              //   xlink_demo_setServerStatus(0, 0); //xlink
            //  network_config_call.TCPsetServerStatus(0);
            TCPSocket_connectd=0;
			  printf("----TCPSocket_connectd =0\r\n");
                 tcp_fd = -1;
                 return -1;
            }

            printf("Connect %lu.%lu.%lu.%lu OK\r\n",
                (remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
                (remoteAddr.sin_addr.s_addr) >> 16 & 0xFF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);
           // xlink_demo_setServerStatus(1, 0); //xlink
          // network_config_call.TCPsetServerStatus(1);
          TCPSocket_connectd=1;printf("----TCPSocket_connectd =1--\r\n");
 		  MQTTclient_connState_init();

                return tcp_fd;
}

int udp_init()
{
	unsigned int		ret = 0;
	struct sockaddr_in	localAddr;

	qcom_power_set_mode(0, MAX_PERF_POWER);		// 锟剿筹拷锟酵癸拷锟斤拷模�?

	udp_fd = qcom_socket(PF_INET, SOCK_DGRAM, 0);
	if (udp_fd < 0)
	{
		printf("create udp server socket error %d.\r\n",udp_fd);
		return -1;
	}

    printf("create udp server socket ok :%d.\r\n",udp_fd);

	/* Connect Socket */
	memset(&localAddr, 0, sizeof (struct sockaddr_in));
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_family = AF_INET;

	
	localAddr.sin_port = htons(UDP_SERVER_PORT);


	ret = qcom_bind(udp_fd, (struct sockaddr *) &localAddr, sizeof (struct sockaddr_in));
	if (ret < 0)
	{
                  printf("create udp server socket error %d.\r\n",udp_fd);
		qcom_socket_close(udp_fd);
		return -1;
	}

	/* Packet Handle */
	//qca_udp_server_handle();

	//qcom_task_exit();
}

void ap_connect_auto(void)//auto
{
	int ret = 0;
	int wificonnect_flag=0;
	 printf("-----------ap connect auto----01 \n");
	 printf("ap_connect wifi route\r\n");   // debugdebug
#ifdef START_STATION_WITH_SCAN
	ret = qca_start_station_mode(WITH_SCAN, WIFI_SSID, WIFI_PASSWD, WIFI_TIMEOUT);
#else
	/* Set auth and encrypt first if start station mode without scan */
	qca_ap_set_auth_encrypt(WLAN_AUTH_WPA2_PSK, WLAN_CRYPT_AES_CRYPT);
	ret = qca_start_station_mode(WITHOUT_SCAN, WIFI_SSID, WIFI_PASSWD, WIFI_TIMEOUT);
#endif
	if(ret == 0){
		printf("Start station succeed\n");wificonnect_flag=1;
	}else{
		printf("Start station failed 1\n");
		while(!wificonnect_flag)
		{
			qcom_thread_msleep(1000);
			printf("wifi_connect_flag =0-------\n");
			ret=2;
			ret = qca_start_station_mode(WITH_SCAN, WIFI_SSID, WIFI_PASSWD, WIFI_TIMEOUT);
			if(ret==0){
				wificonnect_flag=1;
				printf("wifi_connect_flag =1-----\n");
			}
		}
		
	}
	if(wificonnect_flag==1)
	{
			qca_task_start(user_check_connect_network, 0, 1024);		
			printf("CONNECTED_STATE--------------  02\r\n");
		 	qca_task_start(user_https_task_start,0,1024 * 30);
		 	qca_task_start(userNetDataDealThread,0,1024 * 20);
			//hardy add
			qca_task_start(hm_client_event_task,0,1024 * 30);
	}
}
void ap_connect(void)// key wifi connect
{
	int ret = 0;
	printf("-----------ap connect smartlink config----02 \n");
	//wifi_connect_flag=0;
	while(!wifi_connect_flag)
	{
		qcom_thread_msleep(1000);
	}
	if(wifi_connect_flag==1)//while(wifi_connect_flag==1)//
	{
		qcom_thread_msleep(3000);//qcom_thread_msleep(2000);
		//qca_task_start(user_check_connect_network, 0, 1024);		
		printf("CONNECTED_STATE--------------  02\r\n");
			//	qca_task_start(user_https_task_start,0,1024 * 30);
		if(MQTT_is_connected()){
			printf("CONNECTED_STATE-------------- MQTT_is_connected 02\r\n");
			qca_task_start(userNetDataDealThread,0,1024 * 20);//1
			//hardy add
			qca_task_start(hm_client_event_task,0,1024 * 30);//1
			//break;
		}
	}

}
void user_start_dealthread(void)
{
	printf("CONNECTED_STATE-------------- MQTT_is_connected 03\r\n");
	//qca_task_start(userNetDataDealThread,0,1024 * 20);//1
	//qca_task_start(hm_client_event_task,0,1024 * 30);//1
}

void qca_tcp_client_handle(void)
{
	int				recvBytes;
	int				sendBytes;
	int				fdAct;
	static q_fd_set sockSet;
	struct timeval	tmo;
    int             fromSize = 0;
        struct sockaddr_in fromAddr;
        
            volatile union {
                unsigned int ipAddr;
                struct {
                    unsigned char IP3 :8;
                    unsigned char IP2 :8;
                    unsigned char IP1 :8;
                    unsigned char IP0 :8;
                } byte;
            } ip_byte;



	#if 1
		//while(1){
			
		//	}
	#endif
	while(1)
	{

		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(tcp_fd, &sockSet);
        FD_SET(udp_fd, &sockSet);

		//== select time
		tmo.tv_sec = 1;//
		tmo.tv_usec = 0;
        int maxfd = (tcp_fd>udp_fd)?tcp_fd:udp_fd;
		//== select
		fdAct = qcom_select(maxfd + 1, &sockSet, NULL, NULL, &tmo);
		//printf("qcom_select connect tcp server------\n");
		if (fdAct == 0)
		{
            //printf("selelt timeout\n");
			return;
		}
		else if (fdAct == -1)
		{
			printf("selelt failed\n");
			return;
		}
		/*
		//== check event   old //
		printf("FD_ISSET FD_ISSET---------\n");
		fdAct = FD_ISSET(tcp_fd, &sockSet);//add
		if (fdAct == 0)
		{
			printf("FD_ISSET countinue\n");
			//continue;
		}
		else if (fdAct == -1)
		{	tcp_fd = -1;
			printf("FD_ISSET failed\n");
			break;
		}//add
        if(tcp_fd>0)
        {
              //fdAct = FD_ISSET(tcp_fd, &sockSet);//add
              // if(fdAct)//if( FD_ISSET(tcp_fd, &sockSet))
        	{
              	memset(xlink_tcp_rcv_buffer, 0, 1024);
                	//== receive
           		recvBytes = qcom_recv(tcp_fd, (char*)xlink_tcp_rcv_buffer, 1024, 0);
                 if (recvBytes <= 0) 
                 {
                 	printf("[qcom_recv]tcp socket close.\n");
                 	qcom_socket_close(tcp_fd);
                 	printf("[qcom_recv]tcp socket close.%d\n",tcp_fd);
                 	tcp_fd = -1;
                 	break;//return;
                 }
				uint16_t i;
             	printf("hex data:\r\n");
				for(i = 0; i < recvBytes; i ++)
				{
					printf("%02x ",xlink_tcp_rcv_buffer[i]);
				}
				printf("the recv data end\r\n");
                //XlinkPushData(xlink_tcp_rcv_buffer, recvBytes);
                //hmlink_tcp_PushData(xlink_tcp_rcv_buffer, recvBytes);
                //add
                printf("receive %d byte: %s\n", recvBytes, xlink_tcp_rcv_buffer);
				//sendBytes = qcom_send(tcp_fd, (char *)xlink_tcp_rcv_buffer, recvBytes, 0);
				printf("send %d byte\n", sendBytes);
				hmlink_tcp_PushData(xlink_tcp_rcv_buffer, recvBytes);
				//add
            }
					
		}*/

		// new select
		if(tcp_fd>0)
        {
             //fdAct = FD_ISSET(tcp_fd, &sockSet);//add
             if(FD_ISSET(tcp_fd, &sockSet))//if(fdAct)//
        	{
              	memset(xlink_tcp_rcv_buffer, 0, 1024);
                //== receive
           		recvBytes = qcom_recv(tcp_fd, (char*)xlink_tcp_rcv_buffer, 1024, 0);
                 if (recvBytes <= 0) 
                 {
                 	printf("[qcom_recv]tcp socket close.\n");
                 	qcom_socket_close(tcp_fd);
                 	printf("[qcom_recv]tcp socket close.%d\n",tcp_fd);
                 	tcp_fd = -1;
                 	break;//return;
                 }
				uint16_t i;
             	printf("hex data:\r\n");
				for(i = 0; i < recvBytes; i ++)
				{
					printf("%02x ",xlink_tcp_rcv_buffer[i]);
				}
				printf("the recv data end\r\n");
                //XlinkPushData(xlink_tcp_rcv_buffer, recvBytes);
                //hmlink_tcp_PushData(xlink_tcp_rcv_buffer, recvBytes);
                //add
                printf("receive %d byte: %s\n", recvBytes, xlink_tcp_rcv_buffer);
				//sendBytes = qcom_send(tcp_fd, (char *)xlink_tcp_rcv_buffer, recvBytes, 0);
				printf("send %d byte\n", sendBytes);
				hmlink_tcp_PushData(xlink_tcp_rcv_buffer, recvBytes);
				//add
            }
					
		}
		
		//== check event
        if(udp_fd>0)
        {
           	if( FD_ISSET(udp_fd, &sockSet))
        	{
				//== clear buffer
        		memset(xlink_udp_buffer, 0, UDP_SERVER_BUF_SIZE);
        		//== receive
        		recvBytes = qcom_recvfrom(udp_fd, (char*)xlink_udp_buffer, UDP_SERVER_BUF_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
        		if (recvBytes <= 0)
        		{
        			printf("udp socket close.\n");
                	qcom_socket_close(udp_fd);
               		printf("[qcom_recv]tcp socket close.%d\n",udp_fd);
                	udp_fd = -1;
        			break;//return;
        		}
                printf("udp (fd = %d %d)port %d  addr : %x\n", udp_fd,recvBytes,fromAddr.sin_port, fromAddr.sin_addr.s_addr);

        		xlink_addr addr_t;
                addr_t.sin_port = fromAddr.sin_port;
                addr_t.socket = udp_fd;
                addr_t.sin_addr.s_addr = fromAddr.sin_addr.s_addr;
           		//printf("recv udp data  length %d IP=%d.%d.%d.%d port %d\r\n", recvBytes, ip_byte.byte.IP0, ip_byte.byte.IP1, ip_byte.byte.IP2, ip_byte.byte.IP3, addr_t.port);
            	//xlink_demo_process_udp_data(xlink_udp_buffer, recvBytes, &addr_t);// hardy udp handle
				//recvBytes = qcom_recvfrom(udp_fd, (char*)xlink_udp_buffer, UDP_SERVER_BUF_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
				//xlink_demo_process_udp_data(xlink_udp_buffer, recvBytes, &addr_t);
				//hmlink_process_udp_data(xlink_udp_buffer, recvBytes, &addr_t);
        		//xlink_push_local_data(&g_sdk_cfg_t, sdk_inner_buffer, recvBytes, &addr_t);
        	}
       }

				
	}
}

void  pipe1_call(unsigned char * data, unsigned int datalen, x_uint32 device_id, x_uint8 *opt) {
	printf("recv tcp pipe package len %d  deviceid %d \r\n", datalen,device_id);

	uint8_t *tempData;
	uint16_t tempLen;
	do{
	     	if((strstr(data,"ENCRYPT") == NULL) && ((strstr(data,"version") == NULL)))//versionΪechoָ��
	     	{
			printf("the data is encode\r\n");
			tempData = mem_alloc(datalen);
			if(tempData == NULL)
			{
				printf("in udp pipe:malloc mem was error\r\n");
				break;
			}
			if((data[0] == '^') && (data[datalen - 1] == '&'))
			{
				uint8_t i;
				printf("the aes key:");
				for(i = 0; i < 16; i ++)
				{
					printf("%02X ",hmIotsDevInfo.aesKey[i]);
				}
				printf("\r\n");
				data[datalen - 1] = '\0';
				tempLen = user_aes_decode(&data[1],datalen - 2,tempData,hmIotsDevInfo.aesKey);
				if(tempLen == 0)
				{
					printf("decode was error\r\n");
					userCreateJsplToBuf(NULL,APP_CMD_RC_ERROR_NOT_DECODE,MESSAGE_RSP_GETINFO,NULL,NULL,1,SEND_TO_APP_DATA_NOT_NEED_ENCODE,DATA_SEND_RCV_TYPE_TCP,device_id);
				}else{
					cp_to_rcv_buf(tempData,tempLen,DATA_SEND_RCV_TYPE_TCP,device_id);
				}
			}
			if(tempData != NULL)
			{
				mem_free(tempData);
				tempData = NULL;
			}
	     	}else{
			printf("the data is visible:%s,len:%d\r\n",data,datalen);
			cp_to_rcv_buf(data,datalen,DATA_SEND_RCV_TYPE_TCP,device_id);
	     	}
     	}while(0);
}

//static void XLINK_FUNC pipe2_call(unsigned char * data, x_uint16 datalen, x_uint8 *opt) {
//	//printf("recv tcp pipe2 package len %d  \r\n", datalen);
//	XlinkUartSend(data, datalen);
//}

static void  udp_pipe_call(unsigned char *data, unsigned int datalen, xlink_addr *addr) {
	printf("recv udp pipe package len=%d  \r\n",datalen);
	uint8_t *tempData;
	uint16_t tempLen;
	do{
	     	if(strstr(data,"ENCRYPT") == NULL)
	     	{
			printf("the data is encode\r\n");
			tempData = mem_alloc(datalen);
			if(tempData == NULL)
			{
				printf("in udp pipe:malloc mem was error\r\n");
				break;
			}
			if((data[0] == '^') && (data[datalen - 1] == '&'))
			{
				uint8_t i;
				printf("the aes key:");
				for(i = 0; i < 16; i ++)
				{
					printf("%02X ",hmIotsDevInfo.aesKey[i]);
				}
				printf("\r\n");
				data[datalen - 1] = '\0';
				tempLen = user_aes_decode(&data[1],datalen - 2,tempData,hmIotsDevInfo.aesKey);
				if(tempLen == 0)
				{
					printf("decode was error\r\n");
					userCreateJsplToBuf(NULL,APP_CMD_RC_ERROR_NOT_DECODE,MESSAGE_RSP_GETINFO,NULL,NULL,1,SEND_TO_APP_DATA_NOT_NEED_ENCODE,DATA_SEND_RCV_TYPE_UDP,0);
				}else{
					cp_to_rcv_buf(tempData,tempLen,DATA_SEND_RCV_TYPE_UDP,0);
				}
			}
			if(tempData != NULL)
			{
				mem_free(tempData);
				tempData = NULL;
			}
	     	}else{
			printf("the data is visible:%s\r\n",data);
			cp_to_rcv_buf(data,datalen,DATA_SEND_RCV_TYPE_UDP,0);
	     	}
     	}while(0);
     
}
static  void app_status(XLINK_APP_STATUS status) {

	static uint8_t tcpcloseNum = 0;
	switch (status) {
		case XLINK_WIFI_STA_APP_CONNECT: //閻€劍鍩涙潻鐐村复UDP閺堫剙婀撮柅姘�?/
			//printf("****new app connect****\r\n");
			break;
		case XLINK_WIFI_STA_APP_DISCONNECT://閻€劍鍩涚粋璇茬磻UDP閺堫剙婀撮柅姘�?/
			//printf("**** app disconnect****\r\n");
			break;
		case XLINK_WIFI_STA_APP_TIMEOUT://闁俺顔嗙搾鍛UDP閺堫剙婀撮柅姘�?/
			//printf("**** app timeout****\r\n");
			break;
		case XLINK_WIFI_STA_CONNECT_SERVER: //MT7681缁崵绮烘稉顓熸￥閻拷
			break;
		case XLINK_WIFI_STA_DISCONNCT_SERVER: //闁俺绻僑DK娑撳孩婀囬崝鈥虫珤韫囧啳鐑﹂崚銈嗘焽閺勵垰鎯佹稉搴㈡箛閸斺�虫珤閺傤厼绱�?
			printf("****########### tcp disconnect to server###########\r\n");
			tGlobal_net.remote_state = 0;
			g_AllSta.bit.isConnectedServer = 0;
			g_AllSta.bit.islogined = 0;
			XlinkInitData();
			
			if(tcp_fd > 0)
			{
				tcpcloseNum ++;
				if(tcpcloseNum % 2)
				{
					printf("close tcp fd\r\n");
					qcom_socket_close(tcp_fd);
					tcp_fd = -1;
				}else{
					printf("tcpcloseNum :%d\r\n",tcpcloseNum);
				}
			}
			
			break;
		case XLINK_WIFI_STA_LOGIN_SUCCESS: //閻ц缍嶉張宥呭閸ｃ劍鍨氶崝锟�
			printf("**** tcp login server success****\r\n");
			tcpcloseNum = 0;
			//g_AllSta.bit.isNeedSendSta = 1;
			tGlobal_net.remote_state = 1;
			g_AllSta.bit.islogined = 1;
			g_AllSta.bit.isConnectedServer = 1;
			XlinkGetAuthCode(gwStateForRealTime.xlinkAuCode);
			gwStateForRealTime.xlinkAuCode[16] = '\0';
			gwStateForRealTime.xlinkDevId = XlinkSystemGetDeviceID();
			printf("the xlink au code:%s,xlink id:%d\r\n",gwStateForRealTime.xlinkAuCode,gwStateForRealTime.xlinkDevId);
			break;
		default:
			break;
	}
}
void  xlink_demo_setServerStatus(unsigned char stat, unsigned char is80Port) {
	user_config_call.setServerStatus(stat, is80Port);
}
void heiman_setServerStatus(unsigned char stat){
}

void  xlink_demo_process_tcp_data(unsigned char *data, unsigned int datalength) {
	printf("li test for tcp\r\n");
	user_config_call.process_tcp_data(data, datalength, 0);
}

void  xlink_demo_process_udp_data(unsigned char *data, unsigned int datalength, xlink_addr *addr) {
	char cpdata[1024];
	memcpy(cpdata,data,datalength);
	cpdata[datalength] = '\0';
	printf("li test for udp :%d,data:%s\r\n",datalength,cpdata);
	//user_config_call.process_udp_data(data, datalength, addr);
}


 int send_udp_to_remote(xlink_addr *addr, unsigned char *data, unsigned int datalen) {
#if XLINK_DEBUG_ON
	printf("----xlink fun %s line %d udp send data len %d\r\n",\
					__FUNCTION__, __LINE__, datalen);
#endif
	//xlink_uip_udp_send(addr, data, datalen);

    
    struct sockaddr_in fromAddr; 
    fromAddr.sin_addr.s_addr = addr->sin_addr.s_addr;
    fromAddr.sin_family = AF_INET;
    fromAddr.sin_port = addr->sin_port;
    
    printf("udp (fd = %d %d)port %d  addr : %x\n", addr->socket,datalen,fromAddr.sin_port, fromAddr.sin_addr.s_addr);
  int  retlen =   qcom_sendto(udp_fd, (char *)data, datalen, 0, (struct sockaddr*)&fromAddr, sizeof (struct sockaddr_in));
     printf("retlen = %d\n",retlen);
	return datalen;
}


 int send_tcp_to_remote(unsigned char *data, unsigned int datalen) {
#if XLINK_DEBUG_ON
	printf("----xlink fun %s line %d tcp start send datalen=%d\r\n",\
						__FUNCTION__, __LINE__, datalen);
#endif
	xlink_tcp_send_data(data,datalen);
	return datalen;
}
static void GetAllDataPiont(unsigned char *data, int *datalen)
{
}
static void SetDataPiont(unsigned char *data, int datalen)
{

}
void xlink_notify_cb(unsigned short messagetpye,unsigned char * data, unsigned int datalen)
{
   int i=0;
   printf("----notify----messagetpye=%d-----------\r\n",messagetpye);
   printf("----notify----datalen=%d-----------\r\n",datalen);
   printf("notify data=");
   for(i=0;i<datalen;i++){
	   printf("%02X",data[i]);
   }
   printf("\r\n");
}


static int hm_getServeTimeAndSetSysTime(int *time_p)
{
	T_SYSTEM_TIME cale_time;
	HMTM mktm;
	uint32_t temp_time;
	int32_t zoneTime;
	uint32_t temptime=*time_p;
	
	printf("the hm_getServeTimeAndSetSysTime %d\r\n",temptime);
	zoneTime = ((hmIotsDevInfo.time_zones / 100) * 3600 + (hmIotsDevInfo.time_zones % 100) * 60);
	temptime		+= zoneTime; 
	temp_time=temptime;
	
	mktm = userChangeSTimeToYears(temp_time);
	cale_time.year		=  mktm.tm_year;	// year
	cale_time.month	=  mktm.tm_mon;	// month
	cale_time.day		=  mktm.tm_mday;	// day
	cale_time.hour		=  mktm.tm_hour;	// hour
	cale_time.minute	=  mktm.tm_min;	// minute
	cale_time.second	=  mktm.tm_sec;	// second
	printf("the time %d-%d-%d %d:%d:%d \r\n",mktm.tm_year,mktm.tm_mon,mktm.tm_mday,mktm.tm_hour,mktm.tm_min,mktm.tm_sec);
	qca_calendar_init_time(&cale_time);

	printf("--------------set the sys success :%d\r\n",temp_time);
}
int hm_get_deviceid(int server_deviceid)
{
	network_config_call.server_deviceid=server_deviceid;
	printf("\nthe hm get deviceid :%d,%x\r\n",network_config_call.server_deviceid,network_config_call.server_deviceid);
}


XLINK_FUNC void xlink_demo_work_init(void) {
	uint8_t *pMac = mem_alloc(8);
	user_config_call.tcp_pipe2 = NULL;
	user_config_call.tcp_pipe = pipe1_call;
	user_config_call.udp_pipe = udp_pipe_call;  //udp handle
	user_config_call.status = app_status;
	user_config_call.DebugPrintf = printf;
	user_config_call.wifisoftVersion = XLINK_SOFT_VERSION;
	//user_config_call.upgrade = yzyUpdataDeviceSoftware;
	user_config_call.wifi_type = 1;
	user_config_call.in_internet = 1;
	//user_config_call.server_time = getServeiTimeAndSetSysTime;

	user_config_call.mcuHardwareSoftVersion = user_config_call.wifisoftVersion;
	user_config_call.mcuHardwareVersion = user_config_call.wifi_type;

	user_config_call.send_tcp = send_tcp_to_remote;
	user_config_call.send_udp = send_udp_to_remote;

	user_config_call.Xlink_GetAllDataPoint = GetAllDataPiont;
	user_config_call.Xlink_SetDataPoint = SetDataPiont;

	qcom_mac_get(DEVICE_ID, (unsigned char *)pMac);	
	user_config_call.maclen = 6;
	memcpy(user_config_call.mac,pMac,user_config_call.maclen);
	mem_free(pMac);

	printf("------------user_config_call.mac: %x\n",network_config_call.mac);
   

	user_config_call.tcp_notify = xlink_notify_cb;
	user_config_call.tcp_datapoint_send_cb = NULL;
	user_config_call.tcp_pipe2_send_cb = NULL;
	user_config_call.tcp_pipe_send_cb = NULL;
	user_config_call.udp_pipe_send_cb = NULL;


	user_config_call.tcpRecvBuffer = sdk_inner_buffer;
	user_config_call.tcpRecvBuuferLength = TCP_CLIENT_BUF_SIZE; //must 2^n
         user_config_call.readConfig = xlink_read_flash_cb;
        user_config_call.writeConfig = xlink_write_flash_cb;


	if (XlinkSystemInit(XLINK_PRODUCT_ID, XLINK_PRODUCT_KEY, &user_config_call) == 0) {
	} else {
		XlinkGetServerTime();
	}
	XlinkPorcess_UDP_Enable();
	alarm_report_data = (char*)mem_alloc(512);
	if(alarm_report_data == NULL)
	{
		printf("for alloc alarm_report_data:mem alloc was error,reset sys now\r\n");
		userCheckSaveFlashAndReset(0);
	}
}
void HMUpdataDeviceSoftware(LINK_UPGRADE *data)
{
	uint8_t i;
	uint8_t temp_url[130]={0},temp_urllength=0;
	printf("the addr:%s,arryr addr:\r\n",gwStateForRealTime.wifiSoftUpdate.softUrl);
	if(data != NULL)
	{
		printf("test for ota 00000 urlLength:%d,%s\r\n",data->urlLength,data->urlstr);
		sprintf(temp_url,"%s",data->urlstr);
		temp_urllength=data->urlLength;
		if(temp_urllength < MAX_WOFTWARE_ADDR_LEN)
		{
			memset(gwStateForRealTime.wifiSoftUpdate.softUrl,'\0',MAX_WOFTWARE_ADDR_LEN);
			//memcpy(&gwStateForRealTime.wifiSoftUpdate.softUrl[0],&data->urlstr[0],data->urlLength);
			gwStateForRealTime.wifiSoftUpdate.haveNewestSoftWare = 1;
			printf("the new wifi soft addr:%s\r\n",gwStateForRealTime.wifiSoftUpdate.softUrl);
			memcpy(&gwStateForRealTime.wifiSoftUpdate.softUrl[0],&temp_url[0],temp_urllength);
			printf("the --- wifi soft addr:%s\r\n",gwStateForRealTime.wifiSoftUpdate.softUrl);

			gwStateForRealTime.wifiSoftUpdate.appAllowSoftWareUpdate = 1;//---
		}
		
	}	
	printf("test for ota 00000\r\n");
}

void hm_demo_work_init(void) {
	uint8_t *pMac = mem_alloc(20);
	uint8_t temp_mac[30]={0};

	qcom_mac_get(DEVICE_ID, (unsigned char *)pMac);	
	memcpy(hmIotsDevInfo.wifiMac,pMac,6);
	memset(hm_config_call.mac,0,sizeof(network_config_call.mac));
	sprintf(temp_mac, MACTSTR, MACTOSTR(pMac));
	strupr_a(temp_mac);
	sprintf(hm_config_call.mac,"%s",temp_mac);//temp_mac
	hm_config_call.maclen=12;
	mem_free(pMac);
	memset(hm_config_call.factory_id,0,sizeof(hm_config_call.factory_id));
	sprintf(hm_config_call.factory_id,"%s",FACTORYID);
	memset(hm_config_call.product_id,0,sizeof(hm_config_call.product_id));
	sprintf(hm_config_call.product_id,"%s",PRODUCTID);
	memset(hm_config_call.publickey,0,sizeof(hm_config_call.publickey));
	sprintf(hm_config_call.publickey,"%s",GW_PUBPLICKEY);

	hm_config_call.get_deviceid=hm_get_deviceid;
	hm_config_call.upgrade=HMUpdataDeviceSoftware;
	hm_config_call.server_time = hm_getServeTimeAndSetSysTime;
	hm_config_call.network_passprocess= pass_data_process;

	
}


unsigned int time_count;


uint32_t get_user_times(void)
{
	static uint8_t printTimeNum = 0;
	HMTM mktm;
	long  temp_time;
	uint32_t now_time;//�������õ�2106��
	T_SYSTEM_TIME cale_time;
	int32_t zoneTime;
	qca_calendar_get_time(&cale_time);
	//if(XlinkGetSystemTime(&pTime) == 0)
	{
		mktm.tm_year = cale_time.year;
		mktm.tm_mon  = cale_time.month;
		mktm.tm_mday = cale_time.day;
		mktm.tm_hour = cale_time.hour;
		mktm.tm_min  = cale_time.minute;
		mktm.tm_sec  = cale_time.second;
		
		temp_time = hm_mktime(&mktm);
		if(temp_time < 0)
		{
			printf("the mk time was error\r\n");
			return 0;
		}else{
			now_time = (uint32_t) temp_time;
		}
		zoneTime = (hmIotsDevInfo.time_zones / 100) * 3600 + (hmIotsDevInfo.time_zones % 100) * 60;
		now_time = now_time - zoneTime;

		
		printTimeNum ++;
		if(printTimeNum >= 100)
		{
			printTimeNum = 0;
			printf("the 111 time %d-%d-%d %d %d:%d:%d the timezones=%d",cale_time.year,cale_time.month,cale_time.day,cale_time.week,cale_time.hour,cale_time.minute,cale_time.second,hmIotsDevInfo.time_zones);
			printf("the time of s from 1970.1.1:%d\r\n",now_time);
		}
		
		return now_time;
	}
}


extern char TCPSocket_connectd;

void hm_loop_reportcloud(void)
{
	uint32_t temp_time;
	int32_t zoneTime;
	HMTM mktm;
	T_SYSTEM_TIME cale_time;
	uint16_t T_zones=0;
	uint32_t temptime_counts;
	printf("hm_loop_reportcloud thread start \n");
	while(1)
	{
		qcom_thread_msleep(500);
		userUpdateMsgDataToCloud();
		if(TCPSocket_connectd==1){
			//printf("hm_loop_reportcloud --------000\r\n");
			hm_adjust_times();		
		}
	}
}

void hm_adjust_times(void)  
{
	uint32_t temp_time;
	int32_t zoneTime;
	HMTM mktm;
	T_SYSTEM_TIME cale_time;
	uint16_t T_zones=0;
	uint32_t temptime_counts;

	if(network_config_call.gettime_flag)
	{	
		network_config_call.gettime_flag=0;
		temp_time =network_config_call.system_time;
		zoneTime = ((hmIotsDevInfo.time_zones / 100) * 3600 + (hmIotsDevInfo.time_zones % 100)*60);
		//printf("current T_zones:%d,time_zones:%d,zoneTime:%d;\r\n",T_zones,hmIotsDevInfo.time_zones,zoneTime);
		temp_time		+= zoneTime; 					
		mktm = userChangeSTimeToYears(temp_time);
		cale_time.year		=  mktm.tm_year;
		cale_time.month	=  mktm.tm_mon;	
		cale_time.day		=  mktm.tm_mday;
		cale_time.hour		=  mktm.tm_hour;
		cale_time.minute	=  mktm.tm_min;
		cale_time.second	=  mktm.tm_sec;
		printf("the adjust time %d-%d-%d %d %d:%d:%d the timezones=%d\r\n",cale_time.year,cale_time.month,cale_time.day,cale_time.week,cale_time.hour,cale_time.minute,cale_time.second,hmIotsDevInfo.time_zones);
		qca_calendar_init_time(&cale_time);
		printf("get the server time succese\r\n");
	}
}
void hm_demo_loop(void)
{	
	qca_tcp_client_handle();	
}

void hm_loop_thread_process(void)
{
    printf("------>work_thread_process start to run......\r\n");
        while (1)
	{

		qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);			// 锟剿筹拷锟酵癸拷锟斤拷模�?
		hm_demo_loop();//xlink_demo_loop();
           //if tcp has connected to server
		   if(tcp_fd > 0){
			 // XlinkProcessTCPData(); //xlink sdk inner process the tcp package
		   }  
		//qcom_thread_msleep(2000);//qcom_thread_msleep(2000);
		qcom_thread_msleep(100);
		
		
    
	}
       

}

void work_thread_process(void)
{
	printf("------>work_thread_process start to run......\r\n");
	int tcp_int_ret;
	XlinkInitData();
	while (1)
	{
		if (gwStateForRealTime.wifiConnect != 1)
		{
			printf("not connect the wifi\r\n");
			qcom_thread_msleep(500);
			continue;
		}
		qcom_thread_msleep(200);
		qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);			// 锟剿筹拷锟酵癸拷锟斤拷模�?
		if(udp_fd < 0)
		{
			udp_init();
		}
		if(tcp_fd < 0)
		{
			tcp_int_ret = tcp_init();
			if  (tcp_int_ret ==  0)
			{
				printf("tcp_init fail\n");
				continue;
			}
			else if(tcp_int_ret == -1  )
			{
				printf("connect fail\n");
				continue;
			}
		}   

	}
       

}

void MQTT_network_task(void)
{
	
	MQTT_main();
}
void hmwork_thread_process(void)
{
	printf("------>hmwork_thread_process start to run......\r\n");
	int tcp_int_ret;
	while (1)
	{
		if (gwStateForRealTime.wifiConnect != 1)
		{
			printf("not connect the wifi\r\n");
			qcom_thread_msleep(500);
			continue;
		}
		qcom_thread_msleep(200);
		qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);			// 锟剿筹拷锟酵癸拷锟斤拷模�?
		if(udp_fd < 0)
		{
			udp_init();
		}
		if(tcp_fd < 0)
		{
			printf("------hmwork_thread_process tcp_init......\r\n");
			tcp_int_ret = tcp_init();
			if  (tcp_int_ret ==  0)
			{
				printf("tcp_init fail\n");
				continue;
			}
			else if(tcp_int_ret == -1  )
			{
				printf("connect fail\n");
				continue;
			}
		} 
		qcom_thread_msleep(3000);

	}
       

}

char *strupr_c(char*ptr,char*ch)
{
   long i,m;
   for(i=0;i<strlen(ch);i++)
   {
      if(*(ch+i)!=0)
      {
        m=*(ch+i);
        if(96<m&&m<123)
		*(ptr+i)=*(ch+i)-32;
      }
      else if(*(ch+i)=0)
         break;
    }
    return ptr;
}


void hm_demo_func_init(void)
{
	uint8_t *pMac = mem_alloc(20);
	uint8_t temp_mac[30]={0};

	qcom_mac_get(DEVICE_ID, (unsigned char *)pMac);	
	memcpy(hmIotsDevInfo.wifiMac,pMac,6);
	memset(network_config_call.mac,0,sizeof(network_config_call.mac));
	sprintf(temp_mac, MACTSTR, MACTOSTR(pMac));

	strupr_a(temp_mac);
	sprintf(network_config_call.mac,"%s",temp_mac);//temp_mac
	network_config_call.maclen=12;
	mem_free(pMac);
	sprintf(network_config_call.factory_id,"%s",FACTORYID);
	sprintf(network_config_call.product_id,"%s",PRODUCTID);
	network_config_call.gettime_flag=0;
	network_config_call.system_time=0;
	network_config_call.server_deviceid=0;

	printf("-------------------mac mac:%s/%s/%s",network_config_call.factory_id,network_config_call.product_id,network_config_call.mac);
}


//testtest
int tcpc_fd=-1;
void qca_tcp_client_handle1(void)
{
	int				recvBytes;
	int				sendBytes;
 	unsigned char	*recvBuff = mem_alloc(TCP_CLIENT_BUF_SIZE);
	int				fdAct;
	static q_fd_set sockSet;
	struct timeval	tmo;

	while (1)
	{
		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(tcpc_fd, &sockSet);
		if (tcpc_fd <= 0)
		{
			printf("socket closed\n");
			break;
		}

		//== select time
		tmo.tv_sec = 3;
		tmo.tv_usec = 0;

		//== select
		fdAct = qcom_select(tcpc_fd + 1, &sockSet, NULL, NULL, &tmo);
		if (fdAct == 0)
		{
			continue;
		}
		else if (fdAct == -1)
		{
			printf("selelt failed\n");
			break;
		}

		//== check event
		fdAct = FD_ISSET(tcpc_fd, &sockSet);
		if (fdAct == 0)
		{
			printf("FD_ISSET countinue\n");
			continue;
		}
		else if (fdAct == -1)
		{
			printf("FD_ISSET failed\n");
			break;
		}

		memset(recvBuff, 0, TCP_CLIENT_BUF_SIZE);
		//== receive
		recvBytes = qcom_recv(tcpc_fd, (char*)recvBuff, TCP_CLIENT_BUF_SIZE, 0);
		if (recvBytes <= 0) 
		{
			printf("socket close.\n");
			break;
		}
		
		printf("receive %d byte: %s\n", recvBytes, recvBuff);
		sendBytes = qcom_send(tcpc_fd, (char *)recvBuff, recvBytes, 0);

		printf("send %d byte\n", sendBytes);

	}
	printf("close socket\n");
	qcom_socket_close(tcpc_fd);
}

/**
* @fn void qca_tcp_client_task(unsigned int arg)
* @brief tcp client task
* @param arg task input
* @return 
*/
void qca_tcp_client_task1(unsigned int arg)
{
	struct sockaddr_in	remoteAddr;
	int					ret = 0;
	int server_ip = 0;

	while (1)
	{
		printf("qca_tcp_client_task1 run ----\r\n");

		qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);			// �˳��͹���ģʽ

		tcpc_fd = qcom_socket(PF_INET, SOCK_STREAM, 0);
		if (tcpc_fd < 0)
		{
			printf("open tcp client socket error.\r\n");
			goto QUIT;
		}
		
		server_ip = qca_inet_addr(TCP_SERVER_IP_ADDR);
		A_PRINTF("Server Addr:\n");
		printf("%d.%d.%d.%d\n", 
		(server_ip) & 0xFF, (server_ip) >> 8 & 0xFF,
		(server_ip) >> 16 & 0xFF, (server_ip) >> 24 & 0xFF);

		//== ipv4 connect 
		memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_addr.s_addr = htonl(server_ip);
		remoteAddr.sin_port = htons(TCP_SERVER_PORT);

		ret = qcom_connect(tcpc_fd, (struct sockaddr *) &remoteAddr, sizeof (struct sockaddr_in));
		if (ret < 0)
		{
			printf("Connect Failed\r\n");
			qcom_socket_close(tcpc_fd);
			goto QUIT;
		}

		printf("Connect %lu.%lu.%lu.%lu OK\r\n",
			(remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
			(remoteAddr.sin_addr.s_addr) >> 16 & 0xFF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);


		//== packet handle
		qca_tcp_client_handle1();

QUIT:
		qcom_thread_msleep(5000);
	}

	qca_task_exit();
}


void user_tcp_client_handle(void)
{
	int				recvBytes;
	int				sendBytes;
 	unsigned char	*recvBuff = mem_alloc(TCP_CLIENT_BUF_SIZE);
	int				fdAct;
	static q_fd_set sockSet;
	struct timeval	tmo;
	int             fromSize = 0;
    struct sockaddr_in fromAddr;

	while (1)
	{
		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(tcp_fd, &sockSet);
		FD_SET(udp_fd, &sockSet);
		
		if (tcp_fd <= 0)
		{
			printf("socket closed\n");
			break;
		}

		//== select time
		tmo.tv_sec = 1;
		tmo.tv_usec = 0;
		int maxfd = (tcp_fd>udp_fd)?tcp_fd:udp_fd;
		//== select
		fdAct = qcom_select(maxfd + 1, &sockSet, NULL, NULL, &tmo);
		if (fdAct == 0)
		{
			continue;
			//return;
		}
		else if (fdAct == -1)
		{
			printf("selelt failed\n");
			break;
			//return;
		}

		//== check event
		//if(tcp_fd>0)
			{
			fdAct = FD_ISSET(tcp_fd, &sockSet);
			if (fdAct == 0)
			{
				printf("FD_ISSET countinue\n");
				TCPsetServerStatus(1);
				continue;
			}
			else if (fdAct == -1)
			{
				printf("FD_ISSET failed\n");
				TCPsetServerStatus(0);
				break;
			}
		}

		if(tcp_fd>0){
		memset(xlink_tcp_rcv_buffer, 0, TCP_CLIENT_BUF_SIZE);
		//== receive
		recvBytes = qcom_recv(tcp_fd, (char*)xlink_tcp_rcv_buffer, TCP_CLIENT_BUF_SIZE, 0);
		if (recvBytes <= 0) 
		{
			printf("socket close.\n");
			break;
		}
		
		printf("receive %d byte: %s\n", recvBytes, recvBuff);
		printf("receive %d byte: %s\n", recvBytes, xlink_tcp_rcv_buffer);
		//	sendBytes = qcom_send(tcp_fd, (char *)xlink_tcp_rcv_buffer, recvBytes, 0);//调试�?deldel
		printf("send %d byte\n", sendBytes);
		hmlink_tcp_PushData(xlink_tcp_rcv_buffer, recvBytes);
		
		//sendBytes = qcom_send(tcp_fd, (char *)recvBuff, recvBytes, 0);

		//printf("send %d byte\n", sendBytes);
		}
		 if(udp_fd>0)
                {
                       // if( FD_ISSET(udp_fd, &sockSet))
                        {

        				//== clear buffer
        				memset(xlink_udp_buffer, 0, UDP_SERVER_BUF_SIZE);
        				//== receive        				
        				recvBytes = qcom_recvfrom(udp_fd, (char*)xlink_udp_buffer, UDP_SERVER_BUF_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
        				if (recvBytes <= 0)
        				{
        					printf("udp socket close.\n");
                            qcom_socket_close(udp_fd);
                            printf("[qcom_recv]tcp socket close.%d\n",udp_fd);
                            udp_fd = -1;
        					break;//return;
        				}

                  
                            printf("udp (fd = %d %d)port %d  addr : %x\n", udp_fd,recvBytes,fromAddr.sin_port, fromAddr.sin_addr.s_addr);


        					xlink_addr addr_t;

                            addr_t.sin_port = fromAddr.sin_port;
                            addr_t.socket = udp_fd;
                            addr_t.sin_addr.s_addr = fromAddr.sin_addr.s_addr;

        			
        	                  }
                }

		

	}
	printf("close socket\n");
	qcom_socket_close(tcp_fd);qcom_socket_close(udp_fd);
	//tcp_fd=-1;
}

void user_tcp_client_task(void)
{
	struct sockaddr_in	remoteAddr;
	int	ret = 0;
	int server_ip = 0;
	//int ret;
	while(1){
		printf("user_tcp_client_task run ----\r\n");
		//qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);		
		ret=tcp_init();
		if(ret<0)
		{
			goto QUIT;
		}
		user_tcp_client_handle();//qca_tcp_client_handle1();

		QUIT:
			qcom_thread_msleep(3000);
	}

}


//test test
void test_uart1(void)
{
    int ret;
    
    uart_paramer1.BaudRate = 115200;
    uart_paramer1.number = 8;
    uart_paramer1.parity = 0;
    uart_paramer1.StopBits = 1;
    uart_paramer1.FlowControl = 0;
    
    printf("uart testing......\r\n");
    ret = qca_uart_task_start(&uart_paramer1, 1024, uart_reslove_packet1);
    if(ret != 0){
        printf("uart start fail.\r\n");
        return;
    }

    while(1){
		uart_recv1();

		qcom_thread_msleep(10);
	}

    ret = qca_uart_stop();
    if(ret != 0){
        printf("uart stop fail.\r\n");
        return;
    }
    printf("uart test done.\r\n");
}

void uart_recv1(void)
{
    int ret, len;

	//== receive one packet
	memset(recv_uart_buf1, 0, UART_PACKET_MAX_LEN);
	ret = qca_uart_get_one_recv_packet((unsigned char *)recv_uart_buf1, &len);//
	
	if (ret < 0){
		return;
	}
	else{printf("123asd\n");
		}

	printf("[UART]recv: %s\n", recv_uart_buf1);

    qca_uart_send(recv_uart_buf1, len);
}

int uart_reslove_packet1(unsigned char **packet_start, unsigned char **packet_end, 
    unsigned char *buf, int buf_len, unsigned char *data_start, unsigned char *data_end)
{   
	int	i;
	int	len;

start:
	*packet_start = NULL;
	*packet_end = NULL;

	if(data_end >= data_start)
	{
		len = data_end - data_start;

		if(len <= 0)
		{
			if(qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;
		}

		*packet_start = data_start;				

		data_start++;
		for(i = 0; i < len; i++)
        {
			if(*data_start == '\n')
			{
				break;
			}

			data_start++;
		}
		if(i == len)
		{
			if(qca_uart_get_buf_full_status())
			{
				return -3;
			}

			return -1;
		}

		*packet_end = data_start;

		return 0;
	}
	else 
	{
		len = buf_len  - (data_start - data_end);

		if(len <= 0)
		{
			return -1;
		}

		*packet_start = data_start;

		if(data_end >= data_start)
		{
			goto start;
        }

		data_start = qca_uart_get_next_addr(data_start, 1);

		for(i = 0; i < len; i++)
		{
			if(*data_start == '\n')
			{
				break;
			}

			data_start = qca_uart_get_next_addr(data_start, 1);
        }

		*packet_end = data_start;

		return 0;
	}
	return -1;			
}
//test
void test_save_hmIotsDevInfo(void)
{
	uint8_t mode;
	if(strstr(hmIotsDevInfo.wifiInfo.sta_ssid,WIFI_SSID)==NULL){
		mode=1;
		printf("-------------test_save_hmIotsDevInfo\r\n");
		sniffer_success_test(WIFI_SSID,WIFI_PASSWD,mode,mode);
		
	}
}

int main(void)
{
	unsigned char b;
	//MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	// data1= MQTTPacket_connectData_initializer;
    // Add your code here
	printf("------------------------01main run 01-16-------\r\n");
	if(b=outString("it's successful 1"))
		printf("outString have data len=%d\r\n",b);
	b=sum_add(2,3);
	printf("sum_add=%d\r\n",b);
	
	// qcom_watchdog(1,15);
	hm_network_init();
	hm_demo_work_init();
	welcome();
	uart_work_task_init();
	userOtaAndHttpTaskInit();
	qca_task_start(beep_work_task,0,1024);
	qca_task_start(light_work_task,0,1024 * 2);//�쳣
	qca_task_start(timer_work_task,0,1024 * 2);
	qca_task_start(user_manage_button_func,0,1024 * 4);
    //xlink_demo_func_init();//-xlink
	//qca_task_start(work_thread_process, 0, TCP_CLIENT_STACK_SIZE * 20);//-xlink
	//qca_task_start(xlink_loop_thread_process, 0, TCP_CLIENT_STACK_SIZE * 40);//-xlink

	hm_demo_func_init();
	welcome1();//test
	qca_task_start(hmwork_thread_process, 0, TCP_CLIENT_STACK_SIZE * 20);//20
	qca_task_start(hm_loop_thread_process, 0, TCP_CLIENT_STACK_SIZE * 50);//ѭ����������  40
		
	qca_task_start(zbdev_and_scene_manage_task,0,1024 * 4);
	qca_task_start(MQTT_network_task,0,1024*40);//hardy add  20 //
	qca_task_start(hm_loop_reportcloud,0,1024*4);//
		 
	//qca_task_start(qca_tcp_client_task1, 0, TCP_CLIENT_STACK_SIZE * 20);
	//qca_task_start(user_tcp_client_task, 0, TCP_CLIENT_STACK_SIZE * 20);
	//testtest   
	//test_uart1();
        
	return 0;
}





    