/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 *
 * $Id: //depot/sw/branches/olca/target/src/wlan/include/p2p_api.h#63 $
 * Notes:  This File exports the APIs of the P2P module.
 */
#ifndef __P2P_API_H__
#define __P2P_API_H__

#include <athdefs.h>
#include <osapi.h>
#include <athdefs.h>
#include "p2p.h"
#include "p2p_defs.h"

#ifdef OCS_NOA_SUPPORT
#define P2P_GO_MAX_SUPPORT_NOA_DESC         (P2P_MAX_NOA_DESCRIPTORS)
#define P2P_GO_MAX_HOST_NOA_DESC            (P2P_GO_MAX_SUPPORT_NOA_DESC - 3)
#else
#define P2P_GO_MAX_SUPPORT_NOA_DESC         (P2P_MAX_NOA_DESCRIPTORS)
#define P2P_GO_MAX_HOST_NOA_DESC            (P2P_GO_MAX_SUPPORT_NOA_DESC - 1)
#endif

#define P2P_GO_SCH_TYPE_MASK                0xe0
#define P2P_GO_SCH_TYPE_HOST                0x40
#define P2P_GO_SCH_TYPE_TARGET              0x80
//#define P2P_GO_SCH_TYPE_OCS                 0x20
/* The ocs module do not need the type ocs, it will use the type_target */
#define P2P_GO_SCH_TYPE_CLIENT              0x20

#define P2P_GO_SCH_TYPE_INVALID             0x00

#ifndef AR6002_REV43

typedef struct p2p_api_fn_t {
#if !defined(AR6002_REV62)
A_BOOL      (* _P2P_AllocTxPacketID)(devh_t *dev, A_UINT32 ID, void *pPacket);
#else
A_BOOL      (* _P2P_AllocTxPacketID)(A_UINT32 ID, void *pPacket);
#endif
A_UINT8 *   (* _P2P_AddDiscoveryIEs)(devh_t *dev, A_UINT8 *frm, A_UINT8 type);
void        (* _P2P_CancelRemainOnChannel)(devh_t *dev);
void        (* _P2P_DisableRate11B)(devh_t *dev, WMI_DISABLE_11B_RATES_CMD_STRUCT *buf);
#if !defined(AR6002_REV62)
A_BOOL      (* _P2P_FreeTxPacketID)(devh_t *dev, A_UINT32 ID);
#else
A_BOOL      (* _P2P_FreeTxPacketID)(A_UINT32 ID);
#endif
void        (* _P2P_GetInfo)(devh_t *dev, WMI_GET_P2P_INFO_STRUCT *pParam);
#if !defined(AR6002_REV62)
A_UINT32    (* _P2P_GetTxPacketID)(devh_t *dev, void *pPacket);
#else
A_UINT32    (* _P2P_GetTxPacketID)(void *pPacket);
#endif
void        (* _P2P_HandleProbeRequest)(devh_t *dev, A_UINT8 *pBufStart, A_UINT16 Len);
#if !defined(AR6002_REV62)
void        (* _P2P_ProbeReportEnable)(devh_t *dev, A_UINT8 Enable);
#else
void        (* _P2P_ProbeReportEnable)(A_UINT8 Enable);
#endif
A_STATUS    (* _P2P_RemainOnChannel)(devh_t *dev, WMI_REMAIN_ON_CHNL_CMD_STRUCT *pRemainOnChStruct);
void        (* _P2P_RemainOnChannelStartCB)(void *arg, A_STATUS status);
#if !defined(AR6002_REV62)
void        (* _P2P_RemainOnChannelStateInit)(devh_t *dev);
#else
void        (* _P2P_RemainOnChannelStateInit)();
#endif
void        (* _P2P_RemainOnChannelStopCB)(void *arg, A_STATUS status);
void        (* _P2P_RemainOnChannelToHostEvent)(devh_t *dev, A_UINT8 Status);
#if defined(AR6002_REV43) || defined(AR6002_REV61) || defined(AR6002_REV62)
void        (* _P2P_RemainOnChannelUnLockCB)(void *arg, A_STATUS status);
#endif
void        (* _P2P_RxActionFrame)(devh_t *dev, A_UINT8 *pBufStart, A_UINT16 Len);
void        (* _P2P_SendAction)(devh_t *dev, WMI_SEND_ACTION_CMD_STRUCT *pActionCmd);
void        (* _P2P_SendInfoEventToHost)(devh_t *dev, void *pBufStart, A_UINT16 Len, A_UINT32 Flags);
void        (* _P2P_SendProbeResponse)(devh_t *dev, WMI_P2P_PROBE_RESPONSE_CMD_STRUCT *pProbe);
void        (* _P2P_SendTxStatus)(devh_t *dev, A_UINT8 ACKStatus, A_UINT32 ID);
void        (* _P2P_SendWMIEventToHost)(devh_t *dev, void *event, A_UINT16 eventId);
void        (* _P2P_TxDoneCB)(devh_t *dev, struct ath_buf *abf, TXRX_STATUS status);
void        (* _P2P_HostModelInit)(devh_t *pdev, A_UINT8 *ifAddr);
#if !defined (AR6002_REV43) && !defined(AR6002_REV61) && !defined(AR6002_REV62)
A_UINT32    (* _P2P_IsIEPresent)(devh_t *dev, struct ath_buf *abf, A_INT32 subtype);
#else
A_BOOL      (* _P2P_IsP2PIEPresent)(devh_t *dev, struct ath_buf *abf, A_INT32 subtype);
#endif
#if defined(AR6002_REV43) || defined(AR6002_REV61) || defined(AR6002_REV62) || defined(AR6002_REV63) || defined(AR6002_REV64) || defined(AR6002_REV65)
A_BOOL      (* _P2P_IsHomeChEqualP2P)(A_UINT32 P2PFreq);
#endif
A_UINT32    (* _P2P_FreqToChan)(devh_t *dev);
A_BOOL      (* _P2P_UpdateSubMode)(devh_t *dev, tP2P_SUBMODE_STIMULUS Stimulus);
void        (* _P2P_DevInit)(devh_t *pdev);
void        (* _P2P_SwitchMode)(devh_t *pdev, A_UINT8 NewMode);
void        (* _P2P_HandleFrameWithNOA)(devh_t *dev, A_UINT8 *pBufStart, A_UINT16 Len, A_UINT32 frameSubType);
#if !defined(AR6002_REV62)
ie_info_t*  (* _P2P_GetAssocIeInfo)(devh_t *dev);
A_BOOL      (* _P2P_IsP2PGO)(devh_t *dev);
#endif
void        (* _P2P_ResetPs)(devh_t *dev);
#if defined(AR6002_REV64)
A_UINT8     (* _P2P_AddNoa)(devh_t *dev, P2P_NOA_DESCRIPTOR *pNoa);
#else
A_UINT8     (* _P2P_AddNoa)(devh_t *dev, P2P_NOA_DESCRIPTOR *pNoa, A_UINT8 type);
#endif
A_UINT8*    (* _P2P_AddNoaIe)(devh_t *dev, A_UINT8 *frm, A_UINT8 *ie_len);
A_UINT8     (* _P2P_GetLastNoaLen)(devh_t *dev, A_BOOL *one_shot);
void        (* _P2P_GoCheckAllStaSleep)(devh_t *dev);
void        (* _P2P_NotifyBeaconComp)(devh_t *dev);
void        (* _P2P_SetOppPS)(devh_t *dev, WMI_OPPPS_INFO_STRUCT *buffer);
#if defined(AR6002_REV64)
void        (* _P2P_GetOppPS)(devh_t *dev, A_UINT8 fakeenable);
#else
void        (* _P2P_GetOppPS)(devh_t *dev);
#endif
void        (* _P2P_SetNoa)(devh_t *dev, WMI_NOA_INFO_STRUCT *buffer);
void        (* _P2P_GetNoa)(devh_t *dev);
#if defined(AR6002_REV64)
A_UINT8     (* _P2P_DelNoa)(devh_t *pdev, A_UINT8 index);
#else
A_UINT8     (* _P2P_DelNoa)(devh_t *pdev, A_UINT8 index, A_UINT8 type);
#endif
A_UINT8     (* _P2P_Powersave_ie_bytarget)(devh_t *dev);
void        (* _P2P_Powersave_ie_config)(devh_t *dev, A_UINT8 bytarget);
void        (* _P2P_Send_Noa_Action)(devh_t *dev);
A_BOOL      (* _P2P_GO_Fakesleep)(devh_t *pdev, A_UINT8 en, A_UINT32 start, A_UINT32 duration, A_UINT8 cnt, A_UINT32 itv);
#if !defined(AR6002_REV43) && !defined(AR6002_REV61) && !defined(AR6002_REV62) && !defined(AR6002_REV63) && !defined(AR6002_REV64) && !defined(AR6002_REV65)
channel_t*  (* _P2P_GetHomeChannel)(devh_t *dev);
#endif
#ifdef OCS_NOA_SUPPORT
void        (* _P2P_Noa_Action_Send_Comp)(devh_t *dev, A_STATUS status);
#endif
A_BOOL      (* _P2P_GetSchNextTimeout)(devh_t *pdev, A_UINT32 *next_timeout);

} P2P_API_FN;

