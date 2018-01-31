/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#ifndef __FW_CONFIG_H__
#define __FW_CONFIG_H__

/* AR6006 Firmware Configuration */
#define OCS_NOA_SUPPORT 1
#define WLAN_HEADERS 1
#define SUPPORT_11N 1
#define CONFIG_MCC_PROFILE  1

#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76) || defined(FPGA)
#undef CONFIG_WLAN_BTCOEX
#undef CONFIG_WLAN_APMODE_BTCOEX
#undef CONFIG_WLAN_BTCOEX_DASB
#undef WLAN_CONFIG_CCX 
#undef WLAN_LOCATION_SUPPORT 
#undef CONFIG_WLAN_WOW 
#undef CONFIG_WOW_EXT
#undef WLAN_CONFIG_RRM
#else
#define WLAN_CONFIG_RRM 1
#define CONFIG_WLAN_BTCOEX 1
#define CONFIG_WLAN_APMODE_BTCOEX 1

/*RTT SUPPORT ENABLED*/
#define WLAN_LOCATION_SUPPORT 1
#define CONFIG_WLAN_WOW 1
#define CONFIG_WOW_EXT 1
//#define CONFIG_WLAN_BTCOEX_DASB 1 // this feature is only needed for 2X2 radio
#endif

#define CONFIG_WLAN_AP 1
#define CONFIG_EEPROM 1
#define CONFIG_WLAN_IBSS 1
#define CONFIG_WLAN_ARPOFF 1
#define WLAN_CONFIG_MCAST_FILTER 1
#define SECURITY 1
#define BUFPOOL 1
#define BUFPOOL_TX 1
#define BUFPOOL_RX 1
#define CONFIG_WLAN_LOWRSSI_RO_SCAN 1
#define CHECKSUM_OFFLOAD /* requires meta support */
#define TIME_BASED_DISCARD 1
#define USE_PM_INDIRECTION_TABLE 1
//#define PAL_BT30 1
#define AUTHENTICATOR 1
#define CONFIG_WLAN_80211H 1
#define CONFIG_ADHOC_PS_ATH 1
#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76) || defined(FPGA)
#undef CONFIG_WLAN_BTCOEX_ENABLED_ROLE_FINDING 
#undef BTCOEX_DEBUG 
#undef CONFIG_WOW_EXT
#else
#define CONFIG_WLAN_BTCOEX_ENABLED_ROLE_FINDING 1
#define BTCOEX_DEBUG 1
/* make WOW extension available in the ROM */
#define CONFIG_WOW_EXT
#endif
#define TXRX_MGMTBUF_DEBUG 1
#define CM_DEBUG 1
#define CONFIG_AP_POWER_SAVE 1
#define WAPI_ENABLE 1
#define PM_DEBUG 1
#define DC_DEBUG 1
#define CO_DEBUG 1
#define RO_DEBUG 1
#define WMI_DEBUG 1
#define MLME_DEBUG 1
#define KEYMGMT_DEBUG 1
#define TX_TEMPERATURE_THROTTLE 1
#define ATH_SUPPORT_DFS 1
#define  P2P 1 
#define DEBUG_PKTLOG 1
//#define SUPPORT_2SS 1             /* AR6006 is 1x1 */
#define SUPPORT_EDMA 1
#define SUPPORT_EXT_11N 1
#define SPLIT_RTC 1
//#define AR6004_1_0_ALIGN_WAR 0
#define WAR_DUP_DESC_ID 1
#define STA_OFFLOAD 1
#define REPORT_ASSOC_REQ 1
//#define HOSTPAL_SUPPORT 1
#define CONFIG_MULTIIO_SPI_IFACE 1

#if !defined(CONFIG_WLAN_STORE_RECALL)

#if defined(AR6002_REV74) 
#define CONFIG_WLAN_STORE_RECALL 1
#else
#define CONFIG_WLAN_STORE_RECALL 2
#endif

#endif

#if !defined(CONFIG_GPIO)

#if defined(AR6002_REV74)
#define CONFIG_GPIO 1
#else
#define CONFIG_GPIO 2
#endif

#endif

#define CONFIG_POWER_SAFE_RAM 1
#define CONFIG_WAKEUP_MGR 1
#define CONFIG_WPS_OFFLOAD 1
#define WPA_OFFLOAD 1
#define USE_WPS_INDIRECTION_TABLE 1

#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
#define CONFIG_TUNABLE 1
#define PHOST_INDIRECT_ACCESS
#define LIBCUST_INDIRECT_ACCESS
/*WNM Feature set support*/
#define CONFIG_WNM 1
#else
#undef CONFIG_TUNABLE
#undef PHOST_INDIRECT_ACCESS
#undef LIBCUST_INDIRECT_ACCESS
#undef CONFIG_WNM
#endif

