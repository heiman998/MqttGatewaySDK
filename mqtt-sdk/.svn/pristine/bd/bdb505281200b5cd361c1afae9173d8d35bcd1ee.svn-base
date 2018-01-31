/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#ifndef __CDR_API_H__
#define __CDR_API_H__

/* Target domain cross-domain request API */

#include "cdr.h"

struct cdr_api_s {
    void (* _cdr_init)(void);
    int (* _cdr_initiate)(struct cdr_info_s *cdr_info, struct cdr_req_s *cdr_req); 
    void(* _cdr_completed)(struct cdr_req_s *cdr_req);
};

#define cdr_init() cdr_api._cdr_init()
#define cdr_initiate(cdr_info, cdr_req) cdr_api._cdr_initiate((cdr_info), (cdr_req))
#define cdr_completed(cdr_req) cdr_api._cdr_completed(cdr_req)

extern struct cdr_api_s cdr_api;

extern void cdr_module_install(struct cdr_api_s *);



/***************************************
***     the following are for indirect calls    ***
****************************************/


typedef struct  {
	void (* _qcom_ready)(void);

}QCOM_READY_API_INDIRECTION_TABLE;

extern QCOM_READY_API_INDIRECTION_TABLE qcomReadytApiIndirectionTable;
#define qcom_ready() qcomReadytApiIndirectionTable._qcom_ready()



#endif /* __CDR_API_H__ */
