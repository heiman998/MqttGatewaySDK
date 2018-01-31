/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_STA_H_
#define _QCOM_STA_H_



#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct
{
A_STATUS (* _qcom_sta_connect_with_scan)(A_UINT8 device_id, A_CHAR ssid[]);
A_STATUS (* _qcom_sta_connect_without_scan)(A_UINT8 device_id, A_CHAR ssid[]);
A_STATUS (* _qcom_sta_reconnect_start)(A_UINT8 device_id, A_UINT32 seconds);
A_STATUS (* _qcom_sta_reconnect_stop)(A_UINT8 device_id);
A_STATUS (* _qcom_sta_get_rssi)(A_UINT8 device_id, A_UINT8 *prssi);

A_STATUS (* _qcom_sta_set_listen_time)(A_UINT8 device_id, A_UINT32 listentime);
A_STATUS (* _qcom_sta_get_listen_time)(A_UINT8 device_id, A_UINT32 *plistentime);
A_STATUS (* _qcom_sta_connect_event_wait)(A_UINT8 device_id, A_UINT32 *pstate);
A_STATUS (* _qcom_sta_connect_event_wakeup)(A_UINT8 device_id, A_UINT32 state);
A_STATUS (* _qcom_connect_ap)(A_UINT8 device_id, A_CHAR ssid[], A_CHAR pass[], A_UINT8 bssid[], 
                          A_UINT32 auth_dot11_encrypt_mode, A_UINT32 channel, A_UINT32 index_key);
                          
A_STATUS (* _qcom_connect_ap_scaned)(A_UINT8 device_id);
void (* _qcom_sta_wlan_reconnect_timer_handler)(unsigned int alarm, void *data);
A_STATUS (*_qcom_commit)(A_UINT8 device_id);

}QCOM_STA_API_INDIRECTION_TABLE;


extern QCOM_STA_API_INDIRECTION_TABLE qcomStaApiIndirectionTable;
#define QCOM_STA_API_FN(fn) qcomStaApiIndirectionTable.fn

#else


A_STATUS _qcom_sta_connect_with_scan(A_UINT8 device_id, A_CHAR ssid[]);
A_STATUS _qcom_sta_connect_without_scan(A_UINT8 device_id, A_CHAR ssid[]);
A_STATUS _qcom_sta_reconnect_start(A_UINT8 device_id, A_UINT32 seconds);
A_STATUS _qcom_sta_reconnect_stop(A_UINT8 device_id);
A_STATUS _qcom_sta_get_rssi(A_UINT8 device_id, A_UINT8 *prssi);

A_STATUS _qcom_sta_set_listen_time(A_UINT8 device_id, A_UINT32 listentime);
A_STATUS _qcom_sta_get_listen_time(A_UINT8 device_id, A_UINT32 *plistentime);
A_STATUS _qcom_sta_connect_event_wait(A_UINT8 device_id, A_UINT32 *pstate);
A_STATUS _qcom_sta_connect_event_wakeup(A_UINT8 device_id, A_UINT32 state);
A_STATUS _qcom_connect_ap(A_UINT8 device_id, A_CHAR ssid[], A_CHAR pass[], A_UINT8 bssid[], 
                          A_UINT32 auth_dot11_encrypt_mode, A_UINT32 channel, A_UINT32 index_key);
                          
A_STATUS _qcom_connect_ap_scaned(A_UINT8 device_id);
void _qcom_sta_wlan_reconnect_timer_handler(unsigned int alarm, void *data);

#define QCOM_STA_API_FN(fn) fn

#endif

A_STATUS _qcom_commit(A_UINT8 device_id);

#define qcom_sta_connect_with_scan(id, ssid) \
	QCOM_STA_API_FN(_qcom_sta_connect_with_scan((id), (ssid)))
#define qcom_sta_connect_without_scan(id, ssid) \
	QCOM_STA_API_FN(_qcom_sta_connect_without_scan((id), (ssid)))
#define qcom_sta_reconnect_start(id, sec) \
	QCOM_STA_API_FN(_qcom_sta_reconnect_start((id), (sec)))
#define qcom_sta_reconnect_stop(id) \
	QCOM_STA_API_FN(_qcom_sta_reconnect_stop((id)))
#define qcom_sta_get_rssi(id, rssi) \
	QCOM_STA_API_FN(_qcom_sta_get_rssi((id), (rssi)))

/*this is needed by customer*/
#define qcom_sta_connect_legacy(id, ssid) \
	QCOM_STA_API_FN(_qcom_sta_connect_with_scan((id), (ssid)))

#define qcom_sta_set_listen_time(id, time) \
	QCOM_STA_API_FN(_qcom_sta_set_listen_time((id), (time)))
#define qcom_sta_get_listen_time(id, time) \
	QCOM_STA_API_FN(_qcom_sta_get_listen_time((id), (time)))
#define qcom_sta_connect_event_wait(id, state) \
	QCOM_STA_API_FN(_qcom_sta_connect_event_wait((id), (state)))
#define qcom_sta_connect_event_wakeup(id, state) \
	QCOM_STA_API_FN(_qcom_sta_connect_event_wakeup((id), (state)))
#define qcom_connect_ap(id, rssi, pass, bssid, mode, channel, key) \
	QCOM_STA_API_FN(_qcom_connect_ap((id), (rssi), (pass), (bssid), (mode), (channel), (key)))


#define qcom_connect_ap_scaned(id) \
	QCOM_STA_API_FN(_qcom_connect_ap_scaned((id)))
#define qcom_sta_wlan_reconnect_timer_handler(alarm, data) \
	QCOM_STA_API_FN(_qcom_sta_wlan_reconnect_timer_handler((alarm), (data)))

#if defined(AR6002_REV74)
#define qcom_commit(device_id)  \
    _qcom_commit(device_id)
#else
#define qcom_commit(device_id)  \
    QCOM_STA_API_FN(_qcom_commit(device_id))
#endif /*AR6002_REV74*/

#endif
