/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __SELECT_API_H__
#define __SELECT_API_H__

#include "timetype.h"

#define MAX_FDSET_NUM 8
#define MAX_UART_SUPPORT 3 
#define MAX_LSCOK_SUPPORTED 4

typedef int (*UART_RX_DETECT)(int fd);

typedef int (*LSOCK_RX_DETECT)(int lsock);


typedef struct {
   unsigned long fds_bits; 
} q_fd_set;

#ifdef __cplusplus
extern "C" {
#endif



#ifdef PHOST_INDIRECT_ACCESS
typedef struct {
	void (* _QCA_FD_ZERO)(q_fd_set * q_set);
	void (* _QCA_FD_CLR)(int handle, q_fd_set *q_set);
	void (* _QCA_FD_SET)(int handle, q_fd_set *q_set);
	int  (* _QCA_FD_ISSET)(int handle, q_fd_set *q_set);
	A_UINT32 (* _socket_select_scan)(q_fd_set *read, q_fd_set *write, q_fd_set *ex, unsigned long r_mask, unsigned long w_mask, unsigned long ex_mask);
	int (* _qcom_select)(int max, q_fd_set *read, q_fd_set *write, q_fd_set * ex, struct timeval *timeout);
	void (*_uart_wakeup_select)(char* name);
}QCOM_SELECT_API_INDIRECT_TABLE;


extern QCOM_SELECT_API_INDIRECT_TABLE qcom_select_api_indirect_table;

#define QCOM_SELECT_API_FN(fn) qcom_select_api_indirect_table.fn

#else
/*
 * It is a replacement of BSD select(). The parameters are same
 *						
 * @returnnumber of active sockets
 */

void _QCA_FD_ZERO(q_fd_set * set);
void _QCA_FD_CLR(int fd, q_fd_set *set);
void _QCA_FD_SET(int fd, q_fd_set *set);
int  _QCA_FD_ISSET(int fd, q_fd_set *set);
A_UINT32 _socket_select_scan(q_fd_set *read, q_fd_set *write, q_fd_set *ex, unsigned long r_mask, unsigned long w_mask, unsigned long ex_mask);
int _qcom_select(int max, q_fd_set * in, q_fd_set * out, q_fd_set * ev, struct timeval *timeout);
void _uart_wakeup_select(char* name);

#define QCOM_SELECT_API_FN(fn) fn

#endif
void qca_fdset_clr(int fd);

int _qcom_unblock_socket(int handle);

#define qcom_unblock_socket(handle) \
	_qcom_unblock_socket(handle)

#undef FD_ZERO
#undef FD_CLR
#undef FD_SET
#undef FD_ISSET

#define FD_ZERO(fdset)  		QCOM_SELECT_API_FN(_QCA_FD_ZERO(fdset))
#define FD_CLR(fd, fdset)   	QCOM_SELECT_API_FN(_QCA_FD_CLR((fd), (fdset)))
#define FD_SET(fd, fdset)   	QCOM_SELECT_API_FN(_QCA_FD_SET((fd), (fdset)))
#define FD_ISSET(fd, fdset) 	QCOM_SELECT_API_FN(_QCA_FD_ISSET((fd), (fdset)))

#define socket_select_scan(_rd, _wr, _er, _rdm, _wrm, _erm) \
	QCOM_SELECT_API_FN(_socket_select_scan((_rd), (_wr), (_er), (_rdm), (_wrm), (_erm)))

#define qcom_select(_max, _in, _out, _er, _tmo) \
	QCOM_SELECT_API_FN(_qcom_select((_max), (_in), (_out), (_er), (_tmo)))

#define uart_wakeup_select(name) \
	QCOM_SELECT_API_FN(_uart_wakeup_select(name))

#ifdef __cplusplus
}
#endif

#endif

