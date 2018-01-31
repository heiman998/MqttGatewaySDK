/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 *
 * This file contains the definitions of the WMI protocol specified in the
 * Wireless Module Interface (WMI).  It includes definitions of all the
 * commands and events. Commands are messages from the host to the WM.
 * Events and Replies are messages from the WM to the host.
 *
 * Ownership of correctness in regards to commands
 * belongs to the host driver and the WMI is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 */

#ifndef _WMI_H_
#define _WMI_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) || defined(WIN64)
#pragma pack (push, 1)
#endif /* WIN32 || WIN64 */
#ifndef ATH_TARGET
#include "athstartpack.h"
#endif

#include "wmix.h"
#include "wlan_defs.h"
#include "dfs_common.h"
#include "qcom/qcom_p2p_api.h"
#include "qcom/qcom_i2c_slave_api.h"
#include "qcom/qcom_i2s.h"

#define HTC_PROTOCOL_VERSION    0x0002
#define HTC_PROTOCOL_REVISION   0x0000

#define WMI_PROTOCOL_VERSION    0x0002
#define WMI_PROTOCOL_REVISION   0x0000

#define ATH_MAC_LEN             6               /* length of mac in bytes */
#define WMI_CMD_MAX_LEN         100
#define WMI_CONTROL_MSG_MAX_LEN     256
#define WMI_OPT_CONTROL_MSG_MAX_LEN 1536
#define IS_ETHERTYPE(_typeOrLen)        ((_typeOrLen) >= 0x0600)
#define RFC1042OUI      {0x00, 0x00, 0x00}

#define IP_ETHERTYPE 0x0800

#define TCP_BIG_ENDIAN_SYN_FLAG	0x0200

#define WMI_IMPLICIT_PSTREAM 0xFF
#define WMI_MAX_THINSTREAM 15

#ifdef AR6002_REV2
#define IBSS_MAX_NUM_STA          4
#else
#define IBSS_MAX_NUM_STA          8
#endif

#define WMI_MODE_MAX              8
#define WMI_MAX_RATE_MASK         2
#define WMI_NETWORK_TYPE(networkType) ((networkType) & 0x000F)
#define WMI_CONNECTED_PHYMODE(networkType) (((networkType) & 0x0F00) >> 8)

PREPACK struct host_app_area_s {
    A_UINT32 wmi_protocol_ver;
} POSTPACK;

/*
 * Data Path
 */
typedef PREPACK struct {
    A_UINT8     dstMac[ATH_MAC_LEN];
    A_UINT8     srcMac[ATH_MAC_LEN];
    A_UINT16    typeOrLen;
} POSTPACK ATH_MAC_HDR;

typedef PREPACK struct {
    A_UINT8     dsap;
    A_UINT8     ssap;
    A_UINT8     cntl;
    A_UINT8     orgCode[3];
    A_UINT16    etherType;
} POSTPACK ATH_LLC_SNAP_HDR;

typedef enum {
    DATA_MSGTYPE = 0x0,
    CNTL_MSGTYPE,
    SYNC_MSGTYPE,
    OPT_MSGTYPE,
} WMI_MSG_TYPE;

/*
 * Macros for operating on WMI_DATA_HDR (info) field
 */

#define WMI_DATA_HDR_MSG_TYPE_MASK  0x03
#define WMI_DATA_HDR_MSG_TYPE_SHIFT 0
#define WMI_DATA_HDR_UP_MASK        0x07
#define WMI_DATA_HDR_UP_SHIFT       2
/* In AP mode, the same bit (b5) is used to indicate Power save state in
 * the Rx dir and More data bit state in the tx direction.
 */
#define WMI_DATA_HDR_PS_MASK        0x1
#define WMI_DATA_HDR_PS_SHIFT       5

#define WMI_DATA_HDR_MORE_MASK      0x1
#define WMI_DATA_HDR_MORE_SHIFT     5

#define WMI_DATA_HDR_DUMMY_DATA_MASK      0x1
#define WMI_DATA_HDR_DUMMY_DATA_SHIFT     12 

typedef enum {
    WMI_DATA_HDR_DATA_TYPE_802_3 = 0,
    WMI_DATA_HDR_DATA_TYPE_802_11,
    WMI_DATA_HDR_DATA_TYPE_ACL,
} WMI_DATA_HDR_DATA_TYPE;

/* Bitmap of data header flags */
typedef enum {
    WMI_DATA_HDR_FLAGS_MORE = 0x1,
    WMI_DATA_HDR_FLAGS_EOSP = 0x2,
    WMI_DATA_HDR_FLAGS_TRIGGERED = 0x4,
    WMI_DATA_HDR_FLAGS_PSPOLLED = 0x8,
    WMI_DATA_HDR_FLAGS_AMSDU = 0x10,
} WMI_DATA_HDR_FLAGS;

#define WMI_DATA_HDR_DATA_TYPE_MASK     0x3
#define WMI_DATA_HDR_DATA_TYPE_SHIFT    6

#define WMI_DATA_HDR_GET_RSSI(h) ((h)->rssi)

#define WMI_DATA_HDR_SET_MORE_BIT(h) ((h)->info |= (WMI_DATA_HDR_MORE_MASK << WMI_DATA_HDR_MORE_SHIFT))
#define WMI_DATA_HDR_HAS_MORE_BIT(h) ((h)->info & (WMI_DATA_HDR_MORE_MASK << WMI_DATA_HDR_MORE_SHIFT))

#define WMI_DATA_HDR_IS_MSG_TYPE(h, t)  (((h)->info & (WMI_DATA_HDR_MSG_TYPE_MASK)) == (t))
#define WMI_DATA_HDR_SET_MSG_TYPE(h, t) (h)->info = (((h)->info & ~(WMI_DATA_HDR_MSG_TYPE_MASK << WMI_DATA_HDR_MSG_TYPE_SHIFT)) | (t << WMI_DATA_HDR_MSG_TYPE_SHIFT))
#define WMI_DATA_HDR_GET_UP(h)    (((h)->info >> WMI_DATA_HDR_UP_SHIFT) & WMI_DATA_HDR_UP_MASK)
#define WMI_DATA_HDR_SET_UP(h, p) (h)->info = (((h)->info & ~(WMI_DATA_HDR_UP_MASK << WMI_DATA_HDR_UP_SHIFT)) | (p << WMI_DATA_HDR_UP_SHIFT))

#define WMI_DATA_HDR_GET_DATA_TYPE(h)   (((h)->info >> WMI_DATA_HDR_DATA_TYPE_SHIFT) & WMI_DATA_HDR_DATA_TYPE_MASK)
#define WMI_DATA_HDR_SET_DATA_TYPE(h, p) (h)->info = (((h)->info & ~(WMI_DATA_HDR_DATA_TYPE_MASK << WMI_DATA_HDR_DATA_TYPE_SHIFT)) | ((p) << WMI_DATA_HDR_DATA_TYPE_SHIFT))

#define WMI_DATA_HDR_GET_DOT11(h)   (WMI_DATA_HDR_GET_DATA_TYPE((h)) == WMI_DATA_HDR_DATA_TYPE_802_11)
#define WMI_DATA_HDR_SET_DOT11(h, p) WMI_DATA_HDR_SET_DATA_TYPE((h), (p))

#define WMI_DATA_HDR_IS_DUMMY_DATA(h)  (((h)->info3 & (WMI_DATA_HDR_DUMMY_DATA_MASK << WMI_DATA_HDR_DUMMY_DATA_SHIFT)))

/* Macros for operating on WMI_DATA_HDR (info2) field */
#define WMI_DATA_HDR_SEQNO_MASK     0xFFF
#define WMI_DATA_HDR_SEQNO_SHIFT    0

#define WMI_DATA_HDR_TX_SEQNO_LSB_MASK     0xFF

#define WMI_DATA_HDR_AMSDU_MASK     0x1
#define WMI_DATA_HDR_AMSDU_SHIFT    12

#define WMI_DATA_HDR_META_MASK      0x7
#define WMI_DATA_HDR_META_SHIFT     13

#define GET_SEQ_NO(_v)                  ((_v) & WMI_DATA_HDR_SEQNO_MASK)
#define GET_ISMSDU(_v)                  ((_v) & WMI_DATA_HDR_AMSDU_MASK)

#define WMI_DATA_HDR_GET_SEQNO(h)        GET_SEQ_NO((h)->info2 >> WMI_DATA_HDR_SEQNO_SHIFT)
#define WMI_DATA_HDR_SET_SEQNO(h, _v)   ((h)->info2 = ((h)->info2 & ~(WMI_DATA_HDR_SEQNO_MASK << WMI_DATA_HDR_SEQNO_SHIFT)) | (GET_SEQ_NO(_v) << WMI_DATA_HDR_SEQNO_SHIFT))

#define WMI_DATA_HDR_IS_AMSDU(h)        GET_ISMSDU((h)->info2 >> WMI_DATA_HDR_AMSDU_SHIFT)
#define WMI_DATA_HDR_SET_AMSDU(h, _v)   ((h)->info2 = ((h)->info2 & ~(WMI_DATA_HDR_AMSDU_MASK << WMI_DATA_HDR_AMSDU_SHIFT)) | (GET_ISMSDU(_v) << WMI_DATA_HDR_AMSDU_SHIFT))
#define WMI_DATA_HDR_HAS_AMSDU_BIT(h)   ((h)->info2 & (WMI_DATA_HDR_AMSDU_MASK << WMI_DATA_HDR_AMSDU_SHIFT))

#define WMI_DATA_HDR_GET_META(h)        (((h)->info2 >> WMI_DATA_HDR_META_SHIFT) & WMI_DATA_HDR_META_MASK)
#define WMI_DATA_HDR_SET_META(h, _v)    ((h)->info2 = ((h)->info2 & ~(WMI_DATA_HDR_META_MASK << WMI_DATA_HDR_META_SHIFT)) | ((_v) << WMI_DATA_HDR_META_SHIFT))

#define WMI_DATA_HDR_GET_TX_SEQ_LSB(h)        (((h)->info2 >> WMI_DATA_HDR_SEQNO_SHIFT) & WMI_DATA_HDR_TX_SEQNO_LSB_MASK)
#define WMI_DATA_HDR_SET_TX_SEQ_LSB(h, _v)    ((h)->info2 = ((h)->info2 & ~(WMI_DATA_HDR_TX_SEQNO_LSB_MASK << WMI_DATA_HDR_META_SHIFT)) | ((_v) << WMI_DATA_HDR_META_SHIFT))

/* Macros for operating on WMI_DATA_HDR (info3) field */
#define WMI_DATA_HDR_DEVID_MASK      0xF
#define WMI_DATA_HDR_DEVID_SHIFT     0
#define GET_DEVID(_v)                ((_v) & WMI_DATA_HDR_DEVID_MASK)

#define WMI_DATA_HDR_GET_DEVID(h)        (((h)->info3 >> WMI_DATA_HDR_DEVID_SHIFT) & WMI_DATA_HDR_DEVID_MASK)
#define WMI_DATA_HDR_SET_DEVID(h, _v)   ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_DEVID_MASK << WMI_DATA_HDR_DEVID_SHIFT)) | (GET_DEVID(_v) << WMI_DATA_HDR_DEVID_SHIFT))

#define WMI_DATA_HDR_RXFILT_SHIFT   4
#define WMI_DATA_HDR_RXFILT_MASK    (0x1 << WMI_DATA_HDR_RXFILT_SHIFT)

#define WMI_DATA_HDR_IS_RXFILTERED(h)  ((h)->info3 & WMI_DATA_HDR_RXFILT_MASK)
#define SET_WMI_DATA_HDR_RXFILTERED(h)  (h)->info3 |= WMI_DATA_HDR_RXFILT_MASK 

#define WMI_DATA_HDR_TRIGGER_MASK      0x1
#define WMI_DATA_HDR_TRIGGER_SHIFT     4
#define WMI_DATA_HDR_SET_TRIGGER(h, _v)   ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_TRIGGER_MASK << WMI_DATA_HDR_TRIGGER_SHIFT)) | ((_v) << WMI_DATA_HDR_TRIGGER_SHIFT))
#define WMI_DATA_HDR_IS_TRIGGER(h)       ((((h)->info3 >> WMI_DATA_HDR_TRIGGER_SHIFT) & WMI_DATA_HDR_TRIGGER_MASK) == WMI_DATA_HDR_TRIGGER_MASK)

#define WMI_DATA_HDR_EOSP_MASK      WMI_DATA_HDR_TRIGGER_MASK
#define WMI_DATA_HDR_EOSP_SHIFT     WMI_DATA_HDR_TRIGGER_SHIFT

#define WMI_DATA_HDR_SET_EOSP_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_EOSP_MASK << WMI_DATA_HDR_EOSP_SHIFT))
#define WMI_DATA_HDR_HAS_EOSP_BIT(h) ((h)->info3 & (WMI_DATA_HDR_EOSP_MASK << WMI_DATA_HDR_EOSP_SHIFT))


#define WMI_DATA_HDR_PAD_BEFORE_DATA_MASK               0xFF
#define WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT              0x8
#define GET_PAD_BEFORE_DATA_START(_v)                   ((_v) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)

#define WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h)       (((h)->info3 >> WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)
#define WMI_DATA_HDR_SET_PAD_BEFORE_DATA_START(h, _v)   ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_PAD_BEFORE_DATA_MASK << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT)) | (GET_PAD_BEFORE_DATA_START(_v) << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT))

#define WMI_DATA_HDR_TRIGGERED_MASK      0x1
#define WMI_DATA_HDR_TRIGGERED_SHIFT     6
#define WMI_DATA_HDR_SET_TRIGGERED_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_TRIGGERED_MASK << WMI_DATA_HDR_TRIGGERED_SHIFT))
#define WMI_DATA_HDR_HAS_TRIGGERED_BIT(h) ((h)->info3 & (WMI_DATA_HDR_TRIGGERED_MASK << WMI_DATA_HDR_TRIGGERED_SHIFT))

#define WMI_DATA_HDR_PSPOLLED_MASK      0x1
#define WMI_DATA_HDR_PSPOLLED_SHIFT     7
#define WMI_DATA_HDR_SET_PSPOLLED_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_PSPOLLED_MASK << WMI_DATA_HDR_PSPOLLED_SHIFT))
#define WMI_DATA_HDR_HAS_PSPOLLED_BIT(h) ((h)->info3 & (WMI_DATA_HDR_PSPOLLED_MASK << WMI_DATA_HDR_PSPOLLED_SHIFT))

#define WMI_DATA_HDR_PAD_BEFORE_DATA_MASK               0xFF
#define WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT              0x8
#define GET_PAD_BEFORE_DATA_START(_v)                   ((_v) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)

#define WMI_DATA_HDR_HOST_RETRY_MASK                    0x1
#define WMI_DATA_HDR_GET_HOST_RETRY(h)                 (WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h) & WMI_DATA_HDR_HOST_RETRY_MASK)

#define WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h)       (((h)->info3 >> WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)
#define WMI_DATA_HDR_SET_PAD_BEFORE_DATA_START(h, _v)   ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_PAD_BEFORE_DATA_MASK << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT)) | (GET_PAD_BEFORE_DATA_START(_v) << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT))

#define WMI_DATA_HDR_EXCEPTION_BIT_MASK                  0x1
#define WMI_DATA_HDR_EXCEPTION_BIT_SHIFT                 8
#define WMI_DATA_HDR_SET_EXCEPTION_BIT(h)                ((h)->info3 |= (WMI_DATA_HDR_EXCEPTION_BIT_MASK << WMI_DATA_HDR_EXCEPTION_BIT_SHIFT))

#define WMI_DATA_HDR_AMPDU_FLUSH_BIT_MASK                0x1
#define WMI_DATA_HDR_AMPDU_FLUSH_BIT_SHIFT               9
#define WMI_DATA_HDR_SET_AMPDU_FLUSH_BIT(h)              ((h)->info3 |= (WMI_DATA_HDR_AMPDU_FLUSH_BIT_MASK << WMI_DATA_HDR_AMPDU_FLUSH_BIT_SHIFT))


#define WMI_DATA_HDR_WLAN_BIT_MASK                       0x0
#define WMI_DATA_HDR_WLAN_BIT_SHIFT                      10
#define WMI_DATA_HDR_SET_WLAN_BIT(h)                     ((h)->info3 |= (WMI_DATA_HDR_WLAN_BIT_MASK << WMI_DATA_HDR_WLAN_BIT_SHIFT))

#define WMI_DATA_HDR_WAN_BIT_MASK                        0x1
#define WMI_DATA_HDR_WAN_BIT_SHIFT                       10
#define WMI_DATA_HDR_SET_WAN_BIT(h)                      ((h)->info3 |= (WMI_DATA_HDR_WAN_BIT_MASK << WMI_DATA_HDR_WAN_BIT_SHIFT))

#define WMI_DATA_HDR_OUT_OF_ORDER_BIT_MASK               0x1
#define WMI_DATA_HDR_OUT_OF_ORDER_BIT_SHIFT              11
#define WMI_DATA_HDR_SET_OUT_OF_ORDER_BIT(h)             ((h)->info3 |= (WMI_DATA_HDR_OUT_OF_ORDER_BIT_MASK << WMI_DATA_HDR_OUT_OF_ORDER_BIT_SHIFT))

#define WMI_DATA_HDR_FLUSH_ALL_BIT_MASK                  0x1
#define WMI_DATA_HDR_FLUSH_ALL_BIT_SHIFT                 12
#define WMI_DATA_HDR_SET_FLUSH_ALL_BIT(h)                ((h)->info3 |= (WMI_DATA_HDR_FLUSH_ALL_BIT_MASK << WMI_DATA_HDR_FLUSH_ALL_BIT_SHIFT))

#define WMI_DATA_HDR_PARTIAL_FLUSH_BIT_MASK              0x1
#define WMI_DATA_HDR_PARTIAL_FLUSH_BIT_SHIFT             13
#define WMI_DATA_HDR_SET_PARTIAL_FLUSH_BIT(h)            ((h)->info3 |= (WMI_DATA_HDR_PARTIAL_FLUSH_BIT_MASK << WMI_DATA_HDR_PARTIAL_FLUSH_BIT_SHIFT))

typedef PREPACK struct {
    A_INT8      rssi;               /* TX dir: packetid MSB*/
    A_UINT8     info;               /* usage of 'info' field(8-bit):
                                     *  b1:b0       - WMI_MSG_TYPE
                                     *  b4:b3:b2    - UP(tid)
                                     *  b5          - Used in AP mode. More-data in tx dir, PS in rx.
                                     *  b7:b6       -  Dot3 header(0),
                                     *                 Dot11 Header(1),
                                     *                 ACL data(2)
                                     */

    A_UINT16    info2;              /* usage of 'info2' field(16-bit):
                                     * RX dir :b11:b0       - seq_no  Tx dir: b7:b0 packetid LSB
                                     * b12          - A-MSDU?
                                     * b15:b13      - META_DATA_VERSION 0 - 7
                                     */
    A_UINT16    info3;              /* b3:b2:b1:b0  - device id
                                     * b4           - Used in AP mode. uAPSD trigger in rx, EOSP in tx
                                     * b5           - Used by C2X.
                                     * b6           - Used in AP mode. uAPSD triggered frame in tx,
                                     *                meaningless in rx
                                     * b7           - Used in AP mode. PS-POLL triggered frame in tx,
                                     *                meaningless in rx
                                     * b15:b8       - pad before data start(irrespective of meta version)
                                     */
} POSTPACK WMI_DATA_HDR;

/*
 *  TX META VERSION DEFINITIONS
 */
#define WMI_MAX_TX_META_SZ  (12)
#define WMI_MAX_TX_META_VERSION (7)
#define WMI_META_VERSION_1 (0x01)
#define WMI_META_VERSION_2 (0X02)
#define WMI_META_VERSION_3 (0x03)
#define WMI_META_VERSION_4 (0x04)
#define WMI_META_VERSION_5 (0x05)  /* Used by IP Offload */

#define WMI_ACL_TO_DOT11_HEADROOM   36

#if 0 /* removed to prevent compile errors for WM.. */
typedef PREPACK struct {
/* intentionally empty. Default version is no meta data. */
} POSTPACK WMI_TX_META_V0;
#endif

typedef PREPACK struct {
    A_UINT8     pktID;           /* The packet ID to identify the tx request */
    A_UINT8     ratePolicyID;    /* The rate policy to be used for the tx of this frame */
} POSTPACK WMI_TX_META_V1;


#define WMI_CSUM_DIR_TX (0x1)
#define TX_CSUM_CALC_FILL (0x1)
typedef PREPACK struct {
    A_UINT8    csumStart;       /*Offset from start of the Payload(SNAP header) for csum calculation to begin */
    A_UINT8    csumDest;        /*Offset from start of Payload(SNAP header) where final csum goes*/
    A_UINT8     csumFlags;    /*Flag for check sum engine to be offloaded to device*/
} POSTPACK WMI_TX_META_V2;

/* WMI_META_TX_FLAG... are used as TX qualifiers for frames containing WMI_TX_RATE_SCHEDULE in the 
 * meta data.  0 or more of these flags should be assigned to the flags member of the schedule. */
#define WMI_META_TX_FLAG_ACK            0x01 // frame needs ACK response from receiver
#define WMI_META_TX_FLAG_SET_RETRY_BIT  0x02 // device will set retry bit in MAC header for retried frames.
#define WMI_META_TX_FLAG_SET_DURATION   0x04 // device will fill duration field in MAC header
/* NOTE: If WMI_META_TX_FLAG_USE_PREFIX == 0 device will NOT use prefix frame.
 *       If WMI_META_TX_FLAG_USE_PREFIX == 1 && WMI_META_TX_FLAG_PREFIX_RTS == 0 device will use CTS prefix. 
 *       If WMI_META_TX_FLAG_USE_PREFIX == 1 && WMI_META_TX_FLAG_PREFIX_RTS == 1 device will use RTS prefix.
 */  
#define WMI_META_TX_FLAG_USE_PREFIX     0x08 // device will send either RTS or CTS frame prior to subject frame.
#define WMI_META_TX_FLAG_PREFIX_RTS     0x10 // device will send RTS and wait for CTS prior to sending subject frame.
#define WMI_META_TX_LOAD_TSF            0x20 // device will fill the TSF field during transmit procedure. <Beacons/probe responses>

/* WMI_TX_RATE_SCHEDULE - Acts as a host-provided rate schedule to replace what would be normally determined
 * by firmware.  This allows the host to specify what rates and attempts should be used to transmit the 
 * frame. */
typedef PREPACK struct {
#define WMI_TX_MAX_RATE_SERIES (4)
    A_UINT8 rateSeries[WMI_TX_MAX_RATE_SERIES]; //rate index for each series. first invalid rate terminates series.
    A_UINT8 trySeries[WMI_TX_MAX_RATE_SERIES]; //number of tries for each series.
    A_UINT8 flags; // combination of WMI_META_TX_FLAG...
    A_UINT8 accessCategory; // should be WMM_AC_BE for managment frames and multicast frames.
    //A_UINT8 keyIndex;
    //
}POSTPACK WMI_TX_RATE_SCHEDULE;

#ifdef TIME_BASED_DISCARD
typedef PREPACK struct {
    A_UINT32 txdiscardTime;
    A_UINT8 discardDispostion;
} POSTPACK WMI_TX_META_V4;
#endif

typedef PREPACK struct {
    WMI_TX_RATE_SCHEDULE rateSched;
    A_UINT8     pktID;           /* The packet ID to identify the tx request */
} POSTPACK WMI_TX_META_V3;

/*
 *  RX META VERSION DEFINITIONS
 */
/* if RX meta data is present at all then the meta data field
 *  will consume WMI_MAX_RX_META_SZ bytes of space between the
 *  WMI_DATA_HDR and the payload. How much of the available
 *  Meta data is actually used depends on which meta data
 *  version is active. */
#define WMI_MAX_RX_META_SZ  (12)
#define WMI_MAX_RX_META_VERSION (7)

#define WMI_RX_STATUS_OK            0 /* success */
#define WMI_RX_STATUS_DECRYPT_ERR   1 /* decrypt error */
#define WMI_RX_STATUS_MIC_ERR       2 /* tkip MIC error */
#define WMI_RX_STATUS_ERR           3 /* undefined error */

#define WMI_RX_FLAGS_AGGR           0x0001 /* part of AGGR */
#define WMI_RX_FlAGS_STBC           0x0002 /* used STBC */
#define WMI_RX_FLAGS_SGI            0x0004 /* used SGI */
#define WMI_RX_FLAGS_HT             0x0008 /* is HT packet */
/* the flags field is also used to store the CRYPTO_TYPE of the frame
 * that value is shifted by WMI_RX_FLAGS_CRYPTO_SHIFT */
#define WMI_RX_FLAGS_CRYPTO_SHIFT   4
#define WMI_RX_FLAGS_CRYPTO_MASK    0x1f
#define WMI_RX_META_GET_CRYPTO(flags) (((flags) >> WMI_RX_FLAGS_CRYPTO_SHIFT) & WMI_RX_FLAGS_CRYPTO_MASK)

#if 0 /* removed to prevent compile errors for WM.. */
typedef PREPACK struct {
/* intentionally empty. Default version is no meta data. */
} POSTPACK WMI_RX_META_VERSION_0;
#endif

typedef PREPACK struct {
    A_UINT8     status; /* one of WMI_RX_STATUS_... */
    A_UINT8     rix;    /* rate index mapped to rate at which this packet was received. */
    A_UINT8     rssi;   /* rssi of packet */
    A_UINT8     channel;/* rf channel during packet reception */
    A_UINT16    flags;  /* a combination of WMI_RX_FLAGS_... */
} POSTPACK WMI_RX_META_V1;

#define RX_CSUM_VALID_FLAG (0x1)
typedef PREPACK struct {
    A_UINT16    csum;
    A_UINT8     csumFlags;/* bit 0 set -partial csum valid
                             bit 1 set -test mode */
} POSTPACK WMI_RX_META_V2;

#ifdef TIME_BASED_DISCARD
typedef PREPACK struct {
	A_UINT8  	discardDispostion;
	A_UINT32	txdiscardTime;
} POSTPACK WMI_RX_META_V4;
#endif


#define WMI_GET_DEVICE_ID(info1) ((info1) & 0xF)
/* Macros for operating on WMI_CMD_HDR (info1) field */
#define WMI_CMD_HDR_DEVID_MASK      0xF
#define WMI_CMD_HDR_DEVID_SHIFT     0
#define GET_CMD_DEVID(_v)           ((_v) & WMI_CMD_HDR_DEVID_MASK)

#define WMI_CMD_HDR_GET_DEVID(h)        (((h)->info1 >> WMI_CMD_HDR_DEVID_SHIFT) & WMI_CMD_HDR_DEVID_MASK)
#define WMI_CMD_HDR_SET_DEVID(h, _v)   ((h)->info1 = ((h)->info1 & ~(WMI_CMD_HDR_DEVID_MASK << WMI_CMD_HDR_DEVID_SHIFT)) | (GET_CMD_DEVID(_v) << WMI_CMD_HDR_DEVID_SHIFT))

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

/*
 * List of Commnands
 */
