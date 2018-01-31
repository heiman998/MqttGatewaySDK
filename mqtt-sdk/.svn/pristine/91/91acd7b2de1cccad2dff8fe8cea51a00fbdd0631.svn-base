/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _QCOM_INTERNAL_H__
#define _QCOM_INTERNAL_H__

#include "qcom/qcom_p2p_api.h"
#define ATH_MAC_LEN 6
#define FIELD_PACKED 

typedef enum {
    QCOM_P2P_FUNC_CANCEL,
    QCOM_P2P_FUNC_NULL,
} QCOM_P2P_FUNC_CB_ID;

/******************************** SCAN ********************************/

/******************************** wmi.h ********************************/
typedef enum {
    WMI_LONG_SCAN  = 0,
    WMI_SHORT_SCAN = 1,
} WMI_SCAN_TYPE;

typedef enum {
    WMI_P2P_INVITE_ROLE_GO,
    WMI_P2P_INVITE_ROLE_ACTIVE_GO,
    WMI_P2P_INVITE_ROLE_CLIENT,
} WMI_P2P_INVITE_ROLE;

typedef enum {
    WPS_REGISTRAR_ROLE = 0x1,
    WPS_ENROLLEE_ROLE  = 0x2
} WPS_OPER_MODE;

typedef enum {
    DISABLE_SSID_FLAG  = 0,                  /* disables entry */
    SPECIFIC_SSID_FLAG = 0x01,               /* probes specified ssid */
    ANY_SSID_FLAG      = 0x02,               /* probes for any ssid */
    PASSIVE_LISTEN_SSID_FLAG = 0x04,               /* Just Listen on the Channel without Probing for any SSIDs But we wanted to use the Probed SSID List*/ 							   /* for Profile Matching */
} WMI_SSID_FLAG;

typedef PREPACK struct {
    A_UINT8     entryIndex;                     /* 0 to MAX_PROBED_SSID_INDEX */
    A_UINT8     flag;                           /* WMI_SSID_FLG */
    A_UINT8     ssidLength;
    A_UINT8     ssid[32];
} POSTPACK WMI_PROBED_SSID_CMD;

typedef enum {
    NONE_BSS_FILTER = 0x0,              /* no beacons forwarded */
    ALL_BSS_FILTER,                     /* all beacons forwarded */
    PROFILE_FILTER,                     /* only beacons matching profile */
    ALL_BUT_PROFILE_FILTER,             /* all but beacons matching profile */
    CURRENT_BSS_FILTER,                 /* only beacons matching current BSS */
    ALL_BUT_BSS_FILTER,                 /* all but beacons matching BSS */
    PROBED_SSID_FILTER,                 /* beacons matching probed ssid */
    LAST_BSS_FILTER,                    /* marker only */
} WMI_BSS_FILTER;

typedef enum {
    WLAN_DEV_MODE_AP = 0,
    WLAN_DEV_MODE_STATION,
    WLAN_DEV_MODE_INVALID
}WLAN_DEV_MODE;

typedef enum {
    WLAN_11A_MODE = 0x1,
    WLAN_11G_MODE = 0x2,
    WLAN_11AG_MODE = 0x3,
    WLAN_11B_MODE = 0x4,
    WLAN_11GONLY_MODE = 0x5,
} WLAN_DEV_PHY_MODE;

typedef enum {
    INFRA_NETWORK       = 0x01,
    ADHOC_NETWORK       = 0x02,
    ADHOC_CREATOR       = 0x04,
    AP_NETWORK          = 0x10,
    NETWORK_CONNECTED_USING_WPS = 0x20,
} NETWORK_TYPE;

typedef enum {
    OPEN_AUTH           = 0x01,
    SHARED_AUTH         = 0x02,
    LEAP_AUTH           = 0x04,  /* different from IEEE_AUTH_MODE definitions */
} DOT11_AUTH_MODE;

typedef enum {
    WMI_NONE_AUTH           = 0x01,
    WMI_WPA_AUTH            = 0x02,
    WMI_WPA2_AUTH           = 0x04,
    WMI_WPA_PSK_AUTH        = 0x08,
    WMI_WPA2_PSK_AUTH       = 0x10,
    WMI_WPA_AUTH_CCKM       = 0x20,
    WMI_WPA2_AUTH_CCKM      = 0x40,
    WMI_WPA2_PSK_SHA256_AUTH = 0x80,
} AUTH_MODE;

typedef PREPACK struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_BI_FTYPE */
    A_UINT8     snr;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT16    ieMask;
} POSTPACK WMI_BSS_INFO_HDR2;

typedef PREPACK struct {
    A_UINT32    msec_sleep;
} POSTPACK WMI_STORERECALL_START_EVENT;

/******************************** wlan_scan.c ********************************/
#define BEACON_PERIOD_IE_INDEX 8
#define CAPS_IE_INDEX 10
#define IE_INDEX 12
#define BSS_PRINT_DELAY qcom_thread_msleep(10);

/* from ieee80211.h */

/*
 * 802.11 protocol definitions.
 */
#define IEEE80211_WEP_KEYLEN        5   /* 40bit */
#define IEEE80211_WEP_IVLEN         3   /* 24bit */
#define IEEE80211_WEP_KIDLEN        1   /* 1 octet */
#define IEEE80211_WEP_CRCLEN        4   /* CRC-32 */
#define IEEE80211_WEP_NKID          4   /* number of key ids */

/*
 * 802.11i defines an extended IV for use with non-WEP ciphers.
 * When the EXTIV bit is set in the key id byte an additional
 * 4 bytes immediately follow the IV for TKIP.  For CCMP the
 * EXTIV bit is likewise set but the 8 bytes represent the
 * CCMP header rather than IV+extended-IV.
 */
#define IEEE80211_WEP_EXTIV         0x20
#define IEEE80211_WEP_EXTIVLEN      4   /* extended IV length */
#define IEEE80211_WEP_MICLEN        8   /* trailing MIC */

#define IEEE80211_CRC_LEN           4

#ifdef WAPI_ENABLE
#define IEEE80211_WAPI_EXTIVLEN      10 /* extended IV length */
#endif                          /* WAPI ENABLE */

#define IEEE80211_ADDR_LEN  6   /* size of 802.11 address */
/* is 802.11 address multicast/broadcast? */
#define IEEE80211_IS_MULTICAST(_a)  (*(_a) & 0x01)
#define IEEE80211_IS_BROADCAST(_a)  (*(_a) == 0xFF)
#define WEP_HEADER (IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN)
#define WEP_TRAILER IEEE80211_WEP_CRCLEN
#define CCMP_HEADER (IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN + \
                    IEEE80211_WEP_EXTIVLEN)
#define CCMP_TRAILER IEEE80211_WEP_MICLEN
#define TKIP_HEADER (IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN + \
                    IEEE80211_WEP_EXTIVLEN)
#define TKIP_TRAILER IEEE80211_WEP_CRCLEN
#define TKIP_MICLEN  IEEE80211_WEP_MICLEN


#define IEEE80211_KEYBUF_SIZE 16

/*
 * NB: these values are ordered carefully; there are lots of
 * of implications in any reordering.  In particular beware
 * that 4 is not used to avoid conflicting with IEEE80211_F_PRIVACY.
 */
#define IEEE80211_CIPHER_WEP            0
#define IEEE80211_CIPHER_TKIP           1
#define IEEE80211_CIPHER_AES_OCB        2
#define IEEE80211_CIPHER_AES_CCM        3
#define IEEE80211_CIPHER_CKIP           5
#define IEEE80211_CIPHER_CCKM_KRK       6
#define IEEE80211_CIPHER_NONE           7   /* pseudo value */

#define IEEE80211_IS_VALID_WEP_CIPHER_LEN(len) \
        (((len) == 5) || ((len) == 13) || ((len) == 16))

/*
 * generic definitions for IEEE 802.11 frames
 */

#define IEEE80211_FC0_VERSION_MASK          0x03
#define IEEE80211_FC0_VERSION_SHIFT         0
#define IEEE80211_FC0_VERSION_0             0x00
#define IEEE80211_FC0_TYPE_MASK             0x0c
#define IEEE80211_FC0_TYPE_SHIFT            2
#define IEEE80211_FC0_TYPE_MGT              0x00
#define IEEE80211_FC0_TYPE_CTL              0x04
#define IEEE80211_FC0_TYPE_DATA             0x08

#define IEEE80211_FC0_SUBTYPE_MASK          0xf0
#define IEEE80211_FC0_SUBTYPE_SHIFT         4
/* for TYPE_MGT */
#define IEEE80211_FC0_SUBTYPE_ASSOC_REQ     0x00
#define IEEE80211_FC0_SUBTYPE_ASSOC_RESP    0x10
#define IEEE80211_FC0_SUBTYPE_REASSOC_REQ   0x20
#define IEEE80211_FC0_SUBTYPE_REASSOC_RESP  0x30
#define IEEE80211_FC0_SUBTYPE_PROBE_REQ     0x40
#define IEEE80211_FC0_SUBTYPE_PROBE_RESP    0x50
#define IEEE80211_FC0_SUBTYPE_BEACON        0x80
#define IEEE80211_FC0_SUBTYPE_ATIM          0x90
#define IEEE80211_FC0_SUBTYPE_DISASSOC      0xa0
#define IEEE80211_FC0_SUBTYPE_AUTH          0xb0
#define IEEE80211_FC0_SUBTYPE_DEAUTH        0xc0
/* for TYPE_CTL */
#define IEEE80211_FC0_SUBTYPE_PS_POLL       0xa0
#define IEEE80211_FC0_SUBTYPE_RTS           0xb0
#define IEEE80211_FC0_SUBTYPE_CTS           0xc0
#define IEEE80211_FC0_SUBTYPE_ACK           0xd0
#define IEEE80211_FC0_SUBTYPE_CF_END        0xe0
#define IEEE80211_FC0_SUBTYPE_CF_END_ACK    0xf0
/* for TYPE_DATA (bit combination) */
#define IEEE80211_FC0_SUBTYPE_DATA          0x00
#define IEEE80211_FC0_SUBTYPE_CF_ACK        0x10
#define IEEE80211_FC0_SUBTYPE_CF_POLL       0x20
#define IEEE80211_FC0_SUBTYPE_CF_ACPL       0x30
#define IEEE80211_FC0_SUBTYPE_NODATA        0x40
#define IEEE80211_FC0_SUBTYPE_CFACK         0x50
#define IEEE80211_FC0_SUBTYPE_CFPOLL        0x60
#define IEEE80211_FC0_SUBTYPE_CF_ACK_CF_ACK 0x70
#define IEEE80211_FC0_SUBTYPE_QOS           0x80
#define IEEE80211_FC0_SUBTYPE_QOS_NULL      0xc0

