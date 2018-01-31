
#ifndef HM_HEAD_ADAPTATION_H_
#define HM_HEAD_ADAPTATION_H_

#ifdef  __cplusplus
extern "C" {	
#endif

#include "qcom_system.h"

#include "qcom_common.h"
#include "../lt_sample/ltm3210.h"

#include "MQTTinterface.h"

 
//#define AF_INET  0

//extern void Delay_ARMJISHU( uint32_t nCount);
#define	  LINK_MAC_LEN_MAX 	32
#define	  dprintf 	printf
//#define	  Log_info 	printf

#define	 DEBUG
#ifdef   DEBUG
#define	 Log_info 	printf
#else
#define  Log_info(format,...)
#endif

#define   link_strlen(x)           strlen((char*)(x)) 
#define   link_strcmp(x,t)         strcmp((char*)(x),(char*)(t)) 
#define   link_strncmp(x,t,z)      strncmp((char*)(x),(char*)(t),z) 
#define   link_memset(x,d,t)       memset(x,d,t)  


#define   link_memcpy(x,d,l)   memcpy(x,d,l)
#define   link_msleep(n)       qcom_thread_msleep(n)
#define   link_sprintf        sprintf
#define   link_snprintf        snprintf
#define   link_socket          qcom_socket
#define   link_bind             qcom_bind
#define   link_connect       qcom_connect
#define   link_setsockopt    qcom_setsockopt
#define   link_recv              qcom_recv
#define   link_recvfrom      qcom_recvfrom
#define   link_sendto          qcom_sendto
#define   link_send             qcom_send
#define   link_close(x)        qcom_socket_close(x)
#define   link_set_fd            q_fd_set
#define   link_select(a,b,c,d,e)      qcom_select(a,b,c,d,e)

typedef struct LINK_UPGRADE_FUN {
	unsigned int fileSize;
	//char *checkStr;
	unsigned int checkStrLength;
	char urlstr[128];
	unsigned short urlLength;
	unsigned char mCheckFlag;
	unsigned char mHardVersion;
	unsigned short mSoftVersion;
    unsigned short mCurrentVersion;
	unsigned char isWifi;
} LINK_UPGRADE;
//hm_Head_adaptation
typedef struct HM_CALLBACK_FUN {
	unsigned short maclen;
	unsigned char mac[LINK_MAC_LEN_MAX];
	unsigned char factory_id[5];
	unsigned char product_id[6];
	unsigned char publickey[18];
	unsigned char privatekey[18];
	
	int (*get_deviceid)(int server_deviceid);
	int (*upgrade)(LINK_UPGRADE *data);//void
	int (*server_time)(int *time_p);
	int (*network_passprocess)(int cmddata,char* data,int data_len);
	//void (*Xlink_SetDataPoint)(unsigned char *data, int datalen);
	//void (*Xlink_GetAllDataPoint)(unsigned char *data, int *datalen);
} HM_USER_CONFIG;

extern void welcome1(void);
extern int outString1(const char *str);
extern int HM_Sent_point_data(char* data, int length);
extern int HM_Sent_mfunc_data(char* data, int length);
extern int HM_Sent_pass_data(char* data, int length);
extern int mqtt_initconnect_func(void);
extern int mqtt_topicsubscibe_func(void);
extern int mqtt_loophandle_func(void);

extern int HM_gw_report_online(void);
extern int HM_GW_get_ServerDeviceId(void);
extern int hm_sent_pass_mesgdata(void);
extern int hm_sent_pass_gatewayparm(void);
extern int HM_sent_pass_askeymsg(int cmddata);
extern int HM_sent_point_aeskeymsg(void);
extern int HM_get_systemtime(void);
extern int HM_sent_zigbeelist_clearmsg(unsigned char type_mode,unsigned int server_deviceid);
extern int HM_pass_ack_data(int cmddata,char* data,int data_len);

extern void HM_pass_data_handle(char* data, int length);
extern void HM_Point_data_handle(char* data, int length);



#ifdef  __cplusplus
}
#endif
#endif 
