/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __QCOM_P2P_API_H__
#define __QCOM_P2P_API_H__

#define WPS_MAX_DEVNAME_LEN 32
#define MAX_SSID_LEN 32
#define MAC_LEN 6
#define MAX_SSID_LENGTH (32+1)
#define P2P_WPS_PIN_LEN 9
#define MAX_PASSPHRASE_SIZE 64
#define P2P_MAX_NOA_DESCRIPTORS 4
#define WPS_MAX_PASSPHRASE_LEN 9
#define P2P_PERSISTENT_FLAG 0x80
#define MAX_LIST_COUNT 8

typedef void *P2P_DEV_CTXT;

typedef enum p2p_wps_methods {
    P2P_WPS_NOT_READY, 
    P2P_WPS_PIN_LABEL, 
    P2P_WPS_PIN_DISPLAY, 
    P2P_WPS_PIN_KEYPAD,
    P2P_WPS_PBC
} P2P_WPS_METHOD;

typedef enum p2p_disc_type {
    P2P_DISC_START_WITH_FULL,
    P2P_DISC_ONLY_SOCIAL,
    P2P_DISC_PROGRESSIVE
} P2P_DISC_TYPE;

typedef enum {
    P2P_CONFIG_LISTEN_CHANNEL,
    P2P_CONFIG_CROSS_CONNECT,
    P2P_CONFIG_SSID_POSTFIX,
    P2P_CONFIG_INTRA_BSS,
    P2P_CONFIG_CONCURRENT_MODE,
    P2P_CONFIG_GO_INTENT,
    P2P_CONFIG_DEV_NAME,
    P2P_CONFIG_P2P_OPMODE,
    P2P_CONFIG_MAX,
} P2P_CONF_ID;

typedef PREPACK struct {
    A_UINT8 ssidLength;
    A_UINT8 ssid[MAX_SSID_LEN];
} POSTPACK P2P_SSID;

typedef PREPACK struct {
    A_UINT8 go_intent;
    A_UCHAR dummy[3]; /* Deprecated field */
    A_UINT8 reg_class;
    A_UINT8 listen_channel;
    A_UINT8 op_reg_class;
    A_UINT8 op_channel;
    A_UINT32 node_age_to;
    A_UINT8 max_node_count;
} POSTPACK P2P_FW_SET_CONFIG_CMD;

typedef PREPACK struct {
    A_UINT16 go_oper_freq;
    A_UINT8  dialog_token;
    A_UINT8  peer_addr[MAC_LEN];
    A_UINT8  own_interface_addr[MAC_LEN];
    A_UINT8  go_dev_dialog_token;
    P2P_SSID peer_go_ssid;
    A_UINT8  wps_method;
    A_UINT8  dev_capab;
    A_UINT8  dev_auth;
    A_UINT8  go_intent;
} POSTPACK P2P_FW_CONNECT_CMD_STRUCT;

typedef PREPACK struct {
    P2P_FW_CONNECT_CMD_STRUCT info;
    A_UINT8 wps_pin[P2P_WPS_PIN_LEN];
} POSTPACK P2P_JOIN_CMD_STRUCT;

typedef enum {
    P2P_INV_ROLE_GO,
    P2P_INV_ROLE_ACTIVE_GO,
    P2P_INV_ROLE_CLIENT,
} P2P_INV_ROLE;

typedef PREPACK struct {
#if defined(AR6002_REV7) && defined(IOT_BUILD_FLAG)
    A_UINT8 role; /* P2P_INV_ROLE */
#else
    P2P_INV_ROLE role; /* P2P_INV_ROLE */
#endif
    A_UINT16 listen_freq;
    A_UINT16 force_freq;
    A_UINT8 dialog_token;
    A_UINT8 peer_addr[MAC_LEN];
    A_UINT8 bssid[MAC_LEN];
    A_UINT8 go_dev_addr[MAC_LEN];
    P2P_SSID ssid;
    A_UINT8 is_persistent;
    A_UINT8 wps_method;
}POSTPACK P2P_FW_INVITE_CMD;

typedef PREPACK struct {
    A_UINT16 force_freq;
    A_UINT8 status;
    A_UINT8 dialog_token;
    A_UINT8 is_go;
    A_UINT8 group_bssid[MAC_LEN];
}POSTPACK P2P_FW_INVITE_REQ_RSP_CMD;


