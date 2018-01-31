/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_MISC_H__
#define __QCOM_MISC_H__

#define FM_SW_VERSION "5.0.0.0"

#define FW_VERSION_MAJOR_MASK      (0xF0000000)
#define FW_VERSION_MAJOR_SHIFT     (28)
#define FW_VERSION_MINOR_MASK      (0x0F000000)
#define FW_VERSION_MINOR_SHIFT     (24)
#define FW_VERSION_SUB_MINOR_MASK  (0x00FF0000)
#define FW_VERSION_SUB_MINOR_SHIFT (16)
#define FW_VERSION_CL_MASK         (0x0000FFFF)

#define PRINT_FWVERSION(ver)  (ver&FW_VERSION_MAJOR_MASK)>>FW_VERSION_MAJOR_SHIFT,  \
                                (ver&FW_VERSION_MINOR_MASK )>>FW_VERSION_MINOR_SHIFT, \
                                (ver&FW_VERSION_SUB_MINOR_MASK)>>FW_VERSION_SUB_MINOR_SHIFT,  \
                                (ver&FW_VERSION_CL_MASK)

typedef void (*QCOM_PROMISCUOUS_CB)(unsigned char *buf, int length); 
#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {

A_STATUS (*_qcom_mac_get)(A_UINT8 device_id, A_UINT8 *pMac);

A_STATUS (*_qcom_free_heap_size_get)(A_UINT32 *pSize);
A_STATUS (*_qcom_buffer_info_get)(A_UINT8 *pmac_cnt, A_UINT8 *pmac_reap, A_UINT8 *phtc_cnt, A_UINT8 *phtc_reap, \
       A_UINT8 *pfw_cnt, A_UINT8 *pfw_reap, A_UINT8 *pfree_cnt, A_UINT32 *phtc_get, A_UINT32 *phtc_put);


A_STATUS (*_qcom_promiscuous_enable)(A_BOOL promiscuousEn);
A_STATUS (*_qcom_set_promiscuous_rx_cb)(QCOM_PROMISCUOUS_CB cb);

A_STATUS (*_qcom_firmware_version_get)(A_CHAR *date, A_CHAR *time, A_CHAR *ver, A_CHAR *cl);
#if !defined(AR6002_REV76)
  A_STATUS (*_qcom_set_btcoexconfig_callback)(A_UINT8 device_id, void *callback);
  A_STATUS (*_qcom_set_btcoexstats_callback)(A_UINT8 device_id, void *callback);
  A_INT32 (*_qcom_firmware_partition_index_get)();
  A_STATUS (*_qcom_btcoex_scheme_set)(A_UINT8 scheme_id, void* data);
#endif

}QCOM_MISC_API_INDIRECTION_TABLE;

extern QCOM_MISC_API_INDIRECTION_TABLE qcomMiscApiIndirectionTable;

#define QCOM_MISC_API_FN(fn) qcomMiscApiIndirectionTable.fn

 
#else
 
#define QCOM_MISC_API_FN(fn) fn
A_STATUS _qcom_mac_get(A_UINT8 device_id, A_UINT8 *pMac);

A_STATUS _qcom_free_heap_size_get(A_UINT32 *pSize);
A_STATUS _qcom_buffer_info_get(A_UINT8 *pMac_cnt, A_UINT8 *pMac_reap, A_UINT8 *pHtc_cnt, A_UINT8 *phtc_reap, \
       A_UINT8 *pfw_cnt, A_UINT8 *pfw_reap, A_UINT8 *pfree_cnt, A_UINT32 *phtc_get, A_UINT32 *phtc_put);


A_STATUS _qcom_promiscuous_enable(A_BOOL promiscuousEn);
A_STATUS _qcom_set_promiscuous_rx_cb(QCOM_PROMISCUOUS_CB cb);

#endif //Indirection table

#define qcom_mac_get(devid, pMac) \
    QCOM_MISC_API_FN(_qcom_mac_get((devid), (pMac)))

#define qcom_free_heap_size_get(pSize) \
    QCOM_MISC_API_FN(_qcom_free_heap_size_get((pSize)))

