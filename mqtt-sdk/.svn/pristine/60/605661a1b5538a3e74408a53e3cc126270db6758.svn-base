/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_WPS_H__
#define __QCOM_WPS_H__

#include "qcom_sec.h"

#define QCOM_WPS_MAX_KEY_LEN    64

/* Various WPS events that the application can get  */
typedef enum {
    QCOM_WPS_PROFILE_EVENT,
} QCOM_WPS_EVENT_TYPE;

/* WPS Error codes */
typedef enum {
    QCOM_WPS_STATUS_SUCCESS = 0,
    QCOM_WPS_ERROR_INVALID_START_INFO  = 0x1,
    QCOM_WPS_ERROR_MULTIPLE_PBC_SESSIONS,
    QCOM_WPS_ERROR_WALKTIMER_TIMEOUT,
    QCOM_WPS_ERROR_M2D_RCVD,
    QCOM_WPS_ERROR_PWD_AUTH_FAIL,
    QCOM_WPS_ERROR_CANCELLED,
    QCOM_WPS_ERROR_INVALID_PIN,
    QCOM_WPS_STATUS_IDLE,
    QCOM_WPS_STATUS_IN_PROGRESS
} QCOM_WPS_ERROR_CODE;

/* WPS credential information */
typedef struct {
    A_UINT16            ap_channel;
    A_UINT8             ssid[WMI_MAX_SSID_LEN];
    A_UINT8             ssid_len;
    WLAN_AUTH_MODE      auth_type; /* WPS_AUTH_TYPE */
    WLAN_CRYPT_TYPE     encr_type; /* WPS_ENCR_TYPE */
    A_UINT8             key_idx;
    A_UINT8             key[QCOM_WPS_MAX_KEY_LEN+1];
    A_UINT8             key_len;
    A_UINT8             mac_addr[ATH_MAC_LEN];
} qcom_wps_credentials_t;

/* WPS Profile information that gets populated 
 * after 8 way handshake */
typedef struct qcom_wps_profile_info_s
{
    QCOM_WPS_ERROR_CODE     error_code;  /* WPS_ERROR_CODE */
    qcom_wps_credentials_t  credentials ;
} qcom_wps_profile_info_t;

/* Callback structure pointer for WPS events */
typedef struct {
    union {
        qcom_wps_profile_info_t profile_info;
        /* Add more events here if needed */
    } event;
} qcom_wps_event_t;


typedef void (*qcom_wps_event_handler_t)(A_UINT8 device_id, QCOM_WPS_EVENT_TYPE uEventID, qcom_wps_event_t *pEvtBuffer, void *pArg);


#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {
#if defined(AR6002_REV74) 
    /* Deprecated APIs. Use the ones in qcom_sec.c instead */
    A_STATUS (*_qcom_set_ssid_cfg_from_wps)(A_UINT8 device_id, A_CHAR *pssid, int len);
    A_STATUS (*_qcom_set_channel_cfg_from_wps)(A_UINT8 device_id, A_INT32 channel);
    A_STATUS (*_qcom_set_auth_cfg_from_wps)(A_UINT8 device_id, A_INT32 ath_mode);
    A_STATUS (*_qcom_set_encrypt_cfg_from_wps)(A_UINT8 device_id, A_INT32 encry_mode);
    A_STATUS (*_qcom_set_key_index_from_wps)(A_UINT8 device_id, A_UINT32 keyindex);
    A_STATUS (*_qcom_set_pass_cfg_from_wps)(A_UINT8 device_id, char *pass, int pass_len);
    A_STATUS (*_qcom_set_bssid_cfg_from_wps)(A_UINT8 device_id, A_UINT8 *bssid);
