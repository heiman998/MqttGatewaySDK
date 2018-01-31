/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_BEACON_MODE_H__
#define __QCOM_BEACON_MODE_H__

typedef struct { A_UINT8 addr[6]; } A_MACADDR; 

typedef struct { 
    A_UINT8 rate_index;
	A_UINT8 tries;
	A_UINT32 size;
	A_UINT32 chan;
	A_UINT8 header_type;
	A_UINT16  seq; 
	A_MACADDR addr1; 
	A_MACADDR addr2;
	A_MACADDR addr3;
	A_MACADDR addr4;
	A_UINT8 *pdatabuf;
	A_UINT32 buflen;
} QCOM_RAW_MODE_PARAM_t;

extern A_STATUS _qcom_raw_mode_send_pkt(QCOM_RAW_MODE_PARAM_t *ppara);


#endif

