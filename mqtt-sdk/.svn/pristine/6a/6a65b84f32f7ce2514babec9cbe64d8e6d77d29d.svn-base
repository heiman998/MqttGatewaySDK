/*  * Copyright (c) 2013 Qualcomm Atheros, Inc..  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
#if defined(P2P_ENABLED)
#include "qcom/qcom_common.h"
#include "qcom/qcom_internal.h"
#include "qcom/qcom_ap.h"
#include "qcom/qcom_sec.h"
#include "qcom/qcom_wps.h"

#include "qcom_p2p_internal.h"
#include "qcom/qcom_wps_priv.h"
#include "p2p.h"

/* Since threadx allows only a max message size of 64 bytes, the tx_queue_* APIs
 * cannot be used to pass WMI events > 64 bytes. Instead use the APIs to pass on
 * the info that a WMI event has been received and use the p2p_msg_area_ptr for the threads to
 * read/write info. Note that the msg area needs to be allocated prior to using a WMI
 * command and should be freed later on
 *
 * No need for locking the message area. Race
 * condition between application threads is prevented by having a check on
 * tx_queue_create. If queue is already created (and in use), API will
 * return an error
 */
TX_QUEUE p2p_msg_queue;
A_UINT8 *p2p_msg_area_ptr = NULL;
qcom_p2p_event_handler p2p_event_handler = NULL;
void * p2p_event_handler_arg;

A_STATUS _qcom_p2p_func_find(A_UINT8 device_id, P2P_DISC_TYPE type, A_UINT32 timeout)
{
    return qcom_p2p_find(device_id, NULL, type, timeout);
}

A_STATUS _qcom_p2p_func_stop_find(A_UINT8 device_id)
{
    return qcom_p2p_stop_find(device_id, NULL);
}

A_STATUS _qcom_p2p_func_set(A_UINT8 device_id, P2P_CONF_ID config_id, void *data, A_UINT32 data_length)
{
    return qcom_p2p_set(device_id, NULL, config_id, data, data_length);
}

P2P_INV_ROLE _qcom_p2p_func_get_role(A_UINT32 role)
{
    switch (role) {
        case WMI_P2P_INVITE_ROLE_GO:
            return P2P_INV_ROLE_GO;
        case WMI_P2P_INVITE_ROLE_ACTIVE_GO:
            return P2P_INV_ROLE_ACTIVE_GO;
        case WMI_P2P_INVITE_ROLE_CLIENT:
            return P2P_INV_ROLE_CLIENT;
        default:
            return P2P_INV_ROLE_CLIENT;
    }
}

A_STATUS _qcom_p2p_func_get_network_list(A_UINT8 device_id, P2P_PERSISTENT_MAC_LIST *app_buf, A_UINT32 buf_size)
{
    A_STATUS status = A_ERROR;
    A_UINT32 queue_status;
    if ((app_buf == NULL) || (buf_size < P2P_NETWORK_LIST_SIZE)) {
        return A_ERROR;
    }

    /* Allocate queue message area */
    if ((p2p_msg_area_ptr = (A_UINT8 *)mem_alloc(P2P_NETWORK_LIST_SIZE)) == NULL) {
        return A_NO_MEMORY;
    }

    if (tx_queue_create(&p2p_msg_queue, "p2p msg queue", sizeof(queue_status)/4, &queue_status, sizeof(queue_status)) != TX_SUCCESS) {
        mem_free(p2p_msg_area_ptr);
        p2p_msg_area_ptr = NULL;
        return A_ERROR;
    }
    if (qcom_p2p_list_network(device_id, NULL) == A_OK) {
        /* Wait for WMI event */
        if (tx_queue_receive(&p2p_msg_queue, &queue_status, (XT_TICK_PER_SEC)*5) == TX_SUCCESS) {
            if (queue_status) {
                /* Copy from queue area to application buffer */
                WMI_PERSISTENT_MAC_LIST *wmi_evt = (WMI_PERSISTENT_MAC_LIST *)p2p_msg_area_ptr;
                A_MEMCPY(app_buf, wmi_evt, P2P_NETWORK_LIST_SIZE);
                status = A_OK;
            }
        }
    }

    if (tx_queue_delete(&p2p_msg_queue) != TX_SUCCESS) {
        status = A_ERROR;
    }
    A_MEMZERO(&p2p_msg_queue, sizeof(p2p_msg_queue));
    mem_free(p2p_msg_area_ptr);
    p2p_msg_area_ptr = NULL;

    return status;
}