#define IEEE80211_FC1_DIR_MASK              0x03
#define IEEE80211_FC1_DIR_NODS              0x00    /* STA->STA */
#define IEEE80211_FC1_DIR_TODS              0x01    /* STA->AP  */
#define IEEE80211_FC1_DIR_FROMDS            0x02    /* AP ->STA */
#define IEEE80211_FC1_DIR_DSTODS            0x03    /* AP ->AP  */

#define IEEE80211_FC1_MORE_FRAG             0x04
#define IEEE80211_FC1_RETRY                 0x08
#define IEEE80211_FC1_PWR_MGT               0x10
#define IEEE80211_FC1_MORE_DATA             0x20
#define IEEE80211_FC1_WEP                   0x40
#define IEEE80211_FC1_ORDER                 0x80

#define IEEE80211_SEQ_FRAG_MASK             0x000f
#define IEEE80211_SEQ_FRAG_SHIFT            0
#define IEEE80211_SEQ_SEQ_MASK              0xfff0
#define IEEE80211_SEQ_SEQ_SHIFT             4

#define IEEE80211_NWID_LEN                  32

typedef enum {
	INVALID_CRYPT       = 0x00,
    NONE_CRYPT          = 0x01,
    WEP_CRYPT           = 0x02,
    TKIP_CRYPT          = 0x04,
    AES_CRYPT           = 0x08,
#ifdef WAPI_ENABLE
    WAPI_CRYPT          = 0x10,
#endif /*WAPI_ENABLE*/
    BIP_CRYPT           = 0x20,
    KTK_CRYPT           = 0x40,
} CRYPTO_TYPE;

/*
 * 802.11 rate set.
 */
#define IEEE80211_RATE_SIZE     8   /* 802.11 standard */
#define IEEE80211_RATE_MAXSIZE  15  /* max rates we'll handle */

#define WMM_NUM_AC                  4   /* 4 AC categories */

#define WMM_PARAM_ACI_M         0x60    /* Mask for ACI field */
#define WMM_PARAM_ACI_S         5   /* Shift for ACI field */
#define WMM_PARAM_ACM_M         0x10    /* Mask for ACM bit */
#define WMM_PARAM_ACM_S         4   /* Shift for ACM bit */
#define WMM_PARAM_AIFSN_M       0x0f    /* Mask for aifsn field */
#define WMM_PARAM_LOGCWMIN_M    0x0f    /* Mask for CwMin field (in log) */
#define WMM_PARAM_LOGCWMAX_M    0xf0    /* Mask for CwMax field (in log) */
#define WMM_PARAM_LOGCWMAX_S    4   /* Shift for CwMax field */

#define WMM_AC_TO_TID(_ac) (       \
    ((_ac) == WMM_AC_VO) ? 6 : \
    ((_ac) == WMM_AC_VI) ? 5 : \
    ((_ac) == WMM_AC_BK) ? 1 : \
    0)

 /*
  * Management information element payloads.
  */

#define ATH_OUI             0x7f0300    /* Atheros OUI */
#define ATH_OUI_TYPE        0x01
#define ATH_OUI_SUBTYPE     0x01
#define ATH_OUI_VERSION     0x00

#define WPA_OUI             0xf25000
#define WPA_OUI_TYPE        0x01
#define WPA_VERSION         1   /* current supported version */

#define WPA_CSE_NULL        0x00
#define WPA_CSE_WEP40       0x01
#define WPA_CSE_TKIP        0x02
#define WPA_CSE_CCMP        0x04
#define WPA_CSE_WEP104      0x05

#define WPA_ASE_NONE        0x00
#define WPA_ASE_8021X_UNSPEC    0x01
#define WPA_ASE_8021X_PSK   0x02

#define RSN_OUI         0xac0f00
#define RSN_VERSION     1       /* current supported version */

#define RSN_CSE_NULL        0x00
#define RSN_CSE_WEP40       0x01
#define RSN_CSE_TKIP        0x02
#define RSN_CSE_WRAP        0x03
#define RSN_CSE_CCMP        0x04
#define RSN_CSE_WEP104      0x05

#define RSN_ASE_NONE            0x00
#define RSN_ASE_8021X_UNSPEC    0x01
#define RSN_ASE_8021X_PSK       0x02

#define RSN_CAP_PREAUTH         0x01

#define WMM_OUI                 0xf25000
#define WMM_OUI_TYPE            0x02
#define WMM_INFO_OUI_SUBTYPE    0x00
#define WMM_PARAM_OUI_SUBTYPE   0x01
#define WMM_VERSION             1

/* WMM stream classes */
#define WMM_NUM_AC  4
#define WMM_AC_BE   0           /* best effort */
#define WMM_AC_BK   1           /* background */
#define WMM_AC_VI   2           /* video */
#define WMM_AC_VO   3           /* voice */

/* TSPEC related */
#define ACTION_CATEGORY_CODE_TSPEC                 17
#define ACTION_CODE_TSPEC_ADDTS                    0
#define ACTION_CODE_TSPEC_ADDTS_RESP               1
#define ACTION_CODE_TSPEC_DELTS                    2

#define TSPEC_TSID_MASK             0xF
#define TSPEC_TSID_S                1

/*
 * WMM/802.11e Tspec Element
 */

/*
 * BEACON management packets
 *
 *  octet timestamp[8]
 *  octet beacon interval[2]
 *  octet capability information[2]
 *  information element
 *      octet elemid
 *      octet length
 *      octet information[length]
 */

#define IEEE80211_BEACON_INTERVAL(beacon) \
    ((beacon)[8] | ((beacon)[9] << 8))
#define IEEE80211_BEACON_CAPABILITY(beacon) \
    ((beacon)[10] | ((beacon)[11] << 8))

#define IEEE80211_CAPINFO_ESS               0x0001
#define IEEE80211_CAPINFO_IBSS              0x0002
#define IEEE80211_CAPINFO_CF_POLLABLE       0x0004
#define IEEE80211_CAPINFO_CF_POLLREQ        0x0008
#define IEEE80211_CAPINFO_PRIVACY           0x0010
#define IEEE80211_CAPINFO_SHORT_PREAMBLE    0x0020
#define IEEE80211_CAPINFO_PBCC              0x0040
#define IEEE80211_CAPINFO_CHNL_AGILITY      0x0080
/* bits 8-9 are reserved */
#define IEEE80211_CAPINFO_SHORT_SLOTTIME    0x0400
#define IEEE80211_CAPINFO_APSD              0x0800
/* bit 12 is reserved */
#define IEEE80211_CAPINFO_DSSSOFDM          0x2000
/* bits 14-15 are reserved */

/* * Authentication Modes
 */

#define IEEE80211_PS_MAX_QUEUE    50    /*Maximum no of buffers that can be queues for PS */

#define IEEE80211_TKIP_COUNTERMEASURES_TIMEOUT_MSEC (60000)




/******************************** wlan_scan.h ********************************/

#define ATH_CIPHER_TYPE_TKIP 0x04
#define ATH_CIPHER_TYPE_CCMP 0x08
#define ATH_CIPHER_TYPE_WEP  0x02
#define ATH_CIPHER_TYPE_NONE  0x00


#define SECURITY_AUTH_NONE    0x00
#define SECURITY_AUTH_PSK    0x01
#define SECURITY_AUTH_1X     0x02

#define ENET_MEDIACTL_MODE_AUTO                                  (0)
/* Single cell network */
#define ENET_MEDIACTL_MODE_ADHOC                                 (1)
/* Multi cell network, roaming, ... */
#define ENET_MEDIACTL_MODE_INFRA                                 (2)
/* Synchronisation master or Access Point */
#define ENET_MEDIACTL_MODE_MASTER                                (3)
/* Wireless Repeater (forwarder) */
#define ENET_MEDIACTL_MODE_REPEAT                                (4)
 /* Secondary master/repeater (backup) */
#define ENET_MEDIACTL_MODE_SECOND                                (5)
/* Passive monitor (listen only) */
#define ENET_MEDIACTL_MODE_MONITOR                               (6)
/* Mesh (IEEE 802.11s) network */
#define ENET_MEDIACTL_MODE_MESH                                  (7)

typedef enum {
    NONE_AUTH           = 0x01,
    WPA_AUTH            = 0x02,
    WPA2_AUTH           = 0x04,
    WPA_PSK_AUTH        = 0x08,
    WPA2_PSK_AUTH       = 0x10,
    WPA_AUTH_CCKM       = 0x20,
    WPA2_AUTH_CCKM      = 0x40
} xxTH_MODE;


