#ifndef _HMAPP_H_
#define _HMAPP_H_

#include "qcom_common.h"
#include "hmType.h"
#include "qca_calendar.h"
#include "qca_string_ext.h"


//#define HEIMAN_APP
//#define YUNZHIYI_APP
#define HM_TEST

#define HM_DEBUG

#ifndef DEVICE_ID
#define DEVICE_ID	0
#endif



#ifdef HM_TEST

#define XLINK_PRODUCT_ID   "160edcb27f5c0001160edcb27f5cf801"
#define XLINK_PRODUCT_KEY  "a21411d54368a1e817e2ee4322b822c6"
#define WIFI_DEV_DFUALT_NAME	"ZGW"
#define DEFAULT_AES_128_KEY		"8888888866666666"//"88888888666666668888888866666666"//
#define XLINK_SOFT_VERSION		12
#define HM_IOTS_SOFT_VERSION	"2.2.0"
#define HM_IOTS_HARD_VERSION 	"1.0"
#else
#ifdef HEIMAN_APP
#define XLINK_PRODUCT_ID   "160edcb2dff80001160edcb2dff8e001"
#define XLINK_PRODUCT_KEY  "bd84b3222b595d4c90f3f6c096e9f7fc"
#define WIFI_DEV_DFUALT_NAME	"ZGW"
#define DEFAULT_AES_128_KEY		"8888888866666666"
#define XLINK_SOFT_VERSION		2
#define HM_IOTS_SOFT_VERSION	"1.2.0"
#define HM_IOTS_HARD_VERSION 	"1.0"
#else 
#ifdef YUNZHIYI_APP
#define XLINK_PRODUCT_ID   "160fa6b34f0103e9160fa6b34f010c01"
#define XLINK_PRODUCT_KEY  "8f24d166f329e593d2ff3035dde876b2"
#define WIFI_DEV_DFUALT_NAME	"ZGW"
#define DEFAULT_AES_128_KEY		"8888888866666666"
#define XLINK_SOFT_VERSION		3
#define HM_IOTS_SOFT_VERSION	"1.3.0"
#define HM_IOTS_HARD_VERSION 	"1.0"
#endif
#endif
#endif
#define HM_IOTS_SMALL_GW_TYPE	770
#define HM_IOTS_PROTOCOL_VERSION	2


#define GPIO_0				0
#define GPIO_1				1
#define GPIO_2				2
#define GPIO_3				3
#define GPIO_4				4
#define GPIO_5				5
#define GPIO_6				6

#define GPIO_8				8

#define GPIO_10				10
#define GPIO_11				11
#define GPIO_12				12
#define GPIO_13				13


#define GPIO_21				21
#define GPIO_22				22
#define GPIO_23				23
#define GPIO_24				24


#define GPIO_27				27
#define GPIO_28				28
#define GPIO_29				29
#define GPIO_30				30
#define GPIO_31				31
#define GPIO_32				32
#define GPIO_33				33
#define GPIO_35				35

#define ADC_0				0
#define ADC_1				1
#define ADC_2				2
#define ADC_3				3
#define ADC_4				4
#define ADC_5				5
#define ADC_6				6


#define PWM_B				0
#define PWM_1				1
#define PWM_2				2
#define PWM_3				3
#define PWM_4				4
#define PWM_5				5
#define PWM_R				6
#define PWM_G				7

#define PWM_PORT_B         0x1
#define PWM_PORT_1         0x2
#define PWM_PORT_2         0x4
#define PWM_PORT_3         0x8
#define PWM_PORT_4         0x10
#define PWM_PORT_5         0x20
#define PWM_PORT_R         0x40
#define PWM_PORT_G         0x80



#define BEEP_BUSY				GPIO_10
#define BEEP_SDA 				GPIO_11
#define RGB_G 					GPIO_13
#define RGB_R					GPIO_12
#define RGB_B					GPIO_6
#define ADC_LIGHT_CHANNEL		ADC_0
#define ADC_5V_CHECK_CHANNEL	ADC_1
#define ZIGBEE_RESET				GPIO_8
#define BUTTON					GPIO_30

#define LANGUAGE_ENGLISH        1
#define LANGUAGE_CHINESE        2

typedef unsigned long long uint64_t;
typedef uint8_t EmberEUI64[8];


#define ROOM_ID_MAX_LEN	16
#define ZIGBEE_MODEID_LEN	16
#define ZIGBEE_MAC_LEN 		8
#define SUPORT_MAX_EP_NUM	4
#define NAME_MAX_LEN		48
#define MAX_INDEX_NUM		30
#define MAX_SCENE_NUM		8
#define MAX_LINKAGE_NUM		8
#define BASIC_NODE_INFO_SIZE	0x1000//一页，4K，为保证flash的读写寿命
#define BASIC_SCENE_INFO_SIZE	0x1000//一页，4K，为保证flash的读写寿命
#define BASIC_LINKAGE_INFO_SIZE	0x1000//一页，4K，为保证flash的读写寿命

#define BASIC_DEV_INDEX           4000
#define BASIC_SCENE_INDEX         10
#define BASIC_LINKAGE_INDEX       20
#define INVALID_DEV_INDEX       0xffff
#define INVALID_SCENE_LINKAGE_INDEX 0xff


#define FLASH_ACTION_NUM_FOR_NODE          1
#define FLASH_ACTION_NODE_INFO             2

#define FLASH_BASIC_ADDR								0xc8000
//#define USER_FLASH_BACKUP_ADDR            0x1c8000//flash对称位置，1M为分割点，前面的位置分别存应用程序和OTA程序
#define USER_ZB_DEV_IMAGE_MANAGE_ADDR 				0x1c8000
#define USER_XLINK_INFO_OFFSET							0
#define USER_ZB_DEV_IMAGE_INFO_OFFSET 				0x1000
#define USER_ZB_DEV_IMAGE_SAVE_OFFSET					0x2000