typedef struct p2p_fw_api_fn_t {

A_STATUS    (* _p2p_rx_action)(devh_t *dev,const A_UINT8 *data, A_UINT32 len, struct ath_buf *,struct ieee80211_frame *wh, A_UINT8 category);
A_INT8      (* _p2p_probe_req_rx)(devh_t *dev, const A_UINT8 *addr, const A_UINT8 *ie,
                                 A_UINT32 ie_len);
void        (* _p2p_find)(devh_t *dev, WMI_P2P_FW_FIND_CMD *);
void        (* _p2p_listen)(devh_t *dev, WMI_P2P_LISTEN_CMD *);
void        (* _p2p_stop_find)(devh_t *dev);
void        (* _p2p_set_config)(devh_t *dev, WMI_P2P_FW_SET_CONFIG_CMD *);
void        (* _p2p_rx_beacon_probe)(devh_t *dev,const A_UINT8 *ies,A_UINT32 ie_len);
A_UINT8     (* _p2p_get_assoc_ie_len)(devh_t *dev);
A_UINT8 *   (* _p2p_add_assoc_req_ie)(devh_t *dev,A_UINT8 *frm);
void        (* _p2p_add_probe_req_ie)(devh_t *dev, A_UINT8 *probeIe,A_UINT32 wpsLen);
A_UINT8     (* _p2p_is_listening)(devh_t *dev);
void        (*_p2p_scan_ie)(devh_t *dev, A_UINT8 *ies);
void        (* _p2p_dev_init)(devh_t *dev);
void        (* _p2p_init)(devh_t *dev,A_UINT8 *ifAddr);
void        (* _p2p_parse_scanlist)(devh_t *dev, WMI_BI_FTYPE ftype,WMI_BSS_INFO_HDR2 *hdr,const A_UINT8 *data, A_UINT32 len);
void        (*_p2p_send_p2p_list)(devh_t *dev);
void        (*_p2p_begin_go_neg)(devh_t *dev,WMI_P2P_FW_CONNECT_CMD_STRUCT *buf);
void        (*_p2p_auth_go_neg)(devh_t *dev,WMI_P2P_FW_CONNECT_CMD_STRUCT *buf);
void        (*_p2p_set_cmd)(devh_t *dev, WMI_P2P_SET_CMD *buf);
void        (*_p2p_invite)(devh_t *dev, WMI_P2P_FW_INVITE_CMD *buf);
void        (*_p2p_cancel)(devh_t *dev);
A_UINT8     (*_p2p_grp_formation_done)(devh_t *dev, struct go_formation_s *buf);
void        (*_p2p_auth_invite)(devh_t *dev,WMI_P2P_FW_INVITE_REQ_RSP_CMD *buf);
void        (*_p2p_join_profile)(devh_t *dev,WMI_P2P_FW_CONNECT_CMD_STRUCT *buf);
A_STATUS    (*_p2p_read_join_profile)(devh_t *dev,WMI_WPS_START_CMD *buf);
void        (*_p2p_profile_enable)(devh_t * dev,WMI_P2P_SET_PROFILE_CMD *p2pProfile);
A_BOOL      (*_p2p_is_valid_p2p_ie_present)(devh_t *dev, struct ath_buf *abf, A_INT32 subtype);
void        (*_p2p_go_init)(devh_t *dev);
A_STATUS    (*_p2p_go_handle_powersave)(devh_t *dev, A_UINT8 *mac);
#ifdef CONFIG_P2P_FW_OFFLOAD
void        (*_p2p_fw_list_persistent_prof_cmd)(devh_t *dev);
void        (* _p2p_grp_formation_persistent)(devh_t *dev);
void        (* _p2p_copy_p2p_credentials)(devh_t *dev);
void        (* _p2p_get_peer_dev_addr)(devh_t *dev, struct ath_buf *abf, A_UINT16 subtype);
void        (* _p2p_offload_connect_handler_concurrent)(devh_t *dev, bss_t *bss);
void        (* _p2p_offload_disconnect_handler_concurrent)(devh_t *dev);
#endif
void  (* _P2PLite_Parse)(P2P_DEVICE_LITE *p2pDevLite,struct p2p_device *p2pDev);
WMI_FILTER_ACTION (*_p2p_event_filter)(void* pCxt, A_UINT16 EventId, A_UINT16 info,A_UINT8 *pBuffer,int length);

A_UINT32    (*_p2p_strrcl_wakeupmgr_cb)(int calltype, void *priv_data);
void        (*_p2p_strrcl_wakeupmgr_init)(devh_t *pdev);
A_STATUS    (* _storerecall_fill_p2p_snapshot)(devh_t *dev);
void        (* _storerecall_recall_p2p)(devh_t *dev);

A_BOOL      (*_p2p_go_is_cck_rates_enabled)(devh_t *dev);
A_UINT8     (* _p2p_is_channel_valid)(devh_t *dev, A_UINT8 channelNum);
void        (* _p2p_fw_go_timeout)(A_HANDLE alarm, void *data);
void        (* _p2p_write_dset_CB)(A_STATUS status, void *arg);
void        (* _p2p_write_dset_SM)(A_STATUS status, void *arg);
A_UINT32    (* _p2p_fw_IsIEPresent)(devh_t *dev, struct ath_buf *abf, A_INT32 subtype);
    
}P2P_FW_API_FN;

