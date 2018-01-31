#ifndef _T_LOG_H_
#define _T_LOG_H_

#ifdef  __cplusplus
extern "C" {
#endif

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

#include "qcom_system.h"

#include "qcom_common.h"
#include "../lt_sample/ltm3210.h"


 
//#define AF_INET  0

//extern void Delay_ARMJISHU( uint32_t nCount);
#define	  dprintf 	printf

#define   link_strlen(x)           strlen((char*)(x)) //
#define   link_strcmp(x,t)         strcmp((char*)(x),(char*)(t)) //
#define   link_strncmp(x,t,z)      strncmp((char*)(x),(char*)(t),z) //
#define   link_memset(x,d,t)       memset(x,d,t)  //


#define   link_memcpy(x,d,l)   memcpy(x,d,l)
#define   link_msleep(n)       qcom_thread_msleep(n)
#define   link_sprintf        sprintf
#define   link_snprintf        snprintf
#define   link_socket          qcom_socket
#define   link_bind             qcom_bind
#define   link_connect       qcom_connect
#define   link_setsockopt    qcom_setsockopt
#define   link_recv              qcom_recv
#define   link_recvfrom      qcom_recvfrom
#define   link_sendto          qcom_sendto
#define   link_send             qcom_send
#define   link_close(x)        qcom_socket_close(x)
#define   link_set_fd            q_fd_set
#define   link_select(a,b,c,d,e)      qcom_select(a,b,c,d,e)
extern void welcome(void);
extern int outString(const char *str);
extern int sum_add(char a,char b);

#ifdef  __cplusplus
}
#endif

#endif