#define START_ADDR_FLASH_DEV_MYSELF_INFO_OFFSET 	0
#define START_ADDR_FLASH_NODE_NUM_OFFSET 			0x1000
#define START_ADDR_FLASH_NODE_OFFSET				0x2000//0x1000 + MAX_INDEX_NUM * BASIC_NODE_INFO_SIZE = 0x9000
#define START_ADDR_FLASH_SCENE_OFFSET				0x20000//0x20000 + MAX_SCENE_NUM *BASIC_SCENE_INFO_SIZE =0x27000
#define START_ADDR_FLASH_LINKAGE_OFFSET				0x28000//0x28000 + MAX_LINKAGE_NUM * BASIC_LINKAGE_INFO_SIZE = 0x30000


#define START_ADDR_FLASH_NODE_INFO_ADDR			FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_OFFSET
#define START_ADDR_FLASH_SCENE_INFO_ADDR			FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET
#define START_ADDR_FLASH_LINKAGE_INFO_ADDR		FLASH_BASIC_ADDR  + START_ADDR_FLASH_LINKAGE_OFFSET

#define HMIOTS_BASICINFO_FLASH		0x16000

#define START_ADDR_FLASH_XLINK_INFO_ADDR			USER_ZB_DEV_IMAGE_MANAGE_ADDR + USER_XLINK_INFO_OFFSET

#define START_ADDR_FLASH_SUB_FIRMWARE_INFO_ADDR USER_ZB_DEV_IMAGE_MANAGE_ADDR + USER_ZB_DEV_IMAGE_INFO_OFFSET

#define START_ADDR_FLASH_SUB_FIRMWARE_ADDR		USER_ZB_DEV_IMAGE_MANAGE_ADDR + USER_ZB_DEV_IMAGE_SAVE_OFFSET

#define MAX_SUB_FIRMWARE_SIZE						216 * 1024//216K
#define MAX_SUB_FIRMWARE_PAGE						54	




#define ALL_DEV_STATE_ACTION			0xffffffff
typedef union{
uint32_t modeByte;
struct{
uint32_t online						:1;
uint32_t onoffList1					:1;
uint32_t onoffList2					:1;
uint32_t onoffList3					:1;
uint32_t onoffList4					:1;
uint32_t brightnessList1				:1;
uint32_t brightnessList2				:1;
uint32_t brightnessList3				:1;
uint32_t brightnessList4				:1;
uint32_t colorTempList1				:1;
uint32_t colorTempList2				:1;
uint32_t colorTempList3				:1;
uint32_t colorTempList4				:1;
uint32_t times							:1;
uint32_t reserve						:18;
}CUTRGBState;

struct{
uint32_t onoffList1					:1;
uint32_t onoffList2					:1;
uint32_t onoffList3					:1;
uint32_t onoffList4					:1;
uint32_t brightnessList1				:1;
uint32_t brightnessList2				:1;
uint32_t brightnessList3				:1;
uint32_t brightnessList4				:1;
uint32_t colorTempList1				:1;
uint32_t colorTempList2				:1;
uint32_t colorTempList3				:1;
uint32_t colorTempList4				:1;
uint32_t ts1							:1;
uint32_t te1							:1;
uint32_t wflag1						:1;
uint32_t ts2							:1;
uint32_t te2							:1;
uint32_t wflag2						:1;
uint32_t ts3							:1;
uint32_t te3							:1;
uint32_t wflag3						:1;
uint32_t ts4							:1;
uint32_t te4							:1;
uint32_t wflag4						:1;
uint32_t reserve						:8;
}CUTRGBSet;


struct{
uint32_t online 					:1;
uint32_t batteryPercent			:1;
uint32_t batteryAlarm				:1;
uint32_t onoff						:1;
uint32_t time						:1;
uint32_t reserve					:27;
}ACEState;

struct{
uint32_t online 					:1;
uint32_t batteryPercent			:1;
uint32_t batteryAlarm				:1;
uint32_t onoff						:1;
uint32_t time						:1;
uint32_t sensitivityLevel			:1;
uint32_t reserve					:26;
}IASState;
struct{
uint32_t armMode 					:1;
uint32_t sensitivityLevel			:1;
uint32_t reserve					:30;
}IASSet;

struct{
uint32_t online 					:1;
uint32_t batteryPercent			:1;
uint32_t batteryAlarm				:1;
uint32_t time						:1;
uint32_t temp						:1;
uint32_t humi 					:1;		
uint32_t reserve					:26;
}THPState;
struct{
uint32_t humiUp 					:1;
uint32_t humiLow					:1;
uint32_t h_checkEn				:1;
uint32_t tempUp					:1;
uint32_t tempLow					:1;
uint32_t t_checkEn				:1;
uint32_t reserve					:26;
}THPSet;

struct{
uint32_t online 					:1;
uint32_t onoff						:1;
uint32_t level						:1;
uint32_t colour_r					:1;
uint32_t colour_g 					:1;		
uint32_t colour_b					:1;
uint32_t time						:1;
uint32_t reserve					:25;
}RGBState;
struct{
uint32_t onoff 					:1;
uint32_t level						:1;
uint32_t colour_r					:1;
uint32_t colour_g					:1;
uint32_t colour_b					:1;
uint32_t ts					:1;
uint32_t te						:1;
uint32_t wflag					:1;
uint32_t reserve					:22;
}RGBSet;


struct{
uint32_t online 					:1;
uint32_t relayOnoff				:1;
uint32_t usbOnoff					:1;
uint32_t time_r					:1;
uint32_t time_u 					:1;		
uint32_t reserve					:27;
}UPlugState;
struct{
uint32_t relayOnoff 				:1;
uint32_t tsr						:1;
uint32_t ter						:1;
uint32_t wflag_r					:1;
uint32_t usbOnoff					:1;
uint32_t tsu						:1;
uint32_t teu						:1;
uint32_t wflag_u					:1;
uint32_t reserve					:20;
}UPlugSet;




struct{
uint32_t online 					:1;
uint32_t relayOnoff				:1;
uint32_t power_w					:1;
uint32_t power_wh				:1;
uint32_t time 						:1;		
uint32_t reserve					:27;
}SPlugState;
struct{
uint32_t relayOnoff 				:1;
uint32_t ts						:1;
uint32_t te						:1;
uint32_t wflag					:1;
uint32_t reserve					:26;
}SPlugSet;



struct{
uint32_t online 					:1;
uint32_t onoff1					:1;
uint32_t time1 					:1;	
uint32_t onoff2					:1;
uint32_t time2 					:1;	
uint32_t onoff3					:1;
uint32_t time3 					:1;	
uint32_t reserve					:25;
}SWState;
struct{
uint32_t onoff1 					:1;
uint32_t ts1						:1;
uint32_t te1						:1;
uint32_t wflag1					:1;
uint32_t onoff2 					:1;
uint32_t ts2						:1;
uint32_t te2						:1;
uint32_t wflag2					:1;
uint32_t onoff3 					:1;
uint32_t ts3						:1;
uint32_t te3						:1;
uint32_t wflag3					:1;
uint32_t reserve					:20;
}SWSet;

struct{
uint32_t online 					:1;
uint32_t batteryPercent			:1;
uint32_t batteryAlarm				:1;
uint32_t onoff						:1;
uint32_t duration					:1;
uint32_t time 						:1;		
uint32_t reserve					:26;
}WDState;
struct{
uint32_t onoff 					:1;
uint32_t duration					:1;
uint32_t reserve					:30;
}WDSet;

struct{
uint32_t online 					:1;
uint32_t batteryPercent			:1;
uint32_t batteryAlarm				:1;
uint32_t temp						:1;
uint32_t humi						:1;
uint32_t tvoc 						:1;	
uint32_t pm2_5 					:1;
uint32_t airQuality 				:1;
uint32_t pm10 					:1;
uint32_t time 						:1;
uint32_t reserve					:22;
}AirState;
struct{
uint32_t tempUp 					:1;
uint32_t tempLow					:1;
uint32_t t_checkEn 				:1;
uint32_t humiUp					:1;
uint32_t humiLow 					:1;
uint32_t h_checkEn				:1;
uint32_t reserve					:26;
}AirSet;

struct{
uint32_t armtype 					:1;
uint32_t alarmlevel					:1;
uint32_t betimer					:1;
uint32_t gwlanguage 				:1;
uint32_t gwlightlevel				:1;
uint32_t gwlightonoff				:1;
uint32_t lgtimer					:1;
uint32_t roomID					:1;
uint32_t reserve					:24;
}GWStat;

}DEVINFOOPTION;