/******************************** SCAN MISC ********************************/
enum {
    IEEE80211_ELEMID_SSID       = 0,
    IEEE80211_ELEMID_RATES      = 1,
    IEEE80211_ELEMID_FHPARMS    = 2,
    IEEE80211_ELEMID_DSPARMS    = 3,
    IEEE80211_ELEMID_CFPARMS    = 4,
    IEEE80211_ELEMID_TIM        = 5,
    IEEE80211_ELEMID_IBSSPARMS  = 6,
    IEEE80211_ELEMID_COUNTRY    = 7,
    IEEE80211_ELEMID_QBSS       = 11,
    IEEE80211_ELEMID_CHALLENGE  = 16,
    /* 17-31 reserved for challenge text extension */
    IEEE80211_ELEMID_PWRCNSTR   = 32,
    IEEE80211_ELEMID_PWRCAP     = 33,
    IEEE80211_ELEMID_TPCREQ     = 34,
    IEEE80211_ELEMID_TPCREP     = 35,
    IEEE80211_ELEMID_SUPPCHAN   = 36,
    IEEE80211_ELEMID_CHANSWITCH = 37,
    IEEE80211_ELEMID_MEASREQ    = 38,
    IEEE80211_ELEMID_MEASREP    = 39,
    IEEE80211_ELEMID_QUIET      = 40,
    IEEE80211_ELEMID_IBSSDFS    = 41,
    IEEE80211_ELEMID_ERP        = 42,
    IEEE80211_ELEMID_HTCAP_ANA  = 45,   /* Address ANA, and non-ANA story, for interop. CL#171733 */
    IEEE80211_ELEMID_RSN        = 48,
    IEEE80211_ELEMID_XRATES     = 50,
    IEEE80211_ELEMID_APCHANREP  = 51,
    IEEE80211_ELEMID_NEIGHBORREP= 52,
    IEEE80211_ELEMID_HTINFO_ANA = 61,
    IEEE80211_ELEMID_SEC_CHANID = 62,
    IEEE80211_ELEMID_WAPI       = 68,
    IEEE80211_ELEMID_2040_BSS_COEX = 72,
    IEEE80211_ELEMID_TPC        = 150,
    IEEE80211_ELEMID_CCKM       = 156,
    IEEE80211_ELEMID_VENDOR     = 221,  /* vendor private */
};

/******************************** STA ********************************/
typedef enum {
    PAIRWISE_USAGE      = 0x00,
    GROUP_USAGE         = 0x01,
    TX_USAGE            = 0x02,     /* default Tx Key - Static WEP only */
} KEY_USAGE;

typedef enum {
    CONNECT_ASSOC_POLICY_USER           = 0x0001,
    CONNECT_SEND_REASSOC                = 0x0002,
    CONNECT_IGNORE_WPAx_GROUP_CIPHER    = 0x0004,
    CONNECT_PROFILE_MATCH_DONE          = 0x0008,
    CONNECT_IGNORE_AAC_BEACON           = 0x0010,
    CONNECT_CSA_FOLLOW_BSS              = 0x0020,
    CONNECT_DO_WPA_OFFLOAD              = 0x0040,
    CONNECT_DO_NOT_DEAUTH               = 0x0080,
    CONNECT_WPS_FLAG                    = 0x0100,
    CONNECT_IGNORE_BSSID_HINT           = 0x0200,
    CONNECT_STAY_AWAKE                  = 0x0400, 
    CONNECT_SAFE_MODE			= 0x0800,
    /* AP configuration flags */
    AP_NO_DISASSOC_UPON_DEAUTH          = 0x10000,
    AP_HOSTPAL_SUPPORT                  = 0x20000
} WMI_CONNECT_CTRL_FLAGS_BITS;

#define KEY_OP_INIT_VAL     0x03     /* Default Initialise the TSC & RSC */

extern qcom_timer_t reconnect_timer[WLAN_NUM_OF_DEVICES];
extern A_UINT8 wlanFlagWmiconfigConnect[WLAN_NUM_OF_DEVICES];
extern A_UINT8 wlanFlagWmiconfigScan[WLAN_NUM_OF_DEVICES];
extern A_UINT8 wlanFlagGetScan[WLAN_NUM_OF_DEVICES];
extern A_UINT32 wlanDbgMod;

#define QCOM_STA_WLAN_RECONNECT_INTERVAL 10

extern A_UINT32 qcom_sta_wlan_reconnect_cnt[WLAN_NUM_OF_DEVICES];
extern A_UINT32 qcom_sta_wlan_reconnect_next_milestone[WLAN_NUM_OF_DEVICES];
extern A_UINT8 qcom_sta_wlan_reconnect_start_flag[WLAN_NUM_OF_DEVICES];

/******************************** AP ********************************/
#define MAX_SCAN_AP_LIST 40
#define WMI_PASSPHRASE_LEN    64
#define WMI_MAX_SSID_LEN    32

/******************************** COM ********************************/
 typedef void (*WMI_CONNECT_EVENT_CB)(A_UINT8 device_id, int value, A_UINT8 * pEvtBuffer);
 typedef void (*WMI_HTTP_POST_EVENT_CB)(void* cxt, void* buf);
 typedef void (*WMI_HTTP_GET_EVENT_CB)(void* cxt, void* buf);
 typedef void (*WMI_HTTP_PUT_EVENT_CB)(void* cxt, void* buf);
 typedef void (*WMI_HTTP_DELETE_EVENT_CB)(void* cxt, void* buf);
 typedef void (*WMI_HTTP_OTA_EVENT_CB)(QCOM_OTA_STATUS_CODE_t status,A_INT32 image_size);
 
/******************************** BTCOEX ********************************/
typedef void (*WMI_BTCOEX_CONFIG_EVENT_CB)(A_UINT8 device_id, A_UINT8 * pEvtBuffer, int length);
typedef void (*WMI_BTCOEX_STATS_EVENT_CB)(A_UINT8 device_id, A_UINT8 * pEvtBuffer, int length);

typedef enum {
    WMI_WLAN_11A_MODE = 0x1,
    WMI_WLAN_11G_MODE = 0x2,
    WMI_WLAN_11AG_MODE = 0x3,
    WMI_WLAN_11B_MODE = 0x4,
    WMI_WLAN_11GONLY_MODE = 0x5,
    WMI_WLAN_INVALID_MODE,
} WMI_WLAN_DEV_PHY_MODE;

/******************************** EVENT ********************************/
typedef PREPACK struct {
    A_UINT16    protocolReasonStatus;  /* reason code, see 802.11 spec. */
    A_UINT8     bssid[ATH_MAC_LEN];    /* set if known */
    A_UINT8     disconnectReason ;      /* see WMI_DISCONNECT_REASON */
    A_UINT8     assocRespLen;
    A_UINT8     assocInfo[1];
} POSTPACK WMI_DISCONNECT_EVENT;
typedef PREPACK struct {
    PREPACK union {
        struct {
            A_UINT16    channel;
            A_UINT8     bssid[ATH_MAC_LEN];
            A_UINT16    listenInterval;
            A_UINT16    beaconInterval;
            A_UINT16    networkType;
            A_UINT8     tx_scheduler_enabled; /* 0x5A means enabled */
            A_UINT8     aid;
        } infra_ibss_bss;
        struct {
            A_UINT8     phymode;
            A_UINT8     aid;
            A_UINT8     mac_addr[ATH_MAC_LEN];
            A_UINT8     auth;
            A_UINT8     keymgmt;
            A_UINT16    cipher;
            A_UINT8     apsd_info;
            A_UINT8     unused[3];
        } ap_sta;
        struct {
            A_UINT16    channel;
            A_UINT8     bssid[ATH_MAC_LEN];
            A_UINT8     tx_scheduler_enabled; /* 0x5A means enabled */
            A_UINT8     aid;
            A_UINT8     ht_info;
            A_UINT8     unused[1];
            A_UINT32    networkType;
        } ap_bss;
    } POSTPACK u;
    A_UINT8     beaconIeLen;
    A_UINT8     assocReqLen;
    A_UINT8     assocRespLen;
    A_UINT8     assocInfo[1];
} POSTPACK WMI_CONNECT_EVENT;

#define SW_DBG(fmt, args ...)