#endif /* AR6002_REV74 */
    void (*_qcom_wps_connect)(A_UINT8 device_id);
    void (*_qcom_wps_enable)(A_UINT8 device_id, int enable);
    void (*_qcom_wps_stop)(A_UINT8 device_id);
    void (*_qcom_wps_start)(A_UINT8 device_id, int connect, int mode, char *pin);
    void (*_qcom_wps_register_event_handler)(qcom_wps_event_handler_t handler, void *pArg);
    A_STATUS (*_qcom_wps_set_credentials)(A_UINT8 device_id, qcom_wps_credentials_t *pwps_prof);
    //To be added in rom
    //A_STATUS (*_qcom_wps_config_state_set)(A_UINT8 device_id, int mode);
}QCOM_WPS_API_INDIRECTION_TABLE;

extern  QCOM_WPS_API_INDIRECTION_TABLE qcomWpsApiIndirectionTable;

#define QCOM_WPS_API_FN(fn) qcomWpsApiIndirectionTable.fn


#else
 
#define QCOM_WPS_API_FN(fn) fn
void     _qcom_wps_connect(A_UINT8 device_id);
void     _qcom_wps_enable(A_UINT8 device_id, int enable);
void     _qcom_wps_stop(A_UINT8 device_id);
void     _qcom_wps_start(A_UINT8 device_id, int connect, int mode, char *pin);
void     _qcom_wps_register_event_handler(qcom_wps_event_handler_t handler, void *pArg);
A_STATUS _qcom_wps_set_credentials(A_UINT8 device_id, qcom_wps_credentials_t *pwps_prof);
#endif //Indirection Table


#if defined(AR6002_REV74) 
/* Do not use these deprecated APIs. Use the ones in qcom_sec.c instead */
#define qcom_set_ssid_cfg_from_wps(device_id,pssid, len)

#define qcom_set_channel_cfg_from_wps(device_id,channel)

#define qcom_set_auth_cfg_from_wps(device_id, ath_mode)

#define qcom_set_encrypt_cfg_from_wps(device_id,  encry_mode)

#define qcom_set_key_index_from_wps(device_id, keyindex)

#define qcom_set_pass_cfg_from_wps( device_id, pass, pass_len)

#define qcom_set_bssid_cfg_from_wps(device_id,bssid)
/* End of deprecated APIs */
#endif /* AR6002_REV74 */

#define qcom_wps_connect(device_id)\
    QCOM_WPS_API_FN(_qcom_wps_connect((device_id)))

#define qcom_wps_enable( device_id,  enable)\
    QCOM_WPS_API_FN(_qcom_wps_enable((device_id),(enable)))

#define qcom_wps_stop(device_id)\
    QCOM_WPS_API_FN( _qcom_wps_stop(( device_id)))

#define qcom_wps_start(device_id, connect, mode,pin)\
    QCOM_WPS_API_FN(_qcom_wps_start(( device_id), ( connect), ( mode),(pin)))

A_STATUS _qcom_wps_config_state_set(A_UINT8 device_id, int mode);
#define qcom_wps_config_state_set(devid, mode) \
    _qcom_wps_config_state_set((devid), (mode))

#ifdef AR6002_REV74
void     _qcom_wps_register_event_handler(qcom_wps_event_handler_t handler, void *pArg);
A_STATUS _qcom_wps_set_credentials(A_UINT8 device_id, qcom_wps_credentials_t *pwps_prof);

#define qcom_wps_register_event_handler( callback, callback_arg) \
                (_qcom_wps_register_event_handler( (callback), (callback_arg)))

#define qcom_wps_set_credentials(device_id, pwps_prof)\
                (_qcom_wps_set_credentials((device_id),(pwps_prof)))
#else
#define qcom_wps_register_event_handler( callback, callback_arg) \
    QCOM_WPS_API_FN(_qcom_wps_register_event_handler( (callback), (callback_arg)))

#define qcom_wps_set_credentials(device_id, pwps_prof)\
    QCOM_WPS_API_FN(_qcom_wps_set_credentials((device_id),(pwps_prof)))
#endif /* AR6002_REV74 */

#endif //__QCOM_WPS_H_