typedef struct{
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t min;
            uint8_t wkflag;
}TIMEIN;

//定义zigbee设备类型
typedef enum{
ZIGBEE_ZED_RECEIVE_OFF_IN_IDLE = 1,
ZIGBEE_ZED_RECEIVE_ON_IN_IDLE,
ZIGBEE_ZR_RECEIVE_ON_IN_IDLE
}ZIGBEESUBTYPE;

typedef enum{
NO_DEV_NEED_UPDATE_IMAGE = 0,
ZC_NEED_UPDATE_IMAGE,
GW_WIFI_NEED_UPDATE_IMAGE,
OTHER_ZB_DEV_NEED_UPDATE_IMAGE
}DEV_NEED_UPDATE_ENUM;

typedef enum{
NO_IMAGE_SAVE_FLASH = 0,
ZC_IMAGE_SAVE_FLASH,
SPLUG_IMAGE_ZBDEV_SAVE_FLASH,
}SAVE_THE_FLASH_IMAGE_TYPE;

typedef enum{
SCENE_LIST_INVALID_PAYLOAD = 0xff,
SCENE_LIST_ZB_DEV_PAYLOAD = 0,
SCENE_LIST_GW_PAYLOAD = 1,
SCENE_LIST_TIMER_PAYLOAD = 2,
SCENE_LIST_SCENE_PAYLOAD= 3,
SCENE_LIST_EVIROMENT_LIGHT_PAYLOAD = 4,
SCENE_LIST_LINKAGE_PAYLOAD = 5
}SCENE_LIST_TYPE;



typedef enum{
DATA_EQUAL = 0,
DATA_GREATER = 1,
DATA_LESS = 2,
DATA_GREATER_OR_EQUAL = 3,
DATA_LESS_OR_EQUAL = 4
}DATA_CONDITION;

typedef enum {
ENVIRONMENT_DARK = 0,
ENVIRONMENT_BRIGHT = 1
}LIGHT_TYPE;

typedef enum{
COMPARE_NORMAL = 0,
}COMPARE_VALUE;

typedef struct{
uint8_t s_month;
uint8_t s_day;
uint8_t s_hour;
uint8_t s_min;
uint8_t e_month;
uint8_t e_day;
uint8_t e_hour;
uint8_t e_min;
uint8_t wk_flag;
}FUNC_UP_TO_TIMER;


typedef struct{
char sta_ssid[33];
char sta_password[65];
char sap_ssid[33];
char	sta_mode;
char	sta_respond;
char sap_password[65];
char sap_channel;
char work_mode;
}NET_WIFI_INFO;

typedef struct{
TIMEIN armTimer;
TIMEIN disarmTimer;
TIMEIN homeArmTimer;
}ARMFORTIMER;


#define MAX_HOME_ARM_FOR_SENSOR_NUM		15
typedef struct{
uint16_t index[MAX_HOME_ARM_FOR_SENSOR_NUM];
uint8_t indexNum;
}HOME_ARM_FOR_DEV;

typedef struct{
uint16_t index[MAX_HOME_ARM_FOR_SENSOR_NUM];
uint8_t indexNum;
uint8_t enable;
}GWLIGHT_FOR_DEV;

