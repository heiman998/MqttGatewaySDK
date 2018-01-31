/*
 * xlink_stm32f107_lwip.h
 *
 *  Created on: 2015骞�9鏈�6鏃�
 *      Author: XT800
 */

#ifndef XLINK_ALL_DEVICE_H_
#define XLINK_ALL_DEVICE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "qcom_system.h"

#include "qcom_common.h"
#include "../lt_sample/ltm3210.h"


#define  XLINK_FUNC  
//#define AF_INET  0

//extern void Delay_ARMJISHU( uint32_t nCount);

#define   xlink_strlen(x)           strlen((char*)(x)) //
#define   xlink_strcmp(x,t)         strcmp((char*)(x),(char*)(t)) //
#define   xlink_strncmp(x,t,z)      strncmp((char*)(x),(char*)(t),z) //
#define   xlink_memset(x,d,t)       memset(x,d,t)  //


#define   xlink_memcpy(x,d,l)   memcpy(x,d,l)
#define   xlink_msleep(n)       qcom_thread_msleep(n)
#define   xlink_sprintf        sprintf
#define   xlink_snprintf        snprintf
#define   xlink_socket          qcom_socket
#define   xlink_bind             qcom_bind
#define   xlink_connect       qcom_connect
#define   xlink_setsockopt    qcom_setsockopt
#define   xlink_recv              qcom_recv
#define   xlink_recvfrom      qcom_recvfrom
#define   xlink_sendto          qcom_sendto
#define   xlink_send             qcom_send
#define   xlink_close(x)        qcom_socket_close(x)
#define   xlink_set_fd            q_fd_set
#define   xlink_select(a,b,c,d,e)      qcom_select(a,b,c,d,e)



//typedef uint32_t xsdk_time_t;
typedef  unsigned int xsdk_time_t;//old


struct in_address {
	unsigned int s_addr;
};

typedef union {
	unsigned int ip;
	struct {
		unsigned char byte0 :8;
		unsigned char byte1 :8;
		unsigned char byte2 :8;
		unsigned char byte3 :8;
	}bit;
}IPAddr;

#if 1
struct _sockaddr_in {
	//uip_ipaddr_t  addr;
	int addr;
	int socket;
	unsigned short sin_port;
	unsigned int sin_family;
	struct in_address sin_addr;
};
#else
struct sockaddr_in {
	void *data;
	unsigned short sin_port;
	unsigned int sin_family;
	struct in_address sin_addr;
};
#endif


typedef struct _sockaddr_in xlink_addr;



extern XLINK_FUNC void xclose(int s);
extern void xlink_delay(xsdk_time_t x);



#ifdef  __cplusplus
}
#endif
#endif /* XLINK_ALL_DEVICE_H_ */