typedef enum {
    WMI_READY_EVENTID                    = 0x1001,
    WMI_CONNECT_EVENTID,
    WMI_DISCONNECT_EVENTID,
    WMI_BSSINFO_EVENTID,
    WMI_CMDERROR_EVENTID,
    WMI_REGDOMAIN_EVENTID,
    WMI_PSTREAM_TIMEOUT_EVENTID,
    WMI_NEIGHBOR_REPORT_EVENTID,
    WMI_TKIP_MICERR_EVENTID,
    WMI_SCAN_COMPLETE_EVENTID,           /* 0x100a */
    WMI_REPORT_STATISTICS_EVENTID,
    WMI_RSSI_THRESHOLD_EVENTID,
    WMI_ERROR_REPORT_EVENTID,
    WMI_OPT_RX_FRAME_EVENTID,
    WMI_REPORT_ROAM_TBL_EVENTID,
    WMI_EXTENSION_EVENTID,               /* 0x1010 */
    WMI_CAC_EVENTID,
    WMI_SNR_THRESHOLD_EVENTID,
    WMI_LQ_THRESHOLD_EVENTID,
    WMI_TX_RETRY_ERR_EVENTID,            /* 0x1014 */
    WMI_REPORT_ROAM_DATA_EVENTID,
    WMI_TEST_EVENTID,
    WMI_APLIST_EVENTID,
    WMI_GET_WOW_LIST_EVENTID,
    WMI_GET_PMKID_LIST_EVENTID,
    WMI_CHANNEL_CHANGE_EVENTID,          /* 0x101a */
    WMI_PEER_NODE_EVENTID,
    WMI_PSPOLL_EVENTID,
    WMI_DTIMEXPIRY_EVENTID,
    WMI_WLAN_VERSION_EVENTID,
    WMI_SET_COUNTRY_CODE_REPLY_EVENTID,
    WMI_ADDBA_REQ_EVENTID,               /*0x1020 */
    WMI_ADDBA_RESP_EVENTID,
    WMI_DELBA_REQ_EVENTID,
    WMI_TX_COMPLETE_EVENTID,
    WMI_HCI_EVENT_EVENTID,
    WMI_ACL_DATA_EVENTID,
    WMI_REPORT_SLEEP_STATE_EVENTID,
    WMI_WAPI_REKEY_EVENTID,
    WMI_REPORT_BTCOEX_STATS_EVENTID,
    WMI_REPORT_BTCOEX_CONFIG_EVENTID,
    WMI_GET_PMK_EVENTID,                 /* 0x102a */

    /* DFS Events */
    WMI_DFS_HOST_ATTACH_EVENTID,         /* 0x102B */
    WMI_DFS_HOST_INIT_EVENTID,
    WMI_DFS_RESET_DELAYLINES_EVENTID,
    WMI_DFS_RESET_RADARQ_EVENTID,
    WMI_DFS_RESET_AR_EVENTID,
    WMI_DFS_RESET_ARQ_EVENTID,           /* 0x1030 */
    WMI_DFS_SET_DUR_MULTIPLIER_EVENTID,
    WMI_DFS_SET_BANGRADAR_EVENTID,
    WMI_DFS_SET_DEBUGLEVEL_EVENTID,
    WMI_DFS_PHYERR_EVENTID,
    /*  Evants */
    WMI_C2X_RM_STATUS_EVENTID,           /* 0x1035 */

    /* P2P Events */
    WMI_P2P_GO_NEG_RESULT_EVENTID,       /* 0x1036 */

    WMI_WAC_SCAN_DONE_EVENTID,
    WMI_WAC_REPORT_BSS_EVENTID,
    WMI_WAC_START_WPS_EVENTID,
    WMI_WAC_CTRL_REQ_REPLY_EVENTID,      /* 0x103a */
    WMI_REPORT_WMM_PARAMS_EVENTID,
    WMI_WAC_REJECT_WPS_EVENTID,
        
    /* More P2P Events */
    WMI_P2P_GO_NEG_REQ_EVENTID,
    WMI_P2P_INVITE_REQ_EVENTID,
    WMI_P2P_INVITE_RCVD_RESULT_EVENTID,
    WMI_P2P_INVITE_SENT_RESULT_EVENTID,  /* 0x1040 */
    WMI_P2P_PROV_DISC_RESP_EVENTID,
    WMI_P2P_PROV_DISC_REQ_EVENTID,

    /*RFKILL Events*/
    WMI_RFKILL_STATE_CHANGE_EVENTID,
    WMI_RFKILL_GET_MODE_CMD_EVENTID,

    WMI_P2P_START_SDPD_EVENTID,
    WMI_P2P_SDPD_RX_EVENTID,

    /* Special event used to notify host that AR6003
     * has processed sleep command (needed to prevent
     * a late incoming credit report from crashing
     * the system)
     */
    WMI_SET_HOST_SLEEP_MODE_CMD_PROCESSED_EVENTID,   /* 0x1047 */

    WMI_THIN_RESERVED_START_EVENTID                   = 0x8000,
    /* Events in this range are reserved for thinmode
     * See wmi_thin.h for actual definitions */
    WMI_THIN_RESERVED_END_EVENTID                     = 0x8fff,

    WMI_SET_CHANNEL_EVENTID,                         /* 0x9000 */
    WMI_ASSOC_REQ_EVENTID, 

    /* generic ACS event */
    WMI_ACS_EVENTID,                                 /* 0x9002 */
    WMI_STORERECALL_STORE_EVENTID,
    WMI_WOW_EXT_WAKE_EVENTID,
    WMI_GTK_OFFLOAD_STATUS_EVENTID,
    WMI_NETWORK_LIST_OFFLOAD_EVENTID,
    WMI_REMAIN_ON_CHNL_EVENTID,                      /* 0x9007 */
    WMI_CANCEL_REMAIN_ON_CHNL_EVENTID,               /* 0x9008 */           
    WMI_TX_STATUS_EVENTID,
    WMI_RX_PROBE_REQ_EVENTID,                        /* 0x900a */
    WMI_P2P_CAPABILITIES_EVENTID,                    /* 0x900b */
    WMI_RX_ACTION_EVENTID,
    WMI_P2P_INFO_EVENTID,
    /* WPS Events */ 
    WMI_WPS_GET_STATUS_EVENTID,
    WMI_WPS_PROFILE_EVENTID,  

    /* more P2P events */
    WMI_NOA_INFO_EVENTID,                            /* 0x9010 */
    WMI_OPPPS_INFO_EVENTID,                          
    WMI_PORT_STATUS_EVENTID,

    /* 802.11w */
    WMI_GET_RSN_CAP_EVENTID,
    
    WMI_FLOWCTRL_IND_EVENTID,
    WMI_FLOWCTRL_UAPSD_FRAME_DILIVERED_EVENTID,

    /*Socket Translation Events*/
    WMI_SOCKET_RESPONSE_EVENTID,

	WMI_LOG_FRAME_EVENTID,
    WMI_QUERY_PHY_INFO_EVENTID,
    WMI_C2X_ROAMING_EVENTID,	
	
    WMI_P2P_NODE_LIST_EVENTID,                       /* 0x901A */
    WMI_P2P_REQ_TO_AUTH_EVENTID,                     
    WMI_DIAGNOSTIC_EVENTID, /* diagnostic */
    WMI_DISC_PEER_EVENTID,  /* wifi discovery */
    WMI_BSS_RSSI_INFO_EVENTID,
    WMI_ARGOS_EVENTID,
    WMI_AP_IDLE_CLOSE_TIMEOUT_EVENTID = 0x9020,
    WMI_SEND_DUMMY_DATA_EVENTID,
    WMI_FLUSH_BUFFERED_DATA_EVENTID,
    WMI_WLAN_INFO_LTE_EVENTID,
    WMI_CLIENT_POWER_SAVE_EVENTID,

    /* cusomter private events */
    WMI_GET_REG_EVENTID,	/* 0x9025 */
    WMI_GET_STAINFO_EVENTID,
    WMI_GET_TXPOW_EVENTID,
    WMI_GET_VERSION_INFO_EVENTID,
    WMI_GET_TESTMODE_EVENTID,
    WMI_RX_PROBE_RESP_EVENTID,
    WMI_ACL_REJECT_EVENTID,
    WMI_GET_WIDIMODE_EVENTID,/* 0x902C */
    WMI_CSA_EVENTID,
    WMI_GET_CTL_EVENTID,
    WMI_PACKET_DROP_EVENTID,
    WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID, /* 0x9030 */
    
    WMI_HOST_DSET_STORE_EVENTID,	     /* 0x9031 */
    WMI_HOST_DSET_READ_EVENTID,
    WMI_STORERECALL_START_EVENTID,

    /* i2c, i2s related IDs */
	
    /* IDs for i2c operation */
    WMI_I2C_ACK_EVENTID,
    WMI_I2C_READ_DATA_EVENTID,

    /* IDs for i2c operation */
    WMI_I2S_ACK_EVENTID,
    WMI_I2S_RCV_DATA_EVENTID,

    WMI_HOST_DSET_CREATE_EVENTID,	   /* 0x9038   */
    WMI_HOST_DSET_LARGE_WRITE_EVENTID,     /* 0x9039   */
    WMI_HOST_DSET_LARGE_READ_EVENTID,      /* 0x903A   */
    WMI_HOST_DSET_SYNC_EVENTID,

    WMI_DSET_OP_EVENTID,                   /* 0x903C */
    WMI_GET_TEMPERATURE_REPLY_EVENTID,     /*0x903D*/
	WMI_ADC_RESPONSE_EVENTID,			   /*0x903E*/
    WMI_PARAM_SET_REPLY_EVENTID,           /*0x903F*/
    WMI_PARAM_GET_REPLY_EVENTID,           /*0x9040*/
    WMI_AP_GET_STA_INFO_REPLY_EVENTID,  
    WMI_GET_HWANTDIVSTAT_EVENTID,			/*0x9042*/
} WMI_EVENT_ID;

/*
 * Disconnect Event
 */
typedef enum {
    NO_NETWORK_AVAIL   = 0x01,
    LOST_LINK          = 0x02,     /* bmiss */
    DISCONNECT_CMD     = 0x03,
    BSS_DISCONNECTED   = 0x04,
    AUTH_FAILED        = 0x05,
    ASSOC_FAILED       = 0x06,
    NO_RESOURCES_AVAIL = 0x07,
    CSERV_DISCONNECT   = 0x08,
    INVALID_PROFILE    = 0x0a,
    DOT11H_CHANNEL_SWITCH = 0x0b,
    PROFILE_MISMATCH   = 0x0c,
    CONNECTION_EVICTED = 0x0d,
    IBSS_MERGE         = 0x0e,
    EXCESS_TX_RETRY    = 0x0f,     /* TX frames failed after excessive retries */
    SEC_HS_TO_RECV_M1  = 0x10,     /* Security 4-way handshake timed out waiting for M1 */
    SEC_HS_TO_RECV_M3  = 0x11,     /* Security 4-way handshake timed out waiting for M3 */
    TKIP_COUNTERMEASURES = 0x12,
    CCKM_ROAMING_INDICATION = 0xfe,  /* hack for CCKM fast roaming */
} WMI_DISCONNECT_REASON;

typedef PREPACK struct {
    A_INT8 status;      /* WMI_SET_COUNTRY_CODE_REPLY */
} POSTPACK WMI_SET_COUNTRY_CODE_REPLY;

typedef PREPACK struct {
    A_UINT32   tx_packets;
    A_UINT32   tx_bytes;
    A_UINT32   tx_unicast_pkts;
    A_UINT32   tx_unicast_bytes;
    A_UINT32   tx_multicast_pkts;
    A_UINT32   tx_multicast_bytes;
    A_UINT32   tx_broadcast_pkts;
    A_UINT32   tx_broadcast_bytes;
    A_UINT32   tx_rts_success_cnt;
    A_UINT32   tx_packet_per_ac[4];
    A_UINT32   tx_errors_per_ac[4];

    A_UINT32   tx_errors;
    A_UINT32   tx_failed_cnt;
    A_UINT32   tx_retry_cnt;
    A_UINT32   tx_mult_retry_cnt;
    A_UINT32   tx_rts_fail_cnt;
    A_INT32    tx_unicast_rate;
}POSTPACK tx_stats_t;

typedef PREPACK struct {
    A_UINT32   rx_packets;
    A_UINT32   rx_bytes;
    A_UINT32   rx_unicast_pkts;
    A_UINT32   rx_unicast_bytes;
    A_UINT32   rx_multicast_pkts;
    A_UINT32   rx_multicast_bytes;
    A_UINT32   rx_broadcast_pkts;
    A_UINT32   rx_broadcast_bytes;
    A_UINT32   rx_fragment_pkt;

    A_UINT32   rx_errors;
    A_UINT32   rx_crcerr;
    A_UINT32   rx_key_cache_miss;
    A_UINT32   rx_decrypt_err;
    A_UINT32   rx_duplicate_frames;
    A_INT32    rx_unicast_rate;
}POSTPACK rx_stats_t;

