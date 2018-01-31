/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#ifndef _WHAL_CONFIG_H_
#define _WHAL_CONFIG_H_

/*
 * This file determine the build configuration for the various
 * WHAL features based on the h/w support
 */
#if defined(AR6002_REV4)
#define WHAL_HW_SUPPORTS_11N                1
#define WHAL_HW_SUPPORTS_PAL_CALIBRATION    1
#define WHAL_HW_SUPPORTS_INTR_MITIGATION    1
#define WHAL_HW_SUPPORTS_MRC_CCK            0
#define WHAL_HW_SUPPORTS_EDMA               0
#define WHAL_HW_SUPPORTS_MMAC               0
#define WHAL_HW_SUPPORTS_PAPRD              1
#define WHAL_HW_SUPPORTS_LPL                0
#define WHAL_HW_SUPPORTS_ADV_RX_FILTER      1
#define WHAL_HW_SUPPORTS_RADAR_DETECTION    1
#define WHAL_HW_SUPPORTS_CHKSUM             0
#define WHAL_HW_SUPPORTS_SM_PWR_SAVE        0
#define WHAL_HW_SUPPORTS_GREEN_TX           0
#define WHAL_HW_SUPPORTS_EOL                1
#define WHAL_HW_SUPPORTS_TIME_BASED_DISCARD 0

#define WHAL_ENABLE_PAL_CALIBRATION         0
#define WHAL_ENABLE_RADAR_SUPPORT           0
#endif /* AR6002_REV4 */

#if defined(AR6002_REV6)
#define WHAL_HW_SUPPORTS_11N                1
#define WHAL_HW_SUPPORTS_PAL_CALIBRATION    1
#define WHAL_HW_SUPPORTS_INTR_MITIGATION    1
#if defined(FPGA)
#define WHAL_HW_SUPPORTS_MRC_CCK            0
#else
#define WHAL_HW_SUPPORTS_MRC_CCK            1
#endif /* FPGA */
#define WHAL_HW_SUPPORTS_EDMA               1
#define WHAL_HW_SUPPORTS_MMAC               1
#define WHAL_HW_SUPPORTS_PAPRD              1
#if defined(FPGA)
#define WHAL_HW_SUPPORTS_LPL                0
#else
#define WHAL_HW_SUPPORTS_LPL                1
#endif /* FPGA */
#define WHAL_HW_SUPPORTS_ADV_RX_FILTER      1
#define WHAL_HW_SUPPORTS_RADAR_DETECTION    1
#define WHAL_HW_SUPPORTS_CHKSUM             1
#define WHAL_HW_SUPPORTS_SM_PWR_SAVE        1
#define WHAL_HW_SUPPORTS_GREEN_TX           1
#define WHAL_HW_SUPPORTS_EOL                0
#define WHAL_HW_SUPPORTS_TIME_BASED_DISCARD 1

#define WHAL_ENABLE_PAL_CALIBRATION         0
#define WHAL_ENABLE_MMAC_SUPPORT            1
#if defined(FPGA)
#define WHAL_ENABLE_RADAR_SUPPORT           0
#else
#define WHAL_ENABLE_RADAR_SUPPORT           1
#endif /* FPGA */
#endif /* AR6002_REV6 */

#if defined(AR6002_REV7)
#define WHAL_HW_SUPPORTS_11N                1
#define WHAL_HW_SUPPORTS_PAL_CALIBRATION    1
#define WHAL_HW_SUPPORTS_INTR_MITIGATION    1
#define WHAL_HW_SUPPORTS_MRC_CCK            0
#define WHAL_HW_SUPPORTS_EDMA               1
#define WHAL_HW_SUPPORTS_MMAC               0
#define WHAL_HW_SUPPORTS_PAPRD              1
#if defined(FPGA)
#define WHAL_HW_SUPPORTS_LPL                0
#else
#define WHAL_HW_SUPPORTS_LPL                1
#endif /* FPGA */
#define WHAL_HW_SUPPORTS_ADV_RX_FILTER      1
#define WHAL_HW_SUPPORTS_RADAR_DETECTION    1
#define WHAL_HW_SUPPORTS_CHKSUM             1
#define WHAL_HW_SUPPORTS_SM_PWR_SAVE        0
#define WHAL_HW_SUPPORTS_GREEN_TX           1
#define WHAL_HW_SUPPORTS_EOL                0
#define WHAL_HW_SUPPORTS_TIME_BASED_DISCARD 1

#define WHAL_ENABLE_PAL_CALIBRATION         0
#if defined(FPGA)
#define WHAL_ENABLE_RADAR_SUPPORT           0
#else
#define WHAL_ENABLE_RADAR_SUPPORT           1
#endif /* FPGA */
#endif /* AR6002_REV7 */

/*
 * Configuration to enable 11N features in WHAL based on 
 * hardware support
 */
#if (WHAL_HW_SUPPORTS_11N)
#define CONFIG_WHAL_11N_SUPPORT     1
#else
#undef  CONFIG_WHAL_11N_SUPPORT
#endif /* WHAL_HW_SUPPORTS_11N */

/*
 * Configuration to enable PAL calibration in WHAL. Currently
 * PAL calibration is not used in any hardware
 */