typedef enum {
    WMI_CONNECT_CMDID                         = 0x0001,
    WMI_RECONNECT_CMDID,
    WMI_DISCONNECT_CMDID,
    WMI_SYNCHRONIZE_CMDID,
    WMI_CREATE_PSTREAM_CMDID,
    WMI_DELETE_PSTREAM_CMDID,
    WMI_START_SCAN_CMDID,
    WMI_SET_SCAN_PARAMS_CMDID,
    WMI_SET_BSS_FILTER_CMDID,
    WMI_SET_PROBED_SSID_CMDID,               /* 0x0A */
    WMI_SET_LISTEN_INT_CMDID,
    WMI_SET_BMISS_TIME_CMDID,
    WMI_SET_DISC_TIMEOUT_CMDID,
    WMI_GET_CHANNEL_LIST_CMDID,
    WMI_SET_BEACON_INT_CMDID,
    WMI_GET_STATISTICS_CMDID,                /* 0x10 */
    WMI_SET_CHANNEL_PARAMS_CMDID,
    WMI_SET_POWER_MODE_CMDID,
    WMI_SET_IBSS_PM_CAPS_CMDID,
    WMI_SET_POWER_PARAMS_CMDID,              
    WMI_SET_POWERSAVE_TIMERS_POLICY_CMDID,
    WMI_ADD_CIPHER_KEY_CMDID,
    WMI_DELETE_CIPHER_KEY_CMDID,
    WMI_ADD_KRK_CMDID,
    WMI_DELETE_KRK_CMDID,
    WMI_SET_PMKID_CMDID,                     /* 0x1A */
    WMI_SET_TX_PWR_CMDID,
    WMI_GET_TX_PWR_CMDID,
    WMI_SET_ASSOC_INFO_CMDID,
    WMI_ADD_BAD_AP_CMDID,                    
    WMI_DELETE_BAD_AP_CMDID,
    WMI_SET_TKIP_COUNTERMEASURES_CMDID,      /* 0x20 */
    WMI_RSSI_THRESHOLD_PARAMS_CMDID,
    WMI_TARGET_ERROR_REPORT_BITMASK_CMDID,
    WMI_SET_ACCESS_PARAMS_CMDID,
    WMI_SET_RETRY_LIMITS_CMDID,
    WMI_SET_OPT_MODE_CMDID,
    WMI_OPT_TX_FRAME_CMDID,
    WMI_SET_VOICE_PKT_SIZE_CMDID,
    WMI_SET_MAX_SP_LEN_CMDID,                
    WMI_SET_ROAM_CTRL_CMDID,
    WMI_GET_ROAM_TBL_CMDID,                  /* 0x2A */
    WMI_GET_ROAM_DATA_CMDID,
    WMI_ENABLE_RM_CMDID,
    WMI_SET_MAX_OFFHOME_DURATION_CMDID,
    WMI_EXTENSION_CMDID,                        /* Non-wireless extensions */
    WMI_SNR_THRESHOLD_PARAMS_CMDID,
    WMI_LQ_THRESHOLD_PARAMS_CMDID,           /* 0x30 */
    WMI_SET_LPREAMBLE_CMDID,
    WMI_SET_RTS_CMDID,                       
    WMI_CLR_RSSI_SNR_CMDID,
    WMI_SET_FIXRATES_CMDID,
    WMI_GET_FIXRATES_CMDID,
    WMI_SET_AUTH_MODE_CMDID,
    WMI_SET_REASSOC_MODE_CMDID,
    WMI_SET_WMM_CMDID,
    WMI_SET_WMM_TXOP_CMDID,
    WMI_TEST_CMDID,                          /* 0x3A */
    /* COEX AR6002 only*/
    WMI_SET_BT_STATUS_CMDID,
    WMI_SET_BT_PARAMS_CMDID,                

    WMI_SET_KEEPALIVE_CMDID,
    WMI_GET_KEEPALIVE_CMDID,
    WMI_SET_APPIE_CMDID,
    WMI_GET_APPIE_CMDID,                     /* 0x40 */
    WMI_SET_WSC_STATUS_CMDID,

    /* Wake on Wireless */
    WMI_SET_HOST_SLEEP_MODE_CMDID,
    WMI_SET_WOW_MODE_CMDID,
    WMI_GET_WOW_LIST_CMDID,
    WMI_ADD_WOW_PATTERN_CMDID,
    WMI_DEL_WOW_PATTERN_CMDID,               

    WMI_SET_FRAMERATES_CMDID,
    WMI_SET_AP_PS_CMDID,
    WMI_SET_QOS_SUPP_CMDID,                  /* 0x49 */
    WMI_SET_LARGE_APPIE_CMDID,
    WMI_SET_RECOVERY_TEST_PARAMETER_CMDID,
    /* WMI_THIN_RESERVED_... mark the start and end
     * values for WMI_THIN_RESERVED command IDs. These
     * command IDs can be found in wmi_thin.h */
    WMI_THIN_RESERVED_START = 0x8000,
    WMI_THIN_RESERVED_END = 0x8fff,
    /*
     * Developer commands starts at 0xF000
     */
    WMI_SET_BITRATE_CMDID = 0xF000,
    WMI_GET_BITRATE_CMDID,
    WMI_SET_WHALPARAM_CMDID,


    /*Should add the new command to the tail for compatible with
     * etna.
     */
    WMI_SET_MAC_ADDRESS_CMDID,
    WMI_SET_AKMP_PARAMS_CMDID,
    WMI_SET_PMKID_LIST_CMDID,
    WMI_GET_PMKID_LIST_CMDID,
    WMI_ABORT_SCAN_CMDID,
    WMI_SET_TARGET_EVENT_REPORT_CMDID,

    // Unused
    WMI_UNUSED1,
    WMI_UNUSED2,

    /*
     * AP mode commands
     */
    WMI_AP_HIDDEN_SSID_CMDID,                /* F00B */
    WMI_AP_SET_NUM_STA_CMDID,
    WMI_AP_ACL_POLICY_CMDID,
    WMI_AP_ACL_MAC_LIST_CMDID,
    WMI_AP_CONFIG_COMMIT_CMDID,
    WMI_AP_SET_MLME_CMDID,                   /* F010 */
    WMI_AP_SET_PVB_CMDID,
    WMI_AP_CONN_INACT_CMDID,
    WMI_AP_PROT_SCAN_TIME_CMDID,
    WMI_AP_SET_COUNTRY_CMDID,
    WMI_AP_SET_DTIM_CMDID,
    WMI_AP_MODE_STAT_CMDID,

    WMI_SET_IP_CMDID,                        /* F017 */
    WMI_SET_STA_COUNTRY_CODE_CMDID,
    WMI_SET_MCAST_FILTER_CMDID,
    WMI_DEL_MCAST_FILTER_CMDID,

    WMI_ALLOW_AGGR_CMDID,                    /* F01B */
    WMI_ADDBA_REQ_CMDID,
    WMI_DELBA_REQ_CMDID,
    WMI_SET_HT_CAP_CMDID,
    WMI_SET_HT_OP_CMDID,
    WMI_SET_TX_SELECT_RATES_CMDID,
    WMI_SET_TX_SGI_PARAM_CMDID,
    WMI_SET_RATE_POLICY_CMDID,

    WMI_HCI_CMD_CMDID,                       /* F023 */
    WMI_RX_FRAME_FORMAT_CMDID,
    WMI_SET_THIN_MODE_CMDID,
    WMI_SET_BT_WLAN_CONN_PRECEDENCE_CMDID,

    WMI_AP_SET_11BG_RATESET_CMDID,           /* F027 */
    WMI_SET_PMK_CMDID,
    WMI_MCAST_FILTER_CMDID,
    /* COEX CMDID AR6003*/
    WMI_SET_BTCOEX_FE_ANT_CMDID,             /* F02A */
    WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMDID,
    WMI_SET_BTCOEX_SCO_CONFIG_CMDID,
    WMI_SET_BTCOEX_A2DP_CONFIG_CMDID,
    WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMDID,
    WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMDID,
    WMI_SET_BTCOEX_DEBUG_CMDID,              /* F030 */
    WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID,
    WMI_GET_BTCOEX_STATS_CMDID,
    WMI_GET_BTCOEX_CONFIG_CMDID,             /* F033 */
#if !defined(AR6002_REV61) || !defined(ATH_TARGET) /* WAR for EV 89272 */
    WMI_DFS_RESERVED,                        /* F034 */
#endif
    WMI_SET_DFS_MINRSSITHRESH_CMDID,         /* F035 */
    WMI_SET_DFS_MAXPULSEDUR_CMDID,
    WMI_DFS_RADAR_DETECTED_CMDID,

    /* P2P CMDS */
    WMI_P2P_SET_CONFIG_CMDID,                /* F038 for KF */
    WMI_WPS_SET_CONFIG_CMDID,
    WMI_SET_REQ_DEV_ATTR_CMDID,
    WMI_P2P_FIND_CMDID,
    WMI_P2P_STOP_FIND_CMDID,
    WMI_P2P_GO_NEG_START_CMDID,
    WMI_P2P_LISTEN_CMDID,


    WMI_CONFIG_TX_MAC_RULES_CMDID,           /* F03F for KF */
    WMI_SET_PROMISCUOUS_MODE_CMDID,          /* F040 */
    WMI_RX_FRAME_FILTER_CMDID,
    WMI_SET_CHANNEL_CMDID,

    /* WAC commands */
    WMI_ENABLE_WAC_CMDID,
    WMI_WAC_SCAN_REPLY_CMDID,
    WMI_WAC_CTRL_REQ_CMDID,

    WMI_SET_DIV_PARAMS_CMDID,
    WMI_GET_PMK_CMDID,
    WMI_SET_PASSPHRASE_CMDID,
    WMI_SEND_ASSOC_RES_CMDID,
    WMI_SET_ASSOC_REQ_RELAY_CMDID,           /* F04A */

    /* ACS command, consists of sub-commands */
    WMI_ACS_CTRL_CMDID,
    WMI_SET_EXCESS_TX_RETRY_THRES_CMDID,
    WMI_SET_TBD_TIME_CMDID, /*added for wmiconfig command for TBD */
    
    /* Pktlog cmds */
    WMI_PKTLOG_ENABLE_CMDID,
    WMI_PKTLOG_DISABLE_CMDID,

    /* More P2P Cmds */
    WMI_P2P_GO_NEG_REQ_RSP_CMDID,            /* F050 */
    WMI_P2P_GRP_INIT_CMDID,
    WMI_P2P_GRP_FORMATION_DONE_CMDID,
    WMI_P2P_INVITE_CMDID,
    WMI_P2P_INVITE_REQ_RSP_CMDID,
    WMI_P2P_PROV_DISC_REQ_CMDID,
    WMI_P2P_SET_CMDID,

    WMI_GET_RFKILL_MODE_CMDID,
    WMI_SET_RFKILL_MODE_CMDID,
    WMI_AP_SET_APSD_CMDID,
    WMI_AP_APSD_BUFFERED_TRAFFIC_CMDID,      /* F05A */

    WMI_P2P_SDPD_TX_CMDID,
    WMI_P2P_STOP_SDPD_CMDID,
    WMI_P2P_CANCEL_CMDID,
    /* Ultra low power store / recall commands */
    WMI_STORERECALL_CONFIGURE_CMDID,
    WMI_STORERECALL_RECALL_CMDID,
    WMI_STORERECALL_HOST_READY_CMDID,        /* F060 */
    WMI_FORCE_TARGET_ASSERT_CMDID,

    WMI_SET_PROBED_SSID_EX_CMDID,
    WMI_SET_NETWORK_LIST_OFFLOAD_CMDID,
    WMI_SET_ARP_NS_OFFLOAD_CMDID,
    WMI_ADD_WOW_EXT_PATTERN_CMDID,
    WMI_GTK_OFFLOAD_OP_CMDID,

    WMI_REMAIN_ON_CHNL_CMDID,
    WMI_CANCEL_REMAIN_ON_CHNL_CMDID,
    WMI_SEND_ACTION_CMDID,                   /* F069 */
    WMI_PROBE_REQ_REPORT_CMDID,
    WMI_DISABLE_11B_RATES_CMDID,
    WMI_SEND_PROBE_RESPONSE_CMDID,
    WMI_GET_P2P_INFO_CMDID,
    WMI_AP_JOIN_BSS_CMDID,

    WMI_SMPS_ENABLE_CMDID,
    WMI_SMPS_CONFIG_CMDID,                   /* F070 */
    WMI_SET_RATECTRL_PARM_CMDID,
    /*  LPL specific commands*/
    WMI_LPL_FORCE_ENABLE_CMDID,
    WMI_LPL_SET_POLICY_CMDID,
    WMI_LPL_GET_POLICY_CMDID,
    WMI_LPL_GET_HWSTATE_CMDID,
    WMI_LPL_SET_PARAMS_CMDID,
    WMI_LPL_GET_PARAMS_CMDID,

    WMI_SET_BUNDLE_PARAM_CMDID,

    /*GreenTx specific commands*/
    WMI_GREENTX_PARAMS_CMDID,

    /* WPS Commands */
    WMI_WPS_START_CMDID,                     /* F07A */
    WMI_GET_WPS_STATUS_CMDID,

    /* More P2P commands */
    WMI_SET_NOA_CMDID,
    WMI_GET_NOA_CMDID,
    WMI_SET_OPPPS_CMDID,
    WMI_GET_OPPPS_CMDID,
    WMI_ADD_PORT_CMDID,                       /* F080 */
    WMI_DEL_PORT_CMDID,

    /* 802.11w cmd */
    WMI_SET_RSN_CAP_CMDID,
    WMI_GET_RSN_CAP_CMDID,
    WMI_SET_IGTK_CMDID,

    WMI_RX_FILTER_COALESCE_FILTER_OP_CMDID,
    WMI_RX_FILTER_SET_FRAME_TEST_LIST_CMDID,

    WMI_RTT_MEASREQ_CMDID,
    WMI_RTT_CAPREQ_CMDID,
    WMI_RTT_STATUSREQ_CMDID,

    /*led cotrol*/
    WMI_ENABLE_LED_CMDID,                    /* F08A */
    WMI_CONFIG_LED_CMDID,
    WMI_SET_LED_CMDID,
    /*Socket translation commands*/
    WMI_SOCKET_CMDID,
    WMI_P2P_PSIE_CONFIG_CMDID,
    WMI_LOG_FRAME_CMDID,
    WMI_QUERY_PHY_INFO_CMDID,                /* F090 */
    /* P2P FW Offload support */
    WMI_P2P_CONNECT_CMDID,
    WMI_P2P_GET_NODE_LIST_CMDID,
    WMI_P2P_AUTH_GO_NEG_CMDID,
    WMI_P2P_FW_PROV_DISC_REQ_CMDID,

   WMI_SET_DFS_ENABLE_CMDID,   

    WMI_C2X_FRAME_REPORT_CMDID, /* */
    WMI_C2X_HOST_FEATURE_CONFIG_CMDID, 
    WMI_DIAGNOSTIC_CMDID,   /* diagnostic */
    WMI_SET_FILTERED_PROMISCUOUS_MODE_CMDID,
    
    /*added btcoex command*/
    WMI_SET_BTCOEX_HID_CONFIG_CMDID,         /* F09A */
    WMI_RTT_CONFIG_CMDID,
    WMI_STA_BMISS_ENHANCE_CMDID, 
    WMI_P2P_PERSISTENT_PROFILE_CMDID,
    WMI_P2P_SET_JOIN_PROFILE_CMDID,

    /* wifi heart beat */
    WMI_HB_SET_ENABLE_CMDID,
    WMI_HB_SET_TCP_PARAMS_CMDID,             /* F0A0 */
    WMI_HB_SET_TCP_PKT_FILTER_CMDID,
    WMI_HB_SET_UDP_PARAMS_CMDID,
    WMI_HB_SET_UDP_PKT_FILTER_CMDID,
    WMI_HB_SET_NETWORK_INFO_CMDID,

    /* wifi discovery */
    WMI_DISC_SET_IE_FILTER_CMDID,
    WMI_DISC_SET_MODE_CMDID,
    WMI_RTT_CLKOFFSETCAL_CMDID,
    WMI_P2P_SET_PROFILE_CMDID,
    /* P2P FW GO PS Command */
    WMI_P2P_FW_SET_NOA_CMDID,
    WMI_P2P_FW_GET_NOA_CMDID,                /* F0AA */
    WMI_P2P_FW_SET_OPPPS_CMDID,
    WMI_P2P_FW_GET_OPPPS_CMDID,

    /*led cotrol*/
    WMI_ENABLE_BLINKING_LED_CMDID,

    WMI_AP_POLL_STA_CMDID,
    WMI_AP_PSBUF_OFFLOAD_CMDID,
    WMI_SET_REGDOMAIN_CMDID,
    WMI_ARGOS_CMDID,
    WMI_SEND_MGMT_CMDID,
    WMI_BEGIN_SCAN_CMDID,
    WMI_SET_IE_CMDID,
    WMI_SET_RSSI_FILTER_CMDID,
    WMI_SET_CREDIT_REVERSE_CMDID   = 0xF0B6,
    WMI_SET_RCV_DATA_CLASSIFIER_CMDID,
    WMI_AP_SET_IDLE_CLOSE_TIME_CMDID,
    WMI_SET_LTE_COEX_STATE_CMDID,
    WMI_SET_MCC_PROFILE_CMDID,
    WMI_SET_MEDIA_STREAM_CMDID = 0xF0BB,

    /* More customer private commands */
    WMI_SET_CUSTOM_REG,	/* F0BC */
    WMI_GET_CUSTOM_REG,
    WMI_GET_CUSTOM_PRODUCT_INFO,
    WMI_SET_CUSTOM_TESTMODE,
    WMI_GET_CUSTOM_TESTMODE,	/* F0C0 */
    WMI_GET_CUSTOM_STAINFO,
    WMI_GET_CUSTOM_SCANTIME,
    WMI_SET_CUSTOM_SCAN,
    WMI_GET_CUSTOM_SCAN,
    WMI_GET_CUSTOM_VERSION_INFO,
    WMI_GET_CUSTOM_WIFI_TXPOW,
    WMI_GET_CUSTOM_ATHSTATS,

    WMI_TX99TOOL_CMDID,/* F0C8 */
	WMI_SET_CUSTOM_PROBE_RESP_REPORT_CMDID,
    WMI_SET_CUSTOM_WIDI,
    WMI_GET_CUSTOM_WIDI,

	/*Diversity control*/
	WMI_SET_ANTDIVCFG_CMDID, /* F0CC */
	WMI_GET_ANTDIVSTAT_CMDID,

    WMI_SET_SEAMLESS_MCC_SCC_SWITCH_FREQ_CMDID,
    WMI_SET_CHAIN_MASK_CMDID,
    
    WMI_SET_SCAN_CHAN_PLAN_CMDID,
    WMI_SET_MCC_EVENT_MODE_CMDID,    
	WMI_GET_CTL,

    /* 802.11p */
    WMI_SET_OCB_FLAG_CMDID,
    WMI_SET_OCB_CHANNEL_CMDID,
    WMI_SET_LEGACY_OPT_CMDID,
    WMI_SET_TX_POWER_SCALE_CMDID, /* F0D6 */ 
    WMI_P2P_LIST_PERSISTENT_NETWORK_CMDID,   /* F0D7 */	

    WMI_SET_PMU_START_CMDID,			     /* F0D8 */
    WMI_SET_PMU_RECALL_CMDID,
    WMI_SET_PMU_BACKUP_REGISTER_CMDID,
    WMI_SET_PMU_TEST_VALUE_CMDID,

    WMI_PFM_GET_CMDID,				    /* F0DC  */
    WMI_CFG_RC_PARAM_CMDID, 

    WMI_DSET_HOST_CFG_CMDID,			   /* F0DE */
    WMI_DSET_HOST_READ_CMDID,

    WMI_SET_PMU_SUSPEND_GPIO_CMDID,

    /* i2c, i2s related IDs */

    /* IDs for i2c operation */
    WMI_I2C_INIT_CMDID,
    WMI_I2C_READ_CMDID,
    WMI_I2C_WRITE_CMDID,

    /* IDs for i2s operation */
    WMI_I2S_INIT_CMDID,
    WMI_I2S_XMT_DATA_CMDID,
    WMI_I2S_RCV_DATA_CMDID,
    WMI_I2S_RCV_CTRL_CMDID,
    WMI_WNM_SLEEP_CMDID,
    WMI_WNM_TIM_CMDID,


    WMI_DSET_HOST_LARGE_READ_CMDID,		/* F0EA */
    WMI_DSET_HOST_LARGE_WRITE_CMDID,
    WMI_DSET_HOST_RESP_CREATE_CMDID,            /* F0EC */
    WMI_DSET_HOST_RESP_SYNC_CMDID,
    WMI_DSET_HOST_RESP_READ_CMDID,
    WMI_DSET_HOST_RESP_WRITE_CMDID,

    WMI_DSET_OP_CMDID,                           /* F0F0 */
    WMI_GET_TEMPERATURE_CMDID,              /* F0F1 */
    WMI_PWM_CTRL_CMDID,                           /* F0F2 */
    WMI_PWM_PORT_SET_CMDID,                           /* F0F3 */
    WMI_PWM_SDM_SET_CMDID,                           /* F0F4 */
    WMI_I2CM_CTRL_CMDID,                           /* F0F5 */
    WMI_I2CM_INIT_CMDID,                           /* F0F6 */
    WMI_I2CS_CTRL_CMDID,                           /* F0F7 */
    WMI_I2CS_MODE_CMDID,                           /* F0F8 */
    WMI_I2CS_CMD_CMDID,                           /* F0F9 */
	WMI_ADC_CMDID,
    WMI_EBT_MAC_FILTER_CMDID,                     /* F0FB */
    WMI_WPS_SET_CONFIG_STATE_CMDID,               /* F0FC */
    WMI_PARAM_GET_CMDID,                          /* F0FD*/
    WMI_PARAM_SET_CMDID,                          /* F0FE*/
    WMI_AP_GET_STA_INFO_CMDID,                    /*F0FF*/
    WMI_SET_HWANTDIVCFG_CMDID,					  /* F100  */
    WMI_GET_HWANTDIVSTAT_CMDID,					  /* F101  */
    WMI_SET_BTCOEX_SCHEME_CMDID,                  /* F102  */

} WMI_COMMAND_ID;

/* WMI_WPS_CONFIG_SET_CMD */
typedef PREPACK struct {
    A_INT32 mode;
} POSTPACK  WMI_WPS_CONFIG_SET_CMD;

typedef enum {
     OPT_DISABLED=0,
     ONLY_11B_OPT_ENABLED,
     ALL_LEGACY_OPT_ENABLED,
}LEGACY_OPT_MODE;
/*
 * Frame Types
 */
typedef enum {
    WMI_FRAME_BEACON        =   0,
    WMI_FRAME_PROBE_REQ,
    WMI_FRAME_PROBE_RESP,
    WMI_FRAME_ASSOC_REQ,
    WMI_FRAME_ASSOC_RESP,
    WMI_NUM_MGMT_FRAME
} WMI_MGMT_FRAME_TYPE;

/*
 * Large Frame Types
 */
typedef enum {
    WMI_FRAME_BEACON_LARGE,
    WMI_FRAME_PROBE_REQ_LARGE,
    WMI_FRAME_PROBE_RESP_LARGE,
    WMI_FRAME_ASSOC_REQ_LARGE,
    WMI_FRAME_ASSOC_RESP_LARGE,
    WMI_NUM_MGMT_FRAME_LARGE
} WMI_MGMT_FRAME_LARGE_TYPE;

typedef enum {
    WMI_RSN_IE_CAPB	= 0x1,
    WMI_IE_FULL		= 0xFF  /* indicats full IE */
} WMI_IE_FIELD_TYPE;

/*
 * Connect Command
 */
typedef enum {
    INFRA_NETWORK       = 0x01,
    ADHOC_NETWORK       = 0x02,
    ADHOC_CREATOR       = 0x04,
    AP_NETWORK          = 0x10,
    NETWORK_CONNECTED_USING_WPS = 0x20,
} NETWORK_TYPE;

typedef enum {
    SUBTYPE_NONE,
    SUBTYPE_BT,
    SUBTYPE_P2PDEV,
    SUBTYPE_P2PCLIENT,
    SUBTYPE_P2PGO,
    SUBTYPE_P2PDEV_DEDICATE,
    SUBTYPE_FW_P2PDEV,
    SUBTYPE_FW_P2PCLIENT,
    SUBTYPE_FW_P2PGO,
} NETWORK_SUBTYPE;

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

#define WMI_MIN_CRYPTO_TYPE NONE_CRYPT
#define WMI_MAX_CRYPTO_TYPE (AES_CRYPT + 1)

#ifdef WAPI_ENABLE
#undef WMI_MAX_CRYPTO_TYPE
#define WMI_MAX_CRYPTO_TYPE (WAPI_CRYPT + 1)
#endif /* WAPI_ENABLE */

#ifdef WAPI_ENABLE
#define IW_ENCODE_ALG_SM4       0x20
/*
 * Defined this to be some very high bit because it is less likely to be
 * clobbered by future changes to the kernel's wireless.h file
*/
#define IW_AUTH_CIPHER_SMS4     0x40000000
#endif

#define WMI_MIN_KEY_INDEX   0
#define WMI_MAX_KEY_INDEX   3
#define WMI_MAX_KEY_INDEX_EX    6

#ifdef IEEE80211W
#undef WMI_MAX_KEY_INDEX
#define WMI_MAX_KEY_INDEX   5
#endif

#ifdef WAPI_ENABLE
#undef WMI_MAX_KEY_INDEX
#define WMI_MAX_KEY_INDEX   7 /* wapi grpKey 0-3, prwKey 4-7 */
#endif /* WAPI_ENABLE */

#define WMI_MAX_KEY_LEN     32

#define WMI_MAX_SSID_LEN    32

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

#define DEFAULT_CONNECT_CTRL_FLAGS    (CONNECT_CSA_FOLLOW_BSS)

#define WMI_CONNECT_AP_CHAN_SELECT_OFFSET  (14)
#define WMI_CONNECT_AP_CHAN_SELECT_MASK    (0xc000)

typedef enum {
    AP_CHANNEL_TYPE_NONE = 0,
    AP_CHANNEL_TYPE_HT40PLUS,
    AP_CHANNEL_TYPE_HT40MINUS,
    AP_CHANNEL_TYPE_HT20
} WMI_CONNECT_AP_CHANNEL_TYPE;

typedef PREPACK struct {
    A_UINT8     networkType;
    A_UINT8     dot11AuthMode;
    A_UINT8     authMode;
    A_UINT8     pairwiseCryptoType;
    A_UINT8     pairwiseCryptoLen;
    A_UINT8     groupCryptoType;
    A_UINT8     groupCryptoLen;
    A_UINT8     ssidLength;
    A_UCHAR     ssid[WMI_MAX_SSID_LEN];
    A_UINT16    channel;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT32    ctrl_flags;
} POSTPACK WMI_CONNECT_CMD;


typedef PREPACK struct {
    A_UINT32 divIdleTime;
    A_UINT8   antRssiThresh;
    A_UINT8   divEnable;
    A_UINT16 active_treshold_rate;
} POSTPACK WMI_DIV_PARAMS_CMD;

/*
 * WMI_RECONNECT_CMDID
 */
typedef PREPACK struct {
    A_UINT16    channel;                    /* hint */
    A_UINT8     bssid[ATH_MAC_LEN];         /* mandatory if set */
} POSTPACK WMI_RECONNECT_CMD;

typedef PREPACK struct {
    A_INT32  param_val;
}POSTPACK WMI_SET_TX_POWER_SCALE_CMD;

/*
 * WMI_SET_PMK_CMDID
 */
#define WMI_PMK_LEN     32
typedef PREPACK struct {
    A_UINT8 pmk[WMI_PMK_LEN];
    A_UINT8 pmk_len;
} POSTPACK WMI_SET_PMK_CMD, WMI_GET_PMK_REPLY;

/*
 * WMI_SET_PASSPHRASE_CMDID
 */
#define WMI_PASSPHRASE_LEN 64
typedef SET_PASSPHRASE_CMD WMI_SET_PASSPHRASE_CMD;

/*
 * WMI_SET_EXCESS_TX_RETRY_THRES_CMDID
 */
typedef PREPACK struct {
    A_UINT32 threshold;
} POSTPACK WMI_SET_EXCESS_TX_RETRY_THRES_CMD;

/*
 * WMI_ADD_CIPHER_KEY_CMDID
 */
typedef enum {
    PAIRWISE_USAGE      = 0x00,
    GROUP_USAGE         = 0x01,
    TX_USAGE            = 0x02,     /* default Tx Key - Static WEP only */
} KEY_USAGE;

/*
 * Bit Flag
 * Bit 0 - Initialise TSC - default is Initialize
 */
#define KEY_OP_INIT_TSC       0x01
#define KEY_OP_INIT_RSC       0x02
#ifdef WAPI_ENABLE
#define KEY_OP_INIT_WAPIPN    0x10
#endif /* WAPI_ENABLE */

#define KEY_OP_INIT_VAL     0x03     /* Default Initialise the TSC & RSC */
#define KEY_OP_VALID_MASK   0x03

typedef PREPACK struct {
    A_UINT8     keyIndex;
    A_UINT8     keyType;
    A_UINT8     keyUsage;           /* KEY_USAGE */
    A_UINT8     keyLength;
    A_UINT8     keyRSC[8];          /* key replay sequence counter */
    A_UINT8     key[WMI_MAX_KEY_LEN];
    A_UINT8     key_op_ctrl;       /* Additional Key Control information */
    A_UINT8    key_macaddr[ATH_MAC_LEN];
} POSTPACK WMI_ADD_CIPHER_KEY_CMD;

/*
 * WMI_DELETE_CIPHER_KEY_CMDID
 */
typedef PREPACK struct {
    A_UINT8     keyIndex;
} POSTPACK WMI_DELETE_CIPHER_KEY_CMD;


/*
 *for Linux, it need KRK, length is 16
 *for Win7, it need CCKM ie, length is 26
 */
#define WMI_KRK_LEN     16
#define WMI_CCKM_IE_LEN 26
/*
 * WMI_ADD_KRK_CMDID
 */
typedef PREPACK struct {
    A_UINT8     krk[WMI_CCKM_IE_LEN];
} POSTPACK WMI_ADD_KRK_CMD;

/*
 * WMI_SET_TKIP_COUNTERMEASURES_CMDID
 */
typedef enum {
    WMI_TKIP_CM_DISABLE = 0x0,
    WMI_TKIP_CM_ENABLE  = 0x1,
} WMI_TKIP_CM_CONTROL;

typedef PREPACK struct {
    A_UINT8  cm_en;                     /* WMI_TKIP_CM_CONTROL */
} POSTPACK WMI_SET_TKIP_COUNTERMEASURES_CMD;

/*
 * WMI_SET_PMKID_CMDID
 */

#define WMI_PMKID_LEN 16

typedef enum {
   PMKID_DISABLE = 0,
   PMKID_ENABLE  = 1,
} PMKID_ENABLE_FLG;

typedef PREPACK struct {
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT8     enable;                 /* PMKID_ENABLE_FLG */
    A_UINT8     pmkid[WMI_PMKID_LEN];
} POSTPACK WMI_SET_PMKID_CMD;

/*
 * WMI_START_SCAN_CMD
 */
typedef enum {
    WMI_LONG_SCAN  = 0,
    WMI_SHORT_SCAN = 1,
} WMI_SCAN_TYPE;

typedef PREPACK struct {
    A_BOOL   forceFgScan;
    A_BOOL   isLegacy;        /* For Legacy Cisco AP compatibility */
    A_UINT32 homeDwellTime;   /* Maximum duration in the home channel(milliseconds) */
    A_UINT32 forceScanInterval;    /* Time interval between scans (milliseconds)*/
    A_UINT8  scanType;           /* WMI_SCAN_TYPE */
    A_UINT8  numChannels;            /* how many channels follow */
    A_UINT16 channelList[1];         /* channels in Mhz */
} POSTPACK WMI_START_SCAN_CMD;

/*
 * WMI_SET_SCAN_PARAMS_CMDID
 */
#define WMI_SHORTSCANRATIO_DEFAULT      3
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

#define CAN_SCAN_IN_CONNECT(flags)      (flags & CONNECT_SCAN_CTRL_FLAGS)
#define CAN_SCAN_CONNECTED(flags)       (flags & SCAN_CONNECTED_CTRL_FLAGS)
#define ENABLE_ACTIVE_SCAN(flags)       (flags & ACTIVE_SCAN_CTRL_FLAGS)
#define ENABLE_ROAM_SCAN(flags)         (flags & ROAM_SCAN_CTRL_FLAGS)
#define CONFIG_REPORT_BSSINFO(flags)     (flags & REPORT_BSSINFO_CTRL_FLAGS)
#define IS_AUTO_SCAN_ENABLED(flags)      (flags & ENABLE_AUTO_CTRL_FLAGS)
#define SCAN_ABORT_EVENT_ENABLED(flags) (flags & ENABLE_SCAN_ABORT_EVENT)
#define ENABLE_DFS_SKIP_SCAN(flags)     (flags & ENABLE_DFS_SKIP_CTRL_FLAGS)

#define DEFAULT_SCAN_CTRL_FLAGS         (CONNECT_SCAN_CTRL_FLAGS| SCAN_CONNECTED_CTRL_FLAGS| ACTIVE_SCAN_CTRL_FLAGS| ROAM_SCAN_CTRL_FLAGS | ENABLE_AUTO_CTRL_FLAGS)


typedef PREPACK struct {
    A_UINT16    fg_start_period;        /* seconds */
    A_UINT16    fg_end_period;          /* seconds */
    A_UINT16    bg_period;              /* seconds */
    A_UINT16    maxact_chdwell_time;    /* msec */
    A_UINT16    pas_chdwell_time;       /* msec */
    A_UINT8     shortScanRatio;         /* how many shorts scan for one long */
    A_UINT8     scanCtrlFlags;
    A_UINT16    minact_chdwell_time;    /* msec */
    A_UINT16    maxact_scan_per_ssid;   /* max active scans per ssid */
    A_UINT32    max_dfsch_act_time;  /* msecs */
} POSTPACK WMI_SCAN_PARAMS_CMD;

typedef PREPACK struct {
    A_UINT16   chan_index;
    A_INT8    bang_radar;
} POSTPACK WMI_RADAR_DETECTED_CMD;

/*
 * WMI_SET_BSS_FILTER_CMDID
 */
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

typedef PREPACK struct {
    A_UINT8    bssFilter;                      /* see WMI_BSS_FILTER */
    A_UINT8    reserved1;                      /* For alignment */
    A_UINT16   reserved2;                      /* For alignment */
    A_UINT32   ieMask;
} POSTPACK WMI_BSS_FILTER_CMD;

/*
 * WMI_SET_PROBED_SSID_CMDID
 */
#define MAX_PROBED_SSID_INDEX   15

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


typedef PREPACK struct {
    A_UINT8     entryIndex;                     /* 0 to MAX_PROBED_SSID_INDEX */
    A_UINT8     flag;                           /* WMI_SSID_FLG */
    A_UINT8     ssidLength;
    A_UINT8     ssid[32];
    A_UINT8     authentication;
    A_UINT8     encryption;
    A_UINT8     channelHint;
    A_BOOL      bmatchcompleteprofile;         /* If this Flag is True, we do a complete profile match of SSID, AUTH, Encryption */
} POSTPACK WMI_PROBED_SSID_EX_CMD;

/*
 * WMI_SET_LISTEN_INT_CMDID
 * The Listen interval is between 15 and 3000 TUs
 */
#define MIN_LISTEN_INTERVAL 15
#define MAX_LISTEN_INTERVAL 5000
#define MIN_LISTEN_BEACONS 1
#define MAX_LISTEN_BEACONS 50

typedef PREPACK struct {
    A_UINT16     listenInterval;
    A_UINT16     numBeacons;
} POSTPACK WMI_LISTEN_INT_CMD;

/*
 * WMI_SET_BEACON_INT_CMDID
 */
typedef PREPACK struct {
    A_UINT16     beaconInterval;
} POSTPACK WMI_BEACON_INT_CMD;

/*
 * WMI_SET_BMISS_TIME_CMDID
 * valid values are between 1000 and 5000 TUs
 */