A_STATUS _qcom_p2p_func_get_node_list(A_UINT8 device_id, P2P_NODE_LIST_EVENT *app_buf, A_UINT32 buf_size)
{
    A_STATUS status = A_ERROR;
    A_UINT32 queue_status;

    if ((app_buf == NULL) || (buf_size < NODELIST_MSG_BYTES)) {
        return A_ERROR;
    }

    /* Allocate queue message area */
    if ((p2p_msg_area_ptr = (A_UINT8 *)mem_alloc(NODELIST_MSG_BYTES)) == NULL) {
        return A_NO_MEMORY;
    }

    if ((status = tx_queue_create(&p2p_msg_queue, "p2p msg queue", sizeof(queue_status)/4, &queue_status, sizeof(queue_status))) != TX_SUCCESS) {
        mem_free(p2p_msg_area_ptr);
        p2p_msg_area_ptr = NULL;
        return A_ERROR;
    }

    if (qcom_p2p_get_node_list(device_id, NULL) == A_OK) {
        /* Wait for WMI event */
        if (tx_queue_receive(&p2p_msg_queue, &queue_status, (XT_TICK_PER_SEC)*5) == TX_SUCCESS) {
            if (queue_status) {
                /* Copy from queue area to application buffer */
                WMI_P2P_NODE_LIST_EVENT *wmi_evt = (WMI_P2P_NODE_LIST_EVENT *)p2p_msg_area_ptr;
                A_UINT32 ev_len = sizeof(WMI_P2P_NODE_LIST_EVENT) + (wmi_evt->num_p2p_dev * sizeof(P2P_DEVICE_LITE));
                A_MEMCPY((A_UINT8 *)app_buf, p2p_msg_area_ptr, ev_len);
                status = A_OK;
            }
        }
    }

    tx_queue_delete(&p2p_msg_queue);
    A_MEMZERO(&p2p_msg_queue, sizeof(p2p_msg_queue));
    mem_free(p2p_msg_area_ptr);
    p2p_msg_area_ptr = NULL;
    return status;
}

A_STATUS _qcom_p2p_func_invite_auth(A_UINT8 device_id, P2P_FW_INVITE_REQ_RSP_CMD *inv)
{
    return qcom_p2p_invite_auth(device_id, NULL, inv);
}

enum p2p_wps_method qcom_p2p_get_wps(P2P_WPS_METHOD wps)
{
    switch (wps) {
        case P2P_WPS_NOT_READY:
            return WPS_NOT_READY;
        case P2P_WPS_PIN_LABEL:
            return WPS_PIN_LABEL;
        case P2P_WPS_PIN_DISPLAY:
            return WPS_PIN_DISPLAY;
        case P2P_WPS_PIN_KEYPAD:
            return WPS_PIN_KEYPAD;
        case P2P_WPS_PBC:
            return WPS_PBC;
        default:
            return WPS_NOT_READY;
    }
}

A_STATUS _qcom_p2p_func_start_go(A_UINT8 device_id, A_UINT8 *pssid, A_UINT8 *ppass, A_INT32 channel, A_BOOL ucpersistent)
{
    if ((pssid == NULL) || (ppass == NULL)) {
        return A_ERROR;
    }

    qcom_p2p_group_init(device_id, NULL, ucpersistent, 1);

    if (A_OK != qcom_ap_set_max_station_number(device_id, 4)) {
        return A_ERROR;
    }

    qcom_sec_set_auth_mode(device_id, WLAN_AUTH_WPA2_PSK);
    qcom_sec_set_encrypt_mode(device_id, WLAN_CRYPT_AES_CRYPT);
    qcom_sec_set_passphrase(device_id, (A_CHAR *)ppass);
    gDeviceContextPtr[device_id]->wpsFlag = 1;
    gDeviceContextPtr[device_id]->chipMode = QCOM_WLAN_DEV_MODE_AP;
    qcom_ap_set_flag(device_id, CONNECT_WPS_FLAG | CONNECT_DO_WPA_OFFLOAD);
    qcom_set_channel(device_id, channel);
    qcom_ap_hidden_mode_enable(device_id, FALSE);
    qcom_ap_start(device_id, (A_CHAR *)pssid);
    qcom_set_power_mode(device_id, MAX_PERF_POWER);

    return A_OK;
}

