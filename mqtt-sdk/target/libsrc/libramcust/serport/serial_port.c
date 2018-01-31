/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: serial port interface for the HW-specific layer
//
// @Notes:
//
//

#include <osapi.h>
#include "serial_internal.h"

extern SERPORT_MGR_INFO *g_pSerPManager;

SRAPI void _SerPFlushBuffer(SER_PORT_HW_RING_BUFFER *pRingBuffer)
{
    pRingBuffer->ReadIndex = 0;
    pRingBuffer->WriteIndex = 0;
    pRingBuffer->ValidBytes = 0;   
    pRingBuffer->Lock = FALSE;  
}

    /* push bytes into the ring buffer, returns bytes pushed */
SRAPI int _SerPPushBytes(SER_PORT_HW_RING_BUFFER *pRingBuffer, A_UINT8 *pBuffer, int Length)
{
    int i;
        
    pRingBuffer->Lock = TRUE; 
    Length = A_MIN(Length,SER_PORT_RING_BUFF_ROOM(pRingBuffer));
    A_ASSERT(Length >= 0);
            
    for (i = 0; i < Length; i++,pBuffer++) {
        pRingBuffer->pBuffer[pRingBuffer->WriteIndex] = *pBuffer;
        pRingBuffer->WriteIndex++;
        if (pRingBuffer->WriteIndex >= pRingBuffer->Size) {
                /* wrap */
            pRingBuffer->WriteIndex = 0;    
        }  
    }    
    pRingBuffer->ValidBytes += i;
    pRingBuffer->Lock = FALSE;
    return i;
}

    /* pop bytes from the ring buffer, returns number of bytes popped */
SRAPI int _SerPPopBytes(SER_PORT_HW_RING_BUFFER *pRingBuffer, A_UINT8 *pBuffer, int MaxLength)
{
    int i;
    
    pRingBuffer->Lock = TRUE;
    MaxLength = A_MIN(MaxLength,pRingBuffer->ValidBytes);    
    A_ASSERT(MaxLength >= 0);
    
    for (i = 0; i < MaxLength; i++,pBuffer++){
        *pBuffer = pRingBuffer->pBuffer[pRingBuffer->ReadIndex];
        pRingBuffer->ReadIndex++;
        if (pRingBuffer->ReadIndex >= pRingBuffer->Size) {
                /* wrap */
            pRingBuffer->ReadIndex = 0;    
        }
    } 
    pRingBuffer->ValidBytes -= i;
    pRingBuffer->Lock = FALSE;
    return i;
}

static SER_PORT_INSTANCE SERHWAPI_RegisterPort(SER_PORT_HW_DEV *pPortDev)
{
    SER_PORT_INSTANCE_INFO *pInfo;
    
    pInfo = (SER_PORT_INSTANCE_INFO *)A_ALLOCRAM(sizeof(SER_PORT_INSTANCE_INFO));
    pInfo->pHWPort = pPortDev;
    A_ASSERT(pPortDev->Properties.RXBufferSize != 0);
    A_ASSERT(pPortDev->Properties.TXBufferSize != 0);
        /* allocate ring buffers on behalf of the hardware */
    pInfo->RXBuffer.Size = pPortDev->Properties.RXBufferSize;
    pInfo->RXBuffer.pBuffer = A_ALLOCRAM(pInfo->RXBuffer.Size);
    pInfo->TXBuffer.Size = pPortDev->Properties.TXBufferSize;
    pInfo->TXBuffer.pBuffer = A_ALLOCRAM(pInfo->TXBuffer.Size);
        /* setup I/O timers, this layer handles all stream IO timeouts on behalf of hardware */
    A_INIT_TIMER(&pInfo->RXTimer.Timer,SerP_RXTimerCallback,pInfo);
    A_TIMER_SET_FLAGS(&pInfo->RXTimer.Timer,TIMER_FLAGS_NO_FORCE_DISARM);
    A_INIT_TIMER(&pInfo->TXTimer.Timer,SerP_TXTimerCallback,pInfo);  
    A_TIMER_SET_FLAGS(&pInfo->TXTimer.Timer,TIMER_FLAGS_NO_FORCE_DISARM);

    if (!(pInfo->pHWPort->Properties.Flags & SER_PORT_PROP_FLAGS_REMOVABLE)) {
            /* if the hardware is not removable, it's up and available */
        pInfo->PortStateFlags |= SER_PORT_STATE_HW_UP;
    }
       
        /* save this port to the list */
    pInfo->pNext = g_pSerPManager->pPortList;
    g_pSerPManager->pPortList = pInfo;

    return (SER_PORT_INSTANCE)pInfo;
}

int SERHWAPI_PushRXBytes_Push(SER_PORT_INSTANCE Instance, A_UINT8 *pBuffer, int Length)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;
    int                    bytesPushed;
    
    if (!pInfo->pHWPort->OpenCount) {
        return 0;    
    }
    
    bytesPushed = SerPPushBytes(&pInfo->RXBuffer,pBuffer,Length);
    
    return bytesPushed;
}

