/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __SELECT_INTERNAL_H__
#define __SELECT_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* the definitions to support the select() function. These are about 
 *  as UNIX-like as we can make 'em on embedded code. They are also 
 *  fairly compatable with WinSock's select() definitions.
 **/
#ifndef FD_SETSIZE
#define  FD_SETSIZE     12
#endif

#define add_cb_to_cblist(cblist , cb) \
do {    \
    if(!cblist){ \
        cblist = cb; \
    }else{ \
        cblist->prev = cb; \
        cb->next = cblist; \
        cblist = cb; \
    } \
}while(0)


#define del_cb_from_cblist(cblist , cb) \
do { \
    if(cb != cblist){ \
        cb->prev->next = cb->next; \
    }else{ \
        cblist = cb->next;  \
    }\
    if(cb->next != NULL){ \
        cb->next->prev = cb->prev; \
    } \
}while(0)

typedef struct   /* the select socket array manager */
{ 
    unsigned fd_count;               /* how many are SET? */
    long     fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;

typedef struct select_cb{
    unsigned int cmd;		
    struct select_cb *next;
    struct select_cb *prev;
    fd_set  ibits[3];
    fd_set  obits[3];
    int ret;
    int acticve_sowakeup;
    struct cdr_req_s* cdr_req;
    void* timer;
    int time_out;
}select_cb_t;

void _ifd_clr(long so, fd_set *set);
void _ifd_set(long so, fd_set *set);
int  _ifd_isset(long so, fd_set *set);
long _ifd_get(unsigned i, fd_set *set);
int _sock_selscan(fd_set * ibits, fd_set * obits);
int _sock_select(long sock, int flag);

#ifndef DISABLE_FUNCTION_INDIRECTION
typedef struct soselect_int_api
{
    int (* _sock_selscan)(fd_set * ibits,fd_set * obits);
    int (* _sock_select)(long sock,int flag);
    void (* _ifd_clr)(long so, fd_set *set);
    void (* _ifd_set)(long so, fd_set *set);
    int  (* _ifd_isset)(long so, fd_set *set);
    long (* _ifd_get)(unsigned i, fd_set *set);
} SOSELECT_INT_API;
extern SOSELECT_INT_API soselect_internal_api;
#define SOSELECT_CMN_FN(fn) soselect_internal_api.fn

#else
#define SOSELECT_CMN_FN(fn) fn
#endif

#define sock_selscan(ibits,obits)	\
    SOSELECT_CMN_FN(_sock_selscan((ibits),(obits)))

#define sock_select(sock,flag)	\
    SOSELECT_CMN_FN(_sock_select((sock),(flag)))

#define ifd_clr(so,set)	\
    SOSELECT_CMN_FN(_ifd_clr((so),(set)))

#define ifd_set(so,set)	\
    SOSELECT_CMN_FN(_ifd_set((so),(set)))

#define ifd_isset(so,set)	\
    SOSELECT_CMN_FN(_ifd_isset((so),(set)))

#define ifd_get(i,set)	\
    SOSELECT_CMN_FN(_ifd_get((i),(set)))

#define  FD_ZERO(set)         (((fd_set *)(set))->fd_count = 0)
#define  FD_CLR(so, set)       ifd_clr((long)(so), (fd_set *)(set))
#define  FD_SET(so, set)       ifd_set((long)(so), (fd_set *)(set))
#define  FD_ISSET(so, set)     ifd_isset((long)(so), (fd_set *)(set))

void cdr_select_request_handle(struct cdr_req_s* cdr_req);

#ifdef __cplusplus
}
#endif

#endif