typedef PREPACK struct {
    A_UINT32   tkip_local_mic_failure;
    A_UINT32   tkip_counter_measures_invoked;
    A_UINT32   tkip_replays;
    A_UINT32   tkip_format_errors;
    A_UINT32   ccmp_format_errors;
    A_UINT32   ccmp_replays;
}POSTPACK tkip_ccmp_stats_t;

typedef PREPACK struct {
    A_UINT32   power_save_failure_cnt;
    A_UINT16   stop_tx_failure_cnt;
    A_UINT16   atim_tx_failure_cnt;
    A_UINT16   atim_rx_failure_cnt;
    A_UINT16   bcn_rx_failure_cnt;
}POSTPACK pm_stats_t;

typedef PREPACK struct {
    A_UINT32    cs_bmiss_cnt;
    A_UINT32    cs_lowRssi_cnt;
    A_UINT16    cs_connect_cnt;
    A_UINT16    cs_disconnect_cnt;
    A_INT16     cs_aveBeacon_rssi;
    A_UINT16    cs_roam_count;
    A_INT16     cs_rssi;
    A_UINT8     cs_snr;
    A_UINT8     cs_aveBeacon_snr;
    A_UINT8     cs_lastRoam_msec;
} POSTPACK cserv_stats_t;

typedef PREPACK struct {
    tx_stats_t          tx_stats;
    rx_stats_t          rx_stats;
    tkip_ccmp_stats_t   tkipCcmpStats;
}POSTPACK wlan_net_stats_t;

typedef PREPACK struct {
    A_UINT32    arp_received;
    A_UINT32    arp_matched;
    A_UINT32    arp_replied;
} POSTPACK arp_stats_t;

typedef PREPACK struct {
    A_UINT32    wow_num_pkts_dropped;
    A_UINT16    wow_num_events_discarded;
    A_UINT8     wow_num_host_pkt_wakeups;
    A_UINT8     wow_num_host_event_wakeups;
} POSTPACK wlan_wow_stats_t;

typedef PREPACK struct {
    A_UINT32            lqVal;
    A_INT32             noise_floor_calibation;
    pm_stats_t          pmStats;
    wlan_net_stats_t    txrxStats;
    wlan_wow_stats_t    wowStats;
    arp_stats_t         arpStats;
    cserv_stats_t       cservStats;
} POSTPACK WMI_TARGET_STATS;

/*
 * Control Path
 */
typedef PREPACK struct {
    A_UINT16    commandId;
/*
 * info1 - 16 bits
 * b03:b00 - id
 * b15:b04 - unused
 */
    A_UINT16    info1;

    A_UINT16    reserved;      /* For alignment */
} POSTPACK WMI_CMD_HDR;        /* used for commands and events */


/* P2P module events */
typedef P2P_GO_NEG_REQ_EVENT WMI_P2P_GO_NEG_REQ_EVENT;
typedef P2P_GO_NEG_RESULT_EVENT WMI_P2P_GO_NEG_RESULT_EVENT;
typedef P2P_INVITE_REQ_EVENT WMI_P2P_INVITE_REQ_EVENT;
typedef P2P_FW_INVITE_REQ_EVENT WMI_P2P_FW_INVITE_REQ_EVENT;
typedef P2P_INVITE_RCVD_RESULT_EVENT WMI_P2P_INVITE_RCVD_RESULT_EVENT;
typedef P2P_INVITE_SENT_RESULT_EVENT WMI_P2P_INVITE_SENT_RESULT_EVENT;
typedef P2P_PROV_DISC_REQ_EVENT WMI_P2P_PROV_DISC_REQ_EVENT;
typedef P2P_PROV_DISC_RESP_EVENT WMI_P2P_PROV_DISC_RESP_EVENT;
typedef P2P_SDPD_RX_EVENT WMI_P2P_SDPD_RX_EVENT;
typedef P2P_NODE_LIST_EVENT WMI_P2P_NODE_LIST_EVENT;
typedef P2P_GRP_INIT_CMD WMI_P2P_GRP_INIT_CMD;
typedef P2P_REQ_TO_AUTH_EVENT WMI_P2P_REQ_TO_AUTH_EVENT;
typedef P2P_PERSISTENT_MAC_LIST WMI_PERSISTENT_MAC_LIST;

typedef struct _device_context_s
{
   //dev id
   A_UINT8 deviceId; 
  //scan
  A_UINT32 scanmode; 
  A_UINT32 scanOutSize;
  A_UINT32 scanOutCount;
  A_UINT8 *pScanOut;
  A_UINT16 max_dwell_time;
  A_UINT16 pass_dwell_time;
   
  //station
  A_UINT32 listentime;
  A_UINT8 rssi;
  
  //ap
  A_UINT32 beacon_interval;
  A_UINT8 hiddenFlag;
  A_UINT8 sta_num;
  A_UINT32 apflg;
  
  //common
  A_UINT8 forceDisc;
#if defined(AR6002_REV74)
  A_UINT8 dummy[8]; /* Deprecated country code API */
#endif
  A_UINT16 tx_allow_aggr;
  A_UINT16 rx_allow_aggr;
  A_UINT16 channel;
  A_UINT8 chipState;
  A_UINT8 chipMode; // 1:station 0:ap
  A_UINT8 phymode;
  A_UINT32 tx_power;
  
  //sec
  WLAN_AUTH_MODE enath_mode;
  WLAN_CRYPT_TYPE enencrypt_mode;
  A_UINT32 wepmode;  //0 open 1 share
  A_UINT32 keyindex; //wep key index, start from 1
  A_UINT8 wepPassword[4][65];  //save four wep keys
  A_UINT8 ssid[WMI_MAX_SSID_LEN+1];
  A_UINT8 pass[65];
  
  //net
  
  //power
  A_UINT8 powermode; /* 2:MAX_PERF_POWER 1:REC_POWER*/
  
  //wps
  A_UINT8 wpsFlag;
  
  //misc
  A_UINT8 bssid[ATH_MAC_LEN];
  WMI_CONNECT_EVENT_CB connectStateCB;
  WMI_HTTP_POST_EVENT_CB httpPostCb;  /*callback handler for http post events*/
  void* httpPostCbCxt; 
#if !defined(AR6002_REV76)
  WMI_HTTP_PUT_EVENT_CB httpPutCb;  /*callback handler for http post events*/
  void* httpPutCbCxt; 
  WMI_HTTP_DELETE_EVENT_CB httpDeleteCb;  /*callback handler for http post events*/
  void* httpDeleteCbCxt;   
  WMI_HTTP_GET_EVENT_CB httpGetCb;  /*callback handler for http get events*/
  void* httpGetCbCxt; 
  WMI_HTTP_OTA_EVENT_CB httpOtaCb;/*callback handler for http server event*/
  A_UINT8* sta_info;
  WMI_BTCOEX_CONFIG_EVENT_CB BtcoexConfigCB;
  WMI_BTCOEX_STATS_EVENT_CB BtcoexStatsCB;
#endif
} DEVICE_CONTEXT_T;

typedef enum {
    QCOM_WLAN_LINK_STATE_DISCONNECTED_STATE = 0,
    QCOM_WLAN_LINK_STATE_STARTING_STATE,
    QCOM_WLAN_LINK_STATE_CONNECTING_STATE,
    QCOM_WLAN_LINK_STATE_AUTHENTICATING_STATE,
    QCOM_WLAN_LINK_STATE_CONNECTED_STATE,
    QCOM_WLAN_LINK_STATE_DISCONNECTING_STATE,
    QCOM_WLAN_LINK_STATE_WEP_KEY_NOT_MATCH,
} QCOM_WLAN_LINK_STATE;


extern A_UINT8 gNumOfWlanDevices;
extern DEVICE_CONTEXT_T *gDeviceContextPtr[WLAN_NUM_OF_DEVICES];

/* Network */
#define  AF_INET     2     /* internetwork: UDP, TCP, etc. */
#define  AF_INET6    3     /* IPv6 */
#define  AF_INET_ALL 255   /* only used by CLI code to indicate */
//#if !defined(CONFIG_HOSTLESS) || !defined(REV74_TEST_ENV2)
//#define IP_V6 1
//#endif

#define lswap(n)	\
	((((n) & 0xff) << 24) | (((n) & 0xff00) << 8) | (((n) & 0xff0000UL) >> 8) | (((n) & 0xff000000UL) >> 24))
#define htonl(l) lswap(l)
#define ntohl(l) lswap(l)

#define MAX_DNSADDRS 3
#define MAX_URLLENGTH 256
#define HTTPSVR_URI_FILENAMELEN  32