typedef PREPACK struct {
    A_UINT16 wps_method;
    A_UINT8 dialog_token;
    A_UINT8 peer[MAC_LEN];
}POSTPACK P2P_FW_PROV_DISC_REQ_CMD;

/* SET_PASSPHRASE_CMD applies to AP and GO. Need to move this elsewhere */
typedef PREPACK struct {
    A_UCHAR ssid[MAX_SSID_LEN];
    A_UINT8 passphrase[MAX_PASSPHRASE_SIZE];
    A_UINT8 ssid_len;
    A_UINT8 passphrase_len;
} POSTPACK SET_PASSPHRASE_CMD;

typedef PREPACK struct {
    A_UINT8 reg_class;
    A_UINT8 listen_channel;
}POSTPACK P2P_LISTEN_CHANNEL;

typedef PREPACK struct {
    A_UINT8 flag;
}POSTPACK P2P_SET_CROSS_CONNECT;

typedef PREPACK struct {
    A_UINT8 flag;
}POSTPACK P2P_SET_CONCURRENT_MODE;

typedef PREPACK struct {
    A_UINT8 ssid_postfix[MAX_SSID_LEN-9];
    A_UINT8 ssid_postfix_len;
}POSTPACK P2P_SET_SSID_POSTFIX;

typedef PREPACK struct {
    A_UINT8 flag;
}POSTPACK P2P_SET_INTRA_BSS;

typedef PREPACK struct {
    A_UINT8 value;
}POSTPACK P2P_SET_GO_INTENT;

typedef PREPACK struct {
    A_UINT8 dev_name[WPS_MAX_DEVNAME_LEN];
    A_UINT8 dev_name_len;
}POSTPACK P2P_SET_DEV_NAME;

#define P2P_DEV    (1<<0)
#define P2P_CLIENT (1<<1)
#define P2P_GO     (1<<2)

typedef PREPACK struct {
    A_UINT8 p2pmode; /* P2P_DEV, P2P_CLIENT or P2P_GO */
}POSTPACK P2P_SET_MODE;

typedef PREPACK struct {
    A_UINT8 enable;
}POSTPACK P2P_SET_CCK_RATES;

typedef PREPACK struct {
    A_UINT32 duration;
    A_UINT32 interval;
    A_UINT32 start_or_offset;
    A_UINT8 count_or_type;
} POSTPACK P2P_NOA_DESCRIPTOR;

typedef struct {
    A_UINT8 enable;
    A_UINT8 count;
    P2P_NOA_DESCRIPTOR noas[P2P_MAX_NOA_DESCRIPTORS];
} P2P_NOA_INFO;

/* P2P events */
typedef enum {
    P2P_GO_NEG_RESULT_EVENTID,       
    P2P_GO_NEG_REQ_EVENTID,
    P2P_INVITE_REQ_EVENTID,
    P2P_INVITE_RCVD_RESULT_EVENTID,
    P2P_INVITE_SENT_RESULT_EVENTID,  
    P2P_PROV_DISC_RESP_EVENTID,
    P2P_PROV_DISC_REQ_EVENTID,
    P2P_START_SDPD_EVENTID,
    P2P_SDPD_RX_EVENTID,
    P2P_NODE_LIST_EVENTID,
    P2P_REQ_TO_AUTH_EVENTID,
    P2P_LIST_PERSISTENT_NETWORK_EVENTID,
} P2P_EVENT_ID;

typedef PREPACK struct {
    A_UINT8 sa[MAC_LEN];
    A_UINT8 wps_buf[512];
    A_UINT16 wps_buflen;
    A_UINT8 p2p_buf[512];
    A_UINT16 p2p_buflen;
    A_UINT8 dialog_token;
}POSTPACK P2P_GO_NEG_REQ_EVENT;

typedef PREPACK struct {
    A_UINT16 freq;
    A_INT8 status;
    A_UINT8 role_go;
    A_UINT8 ssid[MAX_SSID_LEN];
    A_UINT8 ssid_len;
    A_CHAR pass_phrase[WPS_MAX_PASSPHRASE_LEN];
    A_UINT8 peer_device_addr[MAC_LEN];
    A_UINT8 peer_interface_addr[MAC_LEN];
    A_UINT8 wps_method;
    A_UINT8 persistent_grp;
} POSTPACK P2P_GO_NEG_RESULT_EVENT;

