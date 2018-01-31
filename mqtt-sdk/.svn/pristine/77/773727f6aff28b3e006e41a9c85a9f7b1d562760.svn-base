/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: Generic serial device application interface implementation
//
// @Notes:
//
//

#include <osapi.h>
#include "serial_internal.h"


SERPORT_MGR_INFO *g_pSerPManager = NULL;

extern A_BOOL serial_port_start;

/****** Exported APIS *****************/


static void SERPAPI_Init(void)
{
	if (!serial_port_start)
	{
    	g_pSerPManager = (SERPORT_MGR_INFO *)A_ALLOCRAM(sizeof(SERPORT_MGR_INFO));
    	serial_port_start = TRUE;
	}
}

    /* notify that some I/O threshold event occured */
SRAPI void _NotifyIOThresholdEvent(SER_PORT_INSTANCE_INFO  *pInfo,
                                   SERIAL_EVENT_TYPE       EventType)
{
    A_ASSERT(pInfo->EventCallback != NULL);
    pInfo->EventCallback(pInfo->pEvCbContext,EventType,0,0);       
}

void SerP_CancelTimer(SER_PORT_IO_TIMER *pIOTimer)
{
    if (IS_IO_TIMER_ACTIVE(pIOTimer)) {
        A_UNTIMEOUT(&pIOTimer->Timer);
        pIOTimer->Flags &= ~SER_IO_TIMER_ACTIVE;   
    }
}

static void SerP_ScheduleTimer(SER_PORT_IO_TIMER *pIOTimer)
{   
        /* cancel any previous */
    if (IS_IO_TIMER_ACTIVE(pIOTimer)) {   
            /* cancel it first */
        SerP_CancelTimer(pIOTimer);    
    }   
        /* queue timer */
    pIOTimer->Flags |= SER_IO_TIMER_ACTIVE;    
        /* non repeating */
    A_TIMEOUT_MS(&pIOTimer->Timer,pIOTimer->TimeOutMS,0); 
}


void SerP_RXTimerCallback(A_HANDLE alarm, void *data)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)data;
    NotifyIOThresholdEvent(pInfo,SERIAL_EVENT_RX_TIMEOUT);
    /* NOTE:  application needs to rearm the timer */   
}

void SerP_TXTimerCallback(A_HANDLE alarm, void *data)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)data;
    NotifyIOThresholdEvent(pInfo,SERIAL_EVENT_TX_TIMEOUT); 
    /* NOTE: application needs to rearm the timer */
}

SRAPI void _CleanupSerialPort(SER_PORT_INSTANCE_INFO *pInfo, A_UINT16 EventMaskPreserve)
{
    SerPFlushBuffer(&pInfo->TXBuffer);
    SerPFlushBuffer(&pInfo->RXBuffer);
        /* cancel any pending timers */
    SerP_CancelTimer(&pInfo->RXTimer);
    SerP_CancelTimer(&pInfo->TXTimer);
    pInfo->RXTimer.Flags = 0;
    pInfo->TXTimer.Flags = 0;
    pInfo->EventMask = EventMaskPreserve;
    SERP_Ioctl((SERIAL_PORT_HANDLE)pInfo,SER_PORT_HW_IOCTL_STOP,NULL,0);   
}


static void SERPAPI_Close(SERIAL_PORT_HANDLE hPort)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)hPort;
    
    pInfo->pHWPort->OpenCount--;
    A_ASSERT(pInfo->pHWPort->OpenCount >= 0);
    CleanupSerialPort(pInfo,0);
}   
                                                               
    /* open a serial device */
static SERIAL_PORT_HANDLE SERPAPI_Open(A_CHAR                  *pPortName, 
                                       void                    *CbContext, 
                                       SERIAL_EVENT_CB         Callback, 
                                       SERIAL_PORT_PROPERTIES  *pProp)
{
    SER_PORT_INSTANCE_INFO  *pInfo;
    
    pInfo = g_pSerPManager->pPortList;
    
        /* match portname with ports in our list */    
    while (pInfo != NULL) {
        if (A_STRCMP(pPortName,pInfo->pHWPort->pPortName) == 0) {
                /* found it */
            break;
        }
        pInfo = pInfo->pNext;
    }
    
    do {
        
        if (pInfo == NULL) {
            break;                
        }        
#if 0
        if (pInfo->pHWPort->OpenCount) {
                /* at the moment, only 1 open context at a time */
            A_ASSERT(FALSE);            
            pInfo = NULL;
            break;  
        }   
#endif
        pInfo->pHWPort->OpenCount++;
        pInfo->PortStateFlags &= PORT_STATE_CLEAR_MASK;        
        pInfo->EventMask = 0;    
        pInfo->EventCallback = Callback;
        pInfo->pEvCbContext = CbContext;
        SerPFlushBuffer(&pInfo->RXBuffer);
        SerPFlushBuffer(&pInfo->TXBuffer);        
        A_MEMCPY(pProp,&pInfo->pHWPort->Properties,sizeof(SERIAL_PORT_PROPERTIES)); 
        if (pProp->Flags & SER_PORT_PROP_FLAGS_REMOVABLE) {
            pInfo->EventMask |= SERIAL_EV_DEVICE_DOWN_MASK | SERIAL_EV_DEVICE_UP_MASK;
        }
              
    } while (FALSE);
    
    return (SERIAL_PORT_HANDLE)pInfo;    
}

    /* write port API */
