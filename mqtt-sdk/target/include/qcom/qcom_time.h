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
 
#ifndef __CUST_TIME_H__
#define __CUST_TIME_H__

#include "qcom/timetype.h"

#define IP_MS2TICK(a)  ((a+125-1)/125)     /* TPS=8, IP STACK kernel timer is 125mS */
#define IP_TICK2MS(a)  (a * 125)

#if 0
/**
 * This function is to return the seconds from power-on to now. It is compatible with standard C
 * function time() but the latter returns the seconds from EPOCH to now.
 *
 * @param cur	C pointer to save the return value
 * @return		The seconds from power-on to now
 */
time_t time(time_t *cur);
char *ctime(const time_t *timep);
/**
 * This function is to return the milliseconds from power-on to now.
 *
 * @return		The milliseconds from power-on to now
 */
unsigned long time_ms ();
/**
 * This function is to delay up to 1000uS in polling mode.
 *
 * @param us		micro seconds to delay
 */
void us_delay(int us);
#endif


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct
{
time_t
(* _time)(time_t * cur_time);
char *
(* _ctime)(const time_t * timep);
unsigned long
(* _time_ms)();
void
(* _us_delay)(int us);



}QCOM_TIME_API_INDIRECTION_TABLE;


extern QCOM_TIME_API_INDIRECTION_TABLE qcomTimeApiIndirectionTable;
#define QCOM_TIME_API_FN(fn) qcomTimeApiIndirectionTable.fn

#else


time_t
_time(time_t * cur_time);
char *
_ctime(const time_t * timep);
unsigned long
_time_ms();
void
_us_delay(int us);


#define QCOM_TIME_API_FN(fn) fn

#endif


#define time(time) \
	QCOM_TIME_API_FN(_time(time))
#define ctime(time) \
	QCOM_TIME_API_FN(_ctime(time))
#define time_ms() \
	QCOM_TIME_API_FN(_time_ms())
#define us_delay(time) \
	QCOM_TIME_API_FN(_us_delay(time))







#endif //__CUST_TIME_H__