#define MIN_BMISS_TIME     1000
#define MAX_BMISS_TIME     5000
#define MIN_BMISS_BEACONS  1
#define MAX_BMISS_BEACONS  50

typedef PREPACK struct {
    A_UINT16     bmissTime;
    A_UINT16     numBeacons;
} POSTPACK WMI_BMISS_TIME_CMD;

/*
 * WMI_SET_POWER_MODE_CMDID
 */
typedef enum {
    REC_POWER = 0x01,
    MAX_PERF_POWER,
} WMI_POWER_MODE;

typedef PREPACK struct {
    A_UINT8     powerMode;      /* WMI_POWER_MODE */
} POSTPACK WMI_POWER_MODE_CMD;

typedef PREPACK struct {
    A_INT8 status;      /* WMI_SET_COUNTRY_CODE_REPLY */
} POSTPACK WMI_SET_COUNTRY_CODE_REPLY;

typedef PREPACK struct {
    A_UINT32 opcode;
    A_UINT32 length;
    A_CHAR buffer[1];      /* WMI_SET_STA_COUNTRY_CODE */
} POSTPACK WMI_SET_STA_COUNTRY_CODE_CMD;

typedef PREPACK struct {
    A_UINT8 multicast_mac[ATH_MAC_LEN];      /* WMI_SET_MCAST_FILTER */
} POSTPACK WMI_SET_MCAST_FILTER_CMD;

typedef PREPACK struct {
    A_UINT8 enable;      /* WMI_MCAST_FILTER */
} POSTPACK WMI_MCAST_FILTER_CMD;

/*
 * WMI_SET_POWER_PARAMS_CMDID
 */
typedef enum {
    IGNORE_DTIM = 0x01,
    NORMAL_DTIM = 0x02,
    STICK_DTIM  = 0x03,
    AUTO_DTIM   = 0x04,
} WMI_DTIM_POLICY;

/* Policy to determnine whether TX should wakeup WLAN if sleeping */
typedef enum {
    TX_WAKEUP_UPON_SLEEP = 1,
    TX_DONT_WAKEUP_UPON_SLEEP = 2
} WMI_TX_WAKEUP_POLICY_UPON_SLEEP;

/*
 * Policy to determnine whether power save failure event should be sent to
 * host during scanning
 */
typedef enum {
    SEND_POWER_SAVE_FAIL_EVENT_ALWAYS = 1,
    IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN = 2,
} POWER_SAVE_FAIL_EVENT_POLICY;

typedef PREPACK struct {
    A_UINT16    idle_period;             /* msec */
    A_UINT16    pspoll_number;
    A_UINT16    dtim_policy;
    A_UINT16    tx_wakeup_policy;
    A_UINT16    num_tx_to_wakeup;
    A_UINT16    ps_fail_event_policy;
} POSTPACK WMI_POWER_PARAMS_CMD;

/* Adhoc power save types */
typedef enum {
    ADHOC_PS_DISABLE=1,
    ADHOC_PS_ATH=2,
    ADHOC_PS_IEEE=3,
    ADHOC_PS_OTHER=4,
    ADHOC_PS_KTK=5,
} WMI_ADHOC_PS_TYPE;

typedef PREPACK struct {
    A_UINT8    power_saving;
    A_UINT8    ttl; /* number of beacon periods */
    A_UINT16   atim_windows;          /* msec */
    A_UINT16   timeout_value;         /* msec */
} POSTPACK WMI_IBSS_PM_CAPS_CMD;

/* AP power save types */
typedef enum {
    AP_PS_DISABLE=1,
    AP_PS_ATH=2,
} WMI_AP_PS_TYPE;

typedef PREPACK struct {
    A_UINT32   idle_time;   /* in msec */
    A_UINT32   ps_period;   /* in usec */
    A_UINT8    sleep_period; /* in ps periods */
    A_UINT8    psType;
} POSTPACK WMI_AP_PS_CMD;

typedef PREPACK struct {
    A_UINT8    enable; /* enable/disable IoE AP PS Offload */
    A_UINT8    psBufCount; /* PS Buf count per PS Client */
} POSTPACK WMI_AP_PSBUF_OFFLOAD_CMD;

/*
 * WMI_SET_POWERSAVE_TIMERS_POLICY_CMDID
 */
typedef enum {
    IGNORE_TIM_ALL_QUEUES_APSD = 0,
    PROCESS_TIM_ALL_QUEUES_APSD = 1,
    IGNORE_TIM_SIMULATED_APSD = 2,
    PROCESS_TIM_SIMULATED_APSD = 3,
} APSD_TIM_POLICY;

typedef PREPACK struct {
    A_UINT16    psPollTimeout;          /* msec */
    A_UINT16    triggerTimeout;         /* msec */
    A_UINT32    apsdTimPolicy;      /* TIM behavior with  ques APSD enabled. Default is IGNORE_TIM_ALL_QUEUES_APSD */
    A_UINT32    simulatedAPSDTimPolicy;      /* TIM behavior with  simulated APSD enabled. Default is PROCESS_TIM_SIMULATED_APSD */
} POSTPACK WMI_POWERSAVE_TIMERS_POLICY_CMD;

/*
 * WMI_SET_VOICE_PKT_SIZE_CMDID
 */
typedef PREPACK struct {
    A_UINT16    voicePktSize;
} POSTPACK WMI_SET_VOICE_PKT_SIZE_CMD;

/*
 * WMI_SET_MAX_SP_LEN_CMDID
 */
typedef enum {
    DELIVER_ALL_PKT = 0x0,
    DELIVER_2_PKT = 0x1,
    DELIVER_4_PKT = 0x2,
    DELIVER_6_PKT = 0x3,
} APSD_SP_LEN_TYPE;

typedef PREPACK struct {
    A_UINT8    maxSPLen;
} POSTPACK WMI_SET_MAX_SP_LEN_CMD;

/*
 * WMI_SET_DISC_TIMEOUT_CMDID
 */
typedef PREPACK struct {
    A_UINT8     disconnectTimeout;          /* seconds */
} POSTPACK WMI_DISC_TIMEOUT_CMD;

typedef enum {
    UPLINK_TRAFFIC = 0,
    DNLINK_TRAFFIC = 1,
    BIDIR_TRAFFIC = 2,
} DIR_TYPE;

typedef enum {
    DISABLE_FOR_THIS_AC = 0,
    ENABLE_FOR_THIS_AC  = 1,
    ENABLE_FOR_ALL_AC   = 2,
} VOICEPS_CAP_TYPE;

typedef enum {
    TRAFFIC_TYPE_APERIODIC = 0,
    TRAFFIC_TYPE_PERIODIC = 1,
}TRAFFIC_TYPE;

/*
 * WMI_SYNCHRONIZE_CMDID
 */
typedef PREPACK struct {
    A_UINT8 dataSyncMap;
} POSTPACK WMI_SYNC_CMD;

/*
 * WMI_CREATE_PSTREAM_CMDID
 */
typedef PREPACK struct {
    A_UINT32        minServiceInt;           /* in milli-sec */
    A_UINT32        maxServiceInt;           /* in milli-sec */
    A_UINT32        inactivityInt;           /* in milli-sec */
    A_UINT32        suspensionInt;           /* in milli-sec */
    A_UINT32        serviceStartTime;
    A_UINT32        minDataRate;             /* in bps */
    A_UINT32        meanDataRate;            /* in bps */
    A_UINT32        peakDataRate;            /* in bps */
    A_UINT32        maxBurstSize;
    A_UINT32        delayBound;
    A_UINT32        minPhyRate;              /* in bps */
    A_UINT32        sba;
    A_UINT32        mediumTime;
    A_UINT16        nominalMSDU;             /* in octects */
    A_UINT16        maxMSDU;                 /* in octects */
    A_UINT8         trafficClass;
    A_UINT8         trafficDirection;        /* DIR_TYPE */
    A_UINT8         rxQueueNum;
    A_UINT8         trafficType;             /* TRAFFIC_TYPE */
    A_UINT8         voicePSCapability;       /* VOICEPS_CAP_TYPE */
    A_UINT8         tsid;
    A_UINT8         userPriority;            /* 802.1D user priority */
    A_UINT8         nominalPHY;              /* nominal phy rate */
} POSTPACK WMI_CREATE_PSTREAM_CMD;

/*
 * WMI_DELETE_PSTREAM_CMDID
 */
typedef PREPACK struct {
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;
    A_UINT8     trafficClass;
    A_UINT8     tsid;
} POSTPACK WMI_DELETE_PSTREAM_CMD;

/*
 * WMI_SET_CHANNEL_PARAMS_CMDID
 */
typedef enum {
    WMI_11A_MODE  = 0x1,
    WMI_11G_MODE  = 0x2,
    WMI_11AG_MODE = 0x3,
    WMI_11B_MODE  = 0x4,
    WMI_11GONLY_MODE = 0x5,
} WMI_PHY_MODE;

#define WMI_MAX_CHANNELS        32
#define WMI_NLO_SCAN_PARAM		2

typedef PREPACK struct {
    A_UINT8     reserved1;
    A_UINT8     scanParam;              /* set if enable scan */
    A_UINT8     phyMode;                /* see WMI_PHY_MODE */
    A_UINT8     numChannels;            /* how many channels follow */
    A_UINT16    channelList[1];         /* channels in Mhz */
} POSTPACK WMI_CHANNEL_PARAMS_CMD;


/*
 *  WMI_RSSI_THRESHOLD_PARAMS_CMDID
 *  Setting the polltime to 0 would disable polling.
 *  Threshold values are in the ascending order, and should agree to:
 *  (lowThreshold_lowerVal < lowThreshold_upperVal < highThreshold_lowerVal
 *      < highThreshold_upperVal)
 */

typedef PREPACK struct WMI_RSSI_THRESHOLD_PARAMS{
    A_UINT32    pollTime;               /* Polling time as a factor of LI */
    A_INT16     thresholdAbove1_Val;          /* lowest of upper */
    A_INT16     thresholdAbove2_Val;
    A_INT16     thresholdAbove3_Val;
    A_INT16     thresholdAbove4_Val;
    A_INT16     thresholdAbove5_Val;
    A_INT16     thresholdAbove6_Val;          /* highest of upper */
    A_INT16     thresholdBelow1_Val;         /* lowest of bellow */
    A_INT16     thresholdBelow2_Val;
    A_INT16     thresholdBelow3_Val;
    A_INT16     thresholdBelow4_Val;
    A_INT16     thresholdBelow5_Val;
    A_INT16     thresholdBelow6_Val;         /* highest of bellow */
    A_UINT8     weight;                  /* "alpha" */
    A_UINT8     reserved[3];
} POSTPACK  WMI_RSSI_THRESHOLD_PARAMS_CMD;

/*
 *  WMI_SNR_THRESHOLD_PARAMS_CMDID
 *  Setting the polltime to 0 would disable polling.
 */

typedef PREPACK struct WMI_SNR_THRESHOLD_PARAMS{
    A_UINT32    pollTime;               /* Polling time as a factor of LI */
    A_UINT8     weight;                  /* "alpha" */
    A_UINT8     thresholdAbove1_Val;      /* lowest of uppper*/
    A_UINT8     thresholdAbove2_Val;
    A_UINT8     thresholdAbove3_Val;
    A_UINT8     thresholdAbove4_Val;      /* highest of upper */
    A_UINT8     thresholdBelow1_Val;     /* lowest of bellow */
    A_UINT8     thresholdBelow2_Val;
    A_UINT8     thresholdBelow3_Val;
    A_UINT8     thresholdBelow4_Val;     /* highest of bellow */
    A_UINT8     reserved[3];
} POSTPACK WMI_SNR_THRESHOLD_PARAMS_CMD;

/*
 *  WMI_LQ_THRESHOLD_PARAMS_CMDID
 */
typedef PREPACK struct WMI_LQ_THRESHOLD_PARAMS {
    A_UINT8     enable;
    A_UINT8     thresholdAbove1_Val;
    A_UINT8     thresholdAbove2_Val;
    A_UINT8     thresholdAbove3_Val;
    A_UINT8     thresholdAbove4_Val;
    A_UINT8     thresholdBelow1_Val;
    A_UINT8     thresholdBelow2_Val;
    A_UINT8     thresholdBelow3_Val;
    A_UINT8     thresholdBelow4_Val;
    A_UINT8     reserved[3];
} POSTPACK  WMI_LQ_THRESHOLD_PARAMS_CMD;

typedef enum {
    WMI_LPREAMBLE_DISABLED = 0,
    WMI_LPREAMBLE_ENABLED
} WMI_LPREAMBLE_STATUS;

typedef enum {
    WMI_IGNORE_BARKER_IN_ERP = 0,
    WMI_DONOT_IGNORE_BARKER_IN_ERP
} WMI_PREAMBLE_POLICY;

typedef PREPACK struct {
    A_UINT8     status;
    A_UINT8     preamblePolicy;
}POSTPACK WMI_SET_LPREAMBLE_CMD;

typedef PREPACK struct {
    A_UINT16    threshold;
}POSTPACK WMI_SET_RTS_CMD;

/*
 *  WMI_TARGET_ERROR_REPORT_BITMASK_CMDID
 *  Sets the error reporting event bitmask in target. Target clears it
 *  upon an error. Subsequent errors are counted, but not reported
 *  via event, unless the bitmask is set again.
 */
typedef PREPACK struct {
    A_UINT32    bitmask;
} POSTPACK  WMI_TARGET_ERROR_REPORT_BITMASK;

/*
 * WMI_SET_TX_PWR_CMDID
 */
typedef PREPACK struct {
    A_UINT8     dbM;                  /* in dbM units */
} POSTPACK WMI_SET_TX_PWR_CMD, WMI_TX_PWR_REPLY;

/*
 * WMI_SET_ASSOC_INFO_CMDID
 *
 * A maximum of 2 private IEs can be sent in the [Re]Assoc request.
 * A 3rd one, the C2X version IE can also be set from the host.
 */
#define WMI_MAX_ASSOC_INFO_TYPE    2

#define WMI_MAX_ASSOC_INFO_LEN     240

typedef PREPACK struct {
    A_UINT8     ieType;
    A_UINT8     bufferSize;
    A_UINT8     assocInfo[1];       /* up to WMI_MAX_ASSOC_INFO_LEN */
} POSTPACK WMI_SET_ASSOC_INFO_CMD;


/*
 * WMI_GET_TX_PWR_CMDID does not take any parameters
 */

/*
 * WMI_ADD_BAD_AP_CMDID
 */
#define WMI_MAX_BAD_AP_INDEX      1

typedef PREPACK struct {
    A_UINT8     badApIndex;         /* 0 to WMI_MAX_BAD_AP_INDEX */
    A_UINT8     bssid[ATH_MAC_LEN];
} POSTPACK WMI_ADD_BAD_AP_CMD;

/*
 * WMI_DELETE_BAD_AP_CMDID
 */
typedef PREPACK struct {
    A_UINT8     badApIndex;         /* 0 to WMI_MAX_BAD_AP_INDEX */
} POSTPACK WMI_DELETE_BAD_AP_CMD;

/*
 * WMI_SET_ACCESS_PARAMS_CMDID
 */
#define WMI_DEFAULT_TXOP_ACPARAM    0       /* implies one MSDU */
#define WMI_DEFAULT_ECWMIN_ACPARAM  4       /* corresponds to CWmin of 15 */
#define WMI_DEFAULT_ECWMAX_ACPARAM  10      /* corresponds to CWmax of 1023 */
#define WMI_MAX_CW_ACPARAM          15      /* maximum eCWmin or eCWmax */
#define WMI_DEFAULT_AIFSN_ACPARAM   2
#define WMI_MAX_AIFSN_ACPARAM       15
typedef PREPACK struct {
    A_UINT16 txop;                      /* in units of 32 usec */
    A_UINT8  eCWmin;
    A_UINT8  eCWmax;
    A_UINT8  aifsn;
    A_UINT8  ac;
} POSTPACK WMI_SET_ACCESS_PARAMS_CMD;


/*
 * WMI_SET_RETRY_LIMITS_CMDID
 *
 * This command is used to customize the number of retries the
 * wlan device will perform on a given frame.
 */
#define WMI_MIN_RETRIES 2
#define WMI_MAX_RETRIES 13
typedef enum {
    MGMT_FRAMETYPE    = 0,
    CONTROL_FRAMETYPE = 1,
    DATA_FRAMETYPE    = 2
} WMI_FRAMETYPE;

typedef PREPACK struct {
    A_UINT8 frameType;                      /* WMI_FRAMETYPE */
    A_UINT8 trafficClass;                   /* applies only to DATA_FRAMETYPE */
    A_UINT8 maxRetries;
    A_UINT8 enableNotify;
} POSTPACK WMI_SET_RETRY_LIMITS_CMD;

/*
 * WMI_SET_ROAM_CTRL_CMDID
 *
 * This command is used to influence the Roaming behaviour
 * Set the host biases of the BSSs before setting the roam mode as bias
 * based.
 */

/*
 * Different types of Roam Control
 */

typedef enum {
        WMI_FORCE_ROAM          = 1,      /* Roam to the specified BSSID */
        WMI_SET_ROAM_MODE       = 2,      /* default ,progd bias, no roam */
        WMI_SET_HOST_BIAS       = 3,     /* Set the Host Bias */
        WMI_SET_LOWRSSI_SCAN_PARAMS = 4, /* Set lowrssi Scan parameters */
        WMI_SET_HOST_5G_BIAS    = 5,     /* Set the Host 5G Bias */
} WMI_ROAM_CTRL_TYPE;

#define WMI_MIN_ROAM_CTRL_TYPE WMI_FORCE_ROAM
#define WMI_MAX_ROAM_CTRL_TYPE WMI_SET_LOWRSSI_SCAN_PARAMS

/*
 * ROAM MODES
 */

typedef enum {
        WMI_DEFAULT_ROAM_MODE   = 1,  /* RSSI based ROAM */
        WMI_HOST_BIAS_ROAM_MODE = 2, /* HOST BIAS based ROAM */
        WMI_LOCK_BSS_MODE  = 3  /* Lock to the Current BSS - no Roam */
} WMI_ROAM_MODE;

/*
 * BSS HOST BIAS INFO
 */

typedef PREPACK struct {
        A_UINT8 bssid[ATH_MAC_LEN];
        A_INT8  bias;
} POSTPACK WMI_BSS_BIAS;

typedef PREPACK struct {
        A_UINT8 numBss;
        WMI_BSS_BIAS bssBias[1];
} POSTPACK WMI_BSS_BIAS_INFO;

typedef PREPACK struct WMI_LOWRSSI_SCAN_PARAMS {
        A_UINT16 lowrssi_scan_period;
        A_INT16  lowrssi_scan_threshold;
        A_INT16  lowrssi_roam_threshold;
        A_UINT8  roam_rssi_floor;
        A_UINT8  reserved[1];              /* For alignment */
} POSTPACK WMI_LOWRSSI_SCAN_PARAMS;

typedef PREPACK struct {
    PREPACK union {
        A_UINT8 bssid[ATH_MAC_LEN]; /* WMI_FORCE_ROAM */
        A_UINT8 roamMode;           /* WMI_SET_ROAM_MODE  */
        WMI_BSS_BIAS_INFO bssBiasInfo; /* WMI_SET_HOST_BIAS */
        A_UINT8 bias5G; /* WMI_SET_HOST_5G_BIAS */
        WMI_LOWRSSI_SCAN_PARAMS lrScanParams;
    } POSTPACK info;
    A_UINT8   roamCtrlType ;
} POSTPACK WMI_SET_ROAM_CTRL_CMD;

/*
 * WMI_SET_BT_WLAN_CONN_PRECEDENCE_CMDID
 */
typedef enum {
    BT_WLAN_CONN_PRECDENCE_WLAN=0,  /* Default */
    BT_WLAN_CONN_PRECDENCE_PAL,
} BT_WLAN_CONN_PRECEDENCE;

typedef PREPACK struct {
    A_UINT8 precedence;
} POSTPACK WMI_SET_BT_WLAN_CONN_PRECEDENCE;

/*
 * WMI_ENABLE_RM_CMDID
 */
typedef PREPACK struct {
        A_BOOL enable_radio_measurements;
} POSTPACK WMI_ENABLE_RM_CMD;

/*
 * WMI_SET_MAX_OFFHOME_DURATION_CMDID
 */
typedef PREPACK struct {
        A_UINT8 max_offhome_duration;
} POSTPACK WMI_SET_MAX_OFFHOME_DURATION_CMD;

typedef PREPACK struct {
    A_UINT32 frequency;
    A_UINT8  threshold;
} POSTPACK WMI_SET_HB_CHALLENGE_RESP_PARAMS_CMD;

typedef enum {
    WMI_PKTLOG_CMD_DISABLE,
    WMI_PKTLOG_CMD_SETSIZE,
} WMI_PKTLOG_CMD;

#define PKTLOG_MAX_BYTES 4096

typedef PREPACK struct {
    A_UINT32 nbytes;
    A_UINT8 buffer[PKTLOG_MAX_BYTES];
} POSTPACK WMI_GET_PKTLOG_CMD;

/* LPL commands */
typedef PREPACK struct  {
    A_UINT8 lplPolicy; /*0 - force off, 1 force on, 2 dynamic*/
    A_UINT8 noBlockerDetect; /*don't do blocker detection if lpl policy is set
                               to dynamic*/
    A_UINT8 noRfbDetect; /*don't do rate fall back  detection if lpl policy is set
                               to dynamic*/
    A_UINT8 rsvd;
} POSTPACK  WMI_LPL_FORCE_ENABLE_CMD;

/*command structure for all policy related commands*/
typedef PREPACK struct  {  
    A_UINT64    index;
    A_BOOL      value;
} POSTPACK  WMI_LPL_POLICY_CMD;

typedef PREPACK struct {
    A_UINT32    rfbPeriod;
    A_UINT32    rfbObsDuration;
    A_UINT32    blockerBeaconRssi;
    A_UINT8     chanOff;
    A_UINT32    rfbDiffThold;
    A_UINT32    bRssiThold;
    A_UINT32    maxBlockerRssi;
} POSTPACK WMI_LPL_PARAMS_CMD;

/* reverse credits commands */
typedef PREPACK struct {
    A_UINT8   max_queued_buffers;
    A_UINT8   endpoint_mapping[8];
    A_UINT8   start_credits_per_queue[8];
    A_UINT8   num_credit_queues;
} POSTPACK WMI_INIT_REVERSE_CREDIT_CMD;

typedef PREPACK struct  {
    A_UINT8 offset;
    A_UINT8 shift;
    A_UINT32 mask;
    A_UINT8  classCount;
    A_UINT32 category_mapping[8];
    A_UINT8  ep_mapping[8];
} POSTPACK WMI_INIT_RCV_DATA_CLASSIFIER_CMD;

typedef enum {
    WMI_PKTLOG_OPTION_LOG_TCP_HEADERS = 0x1,
    WMI_PKTLOG_OPTION_TRIGGER_THRUPUT = 0x2,
    WMI_PKTLOG_OPTION_TRIGGER_SACK    = 0x4,
    WMI_PKTLOG_OPTION_TRIGGER_PER     = 0x8,
    WMI_PKTLOG_OPTION_LOG_DIAGNOSTIC  = 0x20
} WMI_PKTLOG_OPTION;

typedef enum {
    WMI_PKTLOG_EVENT_RX  = 0x1,
    WMI_PKTLOG_EVENT_TX  = 0x2,
    WMI_PKTLOG_EVENT_RCF = 0x4, /* Rate Control Find */
    WMI_PKTLOG_EVENT_RCU = 0x8, /* Rate Control Update */
} WMI_PKTLOG_EVENT;

typedef PREPACK struct {
    WMI_PKTLOG_EVENT evlist;
    WMI_PKTLOG_OPTION option;
    A_UINT32 trigger_thresh;
    A_UINT32 trigger_interval;
    A_UINT32 trigger_tail_count;
    A_UINT32 buffer_size;
} POSTPACK WMI_ENABLE_PKTLOG_CMD;

typedef enum {
    WMI_SMPS_OPTION_MODE          = 0x1,
    WMI_SMPS_OPTION_AUTO          = 0x2,
    WMI_SMPS_OPTION_DATATHRESH    = 0x4,
    WMI_SMPS_OPTION_RSSITHRESH    = 0x8,
} WMI_SMPS_OPTION;

typedef enum {
    WMI_SMPS_MODE_STATIC    = 0x1,
    WMI_SMPS_MODE_DYNAMIC   = 0x2,
} WMI_SMPS_MODE;

typedef PREPACK struct {
    A_UINT8 flags;      /* To indicate which options have changed */
    A_UINT8 rssiThresh;
    A_UINT8 dataThresh;
    A_UINT8 mode;       /* static/dynamic */
    A_UINT8 automatic;
} POSTPACK WMI_CONFIG_SMPS_CMD;

typedef PREPACK struct {
    A_UINT8 enable;      /* Enable/disable */
} POSTPACK WMI_ENABLE_SMPS_CMD;

typedef PREPACK struct {
    A_UINT8     enable;
} POSTPACK WMI_SET_DFS_CMD;

typedef PREPACK struct {
    A_UINT32     log_time;
    A_UINT16     event_id;
    A_UINT16     unused;
} POSTPACK WMI_GET_PFM_CMD;

/*---------------------- BTCOEX RELATED -------------------------------------*/
/*----------------------COMMON to AR6002 and AR6003 -------------------------*/
typedef enum {
    BT_STREAM_UNDEF = 0,
    BT_STREAM_SCO,             /* SCO stream */
    BT_STREAM_A2DP,            /* A2DP stream */
    BT_STREAM_SCAN,            /* BT Discovery or Page */
    BT_STREAM_ESCO,
    BT_STREAM_HID,
    BT_STREAM_PAN,
    BT_STREAM_RFCOMM,
    BT_STREAM_LE,
    BT_STREAM_SDP,
    BT_STREAM_PAGESCAN,
    BT_STREAM_HIGH_CMD,
    BT_STREAM_MAX
} BT_STREAM_TYPE;

typedef enum {
    BT_PARAM_SCO_PSPOLL_LATENCY_ONE_FOURTH =1,
    BT_PARAM_SCO_PSPOLL_LATENCY_HALF,
    BT_PARAM_SCO_PSPOLL_LATENCY_THREE_FOURTH,
} BT_PARAMS_SCO_PSPOLL_LATENCY;

typedef enum {
    BT_PARAMS_SCO_STOMP_SCO_NEVER =1,
    BT_PARAMS_SCO_STOMP_SCO_ALWAYS,
    BT_PARAMS_SCO_STOMP_SCO_IN_LOWRSSI,
} BT_PARAMS_SCO_STOMP_RULES;

typedef enum {
    BT_STATUS_UNDEF = 0,
    BT_STATUS_ON,
    BT_STATUS_OFF,
    BT_STATUS_MAX
} BT_STREAM_STATUS;

typedef PREPACK struct {
    A_UINT8 streamType;
    A_UINT8 status;
} POSTPACK WMI_SET_BT_STATUS_CMD;

typedef enum {
    BT_ANT_TYPE_UNDEF=0,
    BT_ANT_TYPE_DUAL,
    BT_ANT_TYPE_SPLITTER,
    BT_ANT_TYPE_SWITCH,
    BT_ANT_TYPE_HIGH_ISO_DUAL
} BT_ANT_FRONTEND_CONFIG;

typedef enum {
    BT_COLOCATED_DEV_BTS4020=0,
    BT_COLCATED_DEV_CSR ,
    BT_COLOCATED_DEV_VALKYRIE
} BT_COLOCATED_DEV_TYPE;

/*********************** Applicable to AR6002 ONLY ******************************/

typedef enum {
    BT_PARAM_SCO = 1,         /* SCO stream parameters */
    BT_PARAM_A2DP ,
    BT_PARAM_ANTENNA_CONFIG,
    BT_PARAM_COLOCATED_BT_DEVICE,
    BT_PARAM_ACLCOEX,
    BT_PARAM_11A_SEPARATE_ANT,
    BT_PARAM_MAX
} BT_PARAM_TYPE;


#define BT_SCO_ALLOW_CLOSE_RANGE_OPT    (1 << 0)
#define BT_SCO_FORCE_AWAKE_OPT          (1 << 1)
#define BT_SCO_SET_RSSI_OVERRIDE(flags)        ((flags) |= (1 << 2))
#define BT_SCO_GET_RSSI_OVERRIDE(flags)        (((flags) >> 2) & 0x1)
#define BT_SCO_SET_RTS_OVERRIDE(flags)   ((flags) |= (1 << 3))
#define BT_SCO_GET_RTS_OVERRIDE(flags)   (((flags) >> 3) & 0x1)
#define BT_SCO_GET_MIN_LOW_RATE_CNT(flags)     (((flags) >> 8) & 0xFF)
#define BT_SCO_GET_MAX_LOW_RATE_CNT(flags)     (((flags) >> 16) & 0xFF)
#define BT_SCO_SET_MIN_LOW_RATE_CNT(flags,val) (flags) |= (((val) & 0xFF) << 8)
#define BT_SCO_SET_MAX_LOW_RATE_CNT(flags,val) (flags) |= (((val) & 0xFF) << 16)

typedef PREPACK struct {
    A_UINT32 numScoCyclesForceTrigger;  /* Number SCO cycles after which
                                           force a pspoll. default = 10 */
    A_UINT32 dataResponseTimeout;       /* Timeout Waiting for Downlink pkt
                                           in response for ps-poll,
                                           default = 10 msecs */
    A_UINT32  stompScoRules;
    A_UINT32 scoOptFlags;               /* SCO Options Flags :
                                            bits:     meaning:
                                             0        Allow Close Range Optimization
                                             1        Force awake during close range
                                             2        If set use host supplied RSSI for OPT
                                             3        If set use host supplied RTS COUNT for OPT
                                             4..7     Unused
                                             8..15    Low Data Rate Min Cnt
                                             16..23   Low Data Rate Max Cnt
                                        */

    A_UINT8 stompDutyCyleVal;           /* Sco cycles to limit ps-poll queuing
                                           if stomped */
    A_UINT8 stompDutyCyleMaxVal;        /*firm ware increases stomp duty cycle
                                          gradually uptill this value on need basis*/
    A_UINT8 psPollLatencyFraction;      /* Fraction of idle
                                           period, within which
                                           additional ps-polls
                                           can be queued */
    A_UINT8 noSCOSlots;                 /* Number of SCO Tx/Rx slots.
                                           HVx, EV3, 2EV3 = 2 */
    A_UINT8 noIdleSlots;                /* Number of Bluetooth idle slots between
                                           consecutive SCO Tx/Rx slots
                                           HVx, EV3 = 4
                                           2EV3 = 10 */
    A_UINT8 scoOptOffRssi;/*RSSI value below which we go to ps poll*/
    A_UINT8 scoOptOnRssi; /*RSSI value above which we reenter opt mode*/
    A_UINT8 scoOptRtsCount;
} POSTPACK BT_PARAMS_SCO;

#define BT_A2DP_ALLOW_CLOSE_RANGE_OPT  (1 << 0)
#define BT_A2DP_FORCE_AWAKE_OPT        (1 << 1)
#define BT_A2DP_SET_RSSI_OVERRIDE(flags)        ((flags) |= (1 << 2))
#define BT_A2DP_GET_RSSI_OVERRIDE(flags)        (((flags) >> 2) & 0x1)
#define BT_A2DP_SET_RTS_OVERRIDE(flags)   ((flags) |= (1 << 3))
#define BT_A2DP_GET_RTS_OVERRIDE(flags)   (((flags) >> 3) & 0x1)
#define BT_A2DP_GET_MIN_LOW_RATE_CNT(flags)     (((flags) >> 8) & 0xFF)
#define BT_A2DP_GET_MAX_LOW_RATE_CNT(flags)     (((flags) >> 16) & 0xFF)
#define BT_A2DP_SET_MIN_LOW_RATE_CNT(flags,val) (flags) |= (((val) & 0xFF) << 8)
#define BT_A2DP_SET_MAX_LOW_RATE_CNT(flags,val) (flags) |= (((val) & 0xFF) << 16)

