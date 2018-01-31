/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */


#ifndef QCOM_LEGACY_API_H
#define QCOM_LEGACY_API_H

//  TODO: USE_QCOM_REL_3_3_API is for KF, what is for Ruby?
//#ifdef USE_QCOM_REL_3_3_API
#if 0
extern A_STATUS depr_qcom_raw_mode_send_pkt(A_UINT8 rate_index, A_UINT8 tries, A_UINT32 size, A_UINT32 chan, A_UINT8 header_type);
#define qcom_beacon_mode_send_pkt(rate_index, tries, size, chan, header_type)  depr_qcom_raw_mode_send_pkt(rate_index, tries, size, chan, header_type)
#else
#define qcom_raw_mode_send_pkt(ppara) \
   _qcom_raw_mode_send_pkt(ppara) 

#endif

//  TODO: USE_QCOM_REL_3_3_API is for KF, what is for Ruby?
//#ifdef USE_QCOM_REL_3_3_API
#if 0
#undef qcom_set_scan
A_STATUS depr_qcom_set_scan(A_UINT8 device_id);
#define qcom_set_scan(device_id)\
        depr_qcom_set_scan((device_id))

#undef  qcom_scan_set_para
A_STATUS depr_qcom_scan_set_para(A_UINT8 device_id, A_UINT16 max_dwell_time, A_UINT16 pass_dwell_time);
#define qcom_scan_set_para(device_id, max_dwell_time,pass_dwell_time) \
        depr_qcom_scan_set_para((device_id), (max_dwell_time), (pass_dwell_time))


#endif

#endif