A_STATUS _qcom_p2p_func_init(A_UINT8 device_id, A_INT32 enable)
{
    static A_INT32 p2p_enable_flag = 0xFEFE;

    if (enable == p2p_enable_flag) {
        return A_OK;
    }
    p2p_enable_flag = enable;
  
    qcom_wps_enable(device_id, enable);
    qcom_p2p_event_register();
    qcom_p2p_init(device_id);
    qcom_p2p_enable(device_id, NULL, enable);

    qcom_p2p_func_set_config(device_id, 0, 1, 1, 3000, 81, 81, 5);

    return A_OK;
}

A_STATUS _qcom_p2p_func_cancel(A_UINT8 device_id)
{
    extern A_UINT32 qcom_wps_connect_flag[WLAN_NUM_OF_DEVICES];

    qcom_wps_connect_flag[device_id] = 0;
    gDeviceContextPtr[device_id]->forceDisc = 1;

    if (qcom_sta_wlan_reconnect_start_flag[device_id] == 1) {
        qcom_sta_reconnect_stop(device_id);
    }

    qcom_p2p_cancel(device_id, NULL);

    /* FIXME: This is done for WMI synchronization */
    qcom_p2p_list_network(device_id, NULL);

    if (2 == g_wifi_state[device_id]) {
        qcom_wlan_disconnect(device_id);
    }

    return A_OK;
}

A_STATUS _qcom_p2p_func_auth(A_UINT8 device_id, A_INT32 dev_auth, P2P_WPS_METHOD wps_method, 
        A_UINT8 *peer_mac, A_BOOL persistent)
{
    P2P_FW_CONNECT_CMD_STRUCT p2p_connect;

    if (NULL == peer_mac)
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2p_connect, sizeof(P2P_FW_CONNECT_CMD_STRUCT));
    p2p_connect.dialog_token = 1;
    p2p_connect.wps_method = qcom_p2p_get_wps(wps_method);
    /* If go_intent <= 0, wlan firmware will use the intent value configured via
     * qcom_p2p_set
     */
    p2p_connect.go_intent = 0;
    if (persistent) {
        p2p_connect.dev_capab  |= P2P_PERSISTENT_FLAG;
    }
    A_MEMCPY(p2p_connect.peer_addr, peer_mac, ATH_MAC_LEN);
    p2p_connect.dev_auth = dev_auth;
    qcom_p2p_auth(device_id, NULL, &p2p_connect);

    return A_OK;
}

A_STATUS _qcom_p2p_func_connect(A_UINT8 device_id, P2P_WPS_METHOD wps_method, 
        A_UINT8 *peer_mac, A_BOOL persistent)
{
    A_STATUS ret = A_OK;
    P2P_FW_CONNECT_CMD_STRUCT p2p_connect;

    if (NULL == peer_mac)
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2p_connect, sizeof(P2P_FW_CONNECT_CMD_STRUCT));
    p2p_connect.dialog_token = 1;
    p2p_connect.wps_method = qcom_p2p_get_wps(wps_method);
    /* If go_intent <= 0, wlan firmware will use the intent value configured via
     * qcom_p2p_set
     */
    p2p_connect.go_intent = 0;
    p2p_connect.dev_capab  = 0x23;
    if (persistent) {
        p2p_connect.dev_capab  |= P2P_PERSISTENT_FLAG;
    }

    A_MEMCPY(p2p_connect.peer_addr, peer_mac, ATH_MAC_LEN);
    qcom_p2p_connect(device_id, NULL, &p2p_connect);

    return ret;
}