typedef PREPACK struct {
    A_UINT32 a2dpWlanUsageLimit; /* MAX time firmware uses the medium for
                                    wlan, after it identifies the idle time
                                    default (30 msecs) */
    A_UINT32 a2dpBurstCntMin;   /* Minimum number of bluetooth data frames
                                   to replenish Wlan Usage  limit (default 3) */
    A_UINT32 a2dpDataRespTimeout;
    A_UINT32 a2dpOptFlags;      /* A2DP Option flags:
                                       bits:    meaning:
                                        0       Allow Close Range Optimization
                                        1       Force awake during close range
                                        2        If set use host supplied RSSI for OPT
                                        3        If set use host supplied RTS COUNT for OPT
                                        4..7    Unused
                                        8..15   Low Data Rate Min Cnt
                                        16..23  Low Data Rate Max Cnt
                                 */
    A_UINT8 isCoLocatedBtRoleMaster;
    A_UINT8 a2dpOptOffRssi;/*RSSI value below which we go to ps poll*/
    A_UINT8 a2dpOptOnRssi; /*RSSI value above which we reenter opt mode*/
    A_UINT8 a2dpOptRtsCount;
}POSTPACK BT_PARAMS_A2DP;

/* During BT ftp/ BT OPP or any another data based acl profile on bluetooth
   (non a2dp).*/
typedef PREPACK struct {
    A_UINT32 aclWlanMediumUsageTime;  /* Wlan usage time during Acl (non-a2dp)
                                       coexistence (default 30 msecs) */
    A_UINT32 aclBtMediumUsageTime;   /* Bt usage time during acl coexistence
                                       (default 30 msecs)*/
    A_UINT32 aclDataRespTimeout;
    A_UINT32 aclDetectTimeout;      /* ACL coexistence enabled if we get
                                       10 Pkts in X msec(default 100 msecs) */
    A_UINT32 aclmaxPktCnt;          /* No of ACL pkts to receive before
                                         enabling ACL coex */

}POSTPACK BT_PARAMS_ACLCOEX;

typedef PREPACK struct {
    PREPACK union {
        BT_PARAMS_SCO scoParams;
        BT_PARAMS_A2DP a2dpParams;
        BT_PARAMS_ACLCOEX  aclCoexParams;
        A_UINT8 antType;         /* 0 -Disabled (default)
                                     1 - BT_ANT_TYPE_DUAL
                                     2 - BT_ANT_TYPE_SPLITTER
                                     3 - BT_ANT_TYPE_SWITCH */
        A_UINT8 coLocatedBtDev;  /* 0 - BT_COLOCATED_DEV_BTS4020 (default)
                                     1 - BT_COLCATED_DEV_CSR
                                     2 - BT_COLOCATED_DEV_VALKYRIe
                                   */
    } POSTPACK info;
    A_UINT8 paramType ;
} POSTPACK WMI_SET_BT_PARAMS_CMD;

/************************ END AR6002 BTCOEX *******************************/
/*-----------------------AR6003 BTCOEX -----------------------------------*/

/*  ---------------WMI_SET_BTCOEX_FE_ANT_CMDID --------------------------*/
/* Indicates front end antenna configuration. This command needs to be issued
 * right after initialization and after WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMDID.
 * AR6003 enables coexistence and antenna switching based on the configuration.
 */
typedef enum {
    WMI_BTCOEX_NOT_ENABLED = 0,
    WMI_BTCOEX_FE_ANT_SINGLE =1,
    WMI_BTCOEX_FE_ANT_DUAL=2,
    WMI_BTCOEX_FE_ANT_DUAL_HIGH_ISO=3,
    WMI_BTCOEX_FE_ANT_DUAL_SH_BT_LOW_ISO = 4,
    WMI_BTCOEX_FE_ANT_DUAL_SH_BT_HIGH_ISO = 5,
    WMI_BTCOEX_FE_ANT_TRIPLE = 6,
    WMI_BTCOEX_FE_ANT_TYPE_MAX
}WMI_BTCOEX_FE_ANT_TYPE;

typedef PREPACK struct {
    A_UINT8 btcoexFeAntType; /* 1 - WMI_BTCOEX_FE_ANT_SINGLE for single antenna front end
                                2 - WMI_BTCOEX_FE_ANT_DUAL for dual antenna front end
                                    (for isolations less 35dB, for higher isolation there
                                    is not need to pass this command).
                                    (not implemented)
                              */
}POSTPACK WMI_SET_BTCOEX_FE_ANT_CMD;

/* -------------WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMDID ----------------*/
/* Indicate the bluetooth chip to the firmware. Firmware can have different algorithm based
 * bluetooth chip type.Based on bluetooth device, different coexistence protocol would be used.
 */
typedef PREPACK struct {
    A_UINT8 btcoexCoLocatedBTdev; /*1 - Qcom BT (3 -wire PTA)
                                    2 - CSR BT  (3 wire PTA)
                                    3 - Atheros 3001 BT (3 wire PTA)
                                    4 - STE bluetooth (4-wire ePTA)
                                    5 - Atheros 3002 BT (4-wire MCI)
                                    defaults= 3 (Atheros 3001 BT )
                                    */
}POSTPACK WMI_SET_BTCOEX_COLOCATED_BT_DEV_CMD;

/* -------------WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMDID ------------*/
/* Configuration parameters during bluetooth inquiry and page. Page configuration
 * is applicable only on interfaces which can distinguish page (applicable only for ePTA -
 * STE bluetooth).
 * Bluetooth inquiry start and end is indicated via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID.
 * During this the station will be  power-save mode.
 */
typedef PREPACK struct {
    A_UINT32 btInquiryDataFetchFrequency;/* The frequency of querying the AP for data
                                            (via pspoll) is configured by this parameter.
                                            "default = 10 ms" */

    A_UINT32 protectBmissDurPostBtInquiry;/* The firmware will continue to be in inquiry state
                                             for configured duration, after inquiry completion
                                             . This is to ensure other bluetooth transactions
                                             (RDP, SDP profiles, link key exchange ...etc)
                                             goes through smoothly without wifi stomping.
                                             default = 10 secs*/

    A_UINT32 maxpageStomp;                 /*Applicable only for STE-BT interface. Currently not
                                             used */
    A_UINT32 btInquiryPageFlag;           /* Not used */
}POSTPACK WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMD;

/*---------------------WMI_SET_BTCOEX_SCO_CONFIG_CMDID ---------------*/
/* Configure  SCO parameters. These parameters would be used whenever firmware is indicated
 * of (e)SCO profile on bluetooth ( via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID).
 * Configration of BTCOEX_SCO_CONFIG data structure are common configuration and applies
 * ps-poll mode and opt mode.
 * Ps-poll Mode - Station is in power-save and retrieves downlink data between sco gaps.
 * Opt Mode - station is in awake state and access point can send data to station any time.
 * BTCOEX_PSPOLLMODE_SCO_CONFIG - Configuration applied only during ps-poll mode.
 * BTCOEX_OPTMODE_SCO_CONFIG - Configuration applied only during opt mode.
 */
#define WMI_SCO_CONFIG_FLAG_ALLOW_OPTIMIZATION   (1 << 0)
#define WMI_SCO_CONFIG_FLAG_IS_EDR_CAPABLE       (1 << 1)
#define WMI_SCO_CONFIG_FLAG_IS_BT_MASTER         (1 << 2)
#define WMI_SCO_CONFIG_FLAG_FW_DETECT_OF_PER     (1 << 3)
#define WMI_SCO_CONFIG_FLAG_DIS_SCANCONN_STOMP   (1 << 4)

typedef PREPACK struct {
    A_UINT32 scoSlots;                  /* Number of SCO Tx/Rx slots.
                                           HVx, EV3, 2EV3 = 2 */
    A_UINT32 scoIdleSlots;              /* Number of Bluetooth idle slots between
                                           consecutive SCO Tx/Rx slots
                                           HVx, EV3 = 4
                                           2EV3 = 10
                                         */
    A_UINT32 scoFlags;                 /* SCO Options Flags :
                                          bits:    meaning:
                                          0   Allow Close Range Optimization
                                          1   Is EDR capable or Not
                                          2   IS Co-located Bt role Master
                                          3   Firmware determines the periodicity of SCO.
                                          4   No Stomping BT During WLAN Scan/Connection
                                         */

    A_UINT32 linkId;                      /* applicable to STE-BT - not used */
}POSTPACK BTCOEX_SCO_CONFIG;

typedef PREPACK struct {
    A_UINT32  scoCyclesForceTrigger;    /* Number SCO cycles after which
                                            force a pspoll. default = 10 */
    A_UINT32 scoDataResponseTimeout;     /* Timeout Waiting for Downlink pkt
                                            in response for ps-poll,
                                            default = 20 msecs */

    A_UINT32 scoStompDutyCyleVal;        /* not implemented */

    A_UINT32 scoStompDutyCyleMaxVal;     /*Not implemented */

    A_UINT32 scoPsPollLatencyFraction;   /* Fraction of idle
                                            period, within which
                                            additional ps-polls can be queued
                                            1 - 1/4 of idle duration
                                            2 - 1/2 of idle duration
                                            3 - 3/4 of idle duration
                                            default =2 (1/2)
                                           */
}POSTPACK BTCOEX_PSPOLLMODE_SCO_CONFIG;

typedef PREPACK struct {
    A_UINT32 scoStompCntIn100ms;/*max number of SCO stomp in 100ms allowed in
                                   opt mode. If exceeds the configured value,
                                   switch to ps-poll mode
                                  default = 3 */

    A_UINT32 scoContStompMax;   /* max number of continous stomp allowed in opt mode.
                                   if excedded switch to pspoll mode
                                    default = 3 */

    A_UINT32 scoMinlowRateMbps; /* Low rate threshold */

    A_UINT32 scoLowRateCnt;     /* number of low rate pkts (< scoMinlowRateMbps) allowed in 100 ms.
                                   If exceeded switch/stay to ps-poll mode, lower stay in opt mode.
                                   default = 36
                                 */

    A_UINT32 scoHighPktRatio;   /*(Total Rx pkts in 100 ms + 1)/
                                  ((Total tx pkts in 100 ms - No of high rate pkts in 100 ms) + 1) in 100 ms,
                                  if exceeded switch/stay in opt mode and if lower switch/stay in  pspoll mode.
                                  default = 5 (80% of high rates)
                                 */

    A_UINT32 scoMaxAggrSize;    /* Max number of Rx subframes allowed in this mode. (Firmware re-negogiates
                                   max number of aggregates if it was negogiated to higher value
                                   default = 1
                                   Recommended value Basic rate headsets = 1, EDR (2-EV3)  =4.
                                 */
    A_UINT32 NullBackoff;         /* Number of us the Null frame should go out before the next SCO slot */
}POSTPACK BTCOEX_OPTMODE_SCO_CONFIG;

typedef PREPACK struct {
    A_UINT32 scanInterval;
    A_UINT32 maxScanStompCnt;
}POSTPACK BTCOEX_WLANSCAN_SCO_CONFIG;

typedef PREPACK struct {
    BTCOEX_SCO_CONFIG scoConfig;
    BTCOEX_PSPOLLMODE_SCO_CONFIG scoPspollConfig;
    BTCOEX_OPTMODE_SCO_CONFIG scoOptModeConfig;
    BTCOEX_WLANSCAN_SCO_CONFIG scoWlanScanConfig;
}POSTPACK WMI_SET_BTCOEX_SCO_CONFIG_CMD;

/* ------------------WMI_SET_BTCOEX_A2DP_CONFIG_CMDID -------------------*/
/* Configure A2DP profile parameters. These parameters would be used whenver firmware is indicated
 * of A2DP profile on bluetooth ( via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID).
 * Configuration of BTCOEX_A2DP_CONFIG data structure are common configuration and applies to
 * ps-poll mode and opt mode.
 * Ps-poll Mode - Station is in power-save and retrieves downlink data between a2dp data bursts.
 * Opt Mode - station is in power save during a2dp bursts and awake in the gaps.
 * BTCOEX_PSPOLLMODE_A2DP_CONFIG - Configuration applied only during ps-poll mode.
 * BTCOEX_OPTMODE_A2DP_CONFIG - Configuration applied only during opt mode.
 */

#define WMI_A2DP_CONFIG_FLAG_ALLOW_OPTIMIZATION    (1 << 0)
#define WMI_A2DP_CONFIG_FLAG_IS_EDR_CAPABLE        (1 << 1)
#define WMI_A2DP_CONFIG_FLAG_IS_BT_ROLE_MASTER     (1 << 2)
#define WMI_A2DP_CONFIG_FLAG_IS_A2DP_HIGH_PRI      (1 << 3)
#define WMI_A2DP_CONFIG_FLAG_FIND_BT_ROLE          (1 << 4)
#define WMI_A2DP_CONFIG_FLAG_DIS_SCANCONN_STOMP    (1 << 5)

typedef PREPACK struct {
    A_UINT32 a2dpFlags;      /* A2DP Option flags:
                                bits:    meaning:
                                0       Allow Close Range Optimization
                                1       IS EDR capable
                                2       IS Co-located Bt role Master
                                3       a2dp traffic is high priority
                                4       Fw detect the role of bluetooth.
                                5       No Stomping BT during WLAN Scan/Connection
                             */
    A_UINT32 linkId;         /* Applicable only to STE-BT - not used */

}POSTPACK BTCOEX_A2DP_CONFIG;

typedef PREPACK struct {
    A_UINT32 a2dpWlanMaxDur; /* MAX time firmware uses the medium for
                                wlan, after it identifies the idle time
                                default (30 msecs) */

    A_UINT32 a2dpMinBurstCnt;   /* Minimum number of bluetooth data frames
                                to replenish Wlan Usage  limit (default 3) */

    A_UINT32 a2dpDataRespTimeout; /* Max duration firmware waits for downlink
                                     by stomping on  bluetooth
                                     after ps-poll is acknowledged.
                                     default = 20 ms
                                   */
}POSTPACK BTCOEX_PSPOLLMODE_A2DP_CONFIG;

typedef PREPACK struct {
    A_UINT32 a2dpMinlowRateMbps;  /* Low rate threshold */

    A_UINT32 a2dpLowRateCnt;    /* number of low rate pkts (< a2dpMinlowRateMbps) allowed in 100 ms.
                                   If exceeded switch/stay to ps-poll mode, lower stay in opt mode.
                                   default = 36
                                 */

    A_UINT32 a2dpHighPktRatio;   /*(Total Rx pkts in 100 ms + 1)/
                                  ((Total tx pkts in 100 ms - No of high rate pkts in 100 ms) + 1) in 100 ms,
                                  if exceeded switch/stay in opt mode and if lower switch/stay in  pspoll mode.
                                  default = 5 (80% of high rates)
                                 */

    A_UINT32 a2dpMaxAggrSize;    /* Max number of Rx subframes allowed in this mode. (Firmware re-negogiates
                                   max number of aggregates if it was negogiated to higher value
                                   default = 1
                                  Recommended value Basic rate headsets = 1, EDR (2-EV3)  =8.
                                 */
    A_UINT32 a2dpPktStompCnt;    /*number of a2dp pkts that can be stomped per burst.
                                   default = 6*/

}POSTPACK BTCOEX_OPTMODE_A2DP_CONFIG;

typedef PREPACK struct {
    BTCOEX_A2DP_CONFIG a2dpConfig;
    BTCOEX_PSPOLLMODE_A2DP_CONFIG a2dppspollConfig;
    BTCOEX_OPTMODE_A2DP_CONFIG a2dpOptConfig;
}POSTPACK WMI_SET_BTCOEX_A2DP_CONFIG_CMD;


/* ------------------WMI_SET_BTCOEX_HID_CONFIG_CMDID -------------------*/
/* Configure HID profile parameters. These parameters would be used whenver firmware is indicated
 * of HID profile on bluetooth ( via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID).
 * Configuration of BTCOEX_HID_CONFIG data structure are common configuration and applies to
  */

#define WMI_HID_CONFIG_FLAG_ALLOW_OPTIMIZATION    (1 << 0)
#define WMI_HID_CONFIG_FLAG_IS_EDR_CAPABLE        (1 << 1)
#define WMI_HID_CONFIG_FLAG_IS_BT_ROLE_MASTER     (1 << 2)
#define WMI_HID_CONFIG_FLAG_IS_A2DP_HIGH_PRI      (1 << 3)
#define WMI_HID_CONFIG_FLAG_DIS_SCANCONN_STOMP    (1 << 4)


typedef PREPACK struct {
    A_UINT32 hidFlags;      /* HID Option flags:
                                bits:    meaning:
                                0       Allow Close Range Optimization
                                4       No Stomping BT During WLAN Scan/Connection
                                                           */
    A_UINT32 hiddevices;         /*current   device number of hid */
    A_UINT32 maxStompSlot;       /* max stomped */
    A_UINT32 aclPktCntLowerLimit;       /*acl dectet when hid on */
    A_UINT32 dummy[2];           /**/

}POSTPACK BTCOEX_HID_CONFIG;

typedef PREPACK struct {
    A_UINT32 hidWlanMaxDur; /*not used */

    A_UINT32 hidMinBurstCnt;   /* not used */

    A_UINT32 hidDataRespTimeout; /* not used */
}POSTPACK BTCOEX_PSPOLLMODE_HID_CONFIG;

typedef PREPACK struct {
    A_UINT32 hidMinlowRateMbps;  /* not used  */

    A_UINT32 hidLowRateCnt;    /* not used */

    A_UINT32 hidHighPktRatio;   /*not used  */

    A_UINT32 hidMaxAggrSize;    /* not used  */
    A_UINT32 hidPktStompCnt;    /*not used */

}POSTPACK BTCOEX_OPTMODE_HID_CONFIG;

typedef PREPACK struct {
    BTCOEX_HID_CONFIG hidConfig;
    BTCOEX_PSPOLLMODE_HID_CONFIG hidpspollConfig;
    BTCOEX_OPTMODE_HID_CONFIG hidOptConfig;
}POSTPACK WMI_SET_BTCOEX_HID_CONFIG_CMD;


/*------------ WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMDID---------------------*/
/* Configure non-A2dp ACL profile parameters.The starts of ACL profile can either be
 * indicated via WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID orenabled via firmware detection
 *  which is configured via "aclCoexFlags".
 * Configration of BTCOEX_ACLCOEX_CONFIG data structure are common configuration and applies
 * ps-poll mode and opt mode.
 * Ps-poll Mode - Station is in power-save and retrieves downlink data during wlan medium.
 * Opt Mode - station is in power save during bluetooth medium time and awake during wlan duration.
 *             (Not implemented yet)
 *
 * BTCOEX_PSPOLLMODE_ACLCOEX_CONFIG - Configuration applied only during ps-poll mode.
 * BTCOEX_OPTMODE_ACLCOEX_CONFIG - Configuration applied only during opt mode.
 */

#define WMI_ACLCOEX_FLAGS_ALLOW_OPTIMIZATION   (1 << 0)
#define WMI_ACLCOEX_FLAGS_DISABLE_FW_DETECTION (1 << 1)
#define WMI_ACLCOEX_FLAGS_DIS_SCANCONN_STOMP   (1 << 2)

typedef PREPACK struct {
    A_UINT32 aclWlanMediumDur;      /* Wlan usage time during Acl (non-a2dp)
                                        coexistence (default 30 msecs)
                                    */

    A_UINT32 aclBtMediumDur;       /* Bt usage time during acl coexistence
                                         (default 30 msecs)
                                   */

    A_UINT32 aclDetectTimeout;     /* BT activity observation time limit.
                                      In this time duration, number of bt pkts are counted.
                                      If the Cnt reaches "aclPktCntLowerLimit" value
                                      for "aclIterToEnableCoex" iteration continuously,
                                      firmware gets into ACL coexistence mode.
                                      Similarly, if bt traffic count during ACL coexistence
                                      has not reached "aclPktCntLowerLimit" continuously
                                      for "aclIterToEnableCoex", then ACL coexistence is
                                      disabled.
                                      -default 100 msecs
                                    */

     A_UINT32 aclPktCntLowerLimit;   /* Acl Pkt Cnt to be received in duration of
                                        "aclDetectTimeout" for
                                        "aclIterForEnDis" times to enabling ACL coex.
                                        Similar logic is used to disable acl coexistence.
                                        (If "aclPktCntLowerLimit"  cnt of acl pkts
                                         are not seen by the for "aclIterForEnDis"
                                         then acl coexistence is disabled).
                                        default = 10
                                   */

     A_UINT32 aclIterForEnDis;      /* number of Iteration of "aclPktCntLowerLimit" for Enabling and
                                       Disabling Acl Coexistence.
                                       default = 3
                                     */

     A_UINT32 aclPktCntUpperLimit; /* This is upperBound limit, if there is more than
                                      "aclPktCntUpperLimit" seen in "aclDetectTimeout",
                                      ACL coexistence is enabled right away.
                                      - default 15*/

    A_UINT32 aclCoexFlags;          /* A2DP Option flags:
                                      bits:    meaning:
                                      0       Allow Close Range Optimization
                                      1       disable Firmware detection
                                      2       No Stomping BT During WLAN Scan/Connection
                                      (Currently supported configuration is aclCoexFlags =0)
                                    */

    A_UINT32 linkId;                /* Applicable only for STE-BT - not used */

}POSTPACK BTCOEX_ACLCOEX_CONFIG;

typedef PREPACK struct {
    A_UINT32 aclDataRespTimeout;   /* Max duration firmware waits for downlink
                                      by stomping on  bluetooth
                                      after ps-poll is acknowledged.
                                     default = 20 ms */

}POSTPACK BTCOEX_PSPOLLMODE_ACLCOEX_CONFIG;


/* Not implemented yet*/
typedef PREPACK struct {
    A_UINT32 aclCoexMinlowRateMbps;
    A_UINT32 aclCoexLowRateCnt;
    A_UINT32 aclCoexHighPktRatio;
    A_UINT32 aclCoexMaxAggrSize;
    A_UINT32 aclPktStompCnt;
}POSTPACK BTCOEX_OPTMODE_ACLCOEX_CONFIG;

typedef PREPACK struct {
    BTCOEX_ACLCOEX_CONFIG aclCoexConfig;
    BTCOEX_PSPOLLMODE_ACLCOEX_CONFIG aclCoexPspollConfig;
    BTCOEX_OPTMODE_ACLCOEX_CONFIG aclCoexOptConfig;
}POSTPACK WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMD;

/* -----------WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMDID ------------------*/
typedef enum {
    WMI_BTCOEX_BT_PROFILE_SCO =1,
    WMI_BTCOEX_BT_PROFILE_A2DP,
    WMI_BTCOEX_BT_PROFILE_INQUIRY_PAGE,
    WMI_BTCOEX_BT_PROFILE_ACLCOEX,
    WMI_BTCOEX_BT_PROFILE_HID,
}WMI_BTCOEX_BT_PROFILE;

typedef PREPACK struct {
    A_UINT32 btProfileType;
    A_UINT32 btOperatingStatus;
    A_UINT32 btLinkId;
}WMI_SET_BTCOEX_BT_OPERATING_STATUS_CMD;

/*--------------------- WMI_SET_BTCOEX_DEBUG_CMDID ---------------------*/
/* Used for firmware development and debugging */
typedef PREPACK struct {
    A_UINT32 btcoexDbgParam1;
    A_UINT32 btcoexDbgParam2;
    A_UINT32 btcoexDbgParam3;
    A_UINT32 btcoexDbgParam4;
    A_UINT32 btcoexDbgParam5;
}WMI_SET_BTCOEX_DEBUG_CMD;

/*---------------------WMI_GET_BTCOEX_CONFIG_CMDID --------------------- */
/* Command to firmware to get configuration parameters of the bt profile
 * reported via WMI_BTCOEX_CONFIG_EVENTID */
typedef PREPACK struct {
    A_UINT32 btProfileType; /* 1 - SCO
                               2 - A2DP
                               3 - INQUIRY_PAGE
                               4 - ACLCOEX
                            */
    A_UINT32 linkId;    /* not used */
}WMI_GET_BTCOEX_CONFIG_CMD;

/*------------------WMI_REPORT_BTCOEX_CONFIG_EVENTID------------------- */
/* Event from firmware to host, sent in response to WMI_GET_BTCOEX_CONFIG_CMDID
 * */
typedef PREPACK struct {
    A_UINT32 btProfileType;
    A_UINT32 linkId; /* not used */
    PREPACK union {
        WMI_SET_BTCOEX_SCO_CONFIG_CMD scoConfigCmd;
        WMI_SET_BTCOEX_A2DP_CONFIG_CMD a2dpConfigCmd;
        WMI_SET_BTCOEX_ACLCOEX_CONFIG_CMD aclcoexConfig;
        WMI_SET_BTCOEX_BTINQUIRY_PAGE_CONFIG_CMD btinquiryPageConfigCmd;
        WMI_SET_BTCOEX_HID_CONFIG_CMD hidConfigCmd;
    } POSTPACK info;
} POSTPACK WMI_BTCOEX_CONFIG_EVENT;

/*------------- WMI_REPORT_BTCOEX_BTCOEX_STATS_EVENTID--------------------*/
/* Used for firmware development and debugging*/
typedef PREPACK struct {
    A_UINT32 highRatePktCnt;
    A_UINT32 firstBmissCnt;
    A_UINT32 psPollFailureCnt;
    A_UINT32 nullFrameFailureCnt;
    A_UINT32 optModeTransitionCnt;
}BTCOEX_GENERAL_STATS;

typedef PREPACK struct {
    A_UINT32    scoStompCntAvg;
    A_UINT32    scoStompIn100ms;
    A_UINT32    scoMaxContStomp;
    A_UINT32    scoAvgNoRetries;
    A_UINT32    scoMaxNoRetriesIn100ms;
}BTCOEX_SCO_STATS;

typedef PREPACK struct {
    A_UINT32    a2dpBurstCnt;
    A_UINT32    a2dpMaxBurstCnt;
    A_UINT32    a2dpAvgIdletimeIn100ms;
    A_UINT32    a2dpAvgStompCnt;
}BTCOEX_A2DP_STATS;

typedef PREPACK struct {
    A_UINT32    aclPktCntInBtTime;
    A_UINT32    aclStompCntInWlanTime;
    A_UINT32    aclPktCntIn100ms;
}BTCOEX_ACLCOEX_STATS;

typedef PREPACK struct {
    BTCOEX_GENERAL_STATS coexStats;
    BTCOEX_SCO_STATS scoStats;
    BTCOEX_A2DP_STATS a2dpStats;
    BTCOEX_ACLCOEX_STATS aclCoexStats;
}WMI_BTCOEX_STATS_EVENT;

/*-----------------------WMI_SET_BTCOEX_SCHEME_CMDID---------------------------*/
typedef PREPACK struct {
    A_UINT32 allBtTimer;
    A_UINT8 stompNoneDutyCycle;
}BTCOEX_SCHEME_ALLBT_CONFIG;

typedef PREPACK struct {
    A_UINT8 btcScheme; /*see BTCOEX_SCHEME*/
    union{
        BTCOEX_SCHEME_ALLBT_CONFIG* allBtConfig;
    };
}WMI_SET_BTCOEX_SCHEME_CMD;

/*--------------------------END OF BTCOEX -------------------------------------*/

/* WAC commands 
 */


typedef PREPACK struct {
    A_UINT32    period;
    A_UINT32    threshold;
    A_INT32     rssi;
    A_BOOL      enable;
    A_CHAR      wps_pin[8];
}WMI_WAC_ENABLE_CMD;

typedef enum {
    WAC_MORE_SCAN = -1,
    WAC_SEND_PROBE_IDX = 0,
}WAC_SUBCMD;

typedef PREPACK struct {
    WAC_SUBCMD  cmdid;
}WMI_WAC_SCAN_REPLY_CMD;

typedef PREPACK struct {
    A_UINT8     req;
    A_UINT8     cmd;
    A_UINT8     frame;
    A_UINT8     ie[64];
    A_INT32     status;
}WMI_WAC_CTRL_REQ_CMD;

/* END OF WAC */

typedef PREPACK struct {
    A_UINT32 sleepState;
}WMI_REPORT_SLEEP_STATE_EVENT;

typedef enum {
    WMI_REPORT_SLEEP_STATUS_IS_DEEP_SLEEP =0,
    WMI_REPORT_SLEEP_STATUS_IS_AWAKE
} WMI_REPORT_SLEEP_STATUS;
typedef enum {
    DISCONN_EVT_IN_RECONN = 0,  /* default */
    NO_DISCONN_EVT_IN_RECONN
} TARGET_EVENT_REPORT_CONFIG;

typedef PREPACK struct {
    A_UINT32 evtConfig;
} POSTPACK WMI_SET_TARGET_EVENT_REPORT_CMD;


typedef PREPACK struct {
    A_UINT16    cmd_buf_sz;     /* HCI cmd buffer size */
    A_UINT8     buf[1];         /* Absolute HCI cmd */
} POSTPACK WMI_HCI_CMD;

/* WMI_LOG_FRAME_CMDID*/
typedef PREPACK struct {
    A_UINT8 request;                /* 1: start log */
} POSTPACK WMI_LOG_FRAME_CMD;       /* 0: stop log */

/*WMI_QUERY_PHY_INFO_CMDID*/
typedef PREPACK struct {
    A_UINT8  group_id; 		   /* 0 - Client Profile
						       1 - Operating Parameters
						       2 - Manufacturer's Information
						       3 - Client Capability */
    A_UINT8  info; 			   /*   0xFF : All information
					      		0-42 : dedicated IE*/						     
} POSTPACK WMI_QUERY_PHY_INFO_CMD;

/*
 * Command Replies
 */

/*
 * WMI_GET_CHANNEL_LIST_CMDID reply
 */
typedef PREPACK struct {
    A_UINT8     reserved1;
    A_UINT8     numChannels;            /* number of channels in reply */
    A_UINT16    channelList[1];         /* channel in Mhz */
} POSTPACK WMI_CHANNEL_LIST_REPLY;

typedef enum {
    A_SUCCEEDED = A_OK,
    A_FAILED_DELETE_STREAM_DOESNOT_EXIST=250,
    A_SUCCEEDED_MODIFY_STREAM=251,
    A_FAILED_INVALID_STREAM = 252,
    A_FAILED_MAX_THINSTREAMS = 253,
    A_FAILED_CREATE_REMOVE_PSTREAM_FIRST = 254,
} PSTREAM_REPLY_STATUS;

typedef PREPACK struct {
    A_UINT8     status;                 /* PSTREAM_REPLY_STATUS */
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficClass;
    A_UINT8     trafficDirection;       /* DIR_TYPE */
} POSTPACK WMI_CRE_PRIORITY_STREAM_REPLY;

typedef PREPACK struct {
    A_UINT8     status;                 /* PSTREAM_REPLY_STATUS */
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;       /* DIR_TYPE */
    A_UINT8     trafficClass;
} POSTPACK WMI_DEL_PRIORITY_STREAM_REPLY;

#ifdef TIME_BASED_DISCARD
/*
 * WMI_SET_TBD_TIME_CMDID
 */
typedef PREPACK struct{
	A_UINT32 discardDispostion:1;
  	A_UINT32 txdiscardTime:31;
} POSTPACK WMI_SET_TBD_TIME;
extern WMI_SET_TBD_TIME *txDiscard ;
#endif
typedef PREPACK struct {
    A_UINT32  tempDegree;
    A_UINT8   tempRegVal;
} POSTPACK WMI_GET_TEMPERATURE_REPLY;
/*
 * List of Events (target to host)
 */
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
    WMI_PARAM_SET_REPLY_EVENTID,            /*0x903F*/
    WMI_PARAM_GET_REPLY_EVENTID,            /*0x9040*/
    WMI_AP_GET_STA_INFO_REPLY_EVENTID,  	/*0x9041*/
    WMI_GET_HWANTDIVSTAT_EVENTID,			/*0x9042*/
} WMI_EVENT_ID;