typedef PREPACK struct {
    A_UINT8 p2p_buf[512];
    A_UINT16 p2p_buflen;
    A_UINT8 sa[MAC_LEN];
    A_UINT8 bssid[MAC_LEN];
    A_UINT8 go_dev_addr[MAC_LEN];
    P2P_SSID ssid;
    A_UINT8 is_persistent;
    A_UINT8 dialog_token;
} POSTPACK P2P_INVITE_REQ_EVENT;

typedef PREPACK struct {
    A_UINT8 sa[MAC_LEN];
    A_UINT8 bssid[MAC_LEN];
    A_UINT8 go_dev_addr[MAC_LEN];
    P2P_SSID ssid;
    A_UINT8 is_persistent;
    A_UINT8 dialog_token;
} POSTPACK P2P_FW_INVITE_REQ_EVENT;

typedef PREPACK struct {
    A_UINT16 oper_freq;
    A_UINT8 sa[MAC_LEN];
    A_UINT8 bssid[MAC_LEN];
    A_UINT8 is_bssid_valid;
    A_UINT8 go_dev_addr[MAC_LEN];
    P2P_SSID ssid;
    A_UINT8 status;
} POSTPACK P2P_INVITE_RCVD_RESULT_EVENT;

typedef PREPACK struct {
    A_UINT8 status;
    A_UINT8 bssid[MAC_LEN];
    A_UINT8 is_bssid_valid;
} POSTPACK P2P_INVITE_SENT_RESULT_EVENT;

typedef PREPACK struct {
    A_UINT8 sa[MAC_LEN];
    A_UINT16 wps_config_method;
    A_UINT8 dev_addr[MAC_LEN];
#define WPS_DEV_TYPE_LEN 8
    A_UINT8 pri_dev_type[WPS_DEV_TYPE_LEN];
    A_UINT8 device_name[WPS_MAX_DEVNAME_LEN];
    A_UINT8 dev_name_len;
    A_UINT16 dev_config_methods;
    A_UINT8 device_capab;
    A_UINT8 group_capab;
} POSTPACK P2P_PROV_DISC_REQ_EVENT;

typedef PREPACK struct {
    A_UINT8 peer[MAC_LEN];
    A_UINT16 config_methods;
} POSTPACK P2P_PROV_DISC_RESP_EVENT;

typedef PREPACK struct {
    A_UINT8 type;
    A_UINT8 transaction_status;
    A_UINT8 dialog_token;
    A_UINT8 frag_id;
    A_UINT8 peer_addr[MAC_LEN];
    A_UINT16 freq;
    A_UINT16 status_code;
    A_UINT16 comeback_delay;
    A_UINT16 tlv_length; 
    A_UINT16 update_indic;
//  Variable length TLV will be placed after the event  
} POSTPACK P2P_SDPD_RX_EVENT;

typedef PREPACK struct {
    A_CHAR wps_pin[P2P_WPS_PIN_LEN] ;
    A_UINT8 peer_addr[MAC_LEN] ;
    A_UINT8 wps_role ;
} POSTPACK P2P_PROV_INFO;

typedef PREPACK struct {
    A_UINT32 wps_method;
    A_UINT16 config_methods;
    A_UINT16 oper_freq;
    A_UINT8 pri_dev_type[8];
    A_UINT8 p2p_device_addr[MAC_LEN];
    A_UINT8 interface_addr[MAC_LEN];
    A_UINT8 dev_capab;
    A_UINT8 group_capab;
    A_UINT8 persistent_grp;
    A_CHAR device_name[33];
}POSTPACK P2P_DEVICE_LITE ;

#define P2P_MAX_NODE_INDIC 3

typedef PREPACK struct {     
    A_UINT8 num_p2p_dev; /* P2P_MAX_NODE_INDIC */
    A_UINT8 data[1]; /* P2P_DEVICE_LITE */
} POSTPACK P2P_NODE_LIST_EVENT;

typedef PREPACK struct {
    A_UINT8 persistent_group;
    A_UINT8 group_formation;
} POSTPACK P2P_GRP_INIT_CMD;

#define WMI_MAX_SSID_LEN    32
#define ATH_MAC_LEN             6               /* length of mac in bytes */
typedef PREPACK struct {
    A_UINT8 ssid[WMI_MAX_SSID_LEN];
    A_UINT8 macaddress[ATH_MAC_LEN];
    A_UINT16 channel;
    A_UINT8 ssid_len;	
} POSTPACK P2P_WPS_SCAN_LIST_ENTRY;

