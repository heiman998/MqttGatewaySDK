/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_MCAST_FILTER_H__
#define __QCOM_MCAST_FILTER_H__


A_STATUS qcom_mcast_filter_enable(A_UINT8 enable);

A_STATUS qcom_mcast_filter_add(A_UINT8 *buffer);

A_STATUS qcom_mcast_filter_del(A_UINT8 *buffer);

#endif