typedef enum {
    WMI_11A_CAPABILITY   = 1,
    WMI_11G_CAPABILITY   = 2,
    WMI_11AG_CAPABILITY  = 3,
    WMI_11NA_CAPABILITY  = 4,
    WMI_11NG_CAPABILITY  = 5,
    WMI_11NAG_CAPABILITY = 6,
    // END CAPABILITY
    WMI_11N_CAPABILITY_OFFSET = (WMI_11NA_CAPABILITY - WMI_11A_CAPABILITY),
} WMI_PHY_CAPABILITY;

typedef PREPACK struct {
    A_UINT8     macaddr[ATH_MAC_LEN];
    A_UINT8     phyCapability;              /* WMI_PHY_CAPABILITY */
} POSTPACK WMI_READY_EVENT_1;

typedef PREPACK struct {
    A_UINT32    sw_version;
    A_UINT32    abi_version;
    A_UINT8     macaddr[ATH_MAC_LEN];
    A_UINT8     phyCapability;              /* WMI_PHY_CAPABILITY */
} POSTPACK WMI_READY_EVENT_2;

#if defined(ATH_TARGET)
#ifdef AR6002_REV2
#define WMI_READY_EVENT WMI_READY_EVENT_1  /* AR6002_REV2 target code */
#else
#define WMI_READY_EVENT WMI_READY_EVENT_2  /* AR6001, AR6002_REV4, AR6002_REV6 */
#endif
#else
#define WMI_READY_EVENT WMI_READY_EVENT_2 /* host code */
#endif

#define HOST_TX_SCHEDULER_ENABLED_MAGIC_NUM     0x5A

/*
 * Connect Event
 *
 * In STA mode networkType comes along with connected phy mode
 * To get networkType, WMI_NETWORK_TYPE (networkType) 
 * To get connected phymode, WMI_CONNECTED_PHYMODE(networkType)
 * will give the phymode value.
 */
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

/* ltecoex structure */
typedef enum _eCOEX_MODE{
    WLAN_COEX_MODE_DISABLED          = 0x0,
    WLAN_COEX_MODE_CHANNEL_AVOIDANCE = 0x1,
    WLAN_COEX_MODE_3WIRE             = 0x2,
    WLAN_COEX_MODE_PWR_BACKOFF       = 0x3,
    WLAN_COEX_MODE_NUM
}eCOEX_MODE;

typedef enum _eWWAN_STATE{
    WWAN_STATE_DEACTIVATED   = 0x0,
    WWAN_STATE_CONNECTED     = 0x1,
    WWAN_STATE_IDLE          = 0x2,
    WWAN_STATE_NUM
}eWWAN_STATE;

typedef enum _eWWAN_MODE{
    WWAN_MODE_INVALID    = 0x0,
    WWAN_MODE_TDD_CONFIG = 0x1,
    WWAN_MODE_FDD_CONFIG = 0x2,
    WWAN_MODE_NUM        = 0x2
}eWWAN_MODE;

typedef enum _eTDD_CONFIG{
    WWAN_TDD_CONFIG_INVALID = 0x0,
    WWAN_TDD_CONFIG_0       = 0x1,
    WWAN_TDD_CONFIG_1       = 0x2,
    WWAN_TDD_CONFIG_2       = 0x3,
    WWAN_TDD_CONFIG_3       = 0x4,
    WWAN_TDD_CONFIG_4       = 0x5,
    WWAN_TDD_CONFIG_5       = 0x6,
    WWAN_TDD_CONFIG_6       = 0x7,
    WWAN_TDD_CONFIG_NUM     = 0x7,
}eTDD_CONFIG;

typedef PREPACK struct _WMI_SET_LTE_COEX_STATE_CMD{
    A_UINT8    wwan_state;               /* One of enum eWWAN_STATE */
    A_UINT8    wwan_mode;                /* One of enum eWWAN_MODE  */
    A_UINT8    wwan_tdd_cfg;             /* One of enum eTDD_CONFIG */

    A_UINT8    sta_lte_coex_mode;        /* One of enum eCOEX_MODE */
    A_INT8     sta_max_tx_pwr;           /* Derived Value based on the LTE Freq & WLAN Freq for WLAN Power BackOff */

    A_UINT8    ap_lte_coex_mode;         /* One of enum eCOEX_MODE */
    A_INT8     ap_max_tx_pwr;            /* Derived Value based on the LTE Freq & WLAN Freq for WLAN Power BackOff */

    A_UINT8    reserved[1];              /* Reserved for Byte Alignment */

    A_UINT32   wwan_off_period;          /* Valid when wwan_state is WWAN_STATE_IDLE else '0' */
}POSTPACK WMI_SET_LTE_COEX_STATE_CMD;

typedef enum _LTE_COEX_WLAN_EVENT_NOTIFY{
    WLAN_SCAN_OPERATION       = 0x0,
    WLAN_CONNECTION_OPERATION = 0x1,
    WLAN_NUM_LTECOEX_EVENT
}LTE_COEX_WLAN_EVENT_NOTIFY;

typedef enum {
    COEX_WLAN_CONN_DISABLED = 0,
    COEX_WLAN_CONN_SETUP    = 1,
    COEX_WLAN_CONN_STEADY   = 2
}LTE_COEX_WLAN_CONN_STATE;

typedef PREPACK struct {
    A_UINT32    center_freq;
    A_UINT8     bandwidth;
    A_UINT8     scan_state;
} WMI_WLAN_SCAN_INFO_LTE;

typedef struct {
    A_UINT32 center_freq;
    A_UINT8 bandwidth;
    A_UINT8 conn_state;
    /* conn_state
     * 0x00 -- COEX_WLAN_CONN_DISABLED
     * 0x01 -- COEX_WLAN_CONN_SETUP
     * 0x02 -- COEX_WLAN_CONN_STEADY 
     * */
    A_UINT8 conn_mode;
    /* conn_mode
     * 0x00 -- COEX_WLAN_CONN_MODE_NONE
     * 0x01 -- COEX_WLAN_CONN_MODE_STATION
     * 0x02 -- COEX_WLAN_CONN_MODE_SOFTAP
     * 0x03-0x05 -- RESERVED.
     * */
}WMI_WLAN_CONN_INFO_LTE;

typedef PREPACK struct{
    A_UINT8 wlan_info_id;
    PREPACK union {
       WMI_WLAN_SCAN_INFO_LTE wlan_scan_info_lte;
       WMI_WLAN_CONN_INFO_LTE wlan_conn_info_lte;
    }POSTPACK;
}POSTPACK WMI_WLAN_INFO_LTE_EVENT;
/*ltecoex structure */

typedef struct {
    A_UINT8   status;
    A_UINT8   rspType;
} WMI_ASSOCREQ_EVENT;

typedef PREPACK struct {
    A_UINT8   host_accept;
    A_UINT8   host_reasonCode;
    A_UINT8   target_status;
    A_UINT8   sta_mac_addr[ATH_MAC_LEN];
    A_UINT8   rspType;
} POSTPACK WMI_SEND_ASSOCRES_CMD;

typedef struct {
    A_UINT8   enable;
} WMI_SET_ASSOCREQ_RELAY;

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
    A_UINT16    protocolReasonStatus;  /* reason code, see 802.11 spec. */
    A_UINT8     bssid[ATH_MAC_LEN];    /* set if known */
    A_UINT8     disconnectReason ;      /* see WMI_DISCONNECT_REASON */
    A_UINT8     assocRespLen;
    A_UINT8     assocInfo[1];
} POSTPACK WMI_DISCONNECT_EVENT;

/*
 * BSS Info Event.
 * Mechanism used to inform host of the presence and characteristic of
 * wireless networks present.  Consists of bss info header followed by
 * the beacon or probe-response frame body.  The 802.11 header is not included.
 */
typedef enum {
    BEACON_FTYPE = 0x1,
    PROBERESP_FTYPE,
    ACTION_MGMT_FTYPE,
    PROBEREQ_FTYPE,
} WMI_BI_FTYPE;

enum {
    BSS_ELEMID_CHANSWITCH = 0x01,
    BSS_ELEMID_ATHEROS = 0x02,
};

typedef PREPACK struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_BI_FTYPE */
    A_UINT8     snr;
    A_INT16     rssi;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT32    ieMask;
} POSTPACK WMI_BSS_INFO_HDR;

/*
 * BSS INFO HDR version 2.0
 * With 6 bytes HTC header and 6 bytes of WMI header
 * WMI_BSS_INFO_HDR cannot be accomodated in the removed 802.11 management
 * header space.
 * - Reduce the ieMask to 2 bytes as only two bit flags are used
 * - Remove rssi and compute it on the host. rssi = snr - 95
 */
typedef PREPACK struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_BI_FTYPE */
    A_UINT8     snr;
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT16    ieMask;
} POSTPACK WMI_BSS_INFO_HDR2;

typedef PREPACK struct {
    A_UINT8    bssid[ATH_MAC_LEN];
    A_UINT8     rssiCtl[3];
    A_UINT8     rssiExt[3];
}POSTPACK WMI_BSS_RSSI_INFO;

typedef PREPACK struct {
    A_UINT8    bssid[ATH_MAC_LEN];
    A_UINT8    pin[8];
} POSTPACK WMI_WPS_PIN_INFO;

typedef PREPACK struct {
    PREPACK union {
        A_UINT8 ie[17];
        A_INT32 wac_status;
    } POSTPACK info;
} POSTPACK WMI_GET_WAC_INFO;

#define IEEE80211_NWID_LEN                  32
typedef PREPACK struct {
    A_UINT8 bssid[ATH_MAC_LEN];
    A_UINT8 ssid[IEEE80211_NWID_LEN];
    A_UINT8 ssid_len;
    A_UINT8 channel;
    A_UINT8 snr;
    A_UINT8 id;
    A_UINT8 device_type;
    A_UINT8 antenna_type;
    A_INT8  threshold_offset1;  /* offset for AP Tx power */
    A_INT8  threshold_offset2;  /* offset for antenna gain */
} POSTPACK WMI_WAC_BSS_INFO_REPORT;

/*
 * Command Error Event
 */
typedef enum {
    INVALID_PARAM  = 0x01,
    ILLEGAL_STATE  = 0x02,
    INTERNAL_ERROR = 0x03,
    STACK_ERROR =    0x04,
    DFS_CHANNEL    = 0x05,
} WMI_ERROR_CODE;

typedef PREPACK struct {
    A_UINT16    commandId;
    A_UINT8     errorCode;
} POSTPACK WMI_CMD_ERROR_EVENT;

/*
 * New Regulatory Domain Event
 */
typedef PREPACK struct {
    A_UINT32    regDomain;
} POSTPACK WMI_REG_DOMAIN_EVENT;

typedef PREPACK struct {
    A_UINT8     txQueueNumber;
    A_UINT8     rxQueueNumber;
    A_UINT8     trafficDirection;
    A_UINT8     trafficClass;
} POSTPACK WMI_PSTREAM_TIMEOUT_EVENT;

/*
 * The WMI_NEIGHBOR_REPORT Event is generated by the target to inform
 * the host of BSS's it has found that matches the current profile.
 * It can be used by the host to cache PMKs and/to initiate pre-authentication
 * if the BSS supports it.  The first bssid is always the current associated
 * BSS.
 * The bssid and bssFlags information repeats according to the number
 * or APs reported.
 */
typedef enum {
    WMI_DEFAULT_BSS_FLAGS   = 0x00,
    WMI_PREAUTH_CAPABLE_BSS = 0x01,
    WMI_PMKID_VALID_BSS     = 0x02,
} WMI_BSS_FLAGS;

typedef PREPACK struct {
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT8     bssFlags;            /* see WMI_BSS_FLAGS */
} POSTPACK WMI_NEIGHBOR_INFO;

typedef PREPACK struct {
    A_INT8      numberOfAps;
    WMI_NEIGHBOR_INFO neighbor[1];
} POSTPACK WMI_NEIGHBOR_REPORT_EVENT;

/*
 * TKIP MIC Error Event
 */
typedef PREPACK struct {
    A_UINT8 keyid;
    A_UINT8 ismcast;
} POSTPACK WMI_TKIP_MICERR_EVENT;

/*
 * WMI_SCAN_COMPLETE_EVENTID - no parameters (old), staus parameter (new)
 */
typedef PREPACK struct {
    A_INT32 status;
} POSTPACK WMI_SCAN_COMPLETE_EVENT;


#define MAX_OPT_DATA_LEN 1400

/*
 * WMI_SET_ADHOC_BSSID_CMDID
 */
typedef PREPACK struct {
    A_UINT8     bssid[ATH_MAC_LEN];
} POSTPACK WMI_SET_ADHOC_BSSID_CMD;

/*
 * Special frame receive Event.
 * Mechanism used to inform host of the receiption of the special frames.
 * Consists of special frame info header followed by special frame body.
 * The 802.11 header is not included.
 */
typedef PREPACK struct {
    A_UINT16    channel;
    A_UINT8     frameType;          /* see WMI_OPT_FTYPE */
    A_INT8      snr;
    A_UINT8     srcAddr[ATH_MAC_LEN];
    A_UINT8     bssid[ATH_MAC_LEN];
} POSTPACK WMI_OPT_RX_INFO_HDR;

/*
 * Reporting statistics.
 */
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
 * WMI_RSSI_THRESHOLD_EVENTID.
 * Indicate the RSSI events to host. Events are indicated when we breach a
 * thresold value.
 */
typedef enum{
    WMI_RSSI_THRESHOLD1_ABOVE = 0,
    WMI_RSSI_THRESHOLD2_ABOVE,
    WMI_RSSI_THRESHOLD3_ABOVE,
    WMI_RSSI_THRESHOLD4_ABOVE,
    WMI_RSSI_THRESHOLD5_ABOVE,
    WMI_RSSI_THRESHOLD6_ABOVE,
    WMI_RSSI_THRESHOLD1_BELOW,
    WMI_RSSI_THRESHOLD2_BELOW,
    WMI_RSSI_THRESHOLD3_BELOW,
    WMI_RSSI_THRESHOLD4_BELOW,
    WMI_RSSI_THRESHOLD5_BELOW,
    WMI_RSSI_THRESHOLD6_BELOW
}WMI_RSSI_THRESHOLD_VAL;

typedef PREPACK struct {
    A_INT16 rssi;
    A_UINT8 range;
}POSTPACK WMI_RSSI_THRESHOLD_EVENT;

/*
 *  WMI_ERROR_REPORT_EVENTID
 */
typedef enum{
    WMI_TARGET_PM_ERR_FAIL      = 0x00000001,
    WMI_TARGET_KEY_NOT_FOUND    = 0x00000002,
    WMI_TARGET_DECRYPTION_ERR   = 0x00000004,
    WMI_TARGET_BMISS            = 0x00000008,
    WMI_PSDISABLE_NODE_JOIN     = 0x00000010,
    WMI_TARGET_COM_ERR          = 0x00000020,
    WMI_TARGET_FATAL_ERR        = 0x00000040,
    WMI_TARGET_BCN_FOUND        = 0x00000080,
    WMI_TARGET_P2P_PROBERSP_ERR = 0x00000100
} WMI_TARGET_ERROR_VAL;

typedef PREPACK struct {
    A_UINT32 errorVal;
}POSTPACK  WMI_TARGET_ERROR_REPORT_EVENT;

typedef PREPACK struct {
    A_UINT8 retrys;
}POSTPACK  WMI_TX_RETRY_ERR_EVENT;

typedef enum{
    WMI_SNR_THRESHOLD1_ABOVE = 1,
    WMI_SNR_THRESHOLD1_BELOW,
    WMI_SNR_THRESHOLD2_ABOVE,
    WMI_SNR_THRESHOLD2_BELOW,
    WMI_SNR_THRESHOLD3_ABOVE,
    WMI_SNR_THRESHOLD3_BELOW,
    WMI_SNR_THRESHOLD4_ABOVE,
    WMI_SNR_THRESHOLD4_BELOW
} WMI_SNR_THRESHOLD_VAL;

typedef PREPACK struct {
    A_UINT8 range;  /* WMI_SNR_THRESHOLD_VAL */
    A_UINT8 snr;
}POSTPACK  WMI_SNR_THRESHOLD_EVENT;

typedef enum{
    WMI_LQ_THRESHOLD1_ABOVE = 1,
    WMI_LQ_THRESHOLD1_BELOW,
    WMI_LQ_THRESHOLD2_ABOVE,
    WMI_LQ_THRESHOLD2_BELOW,
    WMI_LQ_THRESHOLD3_ABOVE,
    WMI_LQ_THRESHOLD3_BELOW,
    WMI_LQ_THRESHOLD4_ABOVE,
    WMI_LQ_THRESHOLD4_BELOW
} WMI_LQ_THRESHOLD_VAL;

typedef PREPACK struct {
    A_INT32 lq;
    A_UINT8 range;  /* WMI_LQ_THRESHOLD_VAL */
}POSTPACK  WMI_LQ_THRESHOLD_EVENT;
/*
 * WMI_REPORT_ROAM_TBL_EVENTID
 */
#define MAX_ROAM_TBL_CAND   5

typedef PREPACK struct {
    A_INT32 roam_util;
    A_UINT8 bssid[ATH_MAC_LEN];
    A_INT8  rssi;
    A_INT8  rssidt;
    A_INT8  last_rssi;
    A_INT8  util;
    A_INT8  bias;
    A_UINT8 reserved; /* For alignment */
} POSTPACK WMI_BSS_ROAM_INFO;


typedef PREPACK struct {
    A_UINT16  roamMode;
    A_UINT16  numEntries;
    WMI_BSS_ROAM_INFO bssRoamInfo[1];
} POSTPACK WMI_TARGET_ROAM_TBL;

/*
 * WMI_HCI_EVENT_EVENTID
 */
typedef PREPACK struct {
    A_UINT16    evt_buf_sz;     /* HCI event buffer size */
    A_UINT8     buf[1];         /* HCI  event */
} POSTPACK WMI_HCI_EVENT;

/*
 *  WMI_CAC_EVENTID
 */
typedef enum {
    CAC_INDICATION_ADMISSION = 0x00,
    CAC_INDICATION_ADMISSION_RESP = 0x01,
    CAC_INDICATION_DELETE = 0x02,
    CAC_INDICATION_NO_RESP = 0x03,
}CAC_INDICATION;

#define WMM_TSPEC_IE_LEN   63

typedef PREPACK struct {
    A_UINT8 ac;
    A_UINT8 cac_indication;
    A_UINT8 statusCode;
    A_UINT8 tspecSuggestion[WMM_TSPEC_IE_LEN];
}POSTPACK  WMI_CAC_EVENT;

/*
 * WMI_APLIST_EVENTID
 */

typedef enum {
    APLIST_VER1 = 1,
} APLIST_VER;

typedef PREPACK struct {
    A_UINT8     bssid[ATH_MAC_LEN];
    A_UINT16    channel;
} POSTPACK  WMI_AP_INFO_V1;

typedef PREPACK union {
    WMI_AP_INFO_V1  apInfoV1;
} POSTPACK WMI_AP_INFO;

typedef PREPACK struct {
    A_UINT8     apListVer;
    A_UINT8     numAP;
    WMI_AP_INFO apList[1];
} POSTPACK WMI_APLIST_EVENT;

typedef PREPACK struct {
    A_UINT64   tsf;
    A_UINT16   frameSize;
    A_UINT8    frameData[1];
} POSTPACK WMI_LOG_FRAME_EVENT;

/*
 * developer commands
 */

/*
 * WMI_SET_BITRATE_CMDID
 *
 * Get bit rate cmd uses same definition as set bit rate cmd
 *
 */
typedef enum {
    RATE_AUTO   = -1,
     AR6003_RATE_1Mb       = 0,
     AR6003_RATE_2Mb       = 1,
     AR6003_RATE_5_5Mb     = 2,
     AR6003_RATE_11Mb      = 3,
     AR6003_RATE_6Mb       = 4,
     AR6003_RATE_9Mb       = 5,
     AR6003_RATE_12Mb      = 6,
     AR6003_RATE_18Mb      = 7,
     AR6003_RATE_24Mb      = 8,
     AR6003_RATE_36Mb      = 9,
     AR6003_RATE_48Mb      = 10,
     AR6003_RATE_54Mb      = 11,
     AR6003_RATE_MCS_0_20  = 12,
     AR6003_RATE_MCS_1_20  = 13,
     AR6003_RATE_MCS_2_20  = 14,
     AR6003_RATE_MCS_3_20  = 15,
     AR6003_RATE_MCS_4_20  = 16,
     AR6003_RATE_MCS_5_20  = 17,
     AR6003_RATE_MCS_6_20  = 18,
     AR6003_RATE_MCS_7_20  = 19,
     AR6003_RATE_MCS_0_40  = 20,
     AR6003_RATE_MCS_1_40  = 21,
     AR6003_RATE_MCS_2_40  = 22,
     AR6003_RATE_MCS_3_40  = 23,
     AR6003_RATE_MCS_4_40  = 24,
     AR6003_RATE_MCS_5_40  = 25,
     AR6003_RATE_MCS_6_40  = 26,
     AR6003_RATE_MCS_7_40  = 27,
     AR6003_RATE_MAX,

     AR6004_RATE_1Mb       = 0,
     AR6004_RATE_2Mb       = 1,
     AR6004_RATE_5_5Mb     = 2,
     AR6004_RATE_11Mb      = 3,
     AR6004_RATE_6Mb       = 4,
     AR6004_RATE_9Mb       = 5,
     AR6004_RATE_12Mb      = 6,
     AR6004_RATE_18Mb      = 7,
     AR6004_RATE_24Mb      = 8,
     AR6004_RATE_36Mb      = 9,
     AR6004_RATE_48Mb      = 10,
     AR6004_RATE_54Mb      = 11,
     AR6004_RATE_MCS_0_20  = 12,
     AR6004_RATE_MCS_1_20  = 13,
     AR6004_RATE_MCS_2_20  = 14,
     AR6004_RATE_MCS_3_20  = 15,
     AR6004_RATE_MCS_4_20  = 16,
     AR6004_RATE_MCS_5_20  = 17,
     AR6004_RATE_MCS_6_20  = 18,
     AR6004_RATE_MCS_7_20  = 19,
     AR6004_RATE_MCS_0_40  = 20,
     AR6004_RATE_MCS_1_40  = 21,
     AR6004_RATE_MCS_2_40  = 22,
     AR6004_RATE_MCS_3_40  = 23,
     AR6004_RATE_MCS_4_40  = 24,
     AR6004_RATE_MCS_5_40  = 25,
     AR6004_RATE_MCS_6_40  = 26,
     AR6004_RATE_MCS_7_40  = 27,
     AR6004_RATE_MAX,
} WMI_BIT_RATE;

typedef PREPACK struct {
    A_INT8      rateIndex;          /* see WMI_BIT_RATE */
    A_INT8      mgmtRateIndex;
    A_INT8      ctlRateIndex;
} POSTPACK WMI_BIT_RATE_CMD;


typedef PREPACK struct {
    A_INT8      rateIndex;          /* see WMI_BIT_RATE */
} POSTPACK  WMI_BIT_RATE_REPLY;

typedef PREPACK struct {
    A_UINT32    time_val_sec;
} POSTPACK  WMI_AP_IDLE_CLOSE_TIME_CMD;
/*
 * WMI_SET_FIXRATES_CMDID
 *
 * Get fix rates cmd uses same definition as set fix rates cmd
 * Fix rate mask is now 64 bit, which is split across two words.
 * Refer WMI_BIT_RATE for bit position for each rate.
 */
#define FIX_RATE_1Mb            ((A_UINT32)0x1)
#define FIX_RATE_2Mb            ((A_UINT32)0x2)
#define FIX_RATE_5_5Mb          ((A_UINT32)0x4)
#define FIX_RATE_11Mb           ((A_UINT32)0x8)
#define FIX_RATE_6Mb            ((A_UINT32)0x10)
#define FIX_RATE_9Mb            ((A_UINT32)0x20)
#define FIX_RATE_12Mb           ((A_UINT32)0x40)
#define FIX_RATE_18Mb           ((A_UINT32)0x80)
#define FIX_RATE_24Mb           ((A_UINT32)0x100)
#define FIX_RATE_36Mb           ((A_UINT32)0x200)
#define FIX_RATE_48Mb           ((A_UINT32)0x400)
#define FIX_RATE_54Mb           ((A_UINT32)0x800)
#define FIX_RATE_MCS_0_20       ((A_UINT32)0x1000)
#define FIX_RATE_MCS_1_20       ((A_UINT32)0x2000)
#define FIX_RATE_MCS_2_20       ((A_UINT32)0x4000)
#define FIX_RATE_MCS_3_20       ((A_UINT32)0x8000)
#define FIX_RATE_MCS_4_20       ((A_UINT32)0x10000)
#define FIX_RATE_MCS_5_20       ((A_UINT32)0x20000)
#define FIX_RATE_MCS_6_20       ((A_UINT32)0x40000)
#define FIX_RATE_MCS_7_20       ((A_UINT32)0x80000)
#define FIX_RATE_MCS_0_40       ((A_UINT32)0x100000)
#define FIX_RATE_MCS_1_40       ((A_UINT32)0x200000)
#define FIX_RATE_MCS_2_40       ((A_UINT32)0x400000)
#define FIX_RATE_MCS_3_40       ((A_UINT32)0x800000)
#define FIX_RATE_MCS_4_40       ((A_UINT32)0x1000000)
#define FIX_RATE_MCS_5_40       ((A_UINT32)0x2000000)
#define FIX_RATE_MCS_6_40       ((A_UINT32)0x4000000)
#define FIX_RATE_MCS_7_40       ((A_UINT32)0x8000000)

typedef PREPACK struct {
    A_UINT32      fixRateMask[WMI_MAX_RATE_MASK];          /* see WMI_BIT_RATE */
} POSTPACK WMI_FIX_RATES_CMD, WMI_FIX_RATES_REPLY;

typedef PREPACK struct {
    A_UINT8      bEnableMask;
    A_UINT8      frameType;              /* type and subtype */
    A_UINT8      reserved[2];            /* for alignment    */
    A_UINT32     frameRateMask[WMI_MAX_RATE_MASK];          /* see WMI_BIT_RATE */
} POSTPACK WMI_FRAME_RATES_CMD, WMI_FRAME_RATES_REPLY;

/*
 * WMI_SET_RECONNECT_AUTH_MODE_CMDID
 *
 * Set authentication mode
 */
typedef enum {
    RECONN_DO_AUTH = 0x00,
    RECONN_NOT_AUTH = 0x01
} WMI_AUTH_MODE;

typedef PREPACK struct {
    A_UINT8 mode;
} POSTPACK WMI_SET_AUTH_MODE_CMD;

/*
 * WMI_SET_REASSOC_MODE_CMDID
 *
 * Set authentication mode
 */
typedef enum {
    REASSOC_DO_DISASSOC = 0x00,
    REASSOC_DONOT_DISASSOC = 0x01
} WMI_REASSOC_MODE;

typedef PREPACK struct {
    A_UINT8 mode;
}POSTPACK WMI_SET_REASSOC_MODE_CMD;

typedef enum {
    ROAM_DATA_TIME = 1,            /* Get The Roam Time Data */
} ROAM_DATA_TYPE;

typedef PREPACK struct {
    A_UINT32        disassoc_time;
    A_UINT32        no_txrx_time;
    A_UINT32        assoc_time;
    A_UINT32        allow_txrx_time;
    A_UINT8         disassoc_bssid[ATH_MAC_LEN];
    A_INT8          disassoc_bss_rssi;
    A_UINT8         assoc_bssid[ATH_MAC_LEN];
    A_INT8          assoc_bss_rssi;
} POSTPACK WMI_TARGET_ROAM_TIME;

typedef PREPACK struct {
    PREPACK union {
        WMI_TARGET_ROAM_TIME roamTime;
    } POSTPACK u;
    A_UINT8 roamDataType ;
} POSTPACK WMI_TARGET_ROAM_DATA;

typedef enum {
    WMI_WMM_DISABLED = 0,
    WMI_WMM_ENABLED
} WMI_WMM_STATUS;

typedef PREPACK struct {
    A_UINT8    status;
}POSTPACK WMI_SET_WMM_CMD;

typedef PREPACK struct {
    A_UINT8    status;
}POSTPACK WMI_SET_QOS_SUPP_CMD;

typedef enum {
    WMI_TXOP_DISABLED = 0,
    WMI_TXOP_ENABLED
} WMI_TXOP_CFG;

typedef PREPACK struct {
    A_UINT8    txopEnable;
}POSTPACK WMI_SET_WMM_TXOP_CMD;

typedef PREPACK struct {
    A_UINT8 keepaliveInterval;
} POSTPACK WMI_SET_KEEPALIVE_CMD;

typedef PREPACK struct {
    A_BOOL configured;
    A_UINT8 keepaliveInterval;
} POSTPACK WMI_GET_KEEPALIVE_CMD;

/*
 * Add Application specified IE to a management frame
 */
#define WMI_MAX_IE_LEN  255
#define WMI_MAX_LARGE_IE_LEN  510

typedef PREPACK struct {
    A_UINT8 mgmtFrmType;  /* one of WMI_MGMT_FRAME_TYPE */
    A_UINT8 ieLen;    /* Length  of the IE that should be added to the MGMT frame */
    A_UINT8 ieInfo[1];
} POSTPACK WMI_SET_APPIE_CMD;

typedef PREPACK struct {
    A_UINT8 ieId;
    A_UINT8 ieField;    /* enum WMI_IE_FIELD_TYPE */ 
    A_UINT8 ieLen;
    A_UINT8 reserved;
    A_UINT8 ieInfo[0];
} POSTPACK WMI_SET_IE_CMD;

typedef PREPACK struct {
    A_UINT8 mgmtFrmType;  /* one of WMI_MGMT_FRAME_LARGE_TYPE */
    A_UINT16 ieLen;    /* Length  of the Large IE that should be added to the MGMT frame */
    A_UINT8 ieInfo[1];
} POSTPACK WMI_SET_APPIE_LARGE_CMD;

/*
 * Notify the WSC registration status to the target
 */
#define WSC_REG_ACTIVE     1
#define WSC_REG_INACTIVE   0
/* Generic Hal Interface for setting hal paramters. */
/* Add new Set HAL Param cmdIds here for newer params */
typedef enum {
   WHAL_SETCABTO_CMDID = 1,
}WHAL_CMDID;

typedef PREPACK struct {
    A_UINT8 cabTimeOut;
} POSTPACK WHAL_SETCABTO_PARAM;

typedef PREPACK struct {
    A_UINT8  whalCmdId;
    A_UINT8 data[1];
} POSTPACK WHAL_PARAMCMD;


#define WOW_MAX_FILTER_LISTS 1 /*4*/
#define WOW_MAX_FILTERS_PER_LIST 4
#define WOW_PATTERN_SIZE 64
#define WOW_MASK_SIZE 64

#define MAC_MAX_FILTERS_PER_LIST 7

typedef PREPACK struct {
    A_UINT8 wow_valid_filter;
    A_UINT8 wow_filter_id;
    A_UINT8 wow_filter_size;
    A_UINT8 wow_filter_offset;
    A_UINT8 wow_filter_mask[WOW_MASK_SIZE];
    A_UINT8 wow_filter_pattern[WOW_PATTERN_SIZE];
} POSTPACK WOW_FILTER;