static int SERPAPI_Write(SERIAL_PORT_HANDLE hPort, A_UINT8 *pBuffer, int *pWriteLength)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)hPort;
 
    if (!IS_SER_PORT_UP(pInfo)) {
        if (pWriteLength != NULL) { 
            *pWriteLength = 0; 
        }
        return 0;    
    }

        /* NOTE : if pBuffer is NULL, the caller just wants the available room in the buffer */
    if (pBuffer != NULL) {
        int          bytesPushed;
        int          totalPushed = 0;
        int          remaining = *pWriteLength;
        A_UINT16     saved;
                   
        do {
            
            bytesPushed = SerPPushBytes(&pInfo->TXBuffer,&pBuffer[totalPushed],remaining);
            if (bytesPushed <= 0) {
                break;
            }                    
            totalPushed += bytesPushed;
            remaining -= bytesPushed;            
            saved = pInfo->EventMask;
                /* mask this to prevent a recursion path due to the notification of TX_ROOM_AVAIL */
            pInfo->EventMask &= ~SERIAL_EV_TX_ROOM_AVAIL_MASK;
                /* drain TX buffer to hardware */
            pInfo->pHWPort->HWTransmit(pInfo->pHWPort->pPortContext,pInfo->TXBuffer.ValidBytes);
                /* restore */
            pInfo->EventMask = saved;        
    
        } while (remaining > 0);
        
        *pWriteLength = totalPushed;
    }   
        
        /* return space that is left now */
    return SER_PORT_RING_BUFF_ROOM(&pInfo->TXBuffer);
}

    /* read port API */
static int SERPAPI_Read(SERIAL_PORT_HANDLE hPort, A_UINT8 *pBuffer, int *pBufferLength)
{
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)hPort;
    
    if (!IS_SER_PORT_UP(pInfo)) {
        if (pBufferLength != NULL) {
            *pBufferLength = 0;
        }
        return 0;    
    }
    
        /* if pBuffer is NULL, caller just wants the number of bytes available */
    if (pBuffer != NULL) {
        int remaining = *pBufferLength;
        int bytesRead;
        int totalBytes = 0;
        
        do {
            
            bytesRead = SerPPopBytes(&pInfo->RXBuffer,&pBuffer[totalBytes],remaining);  
            if (bytesRead <= 0) {
                break;
            }   
            
            totalBytes += bytesRead;
            remaining -= bytesRead;
           
                /* see if there are more bytes to drain */
            if ((pInfo->PortStateFlags & SER_PORT_STATE_NEED_RX_DRAIN) && (pInfo->pHWPort->DrainRX != NULL)) {
                A_UINT16 saved = pInfo->EventMask;
                    /* mask this to prevent a recursion path due to notifications of RX_AVAIL */
                pInfo->EventMask &= ~SERIAL_EV_RX_AVAIL_MASK;
                pInfo->PortStateFlags &= ~SER_PORT_STATE_NEED_RX_DRAIN;            
                    /* go get any bytes that didn't make it in while the ring buffer was full */
                pInfo->pHWPort->DrainRX(pInfo->pHWPort->pPortContext);
                    /* restore */
                pInfo->EventMask = saved;        
            }
            
        } while (remaining > 0);
        
        *pBufferLength = totalBytes;
    }
    
    return pInfo->RXBuffer.ValidBytes;
}

    /* ioctl dispatch */