typedef struct{
uint8_t head;
uint8_t app_language;
uint8_t aesKey[40];
int16_t time_zones;//时区设置
uint8_t sound_level;
uint8_t beep_alarm_times;//喇叭报警时间
uint8_t sensor_arm_type;//撤布防状态，0 撤防，1 外出布防，2在家布防
ARMFORTIMER sensor_arm_timer;//撤布防定时器
HOME_ARM_FOR_DEV home_arm_for_dev;
uint8_t light_level;//网关RGB光亮度调节
uint8_t light_on_times;	//网关RGB联动亮起时间
uint8_t light_onoff;
char romId[ROOM_ID_MAX_LEN];
NET_WIFI_INFO wifiInfo;
uint8_t wifiMac[6];
uint8_t gwName[NAME_MAX_LEN];
uint8_t end;
uint8_t ipaddr[40];


GWLIGHT_FOR_DEV gwLightForNight;//后续增加的功能，所以放在end后面，否则在设备OTA后将恢复所有信息
}DEV_MYSELF_INFO;
#pragma pack(push, 4)

typedef struct{
uint8_t alarmCode;
uint16_t alarmCluster;
}ALARMS_STA;




typedef struct{
uint8_t onOffStatus;
uint8_t level;
}DIMMER_STA;

typedef struct{
uint8_t onOffStatus;
FUNC_UP_TO_TIMER timer;
uint8_t level;
ALARMS_STA alarmSta;
}DIMMER_STA_APP;
    
typedef struct{
uint8_t onOffStatus;
uint16_t rmsVoltage;
int32_t InstantaneousDemand;
uint64_t CurrentSummationDelivered;
}SMARTPLUG_STA;

typedef struct{
uint8_t onOffStatus;
uint16_t rmsVoltage;
int32_t InstantaneousDemand;
uint64_t CurrentSummationDelivered;
FUNC_UP_TO_TIMER timer;
ALARMS_STA alarmSta;
}SMARTPLUG_STA_APP;
    
typedef struct{
uint8_t onOffStatus;
}SWITCH_STA;

typedef struct{
uint8_t onOffStatus;
FUNC_UP_TO_TIMER timer;
ALARMS_STA alarmSta;
}SWITCH_STA_APP;
    
typedef struct{
 int16_t temp;
}TEMP_STA;

typedef struct{
int16_t tempMeterage;
int16_t upTemp;
int16_t lowTemp;
uint8_t checkEnable;
uint8_t alarms;
}TEMP_STA_APP;
    
typedef struct{
 uint16_t humi;
}HUMI_STA;

typedef struct{
uint16_t humiMeterage;
uint16_t upHumi;
uint16_t lowHumi;
uint8_t   checkEnable;
uint8_t alarms;
}HUMI_STA_APP;
    
typedef struct{
uint8_t comId;
uint8_t modeId;
}ACE_STA;

typedef struct{
uint8_t comId;
uint8_t modeId;
uint8_t station;//0-撤防  1-在家布放   2-外出布放 3-SOS报警
ALARMS_STA alarmSta;
}ACE_STA_APP;
    
typedef struct{
uint8_t onOffStatus;
uint8_t level;
uint8_t hue;
uint16_t color_x;
uint16_t color_y;
uint16_t color_temp;
}COLOR_STA;


 typedef struct{
uint8_t onOffStatus;
uint8_t level;
uint16_t rgb_r;
uint16_t rgb_g;
uint16_t rgb_b;
uint16_t color_x;
uint16_t color_y;
FUNC_UP_TO_TIMER timer;
ALARMS_STA alarmSta;
}COLOR_STA_APP;
 
typedef struct{
uint8_t zoneId;
uint16_t zoneType;
uint16_t zoneStatus;
}ZONE_STA;



typedef struct{
uint8_t zoneId;
uint16_t zoneType;
uint16_t zoneStatus;
uint8_t armMode;
uint8_t beep_act_flag;
uint8_t alarms;
uint8_t sensitivityLevel;
}ZONE_STA_APP;

/*******wd***********/

#define MAX_DEV_LINK_WD_NUM		30
typedef struct{
uint16_t zIndex;
uint32_t zAlarmType;
}Z_LINK_WD;

typedef struct{
Z_LINK_WD zLinkWd[MAX_DEV_LINK_WD_NUM];
uint8_t enable;
}DEVLINKWD;

typedef struct{
uint16_t duration;
uint8_t action;//0 -关   1-开     2-未动作
DEVLINKWD  devLinkWd;
}WD_STA_APP;

typedef struct{
uint8_t onoffStatus;
uint8_t brightness;
uint16_t colorTemrature;
}CSTRGB_STA;


typedef struct{
uint8_t onOffStatus;
uint8_t brightness;
uint16_t colorTemrature;
FUNC_UP_TO_TIMER timer;
ALARMS_STA alarmSta;
}CSTRGB_STA_APP;

 
typedef union{
	TEMP_STA tempValue;
	HUMI_STA humiValue;
	SMARTPLUG_STA splugValue;
	COLOR_STA colorValue;
	DIMMER_STA dimmerValue;
	SWITCH_STA witchValue;
        SWITCH_STA onoffOutValue;
        ZONE_STA zoneValue;
        ACE_STA aceValue;
        CSTRGB_STA cstRgbValue;
}DEV_DATA;

typedef union{
TEMP_STA_APP tempStateApp;
HUMI_STA_APP humiStateApp;
SMARTPLUG_STA_APP splugStateApp;
COLOR_STA_APP colorStateApp;
DIMMER_STA_APP dimmerStateApp;
SWITCH_STA_APP witchStateApp;
SWITCH_STA_APP onoffOutStateApp;
ZONE_STA_APP zoneStateApp;
ACE_STA_APP aceStateApp;
WD_STA_APP wdStateApp;
CSTRGB_STA_APP cstRgbStateApp;
}DEV_DATA_APP;



