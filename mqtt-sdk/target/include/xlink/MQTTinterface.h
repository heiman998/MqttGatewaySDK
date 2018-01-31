#ifndef _MQTTINTERFACE_H_
#define _MQTTINTERFACE_H_

#include "stdint.h"


#define MQTT_DEBUG_ON
#ifdef  MQTT_DEBUG_ON
#define Loginfo( format, ... ) printf(format, ## __VA_ARGS__ )//Loginfo  printf //Loginfo( format, ... ) printf( format, ...  )
#else
#define Loginfo( format, ... )
#endif

#define MACTOSTR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACTSTR 	"%02x%02x%02x%02x%02x%02x"
#define FACTORYID		"1000" 
#define PRODUCTID		"10000"
#define ReqQos			1








typedef struct NETWORK_CALLBACK_FUN {
	void (*TCPsetServerStatus)(unsigned char stat);

	unsigned short maclen;
	unsigned char mac[30];
	unsigned char factory_id[5];
	unsigned char product_id[6];
	unsigned int system_time;
	unsigned char gettime_flag;
	unsigned int server_deviceid;//unsigned char server_deviceid[5];

}NETWORK_USER_CONFIG;


typedef enum {
	WIFI_INIT,
	WIFI_CONNECTING,
	WIFI_CONNECTING_ERROR,
	WIFI_CONNECTED,
	DNS_RESOLVE,
	TCP_DISCONNECTING,
	TCP_DISCONNECTED,
	TCP_RECONNECT_DISCONNECTING,
	TCP_RECONNECT_REQ,
	TCP_RECONNECT,
	TCP_CONNECTING,
	TCP_CONNECTING_ERROR,
	TCP_CONNECTED,
	MQTT_CONNECT_SEND,
	MQTT_CONNECT_SENDING,
	MQTT_CONNACK,
	MQTT_SUBSCIBE_SEND,
	MQTT_SUBSCIBE_SENDING,
	MQTT_SUBSACK,
	MQTT_DATA_HANDLE,
	MQTT_KEEPALIVE_SEND,
	MQTT_PUBLISH_RECV,
	MQTT_PUBLISHING,
	MQTT_DELETING,
	MQTT_DELETED,
	MQTT_GET_SERVERDEVIEID,
	CONNSTATE_NULL
} tConnState;

typedef enum{
	MQTT_TOPIC_FIRST,
	MQTT_TOPIC_SECOND,
	MQTT_TOPIC_THIRD,
	MQTT_TOPIC_FOURTH,	
	MQTT_TOPIC_END,
	MQTT_NULL
}MQTTTopic;

typedef enum{
	mqtt_cs_null = 0,
	mqtt_cs_connected = 1,
	mqtt_cs_disconnecting = 2
}tconnetedStatus;


typedef struct{
uint8_t pingreq_work_flag;
uint8_t work_type;
uint8_t pingtoggle;
uint16_t pingack_count;
uint8_t received_ping_ack;
uint16_t ping_count;
uint8_t gettime_work_flag;
uint16_t gettime_count;
}MQTTINFO;


typedef void (*MqttCallback)(uint32_t *args);
typedef void (*MqttDataCallback)(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh);

typedef struct  {
	//struct espconn *pCon;
	uint8_t security;
	uint8_t* host;
	uint32_t port;
	//ip_addr_t ip;
	//mqtt_state_t  mqtt_state;
	//mqtt_connect_info_t connect_info;
	MqttCallback connectedCb;
	MqttCallback disconnectedCb;
	MqttCallback publishedCb;
	MqttCallback timeoutCb;
	MqttDataCallback dataCb;
	//ETSTimer mqttTimer;
	uint32_t keepAliveTick;
	uint32_t reconnectTick;
	uint32_t sendTimeout;
	tConnState connState;
	tconnetedStatus connetedStatus;
//	QUEUE msgQueue;
	void* user_data;
} MQTT_Client;

void MQTTclient_connState_init(void);
void MQTT_main(void);
void mqtt_handle(void);

#endif