enum SOCKET_CMDS
{
    SOCK_OPEN,          /* 00 */
    SOCK_CLOSE,         /* 01 */
    SOCK_CONNECT,       /* 02 */
    SOCK_BIND,          /* 03 */
    SOCK_LISTEN,        /* 04 */
    SOCK_ACCEPT,        /* 05 */
    SOCK_SELECT,        /* 06 */
    SOCK_SETSOCKOPT,    /* 07 */
    SOCK_GETSOCKOPT,    /* 08 */
    SOCK_ERRNO,         /* 09 */
    SOCK_IPCONFIG,      /* 10 */
    SOCK_PING,          /* 11 */
    SOCK_STACK_INIT,    /* 12 */
    SOCK_STACK_MISC,    /* 13 */
    SOCK_PING6,         /* 14 */
    SOCK_IP6CONFIG,     /* 15 */
    SOCK_IPCONFIG_DHCP_POOL,            /* 16 */
    SOCK_IP6CONFIG_ROUTER_PREFIX,       /* 17 */
    SOCK_IP_SET_TCP_EXP_BACKOFF_RETRY,  /* 18 */
    SOCK_IP_SET_IP6_STATUS,             /* 19 */  /* set ip6 module status enable/disable */
    SOCK_IPCONFIG_DHCP_RELEASE,         /* 20 */  /* Release the DHCP IP Addres */
    SOCK_IP_SET_TCP_RX_BUF,             /* 21 */  /* set tcp rx buffer space */
    SOCK_HTTP_SERVER,                   /* 22 */  /* HTTP Server Commands */
    SOCK_HTTP_SERVER_CMD,               /* 23 */  /* Commands to get and post data */
    SOCK_DNC_CMD,                       /* 24 */  /* DNC Resolver comamnds */
    SOCK_DNC_ENABLE,                    /* 25 */  /* Enable/Disable DNS Client */
    SOCK_DNS_SERVER_ADDR,               /* 26 */  /* IP Address to configure DNS Server Address */
    SOCK_HTTPC_CMD,                     /* 27 */  /* HTTP Client commands */
    SOCK_DNS_LOCAL_DOMAIN,              /* 28 */  /* To configure local domain */
    SOCK_IP_HOST_NAME,                  /* 29 */  /* To configure host name */
    SOCK_IP_DNS,                        /* 30 */  /* To configure dns database */
    SOCK_IP_SNTP_SRVR_ADDR,             /* 31 */  /* To configure sntp srvr addr */
    SOCK_IP_SNTP_GET_TIME,              /* 32 */  /* To Get time from SNTP */
    SOCK_IP_SNTP_GET_TIME_OF_DAY,       /* 33 */  /* To Get time of day from SNTP */
    SOCK_IP_SNTP_MODIFY_TIMEZONE_DSE,   /* 34 */  /* To get the timestamp based on zone */
    SOCK_IP_SNTP_QUERY_SNTP_ADDRESS,    /* 35 */  /* To get the SNTP server address*/  
    SOCK_IP_SNTP_CLIENT_ENABLE,         /* 36 */  /* To start/stop SNTP client */
    SOCK_IPV4_ROUTE,                    /* 37 */  /* Commands to add,del and show IPv4 routes */
    SOCK_IPV6_ROUTE,                    /* 38 */  /* Commands to add,del and show IPv6 routes */
    SOCK_IP_BRIDGEMODE,                 /* 39 */  /* Commands to enable bridge mode */
    SOCK_DNS_SERVER_ENABLE,             /* 40 */  /* To start/stop DNS Server*/
    SOCK_SSL_CTX_NEW,                   /* 41 */  /* Create a new SSL context */
    SOCK_SSL_CTX_FREE,       /* Free/close SSL context */
    SOCK_SSL_NEW,            /* Create new SSL connection object/instance */
    SOCK_SSL_SET_FD,         /* Add socket handle to a SSL connection */
    SOCK_SSL_ACCEPT,         /* Accept SSL connection request from SSL client */
    SOCK_SSL_CONNECT,        /* Establish SSL connection from SSL client to SSL server */
    SOCK_SSL_SHUTDOWN,       /* Shutdown/close SSL connection */
    SOCK_SSL_ADD_CERT,       /* Add a certificate to SSL context */
    SOCK_SSL_STORE_CERT,     /* Store a certificate or CA list file in FLASH */
    SOCK_SSL_LOAD_CERT,      /* Read a certificate or CA list from FLASH and adds it to SSL context */
    SOCK_SSL_CONFIGURE,      /* Configure a SSL connection */
    SOCK_SSL_LIST_CERT,       /* List the names of the certificates or CA lists stored in flash */
    SOCK_GET_DEV_ID_FROM_DEST_IP, /* 53 *//* Gets the device ID for the given destination IP address */       
    SOCK_OTA_UPGRADE,             /* 54 *//*Intiate OTA Upgrade*/     
    SOCK_OTA_READ,               /* 55*//*Read from OTA Area*/ 
    SOCK_OTA_DONE,              /*56 */ /*Send OTA done to target*/
    SOCK_SET_DHCP_HOSTNAME,     /*57*/ /*set the dhcp hostname*/
    SOCK_TCP_CONN_TIMEOUT,       /*58*//*TCP Connection Timeout */
    SOCK_HTTP_POST_EVENT,       /*59*/ /*HTTP server post event*/
    SOCK_HTTP_PUT_EVENT,        /*60*/ /*HTTP server put event*/
    SOCK_HTTP_DELETE_EVENT,     /*61*/ /*HTTP server delete event*/
    SOCK_OTA_SESSION_START,  /*62*/ /*start OTA session */
    SOCK_OTA_PARTITION_GET_SIZE,    /*63*/ /*start OTA session */
    SOCK_OTA_PARTITION_ERASE,    /*64*/ /*erase OTA partition */
    SOCK_OTA_PARSE_IMAGE_HDR,  /*65*/ /*parse OTA image header */
    SOCK_OTA_PARTITION_VERIFY_CHECKSUM,  /*66*/ /*veirfy OTA partition checksum */
    SOCK_OTA_PARTITION_WRITE_DATA,  /*67*/ /*write OTA partition data */
    SOCK_OTA_PARTITION_ERASE_SECTORS,    /*68*/ /*erase OTA partition sectors if needed */
    SOCK_DHCPS_SUCCESS_CALLBACK,    /*69*/ /*DHCP Server Success Callback*/
    SOCK_DHCPC_SUCCESS_CALLBACK,    /*70*/ /*DHCP Client Success Callback*/
    SOCK_SSDP_INIT, 			       /*71*/ /*SSDP Daemon init & config */	
    SOCK_SSDP_ENABLE,		       /*72*//*SSDP Enable*/	
    SOCK_SSDP_NOTIFY_CHANGE,	/*73*//*SSDP Notify change*/    
    SOCK_HTTP_GET_EVENT,        /*74*//*HTTP Server Get event*/
    SOCK_HTTP_GET_DATASEND,     /*75*//*App send data to target for HTTP Server Get Command*/
    SOCK_HTTPC_BODY_CMD,          /* 76 */  /* HTTP Client commands */
    SOCK_HTTP_SET_CUSTOM_URL,     /* 77 */  /* HTTP Server set custom URL */
    SOCK_HTTP_RESTRICT_EVENT,       /*78*//*HTTP Server restrict request*/
    SOCK_HTTP_REDIRECT_UNKNOWN_URL,    /*79*//* HTTP Server redirect unknwon url */
    SOCK_HTTP_ADD_REDIECTED_PAGE,        /*80*//* HTTP Server receive http page file */
    SOCK_HTTP_SET_REDIECTED_URL,            /*81*//* HTTP Server set redirected url */
    SOCK_SSL_SET_TM,                /*82*/ /*ssl set tm*/
    SOCK_SSL_CTX_CONFIGURE,                    /*83*//* Configure a SSL context */
    SOCK_HTTPC_REGISTER_CB_CMD = 84,          /* 84 */  /* HTTP Client register callback event */
    SOCK_HTTPC_CONNECT_CMD,                /*85*/ /* http client connect cmd*/
    SOCK_HTTPC_DISCONNECT_CMD,            /*86*/ /* http client disconnect cmd*/
    SOCK_HTTPC_REQUEST_CMD,               /*87*/ /* http client request cmd*/
    SOCK_HTTPC_SET_BODY_CMD,               /*88*/ /* http client set body cmd*/
    SOCK_HTTPC_SET_PARAM_CMD,            /*89*/ /* http client set parameter cmd*/
    SOCK_HTTPC_ADD_HEADER_CMD,            /*90*/ /* http client add header cmd*/
    SOCK_HTTPC_CLEAR_HEADER_CMD,            /*91*/ /* http client clear hheader cmd*/
    SOCK_SSL_REUSE_CERT,     /*92*/  /* resue the cert of the other SSL context */
    SOCK_HTTP_SET_OTA_URI,     /* 93 */  /* HTTP Server set OTA URI */
    SOCK_HTTP_OTA_ENABLE,     /* 94 */  /* HTTP Server enable/disable OTA */
    SOCK_HTTP_OTA_UPGRADE,     /* 95 */  /*Intiate HTTP Server OTA Upgrade*/
    SOCK_SSL_DTLS_CONFIGURE,     /*96*/  /* Configure DTLS connection */
    SOCK_SSL_DTLS_NEW,     /*97*/  /* Create new DTLS connection */
    SOCK_AUTOIP_SUCCESS_CALLBACK,    /*98*/ /*auto IP Client Success Callback*/
    SOCK_SSL_DTLS_RESET,     /*99*/  /* Reset SSL connection and notify peer */
};

typedef PREPACK struct ip6_addr {
   A_UINT8   addr[16];   /* 128 bit IPv6 address */
}POSTPACK IP6_ADDR_T;


typedef PREPACK struct _sockaddr {
    A_UINT16 sin_port ;
    A_UINT16 sin_family ;
    A_UINT32 sin_addr ;
} POSTPACK SOCKADDR_T;


typedef PREPACK struct sockaddr_6
{
   A_UINT16        sin6_family ;   /* AF_INET6 */
   A_UINT16        sin6_port ;     /* transport layer port # */
   A_UINT32        sin6_flowinfo ; /* IPv6 flow information */
   IP6_ADDR_T     sin6_addr ;     /* IPv6 address */
   A_UINT32        sin6_scope_id ; /* set of interfaces for a scope */
}POSTPACK SOCKADDR_6_T;

typedef PREPACK struct ipv46addr
{
   A_UINT8 type FIELD_PACKED;
   A_UINT8 u1Rsvd[3] FIELD_PACKED;
   A_UINT32 addr4 FIELD_PACKED;
   IP6_ADDR_T addr6 FIELD_PACKED;
} POSTPACK IPV46ADDR;

typedef PREPACK struct ipconfig {
    A_UINT32  mode FIELD_PACKED;      //0-query, 1-static, 2-dhcp
    A_UINT32 ipv4 FIELD_PACKED;
    A_UINT32 subnetMask FIELD_PACKED;
    A_UINT32 gateway4 FIELD_PACKED;         
    IP6_ADDR_T  ipv6LinkAddr FIELD_PACKED;
    IP6_ADDR_T  ipv6GlobalAddr FIELD_PACKED;
    IP6_ADDR_T  ipv6DefGw FIELD_PACKED;
    IP6_ADDR_T  ipv6ExtdGlobalAddr FIELD_PACKED; // used for IPv6 LOGO where u need to 2 prefixs
    A_INT32     LinkPrefix FIELD_PACKED;
    A_INT32     GlobalPrefix FIELD_PACKED;
    A_INT32     DefGwPrefix FIELD_PACKED; 
    A_INT32     ExtdGlobalPrefix FIELD_PACKED;	
   IPV46ADDR   DnsSrvr[MAX_DNSADDRS] FIELD_PACKED; 
   A_CHAR      au1HostName[33];
   A_CHAR      sntp_srvraddr[20];
}POSTPACK IPCONFIG_T;