typedef PREPACK struct {
    A_UINT8 wow_valid_list;
    A_UINT8 wow_list_id;
    A_UINT8 wow_num_filters;
    A_UINT8 wow_total_list_size;
    WOW_FILTER list[WOW_MAX_FILTERS_PER_LIST];
} POSTPACK WOW_FILTER_LIST;

typedef PREPACK struct {
    A_UINT8 valid_filter;
    A_UINT8 mac_addr[ATH_MAC_LEN];
} POSTPACK MAC_FILTER;


typedef PREPACK struct {
    A_UINT8 total_list_size;
    A_UINT8 enable;
    MAC_FILTER list[MAC_MAX_FILTERS_PER_LIST];
} POSTPACK MAC_FILTER_LIST;

#define MAX_IP_ADDRS  2
typedef PREPACK struct {
    A_UINT32 ips[MAX_IP_ADDRS];  /* IP in Network Byte Order */
} POSTPACK WMI_SET_IP_CMD;

typedef PREPACK struct {
    A_BOOL awake;
    A_BOOL asleep;
} POSTPACK WMI_SET_HOST_SLEEP_MODE_CMD;

typedef A_UINT32 WMI_WOW_FILTER;

    /* bit positions for filter options */
#define  WOW_FILTER_UNUSED          0
#define  WOW_FILTER_SSID            1   /* wake up on probed SSID (legacy definition) */
    /* the following filter options are available if the WOW extensions are installed in the target
     * otherwise the feature setting has no effect  */

#define WOW_FILTER_OPTION_MASK(option)  (1 << (option))
#define WOW_FILTER_OPTION_MAGIC_PACKET  WOW_FILTER_OPTION_MASK(2)
#define WOW_FILTER_OPTION_EAP_REQ       WOW_FILTER_OPTION_MASK(3)
#define WOW_FILTER_OPTION_PATTERNS      WOW_FILTER_OPTION_MASK(4)
#define WOW_FILTER_OPTION_OFFLOAD_ARP   WOW_FILTER_OPTION_MASK(5)
#define WOW_FILTER_OPTION_OFFLOAD_NS    WOW_FILTER_OPTION_MASK(6)
#define WOW_FILTER_OPTION_OFFLOAD_GTK   WOW_FILTER_OPTION_MASK(7)
#define WOW_FILTER_OPTION_8021X_4WAYHS  WOW_FILTER_OPTION_MASK(8)
#define WOW_FILTER_OPTION_NLO_DISCVRY   WOW_FILTER_OPTION_MASK(9)
#define WOW_FILTER_OPTION_NWK_DISASSOC  WOW_FILTER_OPTION_MASK(10)
#define WOW_FILTER_OPTION_GTK_ERROR     WOW_FILTER_OPTION_MASK(11)
#define WOW_FILTER_OPTION_IPv4_TCP_SYN	WOW_FILTER_OPTION_MASK(12)
#define WOW_FILTER_OPTION_IPv6_TCP_SYN	WOW_FILTER_OPTION_MASK(13)

#define WOW_FILTER_OPTION_TEST_MODE     WOW_FILTER_OPTION_MASK(15)


typedef PREPACK struct {
    A_BOOL enable_wow;
    A_UINT32 filter;
    A_UINT16 hostReqDelay;
} POSTPACK WMI_SET_WOW_MODE_CMD;

typedef PREPACK struct {
    A_UINT8 filter_list_id;
} POSTPACK WMI_GET_WOW_LIST_CMD;

typedef PREPACK struct {
	A_UINT32	gpio;				
	A_BOOL		isActiveLow;
	A_UINT32	triggerMechanism;	/*0: edge triggered 1: level triggered*/
}POSTPACK WMI_CONFIG_GPIO_CMD;

/*
 * WMI_GET_WOW_LIST_CMD reply
 */
typedef PREPACK struct {
    A_UINT8     num_filters;     /* number of patterns in reply */
    A_UINT8     this_filter_num; /*  this is filter # x of total num_filters */
    A_UINT8     wow_mode;
    A_UINT8     host_mode;
    WOW_FILTER  wow_filters[1];
} POSTPACK WMI_GET_WOW_LIST_REPLY;

typedef PREPACK struct {
    A_UINT32	pattern_size;
	A_UINT32	offset;
	A_UINT32	pattern_index;
	A_UINT8		pattern[WOW_PATTERN_SIZE];
} POSTPACK WMI_ADD_WOW_PATTERN_CMD;

typedef PREPACK struct {
    A_UINT16 filter_list_id;
    A_UINT16 filter_id;
} POSTPACK WMI_DEL_WOW_PATTERN_CMD;

typedef PREPACK struct {
    A_UINT8 macaddr[ATH_MAC_LEN];
} POSTPACK WMI_SET_MAC_ADDRESS_CMD;

/*
 * WMI_SET_AKMP_PARAMS_CMD
 */

#define WMI_AKMP_MULTI_PMKID_EN   0x000001

typedef PREPACK struct {
    A_UINT32    akmpInfo;
} POSTPACK WMI_SET_AKMP_PARAMS_CMD;

typedef PREPACK struct {
    A_UINT8 pmkid[WMI_PMKID_LEN];
} POSTPACK WMI_PMKID;

/*
 * WMI_SET_PMKID_LIST_CMD
 */
#define WMI_MAX_PMKID_CACHE   8

typedef PREPACK struct {
    A_UINT32    numPMKID;
    WMI_PMKID   pmkidList[WMI_MAX_PMKID_CACHE];
} POSTPACK WMI_SET_PMKID_LIST_CMD;

/*
 * WMI_GET_PMKID_LIST_CMD  Reply
 * Following the Number of PMKIDs is the list of PMKIDs
 */
typedef PREPACK struct {
    A_UINT32    numPMKID;
    A_UINT8     bssidList[ATH_MAC_LEN][1];
    WMI_PMKID   pmkidList[1];
} POSTPACK WMI_PMKID_LIST_REPLY;

typedef PREPACK struct {
    A_UINT16 oldChannel;
    A_UINT32 newChannel;
} POSTPACK WMI_CHANNEL_CHANGE_EVENT;

typedef PREPACK struct {
    A_UINT32 version;
} POSTPACK WMI_WLAN_VERSION_EVENT;


/* WMI_ADDBA_REQ_EVENTID */
typedef PREPACK struct {
    A_UINT8     tid;
    A_UINT8     win_sz;
    A_UINT16    st_seq_no;
    A_UINT8     status;         /* f/w response for ADDBA Req; OK(0) or failure(!=0) */
} POSTPACK WMI_ADDBA_REQ_EVENT;

/* WMI_ADDBA_RESP_EVENTID */
typedef PREPACK struct {
    A_UINT8     tid;
    A_UINT8     status;         /* OK(0), failure (!=0) */
    A_UINT16    amsdu_sz;       /* Three values: Not supported(0), 3839, 8k */
} POSTPACK WMI_ADDBA_RESP_EVENT;

/* WMI_DELBA_EVENTID
 * f/w received a DELBA for peer and processed it.
 * Host is notified of this
 */
typedef PREPACK struct {
    A_UINT8     tid;
    A_UINT8     is_peer_initiator;
    A_UINT16    reason_code;
} POSTPACK WMI_DELBA_EVENT;


#ifdef WAPI_ENABLE
#define WAPI_REKEY_UCAST    1
#define WAPI_REKEY_MCAST    2
typedef PREPACK struct {
    A_UINT8     type;
    A_UINT8     macAddr[ATH_MAC_LEN];
} POSTPACK WMI_WAPIREKEY_EVENT;
#endif


/* WMI_ALLOW_AGGR_CMDID
 * Configures tid's to allow ADDBA negotiations
 * on each tid, in each direction
 */
typedef PREPACK struct {
    A_UINT16    tx_allow_aggr;  /* 16-bit mask to allow uplink ADDBA negotiation - bit position indicates tid*/
    A_UINT16    rx_allow_aggr;  /* 16-bit mask to allow donwlink ADDBA negotiation - bit position indicates tid*/
} POSTPACK WMI_ALLOW_AGGR_CMD;

/* WMI_ADDBA_REQ_CMDID
 * f/w starts performing ADDBA negotiations with peer
 * on the given tid
 */
typedef PREPACK struct {
    A_UINT8     tid;
} POSTPACK WMI_ADDBA_REQ_CMD;

/* WMI_DELBA_REQ_CMDID
 * f/w would teardown BA with peer.
 * is_send_initiator indicates if it's or tx or rx side
 */
typedef PREPACK struct {
    A_UINT8     tid;
    A_UINT8     is_sender_initiator;

} POSTPACK WMI_DELBA_REQ_CMD;

#define PEER_NODE_JOIN_EVENT 0x00
#define PEER_NODE_LEAVE_EVENT 0x01
#define PEER_FIRST_NODE_JOIN_EVENT 0x10
#define PEER_LAST_NODE_LEAVE_EVENT 0x11
typedef PREPACK struct {
    A_UINT8 eventCode;
    A_UINT8 peerMacAddr[ATH_MAC_LEN];
} POSTPACK WMI_PEER_NODE_EVENT;

typedef PREPACK struct {
    A_UINT8 eventCode;
    A_UINT8 peerMacAddr[ATH_MAC_LEN];
    A_UINT8 aid;
} POSTPACK WMI_PEER_NODE_EVENT_EXT;

#define IEEE80211_FRAME_TYPE_MGT          0x00
#define IEEE80211_FRAME_TYPE_CTL          0x04

typedef PREPACK struct {
    A_UINT32    rules; /* combination of WMI_WRT_... values */
} POSTPACK WMI_CONFIG_TX_MAC_RULES_CMD;

typedef PREPACK struct {
    A_UINT8 enable;     /* 1 == device operates in promiscuous mode , 0 == normal mode <default> */
} POSTPACK WMI_SET_PROMISCUOUS_MODE_CMD;

typedef enum {
    RADIO_STATE_OFF     = 0x1,
    RADIO_STATE_ON      = 0x2,
    RADIO_STATE_INVALID = 0xFF
}RFKILL_RADIO_STATE;

typedef PREPACK struct {
    A_UINT8  GPIOPinNumber;
    A_UINT8  IntrType;
    A_UINT8  RadioState;
} POSTPACK WMI_RFKILL_MODE_CMD;

typedef PREPACK struct {
    A_UINT32 FastScanInterval;
    A_UINT32 FastScanDuration;
    A_UINT32 SlowScanInterval;
    A_UINT8  ScanType; //ActiveUseSSID=0, ActiveUseWildCard=1, PassiveScan=2
    A_UINT8  bStartNetworkListOffload; //TRUE - Use the Probed SSID
}POSTPACK WMI_NETWORK_LIST_OFFLOAD_CMD;

typedef PREPACK struct {
   A_UINT8   bProfileMatch;
   A_UINT8   Index;
   A_UINT32  nFastScansRemaining;
}POSTPACK WMI_NETWORK_LIST_OFFLOAD_INFO;

typedef PREPACK struct _WMI_IPV6_ADDR {
    A_UINT8    address[16];    /* IPV6 in Network Byte Order */
} POSTPACK WMI_IPV6_ADDR;

#define WMI_MAX_NS_OFFLOADS           2
#define WMI_MAX_ARP_OFFLOADS          2

#define WMI_ARPOFF_FLAGS_VALID              (1 << 0)    /* the tuple entry is valid */
#define WMI_ARPOFF_FLAGS_MAC_VALID          (1 << 1)    /* the target mac address is valid */ 
#define WMI_ARPOFF_FLAGS_REMOTE_IP_VALID    (1 << 2)    /* remote IP field is valid */ 

typedef PREPACK struct {
    A_UINT8           flags;                   /* flags */
    A_UINT8           target_ipaddr[4];        /* IPV4 addresses of the local node*/
    A_UINT8           remote_ipaddr[4];        /* source address of the remote node requesting the ARP (qualifier) */
    A_UINT8           target_mac[ATH_MAC_LEN]; /* mac address for this tuple, if not valid, the local MAC is used */
} POSTPACK WMI_ARP_OFFLOAD_TUPLE;

#define WMI_NSOFF_FLAGS_VALID           (1 << 0)    /* the tuple entry is valid */
#define WMI_NSOFF_FLAGS_MAC_VALID       (1 << 1)    /* the target mac address is valid */   
#define WMI_NSOFF_FLAGS_REMOTE_IP_VALID (1 << 2)    /* remote IP field is valid */ 

#define WMI_NSOFF_MAX_TARGET_IPS    2

typedef PREPACK struct {
    A_UINT8           flags;                     /* flags */
    WMI_IPV6_ADDR     target_ipaddr[WMI_NSOFF_MAX_TARGET_IPS]; /* IPV6 target addresses of the local node  */
    WMI_IPV6_ADDR     solicitation_ipaddr;       /* multi-cast source IP addresses for receiving solicitations */ 
    WMI_IPV6_ADDR     remote_ipaddr;             /* address of remote node requesting the solicitation (qualifier) */
    A_UINT8           target_mac[ATH_MAC_LEN];   /* mac address for this tuple, if not valid, the local MAC is used */
} POSTPACK WMI_NS_OFFLOAD_TUPLE;

typedef PREPACK struct {
    A_UINT32                flags;
    WMI_NS_OFFLOAD_TUPLE    ns_tuples[WMI_MAX_NS_OFFLOADS];
    WMI_ARP_OFFLOAD_TUPLE   arp_tuples[WMI_MAX_ARP_OFFLOADS];
} POSTPACK WMI_SET_ARP_NS_OFFLOAD_CMD;

typedef PREPACK struct {
    A_UINT8  filter_list_id;
    A_UINT8  filter_id;
    A_UINT16 filter_size;
    A_UINT8  filter_offset;
    A_UINT8  filter[1];
} POSTPACK WMI_ADD_WOW_EXT_PATTERN_CMD;

    /* WOW extension for larger filter patterns, note these settings are only
     * valid if the WOW extension is installed in the target 
     * When using the WOW extensions, the use of WMI_GET_WOW_LIST_REPLY will return an empty
     * WOW_FILTER structure.  In WOW extended mode, the host is expected to track what filters 
     * are installed in the target. */
#define WOW_EXT_DEF_FILTER_LISTS        1
#define WOW_EXT_DEF_FILTERS_PER_LIST    16
#define WOW_EXT_DEF_PATTERN_SIZE        136     /* typical pattern size */
#define WOW_EXT_FILTER_ID_CLEAR_ALL     0xFFFF  /* use this ID in WMI_DEL_WOW_PATTERN, to clear all patterns */

#define WOW_EXT_MAX_PATTERN_LEN_SIZE    2040  /* absolute max limit on pattern */

    /* compute mask size based on pattern size */
#define WOW_EXT_COMPUTE_MASK_SIZE(msize,psize) \
{                                              \
    *(msize) = ((A_UINT32)psize)/8;            \
    if (((A_UINT32)psize) % 8) {(*(msize))++;} \
}

    /* variable length event providing wake event information. Wake events can be 
     * the result of wake-packets, network events or other miscellaneous wake causes.
     * 
     * NOTE: a WMI data header flag is not sufficient to use as WMI also forwards
     * A-MSDU aggregates. The wake event indicates which frame caused the wake which could
     * be embedded in an A-MSDU frame */ 
typedef enum {
    WOW_EXT_WAKE_TYPE_UNDEF = 0,
    WOW_EXT_WAKE_TYPE_MAGIC  = 1,    /* magic, value = N/A, wake_data = copy of wake packet */
    WOW_EXT_WAKE_TYPE_PATTERN = 2,   /* bitmap pattern, value = filter index, wake_data = copy of wake packet */
    WOW_EXT_WAKE_TYPE_EAPREQ = 3,    /* eap request value = N/A, wake_data = copy of wake packet */
    WOW_EXT_WAKE_TYPE_4WAYHS,        /* 802.1X 4-wake handshake request caused wakeup, wake_data - copy of packet */
    WOW_EXT_WAKE_TYPE_NETWORK_NLO,            /* network list offload match */
    WOW_EXT_WAKE_TYPE_NETWORK_DISASSOC,       /* network association loss */
    WOW_EXT_WAKE_TYPE_NETWORK_GTK_OFFL_ERROR, /* if GTK refresh is enabled, a failure generates a wake event */
    WOW_EXT_WAKE_TYPE_IPV4_TCP_SYN,			  /* If IPv4 TCP sync packets are received, generate a wake event */
	WOW_EXT_WAKE_TYPE_IPV6_TCP_SYN,			  /* If IPv6 TCP sync packets are received, generate a wake event */
    WOW_EXT_WAKE_TYPE_WLAN_HB,                /* wifi heart beat */
    WOW_EXT_WAKE_TYPE_MAX
} WOW_EXT_WAKE_TYPE;

typedef PREPACK struct {
    A_UINT16    flags;                /* flags, currently reserved */ 
    A_UINT8     type;                 /* type of packet that caused the wake event */
    A_UINT8     value;                /* optional value depending on type (see above) */
    A_UINT16    packet_length;        /* if type is a wake packet, then this is the original length */ 
    A_UINT16    wake_data_length;     /* copy length of wake packet data to follow */
    A_UINT8     wake_data[1];         /* start of wake data*/
} POSTPACK WMI_WOW_EXT_WAKE_EVENT;

#define WMI_WOW_EXT_WAKE_MAX_DATA_LENGTH  128

#define GTK_OFFLOAD_REPLAY_COUNTER_BYTES  8

#define WMI_GTK_OFFLOAD_STATUS_FLAGS_RUNNING (1 << 0)  /* GTK offload is running*/ 


#define GTK_OFFLOAD_KEK_BYTES       16
#define GTK_OFFLOAD_KCK_BYTES       16
#define GTK_REPLAY_COUNTER_BYTES    8

typedef PREPACK struct {
    A_UINT32    flags;          /* status flags */
    A_UINT32    refresh_cnt;    /* number of successful GTK refresh exchanges since last SET operation */     
    A_UINT8     replay_counter[GTK_REPLAY_COUNTER_BYTES]; /* current replay counter */
} POSTPACK  WMI_GTK_OFFLOAD_STATUS_EVENT;

#define WMI_GTK_OFFLOAD_OPCODE_SET     1  /* set offload parameters, KEK,KCK and replay counter values are valid */
#define WMI_GTK_OFFLOAD_OPCODE_CLEAR   2  /* clear offload parameters */
#define WMI_GTK_OFFLOAD_OPCODE_STATUS  3  /* get status, generates WMI_GTK_OFFLOAD_STATUS_EVENT  */

 /* structure to issue GTK offload opcode to set/clear or fetch status
  * NOTE: offload is enabled when WOW options are enabled, see WOW_FILTER_OPTION_OFFLOAD_GTK */
typedef PREPACK struct {
    A_UINT32    flags;                          /* control flags */
    A_UINT8     opcode;                         /* opcode */
    A_UINT8     KEK[GTK_OFFLOAD_KEK_BYTES];     /* key encryption key */
    A_UINT8     KCK[GTK_OFFLOAD_KCK_BYTES];     /* key confirmation key */
    A_UINT8     replay_counter[GTK_REPLAY_COUNTER_BYTES];  /* replay counter for re-key */
} POSTPACK  WMI_GTK_OFFLOAD_OP;


#define WMI_RX_FILTER_DEFAULT_MAX_TESTS_PER_FILTER  5   
#define WMI_RX_FILTER_DEFAULT_MAX_FILTERS           10

   /* note the max pattern length was selected to balance size and usefulness.  Header
     * filtering is sized to support IPV6 addresses (16 octets using 2 entries) 
     * and MAC addresses (6 octets) */
#define RX_FILTER_FRAME_TEST_MAX_PATTERN_LEN    8 

typedef enum {
    WMI_RX_FILTER_PACKET_TYPE_NOT_SPEC = 0,    /* not specified */
    WMI_RX_FILTER_PACKET_TYPE_UNICAST = 1,
    WMI_RX_FILTER_PACKET_TYPE_MULTICAST = 2,
    WMI_RX_FILTER_PACKET_TYPE_BROADCAST = 3,
    WMI_RX_FILTER_PACKET_TYPE_MAX      
} WMI_RX_FILTER_PACKET_TYPE;

#define RX_FILTER_FRAME_TEST_FLAGS_EQUALITY     (1 << 0) /* equality test, otherwise !=  */
#define RX_FILTER_FRAME_TEST_FLAGS_MASK         (1 << 1) /* mask test */
#define RX_FILTER_FRAME_TEST_FLAGS_TYPE_CHECK   (1 << 2) /* this entry is a packet type check place holder */

/* testing a frame requires an offset, equality flag, an ANDing mask array and a check bytes array.
 * This provides the flexibity to test a frame for a desired result using the following combinations:
 * 
 *    frame[offset] AND mask[] == check[]
 *    frame[offset] AND mask[] != check[]
 *    frame[offset] == check[] 
 *    frame[offset] != check[] 
 * 
 */
typedef PREPACK struct {
    A_UINT8     flags;     /* frame test flags */
    union {
        A_UINT8  offset;   /* relative offset from this layer's header */
        A_UINT8  type;     /* if flags indicate a type-check, this value is WMI_RX_FILTER_PACKET_TYPE */
    } option;              /* additional test options depending on flags setting */
    A_UINT8     length;    /* number of bytes to test */
    A_UINT8     rsvd0;     /* reserved for target use */
    A_UINT8     mask[RX_FILTER_FRAME_TEST_MAX_PATTERN_LEN];   /* AND mask to apply to each frame byte */
    A_UINT8     check[RX_FILTER_FRAME_TEST_MAX_PATTERN_LEN];  /* check bytes */     
} POSTPACK WMI_RX_FILTER_FRAME_TEST;

typedef PREPACK struct {
    A_UINT8                  index;             /* index for this entry */
    A_UINT8                  reserved[3];       /* padding and future use */
    WMI_RX_FILTER_FRAME_TEST frame_test;        /* frame test information for this entry */
} POSTPACK WMI_RX_FILTER_SET_FRAME_TEST;

    /* structure for WMI_RX_FILTER_SET_FRAME_TEST_LIST_CMDID */
typedef PREPACK struct {
    A_UINT8                       num_entries;            /* number of entries to follow */
    A_UINT8                       reserved[3];            /* padding and future use */
    WMI_RX_FILTER_SET_FRAME_TEST  frame_test_entries[1];  /* start of frame test entries */
} POSTPACK WMI_RX_FILTER_SET_FRAME_TEST_LIST;

typedef enum {
    WMI_RX_FILTER_FRAME_HDR_TEST_MAC = 0,      /* testing MAC fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_SNAP = 1,     /* testing SNAP fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_ARP = 2,      /* testing ARP fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_GEN_ETHPROTO, /* testing a generic pattern in ether payload */
    WMI_RX_FILTER_FRAME_HDR_TEST_IPV4,         /* testing IPV4 fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_IPV6,         /* testing IPV6 fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_GEN_IPPROTO,  /* testing a generic pattern in IP payload */
    WMI_RX_FILTER_FRAME_HDR_TEST_UDP,          /* testing UDP fields */
    WMI_RX_FILTER_FRAME_HDR_TEST_MAX        
} WMI_RX_FILTER_FRAME_HDR_TEST_TYPE;

#define MAKE_RX_FILTER_FRAME_HDR_MASK(b) (1 << (b))

#define WMI_RX_FILT_FRAME_HDR_TEST_MAC  \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_MAC)
#define WMI_RX_FILT_FRAME_HDR_TEST_SNAP  \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_SNAP)
#define WMI_RX_FILT_FRAME_HDR_TEST_ARP  \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_ARP)
#define WMI_RX_FILT_FRAME_HDR_TEST_IPV4 \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_IPV4)
#define WMI_RX_FILT_FRAME_HDR_TEST_IPV6 \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_IPV6)
#define WMI_RX_FILT_FRAME_HDR_TEST_UDP  \
        MAKE_RX_FILTER_FRAME_HDR_MASK(WMI_RX_FILTER_FRAME_HDR_TEST_UDP)

typedef PREPACK struct  {
    A_UINT8  type;               /* header test type  (see WMI_RX_FILTER_FRAME_HDR_TEST_TYPE) */
    A_UINT8  frame_test_index;   /* corresponding frame test */     
    A_UINT8  reserved[2];        /* padding and future use */           
} POSTPACK WMI_RX_FILTER_SET_HDR_TEST;

typedef enum {
    WMI_RX_COALESCE_FILTER_OP_UNDEF = 0,
    WMI_RX_COALESCE_FILTER_OP_SET = 1,
    WMI_RX_COALESCE_FILTER_OP_CLEAR = 2,  
    WMI_RX_COALESCE_FILTER_OP_MAX,
} WMI_RX_COALESCE_FILTER_OPCODE;

typedef PREPACK struct  {
    A_UINT8            op_code;              /* op code */
    A_UINT8            index;                /* filter index  */
    A_UINT8            reserved[2];          /* padding and future use */
    union {
            /* parameters for SET opcode */
        struct {
            A_UINT32   coalesc_delay_ms;     /* coalesc delay in milliseconds */      
            A_UINT8    total_hdr_tests;      /* number of hdr test elements to follow */
            A_UINT8    reserved[3];          /* padding and future use */
        } set_filter_params;
    } params;
    union {
            /* data for SET opcode */
        WMI_RX_FILTER_SET_HDR_TEST  header_tests[1];      /* one or more entries */ 
    } data;
} POSTPACK WMI_RX_FILTER_COALESCE_FILTER_OP;

    
/*
 * Transmit complete event data structure(s)
 */


typedef PREPACK struct {
#define TX_COMPLETE_STATUS_SUCCESS 0
#define TX_COMPLETE_STATUS_RETRIES 1
#define TX_COMPLETE_STATUS_NOLINK  2
#define TX_COMPLETE_STATUS_TIMEOUT 3
#define TX_COMPLETE_STATUS_OTHER   4

    A_UINT8 status; /* one of TX_COMPLETE_STATUS_... */
    A_UINT8 pktID; /* packet ID to identify parent packet */
    A_UINT8 rateIdx; /* rate index on successful transmission */
    A_UINT8 ackFailures; /* number of ACK failures in tx attempt */
#if 0 /* optional params currently ommitted. */
    A_UINT32 queueDelay; // usec delay measured Tx Start time - host delivery time
    A_UINT32 mediaDelay; // usec delay measured ACK rx time - host delivery time
#endif
} POSTPACK TX_COMPLETE_MSG_V1; /* version 1 of tx complete msg */

typedef PREPACK struct {
    A_UINT8 numMessages; /* number of tx comp msgs following this struct */
    A_UINT8 msgLen; /* length in bytes for each individual msg following this struct */
    A_UINT8 msgType; /* version of tx complete msg data following this struct */
    A_UINT8 reserved; /* individual messages follow this header */
} POSTPACK WMI_TX_COMPLETE_EVENT;

#define WMI_TXCOMPLETE_VERSION_1 (0x01)


/*
 * ------- AP Mode definitions --------------
 */

/*
 * !!! Warning !!!
 * -Changing the following values needs compilation of both driver and firmware
 */
#ifdef AR6002_REV2
#define AP_MAX_NUM_STA          4
#else
#ifdef BUILD_MDM9X25
#define AP_MAX_NUM_STA          15
#else /* !BUILD_MDM9X25 */
#define AP_MAX_NUM_STA          10
#endif /* BUILD_MDM9X25 */
#endif

/*Maximum no. of virtual interface supported*/
#if defined(AR6002_REV63) || defined(AR6002_REV64) || defined(AR6002_REV65) || defined(AR6002_REV66) || defined(AR6002_REV67) || defined(AR6002_REV68)|| defined(AR6004_FPGA)
#define NUM_DEV     4
#define NUM_CONN    (AP_MAX_NUM_STA + NUM_DEV - 1) /* As P2P device port won't enter CONN state, so we omit 1 CONN buffer */
#else
#define NUM_DEV     3
#define NUM_CONN    (AP_MAX_NUM_STA + NUM_DEV)
#endif
extern struct wmi_t *wmi_list[NUM_DEV];

#define AP_ACL_SIZE             10
#define IEEE80211_MAX_IE        256
#define MCAST_AID               0xFF /* Spl. AID used to set DTIM flag in the beacons */
#define DEF_AP_COUNTRY_CODE     "US "
#define DEF_AP_WMODE_G          WMI_11G_MODE
#define DEF_AP_WMODE_AG         WMI_11AG_MODE
#define DEF_AP_DTIM             5
#define DEF_BEACON_INTERVAL     100

/* ACS scan policy */
typedef enum {
    AP_ACS_NORMAL = 0,      /* 1, 6, 11 */
    AP_ACS_DISABLE_CH11,    /* 1, 6 */
    AP_ACS_INCLUDE_CH13,    /* 1, 5, 9, 13 */
    AP_ACS_DISABLE_CH1,     /* dont use 1 */
    AP_ACS_DISABLE_CH1_6,   /* dont use 1 & 6 */
    AP_ACS_POLICY_MAX
}WMI_AP_ACS_POLICY_LIST;

/* AP mode disconnect reasons */
#define AP_DISCONNECT_STA_LEFT      101
#define AP_DISCONNECT_FROM_HOST     102
#define AP_DISCONNECT_COMM_TIMEOUT  103
#define AP_DISCONNECT_MAX_STA       104
#define AP_DISCONNECT_ACL           105
#define AP_DISCONNECT_STA_ROAM      106
#define AP_DISCONNECT_DFS_CHANNEL   107 
/*
 * Used with WMI_AP_HIDDEN_SSID_CMDID
 */
#define HIDDEN_SSID_FALSE   0
#define HIDDEN_SSID_TRUE    1
typedef PREPACK struct {
    A_UINT8     hidden_ssid;
} POSTPACK WMI_AP_HIDDEN_SSID_CMD;

/*
 * Used with WMI_AP_ACL_POLICY_CMDID
 */
#define AP_ACL_DISABLE          0x00
#define AP_ACL_ALLOW_MAC        0x01
#define AP_ACL_DENY_MAC         0x02
#define AP_ACL_RETAIN_LIST_MASK 0x80
typedef PREPACK struct {
    A_UINT8     policy;
} POSTPACK WMI_AP_ACL_POLICY_CMD;

/*
 * Used with WMI_AP_ACL_MAC_LIST_CMDID
 */
#define ADD_MAC_ADDR    1
#define DEL_MAC_ADDR    2
typedef PREPACK struct {
    A_UINT8     action;
    A_UINT8     index;
    A_UINT8     mac[ATH_MAC_LEN];
    A_UINT8     wildcard;
} POSTPACK WMI_AP_ACL_MAC_CMD;

typedef PREPACK struct {
    A_UINT16    index;
    A_UINT8     acl_mac[AP_ACL_SIZE][ATH_MAC_LEN];
    A_UINT8     wildcard[AP_ACL_SIZE];
    A_UINT8     policy;
} POSTPACK WMI_AP_ACL;

/*
 * Used with WMI_AP_SET_NUM_STA_CMDID
 */
typedef PREPACK struct {
    A_UINT8     num_sta;
} POSTPACK WMI_AP_NUM_STA_CMD;

/*
 * Used with WMI_AP_SET_MLME_CMDID
 */
typedef PREPACK struct {
    A_UINT8    mac[ATH_MAC_LEN];
    A_UINT16   reason;              /* 802.11 reason code */
    A_UINT8    cmd;                 /* operation to perform */
/* MLME Commands */
#define WMI_AP_MLME_ASSOC       1   /* associate station */
#define WMI_AP_DISASSOC         2   /* disassociate station */
#define WMI_AP_DEAUTH           3   /* deauthenticate station */
#define WMI_AP_MLME_AUTHORIZE   4   /* authorize station */
#define WMI_AP_MLME_UNAUTHORIZE 5   /* unauthorize station */
} POSTPACK WMI_AP_SET_MLME_CMD;

typedef PREPACK struct {
    A_UINT32 period;
} POSTPACK WMI_AP_CONN_INACT_CMD;