typedef struct p2p_fw_pd_fn_t{
void        (*_p2p_prov_disc_req)(devh_t *dev, WMI_P2P_FW_PROV_DISC_REQ_CMD *buf);
}P2P_FW_PD_FN;

typedef struct p2p_fw_sd_fn_t{
void        (*_p2p_sdpd_tx_cmd)(devh_t *dev, WMI_P2P_SDPD_TX_CMD *buf);
void        (*_p2p_stop_sdpd)(devh_t *dev);
}P2P_FW_SD_FN;

typedef struct p2p_fw_go_fn_t {
void     (*_p2p_group_update_ies)(devh_t *dev);
void     (*_p2p_grp_init)(devh_t *dev, WMI_P2P_GRP_INIT_CMD *buf);
A_UINT8 *(*_p2p_grp_add_beacon_probe_ie)(devh_t *dev, A_UINT8 *frm, A_UINT8 type, struct ath_buf *probe_req_abf);
A_UINT8 *(*_p2p_go_add_assoc_resp_ie)(devh_t *dev, A_UINT8 *frm, A_UINT16 status);
void     (*_p2p_go_assoc_event_rx)(devh_t *dev, conn_t *conn,A_UCHAR *assocbuf, A_UINT32 assocbufLen);
void     (*_p2p_go_disassoc_event_rx)(devh_t *dev, conn_t *conn);
void     (*_p2p_go_set_grp_limit)(devh_t *dev, A_UINT8 val);
A_INT8   (*_p2p_go_probe_req_rx)(devh_t *dev, const A_UINT8 *addr,const A_UINT8 *ie, A_UINT32 ie_len);
A_STATUS (*_p2p_group_discover)(devh_t *dev, const A_UINT8 *devid);
void     (*_p2p_grp_update_beacon_ie)(devh_t *dev,A_UINT8 *efrm);
} P2P_FW_GO_FN;

typedef struct p2p_fw_go_ps_fn_t {

void    (*_p2p_go_set_oppPS)(devh_t *pdev, WMI_OPPPS_INFO_STRUCT *buffer);
void    (*_p2p_go_get_oppPS)(devh_t *pdev);
void    (*_p2p_go_set_noa)(devh_t *dev, WMI_NOA_INFO_STRUCT *buffer);
void    (*_p2p_go_get_noa)(devh_t *pdev);
void    (*_p2p_reset_ps)(devh_t *pdev);
void    (*_p2p_go_checkallsta_sleep)(devh_t *pdev);
A_UINT8 *(*_p2p_fw_add_noa_ie)(devh_t *pdev, A_UINT8 *frm, A_UINT8 *noa_len);
void    (*_p2p_notify_beacon_comp)(devh_t *pdev);
A_UINT8 (*_p2p_get_noa_len)(devh_t *pdev, A_BOOL *one_shot);
A_BOOL  (*_p2p_fw_GetSchNextTimeout)(devh_t *dev, A_UINT32 *next_timeout);
A_UINT8 (*_p2p_fw_go_add_noa)(devh_t *pdev, P2P_NOA_DESCRIPTOR *pNoa, A_UINT8 type);
A_UINT8 (*_p2p_fw_go_del_noa)(devh_t *pdev, A_UINT8 index, A_UINT8 type);
void    (*_p2p_fw_go_send_noa_action)(devh_t *dev);
A_UINT8 *  (*_p2p_fw_add_noa_ie_type)(devh_t *pdev, A_UINT8 *frm, A_UINT8 *noa_len,A_UINT8 type);
void (* _p2p_fw_Noa_Action_Send_Comp)(devh_t *dev, A_STATUS status);
}P2P_FW_GO_PS_FN;

extern  P2P_API_FN             p2p_api_table;
extern  P2P_FW_API_FN          p2p_fw_api_table;
extern  P2P_FW_PD_FN           p2p_fw_pd_table;
extern  P2P_FW_SD_FN           p2p_fw_sd_table;
extern  P2P_FW_GO_FN           p2p_fw_go_table;
extern  P2P_FW_GO_PS_FN        p2p_fw_go_ps_table;

#define P2P_API_FUNC(fn)           p2p_api_table.fn
#define P2P_FW_API_FUNC(fn)        p2p_fw_api_table.fn
#define P2P_FW_PD_FUNC(fn)         p2p_fw_pd_table.fn
#define P2P_FW_SD_FUNC(fn)         p2p_fw_sd_table.fn
#define P2P_FW_GO_FUNC(fn)         p2p_fw_go_table.fn
#define P2P_FW_GO_PS_FUNC(fn)      p2p_fw_go_ps_table.fn

#define P2P_AddDiscoveryIEs(dev, frm, type) \
    P2P_API_FUNC(_P2P_AddDiscoveryIEs((dev), (frm), (type)))

#if !defined(AR6002_REV62)
#define P2P_AllocTxPacketID(dev, ID, pPacket) \
    P2P_API_FUNC(_P2P_AllocTxPacketID((dev), (ID), (pPacket)))
#else
#define P2P_AllocTxPacketID(ID, pPacket) \
    P2P_API_FUNC(_P2P_AllocTxPacketID((ID), (pPacket)))
#endif

#define P2P_Powersave_ie_bytarget(dev) \
    P2P_API_FUNC(_P2P_Powersave_ie_bytarget((dev)))

#define P2P_Powersave_ie_config(dev, bytarget) \
    P2P_API_FUNC(_P2P_Powersave_ie_config((dev), (bytarget)))

#define P2P_CancelRemainOnChannel(dev) \
    P2P_API_FUNC(_P2P_CancelRemainOnChannel((dev)))

#define P2P_DisableRate11B(dev, buf) \
    P2P_API_FUNC(_P2P_DisableRate11B((dev),(buf)))

#define P2P_FreeTxPacketID(dev, ID) \
    P2P_API_FUNC(_P2P_FreeTxPacketID((dev), (ID)))

#define P2P_GetInfo(dev, pParam) \
    P2P_API_FUNC(_P2P_GetInfo((dev), (pParam)))

#if !defined(AR6002_REV62)
#define P2P_GetTxPacketID(dev, pPacket) \
    P2P_API_FUNC(_P2P_GetTxPacketID((dev), (pPacket)))
#else
#define P2P_GetTxPacketID(pPacket) \
    P2P_API_FUNC(_P2P_GetTxPacketID((pPacket)))
#endif

#define P2P_HandleProbeRequest(dev, pBufStart, Len) \
    P2P_API_FUNC(_P2P_HandleProbeRequest((dev), (pBufStart), (Len)))

#if !defined(AR6002_REV62)
#define P2P_ProbeReportEnable(dev, Enable) \
    P2P_API_FUNC(_P2P_ProbeReportEnable((dev), (Enable)))
#else
#define P2P_ProbeReportEnable(Enable) \
    P2P_API_FUNC(_P2P_ProbeReportEnable((Enable)))
#endif

#define P2P_RemainOnChannel(dev, pRemainOnChStruct) \
    P2P_API_FUNC(_P2P_RemainOnChannel((dev),(pRemainOnChStruct)))

#define P2P_RemainOnChannelStartCB(arg, status) \
    P2P_API_FUNC(_P2P_RemainOnChannelStartCB((arg),(status)))

#if !defined(AR6002_REV62)
#define P2P_RemainOnChannelStateInit(dev) \
    P2P_API_FUNC(_P2P_RemainOnChannelStateInit((dev)))
