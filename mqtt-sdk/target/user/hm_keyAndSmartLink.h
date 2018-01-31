#ifndef HM_KEYANDSMARTLINK_H
#define HM_KEYANDSMARTLINK_H


/**
* @brief: working mode
*/
//extern DEV_MYSELF_INFO hmIotsDevInfo;

typedef enum{
	NONE_MODE	= -1,
	SOFTAP_MODE = 0,
	STATION_MODE,
	SNIFFER_MODE,
	AIRKISS_MODE,
	CONFIG_MODE,
	LTLINK_MODE,
}ework_mode_type;

enum{
KEY_VALID_NUM_NONE = 0,
KEY_VALID_NUM_NORMAL = 1,
KEY_VALID_NUM_ZB_MAKE_JOIN = 2,
KEY_VALID_NUM_RESET_GW = 10,
KEY_VALID_NUM_LONG_TIME_SMARTLINK = 50
};


typedef enum
{
	IDEL_STATE = 0,
	CONFIG_STATE,
	CONNECTTING_STATE,
	CONNECTED_STATE,
}ework_state_type;

typedef struct {
	unsigned char connect_state;
	unsigned char remote_state;
}tGlobal_net_type;

#define IPV4_ADDR(addr) (addr) >> 24 & 0xFF, (addr) >> 16 & 0xFF, (addr) >> 8 & 0xFF, (addr)& 0xFF
#define IPV4_FMT "%d.%d.%d.%d"

#define VERSION(ver) (ver) >> 12 & 0xF, (ver) >> 8 & 0xF, (ver)& 0xFF
#define VERSION_FMT "%d.%d.%02x"




#endif

