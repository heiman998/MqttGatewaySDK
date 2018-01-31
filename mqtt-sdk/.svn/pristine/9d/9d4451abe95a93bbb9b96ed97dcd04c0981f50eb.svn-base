/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: internal header file for serial device (logical layer)
//
// @Notes:
//
//

#ifndef SERIAL_INTERNAL_H_
#define SERIAL_INTERNAL_H_

#include "serialport_api.h"
#include "serialport_ioctl.h"

    /* auto port events that application does not need to explicity request for */
#define SER_PORT_AUTO_EVENTS  (SERIAL_EV_RX_AVAIL_MASK          | \
                               SERIAL_EV_TX_ROOM_AVAIL_MASK     | \
                               SERIAL_EV_DEVICE_DOWN_MASK       | \
                               SERIAL_EV_DEVICE_UP_MASK)

typedef struct {
    A_UINT8     *pBuffer;       /* ring buffer */
    int         WriteIndex;     /* current write index (with wrap) */
    int         ReadIndex;      /* current read index (with wrap) */
    int         Size;           /* size of ring buffer */ 
    int         ValidBytes;     /* current bytes in ring buffer */
	A_BOOL      Lock;			/* lock status */
} SER_PORT_HW_RING_BUFFER;

#define SER_PORT_RING_BUFF_ROOM(pR) ((pR)->Size - (pR)->ValidBytes)

    /* generic IO (RX or TX) timer */
typedef struct {
    A_UINT8     Flags;              /* timer state flags */
    A_timer_t   Timer;              /* timer */
    int         ThresholdBytes;     /* threshold level for the IO timeout */
    int         TimeOutMS;          /* timeout in milliseconds */
} SER_PORT_IO_TIMER;

#define SER_IO_TIMER_ACTIVE     (1 << 0)
#define SER_IO_TIMER_DUMMY      (1 << 1)

#define IS_IO_TIMER_ACTIVE(pIOT)     ((pIOT)->Flags & SER_IO_TIMER_ACTIVE)

typedef struct _SER_PORT_INSTANCE_INFO {    
    struct _SER_PORT_INSTANCE_INFO      *pNext;      /* list management */
    SER_PORT_HW_DEV                     *pHWPort;    /* associated HW layer */
    SER_PORT_HW_RING_BUFFER             RXBuffer;    /* RX ring buffer managed by this layer */
    SER_PORT_HW_RING_BUFFER             TXBuffer;    /* TX ring buffer managed by this layer */
    A_UINT16                            EventMask;   /* current even mask that application wants notifications for */             
    SER_PORT_IO_TIMER                   RXTimer;     /* timer used for RX timeouts */
    SER_PORT_IO_TIMER                   TXTimer;     /* timer used for TX timeouts */
    void                                *pEvCbContext; /* application event callback context */
    SERIAL_EVENT_CB                     EventCallback; /* application event callback for port events */ 
    A_UINT8                             PortStateFlags;  /* internal port state flags */
} SER_PORT_INSTANCE_INFO;

#define SER_PORT_STATE_NEED_RX_DRAIN   (1 << 0)
#define SER_PORT_STATE_HW_UP           (1 << 1)                        

#define PORT_STATE_CLEAR_MASK          (~SER_PORT_STATE_NEED_RX_DRAIN)
#define IS_SER_PORT_UP(p)              ((p)->PortStateFlags & SER_PORT_STATE_HW_UP)

typedef struct {
    SER_PORT_INSTANCE_INFO  *pPortList;     /* port list of all registered serial ports */
} SERPORT_MGR_INFO;


/* internal APIs */
void SerP_RXTimerCallback(A_HANDLE alarm, void *data);
void SerP_TXTimerCallback(A_HANDLE alarm, void *data);
void SerP_CancelTimer(SER_PORT_IO_TIMER *pIOTimer);
void _NotifyIOThresholdEvent(SER_PORT_INSTANCE_INFO  *pInfo,
                             SERIAL_EVENT_TYPE       EventType);
void _SerPFlushBuffer(SER_PORT_HW_RING_BUFFER *pRingBuffer);
int  _SerPPushBytes(SER_PORT_HW_RING_BUFFER *pRingBuffer,A_UINT8 *pBuffer,int Length);
int  _SerPPopBytes(SER_PORT_HW_RING_BUFFER *pRingBuffer,A_UINT8 *pBuffer,int MaxLength);
void _CleanupSerialPort(SER_PORT_INSTANCE_INFO *pInfo, A_UINT16 EventMaskPreserve);

typedef struct SERP_INTERNAL_APIS {
    void (*_NotifyIOThresholdEvent)(SER_PORT_INSTANCE_INFO  *pInfo,
                                    SERIAL_EVENT_TYPE       EventType);
    void (*_SerPFlushBuffer)(SER_PORT_HW_RING_BUFFER *pRingBuffer);
    int  (*_SerPPushBytes)(SER_PORT_HW_RING_BUFFER *pRingBuffer,A_UINT8 *pBuffer,int Length);
    int  (*_SerPPopBytes)(SER_PORT_HW_RING_BUFFER *pRingBuffer,A_UINT8 *pBuffer,int MaxLength);
    void (*_CleanupSerialPort)(SER_PORT_INSTANCE_INFO *pInfo, A_UINT16 EventMaskPreserve);
} SERP_INTERNAL_APIS;

extern SERP_INTERNAL_APIS g_SerpIndirectionTable;
   
#define CALL_SERP_INDIR_FN(fn) g_SerpIndirectionTable.fn
#define GET_SERP_INDIR_FN(fn)  g_SerpIndirectionTable.fn

#define NotifyIOThresholdEvent(p,E) CALL_SERP_INDIR_FN(_NotifyIOThresholdEvent(p,E))
#define SerPFlushBuffer(pR)         CALL_SERP_INDIR_FN(_SerPFlushBuffer(pR))
#define SerPPushBytes(pR,pB,L)      CALL_SERP_INDIR_FN(_SerPPushBytes(pR,pB,L))
#define SerPPopBytes(pR,pB,L)       CALL_SERP_INDIR_FN(_SerPPopBytes(pR,pB,L))
#define CleanupSerialPort(p,E)      CALL_SERP_INDIR_FN(_CleanupSerialPort(p,E))

#define SRAPI

#endif /*SERIAL_INTERNAL_H_*/