A_STATUS _qcom_p2p_func_invite(A_UINT8 device_id, A_CHAR *pssid, P2P_WPS_METHOD wps_method, 
                            A_UINT8 *pmac, A_BOOL persistent, P2P_INV_ROLE role)
{
    P2P_FW_INVITE_CMD stinvite_cmd;

    if (NULL == pmac) {
        return A_ERROR;
    }

    if (!persistent) {
        if (gDeviceContextPtr[device_id]->chipMode == QCOM_WLAN_DEV_MODE_AP) {
            role = P2P_INV_ROLE_ACTIVE_GO;
        }
        else{
            role = P2P_INV_ROLE_CLIENT; 
        }
    }

    A_MEMZERO(&stinvite_cmd, sizeof(stinvite_cmd));
    stinvite_cmd.ssid.ssidLength = A_STRLEN(pssid);
    stinvite_cmd.ssid.ssidLength = (stinvite_cmd.ssid.ssidLength < MAX_SSID_LEN)? \
                                                 stinvite_cmd.ssid.ssidLength : (MAX_SSID_LEN - 1);
    A_STRCPY((A_CHAR *)(stinvite_cmd.ssid.ssid), pssid);
    stinvite_cmd.role = role;
    stinvite_cmd.wps_method = qcom_p2p_get_wps(wps_method);
    stinvite_cmd.is_persistent = persistent;
    stinvite_cmd.dialog_token  = 1;
    A_MEMCPY(stinvite_cmd.peer_addr, pmac, 6);
    #if 0
    A_PRINTF("%x:%x:%x:%x:%x:%x\n", \
                    stinvite_cmd.peer_addr[0], stinvite_cmd.peer_addr[1], \
                    stinvite_cmd.peer_addr[2], stinvite_cmd.peer_addr[3], \
                    stinvite_cmd.peer_addr[4], stinvite_cmd.peer_addr[5]);
    #endif

    qcom_set_ssid(device_id, pssid);
    qcom_power_set_mode(device_id, MAX_PERF_POWER);
    qcom_p2p_invite(device_id, NULL, &stinvite_cmd);

    return A_OK;
}

A_STATUS _qcom_p2p_func_join(A_UINT8 device_id, P2P_WPS_METHOD wps_method,
                            A_UINT8 *pmac, char *ppin)
{
    int idata;
    P2P_CONF_ID enconfig_id;
    P2P_JOIN_CMD_STRUCT stjoin_cmd;

    if (NULL == pmac) {
        return A_ERROR;
    }

    /*
    * For concurrency SCC mode:
    * 1) If dev1 has connected to an AP and
    * 2) If the AP's channel is different with GO's operating channel
    * Dev0 will not join that GO.
    */
    if (TRUE == qcom_p2p_check_scc_mode()) {
        A_UINT16 usap_ch_freq;
        A_UINT16 usgo_oper_freq;
        if (A_OK != qcom_p2p_get_peer_go_op_ch_freq(device_id, pmac, &usgo_oper_freq)) {
            return A_ERROR;
        }

        /* 
        * used by the other device to check if both of them have same channel in SCC mode. 
        */
        if (A_OK != qcom_p2p_func_set_channel_freq_cfg(device_id, usgo_oper_freq)) {
            return A_ERROR;
        }

        qcom_get_channel_freq(((device_id+1)%(WLAN_NUM_OF_DEVICES)), &usap_ch_freq);
        
        if ((0 == usgo_oper_freq) ||
            ((2 == g_wifi_state[(device_id+1)%(WLAN_NUM_OF_DEVICES)]) && (usgo_oper_freq != usap_ch_freq))) {
            return A_ERROR;
        }
    }

    enconfig_id = P2P_CONFIG_P2P_OPMODE;
    idata = P2P_CLIENT;
    qcom_p2p_set(device_id, NULL, enconfig_id, &idata, 4);
    qcom_p2p_join_profile(device_id, NULL, pmac);

    A_MEMZERO(&stjoin_cmd, sizeof(stjoin_cmd));
    stjoin_cmd.info.wps_method = qcom_p2p_get_wps(wps_method);

    if ((WPS_PIN_DISPLAY == wps_method) || \
        (WPS_PIN_KEYPAD == wps_method)) {
        if ((ppin == NULL) || (A_STRLEN(ppin) == 0)) {
            return A_ERROR;
        }

        A_STRCPY((char *)(stjoin_cmd.wps_pin), ppin);
    }

    qcom_power_set_mode(device_id, MAX_PERF_POWER);

    /* disable backgroud scan */
    qcom_scan_set_mode(device_id, 0);
    qcom_p2p_join(device_id, NULL, &stjoin_cmd);

    return A_OK;
}

A_STATUS _qcom_p2p_func_join_profile(A_UINT8 device_id, A_UINT8 *pmac)
{
    if (NULL == pmac) {
        return A_ERROR;
    }

    qcom_p2p_join_profile(device_id, NULL, pmac);

    return A_OK;
}

