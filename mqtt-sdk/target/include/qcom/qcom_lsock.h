/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
 
#ifndef __QCOM_LSOCK_H__
#define __QCOM_LSOCK_H__
#include "../queue.h"

struct lsock_msg {
    char *p;
    short  cc;
    short  blen; 
    int  taken;
    STAILQ_ENTRY(lsock_msg)   next;
};

typedef struct lsock_ctx_s {
     int fd;
     int tlen;
     STAILQ_HEAD(, lsock_msg)  msg; 
} lsock_ctx_t;

int qcom_lsock_open(int  lsock);
A_STATUS qcom_lsock_write(int  lsock , char *p, short len);
short qcom_lsock_read(int  lsock , char *rbuf, short rlen);
A_STATUS qcom_lsock_close(int  lsock);

#endif