typedef struct{
	uint8_t ep;
	uint16_t deviceId;
	uint32_t times;
	DEV_DATA_APP devData;
}EP_INFO;

typedef enum{
DEV_FOR_HTTPS_NONE_ACTION = 0,
DEV_FOR_HTTPS_ADD_ACTION = 1,
DEV_FOR_HTTPS_UPDATE_ACTION = 2
}DEVFORHTTPSACTION;

typedef struct{
	uint32_t pnext_dev_info;
	uint32_t pprev_dev_info;
	uint8_t mac[ZIGBEE_MAC_LEN];
	uint8_t onLine;
	uint8_t zbSubType;
	uint8_t batteryRemain;
	uint8_t batteryAlarm;
	uint16_t index;//网管内部唯一标识，用于指令控制
	uint16_t shortAddr;
	uint16_t devType;
	uint8_t epNum;
	uint8_t softVersion;
	DEVFORHTTPSACTION httpState;
	int8_t   roomId[ROOM_ID_MAX_LEN]; 
	EP_INFO epList[SUPORT_MAX_EP_NUM];
	uint8_t name[NAME_MAX_LEN];
	uint8_t modeId[ZIGBEE_MODEID_LEN];
       uint32_t flashAddr;
       uint8_t onOffLineState;
       uint32_t lastCommunicationTimes;
}HM_DEV_RAM_LIST;

typedef struct{
       uint8_t head;//0xaa
	uint8_t mac[ZIGBEE_MAC_LEN];
	uint8_t onLine;
	uint8_t zbSubType;
	uint8_t batteryRemain;
	uint8_t batteryAlarm;
	uint16_t index;//网关内部唯一标识，用于指令控制
	uint16_t shortAddr;
	uint16_t devType;
	uint8_t epNum;
	uint8_t softVersion;
	DEVFORHTTPSACTION httpState;
	int8_t   roomId[ROOM_ID_MAX_LEN]; 
	EP_INFO epList[SUPORT_MAX_EP_NUM];
       uint8_t name[NAME_MAX_LEN];
       uint8_t modeId[ZIGBEE_MODEID_LEN];
       uint8_t end;//0x55
}HM_DEV_FLASH_INFO;


//同zigbee SOC uart上报结构体一样，目的问了将接收到的数据强行转换为改结构体
 typedef struct{
 uint8_t macAddr[8];
 uint8_t netState;
 uint8_t batteryRemain;
 uint16_t deviceId;
 uint16_t shortAddr;
 uint8_t epId;
 uint8_t modeId[ZIGBEE_MODEID_LEN];
 DEV_DATA devData;
 }UART_UPDATE_NEW_DEV_DATA_INFO;

typedef struct{
	uint8_t head;
	uint8_t devNum;
	uint16_t devIndex[MAX_INDEX_NUM];
       uint8_t sceneNum;
       uint8_t sceneIndex[MAX_SCENE_NUM];
       uint8_t linkageNum;
       uint8_t linkageIndex[MAX_LINKAGE_NUM];
       uint8_t end;
}DEV_NUM_INFO;


typedef struct{
uint8_t lightOnoff;
uint8_t armType;
uint16_t betimer;
uint8_t language;
uint8_t lightLevel;
}OUTPUTFORGW;

typedef struct{
uint8_t onoff;
}OUTPUTFORSPLUG;

typedef struct{
uint8_t onoff1;
uint8_t onoff2;
uint8_t onoff3;
}OUTPUTFORSWITCH;

typedef struct{
uint8_t onoff_r;
uint8_t onoff_u;
}OUTPUTFORUPLUG;

typedef struct{
uint8_t ronoff1;
uint8_t ronoff2;
uint8_t ronoff3;
uint8_t ronoff4;
}OUTPUTFORRELAY;

typedef struct{
uint8_t onoff;
uint8_t level;
uint16_t rgb_r;
uint16_t rgb_g;
uint16_t rgb_b;
}OUTPUTFORRGB;

typedef struct{
uint8_t onoff1;
uint8_t brightness1;
uint16_t colorTemp1;
uint8_t onoff2;
uint8_t brightness2;
uint16_t colorTemp2;
uint8_t onoff3;
uint8_t brightness3;
uint16_t colorTemp3;
uint8_t onoff4;
uint8_t brightness4;
uint16_t colorTemp4;
}OUTPUTFORCUTRGB;

typedef struct{
uint8_t onoff;
}OUTPUTFORWD;


typedef union{
OUTPUTFORGW gwOutput;
OUTPUTFORSPLUG splugOutput;
OUTPUTFORSWITCH switchOutput;
OUTPUTFORUPLUG uplugOutput;
OUTPUTFORRELAY relayOutput;
OUTPUTFORRGB rgbOutput;
OUTPUTFORWD wdOutput;
OUTPUTFORCUTRGB cutRgbOutput;
}SCENE_LINKAGE_OUTPUT_STA;

typedef struct{
uint8_t onoff;
}IASINPUT;

typedef struct{
uint8_t onoff;
}ACEINPUT;

typedef struct{
uint8_t lightOnoff;
uint8_t armType;
}GWINPUT;

typedef struct{
uint8_t onoff1;
uint8_t onoff2;
uint8_t onoff3;
}SWINPUT;

typedef struct{
uint8_t onoff;
uint16_t power_kw;
}SPLUGINPUT;

typedef struct{
uint8_t onoff_r;
uint8_t onoff_u;
}UPLUGINPUT;

typedef struct{
uint8_t onoff;
}RGBINPUT;

typedef struct{
int16_t tempValue;
uint16_t humiValue;
}THPINPUT;

typedef struct{
uint8_t onoff1;
uint8_t onoff2;
uint8_t onoff3;
uint8_t onoff4;
}CUTRGBINPUT;