void SERHWAPI_PushRXBytes_Notify(SER_PORT_INSTANCE Instance)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;
    if (!pInfo->pHWPort->OpenCount) {
        return;    
    }
    if (pInfo->EventMask & SERIAL_EV_RX_AVAIL_MASK) {
        if (pInfo->RXBuffer.ValidBytes >= pInfo->RXTimer.ThresholdBytes) { 
            if (IS_IO_TIMER_ACTIVE(&pInfo->RXTimer)) {  
                    /* cancel timer */
                SerP_CancelTimer(&pInfo->RXTimer); 
            } 
                /* NOTE: the notification could result in a recursion if the app called 
                 * _Read() AND DrainRX() was called at the HW layer,
                 *  the  Read() API will temporarily disable the event mask there 
                 *  to prevent a recursive path */           
            NotifyIOThresholdEvent(pInfo,SERIAL_EVENT_RX_AVAIL);  
        }    
    }
}

/* Only used in ISR */
A_BOOL SERHWAPI_RXBuffer_Lock(SER_PORT_INSTANCE Instance)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;

	return pInfo->RXBuffer.Lock;
}

/* Only used in ISR */
A_BOOL SERHWAPI_TXBuffer_Lock(SER_PORT_INSTANCE Instance)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;

	return pInfo->TXBuffer.Lock;
}

    /* push bytes into the receive buffer from the hardware layer */
static int SERHWAPI_PushRXBytes(SER_PORT_INSTANCE Instance, A_UINT8 *pBuffer, int Length)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;
    int                    bytesPushed;
    
    if (!pInfo->pHWPort->OpenCount) {
        return 0;    
    }
    
    bytesPushed = SerPPushBytes(&pInfo->RXBuffer,pBuffer,Length);
    
    if (bytesPushed < Length) {
        if (pInfo->pHWPort->Properties.Flags & SER_PORT_PROP_FLAGS_RX_NO_DROP) {
                /* not all the bytes could fit, set flag to call back to the 
                 * hardware layer to drain any remaining bytes */
            pInfo->PortStateFlags |= SER_PORT_STATE_NEED_RX_DRAIN;    
        }
    }
    
    if (pInfo->EventMask & SERIAL_EV_RX_AVAIL_MASK) {
        if (pInfo->RXBuffer.ValidBytes >= pInfo->RXTimer.ThresholdBytes) { 
            if (IS_IO_TIMER_ACTIVE(&pInfo->RXTimer)) {  
                    /* cancel timer */
                SerP_CancelTimer(&pInfo->RXTimer); 
            } 
                /* NOTE: the notification could result in a recursion if the app called 
                 * _Read() AND DrainRX() was called at the HW layer,
                 *  the  Read() API will temporarily disable the event mask there 
                 *  to prevent a recursive path */           
            NotifyIOThresholdEvent(pInfo,SERIAL_EVENT_RX_AVAIL);  
        }    
    } 
    
    return bytesPushed;
}

    /* pop bytes from the TX buffers (called by HW layer */
static int SERHWAPI_PopTXBytes(SER_PORT_INSTANCE Instance, A_UINT8 *pBuffer, int MaxBuffer)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;
    int                    bytesPopped;
    
    bytesPopped = SerPPopBytes(&pInfo->TXBuffer,pBuffer,MaxBuffer);  
    
    if (pInfo->EventMask & SERIAL_EV_TX_ROOM_AVAIL_MASK) {
        if (SER_PORT_RING_BUFF_ROOM(&pInfo->TXBuffer) >= pInfo->TXTimer.ThresholdBytes) { 
            if (IS_IO_TIMER_ACTIVE(&pInfo->TXTimer)) {  
                    /* cancel timer */
                SerP_CancelTimer(&pInfo->TXTimer); 
            }       
                 /* NOTE: the notification could result in a recursion if the caller called 
                  * _Write(), the code will temporarily disable the event mask there */
            NotifyIOThresholdEvent(pInfo,SERIAL_EVENT_TX_ROOM_AVAIL);   
        }
    }   
          
    return bytesPopped;
}

static void SERHWAPI_IndicatePortEvent(SER_PORT_INSTANCE Instance,
                                       A_UINT16          Event, 
                                       A_UINT32          arg0, 
                                       A_UINT32          arg1)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)Instance;
    A_BOOL                  notify = FALSE;
   
    switch (Event) {
        case SERIAL_EVENT_DEVICE_DOWN :
            pInfo->PortStateFlags &= ~SER_PORT_STATE_HW_UP;
                /* cleanup port, but maintain current event mask state */
            CleanupSerialPort(pInfo,pInfo->EventMask);
            notify = TRUE;
            break;
        case SERIAL_EVENT_DEVICE_UP :
            pInfo->PortStateFlags |= SER_PORT_STATE_HW_UP;
            notify = TRUE;
            break;
        case SERIAL_EVENT_LINE_ERROR :
            notify = TRUE;
            break;
        case SERIAL_EVENT_RX_WAKE :
            break;
        default:
            A_ASSERT(FALSE);
    }
    
    if (notify && 
        (pInfo->EventMask & MAKE_PORT_EVENT_MASK(Event)) &&
        (pInfo->EventCallback != NULL)) {
                /* pass to the application */
        pInfo->EventCallback(pInfo->pEvCbContext,Event,arg0,arg1);    
    }
     
}
    
void serport_hw_api_install(struct serp_apis *pAPIs)
{  
    pAPIs->_RegisterPort = SERHWAPI_RegisterPort;
    pAPIs->_PushRXBytes = SERHWAPI_PushRXBytes;
    pAPIs->_PopTXBytes = SERHWAPI_PopTXBytes;
    pAPIs->_IndicatePortEvent = SERHWAPI_IndicatePortEvent;
}

