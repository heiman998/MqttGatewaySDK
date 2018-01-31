/*
 * xlink_data.h
 *
 *  Created on: 2014骞�2鏈�2鏃�
 *      Author: john
 */

#ifndef XLINK_DATA_H_
#define XLINK_DATA_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "Xlink_Head_Adaptation.h"

#define XLINK_DATAPOINT_MAX_BYTES  1000

/*鏀寔澧炲姞绔偣鐨勬暟閲*/

#define XLINK_CONFIG_INDEX_CONFIG       (0)
#define XLINK_CONFIG_INDEX_NAME         (1)
#define XLINK_CONFIG_INDEX_AUTH         (2)
#define XLINK_CONFIG_INDEX_PWD          (3)
#define XLINK_CONFIG_INDEX_TEST         (4)
#define XLINK_CONFIG_INDEX_SHARE_KEY    (6)
#define XLINK_CONFIG_INDEX_SUBKEY_KEY   (7)
#define XLINK_CONFIG_INDEX_DOMAIN_IP    (8)

/*network*/
typedef struct XLINK_NET_INFO_T {
	/*Domain name or IP */
	//char* hostNameOrIp;
	x_int32 ServerPort;
	unsigned char tcpPingCount;
	int tcp_fd;
	int udp_fd;
	/*Last connection to the server time*/
	xsdk_time_t tcp_last_connect_server;
	union {
		unsigned char all;
		struct {
			unsigned char isFirstConnectSuccess :1;/*绗竴娆￠摼鎺ュ叕缃戞垚鍔*/
			unsigned char isLogined :1;/*璁惧宸茬粡鐧婚檰鏍囪瘑*/
			unsigned char is80Port :1;/*杩炴帴鐨勬槸鍚︽槸80绔彛*/
			unsigned char is80ConnectSuccess :1;/*鎴愬姛杩炴帴涓�0绔彛*/
			unsigned char isConnectWifi :1;/*杩炴帴涓婁簡wifi*/
			unsigned char isScanEnable :1;/*鍙壂鎻忓姛鑳芥槸鍚﹀紑鍚*/
			unsigned char Res :2;
		} Bit;
	} tcpFlag;
} XLINK_NET_INFO;

/*client*/
typedef struct XLINK_CLIENT_T {
	xlink_addr Address;
	xsdk_time_t LastReceiveDataTime;
	x_uint8 checkValue;/*鍔ㄦ�鏍￠獙鍊*/
	x_uint8 ArrayIndex;/*鏁扮粍涓嬫爣绱㈠紩锛�  ssid 鍓嶄竴涓瓧鑺傛槸绱㈠紩锛屽悗涓�釜瀛楄妭鏄姩鎬佹牎楠屽�*/
	short keepAliveTime;
	unsigned char isActive;
} XLINK_CLIENT;

/*device*/
typedef struct XLINK_DEVICE_INFO_T {
	char *type;
	char product_id[XLINK_SIZE_33];
	char product_key[XLINK_SIZE_33];
	x_long32 lastGetServerTimeTime;
	x_long32 getServerTimeDelay;
	union {
		unsigned char all;
		struct {
			unsigned char isGetServerTimeTask :1; //寮�惎鑾峰緱鏈嶅姟鍣ㄦ椂闂寸殑浠诲姟
			unsigned char res :7;
		} Bit;
	} flags;
} XLINK_DEVICE_INFO;

typedef struct XLINK_INFO_T {
	XLINK_NET_INFO net_info; /*Network Information*/
	XLINK_DEVICE_INFO dev_info;/*Device Information*/
	xlink_SdkConfig config;

	union {
		unsigned char byte;
		struct {
			unsigned char clientCount :1;
			unsigned char bit1 :1;
			unsigned char bit2 :1;
			unsigned char res :5;
		} bit;
	} flag;
	xsdk_time_t g_XlinkSdkTime;	
	
	int domain_connect_times;
} G_XLINK_INFO;

/*杞欢鍗忚鐗堟湰*/
//share version 3
#define XLINK_AGREEMENT_VERSION2 2
#define XLINK_AGREEMENT_VERSION3 3

#define UTIL_INT32_GET_BIT3(X)  (unsigned char)(((X) & 0xFF000000) >> 24)
#define UTIL_INT32_GET_BIT2(X)  (unsigned char)(((X) & 0x00FF0000) >> 16)
#define UTIL_INT32_GET_BIT1(X)  (unsigned char)(((X) & 0x0000FF00) >> 8)
#define UTIL_INT32_GET_BIT0(X)   (unsigned char)((X) & 0x000000FF)

#define UTIL_INT16_GET_BITH(X)  (unsigned char)(((X)&0xff00)>>8)
#define UTIL_INT16_GET_BITL(X)   (unsigned char)((X)&0x00ff)

#define UTIL_INT32_SET(H,N,K,L) (((H)& 0x000000FF) << 24)+(((N) & 0x000000FF) << 16)+(((K) & 0x000000FF) << 8)+((L) & 0x000000FF)
#define UTIL_INT16_SET(H,L) (((H)<<8)+(L))

#define LE_BE_32(x)  ((((unsigned int) (x) & 0xff000000) >> 24) | ((((unsigned int) (x) & 0x00ff0000) >> 8)) \
			| (((unsigned int) (x) & 0x0000ff00) << 8) | (((unsigned int) (x) & 0x000000ff) << 24))



/**
 * 绯荤粺鍏ㄥ眬淇℃伅
 */
extern G_XLINK_INFO g_xlink_info;
extern XLINK_USER_CONFIG *g_xlink_user_config;

extern XLINK_FUNC void XlinkSdkDataInit(const char* product_id,const char *product_key);


extern XLINK_FUNC void XlinkSdkAppSetDeviceName(char* name, unsigned short nameLength);

#ifdef  __cplusplus
}
#endif
#endif /* XLINK_DATA_H_ */