A_STATUS _qcom_p2p_func_prov(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *pmac)
{
    P2P_FW_PROV_DISC_REQ_CMD stp2p_prov_cmd;

    if ((wps_method > WPS_PBC) ||
        (wps_method < WPS_NOT_READY) ||
        (NULL == pmac)) {
        return A_ERROR;
    }

    A_MEMZERO(&stp2p_prov_cmd, sizeof(stp2p_prov_cmd));
    stp2p_prov_cmd.dialog_token = 1;
    stp2p_prov_cmd.wps_method = qcom_p2p_get_wps(wps_method);
    A_MEMCPY(stp2p_prov_cmd.peer, pmac, ATH_MAC_LEN);
    qcom_p2p_prov_disc_req(device_id, NULL, &stp2p_prov_cmd);

    if (WPS_PIN_DISPLAY != wps_method) {
        qcom_p2p_func_auth(device_id, 0, wps_method, pmac, 0);
    }

    return A_OK;
}

A_STATUS _qcom_p2p_func_set_config(A_UINT8 device_id, A_UINT32 uigo_intent, A_UINT32 uiclisten_ch, A_UINT32 uiop_ch, A_UINT32 uiage, A_UINT32 reg_class, A_UINT32 op_reg_class, A_UINT32 max_node_count)
{
    P2P_FW_SET_CONFIG_CMD stp2p_cfg_cmd;

    A_MEMZERO(&stp2p_cfg_cmd, sizeof(stp2p_cfg_cmd));
    stp2p_cfg_cmd.go_intent = uigo_intent;
    stp2p_cfg_cmd.reg_class = reg_class;
    stp2p_cfg_cmd.op_reg_class = op_reg_class;
    stp2p_cfg_cmd.op_channel = uiop_ch;
    stp2p_cfg_cmd.listen_channel = uiclisten_ch;
    stp2p_cfg_cmd.node_age_to    = uiage;
    stp2p_cfg_cmd.max_node_count = max_node_count;
    qcom_p2p_set_config(device_id, NULL, &stp2p_cfg_cmd);

    return A_OK;
}

A_STATUS _qcom_p2p_func_set_channel_freq_cfg(A_UINT8 device_id, A_UINT16 usch_freq)
{
    if (WLAN_NUM_OF_DEVICES <= device_id) {
        return A_ERROR;
    }
    
    gDeviceContextPtr[device_id]->channel = usch_freq;
    return A_OK;
}


A_STATUS _qcom_p2p_func_get_pass_ssid(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid)
{
    A_STATUS ret;

    if ((NULL == ppass) || \
        (NULL == pssid) || \
        (WLAN_NUM_OF_DEVICES <= device_id)) {
        return A_ERROR;
    }

    ret = qcom_sec_get_passphrase(device_id, ppass);
    if (ret != A_OK) {
        return A_ERROR;
    }

    ret = qcom_get_ssid(device_id, pssid);
    if (ret != A_OK) {
        return A_ERROR;
    }

    return A_OK;
}

A_STATUS _qcom_p2p_func_set_pass_ssid(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid)
{
    A_UINT8 uclen;
    SET_PASSPHRASE_CMD passhrase;

    if ((NULL == ppass) || (NULL == pssid)) {
        return A_ERROR;
    }

    A_MEMZERO(&passhrase, sizeof(passhrase));
    uclen = sizeof(pssid);
    passhrase.ssid_len = (uclen < MAX_SSID_LEN) ? uclen : (MAX_SSID_LEN-1);
    A_STRNCPY((char *)(passhrase.ssid), pssid, passhrase.ssid_len);
    uclen = sizeof(ppass);
    passhrase.passphrase_len = (uclen < MAX_PASSPHRASE_SIZE) ? \
                                             uclen : (MAX_PASSPHRASE_SIZE);
    A_STRNCPY((char *)(passhrase.passphrase), ppass, passhrase.passphrase_len);

    qcom_p2p_set_passphrase(device_id, NULL, &passhrase);
    qcom_sec_set_passphrase(device_id, ppass);
    qcom_set_ssid(device_id, pssid);

    return A_OK;
}