#if !defined(FPGA)
#define CONFIG_LPL 1
#endif

#define IOT_NON_ARGOS_BUILD_FLAG


#define CONFIG_GREENTX 1
#define CONFIG_IP_OFFLOAD 1


#define CONFIG_P2P_FW_OFFLOAD 1
#define USE_P2P_FW_INDIRECTION_TABLE 1


#if !defined(FPGA)
#define PSBUFF_OFFLOAD_SUPPORT 1
#define HOST_TX_SCHEDULER 1
#define ATH6KL_DIAGNOSTIC 1
#define HID_BTCOEX 1

#else /* !defined(FPGA) */
#if 0
#undef PSBUFF_OFFLOAD_SUPPORT
#undef HOST_TX_SCHEDULER 
#else
#define PSBUFF_OFFLOAD_SUPPORT 1
#define HOST_TX_SCHEDULER 1
#endif 

#undef ATH6KL_DIAGNOSTIC 
#undef HID_BTCOEX 
#endif /* !defined(FPGA) */



#define CONFIG_BACKGROUND_SERVICES 1
#define CONFIG_OCAPP 1 /* On Chip Application <aka hostless> */

/* ALWAYS-CONNECTED SERVICE components */
//#define CONFIG_ACS 1
//#define CONFIG_WWAN_STE 1
//#define ACS_3G_RTC_RESET_WAR 1
//#define USE_WWAN_MODEM_RECOVERY 1

/* console (CLI) infrastructure */
//#define CONFIG_CLI 1
//#define VAPMGR_CONSOLE 1

/* rev6 supports BMI over transports other than the mailbox */
#define CONFIG_USE_MULTI_TRANSPORT_BMI 1

/* Format strings used in PRINTF calls are stored in unpatchable space (in ROM) */
#define CONFIG_FMTSTR_UNPATCHABLE 1

/* USB supported as a Host/Target interconnect, including BMI over USB */
#define CONFIG_BMI_USB 1

/* To push a target assert pattern to host by TX PIPE 2 when target is asserted */
#define CONFIG_BMI_USB_TARGET_ASSERT_NOTIFY 1

/*I2C SLAVE_MODULE*/
#define I2C_SLAVE  0
/*dual chain support*/
#define ENABLE_DUAL_CHAINS 1

#if defined(AR6002_REV75) || defined(AR6002_REV76)
#define CONFIG_SEC_BOOT 1
#endif

/* USB use single data pipe */
#define USB_HIF_USE_SINGLE_PIPE_FOR_DATA 1

/*Enable AMSDU slicing*/
#define AMSDU_SLICING_OFFLOADED 1

/* build rom with ratecontrol wmi */
#define ROM_WMI_RATECONTRL 1

#if defined(FPGA) || defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
/* Insufficient room in ROM */
#undef CONFIG_TARGET_PROFILE_SUPPORT
#else
#define CONFIG_TARGET_PROFILE_SUPPORT 1
#endif

#define CONFIG_NLO 1
#define CONFIG_RX_COALESCE_FILTER 1
#define CONFIG_RX_FILTER 1
#define CONFIG_WLAN_RFKILL 1

#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
#undef CONFIG_WLAN_LED
#else
#define CONFIG_WLAN_LED 1
#endif

#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
#define CONFIG_EFUSE_CACHE_SUPPORT 1
#else
#undef CONFIG_EFUSE_CACHE_SUPPORT
#endif

#define CONFIG_TRANSPORT_PM 1
//#define CONFIG_WLAN_BTCOEX_MCI 1
//#define DEBUG_MCCI_TEST 1
/* CONFIG_WLAN_BTCOEX_MCI CONFIG_WLAN_BTCOEX_MCI_TEST DEBUG_MCI_TEST */
/* EV_81823_FIXED */
#define CONFIG_BTCOEX_ENABLE_GPIO_DEBUG 1
#define I2S_SUPPORT 1
#define AH_ASSERT 1

/* Support 802.11w */
#define SUPPORT_11W 1

/* Support PAUSE COMBINATION */
#define PAUSE_COM_SUPPORT 1

/* Support sw-retry for non-aggregation unicast data frames */
#define NON_AGGR_SW_RETRY_SUPPORT 1


/* Enable off channel scheduler */
#define CONFIG_CHANNEL_SCHEDULER 1

#define GPIO_DEFAULT_PULL 1

