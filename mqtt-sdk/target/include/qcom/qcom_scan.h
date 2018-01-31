/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_SCAN_H_
#define _QCOM_SCAN_H_

typedef struct _qcom_scan_info
{
    unsigned char channel;
    unsigned char ssid_len;
    unsigned char rssi;
    unsigned char security_enabled;
    unsigned short beacon_period;
    unsigned char preamble;
    unsigned char bss_type;
    unsigned char bssid[6];
    unsigned char ssid[32];
    unsigned char rsn_cipher;
    unsigned char rsn_auth;
    unsigned char wpa_cipher;
    unsigned char wpa_auth;
    unsigned short caps;
    unsigned char wep_support;
    unsigned char reserved; //keeps struct on 4-byte boundary
} QCOM_BSS_SCAN_INFO, * QCOM_BSS_SCAN_INFO_PTR;

typedef struct _qcom_scan_params
{
   A_UINT16 fgStartPeriod; 
   A_UINT16 fgEndPeriod;
   A_UINT16 bgPeriod; 
   A_UINT16 maxActChDwellTimeInMs;
   A_UINT16 pasChDwellTimeInMs;
   A_UINT8  shortScanRatio;
   A_UINT8  scanCtrlFlags;
   A_UINT16 minActChDwellTimeInMs;
   A_UINT16 maxActScanPerSsid;
   A_UINT32 maxDfsChActTimeInMs;
} qcom_scan_params_t;

#define QCOM_START_SCAN_PARAMS_CHANNEL_LIST_MAX 12

typedef struct _qcom_start_scan_params
{
   A_BOOL   forceFgScan;
   A_UINT32 homeDwellTimeInMs;
   A_UINT32 forceScanIntervalInMs;
   A_UINT8  scanType;
   A_UINT8  numChannels;
   A_UINT16 channelList[QCOM_START_SCAN_PARAMS_CHANNEL_LIST_MAX];
} qcom_start_scan_params_t;

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct
{

A_STATUS (* _qcom_scan_set_mode)(A_UINT8 device_id, A_UINT32 mode);
A_STATUS (* _qcom_scan_get_mode)(A_UINT8 device_id, A_UINT32 *pmode);
A_STATUS (* _qcom_scan_params_set)(A_UINT8 device_id, qcom_scan_params_t *pParams);
    
A_STATUS (* _qcom_scan_bss_start)(A_UINT8 device_id, A_CHAR *ssid, qcom_start_scan_params_t  *pParams);
A_STATUS (* _qcom_scan_all_bss_start)(A_UINT8 device_id, qcom_start_scan_params_t  *pParams);
A_STATUS (* _qcom_scan_get_bss_number)(A_UINT8 device_id, A_UINT32 *pnumber);


A_STATUS (* _qcom_scan_get_bss_info)(A_UINT8 device_id, A_UINT32 id, QCOM_BSS_SCAN_INFO *pbuf);
A_UINT32 (* _qcom_util_freq2ieee)(A_UINT16 freq);
void (* _qcom_bss_packet_print)(A_UINT8 * buf, int length);
A_UINT8 (* _qcom_wpa_auth_parse)(A_UINT8 * sel);
A_UINT8 (* _qcom_wpa_cipher_parse)(A_UINT8 * sel, A_UINT8 * keylen);

int (* _qcom_rsn_cipher_parse)(A_UINT8 * sel, A_UINT8 * keylen);
A_UINT8 (* _qcom_rsn_auth_parse)(A_UINT8 * sel);
void (* _qcom_security_ie_parse)(A_UINT8 * pie, A_UINT8 ie_len, A_UINT8 * pResult, A_UINT8 ie_type);

A_STATUS (* _qcom_parse_bss_info)(void *pCxt, QCOM_BSS_SCAN_INFO *pBssInfo, void * bih, A_INT32 len);

void (* _qcom_record_bss_info)(A_UINT8 device_id, A_UINT8 * pBuf, int len);

void (* _qcom_bss_scan_result_show)(A_UINT8 device_id);
QCOM_BSS_SCAN_INFO * (* _qcom_get_bss_entry_by_ssid)(A_UINT8 device_id, char ssid[]);
A_STATUS (*_qcom_get_scan)(A_UINT8 device_id, QCOM_BSS_SCAN_INFO** buf, A_UINT16* numResults);
A_STATUS (*_qcom_set_scan)(A_UINT8 device_id, qcom_start_scan_params_t  *pParams);
}QCOM_SCAN_API_INDIRECTION_TABLE;


extern QCOM_SCAN_API_INDIRECTION_TABLE qcomScanApiIndirectionTable;
#define QCOM_SCAN_API_FN(fn) qcomScanApiIndirectionTable.fn

#else
A_STATUS _qcom_scan_set_mode(A_UINT8 device_id, A_UINT32 mode);
A_STATUS _qcom_scan_get_mode(A_UINT8 device_id, A_UINT32 *pmode);

A_STATUS  _qcom_scan_params_set(A_UINT8 device_id,qcom_scan_params_t *pParams);

A_STATUS _qcom_scan_bss_start(A_UINT8 device_id, A_CHAR *ssid, qcom_start_scan_params_t  *pParams);
A_STATUS _qcom_scan_all_bss_start(A_UINT8 device_id, qcom_start_scan_params_t  *pParams);
A_STATUS _qcom_scan_get_bss_number(A_UINT8 device_id, A_UINT32 *pnumber);