A_STATUS _qcom_p2p_func_set_opps(A_UINT8 device_id, A_UINT8 enable, A_UINT8 ctwin)
{
    return qcom_p2p_set_oppps(device_id, NULL, enable, ctwin);
}

A_STATUS _qcom_p2p_func_set_noa(A_UINT8 device_id, A_UINT8 uccount, A_UINT32 uistart, A_UINT32 uiduration, A_UINT32 uiinterval)
{
    P2P_NOA_INFO p2p_noa_desc;
#if 0
    if (uccount >= P2P_MAX_NOA_DESCRIPTORS) {
        return A_ERROR;
    }
#endif
    A_MEMZERO(&p2p_noa_desc, sizeof(p2p_noa_desc));

    p2p_noa_desc.enable = 1;
    p2p_noa_desc.count = 1;
    p2p_noa_desc.noas[0].count_or_type = uccount;
    p2p_noa_desc.noas[0].duration = uiduration * 1000; //ms to us
    p2p_noa_desc.noas[0].interval = uiinterval * 1000; //ms to us
    p2p_noa_desc.noas[0].start_or_offset = uistart * 1000; //ms to us
     
    return qcom_p2p_set_noa(device_id, NULL, &p2p_noa_desc);
}

A_STATUS _qcom_p2p_func_cb_dispatch(A_UINT8 device_id, QCOM_P2P_FUNC_CB_ID encb_id)
{
    A_STATUS ret = A_ERROR;
    
    switch (encb_id) {
        case QCOM_P2P_FUNC_CANCEL:
            ret = qcom_p2p_func_cancel(device_id);
            break;
        default :
            break;
    }

    return ret;
}

void qcom_p2p_func_install_cb(void)
{
    extern A_STATUS (*qcom_p2p_func_cb)(A_UINT8 device_id, QCOM_P2P_FUNC_CB_ID cb_id);
    qcom_p2p_func_cb = _qcom_p2p_func_cb_dispatch;
    return;
}

A_STATUS _qcom_p2p_func_register_event_handler(qcom_p2p_event_handler cb, void *arg)
{
    p2p_event_handler = cb;
    p2p_event_handler_arg = arg;
}

A_STATUS _qcom_p2p_func_wps_start_no_scan(A_UINT8 dev_id, A_BOOL is_go, A_BOOL is_push_method,
        A_UINT32 channel, A_UINT8 *pssid, A_UINT8 *pin, A_UINT8 *pmac)
{
    int mode = (is_push_method) ? 1 : 0;
    int role = (is_go) ? (WPS_REGISTRAR_ROLE):(WPS_ENROLLEE_ROLE);
    /* 
     * used by the other device to check if both of them have same channel in SCC mode. 
     */
    qcom_p2p_func_set_channel_freq_cfg(dev_id, channel);
    qcom_wps_start_without_scan(dev_id, role, mode, \
            channel, 1, 30, (char *)pssid, \
            (char *)pin,\
            pmac);
}

A_STATUS _qcom_p2p_func_listen(A_UINT8 dev_id, A_UINT32 timeout)
{
    return qcom_p2p_listen(dev_id, NULL, timeout);
}

#ifdef LIBCUST_INDIRECT_ACCESS
QCOM_P2P_FUNC_API_INDIRECTION_TABLE qcomP2pFuncApiIndirectionTable = {
    _qcom_p2p_func_init,
    _qcom_p2p_func_cancel,
    _qcom_p2p_func_auth,
    _qcom_p2p_func_connect,
    _qcom_p2p_func_invite,
    _qcom_p2p_func_join,
    _qcom_p2p_func_join_profile,
    _qcom_p2p_func_prov,
    _qcom_p2p_func_set_config,
    _qcom_p2p_func_set_pass_ssid,
    _qcom_p2p_func_get_pass_ssid,
    _qcom_p2p_func_set_opps,
    _qcom_p2p_func_set_noa,
    _qcom_p2p_func_find,
    _qcom_p2p_func_stop_find,
    _qcom_p2p_func_set,
    _qcom_p2p_func_get_node_list,
    _qcom_p2p_func_invite_auth,
    _qcom_p2p_func_get_network_list,
    _qcom_p2p_func_register_event_handler,
    _qcom_p2p_func_start_go,
    _qcom_p2p_func_wps_start_no_scan,
    _qcom_p2p_func_get_role,
    _qcom_p2p_func_listen,
};
#endif

#endif

