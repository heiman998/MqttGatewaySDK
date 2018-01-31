/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

#include <qcom/stdint.h>
#include <qcom/socket.h>
#include <qcom/qcom_ssl.h>
#include "cdr.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef PHOST_INDIRECT_ACCESS

typedef struct 
{
	void (* _wmi_socket_cmd)(void *wmip, A_UINT32 cmd_type, void* pData, A_UINT32 length);
	void (* _cdr_sock_common_completed)(void *arg, struct cdr_req_s *cdr_req);
	void (* _BGR_SOCK_CDR_initiate)(void *request, unsigned int req_length);
	int (* _qcom_socket)(int family, int type, int proto);
	int (* _qcom_socket_set_non_blocking)(int s, int enable);

	int (* _qcom_bind)(int s, struct sockaddr *addr, int addrlen);
	int (* _qcom_listen)(int s, int backlog);
	int (* _qcom_accept)(int s, struct sockaddr *addr, int *addrlen);
	int (* _qcom_connect)(int s, struct sockaddr *addr, int addrlen);
	int (* _qcom_setsockopt)(int s, int level, int name, void *arg, int arglen);

	int (* _qcom_getsockopt)(int s, int level, int name, void *arg, int arglen);
	int (* _qcom_send)(int s, char *buf, int len, int flags);
	int (* _qcom_sendto)(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
	void (* _qcom_free_buf)(void* buf);
	int (* _qcom_recv)(int s, char *buf, int len, int flag);

	int (* _qcom_recvfrom)(int s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
	int (* _qcom_socket_close)(int s);
	void (* _qcom_wmi_socket_evt_handler)(void *ev);
    int (* _find_socket_context_from_ssl)(SSL *ssl);
    SSL_CTX* (*_qcom_SSL_ctx_new)(SSL_ROLE_T role, int inbufSize, int outbufSize, int
            reserved);
    int (*_qcom_SSL_ctx_free)(SSL_CTX *ctx);
    SSL_CTX* (*_qcom_SSL_new)(SSL_CTX *ctx);
    int (*_qcom_SSL_addCertApi)(SSL_CTX *ctx, SSL_CERT_TYPE_T type, A_UINT8 *cert, A_UINT32 size);
    int (*_qcom_SSL_setCaList)(SSL_CTX *ctx, A_UINT8 *cert, A_UINT32 size);
    int (*_qcom_SSL_addCert)(SSL_CTX *ctx, A_UINT8 *cert, A_UINT32 size);
#if !defined(AR6002_REV76)    
    int (*_qcom_SSL_reuseCert)(SSL_CTX *ctx, SSL_CTX *refCtx);    
#endif
    int (*_qcom_SSL_storeCert)(A_CHAR *name, A_UINT8 *cert, A_UINT32 size);
    int (*_qcom_SSL_loadCert)(SSL_CTX *ctx, SSL_CERT_TYPE_T type, A_CHAR *name);
    int (*_qcom_SSL_listCert)(SSL_FILE_NAME_LIST *fileNames);
    int (*_qcom_SSL_shutdown)(SSL *ssl);
    int (*_qcom_SSL_configure)(SSL *ssl, SSL_CONFIG *cfg);
#if !defined(AR6002_REV76)
    int (*_qcom_SSL_context_configure)(SSL_CTX *ssl, SSL_CONFIG *cfg);
#endif
    int (*_qcom_SSL_set_fd)(SSL *ssl, A_UINT32 fd);
    int (*_qcom_SSL_accept)(SSL *ssl);
    int (*_qcom_SSL_connect)(SSL *ssl);
    int (*_qcom_SSL_read)(SSL *ssl, void *buf, int num);
    int (*_qcom_SSL_write)(SSL *ssl, const void *buf, int num);
#if !defined(AR6002_REV76)
    int (*_qcom_SSL_set_tm)(tmx_t *ptm);
    A_INT32 (*_qcom_SSL_accept_async)(SSL *ssl);
    A_INT32 (*_qcom_SSL_get_state)(SSL *ssl);    
    SSL_CTX* (*_qcom_DTLS_new)(SSL_CTX *ctx);
    int (*_qcom_DTLS_configure)(SSL_CTX *sslCtx, SSL *ssl, SSL_CONFIG *config,A_UINT16 maxFragLen, A_UINT16 maxFragLenNegDisable);
    int (*_qcom_DTLS_readfrom)(SSL *ssl, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
    int (*_qcom_DTLS_writeto)(SSL *ssl,, char *buf, int len, int flags, struct sockaddr *to, int tolen);
    A_INT32 (*_qcom_DTLS_reset)(SSL *ssl, struct sockaddr *peer_addr);
#endif
}QCOM_SOCKET_API_INDIRECT_TABLE;

extern QCOM_SOCKET_API_INDIRECT_TABLE qcom_socket_api_indirect_table;
#define QCOM_SOCKET_API_FN(fn) qcom_socket_api_indirect_table.fn

#else

void _wmi_socket_cmd(void *wmip, A_UINT32 cmd_type, void* pData, A_UINT32 length);
void _cdr_sock_common_completed(void *arg, struct cdr_req_s *cdr_req);
void _BGR_SOCK_CDR_initiate(void *request, unsigned int req_length);
void _qcom_free_buf(void* buf);
void _qcom_wmi_socket_evt_handler(void *ev);

/**
 * It is a replacement of BSD socket(). The parameters are same.
 *						
 * @return				pointer to created socket cast as a int or SOCKET_ERROR
 */
int _qcom_socket(int family,int type,int proto);
/**
 * It is a replacement of BSD bind(). The parameters are same.
 *						
 * @return				0 if OK, else one of the socket error codes
 */
int _qcom_bind(int s, struct sockaddr * addr,int addrlen);
/**
 * It is a replacement of BSD listen(). The parameters are same.
 *						
 * @return				0 if OK, else one of the socket error codes
 */
int _qcom_listen(int s, int backlog);
/**
 * It is a replacement of BSD accept(). The parameters are same.
 *						
 * @return				pointer to socket cast as a int or SOCKET_ERROR
 */
int _qcom_accept(int s, struct sockaddr *addr, int *addrlen);
/**
 * It is a replacement of BSD connect(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int _qcom_connect(int s, struct sockaddr * addr, int   addrlen);
/**
 * It is a replacement of BSD setsockopt(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int _qcom_setsockopt(int s, int level, int name, void *arg, int arglen);
/**
 * It is a replacement of BSD getsockopt(). The parameters are same.
 *						
 * @return				0 if ok, else one of the socket error codes
 */
int _qcom_getsockopt(int s,int level, int name, void *arg,int arglen);
/**
 * It is a replacement of BSD recv(). The parameters are same.
 *						
 * @return				bytes we receive
 */
int _qcom_recv(int s, char *buf, int len, int flag);
/**
 * It is a replacement of BSD send(). The parameters are same.
 *						
 * @return				bytes we send
 */
int _qcom_send(int s, char *buf, int len, int flags);
/**
 * It is a replacement of BSD sendto(). The parameters are same.
 *						
 * @return				bytes we send
 */
int _qcom_sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
/**
 * It is a replacement of BSD recvfrom(). The parameters are same.
 *						
 * @return				bytes we recv
 */
int _qcom_recvfrom(int s, char *buf, int len,int flags, struct sockaddr *from, int *fromlen);
/**
 * It is a replacement of BSD close(). The parameters are same.
 *						
 * @return				0 on okay or -1 on error
 */
int _qcom_socket_close(int s);
/**
 * By default, socket is set to blocking when created. This API can be 
 * used to enable/disable the blocking option. When the
 * socket is set to non-blocking, recv/recvfrom functions will return
 * immediately
 *
 * @return				0 on sucess or -1 on error
 */
int _qcom_socket_set_non_blocking(int s, int enable);

int _find_socket_context_from_ssl(SSL *ssl);
SSL_CTX* _qcom_SSL_ctx_new(SSL_ROLE_T role, int inbufSize, int outbufSize, int reserved);
int _qcom_SSL_ctx_free(SSL_CTX *ctx);
SSL_CTX* _qcom_SSL_new(SSL_CTX *ctx);
int _qcom_SSL_addCertApi(SSL_CTX *ctx, SSL_CERT_TYPE_T type, A_UINT8 *cert, A_UINT32 size);
int _qcom_SSL_setCaList(SSL_CTX *ctx, A_UINT8 *cert, A_UINT32 size);
int _qcom_SSL_addCert(SSL_CTX *ctx, A_UINT8 *cert, A_UINT32 size);
int _qcom_SSL_storeCert(A_CHAR *name, A_UINT8 *cert, A_UINT32 size);
int _qcom_SSL_loadCert(SSL_CTX *ctx, SSL_CERT_TYPE_T type, A_CHAR *name);
int _qcom_SSL_listCert(SSL_FILE_NAME_LIST *fileNames);
int _qcom_SSL_shutdown(SSL *ssl);
int _qcom_SSL_configure(SSL *ssl, SSL_CONFIG *cfg);
int _qcom_SSL_context_configure(SSL_CTX *ssl, SSL_CONFIG *cfg);
int _qcom_SSL_set_fd(SSL *ssl, A_UINT32 fd);
int _qcom_SSL_accept(SSL *ssl);
int _qcom_SSL_connect(SSL *ssl);
int _qcom_SSL_read(SSL *ssl, void *buf, int num);
int _qcom_SSL_write(SSL *ssl, const void *buf, int num);
int _qcom_SSL_set_tm(tmx_t *ptm);

#define QCOM_SOCKET_API_FN(fn) fn

#endif

#define wmi_socket_cmd(_wmip, _type, _data, _len) \
	QCOM_SOCKET_API_FN(_wmi_socket_cmd((_wmip), (_type), (_data), (_len)))

#define cdr_sock_common_completed(_arg, _cdr) \
	QCOM_SOCKET_API_FN(_cdr_sock_common_completed((_arg), (_cdr)))

#define BGR_SOCK_CDR_initiate(_req, _len) \
	QCOM_SOCKET_API_FN(_BGR_SOCK_CDR_initiate((_req), (_len)))

#define qcom_socket(_fm, _type, _proto) \
	QCOM_SOCKET_API_FN(_qcom_socket((_fm), (_type), (_proto)))

#define qcom_socket_set_non_blocking(_s, _en) \
	QCOM_SOCKET_API_FN(_qcom_socket_set_non_blocking((_s), (_en)))

#define qcom_bind(_s, _addr, _len) \
	QCOM_SOCKET_API_FN(_qcom_bind((_s), (_addr), (_len)))

#define qcom_listen(_s, _bklog) \
	QCOM_SOCKET_API_FN(_qcom_listen((_s), (_bklog)))

#define qcom_accept(_s, _addr, _len) \
	QCOM_SOCKET_API_FN(_qcom_accept((_s), (_addr), (_len)))

#define qcom_connect(_s, _addr, _len) \
	QCOM_SOCKET_API_FN(_qcom_connect((_s), (_addr), (_len)))

#define qcom_setsockopt(_s, _lvl, _name, _arg, _len) \
	QCOM_SOCKET_API_FN(_qcom_setsockopt((_s), (_lvl), (_name), (_arg), (_len)))

#define qcom_getsockopt(_s, _lvl, _name, _arg, _len) \
	QCOM_SOCKET_API_FN(_qcom_getsockopt((_s), (_lvl), (_name), (_arg), (_len)))

#define qcom_send(_s, _buf, _len, _flag) \
	QCOM_SOCKET_API_FN(_qcom_send((_s), (_buf), (_len), (_flag)))

#define qcom_sendto(_s, _buf, _len, _flag, _to, _tolen) \
	QCOM_SOCKET_API_FN(_qcom_sendto((_s), (_buf), (_len), (_flag), (_to), (_tolen)))

#define qcom_free_buf(_buf) \
	QCOM_SOCKET_API_FN(_qcom_free_buf((_buf)))

#define qcom_recv(_s, _buf, _len, _flag) \
	QCOM_SOCKET_API_FN(_qcom_recv((_s), (_buf), (_len), (_flag)))

#define qcom_recvfrom(_s, _buf, _len, _flag, _from, _fromlen)\
	QCOM_SOCKET_API_FN(_qcom_recvfrom((_s), (_buf), (_len), (_flag), (_from), (_fromlen)))

#define qcom_socket_close(_s) \
	QCOM_SOCKET_API_FN(_qcom_socket_close((_s)))

#define qcom_wmi_socket_evt_handler(_evt) \
	QCOM_SOCKET_API_FN(_qcom_wmi_socket_evt_handler((_evt)))

#define find_socket_context_from_ssl(ssl)  \
    QCOM_SOCKET_API_FN(_find_socket_context_from_ssl((ssl)))

#define qcom_SSL_ctx_new(role, inbufSize, outbufSize, reserved)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_ctx_new((role), (inbufSize), (outbufSize), (reserved)))

#define qcom_SSL_ctx_free(ctx)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_ctx_free((ctx)))

#define qcom_SSL_new(ctx)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_new((ctx)))

#define qcom_SSL_addCertApi(ctx, type, cert, size)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_addCertApi((ctx), (type), (cert), (size)))

#define qcom_SSL_setCaList(ctx, cert, size)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_setCaList((ctx), (cert), (size)))

#define qcom_SSL_addCert(ctx, cert, size)  \
QCOM_SOCKET_API_FN(_qcom_SSL_addCert((ctx), (cert), (size)))

#if !defined(AR6002_REV76)
#define qcom_SSL_reuseCert(ctx, refCtx)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_reuseCert((ctx), (refCtx)))
#else
A_INT32 _qcom_SSL_reuseCert(SSL_CTX *ctx, SSL_CTX *refCtx);
#define qcom_SSL_reuseCert(ctx, refCtx)  \
    _qcom_SSL_reuseCert((ctx), (refCtx))
#endif

#define qcom_SSL_storeCert(name, cert, size)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_storeCert((name), (cert), (size)))

#define qcom_SSL_loadCert(ctx, type, name)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_loadCert((ctx), (type), (name)))
        
#define qcom_SSL_listCert(fileNames)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_listCert((fileNames)))

#define qcom_SSL_shutdown(ssl)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_shutdown((ssl)))

#define qcom_SSL_configure(ssl, cfg)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_configure((ssl), (cfg)))

#if !defined(AR6002_REV76)
#define qcom_SSL_accept_async(ssl)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_accept_async((ssl)));

#define qcom_SSL_get_state(ssl)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_get_state((ssl)));

#define qcom_SSL_context_configure(ssl, cfg) \
    QCOM_SOCKET_API_FN(_qcom_SSL_context_configure((ssl), (cfg)))

#define qcom_DTLS_reset(ssl, peer_addr)  \
        QCOM_SOCKET_API_FN(_qcom_DTLS_reset((ssl),(peer_addr)));

#define qcom_DTLS_configure(sslCtx, ssl, config, maxFragLen, maxFragLenNegDisable) \
     QCOM_SOCKET_API_FN(_qcom_DTLS_configure((sslCtx),(ssl), (config),(maxFragLen), (maxFragLenNegDisable)))

#define qcom_DTLS_new(ctx) \
     QCOM_SOCKET_API_FN(_qcom_DTLS_new(ctx))

#define qcom_DTLS_readfrom(ssl, buf, len, flags, from, fromlen) \
     QCOM_SOCKET_API_FN(_qcom_DTLS_readfrom((ssl), (buf), (len), (flags), (from), (fromlen)))

#define qcom_DTLS_writeto(ssl, buf, len, flags, to, tolen) \
     QCOM_SOCKET_API_FN(_qcom_DTLS_writeto((ssl), (buf), (len), (flags), (to), (tolen)))

#else
A_INT32 _qcom_SSL_accept_async(SSL *ssl);
#define qcom_SSL_accept_async(ssl)  \
    _qcom_SSL_accept_async((ssl));

A_INT32 _qcom_SSL_get_state(SSL *ssl);
#define qcom_SSL_get_state(ssl)  \
    _qcom_SSL_get_state((ssl));

A_INT32 _qcom_DTLS_reset(SSL *ssl, struct sockaddr *peer_addr);
#define qcom_DTLS_reset(ssl, peer_addr)  \
    _qcom_DTLS_reset((ssl), (peer_addr));


int _qcom_SSL_context_configure(SSL_CTX *ssl, SSL_CONFIG *cfg);
#define qcom_SSL_context_configure(ssl, cfg) \
    _qcom_SSL_context_configure((ssl), (cfg))

int _qcom_DTLS_configure(SSL_CTX *sslCtx, SSL *ssl, SSL_CONFIG *config,A_UINT16 maxFragLen, A_UINT16 maxFragLenNegDisable);
#define qcom_DTLS_configure(sslCtx, ssl, config, maxFragLen, maxFragLenNegDisable) \
    _qcom_DTLS_configure((sslCtx), (ssl), (config),(maxFragLen), (maxFragLenNegDisable))

SSL_CTX* _qcom_DTLS_new(SSL_CTX *ctx);
#define qcom_DTLS_new(ctx) \
    _qcom_DTLS_new((ctx))

int _qcom_DTLS_readfrom(SSL *ssl, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
#define qcom_DTLS_readfrom(ssl, buf, len, flags, from, fromlen) \
     _qcom_DTLS_readfrom((ssl), (buf), (len), (flags), (from), (fromlen))

int _qcom_DTLS_writeto(SSL *ssl, char *buf, int len, int flags, struct sockaddr *to, int tolen);
#define qcom_DTLS_writeto(ssl, buf, len, flags, to, tolen) \
     _qcom_DTLS_writeto((ssl), (buf), (len), (flags), (to), (tolen))

#endif

#define qcom_SSL_set_fd(ssl, fd)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_set_fd((ssl), (fd)))

#define qcom_SSL_accept(ssl)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_accept((ssl)))

#define qcom_SSL_connect(ssl)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_connect((ssl)))

#define qcom_SSL_read(ssl, buf, num)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_read((ssl), (buf), (num)))

#define qcom_SSL_write(ssl, buf, num)  \
    QCOM_SOCKET_API_FN(_qcom_SSL_write((ssl), (buf), (num)))

#if !defined(AR6002_REV76)
#define qcom_SSL_set_tm(ptm) \
    QCOM_SOCKET_API_FN(_qcom_SSL_set_tm((ptm)))
#else
A_INT32 _qcom_SSL_set_tm(tmx_t *ptm);
#define qcom_SSL_set_tm(ptm) \
    _qcom_SSL_set_tm((ptm))
#endif

#ifdef __cplusplus
}
#endif

#endif