A_STATUS _qcom_scan_get_bss_info(A_UINT8 device_id, A_UINT32 id, QCOM_BSS_SCAN_INFO *pbuf);
/*static A_UINT32 _qcom_util_freq2ieee(A_UINT16 freq);*/
void _qcom_bss_packet_print(A_UINT8 * buf, int length);
#if 0
static A_UINT8 _qcom_wpa_auth_parse(A_UINT8 * sel);
static A_UINT8 _qcom_wpa_cipher_parse(A_UINT8 * sel, A_UINT8 * keylen);

static CRYPTO_TYPE _qcom_rsn_cipher_parse(A_UINT8 * sel, A_UINT8 * keylen);
static A_UINT8 _qcom_rsn_auth_parse(A_UINT8 * sel);
static void _qcom_security_ie_parse(A_UINT8 * pie, A_UINT8 ie_len, A_UINT8 * pResult, A_UINT8 ie_type);
static A_STATUS _qcom_parse_bss_info(void *pCxt, QCOM_BSS_SCAN_INFO *pBssInfo, WMI_BSS_INFO_HDR2 * bih, A_INT32 len);
#endif
void _qcom_record_bss_info(A_UINT8 device_id, A_UINT8 * pBuf, int len);

void _qcom_bss_scan_result_show(A_UINT8 device_id);
QCOM_BSS_SCAN_INFO *_qcom_get_bss_entry_by_ssid(A_UINT8 device_id, char ssid[]);

#define QCOM_SCAN_API_FN(fn) fn


#endif

A_STATUS _qcom_get_scan(A_UINT8 device_id, QCOM_BSS_SCAN_INFO** buf, A_UINT16* numResults);
A_STATUS _qcom_set_scan(A_UINT8 device_id, qcom_start_scan_params_t  *pParams);

#define qcom_scan_set_mode(id, mode) \
	QCOM_SCAN_API_FN(_qcom_scan_set_mode((id), (mode)))
#define qcom_scan_get_mode(id, mode) \
	QCOM_SCAN_API_FN(_qcom_scan_get_mode((id), (mode)))
#define qcom_scan_params_set(device_id, pParams)\
    QCOM_SCAN_API_FN(_qcom_scan_params_set((device_id), (pParams)))

#define qcom_scan_bss_start(id, ssid, pScanParams) \
	QCOM_SCAN_API_FN(_qcom_scan_bss_start((id), (ssid), (pScanParams)))
#define qcom_scan_all_bss_start(id, pScanParams) \
	QCOM_SCAN_API_FN(_qcom_scan_all_bss_start((id), (pScanParams)))
#define qcom_scan_get_bss_number(id, number) \
	QCOM_SCAN_API_FN(_qcom_scan_get_bss_number((id), (number)))



#define qcom_scan_get_bss_info(devid, id, buffer) \
	QCOM_SCAN_API_FN(_qcom_scan_get_bss_info((devid), (id), (buffer)))
#define qcom_util_freq2ieee(freq) \
	QCOM_SCAN_API_FN(_qcom_util_freq2ieee((freq)))
#define qcom_bss_packet_print(buf, len) \
	QCOM_SCAN_API_FN(_qcom_bss_packet_print((buf), (len)))
#define qcom_wpa_auth_parse(sel) \
	QCOM_SCAN_API_FN(_qcom_wpa_auth_parse((sel)))
#define qcom_wpa_cipher_parse(sel, key) \
	QCOM_SCAN_API_FN(_qcom_wpa_cipher_parse((sel), (key)))


#define qcom_rsn_cipher_parse(sel, key) \
	QCOM_SCAN_API_FN(_qcom_rsn_cipher_parse((sel), (key)))
#define qcom_rsn_auth_parse(sel) \
	QCOM_SCAN_API_FN(_qcom_rsn_auth_parse(sel))
#define qcom_security_ie_parse(ie, len, ret, type) \
	QCOM_SCAN_API_FN(_qcom_security_ie_parse((ie), (len), (ret), (type)))
#define qcom_parse_bss_info(ctxt, bssinfo, bih, len) \
	QCOM_SCAN_API_FN(_qcom_parse_bss_info((ctxt), (bssinfo), (bih), (len)))
#define qcom_record_bss_info(dev, buf, len) \
	QCOM_SCAN_API_FN(_qcom_record_bss_info((dev), (buf), (len)))

#define qcom_bss_scan_result_show(dev) \
	QCOM_SCAN_API_FN(_qcom_bss_scan_result_show((dev)))
#define qcom_get_bss_entry_by_ssid(id, ssid) \
	QCOM_SCAN_API_FN(_qcom_get_bss_entry_by_ssid((id), (ssid)))

#if defined(AR6002_REV74) 
#define qcom_set_scan(device_id, scanParams) \
     _qcom_set_scan((device_id), (scanParams))

#define qcom_get_scan(device_id, buf, numResults) \
     _qcom_get_scan((device_id), (buf), (numResults))
     
#else /*AR6002_REV74*/

#define qcom_set_scan(device_id, scanParams) \
     QCOM_SCAN_API_FN(_qcom_set_scan((device_id), (scanParams)))

#define qcom_get_scan(device_id, buf, numResults) \
     QCOM_SCAN_API_FN(_qcom_get_scan((device_id), (buf), (numResults)))

#endif /*AR6002_REV74*/

#include "qcom/qcom_legacy_api.h"

#endif

