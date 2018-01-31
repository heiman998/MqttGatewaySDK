/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __CDR_SOCK_INTERNAL_H__
#define __CDR_SOCK_INTERNAL_H__

#define MAX_TX_PACKET_SIZE    1576

typedef struct {
    void *event;
    int completed;
    void (*set_complete_flag)(struct cdr_req_s *cdr_req);
} cdr_sock_completed_t;

typedef struct {
    unsigned int cmd;
    int family;
    int type;
    int proto;
    int ret;
} open_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int addrlen;
    int ret;
} bind_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int backlog;
    int ret;
} listen_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int *addrlen;
    int ret;
} accept_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    struct sockaddr *addr;
    int addrlen;
    int ret;
    int timeout;
}connect_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int level;
    int name;
    void *arg;
    int arglen;
    int ret;
} setsockopt_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int level;
    int name;
    void *arg;
    int arglen;
    int ret;
} getsockopt_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    void *txbuf;
    int len;
    int flags;
    int ret;
} send_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    void *txbuf;	
    int len;
    int flags;
    struct sockaddr *to;
    int tolen;
    int ret;
} sendto_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    int len;
    int flag;
    int ret;
} recv_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    char *buf;
    int len;
    int flags;
    struct sockaddr *from;
    int *fromlen;
    int ret;
} recvfrom_priv_t;

typedef struct {
    unsigned int cmd;
    int s;
    int ret;
} close_priv_t;

typedef struct {
    unsigned int cmd;
    void* buf;
} free_priv_t;

typedef struct{
	int cdr_async_index;
	int cdr_async_max;
	free_priv_t *free_priv_head;
	struct cdr_req_s *cdr_req_head;
}free_buf_cdr_async_t;


#endif
