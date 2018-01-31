/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef _CDR_H_
#define _CDR_H_

/*
 * Support for Cross-Domain Requests.
 *
 * Permits action to be requested by software running in one domain
 * and serviced by software running in another domain.  For instance,
 * pseudo-Host software may request action from the Target domain or
 * Target domain software may request action from the pseudo-Host.
 */


#define CDR_PACKET_TYPE_WMI     0x0001
#define CDR_PACKET_TYPE_DATA    0x0002

/*
 * Linked list of Cross Domain Requests associated with a
 * particular handler.  A handler services one type of 
 * requests.  For instance:
 *   WMI BGR request handler in the Target domain
 *   Threaded handler in the Target domain
 */
struct cdr_req_s {
    /* linkage for internal-use only -- linked list of requests */
    struct cdr_req_s *next;

    void *req_block; /* Pointer to Request Block and size of Request Block */
    A_UINT16 req_sz;

    A_UINT16 req_flags; /* CDR handler-specific flags, CDR_REQ_FLAG_* */

    /* Completion function/arg. Completion semantics are handler-specific */
    void (* req_completed)(void *completed_arg, struct cdr_req_s *);
    void *completed_arg;
};

/* CDR handler info, one per handler.  */
struct cdr_info_s {
    /* Request queue for this handler */
    struct cdr_req_s *pending_head;
    struct cdr_req_s *pending_tail;

    /*
     * Pointers to domain functions to be called
     * when a new request arrives.
     */
    struct cdr_req_s *(* prepare_fn)(void *cdr_handler_arg, struct cdr_req_s **);
    void (* initiate_fn)(void *cdr_handler_arg, struct cdr_req_s *);
    void *cdr_handler_arg;
};

/* One-time initialization of CDR infrastructure */
extern void cdr_init(void);

/*
 * Initiate a cross-domain request .
 *      cdr_info tells which domain/handler should service this request
 *      cdr_req is the request
 */
extern int cdr_initiate(struct cdr_info_s *cdr_info, struct cdr_req_s *cdr_req);

/* Indicate request completion to the domain that originated the reqeust. */
extern void cdr_completed(struct cdr_req_s *cdr_req);

/*
 * Register a CDR handler.
 *      cdr_info: pointer to pre-allocated
 *      prepare_fn: fn that validates/modifies the incoming request
 *      initiate_fn: fn that enqueues a request and triggers an action
 *      cdr_handler_arg: caller-supplied arg passed to prepare_fn and initiate_fn
 */
extern void
cdr_handler_register(struct cdr_info_s *cdr_info,
                     struct cdr_req_s *(* prepare_fn)(void *cdr_handler_arg, struct cdr_req_s **),
                     void (* initiate_fn)(void *cdr_handler_arg, struct cdr_req_s *),
                     void *cdr_handler_arg);

/* Fetch the next request in the handler's queue. */
extern struct cdr_req_s *
cdr_req_next(struct cdr_info_s * cdr_info);


                /* Generic BGR handler in Target domain */
extern struct cdr_info_s *cdr_bgr_handler;
extern void cdr_bgr_init(void);


                /* WMI BGR handler in Target domain */
extern struct cdr_info_s *cdr_wmi_handler;
extern void cdr_wmi_init(void);

/* WMI header for Cross-Domain Requests to WMI handler */
typedef struct {
    A_UINT16 commandId;
    A_UINT16 info1;
} WMI_CDR_HDR;

                /* ThreadX thread handler in Target domain */
extern struct cdr_info_s *cdr_target_thread_handler;
extern void cdr_target_thread_init(void);

extern struct cdr_info_s *cdr_timer_thread_handler;

                /* cdr handler in threadx domain */
extern struct cdr_info_s *cdr_threadx_handler;

/* cdr target specific --Start */
typedef enum {
    CDR_TARGET_WPS_CMD = 0,
    CDR_TARGET_PMK_CMD = 1,
    CDR_TARGET_WIFI_CONN_CMD = 2, 
} CDR_TARGET_THREAD_CMD;

typedef struct target_cdr_hdr_s {
    unsigned int cmd;
    unsigned int devid;
    void (* prev_req_completed)(void *completed_arg, struct cdr_req_s *);
    void *prev_completed_arg;
    struct cdr_req_s *prev_req;
} TARGET_CDR_HDR;

/* cdr target: wps --Start */
typedef enum {
    CDR_TARGET_WPS_INIT_SUBCMD = 0,
    CDR_TARGET_WPS_START_SUBCMD = 1,
    CDR_TARGET_WPS_EAP_THREAD_SUBCMD = 2,
} CDR_TARGET_WPS_SUBCMD;

typedef struct _cdr_target_wps_hdr {
    struct target_cdr_hdr_s hdr;
    A_UINT32 subcmd;
} cdr_target_wps_hdr;

typedef cdr_target_wps_hdr wps_init_cmd;

typedef struct _wps_start_cmd {
    cdr_target_wps_hdr wpshdr;
    void *start_info;
} wps_start_cmd;

typedef cdr_target_wps_hdr wps_eap_thread_cmd;

typedef struct _cdr_target_wps {
    union {
        wps_init_cmd wps_init;
        wps_start_cmd wps_start;
        wps_eap_thread_cmd wps_eap_thread;
    } u;
} cdr_target_wps;
/* cdr target: wps --End */

/* cdr target: pmk --Start */
typedef struct _cdr_target_pmk {
    struct target_cdr_hdr_s hdr;
    int iterations;
    A_UINT32 buflen;
    A_UINT8 *buf;
    char ssid[32];
    char passphrase[64];
    A_UINT8 ssid_len;
    A_UINT8 passphrase_len;
} cdr_target_pmk;
/* cdr target: pmk --End */

/* cdr target: wifi connect --Start */
typedef struct _cdr_target_wifi_conn {
    struct target_cdr_hdr_s hdr;
    A_UINT8 devid;
} cdr_target_wifi_conn;

typedef struct _cdr_target_cmd {
    union {
        cdr_target_wps wps;
        cdr_target_pmk pmk;
        cdr_target_wifi_conn wifi_conn;
    } u;
} cdr_target_cmd;
/* cdr target specific --End */

#endif /*_CDR_H_ */
