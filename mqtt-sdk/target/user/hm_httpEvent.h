#ifndef _HM_HTTPS_H
#define _HM_HTTPS_H
#define DEV_SOFT_VERSION_BASIC_VALUE		10
#define DEV_SOFT_FIRMWARE_TYPE_SUBDEV	3
#define DEV_SOFT_FIRMWARE_INDENTIFY_ZC      770


typedef enum{
HTTPS_ADD_DEV_CMD = 1,
HTTPS_UPDATE_DEV_CMD,
HTTPS_DELETE_DEV_CMD
}HTTPSCMDTYPE;



#define MAX_HTTPS_SEND_BUF_LEN		1024
#define MAX_HTTPS_SEND_BUF_NUM	5
typedef struct{
uint8_t validFlag;
uint8_t cmdMode;
uint8_t sysDevType;
uint8_t subDevType;
uint8_t data[MAX_HTTPS_SEND_BUF_LEN];
}HTTPSSENDBUF;

#ifdef HM_TEST
#define HTTPS_BASIC_HEADER		"Host: console.heiman.cn\r\nContent-Type:application/json\r\n"
#else
#ifdef HEIMAN_APP
#define HTTPS_BASIC_HEADER		"Host: console.heiman.cn\r\nContent-Type:application/json\r\n"
#else 
#ifdef YUNZHIYI_APP
#define HTTPS_BASIC_HEADER		"Host: api2.xlink.cn\r\nContent-Type:application/json\r\n"
#endif
#endif
#endif

#define HTTPS_RCV_HEADER_OK	"HTTP/1.1 200 OK"


void userFillHttpsBufForDev(uint8_t subDevType,uint8_t *data,uint8_t cmd);



#endif