#define WPS_PIN_LEN (8)
typedef PREPACK struct {
    A_UINT8 pin[WPS_PIN_LEN+1];
    A_UINT8 pin_length;
}POSTPACK P2P_WPS_PIN;

typedef PREPACK struct {
    P2P_WPS_SCAN_LIST_ENTRY ssid_info;
    A_UINT8 config_mode; /* WPS_MODE */
    P2P_WPS_PIN wps_pin;
    A_UINT8 timeout;     /* in Seconds */
    A_UINT8 role;        /* WPS_OPER_MOD */	
    A_UINT8 ctl_flag;    /* WPS_START_CTRL_FLAG */
} POSTPACK P2P_WMI_WPS_START_CMD;

typedef PREPACK struct {
    A_UINT8 sa[ATH_MAC_LEN];
    A_UINT8 dialog_token;
    A_UINT16 dev_password_id;
}POSTPACK P2P_REQ_TO_AUTH_EVENT;

typedef struct {
    A_UINT8  role;
    A_UINT8  macaddr[MAC_LEN];
    A_UINT8  ssid[MAX_SSID_LEN];
    A_UINT8  passphrase[MAX_SSID_LEN];
} P2P_PERSISTENT_MAC_LIST;

#define PERSISTENT_LIST_SIZE (MAX_LIST_COUNT * sizeof(P2P_PERSISTENT_MAC_LIST))

/* Application handler for P2P WMI events */
typedef int (* qcom_p2p_event_handler)(A_UINT8 device_id, P2P_EVENT_ID event_id, A_UINT8 *pBuffer, A_UINT32 len, void *arg);

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {
    A_STATUS (* _qcom_p2p_func_init)(A_UINT8 device_id, A_INT32 enable);
    A_STATUS (* _qcom_p2p_func_cancel)(A_UINT8 device_id);
    A_STATUS (* _qcom_p2p_func_auth)(A_UINT8 device_id, A_INT32 dev_auth, P2P_WPS_METHOD wps_method, A_UINT8 *peer_mac,
            A_BOOL persistent);
    A_STATUS (* _qcom_p2p_func_connect)(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *peer_mac, A_BOOL persistent);
    A_STATUS (* _qcom_p2p_func_invite)(A_UINT8 device_id, A_CHAR *pssid, P2P_WPS_METHOD wps_method, A_UINT8 *pmac, A_BOOL persistent, P2P_INV_ROLE role);
    A_STATUS (* _qcom_p2p_func_join)(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *pmac, char *ppin);
    A_STATUS (* _qcom_p2p_func_join_profile)(A_UINT8 device_id, A_UINT8 *pmac);
    A_STATUS (* _qcom_p2p_func_prov)(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *pmac);
    A_STATUS (* _qcom_p2p_func_set_config)(A_UINT8 device_id, A_UINT32 uigo_intent, A_UINT32 uiclisten_ch, A_UINT32 uiop_ch, A_UINT32 uiage, A_UINT32 reg_class, A_UINT32 op_reg_class, A_UINT32 max_node_count);
    A_STATUS (* _qcom_p2p_func_set_pass_ssid)(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid);
    A_STATUS (* _qcom_p2p_func_get_pass_ssid)(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid);
    A_STATUS (* _qcom_p2p_func_set_opps)(A_UINT8 device_id, A_UINT8 enable, A_UINT8 ctwin);
    A_STATUS (* _qcom_p2p_func_set_noa)(A_UINT8 device_id, A_UINT8 uccount, A_UINT32 uistart, A_UINT32 uiduration, A_UINT32 uiinterval);
    A_STATUS (* _qcom_p2p_func_find)(A_UINT8 device_id, P2P_DISC_TYPE type, A_UINT32 timeout);
    A_STATUS (* _qcom_p2p_func_stop_find)(A_UINT8 device_id);
    A_STATUS (* _qcom_p2p_func_set)(A_UINT8 device_id, P2P_CONF_ID config_id, void *data, A_UINT32 data_length);
    A_STATUS (* _qcom_p2p_func_get_node_list)(A_UINT8 device_id, P2P_NODE_LIST_EVENT *app_buf, A_UINT32 buf_size);
    A_STATUS (* _qcom_p2p_func_invite_auth)(A_UINT8 device_id, P2P_FW_INVITE_REQ_RSP_CMD *inv);
    A_STATUS (* _qcom_p2p_func_get_network_list)(A_UINT8 device_id, P2P_PERSISTENT_MAC_LIST *app_buf, A_UINT32 buf_size);
    A_STATUS (* _qcom_p2p_func_register_event_handler)(qcom_p2p_event_handler cb, void *arg);
    A_STATUS (* _qcom_p2p_func_start_go)(A_UINT8 device_id, A_UINT8 *pssid, A_UINT8 *ppass, A_INT32 channel, A_BOOL ucpersistent);
    A_STATUS (* _qcom_p2p_func_wps_start_no_scan)(A_UINT8 dev_id, A_BOOL is_go, A_BOOL is_push_method,
            A_UINT32 channel, A_UINT8 *pssid, A_UINT8 *pin, A_UINT8 *pmac);
    P2P_INV_ROLE (* _qcom_p2p_func_get_role)(A_UINT32 role);
    A_STATUS (* _qcom_p2p_func_listen)(A_UINT8 dev_id, A_UINT32 timeout);
} QCOM_P2P_FUNC_API_INDIRECTION_TABLE;