#else
#define P2P_RemainOnChannelStateInit() \
    P2P_API_FUNC(_P2P_RemainOnChannelStateInit())
#endif


#define P2P_RemainOnChannelStopCB(arg, status) \
    P2P_API_FUNC(_P2P_RemainOnChannelStopCB((arg),(status)))

#define P2P_RemainOnChannelToHostEvent(dev, Status) \
    P2P_API_FUNC(_P2P_RemainOnChannelToHostEvent((dev),(Status)))

#define P2P_RemainOnChannelUnLockCB(arg, status) \
    P2P_API_FUNC(_P2P_RemainOnChannelUnLockCB((arg),(status)))

#define P2P_RxActionFrame(dev, pBufStart, Len) \
    P2P_API_FUNC(_P2P_RxActionFrame((dev), (pBufStart), (Len)))

#define P2P_SendAction(dev, pActionCmd) \
    P2P_API_FUNC(_P2P_SendAction((dev), (pActionCmd)))

#define P2P_SendInfoEventToHost(dev, pBufStart, Len, Flags) \
    P2P_API_FUNC(_P2P_SendInfoEventToHost((dev), (pBufStart), (Len), (Flags)))

#define P2P_SendProbeResponse(dev, pProbe) \
    P2P_API_FUNC(_P2P_SendProbeResponse((dev), (pProbe)))

#define P2P_SendTxStatus(dev, ACKStatus, ID) \
    P2P_API_FUNC(_P2P_SendTxStatus((dev), (ACKStatus), (ID)))

#define P2P_SendWMIEventToHost(dev, event, eventId) \
    P2P_API_FUNC(_P2P_SendWMIEventToHost((dev), (event), (eventId)))

#define P2P_TxDoneCB(dev, abf, status) \
    P2P_API_FUNC(_P2P_TxDoneCB((dev), (abf), (status)))

#define P2P_HostModelInit(pdev, ifAddr) \
    P2P_API_FUNC(_P2P_HostModelInit((pdev), (ifAddr)))

#if !defined (AR6002_REV43) && !defined(AR6002_REV61) && !defined(AR6002_REV62)
#define P2P_IsIEPresent(pdev, abf, subtype) \
    P2P_API_FUNC(_P2P_IsIEPresent((pdev), (abf), (subtype)))
#else
#define P2P_IsP2PIEPresent(pdev, abf, subtype) \
    P2P_API_FUNC(_P2P_IsP2PIEPresent((pdev), (abf), (subtype)))
#endif

#if defined(AR6002_REV43) || defined(AR6002_REV61) || defined(AR6002_REV62) || defined(AR6002_REV63) || defined(AR6002_REV64) || defined(AR6002_REV65)
#define P2P_IsHomeChEqualP2P(P2PFreq) \
    P2P_API_FUNC(_P2P_IsHomeChEqualP2P((P2PFreq)))
#endif

#if !defined(AR6002_REV43) && !defined(AR6002_REV61) && !defined(AR6002_REV62) && !defined(AR6002_REV63) && !defined(AR6002_REV64) && !defined(AR6002_REV65)
#define P2P_GetHomeChannel(dev) \
    P2P_API_FUNC(_P2P_GetHomeChannel((dev)))
#endif

#define P2P_FreqToChan(dev) \
    P2P_API_FUNC(_P2P_FreqToChan((dev)))

#define P2P_UpdateSubMode(dev, Stimulus) \
    P2P_API_FUNC(_P2P_UpdateSubMode((dev), (Stimulus)))

#define P2P_DevInit(pdev) \
    P2P_API_FUNC(_P2P_DevInit((pdev)))

#define P2P_HandleFrameWithNOA(dev, pBufStart, Len, frameSubType) \
    P2P_API_FUNC(_P2P_HandleFrameWithNOA((dev), (pBufStart), (Len), (frameSubType)))

#define P2P_SwitchMode(pdev, NewMode) \
    P2P_API_FUNC(_P2P_SwitchMode((pdev),(NewMode)))

#if !defined(AR6002_REV62)
#define P2P_GetAssocIeInfo(pdev) \
    P2P_API_FUNC(_P2P_GetAssocIeInfo((pdev)))
#define P2P_IsP2PGO(pdev) \
    P2P_API_FUNC(_P2P_IsP2PGO((pdev)))
#endif

#define P2P_ResetPs(pdev)       \
    P2P_API_FUNC(_P2P_ResetPs((pdev)))
#if defined(AR6002_REV64)
#define P2P_AddNoa(pdev, pNoa)  \
    P2P_API_FUNC(_P2P_AddNoa((pdev), (pNoa)))
#define P2P_DelNoa(pdev, index)  \
	P2P_API_FUNC(_P2P_DelNoa((pdev), (index)))
#else
#define P2P_AddNoa(pdev, pNoa, type)  \
    P2P_API_FUNC(_P2P_AddNoa((pdev), (pNoa), (type)))
#define P2P_DelNoa(pdev, index, type)  \
	P2P_API_FUNC(_P2P_DelNoa((pdev), (index), (type)))
#endif
#define P2P_GO_Fakesleep(pdev, en, offset, duration, count, interval)  \
	P2P_API_FUNC(_P2P_GO_Fakesleep((pdev), (en), (offset), (duration), (count), (interval)))
#define P2P_AddNoaIe(pdev, frm, ie_len)    \
    P2P_API_FUNC(_P2P_AddNoaIe((pdev), (frm), (ie_len)))
#define P2P_Send_Noa_Action(pdev)    \
    P2P_API_FUNC(_P2P_Send_Noa_Action((pdev)))
#ifdef OCS_NOA_SUPPORT
#define P2P_Noa_Action_Send_Comp(dev, status)    \
    P2P_API_FUNC(_P2P_Noa_Action_Send_Comp((dev), (status)))
#endif
#define P2P_GetLastNoaLen(pdev, one_shot)  \
    P2P_API_FUNC(_P2P_GetLastNoaLen((pdev), (one_shot)))
#define P2P_GoCheckAllStaSleep(pdev)   \
    P2P_API_FUNC(_P2P_GoCheckAllStaSleep((pdev)))
#define P2P_NotifyBeaconComp(pdev)     \
    P2P_API_FUNC(_P2P_NotifyBeaconComp((pdev)))
#define P2P_SetOppPS(dev, buf)      \
    P2P_API_FUNC(_P2P_SetOppPS((dev), (buf)))
#if defined(AR6002_REV64)
#define P2P_GetOppPS(dev, fakeenable)           \
    P2P_API_FUNC(_P2P_GetOppPS((dev), (fakeenable)))
#else
#define P2P_GetOppPS(dev)           \
    P2P_API_FUNC(_P2P_GetOppPS((dev)))
#endif
#define P2P_SetNoa(dev, buf)        \
    P2P_API_FUNC(_P2P_SetNoa((dev), (buf)))
#define P2P_GetNoa(dev)             \
    P2P_API_FUNC(_P2P_GetNoa((dev)))
#define P2P_GetSchNextTimeout(dev, next_timeout) \
    P2P_API_FUNC(_P2P_GetSchNextTimeout((dev), (next_timeout)))

/* P2P FW OFFLOAD */
#define p2p_rx_action(pDev,data,len,abf, wh, category) \
    P2P_FW_API_FUNC(_p2p_rx_action((pDev),(data), (len), (abf), (wh), (category)))

