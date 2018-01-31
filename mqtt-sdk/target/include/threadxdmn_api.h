/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __THREADXDMN_API_H__
#define __THREADXDMN_API_H__

#include "cdr.h"
#include "cdr_api.h"
#include "qcom/wmi_cdr.h"
//#include "threadx/tx_api.h"

#ifndef DISABLE_FUNCTION_INDIRECTION
typedef struct {
    void (*_BGR_CDR_initiate)(void *request, unsigned int req_length);
}BGR_INIT_API_INDIR_TABLE;

typedef struct {
    void (*_cdr_target_thread_initiate)(void *arg, struct cdr_req_s *req);
    void (*_cdr_target_thread_entry)(unsigned long which_proxy_thread);
    void (*_cdr_target_thread_init)(void);
    //To be added in rom
    //void (*_cdr_target_req_dispatch)(struct cdr_req_s *cdr_req);
    //void (*_cdr_target_std_done_fn)(void *arg, struct cdr_req_s *cdr_req);
}CDR_TARGET_API_INDIR_TABLE;

typedef struct {
    void (*_rtos_is_threadx)(void);
    void (*_cdr_threadx_thread_initiate)(void *arg, struct cdr_req_s *req);
    void (*_cdr_wmi_event_handler_init)(t_wmi_event_dispatch_handler cb);
    A_BOOL (*_cdr_req_handler)(struct cdr_req_s *cdr_req);
    void (*_cdr_threadx_thread_entry)(unsigned long which_proxy_thread);
    void (*_cdr_threadx_thread_init)(void);                    
}RTOS_THREADX_API_INDIR_TABLE;

typedef struct {
    void (*_std_done_fn)(void *arg, struct cdr_req_s *cdr_req);
}STD_DONE_API_INDIR_TABLE;

extern BGR_INIT_API_INDIR_TABLE bgr_init_api_indirection_table;
#define BGR_INIT_API_FN(fn) bgr_init_api_indirection_table.fn

extern CDR_TARGET_API_INDIR_TABLE cdr_target_api_indirection_table;
#define CDR_TARGET_API_FN(fn) cdr_target_api_indirection_table.fn

extern RTOS_THREADX_API_INDIR_TABLE rtos_threadx_api_indirection_table;
#define RTOS_THREADX_API_FN(fn) rtos_threadx_api_indirection_table.fn

extern STD_DONE_API_INDIR_TABLE std_done_api_indirection_table;
#define STD_DONE_API_FN(fn) std_done_api_indirection_table.fn


#else

#define BGR_INIT_API_FN(fn)      fn
#define CDR_TARGET_API_FN(fn)    fn
#define RTOS_THREADX_API_FN(fn)  fn
#define STD_DONE_API_FN(fn)      fn 

#endif

void _BGR_CDR_initiate(void *request, unsigned int req_length);
void _cdr_target_thread_initiate(void *arg, struct cdr_req_s *req);
void _cdr_target_thread_entry(unsigned long which_proxy_thread);
void _cdr_target_thread_init(void);

void _cdr_threadx_thread_init(void);
void _cdr_threadx_thread_entry(unsigned long which_proxy_thread);
A_BOOL _cdr_req_handler(struct cdr_req_s *cdr_req);
void _cdr_wmi_event_handler_init(t_wmi_event_dispatch_handler cb);
void _cdr_threadx_thread_initiate(void *arg, struct cdr_req_s *req);
void _rtos_is_threadx(void);
void _std_done_fn(void *arg, struct cdr_req_s *cdr_req);



#define BGR_CDR_initiate(request, req_length) \
  BGR_INIT_API_FN(_BGR_CDR_initiate((request), (req_length)))

#define cdr_target_thread_initiate(arg, req)  \
    CDR_TARGET_API_FN(_cdr_target_thread_initiate((arg), (req)))

#define cdr_target_thread_entry(which_proxy_thread)  \
  CDR_TARGET_API_FN(_cdr_target_thread_entry((which_proxy_thread)))

#define cdr_target_thread_init()  \
    CDR_TARGET_API_FN(_cdr_target_thread_init())

void _cdr_target_req_dispatch(struct cdr_req_s *cdr_req);
#define cdr_target_req_dispatch(req) \
    _cdr_target_req_dispatch((req))

void _cdr_target_std_done_fn(void *arg, struct cdr_req_s *cdr_req);
#define cdr_target_std_done_fn(arg, req) \
    _cdr_target_std_done_fn((arg), (req))

#define cdr_threadx_thread_init() \
 RTOS_THREADX_API_FN(_cdr_threadx_thread_init())

#define cdr_threadx_thread_entry(which_proxy_thread)  \
 RTOS_THREADX_API_FN(_cdr_threadx_thread_entry((which_proxy_thread)))

#define cdr_req_handler(cdr_req)  \
 RTOS_THREADX_API_FN(_cdr_req_handler((cdr_req)))

#define cdr_wmi_event_handler_init(cb)  \
 RTOS_THREADX_API_FN(_cdr_wmi_event_handler_init((cb)))

#define cdr_threadx_thread_initiate(arg, req)  \
 RTOS_THREADX_API_FN(_cdr_threadx_thread_initiate((arg), (req)))

#define rtos_is_threadx()  \
 RTOS_THREADX_API_FN(_rtos_is_threadx())

#define std_done_fn(arg,cdr_req)  \
 STD_DONE_API_FN(_std_done_fn((arg),(cdr_req)))

#endif