#define qcom_buffer_info_get(pMac_cnt, pMac_reap, pHtc_cnt, phtc_reap, \
       pfw_cnt,pfw_reap,pfree_cnt, phtc_get, phtc_put) \
    QCOM_MISC_API_FN(_qcom_buffer_info_get(( pMac_cnt),( pMac_reap), (pHtc_cnt), (phtc_reap), \
       (pfw_cnt),(pfw_reap),(pfree_cnt),( phtc_get),( phtc_put )))
  
#define qcom_firmware_version_get(data, time, ver, cl) \
    QCOM_MISC_API_FN(_qcom_firmware_version_get((data), (time), (ver), (cl)))


#define qcom_promiscuous_enable(enable) \
    QCOM_MISC_API_FN(_qcom_promiscuous_enable((enable)))

#define qcom_set_promiscuous_rx_cb(cb) \
    QCOM_MISC_API_FN(_qcom_set_promiscuous_rx_cb((cb)))
    
#if defined(AR6002_REV76)
A_STATUS _qcom_param_set(A_UINT8 device_id,A_UINT16 grp_id,A_UINT16 param_id,void * data,A_UINT32 data_length,A_BOOL wait_for_status);
A_STATUS _qcom_param_get(A_UINT8 device_id,A_UINT16 param_id);
A_STATUS _qcom_wmi_param_event_handler(A_UINT8 ucDeviceID, A_UINT16 uEventID, A_UINT8 * pEvtBuffer, A_UINT16 uLength);
A_STATUS _qcom_set_btcoexconfig_callback(A_UINT8 device_id, void *callback);
A_STATUS _qcom_set_btcoexstats_callback(A_UINT8 device_id, void *callback);
A_STATUS _qcom_btcoex_scheme_set(A_UINT8 scheme_id, void* data);
A_INT32 _qcom_firmware_partition_index_get();

#define qcom_param_set(dev_id,grp_id,param_id,data,data_length,wait_for_status)\
            _qcom_param_set((dev_id),(grp_id),(param_id),(data),(data_length),(wait_for_status))

#define qcom_param_get(dev_id,param_id)\
            _qcom_param_get((dev_id),(param_id))

#define qcom_wmi_param_event_handler(device_id,EventID,pEvtBuffer,Length)\
            _qcom_wmi_param_event_handler((device_id),(EventID),(pEvtBuffer),(Length))

#define qcom_set_btcoexconfig_callback(device_id, callback)\
            _qcom_set_btcoexconfig_callback((device_id), (callback))
#define qcom_set_btcoexstats_callback(device_id, callback)\
            _qcom_set_btcoexstats_callback((device_id), (callback))
#define qcom_firmware_partition_index_get() \
    _qcom_firmware_partition_index_get()
    
#define qcom_btcoex_scheme_set(scheme_id, data) \
            _qcom_btcoex_scheme_set((scheme_id), (data))

extern A_STATUS qcom_bluetooth_get_otp_mac(A_UINT8 bd_addr_len, A_UINT8 *bd_addr);
extern A_STATUS qcom_bluetooth_get_otp_trim(A_UINT16 *trim_value); 

#else

#define qcom_param_set(dev_id,grp_id,param_id,data,data_length,wait_for_status)\
            QCOM_MISC_API_FN(_qcom_param_set((dev_id),(grp_id),(param_id),(data),(data_length),(wait_for_status)))

#define qcom_param_get(dev_id,param_id)\
            QCOM_MISC_API_FN(_qcom_param_get((dev_id),(param_id)))

#define qcom_wmi_param_event_handler(device_id,EventID,pEvtBuffer,Length)\
            QCOM_MISC_API_FN(_qcom_wmi_param_event_handler((device_id),(EventID),(pEvtBuffer),(Length)i))
            
#define qcom_set_btcoexconfig_callback(device_id, callback)\
            QCOM_MISC_API_FN(_qcom_set_btcoexconfig_callback((device_id), (callback)))
#define qcom_set_btcoexstats_callback(device_id, callback)\
            QCOM_MISC_API_FN(_qcom_set_btcoexstats_callback((device_id), (callback)))

#define qcom_btcoex_scheme_set(scheme_id, data) \
            QCOM_MISC_API_FN(_qcom_btcoex_scheme_set((scheme_id), (data)))

#define qcom_firmware_partition_index_get() \
    QCOM_MISC_API_FN(_qcom_firmware_partition_index_get())


#endif /*AR6002_REV76*/

#endif //__QCOM_MISC_H_
