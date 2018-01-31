/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_SEC_H__
#define __QCOM_SEC_H__


typedef enum {
    WLAN_AUTH_NONE  = 0,
    WLAN_AUTH_WPA ,
    WLAN_AUTH_WPA2  ,
    WLAN_AUTH_WPA_PSK ,
    WLAN_AUTH_WPA2_PSK ,
    WLAN_AUTH_WPA_CCKM ,
    WLAN_AUTH_WPA2_CCKM ,
    WLAN_AUTH_WPA2_PSK_SHA256 ,
    WLAN_AUTH_INVALID
} WLAN_AUTH_MODE;

typedef enum {
    WLAN_CRYPT_NONE          = 0,
    WLAN_CRYPT_WEP_CRYPT ,
    WLAN_CRYPT_TKIP_CRYPT ,
    WLAN_CRYPT_AES_CRYPT ,
    WLAN_CRYPT_WAPI_CRYPT ,
    WLAN_CRYPT_BIP_CRYPT ,
    WLAN_CRYPT_KTK_CRYPT,
    WLAN_CRYPT_INVALID
} WLAN_CRYPT_TYPE;


#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct 
{
	A_STATUS (* _qcom_sec_set_wepkey)(A_UINT8 device_id, A_UINT32 keyindex, A_CHAR *pkey);
	A_STATUS (* _qcom_sec_get_wepkey)(A_UINT8 device_id, A_UINT32 keyindex, A_CHAR *pkey);
	A_STATUS (* _qcom_sec_set_wepkey_index)(A_UINT8 device_id, A_UINT32 keyindex);
	A_STATUS (* _qcom_sec_get_wepkey_index)(A_UINT8 device_id, A_UINT32 *pkeyindex);
	A_STATUS (* _qcom_sec_set_wep_mode)(A_UINT8 device_id, A_UINT32 mode);



	A_STATUS (* _qcom_sec_get_wep_mode)(A_UINT8 device_id, A_UINT32 *pmode);
	A_STATUS (* _qcom_sec_set_auth_mode)(A_UINT8 device_id, A_UINT32 mode); //WLAN_AUTH_MODE
	A_STATUS (* _qcom_sec_get_auth_mode)(A_UINT8 device_id, A_UINT32 *pmode);
	A_STATUS (* _qcom_sec_set_encrypt_mode)(A_UINT8 device_id, A_UINT32 mode); //WLAN_CRYPT_TYPE
	A_STATUS (* _qcom_sec_get_encrypt_mode)(A_UINT8 device_id, A_UINT32 *pmode);


	A_STATUS (* _qcom_sec_set_passphrase)(A_UINT8 device_id, A_CHAR *passphrase);
	A_STATUS (* _qcom_sec_get_passphrase)(A_UINT8 device_id, A_CHAR *passphrase);
}QCOM_SEC_API_INDIRECTION_TABLE;

extern QCOM_SEC_API_INDIRECTION_TABLE qcomSecApiIndirectionTable;
#define QCOM_SEC_API_FN(fn) qcomSecApiIndirectionTable.fn


#else

A_STATUS _qcom_sec_set_wepkey(A_UINT8 device_id, A_UINT32 keyindex, A_CHAR *pkey);
A_STATUS _qcom_sec_get_wepkey(A_UINT8 device_id, A_UINT32 keyindex, A_CHAR *pkey);
A_STATUS _qcom_sec_set_wepkey_index(A_UINT8 device_id, A_UINT32 keyindex);
A_STATUS _qcom_sec_get_wepkey_index(A_UINT8 device_id, A_UINT32 *pkeyindex);
A_STATUS _qcom_sec_set_wep_mode(A_UINT8 device_id, A_UINT32 mode);



A_STATUS _qcom_sec_get_wep_mode(A_UINT8 device_id, A_UINT32 *pmode);
A_STATUS _qcom_sec_set_auth_mode(A_UINT8 device_id, A_UINT32 mode); //WLAN_AUTH_MODE
A_STATUS _qcom_sec_get_auth_mode(A_UINT8 device_id, A_UINT32 *pmode);
A_STATUS _qcom_sec_set_encrypt_mode(A_UINT8 device_id, A_UINT32 mode); //WLAN_CRYPT_TYPE
A_STATUS _qcom_sec_get_encrypt_mode(A_UINT8 device_id, A_UINT32 *pmode);


A_STATUS _qcom_sec_set_passphrase(A_UINT8 device_id, A_CHAR *passphrase);
A_STATUS _qcom_sec_get_passphrase(A_UINT8 device_id, A_CHAR *passphrase);

#define QCOM_SEC_API_FN(fn) fn

#endif



#define qcom_sec_set_wepkey(id, key, index) \
	QCOM_SEC_API_FN(_qcom_sec_set_wepkey((id), (key), (index)))
#define qcom_sec_get_wepkey(id, key, index) \
	QCOM_SEC_API_FN(_qcom_sec_get_wepkey((id), (key), (index)))
#define qcom_sec_set_wepkey_index(id, index) \
	QCOM_SEC_API_FN(_qcom_sec_set_wepkey_index((id), (index)))
#define qcom_sec_get_wepkey_index(id, index) \
	QCOM_SEC_API_FN(_qcom_sec_get_wepkey_index((id), (index)))
#define qcom_sec_set_wep_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_set_wep_mode((id), (mode)))




#define qcom_sec_get_wep_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_get_wep_mode((id), (mode)))
#define qcom_sec_set_auth_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_set_auth_mode((id), (mode)))
#define qcom_sec_get_auth_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_get_auth_mode((id), (mode)))
#define qcom_sec_set_encrypt_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_set_encrypt_mode((id), (mode)))
#define qcom_sec_get_encrypt_mode(id, mode) \
	QCOM_SEC_API_FN(_qcom_sec_get_encrypt_mode((id), (mode)))


#define qcom_sec_set_passphrase(id, pass) \
	QCOM_SEC_API_FN(_qcom_sec_set_passphrase((id), (pass)))
#define qcom_sec_get_passphrase(id, pass) \
	QCOM_SEC_API_FN(_qcom_sec_get_passphrase((id), (pass)))
	
#endif
