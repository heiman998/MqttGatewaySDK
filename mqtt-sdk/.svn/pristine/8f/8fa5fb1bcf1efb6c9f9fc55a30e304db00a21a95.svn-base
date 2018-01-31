/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc..
 * All Rights Reserved.  
 * Qualcomm Atheros Confidential and Proprietary.
 */

#include <osapi.h>
#include "qcom/wmi_cdr.h"
#include "qcom/wifi_api.h"
#include "threadxdmn_api.h"
#include "qcom/qcom_event.h"
#include "qcom/qcom_system.h"
#include "qcom/qcom_wlan.h"

//extern int qcom_event_dispatch(void *pContext,    /* app supplied context */
//                            A_UINT16 EventID,  /* event ID */
//                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
//                            A_UINT8 * pEvtBuffer,  /* pointer to event data, if needed */
//                            int Length);  /* length of event data, if needed */

extern A_UINT8 gNumOfWlanDevices;
int user_bgrs_htc = 0;

WMI_FILTER_ACTION
qcom_event_filter_cb_forstart(void *pContext,   /* app supplied context */
                      A_UINT16 EventID, /* event ID */
                      A_UINT16 info,    /* the info param passed to WMI_SendEvent */
                      A_UINT8 * pEvtBuffer, /* pointer to event data, if needed */
                      int Length)
{   
    A_UINT8 device_id = info & 0xFF;

    if (WMI_READY_EVENTID == EventID)
    {
        if (device_id < WLAN_NUM_OF_DEVICES) {
            extern int cdr_ready_flag;
            cdr_ready_flag = 1;
        }
    }
    return WMI_EVT_FILTER_DISCARD;  //WMI_EVT_FILTER_ALLOW; //WMI_EVT_FILTER_DISCARD;
}

void
qcom_sta_register_callbacks()
{
#if defined(AR6002_REV72)
    /* Attach a low-level background service */
    extern void _qcom_HTC_Housekeeping(void *args_ptr, A_UINT32 args_len);
    user_bgrs_htc = A_BGR_ATTACH_LOW(_qcom_HTC_Housekeeping, NULL);
#endif
    /* Add a new wmi event filter */
    WMI_RegisterEventFilter(qcom_event_filter_cb_forstart, NULL, TRUE);

    return;
}

/* must be called in thread context */
void
user_pre_init(void)
{
	int i;
#if defined(FPGA)
    /*for hostless*/
    //extern A_BOOT_INFO AR6K_boot_info;
    //extern A_BOOL AR6K_host_present;
    //AR6K_boot_info.interconnect_type = A_HOST_IC_NONE; //A_HOST_IC_NONE;
	/* the event using CDR mechanism will use this variable */
    //AR6K_host_present = 0;
#endif
    qcom_ready();

   	CDR_wmi_dev_init();
    cdr_wmi_event_handler_init(QCOM_EVENT_API_FN(_qcom_event_dispatch));
    /* Add a new wmi event filter */
    WMI_RegisterEventFilter(qcom_event_filter_cb_forstart, NULL, FALSE);
#if 0
    extern WMI_FILTER_ACTION _qcom_host_less_event_filter(void *pContext,  /* app supplied context */
                                                       A_UINT16 EventID,    /* event ID */
                                                       A_UINT16 info,   /* the info param passed to WMI_SendEvent */
                                                       A_UINT8 * pEvtBuffer,    /* pointer to event data, if needed */
                                                       int Length);
    WMI_RegisterEventFilter(_qcom_host_less_event_filter, NULL, TRUE);
#endif
    /* Enable console output */
    A_ENABLE_DEBUG_PRINT();

    /* set default keep alive interval 60s */
    for (i = 0; i < gNumOfWlanDevices; i++) {
        #define KEEP_ALIVE_INTERVAL_DEFAULT 60
        qcom_set_keep_alive(i, KEEP_ALIVE_INTERVAL_DEFAULT);
    }
    return;
}

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Copyright (c) 2011 The FreeBSD Foundation
 * All rights reserved.
 * Portions of this software were developed by David Chisnall
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(A_SIMOS_DEVHOST)
#include "stdarg.h"
#else   // A_SIMOS_DEVHOST
#if defined(__XCC__) && __XCC__ < 8000
#include "stdarg.h"
#define va_list __gnuc_va_list
#else
#define va_list __builtin_va_list
#define va_arg __builtin_va_arg
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_copy __builtin_va_copy
#endif
#endif  // A_SIMOS_DEVHOST

//extern int cmnos_vprintf(void (*putc)(char c), const char *fmt, va_list ap);
extern int cmnos_vprintf(void (*putc)(char **pbs, char *be, char c), char **pbuf_start, char *buf_end, const char *fmt0, va_list ap);

extern void cmnos_write_char(char **pbuf_start, char *buf_end, char c);
void (*_putc)(char **pbuf_start, char *buf_end, char c) = cmnos_write_char;

int app_printf(const char *fmt, ...)
{
   int ret;
   va_list ap;
   va_start(ap, fmt);
   ret = cmnos_vprintf(_putc, NULL, NULL, fmt, ap);
   va_end(ap);
   return (ret);
}
 

