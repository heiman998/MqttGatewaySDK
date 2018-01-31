/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__WPS_PRIV_H__
#define __QCOM__WPS_PRIV_H__

#ifdef LIBCUST_INDIRECT_ACCESS

/* Function under AR6002_REV74 should go to qcom_wps.h as these APIs should be exported to Applications */
typedef struct  {
    void (*_qcom_wps_init)(A_UINT8 device_id);
    void (*_qcom_wps_cancel)(A_UINT8 device_id);
    void (*_qcom_wps_priv_start)(A_UINT8 device_id, int mode, char *pin);
    void (*_qcom_wps_start_without_scan)(int dev_id, int role, int mode, int channel, int connect, int time_out, char *pssid, char *pin, unsigned char *pmac);
#if defined(AR6002_REV74) 
    A_STATUS (*_qcom_wps_credentials_set)(A_UINT8 device_id, WMI_WPS_PROFILE_EVENT * pwps_prof);
    void (*_qcom_wps_process)(A_UINT8 device_id, void *pEvtBuffer);
#endif /* AR6002_REV74 */
    A_STATUS (*_wmi_wps_event_handler)(A_UINT8 ucDeviceID, A_UINT16 uEventID, A_UINT8 * pEvtBuffer, A_UINT16 uLength);
    //To be added in rom
    //A_STATUS (*_qcom_wps_priv_config_state_set)(A_UINT8 device_id, int mode);
}QCOM_WPS_PRIV_API_INDIRECTION_TABLE;
extern QCOM_WPS_PRIV_API_INDIRECTION_TABLE qcomWpsPrivApiIndirectionTable;

#define QCOM_WPS_PRIV_API_FN(fn) qcomWpsPrivApiIndirectionTable.fn

#else

#define QCOM_WPS_PRIV_API_FN(fn) fn
void _qcom_wps_init(A_UINT8 device_id);
void _qcom_wps_cancel(A_UINT8 device_id);
void _qcom_wps_priv_start(A_UINT8 device_id, int mode, char *pin);
void _qcom_wps_start_without_scan(int dev_id, int role, int mode, int channel, int connect, int time_out, char *pssid, char *pin, unsigned char *pmac);
#if defined(AR6002_REV74)
A_STATUS _qcom_wps_credentials_set(A_UINT8 device_id, WMI_WPS_PROFILE_EVENT * pwps_prof);
void _qcom_wps_process(A_UINT8 device_id, void *pEvtBuffer);
#endif /* AR6002_REV74 */
A_STATUS _wmi_wps_event_handler(A_UINT8 ucDeviceID, A_UINT16 uEventID, A_UINT8 * pEvtBuffer, A_UINT16 uLength);
#endif// DISABLE_FUNCTION_INDIRECTION

#define qcom_wps_init(device_id)\
    QCOM_WPS_PRIV_API_FN(_qcom_wps_init((device_id)))

#define qcom_wps_cancel(device_id)\
    QCOM_WPS_PRIV_API_FN(_qcom_wps_cancel((device_id)))

#define qcom_wps_priv_start(device_id,  mode, pin)\
    QCOM_WPS_PRIV_API_FN(_qcom_wps_priv_start(( device_id),(  mode),( pin)))

A_STATUS _qcom_wps_priv_config_state_set(A_UINT8 device_id, int mode);
#define qcom_wps_priv_config_state_set(devid, mode) \
    _qcom_wps_priv_config_state_set((devid), (mode))

#define qcom_wps_start_without_scan( dev_id,  role,  mode, channel, connect,  time_out,pssid, pin,pmac)\
QCOM_WPS_PRIV_API_FN(_qcom_wps_start_without_scan(( dev_id),( role),( mode),(  channel),( connect),( time_out),(pssid),(pin),(pmac)))

#if defined(AR6002_REV74) 
/* Below functions are deprecated and should not be used */
#define qcom_wps_credentials_set(device_id, pwps_prof)
#define qcom_wps_process(device_id, pEvtBuffer)

A_STATUS _wmi_wps_event_handler(A_UINT8 ucDeviceID, A_UINT16 uEventID, A_UINT8 * pEvtBuffer, A_UINT16 uLength);
#define wmi_wps_event_handler(device_id, event_id, pEvtBuffer, length)\
        _wmi_wps_event_handler((device_id),(event_id), (pEvtBuffer), (length))
#else
#define wmi_wps_event_handler(device_id, event_id, pEvtBuffer, length)\
    QCOM_WPS_PRIV_API_FN(_wmi_wps_event_handler((device_id),(event_id), (pEvtBuffer), (length)))
#endif  /* AR6002_REV74 */

/********internal define***********/
#define UNPACK_U16(x) (*(unsigned char *)(x)) | (*(((unsigned char *)(x)) + 1) << 8)


#endif//QCOM_WPS_PRIV_H