extern QCOM_P2P_FUNC_API_INDIRECTION_TABLE qcomP2pFuncApiIndirectionTable;

#define QCOM_P2P_FUNC_API_FN(fn) qcomP2pFuncApiIndirectionTable.fn

#else
 
#define QCOM_P2P_FUNC_API_FN(fn) fn
A_STATUS _qcom_p2p_func_init(A_UINT8 device_id, A_INT32 enable);
A_STATUS _qcom_p2p_func_cancel(A_UINT8 device_id);
A_STATUS _qcom_p2p_func_auth(A_UINT8 device_id, A_INT32 dev_auth, P2P_WPS_METHOD wps_method, A_UINT8 *peer_mac,
        A_BOOL persistent);
A_STATUS _qcom_p2p_func_connect(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *peer_mac, A_BOOL persistent);
A_STATUS _qcom_p2p_func_invite(A_UINT8 device_id, A_CHAR *pssid, P2P_WPS_METHOD wps_method, A_UINT8 *pmac, A_BOOL persistent, P2P_INV_ROLE role);
A_STATUS _qcom_p2p_func_join(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *pmac, char *ppin);
A_STATUS _qcom_p2p_func_join_profile(A_UINT8 device_id, A_UINT8 *pmac);
A_STATUS _qcom_p2p_func_prov(A_UINT8 device_id, P2P_WPS_METHOD wps_method, A_UINT8 *pmac);
A_STATUS _qcom_p2p_func_set_config(A_UINT8 device_id, A_UINT32 uigo_intent, A_UINT32 uiclisten_ch, A_UINT32 uiop_ch, A_UINT32 uiage, A_UINT32 reg_class, A_UINT32 op_reg_class, A_UINT32 max_node_count);
A_STATUS _qcom_p2p_func_set_pass_ssid(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid);
A_STATUS _qcom_p2p_func_get_pass_ssid(A_UINT8 device_id, A_CHAR *ppass, A_CHAR *pssid);
A_STATUS _qcom_p2p_func_set_opps(A_UINT8 device_id, A_UINT8 enable, A_UINT8 ctwin);
A_STATUS _qcom_p2p_func_set_noa(A_UINT8 device_id, A_UINT8 uccount, A_UINT32 uistart, A_UINT32 uiduration, A_UINT32 uiinterval);
A_STATUS _qcom_p2p_func_find(A_UINT8 device_id, P2P_DISC_TYPE type, A_UINT32 timeout);
A_STATUS _qcom_p2p_func_stop_find(A_UINT8 device_id);
A_STATUS _qcom_p2p_func_set(A_UINT8 device_id, P2P_CONF_ID config_id, void *data, A_UINT32 data_length);
A_STATUS _qcom_p2p_func_get_node_list(A_UINT8 device_id, P2P_NODE_LIST_EVENT *app_buf, A_UINT32 buf_size);
A_STATUS _qcom_p2p_func_invite_auth(A_UINT8 device_id, P2P_FW_INVITE_REQ_RSP_CMD *inv);
A_STATUS _qcom_p2p_func_get_network_list(A_UINT8 device_id, P2P_PERSISTENT_MAC_LIST *app_buf, A_UINT32 buf_size);
A_STATUS _qcom_p2p_func_register_event_handler(qcom_p2p_event_handler cb, void *arg);
A_STATUS _qcom_p2p_func_start_go(A_UINT8 device_id, A_UINT8 *pssid, A_UINT8 *ppass, A_INT32 channel, A_BOOL ucpersistent);
A_STATUS _qcom_p2p_func_wps_start_no_scan(A_UINT8 dev_id, A_BOOL is_go, A_BOOL is_push_method,
        A_UINT32 channel, A_UINT8 *pssid, A_UINT8 *pin, A_UINT8 *pmac);
