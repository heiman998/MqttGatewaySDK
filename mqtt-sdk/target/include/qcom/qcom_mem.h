/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM__MEM_H__
#define __QCOM__MEM_H__

extern unsigned int qcom_mem_alloc_pool_default_sz;
#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {
    void (*_qcom_new_pool)(unsigned int size);
    
    void *(*_qcom_mem_alloc)(unsigned int size);

    void  ( *_qcom_mem_free)(void *ptr);

    void *(*_qcom_mem_calloc)(unsigned int n, unsigned int size);

    unsigned int (*_qcom_mem_remaining)(void);

    unsigned int (*_qcom_mem_heap_get_free_size)(void);

}QCOM_MEM_API_INDIRECTION_TABLE;

extern QCOM_MEM_API_INDIRECTION_TABLE qcomMemApiIndirectionTable;

#define QCOM_MEM_API_FN(fn) qcomMemApiIndirectionTable.fn

#else

#define QCOM_MEM_API_FN(fn) fn
void _qcom_new_pool(unsigned int size);

void *_qcom_mem_alloc(unsigned int size);

void _qcom_mem_free(void *ptr);

void *_qcom_mem_calloc(unsigned int n, unsigned int size);

unsigned int _qcom_mem_remaining(void);

unsigned int _qcom_mem_heap_get_free_size(void);

#endif// DISABLE_FUNCTION_INDIRECTION


#define qcom_new_pool(size) \
    QCOM_MEM_API_FN(_qcom_new_pool((size)))

#define qcom_mem_alloc(size)\
    QCOM_MEM_API_FN(_qcom_mem_alloc((size)))

#define qcom_mem_free(ptr)\
    QCOM_MEM_API_FN(_qcom_mem_free((ptr)))

#define qcom_mem_calloc(n,size)\
    QCOM_MEM_API_FN(_qcom_mem_calloc((n),(size)))

#define qcom_mem_remaining()\
    QCOM_MEM_API_FN(_qcom_mem_remaining())

#define qcom_mem_heap_get_free_size()\
    QCOM_MEM_API_FN(_qcom_mem_heap_get_free_size())


/* backwards compatibility */
#define mem_alloc(s)    qcom_mem_alloc(s)
#define mem_free(p)     qcom_mem_free(p)
#define mem_calloc(n,s) qcom_mem_calloc((n), (s))
#define mem_left()      qcom_mem_heap_get_free_size()

#endif