#define p2p_probe_req_rx(pDev, addr, ie, len) \
    P2P_FW_API_FUNC(_p2p_probe_req_rx((pDev), (addr), (ie), (len)))

#define p2p_find(pDev, wmiCmdBuf) \
    P2P_FW_API_FUNC(_p2p_find((pDev), (wmiCmdBuf)))

#define p2p_listen(pDev, wmiCmdBuf) \
    P2P_FW_API_FUNC(_p2p_listen((pDev), (wmiCmdBuf)))

#define p2p_stop_find(pDev) \
    P2P_FW_API_FUNC(_p2p_stop_find((pDev)))

#define p2p_set_config(pDev, wmiCmdBuf) \
    P2P_FW_API_FUNC(_p2p_set_config((pDev), (wmiCmdBuf)))

#define p2p_rx_beacon_probe(pDev,ies, ie_len) \
    P2P_FW_API_FUNC(_p2p_rx_beacon_probe((pDev),(ies), (ie_len)))

#define p2p_get_assoc_ie_len(pDev) \
    P2P_FW_API_FUNC(_p2p_get_assoc_ie_len((pDev)))

#define p2p_add_assoc_req_ie(pDev, frm) \
    P2P_FW_API_FUNC(_p2p_add_assoc_req_ie((pDev), (frm)))

#define p2p_add_probe_req_ie(pDev, frm,len) \
    P2P_FW_API_FUNC(_p2p_add_probe_req_ie((pDev), (frm),(len)))

#define p2p_is_listening(pDev) \
    P2P_FW_API_FUNC(_p2p_is_listening((pDev)))

#define p2p_scan_ie(pDev,buf) \
    P2P_FW_API_FUNC(_p2p_scan_ie((pDev),(buf)))

#define p2p_dev_init(pDev) \
    P2P_FW_API_FUNC(_p2p_dev_init((pDev)))

#define p2p_init(pDev,ifaddr) \
    P2P_FW_API_FUNC(_p2p_init((pDev),(ifaddr)))

#define p2p_parse_scanlist(dev,ftype,hdr,data,len) \
    P2P_FW_API_FUNC(_p2p_parse_scanlist((dev),(ftype),(hdr),(data),(len)))

#define p2p_send_p2p_list(dev) \
    P2P_FW_API_FUNC(_p2p_send_p2p_list((dev)))

#define p2p_begin_go_neg(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_begin_go_neg((dev),(cmdbuf)))

#define p2p_auth_go_neg(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_auth_go_neg((dev),(cmdbuf)))

#define p2p_set_cmd(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_set_cmd((dev),(cmdbuf)))

#define p2p_invite(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_invite((dev),(cmdbuf)))

#define p2p_cancel(dev) \
    P2P_FW_API_FUNC(_p2p_cancel((dev)))

#define p2p_grp_formation_done(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_grp_formation_done((dev),(cmdbuf)))

#define p2p_auth_invite(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_auth_invite((dev),(cmdbuf)))

#define p2p_join_profile(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_join_profile((dev),(cmdbuf)))

#define p2p_profile_enable(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_profile_enable((dev),(cmdbuf)))

#define p2p_read_join_profile(dev,cmdbuf) \
    P2P_FW_API_FUNC(_p2p_read_join_profile((dev),(cmdbuf)))

#define p2p_is_valid_p2p_ie_present(dev,buf,subtype) \
    P2P_FW_API_FUNC(_p2p_is_valid_p2p_ie_present((dev),(buf),(subtype)))

#define p2p_go_init(dev) \
    P2P_FW_API_FUNC(_p2p_go_init((dev)))

#define p2p_go_handle_powersave(dev,mac) \
    P2P_FW_API_FUNC(_p2p_go_handle_powersave((dev),(mac)))
#ifdef AR6002_REV72
void  _P2PLite_Parse(P2P_DEVICE_LITE *p2pDevLite,struct p2p_device *p2pDev);
WMI_FILTER_ACTION _p2p_event_filter(void* pCxt, A_UINT16 EventId, A_UINT16 info,A_UINT8 *pBuffer,int length);
#define P2PLite_Parse(p2pDevLite,p2pDev) \
        _P2PLite_Parse((p2pDevLite),(p2pDev))

extern A_BOOL _p2p_go_is_cck_rates_enabled(devh_t *dev);
#if !defined(CONFIG_HOSTLESS) && defined(IOT_BUILD_FLAG)
#define p2p_go_is_cck_rates_enabled(dev) \
       _p2p_go_is_cck_rates_enabled((dev))
#else

#define p2p_go_is_cck_rates_enabled(dev) (0)

#endif

#else
#define P2PLite_Parse(p2pDevLite,p2pDev) \
         P2P_FW_API_FUNC(_P2PLite_Parse((p2pDevLite),(p2pDev)))


#define p2p_go_is_cck_rates_enabled(dev)\
        P2P_FW_API_FUNC(_p2p_go_is_cck_rates_enabled(dev))
#endif
#ifdef CONFIG_P2P_FW_OFFLOAD

#undef P2P_AddNoa
#undef P2P_DelNoa
#undef P2P_GetSchNextTimeout
#undef P2P_Send_Noa_Action

#ifdef AR6002_REV72

void _p2p_grp_formation_persistent(devh_t *dev);
void _p2p_copy_p2p_credentials(devh_t *dev);
void _p2p_get_peer_dev_addr(devh_t *dev, struct ath_buf *abf, A_UINT16 subtype);
void _p2p_offload_connect_handler_concurrent(devh_t *dev, bss_t *bss);
void _p2p_offload_disconnect_handler_concurrent(devh_t *dev);
void _p2p_fw_list_persistent_prof_cmd(devh_t *dev);
A_UINT8 _p2p_is_channel_valid(devh_t *dev, A_UINT8 channelNum);
void _p2p_fw_go_timeout(A_HANDLE alarm, void *data);
void _p2p_write_dset_CB(A_STATUS status, void *arg);
void _p2p_write_dset_SM(A_STATUS status, void *arg);
A_STATUS _storerecall_fill_p2p_snapshot(devh_t *dev);
void _storerecall_recall_p2p(devh_t *dev);

#define p2p_fw_list_persistent_prof_cmd(dev) \
        _p2p_fw_list_persistent_prof_cmd((dev))

#define p2p_grp_formation_persistent(dev) \
        _p2p_grp_formation_persistent((dev))

#define p2p_copy_p2p_credentials(dev) \
        _p2p_copy_p2p_credentials((dev))

#define p2p_get_peer_dev_addr(dev,abf,subtype) \
        _p2p_get_peer_dev_addr((dev),(abf),(subtype))

#define p2p_offload_connect_handler_concurrent(dev, bss) \
        _p2p_offload_connect_handler_concurrent((dev), (bss))

#define p2p_offload_disconnect_handler_concurrent(dev) \
        _p2p_offload_disconnect_handler_concurrent((dev))

#define p2p_is_channel_valid(dev, channelNum) \
        _p2p_is_channel_valid((dev), (channelNum))

#define p2p_fw_go_timeout(alarm, data) \
	_p2p_fw_go_timeout((alarm), (data))

#define p2p_write_dset_CB(status, arg) \
    _p2p_write_dset_CB((status), (arg))

#define p2p_write_dset_SM(status, arg) \
        _p2p_write_dset_SM((status), (arg))