static A_STATUS SERPAPI_Ioctl(SERIAL_PORT_HANDLE    hPort, 
                              A_UINT16              Code, 
                              void                  *pBuffer,
                              A_UINT32              LengthOrArg)
{
    A_STATUS status = A_OK;
    SER_PORT_INSTANCE_INFO *pInfo = (SER_PORT_INSTANCE_INFO *)hPort;
    A_BOOL   passIoctlDown = FALSE;
	
    A_ASSERT(pInfo != NULL);
     
    do {
        
        if (DRV_IOCTL_REQUIRES_BUF_PARAM(Code) && (pBuffer == NULL)) {
            status = A_EINVAL;
            A_ASSERT(FALSE);
            break;    
        }
        
        if (!IS_DRV_IOCTL_STD_APP(Code)) {
            passIoctlDown = TRUE;    
            break;
        }
        
            /* if we get here, we are dealing with standard ones, some standard IOCTLs are
             * processed at this layer only, others may be forwarded to the HW layer */
        switch (Code) {
            case SER_PORT_IOCTL_SET_RX_THRESH :
            case SER_PORT_IOCTL_SET_TX_THRESH :
                {
                    A_UINT16            eventMask;
                    SER_PORT_IO_TIMER   *pIOTimer;
                    SER_PORT_HW_RING_BUFFER *pRingBuffer;
                    
                    if (Code == SER_PORT_IOCTL_SET_RX_THRESH) {
                        eventMask = SERIAL_EV_RX_AVAIL_MASK;
                        pIOTimer = &pInfo->RXTimer;
                        pRingBuffer = &pInfo->RXBuffer; 
                    } else {                        
                        eventMask = SERIAL_EV_TX_ROOM_AVAIL_MASK;
                        pIOTimer = &pInfo->TXTimer;
                        pRingBuffer = &pInfo->TXBuffer;
                    }   
                    
                    pIOTimer->ThresholdBytes = A_MIN((int)LengthOrArg,pRingBuffer->Size);
                                                         
                    if (LengthOrArg) {
                        pInfo->EventMask |= eventMask; 
                    } else {
                            /* 0 to disable */
                        pInfo->EventMask &= ~eventMask;
                            /* since threshold is disabled, there is no point to the timer
                             * running any more */
                        SerP_CancelTimer(pIOTimer); 
                    }
                }
                break;
            case SER_PORT_IOCTL_SET_RX_TIMEOUT:
            case SER_PORT_IOCTL_SET_TX_TIMEOUT:
                {
                    SER_PORT_IO_TIMER *pIOTimer;
                                        
                    if (Code == SER_PORT_IOCTL_SET_RX_TIMEOUT) {
                        pIOTimer = &pInfo->RXTimer; 
                    } else {
                        pIOTimer = &pInfo->TXTimer;
                    }       
                        /* first cancel any outstanding timer */
                    SerP_CancelTimer(pIOTimer);
                        /* set new timeout */
                    pIOTimer->TimeOutMS = (int)LengthOrArg;
                    if (pIOTimer->TimeOutMS) {
                         /* schedule timer */
                        SerP_ScheduleTimer(pIOTimer);   
                    } else {
                            /* 0 to disable */
                        pIOTimer->Flags = 0;
                    }
                }       
                break;
            case SER_PORT_IOCTL_FLUSH :
                if (LengthOrArg == SER_PORT_FLUSH_ALL) {
                    SerPFlushBuffer(&pInfo->RXBuffer);
                    SerPFlushBuffer(&pInfo->TXBuffer);
                } else if (LengthOrArg == SER_PORT_FLUSH_RX) {
                    SerPFlushBuffer(&pInfo->RXBuffer);    
                } else if (LengthOrArg == SER_PORT_FLUSH_TX) {
                    SerPFlushBuffer(&pInfo->TXBuffer);    
                }
                    /* HW layer may need to flush hardware buffers */
                passIoctlDown = TRUE;
                break;    
            case SER_PORT_IOCTL_SET_EVENT_MASK :
                pInfo->EventMask |= ((A_UINT16)LengthOrArg) & (~(SER_PORT_AUTO_EVENTS));
                    /* give hardware layer a chance to enable any required interrupts */
                passIoctlDown = TRUE;
                break;
            case SER_PORT_IOCTL_CLEAR_EVENT_MASK :
                pInfo->EventMask &= ~(((A_UINT16)LengthOrArg) & (~(SER_PORT_AUTO_EVENTS)));
                    /* give hardware layer a chance to disable any required interrupts */
                passIoctlDown = TRUE;
                break;
            case SER_PORT_IOCTL_CONFIG_UART_PORT :
                A_ASSERT(LengthOrArg >= sizeof(SER_PORT_UART_CONFIG));
                    /* just pass it down */
                passIoctlDown = TRUE;
                break;
			case SER_PORT_IOCTL_QUERY_TXBUF_SZ:	
				*((A_UINT32 *)pBuffer) = SER_PORT_RING_BUFF_ROOM(&pInfo->TXBuffer);
				break;				
            default:
                status = A_EINVAL;
                A_ASSERT(FALSE);    
                break;            
        }
        
    } while (FALSE);
    
    if (passIoctlDown) {
        if (pInfo->pHWPort->Ioctl != NULL) {
                /* pass the IOCTL to the hw layer */
            status = pInfo->pHWPort->Ioctl(pInfo->pHWPort->pPortContext,
                                           Code,
                                           pBuffer,
                                           LengthOrArg);  
        } else {
            status = A_EINVAL;    
        }   
    }
    
    return status;
}

                            
void serport_hw_api_install(struct serp_apis *pAPIs);
 
void SERP_module_install(struct serp_apis *pAPIs)
{
    pAPIs->_Init = SERPAPI_Init;
    pAPIs->_Open = SERPAPI_Open;
    pAPIs->_Read = SERPAPI_Read;
    pAPIs->_Write = SERPAPI_Write;
    pAPIs->_Ioctl = SERPAPI_Ioctl;
    pAPIs->_Close = SERPAPI_Close;
        /* install HW layer APIs */
    serport_hw_api_install(pAPIs);
    pAPIs->pReserved = &g_pSerPManager;
}

SERP_INTERNAL_APIS g_SerpIndirectionTable = {
    _NotifyIOThresholdEvent,
    _SerPFlushBuffer,
    _SerPPushBytes,
    _SerPPopBytes,
    _CleanupSerialPort
};


