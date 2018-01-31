/*
 * Copyright (c) 2011,2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __DSET_INTERNAL_H__
#define __DSET_INTERNAL_H__

#ifndef ATH_TARGET
#include "athstartpack.h"
#endif

/*
 * Internal memory-based DataSet definitions.
 * TBD: Utilities that need these definitions
 * should be moved from host side to target/support.
 */

#define DSET_TYPE_STANDARD      0
#define DSET_TYPE_BPATCHED      1
#define DSET_TYPE_COMPRESSED    2

/* Dataset descriptor */

typedef PREPACK struct dset_descriptor_s {
  struct dset_descriptor_s  *next;         /* List link. NULL only at the last
                                              descriptor */
  A_UINT16                   id;           /* Dset ID */
  A_UINT16                   size;         /* Dset size. */
  void                      *DataPtr;      /* Pointer to raw data for standard
                                              DataSet or pointer to original
                                              dset_descriptor for patched
                                              DataSet */
  A_UINT32                   data_type;    /* DSET_TYPE_*, above */

  void                      *AuxPtr;       /* Additional data that might
                                              needed for data_type. For
                                              example, pointer to patch
                                              Dataset descriptor for BPatch. */
} POSTPACK dset_descriptor_t;

#ifndef ATH_TARGET
#include "athendpack.h"
#endif

#endif /* __DSET_INTERNAL_H__ */
