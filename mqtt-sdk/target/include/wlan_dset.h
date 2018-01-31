/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __WLAN_DSET_H__
#define __WLAN_DSET_H__

typedef PREPACK struct wow_config_gpio{

    A_UINT32 active_low;
    A_UINT32 triggerMechanism;
    A_UINT32 gpio_pin;
	A_UINT32 gpio_enable;
} POSTPACK WOW_CONFIG_GPIO;

#endif