#define storerecall_fill_p2p_snapshot(dev) \
        _storerecall_fill_p2p_snapshot((dev))

#define storerecall_recall_p2p(dev) \
        _storerecall_recall_p2p((dev))


#else

#undef P2P_Noa_Action_Send_Comp 

#define p2p_fw_list_persistent_prof_cmd(dev) \
    P2P_FW_API_FUNC(_p2p_fw_list_persistent_prof_cmd((dev)))

#define p2p_grp_formation_persistent(dev) \
        P2P_FW_API_FUNC(_p2p_grp_formation_persistent((dev)))

#define p2p_copy_p2p_credentials(dev) \
    P2P_FW_API_FUNC(_p2p_copy_p2p_credentials((dev)))

#define p2p_get_peer_dev_addr(dev, abf, subtype)        \
    P2P_FW_API_FUNC(_p2p_get_peer_dev_addr((dev), (abf), (subtype)))

#define p2p_offload_connect_handler_concurrent(dev, bss) \
   P2P_FW_API_FUNC(_p2p_offload_connect_handler_concurrent((dev), (bss)))

#define p2p_offload_disconnect_handler_concurrent(dev) \
   P2P_FW_API_FUNC(_p2p_offload_disconnect_handler_concurrent((dev)))

#define p2p_strrcl_wakeupmgr_cb(calltype, priv_data) \
   P2P_FW_API_FUNC(_p2p_strrcl_wakeupmgr_cb((calltype), (priv_data)))

#define p2p_strrcl_wakeupmgr_init(dev) \
   P2P_FW_API_FUNC(_p2p_strrcl_wakeupmgr_init)((dev))

#define storerecall_fill_p2p_snapshot(dev) \
    P2P_FW_API_FUNC(_storerecall_fill_p2p_snapshot((dev)))

#define storerecall_recall_p2p(dev) \
    P2P_FW_API_FUNC(_storerecall_recall_p2p((dev)))

#define p2p_is_channel_valid(dev, channelNum) \
        P2P_FW_API_FUNC(_p2p_is_channel_valid((dev), (channelNum)))

#define p2p_fw_go_timeout(alarm, data) \
	P2P_FW_API_FUNC(_p2p_fw_go_timeout((alarm), (data)))

#define p2p_write_dset_CB(status, arg) \
    P2P_FW_API_FUNC(_p2p_write_dset_CB((status), (arg)))

#define p2p_write_dset_SM(status, arg) \
    P2P_FW_API_FUNC(_p2p_write_dset_SM((status), (arg)))


#endif

#endif
#define p2p_prov_disc_req(dev,cmdbuf) \
    P2P_FW_PD_FUNC(_p2p_prov_disc_req((dev),(cmdbuf)))

#define p2p_sdpd_tx_cmd(dev,cmdbuf) \
    P2P_FW_SD_FUNC(_p2p_sdpd_tx_cmd((dev),(cmdbuf)))

#define p2p_stop_sdpd(dev) \
    P2P_FW_SD_FUNC(_p2p_stop_sdpd((dev)))

#define p2p_group_update_ies(dev) \
    P2P_FW_GO_FUNC(_p2p_group_update_ies((dev)))

#define p2p_grp_init(dev,buf) \
    P2P_FW_GO_FUNC(_p2p_grp_init((dev),(buf)))

#define p2p_grp_add_beacon_probe_ie(dev,frm,type,buf) \
    P2P_FW_GO_FUNC(_p2p_grp_add_beacon_probe_ie((dev),(frm),(type),(buf)))

#define p2p_go_add_assoc_resp_ie(dev,frm,status) \
    P2P_FW_GO_FUNC(_p2p_go_add_assoc_resp_ie((dev),(frm),(status)))

#define p2p_go_assoc_event_rx(dev,conn,buf,buflen) \
    P2P_FW_GO_FUNC(_p2p_go_assoc_event_rx((dev),(conn),(buf),(buflen)))

#define p2p_go_disassoc_event_rx(dev,conn) \
    P2P_FW_GO_FUNC(_p2p_go_disassoc_event_rx((dev),(conn)))

#define p2p_go_set_grp_limit(dev,val) \
    P2P_FW_GO_FUNC(_p2p_go_set_grp_limit((dev),(val)))

#define p2p_go_probe_req_rx(dev,addr,ie,len) \
    P2P_FW_GO_FUNC(_p2p_go_probe_req_rx((dev),(addr),(ie),(len)))

#define p2p_group_discover(dev,id) \
    P2P_FW_GO_FUNC(_p2p_group_discover((dev),(id)))

#define p2p_grp_update_beacon_ie(dev,frm) \
    P2P_FW_GO_FUNC(_p2p_grp_update_beacon_ie((dev),(frm)))

#define p2p_go_set_oppPS(dev,buf) \
    P2P_FW_GO_PS_FUNC(_p2p_go_set_oppPS((dev),(buf)))

#define p2p_go_get_oppPS(dev) \
    P2P_FW_GO_PS_FUNC(_p2p_go_get_oppPS((dev)))

#define p2p_go_set_noa(dev,buf) \
    P2P_FW_GO_PS_FUNC(_p2p_go_set_noa((dev),(buf)))

#define p2p_go_get_noa(dev) \
    P2P_FW_GO_PS_FUNC(_p2p_go_get_noa((dev)))

#define p2p_reset_ps(dev) \
    P2P_FW_GO_PS_FUNC(_p2p_reset_ps((dev)))

#define p2p_go_checkallsta_sleep(dev) \
    P2P_FW_GO_PS_FUNC(_p2p_go_checkallsta_sleep((dev)))

#define p2p_fw_add_noa_ie(dev,frm,noa) \
    P2P_FW_GO_PS_FUNC(_p2p_fw_add_noa_ie((dev),(frm),(noa)))

#define p2p_notify_beacon_comp(dev) \
    P2P_FW_GO_PS_FUNC(_p2p_notify_beacon_comp((dev)))

#define p2p_get_noa_len(dev,noa) \
    P2P_FW_GO_PS_FUNC(_p2p_get_noa_len((dev),(noa)))

#else
///////////Supports legacy Venus P2P based firmware implementation
typedef struct p2p_api_fn_t {

void (*_p2p_init)(devh_t *pdev, A_UINT8 *myAddr);
#ifdef AR6002_REV43
void (*_p2p_deinit)(devh_t *dev);
#endif /* AR6002_REV43 */
A_STATUS (*_p2p_rx_action)(devh_t *dev, struct ath_buf *, const A_UINT8 *da,
           const A_UINT8 *sa, const A_UINT8 *bssid, A_UINT8 category,
           const A_UINT8 *data, A_UINT32 len);
A_INT8 (*_p2p_probe_req_rx)(devh_t *dev, const A_UINT8 *addr, const A_UINT8 *ie,
             A_UINT32 ie_len);
#ifdef AR6002_REV43
int (*_p2p_scan_res_handler)(devh_t *dev, const A_UINT8 *bssid, A_UINT32 freq,
             const A_UINT8 *ies, A_UINT32 ie_len);
#endif /* AR6002_REV43 */
void (*_p2p_wps_reg_success)(devh_t *dev, const A_UINT8 *macAddr,
             const A_UINT8 *uuid);
A_STATUS (*_p2p_go_neg_start)(devh_t *dev, WMI_P2P_GO_NEG_START_CMD *);
void (*_p2p_find)(devh_t *dev, WMI_P2P_FIND_CMD *);
void (*_p2p_listen)(devh_t *dev, WMI_P2P_LISTEN_CMD *);
void (*_p2p_stop_find)(devh_t *dev);
void (*_p2p_wps_set_config)(devh_t *dev, WMI_WPS_SET_CONFIG_CMD *);
void (*_p2p_set_config)(devh_t *dev, WMI_P2P_SET_CONFIG_CMD *);
void (*_p2p_set_req_dev_attr)(devh_t *dev, WMI_SET_REQ_DEV_ATTR_CMD *);
A_UINT32 (*_p2p_add_probereq_ies)(devh_t *dev, A_UINT8 *);
} P2P_API_FN;
#endif //#ifndef AR6002_REV43
/*****************************************************************************/
/* MACRO REDIRECTION DEFINITION START */
/*****************************************************************************/   