#if (WHAL_HW_SUPPORTS_PAL_CALIBRATION)
#define CONFIG_WHAL_PAL_CALIBRATION  1
#if  (WHAL_ENABLE_PAL_CALIBRATION)
#define CONFIG_WHAL_PAL_CALIBRATION_ENABLE 1
#else
#undef  CONFIG_WHAL_PAL_CALIBRATION_ENABLE
#endif /* WHAL_ENABLE_PAL_CALIBRATION */ 
#else
#undef  CONFIG_WHAL_PAL_CALIBRATION
#endif /* WHAL_HW_SUPPORTS_PAL_CALIBRATION */

/*
 * Configuration to support interrupt mitigation in WHAL
 */
#if (WHAL_HW_SUPPORTS_INTR_MITIGATION)
#define CONFIG_WHAL_INTR_MITIGATION_SUPPORT 1
#else
#undef CONFIG_WHAL_INTR_MITIGATION_SUPPORT
#endif /* WHAL_HW_SUPPORTS_INTR_MITIGATION */

/*
 * Configuration to enable MRC CCK based on hardware support.
 * MRC CCK is supported only if the hardware has more than one
 * receive chains
 */
#if (WHAL_HW_SUPPORTS_MRC_CCK)
#define CONFIG_WHAL_MRC_CCK_SUPPORT 1
#else
#undef  CONFIG_WHAL_MRC_CCK_SUPPORT
#endif /* WHAL_HW_SUPPORTS_MRC_CCK */

/*
 * Configuration to enable enhanced DMA based on hardware
 * support
 */
#if (WHAL_HW_SUPPORTS_EDMA)
#define CONFIG_WHAL_EDMA_SUPPORT    1
#else
#undef  CONFIG_WHAL_EDMA_SUPPORT
#endif /* WHAL_HW_SUPPORTS_EDMA */

/*
 * Configuration to enable MMAC support in WHAL based on 
 * hardware support and s/w configuration
 */
#undef  CONFIG_WHAL_MMAC_SUPPORT

/*
 * Configuration to enable PAPRD support in WHAL
 */
#if (WHAL_HW_SUPPORTS_PAPRD)
#define CONFIG_WHAL_PAPRD_SUPPORT    1
#else
#undef  CONFIG_WHAL_PAPRD_SUPPORT
#endif /* WHAL_HW_SUPPORTS_PAPRD */

/*
 * Configuration to enable low power listen 
 */
#if (WHAL_HW_SUPPORTS_LPL)
#define CONFIG_WHAL_LPL_SUPPORT    1
#else
#undef  CONFIG_WHAL_LPL_SUPPORT
#endif /* WHAL_HW_SUPPORTS_LPL */

/*
 * Configuration to enable advanced receive filter support
 */
#if (WHAL_HW_SUPPORTS_ADV_RX_FILTER)
#define CONFIG_WHAL_ADV_RX_FILTER_SUPPORT    1
#else
#undef  CONFIG_WHAL_ADV_RX_FILTER_SUPPORT
#endif /* WHAL_HW_SUPPORT_ADV_RX_FILTER */

/*
 * Configuration to enable radar detection 
 */
#if (WHAL_HW_SUPPORTS_RADAR_DETECTION)
#define CONFIG_WHAL_RADAR_SUPPORT           1
#if (WHAL_ENABLE_RADAR_SUPPORT)
#define CONFIG_WHAL_RADAR_ENABLE 1
#else
#undef  CONFIG_WHAL_RADAR_ENABLE
#endif /* WHAL_ENABLE_RADAR_SUPPORT */
#else
#undef  CONFIG_WHAL_RADAR_SUPPORT
#endif /* WHAL_HW_SUPPORTS_RADAR_DETECTION */

/*
 * Configuration to enable checksum support 
 */
#if (WHAL_HW_SUPPORTS_CHKSUM)
#define CONFIG_WHAL_CHKSUM_SUPPORT    1
#else
#undef  CONFIG_WHAL_CHKSUM_SUPPORT
#endif /* WHAL_HW_SUPPORTS_CHKSUM */

/*
 * Confinguration to enable SM power save
 */
#if (WHAL_HW_SUPPORTS_SM_PWR_SAVE)
#define CONFIG_WHAL_SM_PWR_SAVE_SUPPORT 1
#else
#undef  CONFIG_WHAL_SM_PWR_SAVE_SUPPORT
#endif /* WHAL_HW_SUPPORTS_SM_PWR_SAVE */

/*
 * Confinguration to enable Green TX
 */
#if (WHAL_HW_SUPPORTS_GREEN_TX)
#define CONFIG_WHAL_GREEN_TX_SUPPORT 1
#else
#undef  CONFIG_WHAL_GREEN_TX_SUPPORT
#endif /* WHAL_HW_SUPPORTS_GREEN_TX */

/*
 * Confiugration to enable End Of Life (EOL)
 */
#if (WHAL_HW_SUPPORTS_EOL)
#define CONFIG_WHAL_EOL_SUPPORT    1
#else
#undef  CONFIG_WHAL_EOL_SUPPORT
#endif /* WHAL_HW_SUPPORTS_EOL */

/*
 * Confiugration to enable time based discard
 */
#if (WHAL_HW_SUPPORTS_TIME_BASED_DISCARD)
#define CONFIG_WHAL_TIME_BASED_DISCARD_SUPPORT    1
#else
#undef  CONFIG_WHAL_TIME_BASED_DISCARD_SUPPORT
#endif /* WHAL_HW_SUPPORTS_TIME_BASED_DISCARD */

#endif /* _WHAL_CONFIG_H_ */