typedef PREPACK struct {
    A_UINT32 period_min;
    A_UINT32 dwell_ms;
} POSTPACK WMI_AP_PROT_SCAN_TIME_CMD;

typedef struct {
    A_BOOL flag;
    A_UINT16 rsvd;
    A_UINT16 aid;
} WMI_AP_SET_PVB_CMD;

#define WMI_DISABLE_REGULATORY_CODE "FF"

typedef PREPACK struct {
    A_UCHAR countryCode[3];
} POSTPACK WMI_AP_SET_COUNTRY_CMD;

typedef PREPACK struct {
    A_UINT8 dtim;
} POSTPACK WMI_AP_SET_DTIM_CMD;

/* user selected ldpc and stbc infor are passed as bit field
 * in enable param along 11n enable status from host to
 * firmware. Meaning of each bit set is defined  below.
 * Currently  5 bits are used for LDPC and STBC in u8 enable.
 * this is only for MDM9x25 project, rest is similar to old
 * */
#define WMI_HTCAP_11N_ENABLE            (1 << 0)
#define WMI_HTCAP_LDPC_CODING           (1 << 1)
#define WMI_HTCAP_RX_STBC_1SS           (1 << 2)
#define WMI_HTCAP_RX_STBC_2SS           (1 << 3)
#define WMI_HTCAP_RX_STBC_3SS           (1 << 4)
#define WMI_HTCAP_TX_STBC               (1 << 5)
#define WMI_REQUIRE_HT			(1 << 7)

#define WMI_MODE_11N_ONLY      2 
#define WMI_HT40_MODE_MASK      0xC0
#define WMI_HT40_MODE_OFFSET    6
#define WMI_HT40_MODE_DEFAULT   0
#define WMI_HT40_MODE_ABOVE     1
#define WMI_HT40_MODE_BELOW     2
typedef PREPACK struct {
    A_UINT8  band; /* specifies which band to apply these values */
    A_UINT8  enable; /* allows 11n to be disabled on a per band basis */
    A_UINT8  chan_width_40M_supported;
    A_UINT8  short_GI_20MHz;
    A_UINT8  short_GI_40MHz;
    A_UINT8  intolerance_40MHz;
    A_UINT8  max_ampdu_len_exp;
} POSTPACK WMI_SET_HT_CAP_CMD;

typedef PREPACK struct {
    A_UINT8   sta_chan_width;
    A_UINT8   ap_ht_info; /* b0:b1 - ht_opmode */
} POSTPACK WMI_SET_HT_OP_CMD;

typedef PREPACK struct {
    A_UINT32 rateMasks[WMI_MODE_MAX*WMI_MAX_RATE_MASK];
} POSTPACK WMI_SET_TX_SELECT_RATES_CMD;

typedef PREPACK struct {
    A_UINT32    sgiMask[WMI_MAX_RATE_MASK];
    A_UINT8     sgiPERThreshold;
} POSTPACK WMI_SET_TX_SGI_PARAM_CMD;
/* Enable for MCS 7/MCS 15 */
#define DEFAULT_SGI_MASK_L32 0x08080000
#define DEFAULT_SGI_MASK_U32 0x00000808
#define DEFAULT_SGI_PER 10

typedef PREPACK struct {
    A_UINT32 rateField[WMI_MAX_RATE_MASK]; /* 1 bit per rate corresponding to index */
#define WMI_RATE_POLICY_ID_MAX 5    
    A_UINT8 id; /* valid values == 1->WMI_RATE_POLICY_ID_MAX */    
    A_UINT8 shortTrys;
    A_UINT8 longTrys;
    A_UINT8 reserved; /* padding */
} POSTPACK WMI_SET_RATE_POLICY_CMD;

typedef PREPACK struct {
    A_UINT8 metaVersion; /* version of meta data for rx packets <0 = default> (0-7 = valid) */
    A_UINT8 dot11Hdr; /* 1 == leave .11 header intact , 0 == replace .11 header with .3 <default> */
    A_UINT8 defragOnHost; /* 1 == defragmentation is performed by host, 0 == performed by target <default> */
    A_UINT8 reserved[1]; /* alignment */
} POSTPACK WMI_RX_FRAME_FORMAT_CMD;


typedef PREPACK struct {
    A_UINT8 enable;     /* 1 == device operates in thin mode , 0 == normal mode <default> */
    A_UINT8 reserved[3];
} POSTPACK WMI_SET_THIN_MODE_CMD;


typedef PREPACK struct {
    A_UINT16    channel; /* frequency in Mhz */
    //A_UINT8 mode; // HT20 or HT40
    //A_UINT8 secondary; // if mode == HT40 secondary channel above | below
} POSTPACK WMI_SET_CHANNEL_CMD;


typedef enum {
    WMI_SET_CHANNEL_RES_SUCCESS = 0, // device has joined the network
    WMI_SET_CHANNEL_RES_FAIL, // device failed for unspecified reason   
}WMI_SET_CHANNEL_RESULT;

typedef PREPACK struct {
    A_UINT8 result; /* the result of the join cmd. one of WMI_THIN_JOIN_RESULT */
    A_UINT8 reserved[3]; /* alignment */
} POSTPACK WMI_SET_CHANNEL_EVENT; 

typedef PREPACK struct {
    A_UINT8 enable;     /* 1 == device operates in promiscuous mode , 0 == normal mode <default> */
#define WMI_PROM_FILTER_SRC_ADDR 0x01
#define WMI_PROM_FILTER_DST_ADDR 0x02
    A_UINT8 filters;
    A_UINT8 srcAddr[ATH_MAC_LEN];
    A_UINT8 dstAddr[ATH_MAC_LEN];
} POSTPACK WMI_SET_FILTERED_PROMISCUOUS_MODE_CMD;

typedef enum {
    WMI_FILTERMASK_MGMT=0,
    WMI_FILTERMASK_CTRL,
    WMI_FILTERMASK_DATA,
    WMI_FILTERMASK_MAX
}WMI_FILTERMASK_INDEX;

typedef PREPACK struct {
    A_UINT16 filtermask[WMI_FILTERMASK_MAX];     
    A_UINT16 reserved; /* alignment */
} POSTPACK WMI_RX_FRAME_FILTER_CMD;

/* AP mode events */
/* WMI_PS_POLL_EVENT */
typedef PREPACK struct {
    A_UINT16 aid;
} POSTPACK WMI_PSPOLL_EVENT;

typedef PREPACK struct {
    A_UINT32 tx_bytes;
    A_UINT32 tx_pkts;
    A_UINT32 tx_error;
    A_UINT32 tx_discard;
    A_UINT32 rx_bytes;
    A_UINT32 rx_pkts;
    A_UINT32 rx_error;
    A_UINT32 rx_discard;
    A_UINT8 aid;
    A_UINT8 tx_ucast_rate;
    A_UINT16 last_txrx_time;
#ifdef SB_SPECIFIC_INTEREST_SUPPORT
    A_UINT8 mac[ATH_MAC_LEN];
    char rsRssi;
#endif	
} POSTPACK WMI_PER_STA_STAT;

#define AP_GET_STATS    0
#define AP_CLEAR_STATS  1

typedef PREPACK struct {
    A_UINT32            action;
    WMI_PER_STA_STAT    sta[AP_MAX_NUM_STA];
} POSTPACK WMI_AP_MODE_STAT;

#define AP_11BG_RATESET1        1
#define AP_11BG_RATESET2        2
#define DEF_AP_11BG_RATESET     AP_11BG_RATESET1
typedef PREPACK struct {
    A_UINT8 rateset;
} POSTPACK WMI_AP_SET_11BG_RATESET_CMD;

typedef PREPACK struct {
    A_UINT8     numSta;
    A_UINT8     data[1];
} POSTPACK WMI_STALIST_EVENT;

/*
 * End of AP mode definitions
 */

struct _wmm_params {
    A_UINT8     acm;           /* ACM parameter */
    A_UINT8     aifsn;         /* AIFSN parameters */
    A_UINT8     logcwmin;      /* cwmin in exponential form */
    A_UINT8     logcwmax;      /* cwmax in exponential form */
    A_UINT16    txopLimit;     /* txopLimit */
};

/* WMI_REPORT_WMM_PARAMS_EVENT */
typedef PREPACK struct {
    struct _wmm_params wmm_params[4];
} POSTPACK WMI_REPORT_WMM_PARAMS_EVENT;

/* P2P module definitions */

/* P2P module commands */
typedef PREPACK struct {
    A_UINT8 go_intent;
    A_UINT8 country[3];
    A_UINT8 reg_class;
    A_UINT8 listen_channel;
    A_UINT8 op_reg_class;
    A_UINT8 op_channel;
    A_UINT16 config_methods;
} POSTPACK WMI_P2P_SET_CONFIG_CMD;

typedef P2P_FW_SET_CONFIG_CMD WMI_P2P_FW_SET_CONFIG_CMD;

typedef PREPACK struct {
    A_UINT16 categ;
    A_UINT16 sub_categ;
} POSTPACK device_type_tuple;

typedef PREPACK struct {
    device_type_tuple pri_dev_type;
    //A_UINT8 pri_device_type[8];
#define MAX_P2P_SEC_DEVICE_TYPES 5
    device_type_tuple sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES];
#define WPS_UUID_LEN 16
    A_UINT8 uuid[WPS_UUID_LEN];
#define WPS_MAX_DEVNAME_LEN 32
    A_UINT8 device_name[WPS_MAX_DEVNAME_LEN];
    A_UINT8 dev_name_len;
   A_UINT16 config_methods;

} POSTPACK WMI_WPS_SET_CONFIG_CMD;

typedef PREPACK struct {
    device_type_tuple pri_dev_type;
    device_type_tuple sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES];
    A_UINT8  device_addr[ATH_MAC_LEN];
} POSTPACK WMI_SET_REQ_DEV_ATTR_CMD;

typedef enum wmi_p2p_discovery_type {
    WMI_P2P_FIND_START_WITH_FULL,
    WMI_P2P_FIND_ONLY_SOCIAL,
    WMI_P2P_FIND_PROGRESSIVE
} WMI_P2P_DISC_TYPE;

typedef PREPACK struct {
   A_UINT32 timeout;
#if defined(AR6002_REV7) && defined(IOT_BUILD_FLAG)
    A_UINT8 type;
#else
   WMI_P2P_DISC_TYPE type;
#endif
} POSTPACK WMI_P2P_FIND_CMD;

typedef PREPACK struct {
    A_UINT32 timeout;
    A_UINT8 type;
} POSTPACK WMI_P2P_FW_FIND_CMD;


typedef PREPACK struct {
    A_UINT16 listen_freq;
    A_UINT16 force_freq;
    A_UINT16 go_oper_freq;
    A_UINT8  dialog_token;
    A_UINT8  peer_addr[ATH_MAC_LEN];
    A_UINT8  own_interface_addr[ATH_MAC_LEN];
    A_UINT8  member_in_go_dev[ATH_MAC_LEN];
    A_UINT8  go_dev_dialog_token;
    P2P_SSID peer_go_ssid;
    A_UINT8  wps_method;
    A_UINT8 dev_capab;
    A_INT8  go_intent;
    A_UINT8 persistent_grp;
} POSTPACK WMI_P2P_GO_NEG_START_CMD;


typedef PREPACK struct {
    A_UINT8 peer_addr[ATH_MAC_LEN];
    A_UINT8 grp_formation_status;
} POSTPACK WMI_P2P_GRP_FORMATION_DONE_CMD;

typedef PREPACK struct {
    A_UINT32 timeout;
}POSTPACK WMI_P2P_LISTEN_CMD;

typedef PREPACK struct {
    A_UINT16 listen_freq;
    A_UINT16 force_freq;
    A_UINT8 status;
    A_INT8 go_intent;
    A_UINT8 wps_buf[512];
    A_UINT16 wps_buflen;
    A_UINT8 p2p_buf[512];
    A_UINT16 p2p_buflen;
    A_UINT8 dialog_token;
    A_UINT8 wps_method;
    A_UINT8 persistent_grp;
    A_UINT8 sa[ATH_MAC_LEN];
}POSTPACK WMI_P2P_GO_NEG_REQ_RSP_CMD;

typedef enum {
    WMI_P2P_INVITE_ROLE_GO,
    WMI_P2P_INVITE_ROLE_ACTIVE_GO,
    WMI_P2P_INVITE_ROLE_CLIENT,
} WMI_P2P_INVITE_ROLE;

typedef PREPACK struct {
#if defined(AR6002_REV7) && defined(IOT_BUILD_FLAG)
    A_UINT8 role;
#else
    WMI_P2P_INVITE_ROLE role;
#endif
    A_UINT16 listen_freq;
    A_UINT16 force_freq;
    A_UINT8 dialog_token;
    A_UINT8 peer_addr[ATH_MAC_LEN];
    A_UINT8 bssid[ATH_MAC_LEN];
    A_UINT8 go_dev_addr[ATH_MAC_LEN];
    P2P_SSID ssid;
    A_UINT8 is_persistent;
    A_UINT8 wps_method;
}POSTPACK WMI_P2P_INVITE_CMD;

typedef P2P_FW_INVITE_CMD WMI_P2P_FW_INVITE_CMD;

typedef PREPACK struct {
    A_UINT16 force_freq;
    A_UINT8 status;
    A_UINT8 dialog_token;
    A_UINT8 p2p_buf[512];
    A_UINT16 p2p_buflen;
    A_UINT8 is_go;
    A_UINT8 group_bssid[ATH_MAC_LEN];
}POSTPACK WMI_P2P_INVITE_REQ_RSP_CMD;

typedef P2P_FW_INVITE_REQ_RSP_CMD WMI_P2P_FW_INVITE_REQ_RSP_CMD;

typedef PREPACK struct {
    A_UINT16 wps_method;
    A_UINT16 listen_freq;
    A_UINT8 dialog_token;
    A_UINT8 peer[ATH_MAC_LEN];
    A_UINT8 go_dev_addr[ATH_MAC_LEN];
    P2P_SSID go_oper_ssid;
}POSTPACK WMI_P2P_PROV_DISC_REQ_CMD;

typedef P2P_FW_PROV_DISC_REQ_CMD WMI_P2P_FW_PROV_DISC_REQ_CMD;

typedef PREPACK struct {
    A_UINT8 enable;
}POSTPACK WMI_P2P_SET_PROFILE_CMD;

typedef enum {
    WMI_P2P_CONFID_LISTEN_CHANNEL=1,
    WMI_P2P_CONFID_CROSS_CONNECT=2,
    WMI_P2P_CONFID_SSID_POSTFIX=3,
    WMI_P2P_CONFID_INTRA_BSS=4,
    WMI_P2P_CONFID_CONCURRENT_MODE=5,
    WMI_P2P_CONFID_GO_INTENT=6,
    WMI_P2P_CONFID_DEV_NAME=7,
    WMI_P2P_CONFID_P2P_OPMODE=8,
    WMI_P2P_CONFID_CCK_RATES=9,
} WMI_P2P_CONF_ID;

typedef P2P_LISTEN_CHANNEL WMI_P2P_LISTEN_CHANNEL;
typedef P2P_SET_CROSS_CONNECT WMI_P2P_SET_CROSS_CONNECT;
typedef P2P_SET_CONCURRENT_MODE WMI_P2P_SET_CONCURRENT_MODE;
typedef P2P_SET_SSID_POSTFIX WMI_P2P_SET_SSID_POSTFIX;
typedef P2P_SET_INTRA_BSS WMI_P2P_SET_INTRA_BSS;
typedef P2P_SET_GO_INTENT WMI_P2P_SET_GO_INTENT;
typedef P2P_SET_DEV_NAME WMI_P2P_SET_DEV_NAME;
typedef P2P_SET_MODE WMI_P2P_SET_MODE;
typedef P2P_SET_CCK_RATES WMI_P2P_SET_CCK_RATES;

typedef PREPACK struct {
    A_UINT8 config_id;    /* set to one of WMI_P2P_CONF_ID */
    PREPACK union {
        WMI_P2P_LISTEN_CHANNEL listen_ch;
        WMI_P2P_SET_CROSS_CONNECT cross_conn;
        WMI_P2P_SET_SSID_POSTFIX ssid_postfix;
        WMI_P2P_SET_INTRA_BSS intra_bss;
        WMI_P2P_SET_CONCURRENT_MODE concurrent_mode;
        WMI_P2P_SET_GO_INTENT go_intent;
        WMI_P2P_SET_DEV_NAME device_name;
        WMI_P2P_SET_MODE mode;
        WMI_P2P_SET_CCK_RATES cck_rates;
    }POSTPACK val;
}POSTPACK WMI_P2P_SET_CMD;

#define RATECTRL_MODE_DEFAULT 0
#define RATECTRL_MODE_PERONLY 1

typedef PREPACK struct {
    A_UINT32 mode;
}POSTPACK WMI_SET_RATECTRL_PARM_CMD;

/* Parameters in rate control module that can be configured via WMI */
typedef enum {
    WMI_RC_PROBE_TIME,
    WMI_RC_RSSI_DOWNTIME,
    WMI_RC_PER_DOWNTIME,
    WMI_RC_PER_THRESHOLD,
    WMI_RC_PER_FILTER_COEFF,
    WMI_RC_PER_AGING_FACTOR,
    WMI_RC_PROBE_THRESHOLD,
} WMI_RC_PARAM_TYPE; 

typedef PREPACK struct {
    WMI_RC_PARAM_TYPE param;
    A_UINT32 value;
}POSTPACK WMI_CFG_RC_PARAM_CMD;

#define WMI_P2P_MAX_TLV_LEN 1024
typedef enum {
    WMI_P2P_SD_TYPE_GAS_INITIAL_REQ     = 0x1,
    WMI_P2P_SD_TYPE_GAS_INITIAL_RESP    = 0x2,
    WMI_P2P_SD_TYPE_GAS_COMEBACK_REQ    = 0x3,
    WMI_P2P_SD_TYPE_GAS_COMEBACK_RESP   = 0x4,
    WMI_P2P_PD_TYPE_RESP                = 0x5,
    WMI_P2P_SD_TYPE_STATUS_IND          = 0x6,
} WMI_P2P_SDPD_TYPE;

typedef enum {
    WMI_P2P_SDPD_TRANSACTION_PENDING = 0x1,
    WMI_P2P_SDPD_TRANSACTION_COMP = 0x2,
} WMI_P2P_SDPD_TRANSACTION_STATUS;

typedef PREPACK struct {
    A_UINT8 type;
    A_UINT8 dialog_token;
    A_UINT8 frag_id;
    A_UINT8 reserved1;          /* alignment */
    A_UINT8 peer_addr[ATH_MAC_LEN];
    A_UINT16 freq;
    A_UINT16 status_code;
    A_UINT16 comeback_delay;
    A_UINT16 tlv_length; 
    A_UINT16 update_indic;
    A_UINT16 total_length;
    A_UINT16 reserved2;         /* future */
    A_UINT8  tlv[WMI_P2P_MAX_TLV_LEN];
} POSTPACK WMI_P2P_SDPD_TX_CMD;

typedef PREPACK struct {
    A_UINT16 go_oper_freq;
    A_UINT8  dialog_token;
    A_UINT8  peer_addr[ATH_MAC_LEN];
    A_UINT8  own_interface_addr[ATH_MAC_LEN];
    A_UINT8  go_dev_dialog_token;
    P2P_SSID peer_go_ssid;
    A_UINT8  wps_method;
    A_UINT8 dev_capab;
} POSTPACK WMI_P2P_CONNECT_CMD_STRUCT;

typedef P2P_FW_CONNECT_CMD_STRUCT WMI_P2P_FW_CONNECT_CMD_STRUCT;

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

typedef PREPACK struct {
    A_UINT8    event_id;    /* event identifier  */
    A_UINT8    length;      /* number of bytes of data that follows */
    A_UINT8    data[1];     /* start of event data */
} POSTPACK WMI_ACS_EVENT_MSG;

#define WMI_ACS_EVENT_HDR_LEN  (sizeof(WMI_ACS_EVENT_MSG) - sizeof(A_UINT8))

typedef PREPACK struct {
    A_UINT8    ctrl_id;     /* control identifier */
    A_UINT8    length;      /* number of bytes of data to follow */
    A_UINT8    data[1];     /* start of control data */
} POSTPACK WMI_ACS_CTRL_MSG;

#define WMI_ACS_CTRL_HDR_LEN  (sizeof(WMI_ACS_CTRL_MSG) - sizeof(A_UINT8))

/* STORE / RECALL Commands AND Events DEFINITION START */
typedef PREPACK struct {
    A_UINT8 enable;
#define STRRCL_RECIPIENT_HOST 1    
#define STRRCL_RECIPIENT_POWERSAFE_MEM 2
    A_UINT8 recipient;
} POSTPACK WMI_STORERECALL_CONFIGURE_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id; /* number of bytes of data to follow */
    A_UINT32 length; /* start of data */
} POSTPACK WMI_STORERECALL_RECALL_DSET;

typedef PREPACK struct {
    A_UINT32 length; /* number of bytes of data to follow */
    A_UINT8  data[1]; /* start of data */
} POSTPACK WMI_STORERECALL_RECALL_CMD;

typedef PREPACK struct {
    A_UINT32 sleep_msec;
    A_UINT8 store_after_tx_empty;
    A_UINT8 store_after_fresh_beacon_rx;    
} POSTPACK WMI_STORERECALL_HOST_READY_CMD;

typedef PREPACK struct {
    A_UINT32    msec_sleep; /* time between power off/on */
    A_UINT32    length; /* length of following data */
    A_UINT8     data[1]; /* start of data */
} POSTPACK WMI_STORERECALL_STORE_EVENT;

typedef PREPACK struct {
    A_UINT16    dset_id; /* dset id */
    A_UINT16    offset;  /* start of data */
    A_UINT32    len;
} POSTPACK WMI_HOST_DSET_READ_EVENT;

typedef PREPACK struct {
    A_UINT16    dset_id; /* dset id */
    A_UINT16    length; /* length of following data */
    A_UINT32    offset;
    A_UINT8     data[1]; /* start of data */
} POSTPACK WMI_HOST_DSET_STORE_EVENT;

typedef PREPACK struct {
    A_UINT32    dset_id; /* dset id */
    A_UINT32    length;  /* start of data */
} POSTPACK WMI_HOST_DSET_CREATE_EVENT;

typedef PREPACK struct {
    A_UINT32    dset_id; /* dset id */
    A_UINT32    offset;
    A_UINT32    length;  /* start of data */
    A_UINT8     data[1];
} POSTPACK WMI_HOST_DSET_LARGE_WRITE_EVENT;

typedef PREPACK struct {
    A_UINT32    dset_id; /* dset id */
    A_UINT32    offset;  /* start of data */
    A_UINT32    len;
} POSTPACK WMI_HOST_DSET_LARGE_READ_EVENT;

typedef PREPACK struct {
    A_UINT32    dset_count;      /* dset max */
} POSTPACK WMI_HOST_DSET_SYNC_EVENT;

typedef PREPACK struct {
    A_UINT32    msec_sleep;
} POSTPACK WMI_STORERECALL_START_EVENT;

/* STORE / RECALL Commands AND Events DEFINITION END */
/* WPS Commands AND Events DEFINITION START */

typedef P2P_WPS_SCAN_LIST_ENTRY WPS_SCAN_LIST_ENTRY;
#define WPS_PIN_LEN (8)

typedef P2P_WPS_PIN WPS_PIN;
typedef enum {
    WPS_PIN_MODE = 0x1,
    WPS_PBC_MODE = 0x2
} WPS_MODE;

typedef enum {
    WPS_REGISTRAR_ROLE = 0x1,
    WPS_ENROLLEE_ROLE  = 0x2
} WPS_OPER_MODE;

typedef enum {
    WPS_DO_CONNECT_AFTER_CRED_RECVD = 0x1
} WPS_START_CTRL_FLAG;

typedef P2P_WMI_WPS_START_CMD WMI_WPS_START_CMD;

typedef enum {
    WPS_STATUS_SUCCESS = 0x0,
    WPS_STATUS_FAILURE = 0x1,
    WPS_STATUS_IDLE = 0x2,       
    WPS_STATUS_IN_PROGRESS  = 0x3,
} WPS_STATUS;

typedef PREPACK struct {
    A_UINT8  wps_status;  /* WPS_STATUS */
    A_UINT8  wps_state;
} POSTPACK WMI_WPS_GET_STATUS_EVENT;

typedef enum {
    WPS_ERROR_INVALID_START_INFO  = 0x1,
    WPS_ERROR_MULTIPLE_PBC_SESSIONS,
    WPS_ERROR_WALKTIMER_TIMEOUT,
    WPS_ERROR_M2D_RCVD,
    WPS_ERROR_PWD_AUTH_FAIL,
    WPS_ERROR_CANCELLED,
    WPS_ERROR_INVALID_PIN
} WPS_ERROR_CODE;

typedef enum {
    WPS_AUTH_TYPE_OPEN    = 0x0001,
    WPS_AUTH_TYPE_WPAPSK  = 0x0002,
    WPS_AUTH_TYPE_SHARED  = 0x0004,
    WPS_AUTH_TYPE_WPA     = 0x0008,
    WPS_AUTH_TYPE_WPA2    = 0x0010,
    WPS_AUTH_TYPE_WPA2PSK = 0x0020
} WPS_AUTH_TYPE;

typedef enum {
    WPS_ENCR_TYPE_NONE = 0x0001,
    WPS_ENCR_TYPE_WEP  = 0x0002,
    WPS_ENCR_TYPE_TKIP = 0x0004,
    WPS_ENCR_TYPE_AES  = 0x0008,
} WPS_ENCR_TYPE;

typedef PREPACK struct {
    A_UINT16 ap_channel; 
    A_UINT8  ssid[WMI_MAX_SSID_LEN];
    A_UINT8  ssid_len;
    A_UINT16 auth_type; /* WPS_AUTH_TYPE */ 
    A_UINT16 encr_type; /* WPS_ENCR_TYPE */
    A_UINT8  key_idx;
    A_UINT8  key[64];
    A_UINT8  key_len;
    A_UINT8  mac_addr[ATH_MAC_LEN];
} POSTPACK WPS_CREDENTIAL;

typedef PREPACK struct {
    A_UINT8 status;      /* WPS_STATUS */
    A_UINT8 error_code;  /* WPS_ERROR_CODE */
    WPS_CREDENTIAL  credential;
    A_UINT8  peer_dev_addr[ATH_MAC_LEN];
} POSTPACK WMI_WPS_PROFILE_EVENT;

typedef PREPACK struct {
    WPS_CREDENTIAL  credential;      
}POSTPACK WMI_P2P_PERSISTENT_PROFILE_CMD;


/* WPS Commands AND Events DEFINITION END */

/*Socket Translation Layer Commands & Events start*/
typedef PREPACK struct {
    A_UINT32  cmd_type;   // WMI socket sub command ID, defined in socketLayer.h
    A_UINT32  length;
    A_UINT8   data[1];
/*    A_UINT32  handle;     //socket handle
    A_UINT32  domain;     //AF_INET, AF_INET6...
    A_UINT32  type;       
    A_UINT32  protocol;	           
    A_UINT32  ip_addr;
    A_UINT16  port;
    A_UINT32  name_length;
    A_UINT16  misc;*/
} POSTPACK WMI_SOCKET_CMD;

typedef PREPACK struct {
    A_UINT32    resp_type;
    A_UINT32    sock_handle; //handle to open socket
    A_UINT32    result;
    A_UINT8     data[1];
} POSTPACK WMI_SOCKET_RESPONSE_EVENT;
/*Socket Translation Layer Command end*/
/* P2P FW OFFLOAD */


typedef PREPACK struct {
    A_UINT8 data[1];
} POSTPACK WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT;


/* P2P FW OFFLOAD */

typedef enum {
    WMI_AP_APSD_DISABLED = 0,
    WMI_AP_APSD_ENABLED
} WMI_AP_APSD_STATUS;

typedef PREPACK struct {
    A_UINT8    enable;
} POSTPACK WMI_AP_SET_APSD_CMD;

typedef enum {
    WMI_AP_APSD_NO_DELIVERY_FRAMES_FOR_THIS_TRIGGER =  0x1,
} WMI_AP_APSD_BUFFERED_TRAFFIC_FLAGS;

typedef PREPACK struct {
    A_UINT16   aid;
    A_UINT16   bitmap;
    A_UINT32   flags;
} POSTPACK WMI_AP_APSD_BUFFERED_TRAFFIC_CMD;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT32 duration;
} POSTPACK WMI_REMAIN_ON_CHNL_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT32 ID;
    A_UINT32 freq;
    A_UINT32 wait;
    A_UINT16 len;
    A_UINT8  data[1];
} POSTPACK  WMI_SEND_ACTION_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT32 ID;
    A_UINT8 ACKStatus;
} POSTPACK  WMI_TX_STATUS_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT8 enable;
} POSTPACK  WMI_PROBE_REQ_REPORT_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT8 disable;
} POSTPACK  WMI_DISABLE_11B_RATES_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT8 RoleID;
    A_UINT8 mgmtFrmType;
    A_UINT8 ieLen;
    A_UINT8 ieInfo[1];
} POSTPACK  WMI_SET_APPIE_EXTENDED_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT32 duration;
} POSTPACK  WMI_REMAIN_ON_CHNL_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT32 duration;
    A_UINT8  status;
} POSTPACK  WMI_CANCEL_REMAIN_ON_CHNL_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT16 len;
    A_UINT8 data[1];
} POSTPACK  WMI_RX_ACTION_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT16 len;
    A_UINT8 data[1];
} POSTPACK  WMI_P2P_CAPABILITIES_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT16 len;
    A_UINT8 data[1];
} POSTPACK  WMI_P2P_RX_PROBE_REQ_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT16 len;
    A_UINT8 data[1];
} POSTPACK  WMI_P2P_RX_PROBE_RESP_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT16 len;
    A_UINT8  data[1];
} POSTPACK  WMI_ACL_REJECT_EVENT_STRUCT;


#define P2P_FLAG_CAPABILITIES_REQ   (0x00000001)
#define P2P_FLAG_MACADDR_REQ        (0x00000002)
#define P2P_FLAG_P2P_MODEL_REQ      (0x00000004)
#define P2P_FLAG_MACADDR_BUNCH_REQ  (0x00000008)

typedef PREPACK struct {
    A_UINT32 InfoReqFlags;
} POSTPACK  WMI_GET_P2P_INFO_STRUCT;

typedef PREPACK struct {
    A_UINT32 InfoReqFlags;
    A_UINT16 len;
    A_UINT8  data[1];
} POSTPACK  WMI_P2P_INFO_EVENT_STRUCT;

typedef PREPACK struct {
    A_UINT8  GOPowerSave;
} POSTPACK  WMI_P2P_CAPABILITIES_STRUCT;

typedef PREPACK struct {
    A_UINT8  MACAddr[6];
} POSTPACK  WMI_P2P_MACADDR_STRUCT;

typedef PREPACK struct {
    A_UINT8  NumDev;
    A_UINT8  MACAddr[4][6];
} POSTPACK  WMI_MACADDR_BUNCH_STRUCT;

#define P2P_HOST_MODEL          1
#define P2P_FW_MODEL            0

typedef PREPACK struct {
    A_UINT8  P2PModel;
} POSTPACK  WMI_P2P_MODEL_STRUCT;

typedef PREPACK struct {
    A_UINT32 freq;
    A_UINT8  DestinationAddr[6];
    A_UINT16 len;
    A_UINT8  data[1];
} POSTPACK  WMI_P2P_PROBE_RESPONSE_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT8 enable;
    A_UINT8 count;
    A_UINT8 noas[1]; /* P2P_NOA_DESCRIPTOR */
} POSTPACK WMI_NOA_INFO_STRUCT;

typedef PREPACK struct {
    A_UINT8 en;
} POSTPACK WMI_P2P_PSIE_CMD_STRUCT;