P2P_INV_ROLE _qcom_p2p_func_get_role(A_UINT32 role);
A_STATUS _qcom_p2p_func_listen(A_UINT8 dev_id, A_UINT32 timeout);
#endif 

#define qcom_p2p_func_init(dev_id, en) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_init((dev_id), (en)))

#define qcom_p2p_func_cancel(dev_id) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_cancel((dev_id)))

#define qcom_p2p_func_auth(dev_id, dev_auth, wps_method, peer_mac, per) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_auth((dev_id), (dev_auth), (wps_method), (peer_mac), (per)))

#define qcom_p2p_func_connect(dev_id, wps_method, peer_mac, per) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_connect((dev_id), (wps_method), (peer_mac), (per)))

#define qcom_p2p_func_invite(dev_id, pssid, wps_method, peer_mac, per, role) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_invite((dev_id), (pssid), (wps_method), (peer_mac), (per), (role)))

#define qcom_p2p_func_join(dev_id, wps_method, pmac, pin) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_join((dev_id), (wps_method), (pmac), (pin)))

#define qcom_p2p_func_join_profile(dev_id, pmac) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_join_profile((dev_id), (pmac)))

#define qcom_p2p_func_prov(dev_id, wps_method, pmac) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_prov((dev_id), (wps_method), (pmac)))

#define qcom_p2p_func_set_config(device_id, uigo_intent, uiclisten_ch, uiop_ch, uiage, reg_class, op_reg_class, max_node_count) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_set_config((device_id), (uigo_intent), (uiclisten_ch), (uiop_ch), (uiage), (reg_class), (op_reg_class), (max_node_count)))

#define qcom_p2p_func_set_pass_ssid(device_id, ppass, pssid) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_set_pass_ssid((device_id), (ppass), (pssid)))

#define qcom_p2p_func_get_pass_ssid(device_id, ppass, pssid) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_get_pass_ssid((device_id), (ppass), (pssid)))

#define qcom_p2p_func_set_opps(device_id, enable, ctwin) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_set_opps((device_id), (enable), (ctwin)))

#define qcom_p2p_func_set_noa(device_id, uccount, uistart, uiduration, uiinterval) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_set_noa((device_id), (uccount), (uistart), (uiduration), (uiinterval)))

#define qcom_p2p_func_find(device_id, type, timeout) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_find((device_id), (type), (timeout)))

#define qcom_p2p_func_stop_find(device_id) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_stop_find((device_id)))

#define qcom_p2p_func_set(device_id, config_id, data, data_length) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_set((device_id), (config_id), (data), (data_length))) 

#define qcom_p2p_func_get_node_list(device_id, app_buf, buf_size) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_get_node_list((device_id), (app_buf), (buf_size)))

#define qcom_p2p_func_invite_auth(device_id, inv) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_invite_auth((device_id), (inv)))

#define qcom_p2p_func_get_network_list(device_id, app_buf, buf_size) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_get_network_list((device_id), (app_buf), (buf_size)))

#define qcom_p2p_func_register_event_handler(cb, arg) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_register_event_handler((cb), (arg)))

#define qcom_p2p_func_start_go(device_id, pssid, ppass, channel, ucpersistent) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_start_go((device_id), (pssid), (ppass), (channel), (ucpersistent)))

#define qcom_p2p_func_wps_start_no_scan(dev_id, is_go, is_push_method, channel, pssid, pin, pmac) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_wps_start_no_scan((dev_id), (is_go), (is_push_method), (channel), (pssid), (pin), (pmac)))

#define qcom_p2p_func_get_role(role) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_get_role((role)))

#define qcom_p2p_func_listen(dev_id, timeout) \
    QCOM_P2P_FUNC_API_FN(_qcom_p2p_func_listen((dev_id), (timeout)))

#endif