typedef struct{
ACEINPUT aceStaInput;
IASINPUT iasStaInput;
GWINPUT gwStaInput;
SWINPUT swStaInput;
SPLUGINPUT splugStaInput;
UPLUGINPUT uplugStaInput;
RGBINPUT rgbStaInput;
THPINPUT thpStaInput;
CUTRGBINPUT cutRgbInput;
}LINKAGE_INPUT_CHECK_DATA;

 

typedef struct{
	uint8_t valid;
        SCENE_LIST_TYPE type;////0-dev;1-gw;2-scene;3-time
        uint16_t targetId;//depend on type
	 DEVINFOOPTION devAction;
	 SCENE_LINKAGE_OUTPUT_STA devData;
        uint16_t delay;//s 用于联动输出，场景输出在场景结构体里
}OUTPUTINFO;

#define MAX_OUTPUT_NUM          6

typedef struct{
        uint8_t head;
        uint8_t valid;
        uint8_t sceneId;
        uint8_t sceneName[NAME_MAX_LEN];
        uint8_t outputNum;
        OUTPUTINFO outDevInfo[MAX_OUTPUT_NUM];  
        uint16_t delay;//s
        uint32_t pictureId;
        uint8_t end;
}SCENEINFO;

#define MAX_INPUT_NUM   1

typedef struct{
            uint16_t targetId;//index   
	     LINKAGE_INPUT_CHECK_DATA devData;
	     DEVINFOOPTION devAction;
          }DEVIN;


typedef union{
DEVIN devIn;
TIMEIN timeIn;
}DATAIN;
typedef struct{
	uint8_t valid;
        SCENE_LIST_TYPE type;////0-dev;1-gw;2-time ;3-scene;4环境光强度
        uint8_t triggering;//0-无效，1-有效
        uint8_t action;//当type 为0/1时该字段有效，0等于，1大于，2小于，3大于等于，4小于等于
	DATAIN IN;
}INPUTINFO;


typedef union{
FUNC_UP_TO_TIMER timeCheck;
DEVIN devCheck;
uint8_t eviromentLight;//0 -光照度弱；1-光照度强
}CHECK;



#define MAX_CHECK_NUM   3
typedef struct{
	uint8_t valid;
        SCENE_LIST_TYPE type;////0-dev;1-gw;2-time ;3-scene;4环境光强度
        uint8_t action;
	 CHECK check;
}CHCEKINFO;

typedef struct{
        uint8_t head;
        uint8_t valid;
        uint8_t Enable;
        uint8_t linkageId;
        uint8_t linkName[NAME_MAX_LEN];
        INPUTINFO inputInfo;
        uint8_t checkNum;
        CHCEKINFO checkInfo[MAX_CHECK_NUM];
        uint8_t outputNum;
        OUTPUTINFO outputInfo[MAX_OUTPUT_NUM];
        uint8_t end;
}LINKAGEINFO;

typedef struct{
	uint8_t valid;
	uint8_t zbCmd;
	uint8_t zbEp;
	uint16_t zbClusterId;
	uint16_t zbShortAddr;
	uint32_t cmdSn;
	uint32_t rcvTimems;
	uint32_t setData;
}WIFICMDWATIRSP;
#define SPECIALCMDNUM					3


typedef struct __GW_INFO__
{
	uint8_t node_cnt;
	uint32_t pnode_list;			
	uint8_t linkage_cnt;
	LINKAGEINFO linkage_list[MAX_LINKAGE_NUM];
	uint8_t scene_cnt;
	SCENEINFO scene_list[MAX_SCENE_NUM];
	uint8_t ramChangeFlag;
}HM_GW_LIST;
////uart data type
#pragma pack(pop)
//定义串口cmd命令
#define UART_CMD_PERMIT_JOIN_REQ                   0x11
#define UART_CMD_PERMIT_JOIN_RSP                   0x12
#define UART_CMD_NEW_DEV_IDCT                      0x10
#define UART_CMD_DEV_INFO_REQ                      0x13
#define UART_CMD_DEV_INFO_RSP                      0x14
#define UART_CMD_READ_ZC_INFO_REQ                  0x15
#define UART_CMD_READ_ZC_INFO_RSP                  0x16
#define UART_CMD_LEAVE_DEV_REQ                     0x17
#define UART_CMD_LEAVE_DEV_RSP                     0x18
#define UART_CMD_DEV_LEAVE_IDCT                    0x1a
#define UART_CMD_UPDATE_ZC_SOFT_REQ                0x1b
#define UART_CMD_UPDATE_ZC_SOFT_RSP                0x1c
#define UART_CMD_SET_ZC_TO_FACTORY_REQ             0x1d
#define UART_CMD_SET_ZC_TO_FACTORY_RSP             0x1e

#define UART_CMD_SET_RSSI_REPORT_REQ               0x21
#define UART_CMD_SET_RSSI_REPORT_RSP               0x22
#define UART_CMD_DEV_RSSI_IDCT                     0x24
#define UART_CMD_SEND_ZC_HEART_REQ			0x25
#define UART_CMD_SEND_ZC_HEART_RSP			0x26


#define UART_CMD_WRITE_ATTR_REQ                    0x31
#define UART_CMD_WRITE_ATTR_RSP                    0x32
#define UART_CMD_READ_ATTR_REQ                     0x33
#define UART_CMD_READ_ATTR_RSP                     0x34
#define UART_CMD_ATTR_DATA_IDCT                    0x36
#define UART_CMD_ATTR_REPORT_IDCT                  0x38
#define UART_CMD_GET_ACTIVE_EP_REQ                 0x39
#define UART_CMD_GET_ACTIVE_EP_RSP                 0x3a
#define UART_CMD_GET_EP_SMPDST_REQ                 0x3b
#define UART_CMD_GET_EP_SMPDST_RSP                 0x3c
#define UART_CMD_CFG_ATTR_REPORT_REQ               0x3d
#define UART_CMD_CFG_ATTR_REPORT_RSP               0x3e
#define UART_CMD_CFG_ATTR_REPORT_IDCT              0x40
#define UART_CMD_GET_NODE_DESCRIPTOR_REQ           0x41
#define UART_CMD_GET_NODE_DESCRIPTOR_RSP           0x42
#define UART_CMD_GET_NODE_DESCRIPTOR_IDCT          0x44
#define UART_CMD_BIND_REQ                         		 	0x45
#define UART_CMD_BIND_RSP                          			0x46
#define UART_CMD_UNBIND_REQ                        			0x47
#define UART_CMD_UNBIND_RSP                        			0x48


