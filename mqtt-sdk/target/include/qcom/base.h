/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__BASE_H__
#define __QCOM__BASE_H__

/*
 * Intended for inclusion only in pseudoHost files,
 * this pulls in some basic definitions.
 */

#include "qcom/basetypes.h"
//#include "athdefs.h"

#define PREPACK
#ifdef __GNUC__
#define POSTPACK         __attribute__ ((packed))
#else
#define POSTPACK
#endif
#define PREPACK64
#define POSTPACK64
//#include "targaddrs.h"

typedef enum {
    A_ERROR = -1,               /* Generic error return */
    A_OK = 0,                   /* success */
                                /* Following values start at 1 */
    A_DEVICE_NOT_FOUND,         /* not able to find PCI device */
    A_NO_MEMORY,                /* not able to allocate memory, not available */
    A_MEMORY_NOT_AVAIL,         /* memory region is not free for mapping */
    A_NO_FREE_DESC,             /* no free descriptors available */
    A_BAD_ADDRESS,              /* address does not match descriptor */
    A_WIN_DRIVER_ERROR,         /* used in NT_HW version, if problem at init */
    A_REGS_NOT_MAPPED,          /* registers not correctly mapped */
    A_EPERM,                    /* Not superuser */
    A_EACCES,                   /* Access denied */
    A_ENOENT,                   /* No such entry, search failed, etc. */
    A_EEXIST,                   /* The object already exists (can't create) */
    A_EFAULT,                   /* Bad address fault */
    A_EBUSY,                    /* Object is busy */
    A_EINVAL,                   /* Invalid parameter */
    A_EMSGSIZE,                 /* Inappropriate message buffer length */
    A_ECANCELED,                /* Operation canceled */
    A_ENOTSUP,                  /* Operation not supported */
    A_ECOMM,                    /* Communication error on send */
    A_EPROTO,                   /* Protocol error */
    A_ENODEV,                   /* No such device */
    A_EDEVNOTUP,                /* device is not UP */
    A_NO_RESOURCE,              /* No resources for requested operation */
    A_HARDWARE,                 /* Hardware failure */
    A_PENDING,                  /* Asynchronous routine; will send up results la
ter (typically in callback) */
    A_EBADCHANNEL,              /* The channel cannot be used */
    A_DECRYPT_ERROR,            /* Decryption error */
    A_PHY_ERROR,                /* RX PHY error */
    A_CONSUMED,                 /* Object was consumed */
    A_CLONE,                    /* The buffer is cloned */
    A_NO_HTTP_SESSION,     /* no http session available */
} A_STATUS;

#define A_SUCCESS(x)        (x == A_OK)
#define A_FAILED(x)         (!A_SUCCESS(x))

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if defined(AR6002_REV75) || defined(AR6002_REV76)
#define AR6006_HOST_INTEREST_ADDRESS                0x00420800
#else /*AR6002_REV75 || AR6002_REV76*/
#define AR6006_HOST_INTEREST_ADDRESS                0x00428800
#endif /*AR6002_REV75 || AR6002_REV76*/

#if defined(AR6002_REV7)
#define HOST_INTEREST ((struct host_interest_s *)AR6006_HOST_INTEREST_ADDRESS)
#endif

extern void *memset(void *dest, int val, unsigned int nbyte);
extern void *memcpy(void *dest, const void *src, unsigned int nbyte);
extern int   memcmp(const void *string1, const void *string2, unsigned int nbyte);

#define A_PRINTF cmnos_printf
#define A_MEMCPY          memcpy
#define A_MEMSET          memset
#define A_STRCPY          strcpy
#define A_STRLEN          strlen

extern int cmnos_printf(const char * format, ...);

#define QCOM_DEBUG_PRINTF_EN 0
#if (QCOM_DEBUG_PRINTF_EN == 0)
#define QCOM_DEBUG_PRINTF(...)
#else
#define QCOM_DEBUG_PRINTF    cmnos_printf
#endif

#endif /* __QCOM__BASE_H__ */
