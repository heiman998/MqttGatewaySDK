/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 ******************************************************************************/

#ifndef __QCOM_TIMER_H__
#define __QCOM_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define A_MS_PER_SECOND      1000         /* Milliseconds */
#define A_US_PER_SECOND      1000000      /* Microseconds */
#define A_NS_PER_SECOND      1000000000   /* Nanoseconds  */

/* Timer API */
typedef enum {
    ONESHOT = 0,
    PERIODIC = 1,
} qcom_timer_type_e;

typedef struct {
    unsigned int timer[6];
    int timeout;
    int type;
    unsigned int arg;
} qcom_timer_t;

#if 0
/**
 * This function is used to initialize a timer.
 * @param qtimer                pointer to qcom_timer_t, used to save internal data of timer.
 * @param fn                    call back function for the timer.
 * @param timeout               timeout value for the timer, unit in milliseconds.
 * @param type                  ONESHOT or PERIODIC.
 * @return                      0 on success. else on error.
 */
int qcom_timer_init(qcom_timer_t* qtimer, void (*fn)(unsigned int, void *), void* arg, int timeout, qcom_timer_type_e type);
/**
 * This function is used to start a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_start(qcom_timer_t* qtimer);
/**
 * This function is used to stop a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_stop(qcom_timer_t* qtimer);
/**
 * This function is used to delete a timer.
 * @param qtimer                pointer to qcom_timer_t returned from qcom_timer_init.
 * @return                      0 on success. else on error.
 */
int qcom_timer_delete(qcom_timer_t* qtimer);
#endif


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct 
{
int (* _qcom_timer_init)(qcom_timer_t * qtimer, void (*fn) (unsigned int, void *), void *arg,
                    int timeout, qcom_timer_type_e type);
int (* _qcom_timer_start)(qcom_timer_t * qtimer);
int (* _qcom_timer_stop)(qcom_timer_t * qtimer);
int (* _qcom_timer_delete)(qcom_timer_t * qtimer);
void (* _sleep_timeout)(unsigned int nouse, void *data);
void (* _qcom_thread_msleep)(unsigned long ms);
// To be added in rom
//A_UINT32 (*_qcom_yield)();
}QCOM_TIMER_API_INDIRECTION_TABLE;


extern QCOM_TIMER_API_INDIRECTION_TABLE qcomTimerApiIndirectionTable;
#define QCOM_TIMER_API_FN(fn) qcomTimerApiIndirectionTable.fn

#else



int _qcom_timer_init(qcom_timer_t * qtimer, void (*fn) (unsigned int, void *), void *arg,
                    int timeout, qcom_timer_type_e type);
int _qcom_timer_start(qcom_timer_t * qtimer);
int _qcom_timer_stop(qcom_timer_t * qtimer);
int _qcom_timer_delete(qcom_timer_t * qtimer);
void _sleep_timeout(unsigned int nouse, void *data);
void _qcom_thread_msleep(unsigned long ms);
A_UINT32 _qcom_yield();
int _qcom_timer_us_start(qcom_timer_t * qtimer);
#define QCOM_TIMER_API_FN(fn) fn

/*Real-time timer which has high priority, TBD for ROM support*/
A_UINT32 _qcom_isr_timer_start(void (*fn) (void *), void *arg, A_UINT32 timeout, A_UINT32 type);
void _qcom_isr_timer_stop();

#endif


#define qcom_timer_init(timer, fn, fn_arg, timeout, type) \
	QCOM_TIMER_API_FN(_qcom_timer_init((timer), (fn), (fn_arg), (timeout), (type)))
#define qcom_timer_start(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_start(timer))
#define qcom_timer_stop(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_stop(timer))
#define qcom_timer_delete(timer) \
	QCOM_TIMER_API_FN(_qcom_timer_delete(timer))
#define sleep_timeout(nouse, data) \
	QCOM_TIMER_API_FN(_sleep_timeout((nouse), (data)))
#define qcom_thread_msleep(time) \
	QCOM_TIMER_API_FN(_qcom_thread_msleep(time))
A_UINT32 _qcom_yield();
#define qcom_yield() _qcom_yield()
int _qcom_timer_us_start(qcom_timer_t * qtimer);
#define qcom_timer_us_start(timer)\
          _qcom_timer_us_start(timer)

/*Real-time timer which has high priority, TBD for ROM support*/
/*This timer uses LF1 and normally qcom api timer uses LF0, we put LF1 in high priority
** handler to make it real-time.
*/

/* fn :timeout handler
** arg: args of fn
** timeout: microsecond, set by user
** type: ONESHOT and PERIODIC
*/
A_UINT32 _qcom_isr_timer_start(void (*fn) (void *), void *arg, A_UINT32 timeout, A_UINT32 type);
void _qcom_isr_timer_stop();

#define qcom_isr_timer_start(fn, fn_arg, timeout, type) \
          _qcom_isr_timer_start((fn), (fn_arg), (timeout), (type))

#define qcom_isr_timer_stop()\
          _qcom_isr_timer_stop()


#ifdef __cplusplus
}
#endif

#endif