#define UART_CMD_DEV_UPDATE_SHORT_ADDR_IDCT        0x60
#define UART_CMD_DEV_OFFLINE_IDCT                  		0x62
#define UART_CMD_IAS_ZONE_ALARM_IDCT               0x64
#define UART_CMD_DEV_ALARM_IDCT                    0x66
#define UART_CMD_IAS_ACE_COM_IDCT                  0x68

#define UART_CMD_ONOFF_REPORT_IDCT                 0x80
#define UART_CMD_ONOFF_CONTROL_REQ                 0x81
#define UART_CMD_ONOFF_CONTROL_RSP                 0x82
#define UART_CMD_LEVEL_CONTROL_REQ                 0x83
#define UART_CMD_LEVEL_CONTROL_RSP                 0x84
#define UART_CMD_WD_CONTROL_REQ                    0x85
#define UART_CMD_WD_CONTROL_RSP                    0x86
#define UART_CMD_IDENTIFY_REQ                      0x87
#define UART_CMD_IDENTIFY_RSP                      0x88
#define UART_CMD_COLOR_CONTROL_REQ                 0x89
#define UART_CMD_COLOR_CONTROL_RSP                 0x8a

#define UART_CMD_GROUP_CONTROL_REQ                 0x8b
#define UART_CMD_GROUP_CONTROL_RSP                 0x8c

#define UART_CMD_RGBLIGHT_CONTROL_REQ              0x8d
#define UART_CMD_RGBLIGHT_CONTROL_RSP              0x8e 




#define UART_CMD_ONOFF_CONTROL_BY_GROUP_REQ        0xc1
#define UART_CMD_ONOFF_CONTROL_BY_GROUP_RSP        0xc2
#define UART_CMD_LEVEL_CONTROL_BY_GROUP_REQ        0xc3
#define UART_CMD_LEVEL_CONTROL_BY_GROUP_RSP        0xc4
//end uart cmd define


//define all light groups
#define ALL_LIGHT_GROUPS	12
#define GROUPS_ADDR			1


#define MAX_UART_RX_BUF_LENTH   256
#define MAX_UART_RX_BUF_NUM     2

#define UART_PACKET_BASIC_LEN				1
#define UART_DATA_HEAD					0xfa
#define UART_DATA_END						0xf5


#define MONITOR_FIND_FOR_ZC_SHORTADDR	0
#define MONITOR_FINE_FOR_ANY_SHORTADDR	0xffff

enum{
 MONITOR_SUB_CMD_ONOFF	= 0x01,
 MONITOR_SUB_CMD_LEVEL,
 MONITOR_SUB_CMD_COLOR,
 MONITOR_SUB_CMD_CUTRGB
};


typedef struct{
uint8_t head;
uint8_t firmwareValid;
uint8_t devUpdateNeedFlag;
uint8_t flashSaveImageType;
uint8_t devImageVersion;
uint8_t needDownloadImageFromCloud;
uint32_t devImageSize;
uint8_t end;
}DEV_OTA_STATION;


typedef enum{
ZB_NET_WORK_NORMAL = 0,
ZB_NET_WORK_PERMIT_JOINING
}ZIGBEE_NET_STATION;

typedef enum{
ZB_INVALID_DATA = 0,
ZB_NORMAL_DATA = 1,
ZB_IMPORTANT_DATA = 2
}ZB_DATA_TYPE;


typedef enum{
SENSOR_DISARM = 0,
SENSOR_ARM,
SENSOR_HOME_ARM
}ARM_TYPE;

typedef struct{
uint8_t data[MAX_UART_RX_BUF_LENTH];
ZB_DATA_TYPE validFlag;
}UART_RX_DATA_BUF;


#define MAX_UART_TX_CMD_BUF_NUM             10
#define UART_FIND_BUF_INVALID_INDEX         0xff

typedef struct{
uint8_t inData[20];
uint8_t inDataLen;
}INPAYLOAD;

typedef struct{
uint8_t *outData;
uint8_t outDataLen;
}OUTPAYLOAD;

typedef struct{
uint8_t inCmd;
uint8_t dstEp;//ep跟daar type复用，当incmd为组播/广播控制的时候，该位表示addr type
uint16_t dstAddr;
INPAYLOAD inPayload;
ZB_DATA_TYPE cmdValid;
}UART_TX_CMD_BUF;

#define MAX_BACKUP_DATA_LEN				64
typedef struct{
uint8_t bkLen;
uint8_t bkBuff[MAX_BACKUP_DATA_LEN];
}CMD_BACKUP;

#define MAX_UART_CMD_MONITOR_NUM 		8
typedef struct{
uint8_t validFlag;
uint8_t cmd;
uint8_t ep;
uint16_t shortAddr;
uint32_t sendTime100Ms;
uint8_t sendCount;
uint8_t rspRcvFlag;
CMD_BACKUP cmdbk;
}UARTCMDMONITOR;

typedef enum{
UART_WORK_COMMUNICATION,
UART_WORK_OTA
}UARTWORKTYPE;

typedef struct{
uint32_t lastCommuTime100ms;//最大
uint8_t uartWorkDataType;
uint8_t uartHeartSendCount;
}UARTHEARTINFO;

