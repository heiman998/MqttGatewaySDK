/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2012 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/

/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#ifndef TXQC_INDIRECTION_H
#define TXQC_INDIRECTION_H

/*
 * Support for ThreadX indirection table.
 *
 * All ThreadX functions called FROM ROM C code are called indirectly
 * through this table.  This facilitates post-tapeout replacement
 * of ROM functions with RAM functions, as needed.  Calls from ROM
 * assembler are handled specially with wrapper functions.
 */

#if defined(AR6002_REV72) || defined(DISABLE_FUNCTION_INDIRECTION)
/* 
 * Using ANY function indirection requires threadx_api.
 * threadx_api pulls in much of libthreadx, which consumes
 * lots of IRAM. This won't work for REV72-based environemnts.
 * For REV74, ThreadX is entirely in ROM so we must use
 * function indirections when calling from ROM.  Calls from
 * RAM code can be direct.
 *
 * No, this isn't the ideal place to set this....
 */
#define TX_DISABLE_FUNCTION_INDIRECTION 1
#endif

#if !defined(__ASSEMBLER__)

#if defined(TX_DISABLE_FUNCTION_INDIRECTION)
#define _tx_block_allocate(p, b, w)      __tx_block_allocate((p), (b), (w))
#define _tx_block_pool_create(t,n,b,s,l) __tx_block_pool_create((t),(n),(b),(s),(l))
#define _tx_block_pool_delete(p)                __tx_block_pool_delete(p)
#define _tx_block_pool_info_get(p,n,a,t,f,s,np) __tx_block_pool_info_get((p),(n),(a),(t),(f),(s),(np))
#define _tx_block_pool_prioritize(p)            __tx_block_pool_prioritize(p)
#define _tx_block_release(b)                    __tx_block_release(b)
#define _tx_byte_allocate(p,m,s,w)              __tx_byte_allocate((p),(m),(s),(w))
#define _tx_byte_pool_create(p,n,s,sz)          __tx_byte_pool_create((p),(n),(s),(sz))
#define _tx_byte_pool_delete(p)                 __tx_byte_pool_delete(p)
#define _tx_byte_pool_info_get(p,n,b,f,fs,sc,np)__tx_byte_pool_info_get((p),(n),(b),(f),(fs),(sc),(np))
#define _tx_byte_pool_prioritize(p)             __tx_byte_pool_prioritize(p)
#define _tx_byte_release(p)                     __tx_byte_release(p)
#define _tx_byte_pool_release(m)                __tx_byte_pool_release(m)
#define _tx_byte_pool_search(p,sz)              __tx_byte_pool_search((p),(sz))
#if defined(AR6002_REV74)
#define _tx_timer_create(t,n,fn,in,it,rt,a)     __txe_timer_create((t),(n),(fn),(in),(it),(rt),(a),0)
#define _tx_timer_deactivate(t)                 __txe_timer_deactivate(t)
#define _tx_timer_delete(t)                     __txe_timer_delete(t)
#define _tx_timer_info_get(t,n,a,rmt,rst,nt)    __txe_timer_info_get((t),(n),(a),(rmt),(rst),(nt))
#else
#define _tx_timer_create(t,n,fn,in,it,rt,a)     __tx_timer_create((t),(n),(fn),(in),(it),(rt),(a))
#define _tx_timer_deactivate(t)                 __tx_timer_deactivate(t)
#define _tx_timer_delete(t)                     __tx_timer_delete(t)
#define _tx_timer_info_get(t,n,a,rmt,rst,nt)    __tx_timer_info_get((t),(n),(a),(rmt),(rst),(nt))
#endif
#define _tx_event_flags_create(g,n)             __tx_event_flags_create((g),(n))
#define _tx_event_flags_delete(g)               __tx_event_flags_delete(g)
#define _tx_event_flags_get(g,rf,o,af,w)        __tx_event_flags_get((g),(rf),(o),(af),(w))
#define _tx_event_flags_info_get(g,n,cf,fs,sc,ng)__tx_event_flags_info_get((g),(n),(cf),(fs),(sc),(ng))
#define _tx_event_flags_set(g,f,o)              __tx_event_flags_set((g),(f),(o))
#define _tx_event_flags_set_notify(g,fn)        __tx_event_flags_set_notify((g),(fn))
#define _tx_initialize_high_level()             __tx_initialize_high_level()
#define _tx_initialize_low_level()              __tx_initialize_low_level()
#define _tx_initialize_high_level()             __tx_initialize_high_level()
#define _tx_initialize_low_level()              __tx_initialize_low_level()
#define _tx_mutex_create(m,n,i)                 __tx_mutex_create((m),(n),(i))
#define _tx_mutex_delete(m)                     __tx_mutex_delete(m)
#define _tx_mutex_get(m,w)                      __tx_mutex_get((m),(w))
#define _tx_mutex_info_get(m,n,c,o,fw,sc,nm)    __tx_mutex_info_get((m),(n),(c),(o),(fw),(sc),(nm))
#define _tx_mutex_prioritize(m)                 __tx_mutex_prioritize(m)
#define _tx_mutex_priority_change(t,p)          __tx_mutex_priority_change((t),(p))
#define _tx_mutex_put(m)                        __tx_mutex_put(m)
#define txqc_app_entry_get()                    _txqc_app_entry_get()
#define txqc_init()                             _txqc_init()
#define txqc_prep_threadx_scheduler()           _txqc_prep_threadx_scheduler()
#define txqc_meta_scheduler()                   __txqc_meta_scheduler()
#define txqc_is_meta_scheduler_activity_pending()_txqc_is_meta_scheduler_activity_pending()
#define txqc_pool_start(st,sz)                  _txqc_pool_start((st),(sz))
#define _tx_queue_create(q,n,msz,s,qsz)         __tx_queue_create((q),(n),(msz),(s),(qsz))
#define _tx_queue_delete(q)                     __tx_queue_delete(q)
#define _tx_queue_flush(q)                      __tx_queue_flush(q)
#define _tx_queue_front_send(q,s,w)             __tx_queue_front_send((q),(s),(w))
#define _tx_queue_info_get(q,n,e,as,fs,sc,nq)   __tx_queue_info_get((q),(n),(e),(as),(fs),(sc),(nq))
#define _tx_queue_prioritize(q)                 __tx_queue_prioritize(q)
#define _tx_queue_receive(q,d,w)                __tx_queue_receive((q),(d),(w))
#define _tx_queue_send(q,s,w)                   __tx_queue_send((q),(s),(w))
#define _tx_queue_send_notify(q,fn)             __tx_queue_send_notify((q),(fn))
#define _tx_semaphore_ceiling_put(s,c)          __tx_semaphore_ceiling_put((s),(c))
#define _tx_semaphore_create(s,n,c)             __tx_semaphore_create((s),(n),(c))
#define _tx_semaphore_delete(s)                 __tx_semaphore_delete(s)
#define _tx_semaphore_get(s,w)                  __tx_semaphore_get((s),(w))
#define _tx_semaphore_info_get(s,n,v,fs,sc,ns)  __tx_semaphore_info_get((s),(n),(v),(fs),(sc),(ns))
#define _tx_semaphore_prioritize(s)             __tx_semaphore_prioritize(s)
#define _tx_semaphore_put(s)                    __tx_semaphore_put(s)
#define _tx_semaphore_put_notify(s,fn)          __tx_semaphore_put_notify((s),(fn))
#define _tx_thread_create(t,n,fn,in,stk,stksz,pri,pt,ts,as) \
        __tx_thread_create((t),(n),(fn),(in),(stk),(stksz),(pri),(pt),(ts),(as))