/*support LPM
* BESL threshold is defined according to LPM ECN Errata which change the HIRD to BESL.
* The range of BESL is 0~15 each related to a resume signal time driver by host.
* 0:125, 1:150, 2:200, 3:300, 4:400, 5:500, 6:1000, 7:2000, 8:3000, 9:4000, 10:5000
* 11:6000, 12:7000, 13:8000, 14:9000, 15:10000
**/
#define CONFIG_LPM_SUPPORT 1
#define CONFIG_LPM_BESL_THRESHOLD 2

/* hcibridge support */
#define CONFIG_HCIBRIDGE 1

/* malloc support */
#define CONFIG_MALLOC 1

#define P2P_NO_DEDICATE 1

/*Enable WDT reset before usb host appear.*/
#define ENABLE_WDT_BEFORE_USB_HOST_CONTACT   1
/*warkaround for usb enumeration failed during host cold boot*/
#define WAR_USB_ENUM_FAILED_IN_COLDBOOT  1

#define CONFIG_INSTRUMENTATION           1

#if defined(AR6002_REV72) || defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
/* Enable DataSet Write Extensions */
#define CONFIG_DSET_XTNS 1
#endif

#define CONFIG_CDR 1   /* Domains with Cross Domain Requests */

#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76) || defined(REV74_TEST_ENV1)
/* Include the "top half" of the ipstack, used with hostless */
#define CONFIG_IPSTACK_TOP 1
#else
#undef CONFIG_IPSTACK_TOP
#endif

#if defined (AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)
#define CONFIG_EARLY_CLOCK_SETUP 1
#endif

#if defined(AR6002_REV72)|| defined(FPGA)
#undef  CONFIG_EARLY_CLOCK_SETUP
#endif

/* Space for use by allocram and malloc during startup */
#if defined(AR6002_REV74) 
#define EARLY_ALLOCRAM_ARENA_SZ (16*1024)
#elif defined(AR6002_REV75) || defined(AR6002_REV76)
#define EARLY_ALLOCRAM_ARENA_SZ (32*1024)
#else
#undef EARLY_ALLOCRAM_ARENA_SZ
#endif

#if defined(EARLY_ALLOCRAM_ARENA_SZ)
#define CONFIG_EARLY_ALLOCRAM_ARENA 1
#else
#undef CONFIG_EARLY_ALLOCRAM_ARENA
#endif

/* LF Timer workarounds */
#define CONFIG_STALE_LF_TIMER_WAR   1
#define CONFIG_NOSLEEP_NEAR_TIMER_WAR 1
#define CONFIG_TIMER_TOO_CLOSE_WAR 1

/* NOT currently used */
#undef DISABLE_CMNOS_INDIRECTION 
#undef DISABLE_FUNCTION_INDIRECTION
#undef DISABLE_ASSERT 
#undef DISABLE_PRINTF 
#undef CONFIG_INIT_TEXT
#undef COMPUTE_BEACON_MISS
#undef COPY_FLAT_BUFFER
#undef DISABLE_THINMODE_SUPPORT
#undef DISABLE_RATE_INDIRECTION_TABLE
#undef TXRX_TXBUF_DEBUG
#undef TXRX_RXBUF_DEBUG
#undef SUPPORT_2SS
#undef CONFIG_WLAN_BTCOEX_DASB

/* Include WHAL module configuration */
#include "whal/whal_config.h"

#if defined(REV74_BOOTROMTEST_ENV)
#undef CONFIG_WLAN_LED
#undef CONFIG_WLAN_RFKILL
#endif

/* There are a couple of options when ED25519 is used. 
 * 1. When USE_BIG_TABLE is defined, a table of 30720 bytes is used 
 * 2. When USE_MIDDLE_TABLE is defined, a table of 7680 bytes is used and sign
 * generation time suffers a 3% penalty compared to option 1.
 * 3. When neither USE_BIG_TABLE nor USE_MIDDLE_TABLE is defined, no table is
 * used. However there is a speed penalty of 2.5X compared to option 1
 *
 * Adding "UNPATCHABLE" will move the const data to ROM. So option 1 which is
 * the fastest is enabled by default
 */
#define CONFIG_ED25519_USE_BIG_TABLE 

#undef CONFIG_ED25519_USE_MIDDLE_TABLE 

#if (defined(CONFIG_ED25519_USE_BIG_TABLE) && defined(CONFIG_ED25519_USE_MIDDLE_TABLE))
#error "Cannot define both CONFIG_ED25519_USE_BIG_TABLE and CONFIG_ED25519_USE_MIDDLE_TABLE!"
#endif

/* Not defining USE_SMALL_TABLE saves 960 bytes of data without any impact to
 * speed. This can be used independently of USE_BIG_TABLE and USE_MIDDLE_TABLE
 */
#undef CONFIG_ED25519_USE_SMALL_TABLE 
#endif /* __FW_CONFIG_H__ */