#define MAX_WOFTWARE_ADDR_LEN		128
typedef struct{
uint8_t haveNewestSoftWare;
uint8_t appAllowSoftWareUpdate;
char softUrl[MAX_WOFTWARE_ADDR_LEN];
}SOFT_MANAGE;

typedef struct{
uint8_t zcSoftVersion;
uint8_t channel;
uint16_t panId;
uint8_t zbMac[8];
uint8_t appAllowSoftWareUpdate;
}SOFT_ZC;

typedef struct{
uint8_t gwIsAlarning;
uint8_t gwZbIsJoining;//0 none; 1-joining(第一位); 2-dev has joined(第二位，一般此状态gwZbIsJoining值为3，即第一位也是置1的)
uint8_t gwDevIsOta; //0 none ota; 1- wifi ota; 2- zc ota; 3-zed ota;
uint8_t exPowerDown;
T_SYSTEM_TIME sysRealTime;
uint32_t sysRealTimeFors;
uint8_t sysRealTimerValid;
uint8_t environment_lux;//环境光强度，0-光照度弱，1-光照度强
uint32_t xlinkDevId;
uint8_t xlinkAuCode[17];
char accessToken[128];
char refreshToken[128];
uint8_t httpsConnect;
uint8_t wifiConnect;
SOFT_MANAGE wifiSoftUpdate;
SOFT_ZC zigbeeZcBasicInfo;
uint8_t echoNeedUpdateFlag;
//add
uint8_t ota_id;
uint8_t ota_type;
}GWSTATFORTIME;



typedef struct{
uint8_t subOtaStart;
uint8_t subFileType;
uint8_t subFileVersion;
uint8_t subFileMd5[32];
char *subFileUrl[MAX_WOFTWARE_ADDR_LEN];
uint32_t sufFileSize;
uint32_t flashAddr;
}SUBOTARAM;


void userDeleteDevFromLightNightAndHomeArm(uint16_t index);
void userDeleteDevFromWDLinkList(uint16_t index);
void userDeleteDevFromSceneAndLinkage(uint8_t index);
void user_flash_delete_all_node_info(void);
int checkDataAllowConditionToAnother(int sourceData,unsigned char condition,int dstData);
void zbdev_and_scene_manage_task(unsigned int arg);
void userInitTheLinkageTimerEvent(char *linkInfo);
char is_today (unsigned char today, unsigned char flag);
void userInitGwArmForTimerEvent(char *gwInfo);
void userUpdateGwArmForTimerEvent(char *gwInfo);
void userInitTheDevTimerEvent(char *devInfo);
void userUpdateTheDevTimerEvent(char *devInfo);
void gpio_init_func(void);
void userUpdateDevDataToCloud(void);
void userOtaAndHttpTaskInit(void);
int get_dot_num_from_OID(char *OID);
int get_last_id_from_OID(char *OID);
int get_id_from_OID(char *OID, int n)	;
void timer_work_task(unsigned int arg);
void userControlGWLedFunc(uint8_t state,uint16_t timers);
void userLightWitheOnOffControl(uint8_t onoff);
void light_work_task(unsigned int arg);
HMTM userChangeSTimeToYears(uint32_t time);
void userNetDataDealThread(unsigned int arg);
double Atof(const char *pstr);
void userDeleteOneDevFromRamAndFlash(HM_DEV_RAM_LIST *pni);
void userAddDevNumToNumInfo(uint16_t index);
void userUpdateDevInfoDataToFlash(HM_DEV_RAM_LIST *pni);
unsigned char is_time_achieve_hm(uint8_t enable, uint8_t sh, uint8_t sm, uint8_t eh, uint8_t em, uint8_t wkflag);
long hm_mktime(HMTM *times);
uint32_t get_user_times(void);
void putNiInList(HM_DEV_RAM_LIST *pni_ram);
void removeNodeList(HM_DEV_RAM_LIST *pni);
HM_DEV_FLASH_INFO userFlashReadDevInfoFunc(uint32_t addr);
void getValidDevNameAndDevType(HM_DEV_RAM_LIST *pni,uint8_t *payload,uint8_t len);
uint8_t findFreeSceneIndex(void);
uint8_t findFreeLinkageIndex(void);
uint16_t findFreeDevIndex(void);
void userFlashInitToRam(void);
HM_DEV_RAM_LIST *findInDevListByIndex(uint16_t index);
HM_DEV_RAM_LIST *findInDevListByNwkadd(uint16_t shortAdd);
HM_DEV_RAM_LIST *findInDevListByMac(uint8_t *zclAdd);
void userCheckSaveFlashAndReset(uint8_t ramChangeFlag);
void user_manage_button_func(unsigned int arg);
void user_make_aes_key(char *key16);
unsigned int DecryptBuf(char *lpInBuf, unsigned int dwDataLen, char *lpOutBuf, char *lpKey16Byte);
unsigned int EncryptBuf(char *lpInBuf, unsigned int dwDataLen, char *lpOutBuf, char *lpKey16Byte);
void user_https_task_start(unsigned int arg);
void user_check_connect_network(unsigned int arg);
void hm_network_init(void);
uint16_t user_aes_encode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key);
uint16_t user_aes_decode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key);
void uart_work_task_init(void);
void beep_work_task(unsigned int arg);
void uartRcvDataDeal(uint8_t* data,uint16_t dataLen);
void uartTxDataCmdDeal(void);
void zbdev_and_scene_manage_thread(void *inContext);
void userFlashWriteNumFunc(uint8_t* data);
DEV_NUM_INFO userFlashReadNumFunc(void);
void userFlashWriteDevInfoFunc(uint8_t* data,uint32_t addr);
void userFlashWriteSceneLinkageInfoFunc(uint8_t* data,uint32_t addr,uint16_t dataLen);
void zigbeeModeReset(void);

void hm_client_event_task(void);
void userFlashWriteSysInfoFunc(uint8_t* data);


#endif