#define _tx_thread_delete(t)                    __tx_thread_delete(t)
#define _tx_thread_entry_exit_notify(t,fn)      __tx_thread_entry_exit_notify((t),(fn))
#define _tx_thread_info_get(t,n,s,rc,pri,pt,ts,nt,nst) \
        __tx_thread_info_get((t),(n),(s),(rc),(pri),(pt),(ts),(nt),(nst))
#define _tx_thread_initialize()                 __tx_thread_initialize()
#define _tx_thread_interrupt_control(np)        __tx_thread_interrupt_control(np)
#define _tx_thread_preemption_change(t,nt,ot)   __tx_thread_preemption_change((t),(nt),(ot))
#define _tx_thread_priority_change(t,np,op)     __tx_thread_priority_change((t),(np),(op))
#define _tx_thread_relinquish()                 __tx_thread_relinquish()
#define _tx_thread_reset(t)                     __tx_thread_reset(t)
#define _tx_thread_resume(t)                    __tx_thread_resume(t)
#define _tx_thread_system_suspend(t)            __tx_thread_system_suspend(t)
#define _tx_thread_system_resume(t)             __tx_thread_system_resume(t)
#define _tx_thread_system_return()              __tx_thread_system_return()
#define _tx_thread_schedule()                   __tx_thread_schedule()
#define _tx_thread_stack_build(t,fn)            __tx_thread_stack_build((t),(fn))
#define _tx_thread_suspend(t)                   __tx_thread_suspend(t)
#define _tx_thread_system_preempt_check()       __tx_thread_system_preempt_check()
#define _tx_thread_terminate(t)                 __tx_thread_terminate(t)
#define _tx_thread_time_slice_change(t,nts,ots) __tx_thread_time_slice_change((t),(nts),(ots))
#define _tx_thread_wait_abort(t)                __tx_thread_wait_abort(t)
#define _tx_timer_activate(t)                   __tx_timer_activate(t)
#define _tx_timer_change(t,it,rt)               __tx_timer_change((t),(it),(rt))
#define _tx_timer_initialize()                  __tx_timer_initialize()
#define _tx_timer_internal_system_init(t)       __tx_timer_internal_system_init(t)
#define _tx_timer_system_activate(t)            __tx_timer_system_activate(t)
#define _tx_timer_system_deactivate(t)          __tx_timer_system_deactivate(t)
#else

