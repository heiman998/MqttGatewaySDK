/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 */

#ifndef __QCOM__SECURITY_H__
#define __QCOM__SECURITY_H__

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct
{
	char *
	(* _MDString)(char *string);
	char *
	(* _secur_md5_string)(char *input);
	void
	(* _qcom_sec_md5_init)();
	void
	(* _qcom_sec_md5_update)(unsigned char *data, int len);
	void
	(* _qcom_sec_md5_final)(char md5_val[]);


}QCOM_SECURITY_API_INDIRECTION_TABLE;


extern QCOM_SECURITY_API_INDIRECTION_TABLE qcomSecurityApiIndirectionTable;
#define QCOM_SECURITY_API_FN(fn) qcomSecurityApiIndirectionTable.fn

#else


char *
_MDString(char *string);
char *
_secur_md5_string(char *input);
void
_qcom_sec_md5_init();
void
_qcom_sec_md5_update(unsigned char *data, int len);
void
_qcom_sec_md5_final(char md5_val[]);


#define QCOM_SECURITY_API_FN(fn) fn


#endif
void *_qcom_aes_encrypt_init(const unsigned char *key, int len);
void _qcom_aes_encrypt(void *ctx, const unsigned char *plain, unsigned char *crypt);
void _qcom_aes_encrypt_deinit(void *ctx);
void *_qcom_aes_decrypt_init(const unsigned char *key, int len);
void _qcom_aes_decrypt(void *ctx, const unsigned char *crypt, unsigned char *plain);
void _qcom_aes_decrypt_deinit(void *ctx);

#define MDString(str) \
	QCOM_SECURITY_API_FN(_MDString(str))
#define secur_md5_string(str) \
	QCOM_SECURITY_API_FN(_secur_md5_string(str))
#define qcom_sec_md5_init() \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_init())
#define qcom_sec_md5_update(str, len) \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_update((str), (len)))
#define qcom_sec_md5_final(str) \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_final(str))

#define qcom_aes_encrypt_init(key, len) \
		_qcom_aes_encrypt_init((key), (len))
		
#define qcom_aes_encrypt(ctx, plain, crypt) \
		_qcom_aes_encrypt((ctx), (plain), (crypt))
		
#define qcom_aes_encrypt_deinit(ctx) \
		_qcom_aes_encrypt_deinit((ctx))
	
#define qcom_aes_decrypt_init(key, len) \
			_qcom_aes_decrypt_init((key), (len))
			
#define qcom_aes_decrypt(ctx, crypt, plain) \
			_qcom_aes_decrypt((ctx), (crypt), (plain))
			
#define qcom_aes_decrypt_deinit(ctx) \
			_qcom_aes_decrypt_deinit((ctx))


#endif