typedef PREPACK struct {
    A_UINT8 enable;
    A_UINT8 ctwin;
} POSTPACK WMI_OPPPS_INFO_STRUCT;

typedef PREPACK struct {
    A_UINT8 port_id;
    A_UINT8 port_opmode;
    A_UINT8 port_subopmode;
    A_UINT8 mac_addr[6];
} POSTPACK WMI_ADD_PORT_CMD;

typedef PREPACK struct {
    A_UINT8 port_id;
} POSTPACK WMI_DEL_PORT_CMD;

typedef PREPACK struct {
    A_UINT8 status;
    A_UINT8 port_id;
    A_UINT8 mac_addr[6];
} POSTPACK WMI_PORT_STATUS_INFO;

typedef PREPACK struct {
    A_UINT16   rsn_cap;
} POSTPACK WMI_RSN_CAP_CMD;


typedef PREPACK struct {
    A_UINT8     keyIndex;
    A_UINT8     keyLength;
    A_UINT8     keyRSC[6];/* key replay sequence counter */
    A_UINT8     key[WMI_MAX_KEY_LEN];
} POSTPACK WMI_SET_IGTK_CMD;

/*
 * These constants are used with A_WLAN_BAND_SET.
 */ 
#define A_BAND_24GHZ           0
#define A_BAND_5GHZ            1
#define A_NUM_BANDS            2

#define TXFLOW_CTRL_OCS_FLAG_MASK        0x20
#define TXFLOW_CTRL_OCS_FLAG_SHIFT_BITS  5
#define TXFLOW_CTRL_PS_FLAG_MASK         0x10
#define TXFLOW_CTRL_PS_FLAG_SHIFT_BITS   4
#define TXFLOW_CTRL_VO_UAPSD_FLAG_MASK   0x08
#define TXFLOW_CTRL_VO_UAPSD_SHIFT_BITS  3
#define TXFLOW_CTRL_VI_UAPSD_FLAG_MASK   0x04
#define TXFLOW_CTRL_VI_UAPSD_SHIFT_BITS  2
#define TXFLOW_CTRL_BE_UAPSD_FLAG_MASK   0x02
#define TXFLOW_CTRL_BE_UAPSD_SHIFT_BITS  1
#define TXFLOW_CTRL_BK_UAPSD_FLAG_MASK   0x01
#define TXFLOW_CTRL_BK_UAPSD_SHIFT_BITS  0

#define TXFLOW_CTRL_UAPSD_MASK           0x0F
#define TXFLOW_CTRL_UAPSD_SHIFT_BITS     0

#define TXFLOW_CTRL_GET_OCS_STATE(val) (((val)& TXFLOW_CTRL_OCS_FLAG_MASK)>> TXFLOW_CTRL_OCS_FLAG_SHIFT_BITS)
#define TXFLOW_CTRL_GET_PS_FLAG(val)   (((val)& TXFLOW_CTRL_PS_FLAG_MASK)>> TXFLOW_CTRL_PS_FLAG_SHIFT_BITS)
#define TXFLOW_CTRL_GET_UAPSD_FLAG(val, mask, shift) (((val)&(mask)) >>(shift))

typedef PREPACK struct {
    A_UINT8 min_rx_bundle_frame;
    A_UINT8 min_rx_bundle_timeout;
} POSTPACK WMI_SET_BUNDLE_PARAM_CMD;

typedef PREPACK struct {
    A_BOOL  enable;
    A_UINT8 nextProbeCount;
    A_UINT8 maxBackOff;
    A_UINT8 minGtxRssi;
    A_UINT8 forceBackOff;
} POSTPACK WMI_GREENTX_PARAMS_CMD;

typedef PREPACK struct {
    A_UINT8 num_of_conn;
    A_UINT8 ac_map[NUM_CONN];
    A_UINT8 ac_queue_depth[NUM_CONN*2];
} POSTPACK WMI_FLOWCTRL_IND_EVENT;

#define NUM_HOST_RETRY_PKT 32
typedef PREPACK struct {
    A_UINT16 last_pkt_id;
    A_UINT16 num_retry_pkt;
    A_UINT16 retry_pkt_id[NUM_HOST_RETRY_PKT];
} POSTPACK WMI_FLOWCTRL_IND_EVENT_EXT;

typedef PREPACK struct {
    A_UINT8 aid;
    A_UINT8 ac;
    A_UINT8 frames_delivered;
    A_UINT8 reserved;
} POSTPACK WMI_FLOWCTRL_UAPSD_FRAME_DILIVERED_EVENT;



typedef enum {
    SET_LED_SYS_POWEROFF,
    SET_LED_SYS_S3_SUSPEND,
    SET_LED_SYS_S4_S5,
    SET_LED_SYS_DRIVER_DISABLE,
    SET_LED_SYS_WAKEUP,
    SET_LED_SYS_ALWAYS_ON, //just for test!
} SET_LED_SYS_STATE;

     

typedef PREPACK struct {
    SET_LED_SYS_STATE   system_state;
} POSTPACK WMI_SET_LED_CMD;


typedef enum {
    CONFIG_LED_TO_VDD = 0,
    CONFIG_LED_TO_GND = 1,
} CONFIG_LED_CONNECT_TYPE;

typedef enum {
   CONFIG_LED_NOT_WITH_BT = 0,
   CONFIG_LED_WITH_BT = 1,
} CONFIG_LED_WITH_BT_FLG;


typedef PREPACK struct {
    A_UINT8	led_gpio_pin;
    A_UINT8 connect_type;
    A_UINT8	with_bt;
	A_UINT8	led_enable;
} POSTPACK WMI_CONFIG_LED_CMD;


typedef enum {
   CONFIG_LED_DISABLE = 0,
   CONFIG_LED_ENABLE  = 1,
} CONFIG_LED_ENABLE_FLG;

typedef PREPACK struct {
	CONFIG_LED_ENABLE_FLG	led_enable;
} POSTPACK WMI_ENABLE_LED_CMD;


/* Diagnostic WMI (WMID)
 *
 * Diagnostic WMI commands are only for diagnostic tool.
 * Diagnostic WMI commands are encapsulated in a WMI message with
 * WMI_COMMAND_ID=WMI_DIAGNOSTIC_CMDID.
 * Diagnostic WMI events are similarly embedded in a WMI event message with
 * WMI_EVENT_ID=WMI_DIAGNOSTIC_EVENTID.
 */
typedef PREPACK struct {
    A_UINT32    commandId;
    A_UINT32    seq_num;
} POSTPACK WMID_CMD_HDR;

typedef PREPACK struct {
    A_UINT16    seqnum;
    A_UINT8     aid;
    A_UINT8     tid;
}POSTPACK WMI_FLUSH_BUFFERED_DATA_EVENT;

typedef PREPACK struct {
    A_UINT8     num_packets;
    A_UINT8     ac_category;
}POSTPACK WMI_SEND_DUMMY_DATA_EVENT;

typedef PREPACK struct {
    A_UINT8     power_save;
    A_UINT8     aid;
}POSTPACK WMI_CLIENT_POWER_SAVE_EVENT;

typedef PREPACK struct {
    A_UINT8     enable_drop;
}POSTPACK WMI_PACKET_DROP_EVENT;

typedef PREPACK struct {
    A_BOOL  enable;
} POSTPACK WMID_EVENT_SET_CMD;

typedef PREPACK struct {
    A_UINT32    type;
    A_UINT32    low;
    A_UINT32    high;
} POSTPACK WMID_MACFILTER_CMD;

typedef enum {
    WMID_INTERFERENCE_CMDID = 0x2001,
    WMID_RXTIME_CMDID,
    WMID_FSM_EVENT_CMDID,
    WMID_PWR_SAVE_EVENT_CMDID,
    WMID_MACFILTER_CMDID,
    WMID_STAT_RX_RATE_CMDID,
    WMID_STAT_TX_RATE_CMDID,
} WMID_COMMAND_ID;

typedef enum {
    WMID_START_SCAN_EVENTID = 0x3001,
    WMID_FSM_AUTH_EVENTID,
    WMID_FSM_ASSOC_EVENTID,
    WMID_FSM_DEAUTH_EVENTID,
    WMID_FSM_DISASSOC_EVENTID,
    WMID_STAT_RX_RATE_EVENTID,
    WMID_STAT_TX_RATE_EVENTID,
    WMID_INTERFERENCE_EVENTID,
    WMID_RXTIME_EVENTID,
    WMID_PWR_SAVE_EVENTID,
    WMID_FSM_CONNECT_EVENTID,
    WMID_FSM_DISCONNECT_EVENTID,
} WMID_EVENT_ID;

typedef PREPACK struct {
    A_UINT16    oldState;
    A_UINT16    pmState;
} POSTPACK WMID_PWR_SAVE_EVENT;
/* End of Diagnostic WMI (WMID) */

typedef PREPACK struct {
    A_UINT8 enable;
} POSTPACK WMI_STA_BMISS_ENHANCE_CMD;

/* wifi discovery */
typedef PREPACK struct {
    A_UINT8 enable;
    A_UINT8 startPos;
    A_UINT8 length;
    A_UINT8 pattern[1];
}POSTPACK WMI_DISC_IE_FILTER_CMD;

typedef PREPACK struct {
    A_UINT16 enable;
    A_UINT16 channel; /* channels in Mhz */
    A_UINT32 homeDwellTime;	/* max duration in the home channel(msec) */
    A_UINT32 sleepTime;
    A_UINT32 random;
    A_UINT32 numPeers;
    A_UINT32 peerTimeout;
}POSTPACK WMI_DISC_MODE_CMD;

struct disc_peer {
    A_BOOL  valid;
    A_INT32 rssi;
    A_UINT32 rstamp;
    A_UINT8 addr[ATH_MAC_LEN];
};

typedef PREPACK struct {
    A_INT32 rssi;
    A_UINT8 addr[ATH_MAC_LEN];
} POSTPACK WMI_DISC_PEER;

typedef PREPACK struct {
    A_UINT8 peer_num;
    A_UINT8 peer_data[1];
} POSTPACK WMI_DISC_PEER_EVENT;

typedef PREPACK struct {
    A_UINT8 length;
    A_CHAR isoName[1];
} POSTPACK WMI_SET_REGDOMAIN_CMD;

/* WLAN Heart Beat command ID */
#define WLAN_HB_MAX_FILTER_SIZE     32

typedef PREPACK struct {
    A_UINT8 enable;
    A_UINT8 item;
    A_UINT8 session;
} POSTPACK WMI_HB_SET_ENABLE_CMD;

typedef PREPACK struct {
    A_UINT32 srv_ip;
    A_UINT32 dev_ip;
    A_UINT16 src_port;
    A_UINT16 dst_port;
    A_UINT16 timeout;
    A_UINT8  session;
    A_UINT8  gateway_mac[ATH_MAC_LEN];
} POSTPACK WMI_HB_SET_TCP_PARAMS_CMD;

typedef PREPACK struct {
    A_UINT8 length;
    A_UINT8 offset;
    A_UINT8 session;
    A_UINT8 filter[WLAN_HB_MAX_FILTER_SIZE];
} POSTPACK WMI_HB_SET_TCP_PKT_FILTER_CMD;

typedef PREPACK struct {
    A_UINT32 srv_ip;
    A_UINT32 dev_ip;
    A_UINT16 src_port;
    A_UINT16 dst_port;
    A_UINT16 interval;
    A_UINT16 timeout;
    A_UINT8  session;
    A_UINT8  gateway_mac[ATH_MAC_LEN];
} POSTPACK WMI_HB_SET_UDP_PARAMS_CMD;

typedef PREPACK struct {
    A_UINT8 length;
    A_UINT8 offset;
    A_UINT8 session;
    A_UINT8 filter[WLAN_HB_MAX_FILTER_SIZE];
} POSTPACK WMI_HB_SET_UDP_PKT_FILTER_CMD;

typedef PREPACK struct {
    A_UINT32 device_ip;
    A_UINT32 server_ip;
    A_UINT32 gateway_ip;
    A_UINT8 gateway_mac[ATH_MAC_LEN];
} POSTPACK WMI_HB_SET_NETWORK_INFO_CMD;

typedef PREPACK struct {
    A_UINT32 addr;
    A_UINT32 val;
} POSTPACK WMI_CUSTOMER_REG_STRUCT;

/**
 * brief get connection type
 */
typedef enum athcfg_wcmd_phymode{   
    ATHCFG_WCMD_PHYMODE_AUTO,               /* autoselect */
    ATHCFG_WCMD_PHYMODE_11A,                /* 5GHz, OFDM */
    ATHCFG_WCMD_PHYMODE_11B,                /* 2GHz, CCK  */
    ATHCFG_WCMD_PHYMODE_11G,                /* 2GHz, OFDM */
    ATHCFG_WCMD_PHYMODE_11NA_HT20,          /* 5GHz, HT20 */
    ATHCFG_WCMD_PHYMODE_11NG_HT20,          /* 2GHz, HT20 */
    ATHCFG_WCMD_PHYMODE_11NA_HT40,          /* 5GHz, HT40 */    
    ATHCFG_WCMD_PHYMODE_11NG_HT40,          /* 2GHz, HT40 */
}athcfg_wcmd_phymode_t;

#define ATHCFG_WCMD_ADDR_LEN             6
typedef PREPACK struct {
#define ATHCFG_WCMD_STAINFO_LINKUP        0x1 
#define ATHCFG_WCMD_STAINFO_SHORTGI       0x2
    A_UINT32                    flags;                       /* Flags */
    athcfg_wcmd_phymode_t       phymode;                     /* STA Connection Type */
    A_UINT8                     bssid[ATHCFG_WCMD_ADDR_LEN]; /* STA Current BSSID */
    A_UINT32                    assoc_time;                  /* STA association time */
    A_UINT32                    wpa_4way_handshake_time;     /* STA 4-way time */
    A_UINT32                    wpa_2way_handshake_time;     /* STA 2-way time */  
    A_UINT32                    rx_rate_kbps;                /* STA latest RX Rate (Kbps) */    
#define ATHCFG_WCMD_STAINFO_MCS_NULL      0xff    
    A_UINT8                     rx_rate_mcs;                 /* STA latest RX Rate MCS (11n) */    
    A_UINT32                    tx_rate_kbps;                /* STA latest TX Rate (Kbps) */    
    A_UINT8                     rx_rssi;                     /* RSSI of all received frames */
    A_UINT8                     rx_rssi_beacon;              /* RSSI of Beacon */
    /* TBD : others information */
} POSTPACK WMI_CUSTOMER_STAINFO_STRUCT;

/**
 * @brief get version-info
 */
typedef PREPACK struct {
    A_UINT32    sw_version;        /* SW Version */
    A_UINT8    driver[32];         /* Driver Short Name */
} POSTPACK WMI_CUSTOMER_VERSION_INFO_STRUCT;

/**
 * @brief  set/get testmode-info
 */
typedef PREPACK struct {
    A_UINT8      bssid[ATHCFG_WCMD_ADDR_LEN];
    A_UINT32     chan;         /* ChanID */

#define ATHCFG_WCMD_TESTMODE_CHAN     0x1    
#define ATHCFG_WCMD_TESTMODE_BSSID    0x2    
#define ATHCFG_WCMD_TESTMODE_RX       0x3    
#define ATHCFG_WCMD_TESTMODE_RESULT   0x4    
#define ATHCFG_WCMD_TESTMODE_ANT      0x5    
    A_UINT16     operation;    /* Operation */
    A_UINT8      antenna;      /* RX-ANT */
    A_UINT8      rx;           /* RX START/STOP */
    A_UINT32     rssi_combined;/* RSSI */
    A_UINT32     rssi0;        /* RSSI */
    A_UINT32     rssi1;        /* RSSI */
    A_UINT32     rssi2;        /* RSSI */
} POSTPACK WMI_CUSTOMER_TESTMODE_STRUCT;

typedef PREPACK struct {
    A_UINT8 enable;
} POSTPACK  WMI_CUSTOME_PROBE_RESP_REPORT_CMD_STRUCT;

/**
 * @brief csa info
 */
typedef PREPACK struct {
    A_UINT32    freq;
    A_UINT16	networkType;
} POSTPACK WMI_CSA_INFO_STRUCT;

#define WMI_CE_RATE_MAX (48)
#define WMI_CE_TPWR_MODE_MAX (6)
typedef PREPACK struct {
	A_UINT32 channel;//input
	A_UINT32 flags;//input
	A_UINT32 txpower[WMI_CE_TPWR_MODE_MAX];//output
	A_UINT8  ctlmode[WMI_CE_TPWR_MODE_MAX];//output
	A_UINT16 cal_txpower[WMI_CE_RATE_MAX];
} POSTPACK  WMI_CTL_STRUCT;


/* MCC Profile setting options */
#define TBTT_MCC_PROFILE_STA50      50
#define TBTT_MCC_PROFILE_STA20      24
#define TBTT_MCC_PROFILE_STA80      76

typedef enum {
        MCC_PROFILE_STA50       = 0x0001,
        MCC_PROFILE_STA20       = 0x0002,
        MCC_PROFILE_STA80       = 0x0004,
        MCC_CTS_ENABLE          = 0x0010,
        MCC_PSPOLL_ENABLE       = 0x0020,
        MCC_BADAP_CTS_ENABLE    = 0x0040,
        MCC_BADAP_PSPOLL_ENABLE = 0x0080,
        MCC_DUAL_TIME_MASK      = 0x0100,
} MCC_PROFILE;


typedef PREPACK struct {
    A_UINT32    mcc_profile;
} POSTPACK WMI_MCC_PROFILE_CMD;

typedef enum {
    MCC_WPS_PROTECT_DISABLED = 0,
    MCC_WPS_PROTECT_ENABLED  = 1,
    MCC_WPS_PROTECT_IDLE  = 2,
    MCC_WPS_PROTECT_MAX = 3,
} MCC_WPS_PROTECT;

typedef PREPACK struct {
    A_UINT8 mode;
    A_UINT8 reserved;
}POSTPACK WMI_SET_CHAIN_MASK_CMD;

typedef PREPACK struct {
    A_UINT8 enable;     /* 1: enable, 0: disable */
    A_UINT8 reserved;
} POSTPACK WMI_SET_MEDIA_STREAM_CMD;

typedef PREPACK struct {
    A_UINT8 send_event;
}POSTPACK WMI_SET_MCC_EVENT_MODE_CMD;

typedef enum {
    RECOVERY_SIM_ASSERT     = 0x01,
    RECOVERY_SIM_NO_DETECT  = 0x02,
    RECOVERY_SIM_CTR_EP_FULL= 0x03,
    RECOVERY_SIM_EMPTY_POINT= 0x04,
    RECOVERY_SIM_STACK_OV   = 0x05,
} RECOVERY_SIM_TYPE;

typedef PREPACK struct {
    A_UINT8 legacy_opt_mode;
}POSTPACK WMI_SET_LEGACY_OPT_CMD;

typedef PREPACK struct {
    A_UINT8 type;     /*0:unused 1: ASSERT, 2: not respond detect command,3:  simulate ep-full(),4:...*/
    A_UINT8 reserved; /*unused now*/
    A_UINT16 delay_time_ms;   /*0xffff means the simulate will delay for random time (0 ~0xffff ms)*/
}POSTPACK WMI_SET_RECOVERY_TEST_PARAMETER_CMD;



typedef PREPACK struct
{
	A_UINT8					diversity_control;
    A_UINT8 				main_lna_conf;
    A_UINT8 				alt_lna_conf;
    A_UINT8 				fast_div_bias;
    A_UINT8 				main_gaintb;
    A_UINT8 				alt_gaintb;
} POSTPACK WMI_ANT_DIV_CMD;

typedef PREPACK struct
{
    A_UINT32     mode;
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

//#ifdef ANTENNA_DIVERSITY
typedef PREPACK struct
{
    A_UINT32     			mode;
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
//#endif

typedef enum {
    WMI_SCAN_PLAN_IN_ORDER  = 0,
    WMI_SCAN_PLAN_REVERSE_ORDER = 1,
    WMI_SCAN_PLAN_HOST_ORDER = 2,
} WMI_SCAN_PLAN_TYPE;

typedef PREPACK struct {
    A_UINT32 flag;
    A_UINT8 type;                    /* WMI_SCAN_PLAN_TYPE */
    A_UINT8 numChannels;             /* number of channels */
    A_UINT16 channelList[1];         /* channels in Mhz */
} POSTPACK WMI_SET_SCAN_CHAN_PLAN;

typedef PREPACK struct {
    A_UINT32 options;
    A_UINT32 wake_msec;                
} POSTPACK WMI_PMU_START_CMD;

#define  SUSPEND_ON_GPIO30		   0x00000001
#define  SUSPEND_ON_GPIO30_RISING          0x00000002
#define  SUSPEND_ON_GPIO31                 0x00000004
#define  SUSPEND_ON_GPIO31_RISING          0x00000008

typedef PREPACK struct {
    A_UINT32 sleepOnGPIO_flag;
    A_UINT32 options;
    A_UINT32 wake_msec;
} POSTPACK WMI_PMU_SUSPENDONGPIO_CMD;

typedef PREPACK struct {
    A_UINT32 options;
} POSTPACK WMI_PFM_CMD;

typedef PREPACK struct {
    A_UINT16 dset_id;
    A_UINT16 len;
    A_UINT32 offset;
} POSTPACK WMI_DSET_HOST_CFG_ITEM;

typedef PREPACK struct {
    A_UINT32 length;
    WMI_DSET_HOST_CFG_ITEM buffer[1];      /* WMI_SET_PARAMS */
} POSTPACK WMI_DSET_HOST_CFG_CMD;

typedef PREPACK struct {
    A_UINT32 length;
    A_UINT32 offset;
    A_UINT8  data[1];
} POSTPACK WMI_DSET_HOST_READ_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id;
    A_UINT32 offset;
    A_UINT32 length;
} POSTPACK WMI_DSET_HOST_LARGE_READ_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id;
    A_UINT32 offset;
    A_UINT32 length;
} POSTPACK WMI_DSET_HOST_LARGE_WRITE_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id;
    A_UINT32 length;
} POSTPACK WMI_DSET_HOST_SYNC_ITEM;

typedef PREPACK struct {
    A_UINT32 dsets_count;
    A_UINT8  data[1];
} POSTPACK WMI_DSET_HOST_RESP_SYNC_CMD;

typedef PREPACK struct {
    A_UINT32 flags;
} POSTPACK WMI_DSET_HOST_RESP_CREATE_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id;
    A_UINT32 offset;
    A_UINT32 length;
} POSTPACK WMI_DSET_HOST_RESP_WRITE_CMD;

typedef PREPACK struct {
    A_UINT32 dset_id;
    A_UINT32 length;
    A_UINT32 offset;
    A_UINT8  data[1];
} POSTPACK WMI_DSET_HOST_RESP_READ_CMD;

/************************************************
****************      i2c support        ****************
************************************************/
typedef PREPACK struct {
    A_UINT16              cmd;            /* status returned */
    A_UINT32              status;            /* status returned */
    A_UINT32              data;              /* usually 1~4 */
} POSTPACK WMI_I2C_DATA_EVENT;

typedef PREPACK struct {
    A_UINT32              sck;             /* pin of sck, refer to structure I2C_SCK_PIN */
    A_UINT32              sda;             /* pin of sda, refer to structure I2C_SDA_PIN  */
    A_UINT32              freq;           /* sck freq id, refer to structure I2C_FREQ */
} POSTPACK WMI_I2C_INIT_CMD;

typedef PREPACK struct {
    A_UINT32              devAddr;          /* device address, 7 bits */
    A_UINT32              addr;               /* offset in device */
    A_UINT32              addrLen;           /* in unit of byte */
    A_UINT32              value;           /* usually 1~4 */
    A_UINT32              valueLen;           /* usually 1~4 */
} POSTPACK WMI_I2C_DATA_CMD;


/************************************************
****************      i2s support        ****************
************************************************/

typedef PREPACK struct {
    A_UINT8                port;                   /* i2s0/1 */
    A_UINT32              cmd;                   /* cmd id */
    A_UINT32              status;               /* returned status */
    A_UINT32              len;                     /* the length of data */
    A_UINT8                data[1];               /* just a data pointer */
} POSTPACK WMI_I2S_DATA_EVENT;
#if 0
typedef PREPACK struct {
    A_UINT8                port;                   /* i2s0/1 */
    A_UINT32              freq;                   /* refer to structure I2S_FREQ */
    A_UINT8               data_size;           /* 8/16/24/32 */
    A_UINT8               tx_buf_cnt;         /* tx_buf_cnt */
    A_UINT8               rx_buf_cnt;         /* rx_buf_cnt */
    A_UINT16             buf_size;             /* buf_size */
} POSTPACK WMI_I2S_INIT_CMD;
#else
typedef PREPACK struct {
    i2s_api_config_s *i2s_api_config;
    A_UINT32             intr_cb_txcomp[1];                   /* void* cb func */
    A_UINT32            intr_cb_rx[1];               /* void* cb func */
} POSTPACK WMI_I2S_INIT_CMD;

#endif
typedef PREPACK struct {
    A_UINT8                port;                   /* i2s0/1 */
    A_UINT32              len;                     /* the length of data */
    A_UINT8                data[1];               /* just a data pointer */
} POSTPACK WMI_I2S_DATA_CMD;

typedef PREPACK struct {
    A_UINT8                port;                   /* i2s0/1 */
    A_UINT8                control;               /* refer to I2S_RCV_CONTROL */
} POSTPACK WMI_I2S_RCV_CTRL_CMD;


/*
 *
 */
/*pwm cdr*/
typedef PREPACK struct {
	A_INT16 	       	ret;
	A_UINT8			module_select;         
	A_UINT8			enable;               
	A_UINT32		port_id;           

} POSTPACK WMI_PWM_CTRL_CMD;

typedef PREPACK struct {
	A_INT32 	       	ret;
	A_UINT32              freq;         
	A_UINT32              duty;  
	A_UINT32              phase; 
	A_UINT32              port_id;           
	A_UINT32 		src_clk;
} POSTPACK WMI_PWM_SET_CMD;

/*end pwm cdr*/
/*i2cm cdr*/
typedef PREPACK struct {
	A_INT16 	       	ret;
	A_UINT8			pin_pair_id;         
	A_INT8			freq;   
	A_UINT32		timeout;
} POSTPACK WMI_I2CM_INIT_CMD;
typedef PREPACK struct {
	A_INT32 	       	ret;
	A_INT32 			tx_len;
	A_INT32 			rx_len;  
	A_UINT32		pin_pair_id;         
	A_UCHAR *		tx_data;
	A_UCHAR *		rx_data;
      

} POSTPACK WMI_I2CM_CTRL_CMD;
/*end i2cm cdr*/
typedef PREPACK struct {
	A_UINT32              enable;
	A_UINT32		pin_pair_id;
} POSTPACK WMI_I2CS_CTRL_CMD;
typedef PREPACK struct {   
	I2CS_REG_PARA* 	reg_params;               
	I2CS_FIFO_PARA* 	fifo_params;           
	I2CS_CSR_PARA* 	csr_params;
	A_UINT8	              mode_type;      
} POSTPACK WMI_I2CS_MODE_CMD;

typedef  struct {
	A_INT16			ret;
	A_UINT16 		cmd;               
	A_UINT32              data;    
	
}  WMI_I2CS_COMMD_CMD;


/*end i2cs cdr*/
#define     DSET_OP_CREATE		0x1001
#define     DSET_OP_OPEN                0x1002
#define     DSET_OP_READ                0x1003
#define     DSET_OP_WRITE               0x1004
#define     DSET_OP_COMMIT              0x1005
#define     DSET_OP_CLOSE               0x1006
#define     DSET_OP_SIZE                0x1007
#define     DSET_OP_DELETE              0x1008

typedef PREPACK struct {
    A_UINT32       commandId;
} POSTPACK WMIX_DSET_CMD_HDR;

typedef PREPACK struct {
    A_UINT32       dset_id;
    A_UINT32       flags;
    A_UINT32       length;
} POSTPACK WMIX_DSET_CREATE_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
    A_UINT32       flags;
    A_UINT32       length;
} POSTPACK WMIX_DSET_OPEN_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
    A_UINT32       offset;
    A_UINT32       dataLen;
} POSTPACK WMIX_DSET_READ_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
    A_UINT32       flags;
    A_UINT32       offset;
    A_UINT32       dataLen;
    A_UINT8        data[1];
} POSTPACK WMIX_DSET_WRITE_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
} POSTPACK WMIX_DSET_SIZE_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
} POSTPACK WMIX_DSET_COMMIT_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
    A_UINT32       flags;
} POSTPACK WMIX_DSET_DELETE_CMD;

typedef PREPACK struct {
    A_UINT32       dset_id;
} POSTPACK WMIX_DSET_CLOSE_CMD;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
} POSTPACK WMIX_DSET_CREATE_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
} POSTPACK WMIX_DSET_OPEN_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
} POSTPACK WMIX_DSET_COMMIT_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
} POSTPACK WMIX_DSET_CLOSE_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
} POSTPACK WMIX_DSET_DELETE_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
    A_UINT32       offset;
    A_UINT32       length;
    A_UINT8        data[1];
} POSTPACK WMIX_DSET_READ_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       status;
    A_UINT32       length;
} POSTPACK WMIX_DSET_WRITE_EVENT;

typedef PREPACK struct {
    WMIX_DSET_CMD_HDR    hdr;
    A_UINT32       dset_id;
    A_UINT32       size;
    A_UINT32       status;
} POSTPACK WMIX_DSET_SIZE_EVENT;

typedef PREPACK struct {
    A_UINT32  cmd_type;   // WMI adc sub command ID, defined in adc_layer_wmi.h
    A_UINT32  length;
    A_UINT8   data[1];
} POSTPACK WMI_ADC_CMD;

typedef enum {
    EBT_MAC_FILTER_DISABLE      = 0,
    EBT_MAC_FILTER_ENABLE       = 1,
    EBT_MAC_FILTER_UNCHANGE     = 2,
} WMI_EBT_MAC_FILTER_STATE;

typedef PREPACK struct {
    A_UINT8  cmd_act; // 0 - Get, 1 - Set
    A_UINT8  ebt;
    A_UINT8  mac_filter;
} POSTPACK WMI_EBT_MAC_FILTER_CMD;

/*Define all  SET/GET Param Groups here  */
#define PARAM_GROUPID_OFFSET 16  

#define PARAM_GROUPID_MASK 0xFFFF

#define PARAM_PARAMID_MASK 0xFFFF

#define PARAM_MAKE_ID(grpid,paramid)    (grpid<<PARAM_GROUPID_OFFSET | paramid)

#define PARAM_GET_GROUP_ID(which_param)  (which_param>>PARAM_GROUPID_OFFSET && PARAM_GROUPID_MASK)

#define PARAM_GET_PARAM_ID(which_param)  (which_param && PARAM_PARAMID_MASK)



#define PARAM_GROUP_SYSTEM         1
#define PARAM_GROUP_WLAN           2
#define PARAM_GROUP_NETWORK        3
/*define new param groups here*/


#define PARAM_GROUP_WLAN_WOW_ENABLE	1
#define PARAM_GROUP_WLAN_WOW_PATTERN	2
#define PARAM_GROUP_WLAN_WOW_GPIO       3
#define PARAM_GROUP_WLAN_WOW_HOSTSLEEP	4

/* PARAM_GROUP_NETWORK Parameters */
#define PARAM_GROUP_NETWORK_DNS_TIMEOUT_SECS    1

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

#ifndef ATH_TARGET
#include "athendpack.h"
#endif
#if defined(WIN32) || defined(WIN64)
#pragma pack(pop)
#endif /* WIN32 || WIN64 */

#ifdef __cplusplus
}
#endif

#endif /* _WMI_H_ */