typedef PREPACK struct dncrespinfo {
   char dns_names[64] FIELD_PACKED;
   A_INT32    h_length FIELD_PACKED;
   A_INT32    h_addrtype FIELD_PACKED; //host address type
   A_INT32    ipaddrs FIELD_PACKED;    //count of entries in ipaddress list
   A_UINT32   ipaddrs_list[MAX_DNSADDRS] FIELD_PACKED; //list of ipv4 address
   A_INT32    ip6addrs FIELD_PACKED; //count of ip6 entries in ip6 address list
  IP6_ADDR_T ip6addrs_list[MAX_DNSADDRS] FIELD_PACKED; //list of ip6address
}POSTPACK DNC_RESP_INFO;  

typedef PREPACK struct {
    A_UINT32    resp_type;
    A_UINT32    sock_handle; //handle to open socket
    A_UINT32    result;
    A_UINT8     data[1];
} POSTPACK WMI_SOCKET_RESPONSE_EVENT;

typedef enum ip_config_mode {
    IP_CONFIG_QUERY,
    IP_CONFIG_SET,
    IP_CONFIG_DHCP,
    IP_CONFIG_AUTO,
} ip_config_mode_e;

/* Description of data base entry for a single host.  */
struct hostent
{
   char *   h_name;        /* Official name of host. */
   char **h_aliases;       /* Alias list.  */
   int   h_addrtype;       /* Host address type.  */
   int   h_length;         /* Length of address.  */
   char **h_addr_list;     /* List of addresses from name server.  */
#define  h_addr   h_addr_list[0] /* Address, for backward compatibility.  */

#if 0 //def DNS_CLIENT_UPDT
   /* Extra variables passed in to Dynamic DNS updates. */
   char *   h_z_name;      /* IN- zone name for UPDATE packet */
   ip_addr  h_add_ipaddr;  /* IN- add this ip address for host name in zone */
   u_long   h_ttl;         /* IN- time-to-live field for UPDATE packet */
#endif /* DNS_CLIENT_UPDT */
};

typedef enum 

{

  AUTOIP_DISABLED = 0,

  AUTOIP_START,

  AUTOIP_GOT_ADDRESS,

  AUTOIP_ARP_PROBE,

  AUTOIP_ARP_RESPONSE_WAIT,

  AUTOIP_ARP_ADDRESS_USED,

  AUTOIP_ARP_VERIFY_WAIT,

  AUTOIP_ARP_VERIFY_PROBE,

  AUTOIP_FAILED,

  AUTOIP_ANNOUNCE_ADDRESS,

  AUTOIP_ANNOUNCE_RESPONSE_WAIT,

  AUTOIP_PROBE_WAIT,

  AUTOIP_LAST

} eAUTO_IP_STATE;

struct autoIPext
{
   eAUTO_IP_STATE state;
   u_long      response_timer;
   ip_addr     try_address;
   unshort     probe_count;
   unshort     arp_attempts;
   unshort     announce_count;
   unshort     rate_limit;
   unshort     defense_status;
   unshort     defense_timer;
   bool_t       new_try;   
};

typedef PREPACK  struct dhs_pool
{
    A_UINT32    StartIP;         /* IP assigned to the DHCP client */
    A_UINT32    EndIP;         /* IP assigned to the DHCP client */
    A_UINT32    ServerIP;                 /* our DHCP and gateway IP */
    A_UINT32    SubNetMask;               /* subnet mask */
    A_UINT32    LocalBroadCastIP;         /* local broadcast IP */
    A_UINT32     LeaseTime;
    A_INT32        FreeIndex;
}POSTPACK DHSPOOL;


typedef PREPACK struct {
    A_UINT16 ap_channel;
    A_UINT8  ssid[WMI_MAX_SSID_LEN];
    A_UINT8  ssid_len;
    A_UINT16 auth_type; /* WPS_AUTH_TYPE */
    A_UINT16 encr_type; /* WPS_ENCR_TYPE */
    A_UINT8  key_idx;
    A_UINT8  key[WMI_PASSPHRASE_LEN];
    A_UINT8  key_len;
    A_UINT8  mac_addr[ATH_MAC_LEN];
} POSTPACK WPS_CREDENTIAL;

typedef PREPACK struct {
    A_UINT8 status;      /* WPS_STATUS */
    A_UINT8 error_code;  /* WPS_ERROR_CODE */
    WPS_CREDENTIAL  credential;
    A_UINT8  peer_dev_addr[ATH_MAC_LEN];
} POSTPACK WMI_WPS_PROFILE_EVENT;

#define MAX_OTA_AREA_READ_SIZE 1024
typedef PREPACK struct sock_ota_upgrade{
    A_UINT32 addr;
    A_CHAR source[256];
    A_UINT8 mode;
    A_UINT8 preserve_last;
    A_UINT8 protocol;  
}POSTPACK SOCK_OTA_UPGRADE_T;

typedef struct ota_info
{   A_UINT32 resp_code;
    A_UINT32 size;
}SOCK_OTA_UPGRADE_RESP_T;

typedef PREPACK struct sock_ota_upgrade_response_s{
    A_UINT32   resp_type ;
    A_UINT32   handle ;
    A_UINT32   result ;
    SOCK_OTA_UPGRADE_RESP_T  data ;
}POSTPACK WMI_OTA_UPGRADE_RESPONSE_EVENT;

typedef struct ota_read_ota_area_info
{   A_UINT32 resp_code;
    A_UINT32 size; 
    A_UCHAR *data;
}SOCK_OTA_READ_OTA_AREA_RESP_T;

typedef PREPACK struct sock_ota_read_ota_area_response_s{
    A_UINT32   resp_type ;
    A_UINT32   handle ;
    A_UINT32   result ;
    SOCK_OTA_READ_OTA_AREA_RESP_T  data ;
}POSTPACK WMI_OTA_READ_OTA_AREA_RESPONSE_EVENT;

typedef PREPACK struct sock_ota_read_ota_area{
    A_UINT32 offset;
    A_UINT32 size;
}POSTPACK SOCK_OTA_READ_OTA_AREA_T;

typedef PREPACK struct sock_ota_done{
    A_UINT32 data;
}POSTPACK SOCK_OTA_DONE_T;


typedef struct ota_done{ 
    A_UINT32 resp_code;
}SOCK_OTA_DONE_RESP_T;

typedef PREPACK struct sock_ota_done_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
       SOCK_OTA_DONE_RESP_T  data ;
}POSTPACK WMI_OTA_DONE_RESPONSE_EVENT;

typedef PREPACK struct sock_ota_session_start_s{
     A_UINT32 flags;
     A_UINT32 partition_index;
}POSTPACK SOCK_OTA_SESSION_START_T;

typedef PREPACK struct sock_ota_partition_erase_sectors_s{
     A_UINT32 offset_next;
}POSTPACK SOCK_OTA_PARTITION_ERASE_SECTORS_T;

typedef struct ota_response_s{ 
    A_UINT32 resp_code;
}SOCK_OTA_RESP_T;

typedef PREPACK struct sock_ota_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
        SOCK_OTA_RESP_T  data ;
}POSTPACK WMI_OTA_RESPONSE_EVENT;

typedef struct ota_partition_get_size_response_s{ 
    A_UINT32 resp_code;
    A_UINT32 size;
}SOCK_OTA_PARTITION_GET_SIZE_RESP_T;

typedef PREPACK struct sock_ota_partition_get_size_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
        SOCK_OTA_PARTITION_GET_SIZE_RESP_T  data ;
}POSTPACK WMI_OTA_PARTITION_GET_SIZE_RESPONSE_EVENT;

typedef struct ota_parse_image_hdr_response_s{ 
    A_UINT32 resp_code;
    A_UINT32 offset;
}SOCK_OTA_PARSE_IMAGE_HDR_RESP_T;

typedef PREPACK struct sock_ota_parse_image_hdr_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
        SOCK_OTA_PARSE_IMAGE_HDR_RESP_T  data ;
}POSTPACK WMI_OTA_PARSE_IMAGE_HDR_RESPONSE_EVENT;

typedef PREPACK struct sock_ota_parse_image_hdr_s{
     A_UINT8 header[40];
}POSTPACK SOCK_OTA_PARSE_IMAGE_HDR_T;

typedef struct ota_partition_write_data_response_s{ 
    A_UINT32 resp_code;
    A_UINT32 size;
}SOCK_OTA_PARTITION_WRITE_DATA_RESP_T;

typedef PREPACK struct sock_ota_partition_write_data_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
        SOCK_OTA_PARTITION_WRITE_DATA_RESP_T  data ;
}POSTPACK WMI_OTA_PARTITION_WRITE_DATA_RESPONSE_EVENT;

typedef PREPACK struct sock_ota_partion_write_data_s{
     A_UINT32 offset;
     A_UINT32 size;
     A_UINT8  data[1];
}POSTPACK SOCK_OTA_PARTITON_WRITE_DATA_T;


typedef PREPACK struct sock_set_dhcp_hostname{
    A_CHAR dhcp_data[33];
}POSTPACK SOCK_SET_DHCP_HOSTNAME_T;
typedef struct set_dhcp_hostname{
    A_UINT32 resp_code;
}SOCK_SET_DHCP_HOSTNAME_RESP_T;

typedef struct dhcps_success_cb_resp{
    A_UINT32 resp_code;
    A_UINT8  mac[16];
    A_UINT32 ip;
}SOCK_DHCPS_SUCCESS_CALLBACK_T;

typedef struct dhcpc_success_cb_resp{
    A_UINT32 resp_code;
    A_UINT32 ip;
    A_UINT32 mask;
    A_UINT32 gw;
}SOCK_DHCPC_SUCCESS_CALLBACK_T;