/* these macros allow us to avoid spewing #ifdef's all over the code base */

#define P2PLITE_PARSE(p2pDevLite,p2pDev)                 P2PLite_Parse(p2pDevLite,p2pDev)
#ifdef CONFIG_P2P_FW_OFFLOAD
#define P2P_RX_ACTION(pDev,data,len,abf, wh, category)    p2p_rx_action((pDev),(data),(len),(abf), (wh), (category))
#define P2P_PROBE_REQ_RX(pDev, addr, ie, len)             p2p_probe_req_rx((pDev),(addr),( ie),( len))
#define P2P_FIND(pDev, wmiCmdBuf)                         p2p_find((pDev), (wmiCmdBuf))
#define P2P_LISTEN(pDev, wmiCmdBuf)                       p2p_listen((pDev),(wmiCmdBuf))
#define P2P_STOP_FIND(pDev)                               p2p_stop_find((pDev))
#define P2P_SET_CONFIG(pDev, wmiCmdBuf)                   p2p_set_config((pDev), (wmiCmdBuf))
#define P2P_RX_BEACON_PROBE(pDev,ies, ie_len)             p2p_rx_beacon_probe((pDev),(ies),(ie_len))
#define P2P_GET_ASSOC_IE_LEN(pDev)                        p2p_get_assoc_ie_len((pDev))
#define P2P_ADD_ASSOC_REQ_IE(pDev, frm)                   p2p_add_assoc_req_ie((pDev),(frm))
#define P2P_ADD_PROBE_REQ_IE(pDev, frm,len)               p2p_add_probe_req_ie((pDev),(frm),(len))
#define P2P_IS_LISTENING(pDev)                            p2p_is_listening((pDev))
#define P2P_SCAN_IE(pDev,buf)                             p2p_scan_ie((pDev),(buf))
#define P2P_DEV_INIT(pDev)                                p2p_dev_init((pDev))
#define P2P_INIT(pDev,ifaddr)                             p2p_init((pDev),(ifaddr))
#define P2P_PARSE_SCANLIST(dev,ftype,hdr,data,len)        p2p_parse_scanlist((dev),(ftype),(hdr),(data),(len))
#define P2P_SEND_P2P_LIST(dev)                            p2p_send_p2p_list((dev))
#define P2P_BEGIN_GO_NEG(dev,cmdbuf)                      p2p_begin_go_neg((dev),(cmdbuf))
#define P2P_AUTH_GO_NEG(dev,cmdbuf)                       p2p_auth_go_neg((dev),(cmdbuf))
#define P2P_SET_CMD(dev,cmdbuf)                           p2p_set_cmd((dev),(cmdbuf))
#define P2P_INVITE(dev,cmdbuf)                            p2p_invite((dev),(cmdbuf))
#define P2P_CANCEL(dev)                                   p2p_cancel((dev))
#define P2P_PROV_DISC_REQ(dev,cmdbuf)                     p2p_prov_disc_req((dev),(cmdbuf))
#define P2P_SDPD_TX_CMD(dev,cmdbuf)                       p2p_sdpd_tx_cmd((dev),(cmdbuf))
#define P2P_STOP_SDPD(dev)                                p2p_stop_sdpd((dev))
#define P2P_AUTH_INVITE(dev,cmdbuf)                       p2p_auth_invite((dev),(cmdbuf))
#define P2P_JOIN_PROFILE(dev,cmdbuf)                      p2p_join_profile((dev),(cmdbuf))
#define P2P_READ_JOIN_PROFILE(dev,cmdbuf)                 p2p_read_join_profile((dev),(cmdbuf))
#define P2P_GRP_INIT(dev,cmdbuf)                          p2p_grp_init((dev),(cmdbuf))
#define P2P_GO_SET_NOA(dev,cmdbuf)                        p2p_go_set_noa((dev),(cmdbuf))
#define P2P_GO_GET_NOA(dev)                               p2p_go_get_noa((dev))
#define P2P_GRP_FORMATION_DONE(dev,buf)                   p2p_grp_formation_done((dev),(buf))
#define P2P_IS_VALID_P2P_IE_PRESENT(dev,buf,type)         p2p_is_valid_p2p_ie_present((dev),(buf),(type))
#define P2P_GO_HANDLE_POWERSAVE(dev,mac)                  p2p_go_handle_powersave((dev),(mac))
#define P2P_GO_SET_OPPPS(dev,buf)                         p2p_go_set_oppPS((dev),(buf))
#define P2P_GO_GET_OPPPS(dev)                             p2p_go_get_oppPS((dev))
#define P2P_RESET_PS(dev)                                 p2p_reset_ps((dev))
#define P2P_GO_CHECKALLSTA_SLEEP(dev)                     p2p_go_checkallsta_sleep((dev))
#define P2P_FW_ADD_NOA_IE(dev,frm,noa)                    p2p_fw_add_noa_ie((dev),(frm),(noa))
#define P2P_NOTIFY_BEACON_COMP(dev)                       p2p_notify_beacon_comp((dev))
#define P2P_GET_NOA_LEN(dev,noa)                          p2p_get_noa_len((dev),(noa)) 
#define P2P_GROUP_UPDATE_IES(dev)                         p2p_group_update_ies((dev)) 
#define P2P_GRP_ADD_BEACON_PROBE_IE(dev,frm,type,buf)     p2p_grp_add_beacon_probe_ie((dev),(frm),(type),(buf))
#define P2P_GO_ADD_ASSOC_RESP_IE(dev,frm,status)          p2p_go_add_assoc_resp_ie((dev),(frm),(status)) 
#define P2P_GO_ASSOC_EVENT_RX(dev,conn,buf,buflen)        p2p_go_assoc_event_rx((dev),(conn),(buf),(buflen)) 
#define P2P_GO_DISASSOC_EVENT_RX(dev,conn)                p2p_go_disassoc_event_rx((dev),(conn))
#define P2P_GO_SET_GRP_LIMIT(dev,val)                     p2p_go_set_grp_limit((dev),(val)) 
#define P2P_GO_PROBE_REQ_RX(dev,addr,ie,len)              p2p_go_probe_req_rx((dev),(addr),(ie),(len)) 
#define P2P_GROUP_DISCOVER(dev,id)                        p2p_group_discover((dev),(id))
#define P2P_GRP_UPDATE_BEACON_IE(dev,frm)                 p2p_grp_update_beacon_ie((dev),(frm)) 
#define P2P_PROFILE_ENABLE(dev,cmdbuf)                    p2p_profile_enable((dev),(cmdbuf))
#define P2P_LIST_PERSISTENT_NETWORK(dev)                  p2p_fw_list_persistent_prof_cmd((dev))
#define P2P_GRP_FORMATION_PERSISTENT(dev)                 p2p_grp_formation_persistent((dev))
#define P2P_COPY_P2P_CREDENTIALS(dev)	                  p2p_copy_p2p_credentials((dev))
#define P2P_GET_PEER_DEV_ADDR(dev,abf,subtype)            p2p_get_peer_dev_addr((dev), (abf), (subtype))
#define P2P_GO_IS_CCK_RATES_ENABLED(dev)                  p2p_go_is_cck_rates_enabled(dev)
#ifdef AR6002_REV72
#define P2P_AddNoa(pdev, pNoa, type)                      p2p_fw_go_add_noa((pdev), (pNoa), (type))
#define P2P_DelNoa(pdev, index, type)                 	  p2p_fw_go_del_noa((pdev), (index), (type))
#define P2P_Send_Noa_Action(pdev)                         p2p_fw_go_send_noa_action((pdev))
#define P2P_GetSchNextTimeout(dev,next_timeout)           p2p_fw_GetSchNextTimeout((dev),(next_timeout))
#else /* AR6002_REV72 */
#define P2P_GetSchNextTimeout(dev,next_timeout)           P2P_FW_GO_PS_FUNC(_p2p_fw_GetSchNextTimeout((dev),(next_timeout)))
#define P2P_AddNoa(pdev, pNoa, type)                      P2P_FW_GO_PS_FUNC(_p2p_fw_go_add_noa((pdev), (pNoa), (type)))
#define P2P_DelNoa(pdev, index, type)   	              P2P_FW_GO_PS_FUNC(_p2p_fw_go_del_noa((pdev), (index), (type)))
#define P2P_Send_Noa_Action(pdev)                         P2P_FW_GO_PS_FUNC(_p2p_fw_go_send_noa_action((pdev)))
#define p2p_fw_GetSchNextTimeout(dev,next_timeout)        P2P_FW_GO_PS_FUNC(_p2p_fw_GetSchNextTimeout((dev),(next_timeout)))
#define p2p_fw_add_noa_ie_type(dev,frm,noa,type)          P2P_FW_GO_PS_FUNC(_p2p_fw_add_noa_ie_type((dev),(frm),(noa), (type)))
#define P2P_Noa_Action_Send_Comp(dev, status)             P2P_FW_GO_PS_FUNC(_p2p_fw_Noa_Action_Send_Comp((dev), (status)))
#endif /* AR6002_REV72 */
#define p2p_fw_IsIEPresent(dev, buf, type)                P2P_FW_API_FUNC(_p2p_fw_IsIEPresent((dev), (buf), (type)))
#else /* CONFIG_P2P_FW_OFFLOAD */

