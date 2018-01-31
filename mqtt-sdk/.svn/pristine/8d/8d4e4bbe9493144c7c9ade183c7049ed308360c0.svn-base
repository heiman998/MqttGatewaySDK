/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_WLAN_H__
#define _QCOM_WLAN_H__

typedef enum {
    QCOM_WLAN_DEV_MODE_STATION = 0,
    QCOM_WLAN_DEV_MODE_AP,
    QCOM_WLAN_DEV_MODE_ADHOC,
    QCOM_WLAN_DEV_MODE_INVALID
}QCOM_WLAN_DEV_MODE;
typedef enum {
    QCOM_11A_MODE = 0x1,
    QCOM_11B_MODE = 0x2,
    QCOM_11G_MODE = 0x3,
    QCOM_11N_MODE = 0x4,
    QCOM_HT40_MODE = 0x5,
} QCOM_WLAN_DEV_PHY_MODE;

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {
    A_STATUS (*_qcom_op_set_mode)(A_UINT8 device_id, A_UINT32 mode);
    A_STATUS (*_qcom_op_get_mode)(A_UINT8 device_id, A_UINT32 *pmode);
    A_STATUS (*_qcom_disconnect)(A_UINT8 device_id);
#if defined(AR6002_REV74)
    /* Deprecated APIs */
    A_STATUS (*_qcom_set_country_code)(A_UINT8 device_id, A_CHAR *pcountry_code);
    A_STATUS (*_qcom_get_country_code)(A_UINT8 device_id, A_CHAR *pcountry_code);
#endif
    A_STATUS (*_qcom_set_phy_mode)(A_UINT8 device_id, A_UINT8 phymode);
    A_STATUS (*_qcom_get_phy_mode)(A_UINT8 device_id, A_UINT8 *pphymode);

    //channel value start from 1
    A_STATUS (*_qcom_set_channel)(A_UINT8 device_id, A_UINT16 channel);
    A_STATUS (*_qcom_get_channel)(A_UINT8 device_id, A_UINT16 *pchannel);
    A_STATUS (*_qcom_get_channel_freq)(A_UINT8 device_id, A_UINT16 *pch_freq);
    A_STATUS (*_qcom_set_tx_power)(A_UINT8 device_id, A_UINT32 dbm);
    A_STATUS (*_qcom_get_tx_power)(A_UINT8 device_id, A_UINT32 *pdbm);

    A_STATUS (*_qcom_set_lpl_enable)(A_UINT8 device_id, A_UINT8 enable);
    A_STATUS (*_qcom_set_gtx_enable)(A_UINT8 device_id, A_UINT8 enable);
    A_STATUS (*_qcom_set_rate)(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);
    A_STATUS (*_qcom_allow_aggr_set_tid)(A_UINT8 device_id, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
    A_STATUS (*_qcom_allow_aggr_get_tid)(A_UINT8 device_id, A_UINT16 *ptx_allow_aggr, A_UINT16 *prx_allow_aggr);

	A_STATUS (*_qcom_set_connect_callback)(A_UINT8 device_id, void *callback);
    A_STATUS (*_qcom_set_ssid)(A_UINT8 device_id, A_CHAR *pssid);
    A_STATUS (*_qcom_get_ssid)(A_UINT8 device_id, A_CHAR *pssid);
    A_STATUS (*_qcom_get_state)(A_UINT8 device_id, A_UINT8 *pstate);
    A_STATUS (*_qcom_get_disconnect_reason)(A_UINT8 device_id, A_UINT32 *preason);

    A_STATUS (*_qcom_init_dev_context)(void);
    A_STATUS (*_qcom_set_bssid)(A_UINT8 device_id, A_UINT8 *pbssid);
    A_STATUS (*_qcom_get_bssid)(A_UINT8 device_id, A_UINT8 *pbssid);
    A_STATUS (*_qcom_set_channel_freq)(A_UINT8 device_id, A_UINT32 ch_frequency);
    A_STATUS (*_qcom_roam_mode_enable)(A_UINT8 device_id, A_UINT8 enable);

    A_STATUS (*_qcom_roam_para_set)(A_UINT8 device_id, A_UINT16 scan_perid, A_INT16 scan_threshold, A_INT16 rom_threshold, A_UINT8 rssi_floor);
    A_STATUS (*_qcom_set_keep_alive)(A_UINT8 device_id, A_UINT32 keep_alive_interval);
    
} QCOM_WLAN_API_INDIRECTION_TABLE;

extern QCOM_WLAN_API_INDIRECTION_TABLE qcomWlanApiIndirectionTable;
#define QCOM_WLAN_API_FN(fn) qcomWlanApiIndirectionTable.fn

#else

#define QCOM_WLAN_API_FN(fn) fn


A_STATUS _qcom_op_set_mode(A_UINT8 device_id, A_UINT32 mode);
A_STATUS _qcom_op_get_mode(A_UINT8 device_id, A_UINT32 *pmode);

A_STATUS _qcom_disconnect(A_UINT8 device_id);

A_STATUS _qcom_set_phy_mode(A_UINT8 device_id, A_UINT8 phymode);
A_STATUS _qcom_get_phy_mode(A_UINT8 device_id, A_UINT8 *pphymode);

//channel value start from 1
A_STATUS _qcom_set_channel(A_UINT8 device_id, A_UINT16 channel);
A_STATUS _qcom_get_channel(A_UINT8 device_id, A_UINT16 *pchannel);
A_STATUS _qcom_get_channel_freq(A_UINT8 device_id, A_UINT16 *pch_freq);

A_STATUS _qcom_set_tx_power(A_UINT8 device_id, A_UINT32 dbm);
A_STATUS _qcom_get_tx_power(A_UINT8 device_id, A_UINT32 *pdbm);

A_STATUS _qcom_allow_aggr_set_tid(A_UINT8 device_id, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
A_STATUS _qcom_allow_aggr_get_tid(A_UINT8 device_id, A_UINT16 *ptx_allow_aggr, A_UINT16 *prx_allow_aggr);

A_STATUS _qcom_set_connect_callback(A_UINT8 device_id, void *callback);

A_STATUS _qcom_get_ssid(A_UINT8 device_id, A_CHAR *pssid);
A_STATUS _qcom_set_ssid(A_UINT8 device_id, A_CHAR *pssid);

A_STATUS _qcom_get_state(A_UINT8 device_id, A_UINT8 *pstate);
A_STATUS _qcom_get_disconnect_reason(A_UINT8 device_id, A_UINT32 *preason);

A_STATUS _qcom_init_dev_context(void);

A_STATUS _qcom_set_channel_freq(A_UINT8 device_id, A_UINT32 ch_frequency);
A_STATUS _qcom_set_bssid(A_UINT8 device_id, A_UINT8 *pbssid);
A_STATUS _qcom_get_bssid(A_UINT8 device_id, A_UINT8 *pbssid);
A_STATUS _qcom_set_lpl_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_set_gtx_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_set_rate(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);

#endif //DISABLE_FUNC_INDIRECTION

#define qcom_op_set_mode(device_id,mode)\
    QCOM_WLAN_API_FN(_qcom_op_set_mode((device_id),(mode)))

#define qcom_op_get_mode(device_id,pmode)\
    QCOM_WLAN_API_FN(_qcom_op_get_mode((device_id),(pmode)))

#define qcom_disconnect(device_id)\
    QCOM_WLAN_API_FN(_qcom_disconnect((device_id)))

#define qcom_set_country_code(device_id,coutry_code)\
    QCOM_WLAN_API_FN(_qcom_set_country_code((device_id),(country_code)))

#define qcom_get_country_code(device_id,pCountry_code)\
    QCOM_WLAN_API_FN(_qcom_get_country_code((device_id),(pCountry_code)))

#define qcom_set_phy_mode(device_id,phymode)\
    QCOM_WLAN_API_FN(_qcom_set_phy_mode((device_id),(phymode)))

#define qcom_get_phy_mode(device_id,pPhymode)\
    QCOM_WLAN_API_FN(_qcom_get_phy_mode((device_id),(pPhymode)))


#define qcom_set_channel(device_id, channel)\
    QCOM_WLAN_API_FN(_qcom_set_channel((device_id),(channel)))

#define qcom_get_channel(device_id, pChannel)\
    QCOM_WLAN_API_FN(_qcom_get_channel((device_id),(pChannel)))

#define qcom_get_channel_freq(device_id, pch_freq)\
    QCOM_WLAN_API_FN(_qcom_get_channel_freq((device_id), (pch_freq)))

#define qcom_set_tx_power(device_id, dbm)\
    QCOM_WLAN_API_FN(_qcom_set_tx_power((device_id), (dbm)))

#define qcom_get_tx_power(device_id, pdbm)\
    QCOM_WLAN_API_FN(_qcom_get_tx_power((device_id), (pdbm)))


#define qcom_allow_aggr_set_tid(device_id, tx_allow_aggr,rx_allow_aggr)\
    QCOM_WLAN_API_FN(_qcom_allow_aggr_set_tid((device_id),( tx_allow_aggr),( rx_allow_aggr)))

#define qcom_allow_aggr_get_tid(device_id, prx_allow_aggr)\
    QCOM_WLAN_API_FN(_qcom_allow_aggr_get_tid((device_id),(prx_allow_aggr)))


#define qcom_set_connect_callback(device_id,callback)\
    QCOM_WLAN_API_FN(_qcom_set_connect_callback(( device_id),(callback)))

#define qcom_set_ssid(device_id,ssid)\
    QCOM_WLAN_API_FN(_qcom_set_ssid((device_id),(ssid)))
#define qcom_get_ssid(device_id,pssid)\
    QCOM_WLAN_API_FN(_qcom_get_ssid((device_id),(pssid)))


#define qcom_get_state(device_id, pstate)\
    QCOM_WLAN_API_FN(_qcom_get_state((device_id),(pstate)))

#define qcom_init_dev_context()\
    QCOM_WLAN_API_FN(_qcom_init_dev_context())


#ifdef AR6002_REV74
A_STATUS _qcom_set_bssid(A_UINT8 device_id, A_UINT8 *pbssid);
A_STATUS _qcom_get_bssid(A_UINT8 device_id, A_UINT8 *pbssid);
A_STATUS _qcom_set_channel_freq(A_UINT8 device_id, A_UINT32 ch_frequency);
A_STATUS _qcom_set_lpl_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_set_gtx_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_set_rate(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);
A_STATUS _qcom_get_disconnect_reason(A_UINT8 device_id, A_UINT32 *preason);


#define qcom_set_bssid(device_id,bssid)\
    _qcom_set_bssid((device_id),(bssid))
#define qcom_get_bssid(device_id,bssid)\
    _qcom_get_bssid((device_id),(bssid))
#define qcom_set_channel_freq(device_id, ch_frequency)\
    _qcom_set_channel_freq((device_id), (ch_frequency))
#define qcom_set_lpl_enable(device_id, enable) \
    _qcom_set_lpl_enable((device_id), (enable))
#define qcom_set_gtx_enable(device_id, enable) \
    _qcom_set_gtx_enable((device_id), (enable))
#define qcom_set_rate(device_id, mcs, rate) \
    _qcom_set_rate((device_id), (mcs), (rate))
#define qcom_get_disconnect_reason(device_id, preason)\
    _qcom_get_disconnect_reason((device_id),(preason))
#else   /* AR6002_REV74 */

#define qcom_roam_mode_enable(device_id, enable)\
    QCOM_WLAN_API_FN(_qcom_roam_mode_enable((device_id),(enable)))
#define qcom_roam_para_set(device_id, scan_perid, preason)\
    QCOM_WLAN_API_FN(_qcom_roam_para_set((device_id),(scan_perid),(preason)))
#define qcom_set_keep_alive(device_id, keep_alive_interval)\
    QCOM_WLAN_API_FN(_qcom_set_keep_alive((device_id), (keep_alive_interval)))	
        
#define qcom_set_bssid(device_id,bssid)\
    QCOM_WLAN_API_FN(_qcom_set_bssid((device_id),(bssid)))
#define qcom_get_bssid(device_id,bssid)\
    QCOM_WLAN_API_FN(_qcom_get_bssid((device_id),(bssid)))
#define qcom_set_channel_freq(device_id, ch_frequency)\
    QCOM_WLAN_API_FN(_qcom_set_channel_freq((device_id), (ch_frequency)))
#define qcom_set_lpl_enable(device_id, enable) \
    QCOM_WLAN_API_FN(_qcom_set_lpl_enable((device_id), (enable)))
#define qcom_set_gtx_enable(device_id, enable) \
    QCOM_WLAN_API_FN(_qcom_set_gtx_enable((device_id), (enable)))
#define qcom_set_rate(device_id, mcs, rate) \
    QCOM_WLAN_API_FN(_qcom_set_rate((device_id), (mcs), (rate)))
#define qcom_get_disconnect_reason(device_id, preason) \
    QCOM_WLAN_API_FN(_qcom_get_disconnect_reason((device_id), (preason)))
#endif /* AR6002_REV74 */
#endif
