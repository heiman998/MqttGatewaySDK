/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
#ifndef _QCOM_AP_H_
#define _QCOM_AP_H_

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {
	A_STATUS (* _qcom_ap_start)(A_UINT8 device_id, A_CHAR ssid[]);
	A_STATUS (* _qcom_ap_set_beacon_interval)(A_UINT8 device_id, A_UINT16 beacon_interval);
	A_STATUS (* _qcom_ap_get_beacon_interval)(A_UINT8 device_id, A_UINT16 *pbeacon_interval);
	A_STATUS (* _qcom_ap_set_inact_time)(A_UINT8 device_id, A_UINT16 minutes);
	A_STATUS (* _qcom_ap_hidden_mode_enable)(A_UINT8 device_id, A_BOOL enable);

	A_STATUS (* _qcom_ap_set_max_station_number)(A_UINT8 device_id, A_UINT32 sta_num);
	A_STATUS (* _qcom_ap_set_flag)(A_UINT8 device_id, A_UINT32 flg);
	A_STATUS (* _qcom_start_ap)(A_UINT8 device_id, char *pssid, char *pkey, int ath_mode, int encrypt_mode, int sta_num, unsigned int uiFlags);
	A_STATUS (* _qcom_ap_get_sta_info)(A_UINT8 device_id, A_UINT8 *pnum, A_UINT8 *pmac);

}QCOM_AP_API_INDIRECTION_TABLE;

extern QCOM_AP_API_INDIRECTION_TABLE qcomApApiIndirectionTable;
#define QCOM_AP_API_FN(fn) qcomApApiIndirectionTable.fn

#else

	A_STATUS _qcom_ap_start(A_UINT8 device_id, A_CHAR ssid[]);
	A_STATUS _qcom_ap_set_beacon_interval(A_UINT8 device_id, A_UINT16 beacon_interval);
	A_STATUS _qcom_ap_get_beacon_interval(A_UINT8 device_id, A_UINT16 *pbeacon_interval);
	A_STATUS _qcom_ap_hidden_mode_enable(A_UINT8 device_id, A_BOOL enable);
	A_STATUS _qcom_ap_set_max_station_number(A_UINT8 device_id, A_UINT32 sta_num);
	A_STATUS _qcom_ap_set_flag(A_UINT8 device_id, A_UINT32 flg);
	A_STATUS _qcom_start_ap(A_UINT8 device_id, char *pssid, char *pkey, int ath_mode, int encrypt_mode, int sta_num, unsigned int uiFlags);
	A_STATUS _qcom_ap_set_inact_time(A_UINT8 device_id, A_UINT16 minutes);
    A_STATUS _qcom_ap_get_sta_info(A_UINT8 device_id, A_UINT8 *pnum, A_UINT8 *pmac);

#define QCOM_AP_API_FN(fn) fn

#endif


#define qcom_ap_start(id, ssid) \
	QCOM_AP_API_FN(_qcom_ap_start((id), (ssid)))

#define qcom_ap_set_beacon_interval(id, interval) \
	QCOM_AP_API_FN(_qcom_ap_set_beacon_interval((id), (interval)))

#define qcom_ap_get_beacon_interval(id, interval) \
	QCOM_AP_API_FN(_qcom_ap_get_beacon_interval((id), (interval)))
	
#define qcom_ap_set_inact_time(id, minutes) \
	QCOM_AP_API_FN(_qcom_ap_set_inact_time((id), (minutes)))

#define qcom_ap_hidden_mode_enable(id, en) \
	QCOM_AP_API_FN(_qcom_ap_hidden_mode_enable((id), (en)))

#define qcom_ap_set_max_station_number(id, num) \
	QCOM_AP_API_FN(_qcom_ap_set_max_station_number((id), (num)))

#define qcom_ap_set_flag(id, flag) \
	QCOM_AP_API_FN(_qcom_ap_set_flag((id), (flag)))

#define qcom_start_ap(id, ssid, key, athMode, encryptMode, num, flag) \
	QCOM_AP_API_FN(_qcom_start_ap((id), (ssid), (key), (athMode), (encryptMode), (num), (flag)))
	
#define qcom_ap_get_sta_info(id, pnum, pmac) \
	QCOM_AP_API_FN(_qcom_ap_get_sta_info((id), (pnum), (pmac)))




#endif