#define P2P_RX_ACTION(pDev,data,len,abf, wh, category)    A_ERROR 
#define P2P_PROBE_REQ_RX(pDev, addr, ie, len)             (0)
#define P2P_FIND(pDev, wmiCmdBuf)                         
#define P2P_LISTEN(pDev, wmiCmdBuf)                       
#define P2P_STOP_FIND(pDev)                               
#define P2P_SET_CONFIG(pDev, wmiCmdBuf)                   
#define P2P_RX_BEACON_PROBE(pDev,ies, ie_len)              
#define P2P_GET_ASSOC_IE_LEN(pDev)                        (0) 
#define P2P_ADD_ASSOC_REQ_IE(pDev, frm)                   (NULL) 
#define P2P_ADD_PROBE_REQ_IE(pDev, frm,len)                
#define P2P_IS_LISTENING(pDev)                            (0) 
#define P2P_SCAN_IE(pDev,buf)                             
#define P2P_DEV_INIT(pDev)                                 
#define P2P_INIT(pDev,ifaddr)                             
#define P2P_PARSE_SCANLIST(dev,ftype,hdr,data,len)         
#define P2P_SEND_P2P_LIST(dev)                            
#define P2P_BEGIN_GO_NEG(dev,cmdbuf)                       
#define P2P_AUTH_GO_NEG(dev,cmdbuf)                        
#define P2P_SET_CMD(dev,cmdbuf)                           
#define P2P_INVITE(dev,cmdbuf)                             
#define P2P_CANCEL(dev)                                   
#define P2P_PROV_DISC_REQ(dev,cmdbuf)                      
#define P2P_SDPD_TX_CMD(dev,cmdbuf)                       
#define P2P_STOP_SDPD(dev)                                
#define P2P_AUTH_INVITE(dev,cmdbuf)                    
#define P2P_GRP_INIT(dev,cmdbuf)                          
#define P2P_GO_SET_NOA(dev,cmdbuf)                        
#define P2P_GO_GET_NOA(dev)                                  
#define P2P_GRP_FORMATION_DONE(dev,buf)                    (0)
#define P2P_JOIN_PROFILE(dev,cmdbuf)                         
#define P2P_READ_JOIN_PROFILE(dev,cmdbuf)                  (A_ERROR)
#define P2P_GO_HANDLE_POWERSAVE(dev,mac)                   (FALSE)
#define P2P_GO_ADD_ASSOC_RESP_IE(dev,frm,status)           (NULL)
#define P2P_IS_VALID_P2P_IE_PRESENT(dev,buf,type)          (FALSE)
#define P2P_GO_SET_OPPPS(dev,buf)                           
#define P2P_GO_GET_OPPPS(dev)                               
#define P2P_RESET_PS(dev)                                 
#define P2P_GO_CHECKALLSTA_SLEEP(dev)                     
#define P2P_FW_ADD_NOA_IE(dev,frm,noa)                     (NULL)  
#define P2P_NOTIFY_BEACON_COMP(dev)                       
#define P2P_GET_NOA_LEN(dev,noa)                           (0) 
#define P2P_GROUP_UPDATE_IES(dev)                          
#define P2P_GRP_ADD_BEACON_PROBE_IE(dev,frm,type,buf)      (NULL) 
#define P2P_GO_ASSOC_EVENT_RX(dev,conn,buf,buflen)         
#define P2P_GO_DISASSOC_EVENT_RX(dev,conn)                
#define P2P_GO_SET_GRP_LIMIT(dev,val)                      
#define P2P_GO_PROBE_REQ_RX(dev,addr,ie,len)               (0) 
#define P2P_GROUP_DISCOVER(dev,id)                         (FALSE) 
#define P2P_GRP_UPDATE_BEACON_IE(dev,frm)                   
#define P2P_PROFILE_ENABLE(dev,cmdbuf)                  
#define P2P_GO_IS_CCK_RATES_ENABLED(dev)                    (0)
#define P2P_LIST_PERSISTENT_NETWORK(dev)                  
#define P2P_GRP_FORMATION_PERSISTENT(dev)                 
#define P2P_COPY_P2P_CREDENTIALS(dev)	                  
#define P2P_GET_PEER_DEV_ADDR(dev,abf,subtype)            
#define p2p_fw_IsIEPresent(dev, buf, type)             
#endif /* CONFIG_P2P_FW_OFFLOAD */

/*****************************************************************************/
/* MACRO REDIRECTION DEFINITION END */
/*****************************************************************************/
#endif /* __P2P_API_H__ */
