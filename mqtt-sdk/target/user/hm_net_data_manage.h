#ifndef _HM_NET_DATA_MANAGE_H
#define _HM_NET_DATA_MANAGE_H

typedef enum{
DATA_SEND_RCV_TYPE_ALL = 0,
DATA_SEND_RCV_TYPE_TCP,
DATA_SEND_RCV_TYPE_UDP
}DATA_SEND_RCV_TYPE;

typedef struct{
	uint32_t appDevId; //用于区分是哪个app的指令
	uint32_t sendCid;
	uint8_t dealType;
}APPINFO;


#define NET_SEND_BUF_BASIC			1024
#define NET_SEND_PK_NUM			15
typedef struct{
	unsigned char netsendflag;
	unsigned char netsendnum;
	APPINFO netappinfo[NET_SEND_PK_NUM];//由于处理接收的指令与发送指令不在同一个地方，故在发送的时候需要判断这条指令是发给单个app还是所有app
	char *netsendbuf;
}NETSENDINFO;



#define NET_RCV_BUF_BASIC			1024
#define NET_RCV_PK_NUM				5
typedef struct{
	unsigned char netrcvflag;
	unsigned char netrcvnum;
	APPINFO netappinfo[NET_RCV_PK_NUM];
	char *netrcvbuf;
}NETRCVINFO;



typedef struct{
uint8_t zigbee_fun_to_net_event_id;
uint16_t zigbee_dev_node_id;
uint8_t para[32];
}HMIOTS_STRUCT;
#define MAX_IOTS_BUF_SIZE			5

#define MESSAGE_FROM_APP_CHANGE_LIGHT_STATUS	30011
#define MESSAGE_FROM_APP_GETLIGHTLISTINFO		30021

#define MESSAGE_RSP_STATUS_CHANGE					30012
#define MESSAGE_RSP_GETINFO						30022

#define MESSAGE_DEV_STATE_UP						40000

#define WIFI_DEV_BASIC_STATE_OID					"2.1.1.1"
#define WIFI_DEV_BASIC_ZONES_OID					"2.1.1.1.2"
#define WIFI_DEV_BASIC_AES_OID						"2.1.1.1.1"
#define WIFI_DEV_BASIC_NAME_OID					"2.1.1.254"
#define WIFI_DEV_GW_BASIC_INFO_OID				"2.1.1.255.0.1"
#define GW_BASIC_STATE_LIGHT_SET_OID				"2.1.1.255.0.1.1"
#define GW_FUNC_ACTION_ARM_MODE_OID				"2.1.1.255.0.5"
#define GW_FUNC_ACTION_SET_ZC_JOIN_OID			"2.1.1.255.0.6"
#define GW_FUNC_ACTION_NEW_DEV_UP_OID			"2.1.1.255.0.7"
#define GW_GET_NODE_LIST_OID						"2.1.1.0"
#define GW_APP_OTA_OID								"2.1.1.6."
#define GW_APP_SCENE_ACTION_OID					"2.1.1.2."
#define GW_APP_LINKAGE_ACTION_OID					"2.1.1.3."
//dev for security
#define GW_APP_COM_DOOR_OID						"2.1.1.0.1.1."
#define GW_APP_COM_PIR_OID						"2.1.1.0.1.2."
#define GW_APP_COM_SMOKE_OID						"2.1.1.0.1.3."
#define GW_APP_COM_CO_OID							"2.1.1.0.1.4."
#define GW_APP_COM_GAS_OID						"2.1.1.0.1.5."
#define GW_APP_COM_WATER_OID						"2.1.1.0.1.6."
#define GW_APP_COM_WD_OID							"2.1.1.0.1.7."
#define GW_APP_COM_EB_OID							"2.1.1.0.1.8."
#define GW_APP_COM_ACERC_OID						"2.1.1.0.1.9."

//dev for environment
#define GW_APP_COM_THP_OID						"2.1.1.0.2.1."
#define GW_APP_COM_VIBRATION_OID					"2.1.1.0.2.7."


//dev for light
#define GW_APP_COM_RGB_OID						"2.1.1.0.3.1."
#define GW_APP_COM_AISW1_OID						"2.1.1.0.3.2."
#define GW_APP_COM_AISW2_OID						"2.1.1.0.3.3."
#define GW_APP_COM_AISW3_OID						"2.1.1.0.3.4."
#define GW_APP_COM_CUTRGB4_OID					"2.1.1.0.3.8."

//dev for household electrical
#define GW_APP_COM_USB_PLUG_OID					"2.1.1.0.4.1."
#define GW_APP_COM_SMART_PLUG_OID				"2.1.1.0.4.2."

#define GW_APP_COM_DEV_CHANGE_ROOMID_OID		"2.1.1.0.253."
#define GW_APP_COM_DEV_RENAME_OID				"2.1.1.0.254."
#define GW_APP_COM_DEV_DELETE_OID				"2.1.1.0.255."

#define GW_APP_GET_DEV_STAT_LIST_OID				"2.1.1.0.252"

#define GW_APP_COM_CONTROL_ALL_ZB_LIGHT			"2.1.1.7"


#define MAX_DEV_BASIC_LIST_NUM_FOR_PK			7
#define MAX_DEV_STAT_LIST_NUM_FOR_PK				6

typedef enum {
HFZ_EVENT_INVALID_EVENT = 0,	
HFZ_EVENT_NEW_NODE_ADDED,
HFZ_EVENT_NODE_REMOVED,
HFZ_EVENT_NODE_STATE_CHANGE,
HFZ_EVENT_NODE_STATE_ALARM,
HFZ_EVENT_AES_KEY_CHANGE,
HFZ_EVENT_SOURCE_OTA,
HFZ_EVENT_NODE_UPDATE_SPECIAL,
HFZ_EVENT_WIFI_CMD_WAIT_RSP
}HMZIGBEE_EVENT_E;

typedef enum{
CREATE_DEV_STATE_FOR_GET_NODE_LIST_OID = 0,
CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID,
CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID,
CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID,
CREATE_DEV_STATE_FOR_UPDATE_STATE_OID,
CREATE_dEV_STATE_FOR_ALARM_OID
}DEVSTATECEATEFOROID;

typedef enum{
OTA_MODE_WIFI = 1,
OTA_MODE_ZC = 2,
OTA_MODE_ZED = 3
}OTATYPE;

typedef enum{
APP_CMD_RC_ERROR_UNKNOW = 0,
APP_CMD_RC_SUCCESS = 1,
APP_CMD_RC_ERROR_PARAMETER = -1,
APP_CMD_RC_ERROR_NOT_DECODE = -2,
APP_CMD_RC_ERROR_UNKNOW_OID = -3
}APP_CMD_RC_TYPE;

#define SEND_TO_APP_DATA_NEED_ENCODE 			1
#define SEND_TO_APP_DATA_NOT_NEED_ENCODE	0

#define ECHO_CONTROL_DEV_SUB_CMD_LEN			18
#define ECHO_CONTROL_DEV_CMD_LEN				16
#define ECHO_CONTROL_GW_CMD_LEN				14



void userCreateJsplToBuf(json_t *json_newPL,int8_t rc,uint16_t cid,char *sid,char *teid,uint32_t sn,uint8_t security,uint8_t rcvType,uint32_t appDevId);
int hmSetIotsEvent(uint8_t event_id, uint16_t id, void * param,uint8_t paramLen);
int userPkSendBuffer(char *data,unsigned int len,uint8_t rcvType,uint32_t appDevId,unsigned int cid,uint8_t encodeFlag);
void zigbeegwDeviceServiceCallback(void);
void userNetMemInit(void);
#endif