typedef struct autoip_success_cb_resp{
    A_UINT32 resp_code;
    A_UINT32 ip;
    A_UINT32 mask;
    A_UINT32 gw;
}SOCK_AUTOIP_SUCCESS_CALLBACK_T;

typedef PREPACK struct sock_set_dhcp_hostname_response_s{
        A_UINT32   resp_type ;
        A_UINT32   handle ;
        A_UINT32   result ;
       SOCK_SET_DHCP_HOSTNAME_RESP_T  data ;
}POSTPACK WMI_SET_DHCP_HOSTNAME_RESPONSE_EVENT;

typedef PREPACK struct sock_dhcps_success_callback_response_s{
    A_UINT32   resp_type ;
    A_UINT32   handle ;
    A_UINT32   result ;
    SOCK_DHCPS_SUCCESS_CALLBACK_T  data ;
}POSTPACK WMI_DHCPS_SUCCESS_CB_RESPONSE_EVENT ;

typedef PREPACK struct sock_dhcpc_success_callback_response_s{
    A_UINT32   resp_type ;
    A_UINT32   handle ;
    A_UINT32   result ;
    SOCK_DHCPC_SUCCESS_CALLBACK_T  data ;
}POSTPACK WMI_DHCPC_SUCCESS_CB_RESPONSE_EVENT ;

typedef PREPACK struct sock_autoip_success_callback_response_s{
     A_UINT32   resp_type ;
     A_UINT32   handle ;
     A_UINT32   result ;
     SOCK_AUTOIP_SUCCESS_CALLBACK_T  data ;
}POSTPACK WMI_AUTOIP_SUCCESS_CB_RESPONSE_EVENT ;




typedef PREPACK struct sock_tcp_conn_timeout{
        A_UINT32 timeout_val;
}POSTPACK SOCK_TCP_CONN_TIMEOUT_T;

/* Power */
typedef enum {
    REC_POWER = 0x01,
    MAX_PERF_POWER,
} WMI_POWER_MODE;

/*Define all  SET/GET Param Groups here  */
#define QCOM_GROUPID_OFFSET 16  

#define QCOM_GROUPID_MASK 0xFFFF

#define QCOM_PARAMID_MASK 0xFFFF


#define QCOM_PARAM_MAKE_ID(grpid,paramid)    (grpid<<QCOM_GROUPID_OFFSET | paramid)

#define QCOM_PARAM_GET_GROUP_ID(which_param)  (which_param>>QCOM_GROUPID_OFFSET && QCOM_GROUPID_MASK)

#define QCOM_PARAM_GET_PARAM_ID(which_param)  (which_param && QCOM_PARAMID_MASK)


#define QCOM_PARAM_GROUP_SYSTEM         1
#define QCOM_PARAM_GROUP_WLAN           2
#define QCOM_PARAM_GROUP_NETWORK        3
/*define new param groups here*/



/*QCOM_PARAM_GROUP_SYSTEM Parameters*/
#define QCOM_PARAM_GROUP_SYSTEM_SLEEP_MAINT_TIMEOUT_MSECS            1
#define QCOM_PARAM_GROUP_SYSTEM_SLEEP_WATCHDOG_TIMEOUT_SECS         2

/*QCOM_PARAM_GROUP_NETWORK Parameters*/
#define QCOM_PARAM_GROUP_NETWORK_DNS_TIMEOUT_SECS            1

/* Add new system tunables here, at the end */


/* WMI_SYS_SET_PARAM_REPLY */

typedef PREPACK struct {
    A_STATUS status;  
} POSTPACK WMI_PARAM_SET_REPLY;


 /* WMI_SYS_SET_PARAMS */

typedef PREPACK struct {
    A_UINT32 length;
    A_UINT32 which_param;
    A_UCHAR data_buffer[1]; 
} POSTPACK WMI_PARAM_SET_CMD;

/* WMI_SYS_PARAM_GET_REPLY */

typedef PREPACK struct {
    A_UINT8 status;
    A_UINT32 length;
    A_UINT32 which_param;    
    A_UCHAR retval[1];
} POSTPACK WMI_PARAM_GET_REPLY;

/* WMI_SYS_GET_PARAM_CMD */
typedef PREPACK struct {
    A_UINT32 length;
    A_UINT32 which_param;
} POSTPACK WMI_PARAM_GET_CMD;

typedef PREPACK struct
{
	A_UINT32    mode;
	A_UINT32    scan_start_time;
	A_UINT16    total_pkt_count;
	A_UINT16    count;
    int         alt_recv_cnt;
    int         main_recv_cnt;
	int 		alt_ratio;
	int 		main_rssi_avg;
	int 		alt_rssi_avg;
	int			curr_main_set;
	int			curr_alt_set;
	int			end_st;
	int 		scan;
	int			scan_not_start;
	int			curr_bias;
	A_UINT8		main_lna_conf;
	A_UINT8		alt_lna_conf;
	A_UINT8		fast_div_bias;
} POSTPACK WMI_ANT_DIV_STAT;

typedef PREPACK struct
{
	A_UINT32				mode;
	A_UINT8					hw_ant_div;		// HW_ANT_DIV_ENABLE/HW_ANT_DIV_DISABLE
	A_UINT8					hw_rx_curr_ant;	// PHYSICAL_ANT_1/PHYSICAL_ANT_2
	A_UINT8					hw_tx_curr_ant;	// PHYSICAL_ANT_1/PHYSICAL_ANT_2
	A_UINT8					tx_ant_adjust;	// 0/1
	A_UINT32				total_pkt_count;
	A_UINT32				main_total_rssi;
	A_UINT32				alt_total_rssi;
	A_UINT8					ant_select_cnt;
	A_UINT8					ant_switch_cnt;	
} POSTPACK WMI_HW_ANT_DIV_CMD_STAT;


/*
 * WMI_SET_SCAN_PARAMS_CMDID
 */
//#define WMI_SHORTSCANRATIO_DEFAULT      3
/*
 *  Warning: ScanCtrlFlag value of 0xFF is used to disable all flags in WMI_SCAN_PARAMS_CMD
 *  Do not add any more flags to WMI_SCAN_CTRL_FLAG_BITS
 */
typedef enum {
    CONNECT_SCAN_CTRL_FLAGS = 0x01,    /* set if can scan in the Connect cmd */
    SCAN_CONNECTED_CTRL_FLAGS = 0x02,  /* set if scan for the SSID it is */
                                       /* already connected to */
    ACTIVE_SCAN_CTRL_FLAGS = 0x04,     /* set if enable active scan */
    ROAM_SCAN_CTRL_FLAGS = 0x08,       /* set if enable roam scan when bmiss and lowrssi */
    REPORT_BSSINFO_CTRL_FLAGS = 0x10,   /* set if follows customer BSSINFO reporting rule */
    ENABLE_AUTO_CTRL_FLAGS = 0x20,      /* if disabled, target doesn't
                                          scan after a disconnect event  */
    ENABLE_SCAN_ABORT_EVENT = 0x40,     /* Scan complete event with canceled status will be generated when a scan is prempted before it gets completed */
    ENABLE_DFS_SKIP_CTRL_FLAGS = 0x80   /* set if skip scanning dfs channel */
} WMI_SCAN_CTRL_FLAGS_BITS;

//#define CAN_SCAN_IN_CONNECT(flags)      (flags & CONNECT_SCAN_CTRL_FLAGS)
//#define CAN_SCAN_CONNECTED(flags)       (flags & SCAN_CONNECTED_CTRL_FLAGS)
//#define ENABLE_ACTIVE_SCAN(flags)       (flags & ACTIVE_SCAN_CTRL_FLAGS)
//#define ENABLE_ROAM_SCAN(flags)         (flags & ROAM_SCAN_CTRL_FLAGS)
//#define CONFIG_REPORT_BSSINFO(flags)     (flags & REPORT_BSSINFO_CTRL_FLAGS)
//#define IS_AUTO_SCAN_ENABLED(flags)      (flags & ENABLE_AUTO_CTRL_FLAGS)
//#define SCAN_ABORT_EVENT_ENABLED(flags) (flags & ENABLE_SCAN_ABORT_EVENT)
//#define ENABLE_DFS_SKIP_SCAN(flags)     (flags & ENABLE_DFS_SKIP_CTRL_FLAGS)

#define DEFAULT_SCAN_CTRL_FLAGS         (CONNECT_SCAN_CTRL_FLAGS| SCAN_CONNECTED_CTRL_FLAGS| ACTIVE_SCAN_CTRL_FLAGS| ROAM_SCAN_CTRL_FLAGS | ENABLE_AUTO_CTRL_FLAGS)


/******************************** INTERNAL APIs ********************************/

//scan
extern QCOM_BSS_SCAN_INFO *_qcom_get_bss_entry_by_ssid(A_UINT8 device_id, char ssid[]);
extern void _qcom_bss_scan_result_show(A_UINT8 device_id);
extern void _qcom_record_bss_info(A_UINT8 device_id, A_UINT8 * pBuf, int len);

//station
extern A_STATUS _qcom_connect_ap(A_UINT8 device_id, A_CHAR ssid[], A_CHAR pass[], A_UINT8 bssid[], \
                          A_UINT32 auth_dot11_encrypt_mode, A_UINT32 channel, A_UINT32 index_key);
extern A_STATUS _qcom_connect_ap_scaned(A_UINT8 device_id);
extern void _qcom_sta_wlan_reconnect_timer_handler(unsigned int alarm, void *data);
extern A_STATUS _qcom_sta_connect_event_wakeup(A_UINT8 device_id, A_UINT32 state);
//ap
extern A_STATUS _qcom_start_ap(A_UINT8 device_id, char *pssid, char *pkey, int ath_mode, int encrypt_mode, int sta_num, unsigned int uiFlags);

//dhcp

//common
extern void _qcom_wlan_wakemgr_power_down(unsigned int wm_time_cn);

#include "tx_alloc_api.h"
#include "qcom_cdr.h"

#define MAX_SOCKETS_SUPPORTED 12


#endif