struct threadx_api_s {
    UINT  (* __tx_block_allocate)(TX_BLOCK_POOL *pool_ptr, VOID **block_ptr, ULONG wait_option);
    UINT  (* __tx_block_pool_create)(TX_BLOCK_POOL *pool_ptr, CHAR *name_ptr, ULONG block_size,
                    VOID *pool_start, ULONG pool_size);
    UINT  (* __tx_block_pool_delete)(TX_BLOCK_POOL *pool_ptr);
    UINT  (* __tx_block_pool_info_get)(TX_BLOCK_POOL *pool_ptr, CHAR **name, ULONG *available_blocks, 
                    ULONG *total_blocks, TX_THREAD **first_suspended, 
                    ULONG *suspended_count, TX_BLOCK_POOL **next_pool);
    UINT  (* __tx_block_pool_prioritize)(TX_BLOCK_POOL *pool_ptr);
    UINT  (* __tx_block_release)(VOID *block_ptr);
    UINT  (* __tx_byte_allocate)(TX_BYTE_POOL *pool_ptr, VOID **memory_ptr, ULONG memory_size,
                    ULONG wait_option);
    UINT  (* __tx_byte_pool_create)(TX_BYTE_POOL *pool_ptr, CHAR *name_ptr, VOID *pool_start,
                    ULONG pool_size);
    UINT  (* __tx_byte_pool_delete)(TX_BYTE_POOL *pool_ptr);
    UINT  (* __tx_byte_pool_info_get)(TX_BYTE_POOL *pool_ptr, CHAR **name, ULONG *available_bytes,
                    ULONG *fragments, TX_THREAD **first_suspended, 
                    ULONG *suspended_count, TX_BYTE_POOL **next_pool);
    UINT  (* __tx_byte_pool_prioritize)(TX_BYTE_POOL *pool_ptr);
    UINT  (* __tx_byte_release)(VOID *memory_ptr);
    VOID *(* __tx_byte_pool_search)(TX_BYTE_POOL *pool_ptr, ULONG memory_size);
#if defined(AR6002_REV74)
    UINT  (* __txe_timer_create)(TX_TIMER *timer_ptr, CHAR *name_ptr, 
            VOID (*expiration_function)(ULONG), ULONG expiration_input,
            ULONG initial_ticks, ULONG reschedule_ticks, UINT auto_activate, UINT timer_control_block_size);
    UINT  (* __txe_timer_deactivate)(TX_TIMER *timer_ptr);
    UINT  (* __txe_timer_delete)(TX_TIMER *timer_ptr);
    UINT  (* __txe_timer_info_get)(TX_TIMER *timer_ptr, CHAR **name, UINT *active, ULONG *remaining_ticks, 
                ULONG *reschedule_ticks, TX_TIMER **next_timer);
#else
    UINT  (* __tx_timer_create)(TX_TIMER *timer_ptr, CHAR *name_ptr, 
            VOID (*expiration_function)(ULONG), ULONG expiration_input,
            ULONG initial_ticks, ULONG reschedule_ticks, UINT auto_activate);
    UINT  (* __tx_timer_deactivate)(TX_TIMER *timer_ptr);
    UINT  (* __tx_timer_delete)(TX_TIMER *timer_ptr);
    UINT  (* __tx_timer_info_get)(TX_TIMER *timer_ptr, CHAR **name, UINT *active, ULONG *remaining_ticks, 
                ULONG *reschedule_ticks, TX_TIMER **next_timer);
#endif
    UINT  (* __tx_event_flags_create)(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
    UINT  (* __tx_event_flags_delete)(TX_EVENT_FLAGS_GROUP *group_ptr);
    UINT  (* __tx_event_flags_get)(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                    UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option);
    UINT  (* __tx_event_flags_info_get)(TX_EVENT_FLAGS_GROUP *group_ptr, CHAR **name, ULONG *current_flags, 
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_EVENT_FLAGS_GROUP **next_group);
    UINT  (* __tx_event_flags_set)(TX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, 
                    UINT set_option);
    UINT  (* __tx_event_flags_set_notify)(TX_EVENT_FLAGS_GROUP *group_ptr, VOID (*events_set_notify)(TX_EVENT_FLAGS_GROUP *));
    VOID (* __tx_initialize_high_level)(VOID);
    VOID (* __tx_initialize_low_level)(VOID);
    UINT (* __tx_mutex_create)(TX_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
    UINT (* __tx_mutex_delete)(TX_MUTEX *mutex_ptr);
    UINT (* __tx_mutex_get)(TX_MUTEX *mutex_ptr, ULONG wait_option);
    UINT (* __tx_mutex_info_get)(TX_MUTEX *mutex_ptr, CHAR **name, ULONG *count, TX_THREAD **owner, 
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_MUTEX **next_mutex);
    UINT (* __tx_mutex_prioritize)(TX_MUTEX *mutex_ptr);
    VOID (* __tx_mutex_priority_change)(TX_THREAD *thread_ptr, UINT priority);
    UINT (* __tx_mutex_put)(TX_MUTEX *mutex_ptr);
    VOID *(* _txqc_app_entry_get)(VOID);
    void (* _txqc_init)(void);
    void (* _txqc_prep_threadx_scheduler)(void);
    void (* _txqc_meta_scheduler)(void);
    int  (* _txqc_is_meta_scheduler_activity_pending)(void);
    VOID *(* _txqc_pool_start)(VOID *pool_start, ULONG pool_size);
    UINT (* __tx_queue_create)(TX_QUEUE *queue_ptr, CHAR *name_ptr, UINT message_size, 
                        VOID *queue_start, ULONG queue_size);
    UINT (* __tx_queue_delete)(TX_QUEUE *queue_ptr);
    UINT (* __tx_queue_flush)(TX_QUEUE *queue_ptr);
    UINT (* __tx_queue_front_send)(TX_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
    UINT (* __tx_queue_info_get)(TX_QUEUE *queue_ptr, CHAR **name, ULONG *enqueued, ULONG *available_storage,
                    TX_THREAD **first_suspended, ULONG *suspended_count, TX_QUEUE **next_queue);
    UINT (* __tx_queue_prioritize)(TX_QUEUE *queue_ptr);
    UINT (* __tx_queue_receive)(TX_QUEUE *queue_ptr, VOID *destination_ptr, ULONG wait_option);
    UINT (* __tx_queue_send)(TX_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
    UINT (* __tx_queue_send_notify)(TX_QUEUE *queue_ptr, VOID (*queue_send_notify)(TX_QUEUE *));
    UINT (* __tx_semaphore_ceiling_put)(TX_SEMAPHORE *semaphore_ptr, ULONG ceiling);
    UINT (* __tx_semaphore_create)(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count);
    UINT (* __tx_semaphore_delete)(TX_SEMAPHORE *semaphore_ptr);
    UINT (* __tx_semaphore_get)(TX_SEMAPHORE *semaphore_ptr, ULONG wait_option);
    UINT (* __tx_semaphore_info_get)(TX_SEMAPHORE *semaphore_ptr, CHAR **name, ULONG *current_value, 
                    TX_THREAD **first_suspended, ULONG *suspended_count, 
                    TX_SEMAPHORE **next_semaphore);
    UINT (* __tx_semaphore_prioritize)(TX_SEMAPHORE *semaphore_ptr);
    UINT (* __tx_semaphore_put)(TX_SEMAPHORE *semaphore_ptr);
    UINT (* __tx_semaphore_put_notify)(TX_SEMAPHORE *semaphore_ptr, VOID (*semaphore_put_notify)(TX_SEMAPHORE *));
    UINT (* __tx_thread_create)(TX_THREAD *thread_ptr, CHAR *name_ptr, 
                VOID (*entry_function)(ULONG), ULONG entry_input,
                VOID *stack_start, ULONG stack_size, 
                UINT priority, UINT preempt_threshold, 
                ULONG time_slice, UINT auto_start);
    UINT (* __tx_thread_delete)(TX_THREAD *thread_ptr);
    UINT (* __tx_thread_entry_exit_notify)(TX_THREAD *thread_ptr, VOID (*thread_entry_exit_notify)(TX_THREAD *, UINT));
    UINT (* __tx_thread_info_get)(TX_THREAD *thread_ptr, CHAR **name, UINT *state, ULONG *run_count, 
                UINT *priority, UINT *preemption_threshold, ULONG *time_slice, 
                TX_THREAD **next_thread, TX_THREAD **next_suspended_thread);
    VOID (* __tx_thread_initialize)(VOID);
    UINT (* __tx_thread_interrupt_control)(UINT new_posture);
    UINT (* __tx_thread_preemption_change)(TX_THREAD *thread_ptr, UINT new_threshold,
                        UINT *old_threshold);
    UINT (* __tx_thread_priority_change)(TX_THREAD *thread_ptr, UINT new_priority,
                        UINT *old_priority);
    VOID (* __tx_thread_relinquish)(VOID);
    UINT (* __tx_thread_reset)(TX_THREAD *thread_ptr);
    UINT (* __tx_thread_resume)(TX_THREAD *thread_ptr);
    VOID (* __tx_thread_schedule)(VOID);
    VOID (* __tx_thread_system_suspend)(TX_THREAD *thread_ptr);
    VOID (* __tx_thread_system_resume)(TX_THREAD *thread_ptr);
    VOID (* __tx_thread_system_return)(VOID);
    VOID (* __tx_thread_stack_build)(TX_THREAD *thread_ptr, VOID (*function_ptr)(VOID));
    UINT (* __tx_thread_suspend)(TX_THREAD *thread_ptr);
    VOID (* __tx_thread_system_preempt_check)(VOID);
    UINT (* __tx_thread_terminate)(TX_THREAD *thread_ptr);
    UINT (* __tx_thread_time_slice_change)(TX_THREAD *thread_ptr, ULONG new_time_slice, ULONG *old_time_slice);
    UINT (* __tx_thread_wait_abort)(TX_THREAD *thread_ptr);
    UINT (* __tx_timer_activate)(TX_TIMER *timer_ptr);
    UINT (* __tx_timer_change)(TX_TIMER *timer_ptr, ULONG initial_ticks, ULONG reschedule_ticks);
    VOID (* __tx_timer_initialize)(VOID);
    VOID (* __tx_timer_internal_system_init)(TX_TIMER_INTERNAL *tx_timer_internal);
    VOID (* __tx_timer_system_activate)(TX_TIMER_INTERNAL *timer_ptr);
    VOID (* __tx_timer_system_deactivate)(TX_TIMER_INTERNAL *timer_ptr);
};
extern struct threadx_api_s *threadx_api;

#define _tx_block_allocate(p,b,w)               threadx_api->__tx_block_allocate((p),(b),(w))
#define _tx_block_pool_create(t,n,b,s,l)        threadx_api->__tx_block_pool_create((t),(n),(b),(s),(l))
#define _tx_block_pool_delete(p)                threadx_api->__tx_block_pool_delete(p)
#define _tx_block_pool_info_get(p,n,a,t,f,s,np) threadx_api->__tx_block_pool_info_get((p),(n),(a),(t),(f),(s),(np))
#define _tx_block_pool_prioritize(p)            threadx_api->__tx_block_pool_prioritize(p)
#define _tx_block_release(b)                    threadx_api->__tx_block_release(b)
#define _tx_byte_allocate(p,m,s,w)              threadx_api->__tx_byte_allocate((p),(m),(s),(w))
#define _tx_byte_pool_create(p,n,s,sz)          threadx_api->__tx_byte_pool_create((p),(n),(s),(sz))
#define _tx_byte_pool_delete(p)                 threadx_api->__tx_byte_pool_delete(p)
#define _tx_byte_pool_info_get(p,n,b,f,fs,sc,np)threadx_api->__tx_byte_pool_info_get((p),(n),(b),(f),(fs),(sc),(np))
#define _tx_byte_pool_prioritize(p)             threadx_api->__tx_byte_pool_prioritize(p)
#define _tx_byte_release(p)                     threadx_api->__tx_byte_release(p)
#define _tx_byte_pool_release(m)                threadx_api->__tx_byte_pool_release(m)
#define _tx_byte_pool_search(p,sz)              threadx_api->__tx_byte_pool_search((p),(sz))
#if defined(AR6002_REV74)
#define _tx_timer_create(t,n,fn,in,it,rt,a)     threadx_api->__txe_timer_create((t),(n),(fn),(in),(it),(rt),(a),0)
#define _tx_timer_deactivate(t)                 threadx_api->__txe_timer_deactivate(t)
#define _tx_timer_delete(t)                     threadx_api->__txe_timer_delete(t)
#define _tx_timer_info_get(t,n,a,rmt,rst,nt)    threadx_api->__txe_timer_info_get((t),(n),(a),(rmt),(rst),(nt))
#else
#define _tx_timer_create(t,n,fn,in,it,rt,a)     threadx_api->__tx_timer_create((t),(n),(fn),(in),(it),(rt), (a))
#define _tx_timer_deactivate(t)                 threadx_api->__tx_timer_deactivate(t)
#define _tx_timer_delete(t)                     threadx_api->__tx_timer_delete(t)
#define _tx_timer_info_get(t,n,a,rmt,rst,nt)    threadx_api->__tx_timer_info_get((t),(n),(a),(rmt),(rst),(nt))
#endif
#define _tx_event_flags_create(g,n)             threadx_api->__tx_event_flags_create((g),(n))
#define _tx_event_flags_delete(g)               threadx_api->__tx_event_flags_delete(g)
#define _tx_event_flags_get(g,rf,o,af,w)        threadx_api->__tx_event_flags_get((g),(rf),(o),(af),(w))
#define _tx_event_flags_info_get(g,n,cf,fs,sc,ng)threadx_api->__tx_event_flags_info_get((g),(n),(cf),(fs),(sc),(ng))
#define _tx_event_flags_set(g,f,o)              threadx_api->__tx_event_flags_set((g),(f),(o))
#define _tx_event_flags_set_notify(g,fn)        threadx_api->__tx_event_flags_set_notify((g),(fn))
#define _tx_initialize_high_level()             threadx_api->__tx_initialize_high_level()
#define _tx_initialize_low_level()              threadx_api->__tx_initialize_low_level()
#define _tx_initialize_high_level()             threadx_api->__tx_initialize_high_level()
#define _tx_initialize_low_level()              threadx_api->__tx_initialize_low_level()
#define _tx_mutex_create(m,n,i)                 threadx_api->__tx_mutex_create((m),(n),(i))
#define _tx_mutex_delete(m)                     threadx_api->__tx_mutex_delete(m)
#define _tx_mutex_get(m,w)                      threadx_api->__tx_mutex_get((m),(w))
#define _tx_mutex_info_get(m,n,c,o,fw,sc,nm)    threadx_api->__tx_mutex_info_get((m),(n),(c),(o),(fw),(sc),(nm))
#define _tx_mutex_prioritize(m)                 threadx_api->__tx_mutex_prioritize(m)
#define _tx_mutex_priority_change(t,p)          threadx_api->__tx_mutex_priority_change((t),(p))
#define _tx_mutex_put(m)                        threadx_api->__tx_mutex_put(m)
#define txqc_app_entry_get()                    threadx_api->_txqc_app_entry_get()
#define txqc_init()                             threadx_api->_txqc_init()
#define txqc_prep_threadx_scheduler()           threadx_api->_txqc_prep_threadx_scheduler()
#define txqc_meta_scheduler()                   threadx_api->_txqc_meta_scheduler()
#define txqc_is_meta_scheduler_activity_pending()  \
        threadx_api->_txqc_is_meta_scheduler_activity_pending()
#define txqc_pool_start(st,sz)                  threadx_api->_txqc_pool_start((st),(sz))
#define _tx_queue_create(q,n,msz,s,qsz)         threadx_api->__tx_queue_create((q),(n),(msz),(s),(qsz))
#define _tx_queue_delete(q)                     threadx_api->__tx_queue_delete(q)
#define _tx_queue_flush(q)                      threadx_api->__tx_queue_flush(q)
#define _tx_queue_front_send(q,s,w)             threadx_api->__tx_queue_front_send((q),(s),(w))
#define _tx_queue_info_get(q,n,e,as,fs,sc,nq)   threadx_api->__tx_queue_info_get((q),(n),(e),(as),(fs),(sc),(nq))
#define _tx_queue_prioritize(q)                 threadx_api->__tx_queue_prioritize(q)
#define _tx_queue_receive(q,d,w)                threadx_api->__tx_queue_receive((q),(d),(w))
#define _tx_queue_send(q,s,w)                   threadx_api->__tx_queue_send((q),(s),(w))
#define _tx_queue_send_notify(q,fn)             threadx_api->__tx_queue_send_notify((q),(fn))
#define _tx_semaphore_ceiling_put(s,c)          threadx_api->__tx_semaphore_ceiling_put((s),(c))
#define _tx_semaphore_create(s,n,c)             threadx_api->__tx_semaphore_create((s),(n),(c))
#define _tx_semaphore_delete(s)                 threadx_api->__tx_semaphore_delete(s)
#define _tx_semaphore_get(s,w)                  threadx_api->__tx_semaphore_get((s),(w))
#define _tx_semaphore_info_get(s,n,v,fs,sc,ns)  threadx_api->__tx_semaphore_info_get((s),(n),(v),(fs),(sc),(ns))
#define _tx_semaphore_prioritize(s)             threadx_api->__tx_semaphore_prioritize(s)
#define _tx_semaphore_put(s)                    threadx_api->__tx_semaphore_put(s)
#define _tx_semaphore_put_notify(s,fn)          threadx_api->__tx_semaphore_put_notify((s),(fn))
#define _tx_thread_create(t,n,fn,in,stk,stksz,pri,pt,ts,as) \
        threadx_api->__tx_thread_create((t),(n),(fn),(in),(stk),(stksz),(pri),(pt),(ts),(as))
#define _tx_thread_delete(t)                    threadx_api->__tx_thread_delete(t)
#define _tx_thread_entry_exit_notify(t,fn)      threadx_api->__tx_thread_entry_exit_notify((t),(fn))
#define _tx_thread_info_get(t,n,s,rc,pri,pt,ts,nt,nst) \
        threadx_api->__tx_thread_info_get((t),(n),(s),(rc),(pri),(pt),(ts),(nt),(nst))
#define _tx_thread_initialize()                 threadx_api->__tx_thread_initialize()
#define _tx_thread_interrupt_control(np)        threadx_api->__tx_thread_interrupt_control(np)
#define _tx_thread_preemption_change(t,nt,ot)   threadx_api->__tx_thread_preemption_change((t),(nt),(ot))
#define _tx_thread_priority_change(t,np,op)     threadx_api->__tx_thread_priority_change((t),(np),(op))
#define _tx_thread_relinquish()                 threadx_api->__tx_thread_relinquish()
#define _tx_thread_reset(t)                     threadx_api->__tx_thread_reset(t)
#define _tx_thread_resume(t)                    threadx_api->__tx_thread_resume(t)
#define _tx_thread_system_suspend(t)            threadx_api->__tx_thread_system_suspend(t)
#define _tx_thread_system_resume(t)             threadx_api->__tx_thread_system_resume(t)
#define _tx_thread_system_return()              threadx_api->__tx_thread_system_return()
#define _tx_thread_schedule()                   threadx_api->__tx_thread_schedule()
#define _tx_thread_stack_build(t,fn)            threadx_api->__tx_thread_stack_build((t),(fn))
#define _tx_thread_suspend(t)                   threadx_api->__tx_thread_suspend(t)
#define _tx_thread_system_preempt_check()       threadx_api->__tx_thread_system_preempt_check()
#define _tx_thread_terminate(t)                 threadx_api->__tx_thread_terminate(t)
#define _tx_thread_time_slice_change(t,nts,ots) threadx_api->__tx_thread_time_slice_change((t),(nts),(ots))
#define _tx_thread_wait_abort(t)                threadx_api->__tx_thread_wait_abort(t)
#define _tx_timer_activate(t)                   threadx_api->__tx_timer_activate(t)
#define _tx_timer_change(t,it,rt)               threadx_api->__tx_timer_change((t),(it),(rt))
#define _tx_timer_initialize()                  threadx_api->__tx_timer_initialize()
#define _tx_timer_internal_system_init(t)       threadx_api->__tx_timer_internal_system_init(t)
#define _tx_timer_system_activate(t)            threadx_api->__tx_timer_system_activate(t)
#define _tx_timer_system_deactivate(t)          threadx_api->__tx_timer_system_deactivate(t)
#endif /* !TX_DISABLE_FUNCTION_INDIRECTION */

#endif /* !__ASSEMBLER__ */
#endif /* TXQC_INDIRECTION_H */
