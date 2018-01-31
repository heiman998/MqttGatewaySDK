#ifndef _XLINK_MESSAGE_H_
#define _XLINK_MESSAGE_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "xlink_type.h"


//协议版本
#define  PROTOCOL_VERSION  3

//#define REVERSED

typedef enum _eMessageType {
	E_TYPE_SCAN = 1, //
	E_TYPE_HANDSHAKE = 2, //
	E_TYPE_PROBE = 3, //
	E_TYPE_SET = 4, //
	E_TYPE_SYNC = 5, //
	E_TYPE_DEVONLINE = 6, //
	E_TYPE_TICKEY = 7, //
	E_TYPE_PIPE = 8, //
	E_TYPE_SETPASSWORD = 9,//
	E_TYPE_PRODUCT_TEST=10, //
	E_TYPE_PING = 13, //
	E_TYPE_BYEBYE = 14, //
	E_TYPE_TIME = 15, //
	E_TYPE_SETACK = 11, //
    E_TYPE_SUBKEY = 7, //
} XLINK_EMESSAGETYPE;

/**
 * 固定协议头定义
 */
typedef volatile union {
	/*unsigned*/char byte; /**< the whole byte */
#if defined(REVERSED)
	struct
	{
		unsigned int type : 4; /** 任务类型 */
		unsigned int resp : 1; /** bit or request(0) or response(1) */
		unsigned int version : 3;/** 预留3 bits */
	}bits;
#else
	struct {
		unsigned int version :3;/** 预留3 bits */
		unsigned int resp :1; /** bit or request(0) or response(1) */
		unsigned int type :4; /** 任务类型 */
	} bits;
#endif
} xlink_Message;

/**
 * Scan
 */
typedef struct {
	xlink_Message header;
	unsigned char version;
	unsigned short port;
	unsigned char reserved;
} xlink_Scan;
static const int msgScanLen = 4;

typedef struct {
	xlink_Message header; //1byte
	unsigned char version; //1byte
	unsigned char mcu_hard_ver;	//1
	unsigned short mcu_soft_ver;	//1
	unsigned short port;	//2
	union {
		unsigned char all; /**< all set flags */
#if defined(REVERSED)
		struct
		{
			int reserved : 7;
			unsigned int device_name : 1; /** device_name */
		}bits;
#else
		struct {
			unsigned int device_name :1; /** device_name */
			int reserved :7;
		} bits;
#endif
	} flags; /**< connect flags byte */
} xlink_ScanBack;
static const int msgScanBackLen = 81;

/**
 *  Handshake
 */
typedef struct {
	xlink_Message header;
	unsigned char version;
	unsigned short port;
	unsigned char reserved;
	short keepAliveTime;
} xlink_Handshake;
static const int msgHandshake = 40;

typedef struct {
	xlink_Message header;
} xlink_HandshakeBack;
#define MsgHandShakeBackLen  16

static const int msgByeByeLen = 2;

static const int msgPingLen = 4;

//#define MsgPingBackLen  7

static const int msgProbeLen = 3;

/**
 *  Sync
 */
typedef struct {
	xlink_Message header;
	union {
		unsigned char all; /**< all set flags */
		struct {
#if defined(REVERSED)
			int reserved :7;
			unsigned int device_name :1; /** device_name */

#else
			unsigned int device_name :1; /** device_name */
			int reserved :7;
#endif
		} bits;
	} flags; /**< connect flags byte */
} Sync;

typedef volatile union {
	unsigned char all; /**< all set flags */
	struct {
#if defined(REVERSED)
		int reserved :6;
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		unsigned int device_name :1; /** device_name */
		
#else
		
		unsigned int device_name :1; /** device_name */
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		int reserved :6;
#endif
	} bits;
} SyncFlags;
/**< connect flags byte */
#define  MsgSyncLen  11

typedef volatile union {
	unsigned char all; /**< all set flags */
	struct {
#if defined(REVERSED)
		int reserved :6;
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		unsigned int device_name :1; /** device_name */
#else
		unsigned int device_name :1; /** device_name */
		
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		int reserved :6;
#endif
	} bits;

} Setflags; /**< connect flags byte */
static const int msgSetLen = 5;

typedef volatile union {
	unsigned char all; /**< all set flags */
	struct {
#if defined(REVERSED)
		int reserved :6;
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		unsigned int device_name :1; /** device_name */
#else

		unsigned int device_name :1; /** device_name */
		
		unsigned int datapoint_v2 :1; /** datapoint V2 */
		int reserved :6;

#endif
	} bits;

} Set_flags; /**< connect flags byte */

#ifdef  __cplusplus
}
#endif
#endif
