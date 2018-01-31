/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __CUST_SNTP_H__
#define __CUST_SNTP_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SNTP_SERVERS         2
#define DNS_NAME_NOT_SPECIFIED   0
#define MAX_ADDR_SIZE            68

typedef struct sntp_time 
{
   int  Sec ;
   int  min ;
   int  hour ;
   int  mon ;
   int  year ;
   int  wday ;
   int  yday ;
} tSntpTime;

typedef  struct sntp_TM
{
   unsigned int tv_sec ;
   unsigned int tv_usec ;
} tSntpTM;

typedef struct sntpAddr
{
        A_UINT8 addr[MAX_ADDR_SIZE] ;
        A_UINT8 resolve ;
        A_UINT8 pad[3];
} t_SntpAddr;

typedef  struct sntp_query
{
   t_SntpAddr SntpAddr[MAX_SNTP_SERVERS] ;
}SNTP_QUERY_SRVR_ADDRESS;

#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct  {
   
 
    
void (*_qcom_enable_sntp_client)(int enable);
void (*_qcom_sntp_srvr_addr)(int flag,char* srv_addr);
void (*_qcom_sntp_zone)(int hour,int min,int add_sub,int enable);
void (*_qcom_sntp_get_time)(unsigned char device_id, tSntpTime* time);
void (*_qcom_sntp_show_config)(void);
void (*_qcom_sntp_get_time_of_day)(unsigned char device_id, tSntpTM* time);
void (*_qcom_sntp_query_srvr_address)(A_UINT8 device_id, SNTP_QUERY_SRVR_ADDRESS* addr);
}QCOM_SNTP_API_INDIRECTION_TABLE;

extern QCOM_SNTP_API_INDIRECTION_TABLE qcomSntpApiIndirectionTable;

#define QCOM_SNTP_API_FN(fn) qcomSntpApiIndirectionTable.fn

#else

#define QCOM_SNTP_API_FN(fn) fn

void _qcom_enable_sntp_client(int enable);
void _qcom_sntp_srvr_addr(int flag,char* srv_addr);
void _qcom_sntp_zone(int hour,int min,int add_sub,int enable);
void _qcom_sntp_get_time(A_UINT8 device_id, tSntpTime* time);
void _qcom_sntp_show_config();
void _qcom_sntp_get_time_of_day(A_UINT8 device_id, tSntpTM* time);
void _qcom_sntp_query_srvr_address(A_UINT8 device_id, SNTP_QUERY_SRVR_ADDRESS* addr);
#endif




#define qcom_enable_sntp_client(enable) \
    QCOM_SNTP_API_FN(_qcom_enable_sntp_client((enable)))

#define qcom_sntp_srvr_addr(flag,srv_addr) \
    QCOM_SNTP_API_FN(_qcom_sntp_srvr_addr((flag),(srv_addr)))

#define qcom_sntp_zone(hour,min,add_sub,enable) \
    QCOM_SNTP_API_FN(_qcom_sntp_zone((hour),(min),(add_sub),(enable)))

#define qcom_sntp_get_time(device_id, time) \
    QCOM_SNTP_API_FN(_qcom_sntp_get_time((device_id), (time)))

#define qcom_sntp_show_config() \
    QCOM_SNTP_API_FN(_qcom_sntp_show_config())

#define qcom_sntp_get_time_of_day(device_id, time) \
    QCOM_SNTP_API_FN(_qcom_sntp_get_time_of_day((device_id), (time)))


#ifdef AR6002_REV74

void _qcom_sntp_query_srvr_address(A_UINT8 device_id, SNTP_QUERY_SRVR_ADDRESS* addr);

#define qcom_sntp_query_srvr_address(device_id, addr)  \
   _qcom_sntp_query_srvr_address((device_id), (addr))


#else
#define qcom_sntp_query_srvr_address(device_id, addr)  \
   QCOM_SNTP_API_FN(_qcom_sntp_query_srvr_address((device_id), (addr)))

#endif

#ifdef __cplusplus
}
#endif




#endif

