/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __TX_ALLOC_API_H__
#define __TX_ALLOC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bpool_info_s {
    unsigned char htc_cnt;
    unsigned char  htc_reap;
    unsigned char  mac_cnt;
    unsigned char  mac_reap;
    unsigned char  fw_cnt;
    unsigned char  fw_reap;
    unsigned char  free_cnt;
    unsigned char  tx_buf_queued;
    unsigned char  rx_buf_queued;
    unsigned char  resv[3];
    unsigned int  htc_get;
    unsigned int  htc_put;
} bpool_info_t;



#ifdef PHOST_INDIRECT_ACCESS

typedef struct {
	void * (* _cdr_tx_pkt_alloc)(int len);
	void (* _cdr_bpool_info_get)(bpool_info_t *bp_info);
}TX_ALLOC_API_INDIRECT_TABLE;

extern TX_ALLOC_API_INDIRECT_TABLE tx_alloc_api_indirect_table;

#define TX_ALLOC_API_FN(fn) tx_alloc_api_indirect_table.fn
#else

void * _cdr_tx_pkt_alloc(int len);
void _cdr_bpool_info_get(bpool_info_t *bp_info);

#define TX_ALLOC_API_FN(fn) fn

#endif

#define cdr_tx_pkt_alloc(_len) TX_ALLOC_API_FN(_cdr_tx_pkt_alloc((_len)))
#define cdr_bpool_info_get(_bp) TX_ALLOC_API_FN(_cdr_bpool_info_get((_bp)))

#ifdef __cplusplus
}
#endif

#define TX_BUFF_FAIL -100
#define TXBUF_ALLOC_TIMEOUT 4000

#endif
