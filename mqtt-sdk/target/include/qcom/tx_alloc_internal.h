/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __TX_ALLOC_INTERNAL_H__
#define __TX_ALLOC_INTERNAL_H__

//#include "os/timer_api.h"
typedef unsigned int _qcom_A_TIMER_SPACE[6];
typedef _qcom_A_TIMER_SPACE _qcom_A_timer_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tx_alloc_cb{
    unsigned int cmd;
    struct tx_alloc_cb *next;
    struct tx_alloc_cb *prev;	
    void* buf;
    struct cdr_req_s* cdr_req;
    _qcom_A_timer_t timer;
}tx_alloc_cb_t;

typedef struct {
    unsigned int cmd;
    void* bp_info ;
} bp_get_priv_t;
void cdr_txbuf_alloc_handle(struct cdr_req_s* cdr_req);
void tx_alloc_wakeup(int buget);
void cdr_bpool_info_get_handle(struct cdr_req_s *cdr_req);
#ifdef __cplusplus
}
#endif

#endif      /* __TX_ALLOC_INTERNAL_H__*/

