/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2011 by Express Logic Inc.               */ 
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
 * Copyright (c) 2013-2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 */

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** ThreadX Component                                                     */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    tx_port.h                                           Xtensa/GNU      */ 
/*                                                           5.7          */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Ross Morley, Tensilica, Inc.                                        */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make the ThreadX      */ 
/*    real-time kernel function identically on a variety of different     */ 
/*    processor architectures.  For example, the size or number of bits   */ 
/*    in an "int" data type vary between microprocessor architectures and */ 
/*    even C compilers for the same microprocessor.  ThreadX does not     */ 
/*    directly use native C data types.  Instead, ThreadX creates its     */ 
/*    own special types that can be mapped to actual data types by this   */ 
/*    file to guarantee consistency in the interface and functionality.   */ 
/*                                                                        */
/*    This port supports Xtensa exception architecture 2 (XEA2) only.     */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2006     Ross Morley              Initial Xtensa/GNU            */ 
/*                                            support version 5.0         */ 
/*  04-02-2007     Ross Morley              Modified comment(s),          */ 
/*                                            modified check for          */ 
/*                                            XCHAN_CP_NUM, and           */ 
/*                                            deprecated XT_XT2000,       */ 
/*                                            resulting in version 5.1    */ 
/*  06-18-2008     Ross Morley              Modified comment(s), added    */ 
/*                                            "extern" to the             */ 
/*                                            interrupt control           */ 
/*                                            function prototype, and     */ 
/*                                            updated version ID string,  */ 
/*                                            resulting in version 5.2    */ 
/*  07-15-2009     Ross Morley              Modified comment(s), added    */ 
/*                                            include of string.h, added  */ 
/*                                            GDBIO constants, and        */ 
/*                                            updated version ID string,  */ 
/*                                            resulting in version 5.3    */ 
/*  05-01-2010     William E. Lamie         Modified comment(s), and      */ 
/*                                            updated version string,     */ 
/*                                            resulting in version 5.4    */ 
/*  01-28-2011     William E. Lamie         Modified comment(s), added    */ 
/*                                            padding for alignment, and  */ 
/*                                            updated version string,     */ 
/*                                            resulting in version 5.5    */ 
/*  07-15-2011     William E. Lamie         Modified comment(s), modified */ 
/*                                            control block and thread    */ 
/*                                            processing extensions to be */ 
/*                                            compatible with all cores,  */ 
/*                                            and updated version string, */ 
/*                                            resulting in version 5.6    */ 
/*  10-01-2011     William E. Lamie         Modified comment(s), and      */ 
/*                                            updated version string,     */ 
/*                                            resulting in version 5.7    */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef TX_PORT_H
#define TX_PORT_H

#define TX_DISABLE_ERROR_CHECKING

/* Determine if the optional ThreadX user define file should be used.  */

#ifdef TX_INCLUDE_USER_DEFINE_FILE


/* Yes, include the user defines in tx_user.h. The defines in this file may 
   alternately be defined on the command line.  */

#include "tx_user.h"
#endif

/* Non-standard port-specific flags */
#if defined(AR6002_REV7)
/*
 * Allow the placement of memory pools to be decided by
 * ThreadX rather than by the caller.
 */
#define TXQC_CONFIG_DYNAMIC_POOLS 1

/*
 * Use the AR6K SoC's timer rather than the Xtensa CPU's
 * timer for ThreadX timers.
 */
#define TXQC_CONFIG_USE_AR6K_TIMER 1

/*
 * Permit application start address to be defined at run-time
 * rather than at link time.  This is needed especially when
 * ThreadX is in ROM and the application is in RAM.
 */
#define TXQC_CONFIG_RUN_TIME_APP_START 1
#endif /* AR6002_REV7 */


#if defined(TXQC_CONFIG_USE_AR6K_TIMER)
/* Need to define this before we include xtensa_rtos.h */
#define TXQC_TICK_PER_SEC 1000
#endif

/* Include the glue to Xtensa-generic parts of this ThreadX port. */
#include "xtensa_rtos.h"

/* Uncomment this to get thread-safe C library (newlib) support. */
//#define  TX_THREAD_SAFE_NEWLIB


/* Parts of this file should not been seen by assembler sources. */
#ifndef __ASSEMBLER__

/* Some generic C sources call memset() and need this (else compiler warns).
   Until the generic sources take care of this, do it here. */
#include <string.h>


/* Define compiler library include files and library-specific macros. */

/*  Thread-safety support for the newlib C library.
    To simplify distribution of newlib for Xtensa targets, this
    thread-safety support is only partial for now.  More complete support
    for thread-safety requires a custom implementation of newlib's
    libc/include/sys/lock.h header file, and thus a custom recompile
    of the newlib C library, as well as a recent version of newlib
    (preferably post 1.14) that has fixes for a number of reentrancy
    issues.  Instead, for compatibility with the pre-compiled version
    of newlib shipped with Xtensa Tools version 6, the support provided
    in tx_clib_lock.c simply overrides available lock functions
    (__malloc_lock / __env_lock / etc) in addition to context-switching
    newlib's re-entrancy structure.  Pre-compiled versions of newlib
    shipped with subsequent versions of Xtensa Tools may provide
    different or additional hooks for thread safety.

    The C library reent structure is huge (about 1 kB in newlib 1.13),
    so we place it at the end of TX_THREAD, and put a pointer to it
    near the beginning of TX_THREAD where assembly code can easily
    get to it at a fixed offset.
    */
#ifdef TX_THREAD_SAFE_NEWLIB
extern char *_tx_clib_heap_start, *_tx_clib_heap_end;
#include    <sys/reent.h>
#define TX_CLIB_THREAD_EXTENSION \
    struct _reent *     tx_thread_clib_ptr;     /* C lib reentrancy ptr */
#define TX_CLIB_THREAD_EXTENSION_END \
    struct _reent       tx_thread_clib_reent;   /* C lib reentrancy struct */
#define TX_CLIB_THREAD_CREATE_EXTENSION(thread_ptr) \
    _tx_clib_reent_init(thread_ptr);
#define TX_CLIB_THREAD_DELETE_EXTENSION(thread_ptr) \
    _tx_clib_reent_cleanup(thread_ptr, 0);
#define TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr) \
    _tx_clib_reent_cleanup(thread_ptr, 1);
#else
#define TX_CLIB_THREAD_EXTENSION
#define TX_CLIB_THREAD_EXTENSION_END
#define TX_CLIB_THREAD_CREATE_EXTENSION(thread_ptr)
#define TX_CLIB_THREAD_DELETE_EXTENSION(thread_ptr)
#define TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr)
#endif


/* Define ThreadX basic types for this port.  */ 

#define VOID                                    void
typedef char                                    CHAR;
typedef unsigned char                           UCHAR;
typedef int                                     INT;
typedef unsigned int                            UINT;
typedef long                                    LONG;
typedef unsigned long                           ULONG;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;


#endif /* #ifndef __ASSEMBLER__ */


/* Define the priority levels for ThreadX.  Legal values range
   from 32 to 1024 and MUST be evenly divisible by 32.  */

#ifndef TX_MAX_PRIORITIES
#define TX_MAX_PRIORITIES                       32
#endif


/* Define the minimum stack for a ThreadX thread on this processor. If the size supplied during
   thread creation is less than TX_MINIMUM_STACK, the thread create call will return an error. 
   The minimum allows for a thread whose entry function makes no calls and needs no local frame.
   TX_MINIMUM_STACK_BASIC allows the entry function to at least call _tx_thread_relinquish().
   An extra 0x10 bytes is allowed in all cases to allow for stack pointer alignment to 16 bytes.
   There is an additional premium for the stack checking functionality of TX_ENABLE_STACK_CHECKING.
   In Xtensa, all these amounts depend on the function call ABI used by the configuration
   (in general, Call0 ABI needs about 0x20 bytes less stack space per function call frame).
   These amounts assume no compiler optimization (-O0). Optimization usually requires less stack.

   !!!! TX_MINIMUM_STACK_BASIC is a MINIMUM for threads that call tx_thread_relinquish() only. !!!!
   Threads that do more, and in particular call C library functions such as printf(), need even
   more stack space and it is up to the application developer to determine how much (it depends
   on which functions are called - note newlib's printf requires about 3KB on Windowed ABI). */

#ifdef __XTENSA_CALL0_ABI__
        /* Call0 ABI */

#ifndef TX_MINIMUM_STACK
#define TX_MINIMUM_STACK                        (XT_STK_FRMSZ + 0x10)
#endif
#ifdef  TX_ENABLE_STACK_CHECKING
#define TX_STACK_CHECK_PREMIUM                  0x30
#else
#define TX_STACK_CHECK_PREMIUM                  0
#endif
#ifndef TX_MINIMUM_STACK_BASIC
#define TX_MINIMUM_STACK_BASIC                  (XT_STK_FRMSZ + 0x70 + TX_STACK_CHECK_PREMIUM)
#endif

#else   /* Windowed ABI */

#ifndef TX_MINIMUM_STACK
#define TX_MINIMUM_STACK                        (XT_STK_FRMSZ + 0x20)
#endif
#ifdef  TX_ENABLE_STACK_CHECKING
#define TX_STACK_CHECK_PREMIUM                  0x50
#else
#define TX_STACK_CHECK_PREMIUM                  0
#endif
#ifndef TX_MINIMUM_STACK_BASIC
#define TX_MINIMUM_STACK_BASIC                  (XT_STK_FRMSZ + 0x100 + TX_STACK_CHECK_PREMIUM)
#endif

#endif

/* Define a minimum stack size for the ThreadX system stack on this processor. This is just
   a useful starting point for an application, it is not checked by ThreadX. The minimum 
   system stack size allows for the possible depth of interrupt nesting (XCHAL_EXCM_LEVEL-1
   interrupt stack frames and XCHAL_EXCM_LEVEL interrupt handlers including timer), assuming 
   very basic interrupt handlers (allows 1 call12). It needs to be increased to support the 
   application's real interrupt handlers (and timer interrupt if TX_TIMER_PROCESS_IN_ISR).
   The system stack is located where the stack pointer is inside tx_kernel_enter() which is 
   usually from main(), and so is determined by the development tools. It grows downward 
   toward the first available memory pointer passed to tx_application_define(). 
   An application should allow sufficient space for the system stack. */
#ifndef TX_SYSTEM_STACK_MINIMUM
#define TX_SYSTEM_STACK_MINIMUM                 (((XCHAL_EXCM_LEVEL-1) * (0x40 + XT_STK_FRMSZ)) + 0x40)
#endif

/* Define the system timer thread's default stack size and priority.  These are only applicable
   if TX_TIMER_PROCESS_IN_ISR is not defined.  */

#ifndef TX_TIMER_THREAD_STACK_SIZE
#ifdef __XTENSA_CALL0_ABI__
#define TX_TIMER_THREAD_STACK_SIZE              (TX_MINIMUM_STACK_BASIC + 0x50)
#else
#define TX_TIMER_THREAD_STACK_SIZE              (TX_MINIMUM_STACK_BASIC + 0x90)
#endif
#endif


/* Parts of this file should not been seen by assembler sources. */
#ifndef __ASSEMBLER__


#ifndef TX_TIMER_THREAD_PRIORITY    
#define TX_TIMER_THREAD_PRIORITY                0           /* Default timer thread priority    */ 
#endif


/* Define various constants for the ThreadX Xtensa port.  */ 

#define TX_INT_DISABLE                          XCHAL_EXCM_LEVEL  /* Disable interrupts value */
#define TX_INT_ENABLE                           0x0               /* Enable interrupt value   */


/* Define the clock source for trace event entry time stamp. The following two item are port specific.
   For example, if the time source is at the address 0x0a800024 and is 16-bits in size, the clock 
   source constants would be:

#define TX_TRACE_TIME_SOURCE                    *((ULONG *) 0x0a800024)
#define TX_TRACE_TIME_MASK                      0x0000FFFFUL

*/

#ifndef TX_TRACE_TIME_SOURCE
#define TX_TRACE_TIME_SOURCE                    ++_tx_trace_simulated_time
#endif
#ifndef TX_TRACE_TIME_MASK
#define TX_TRACE_TIME_MASK                      0xFFFFFFFFUL
#endif


/* Define the port specific options for the _tx_build_options variable. This variable indicates
   how the ThreadX library was built.  */

#ifdef  XT_SIMULATOR
#define TX_XT_OPT_SIMULATOR                     (1UL << 0)
#else
#define TX_XT_OPT_SIMULATOR                     0
#endif

#if defined(XT_BOARD) || defined(XT_XT2000)     /* XT_XT2000 is deprecated */
#define TX_XT_OPT_BOARD                         (1UL << 1)
#else
#define TX_XT_OPT_BOARD                         0
#endif

#ifdef  XT_INTEXC_HOOKS
#define TX_XT_OPT_INTEXC_HOOKS                  (1UL << 2)
#else
#define TX_XT_OPT_INTEXC_HOOKS                  0
#endif

#ifdef  TX_THREAD_SAFE_NEWLIB                   /* This need not be port-specific */
#define TX_XT_OPT_NEWLIB                        (1UL << 3)
#else
#define TX_XT_OPT_NEWLIB                        0
#endif

#ifdef  XT_GDBIO
#define TX_XT_OPT_GDBIO                         (1UL << 0)
#else
#define TX_XT_OPT_GDBIO                         0
#endif

#define TX_PORT_SPECIFIC_BUILD_OPTIONS          (TX_XT_OPT_SIMULATOR | TX_XT_OPT_BOARD \
                                                | TX_XT_OPT_INTEXC_HOOKS | TX_XT_OPT_NEWLIB \
                                                | TX_XT_OPT_GDBIO)


/* Define the in-line initialization constant so that modules with in-line
   initialization capabilities can prevent their initialization from being
   a function call.  */

#define TX_INLINE_INITIALIZATION


/* Determine whether or not stack checking is enabled. By default, ThreadX stack checking is 
   disabled. When the following is defined, ThreadX thread stack checking is enabled.  If stack
   checking is enabled (TX_ENABLE_STACK_CHECKING is defined), the TX_DISABLE_STACK_FILLING
   define is canceled, thereby forcing the stack fill which is necessary for the stack checking
   logic.  */

#ifdef TX_ENABLE_STACK_CHECKING
#undef TX_DISABLE_STACK_FILLING
#endif


/* Define the TX_THREAD control block extensions for this port. The main reason
   for the multiple macros is so that backward compatibility can be maintained with 
   existing ThreadX kernel awareness modules.  */

#if XCHAL_CP_NUM > 0
#define TX_CP_THREAD_EXTENSION  \
    /* Co-proc state save areas, with padding for alignment: */ \
    UINT                tx_thread_cp_state[(XT_CP_SIZE+3)/4];
#else
#define TX_CP_THREAD_EXTENSION
#endif

#define TX_THREAD_EXTENSION_0 \
    /* These extensions needs to be accessed from assembly code at context switches */ \
    UINT                tx_thread_solicited;    /* Non-zero indicates solicited entry */ \
    TX_CLIB_THREAD_EXTENSION                    /* Ptr to C library re-ent struct */ \
    TX_CP_THREAD_EXTENSION          /* Coprocessor state save areas */

#define TX_THREAD_EXTENSION_1

#define TX_THREAD_EXTENSION_2

#define TX_THREAD_EXTENSION_3 \
    TX_CLIB_THREAD_EXTENSION_END

/* Define the port extensions of the remaining ThreadX objects.  */

#define TX_BLOCK_POOL_EXTENSION
#define TX_BYTE_POOL_EXTENSION
#define TX_EVENT_FLAGS_GROUP_EXTENSION
#define TX_MUTEX_EXTENSION
#define TX_QUEUE_EXTENSION
#define TX_SEMAPHORE_EXTENSION
#define TX_TIMER_EXTENSION


/* Define the user extension field of the thread control block.  Nothing 
   additional is needed for this port so it is defined as white space.  */

#ifndef TX_THREAD_USER_EXTENSION
#define TX_THREAD_USER_EXTENSION    
#endif


/* Define the macros for processing extensions in tx_thread_create, tx_thread_delete,
   tx_thread_shell_entry, and tx_thread_terminate.  */

#if XCHAL_CP_NUM > 0
#define TX_THREAD_DELETE_EXTENSION(thread_ptr) \
        _xt_coproc_release(&thread_ptr->tx_thread_cp_state); \
        TX_CLIB_THREAD_DELETE_EXTENSION(thread_ptr);
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr) \
        _xt_coproc_release(&thread_ptr->tx_thread_cp_state); \
        TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr);
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr) \
        _xt_coproc_release(&thread_ptr->tx_thread_cp_state); \
        TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr);
#define TX_THREAD_CP_EXTENSION(thread_ptr) \
    /*thread_ptr->tx_thread_cp_state[0] = 0;*/ /*redundant*/ \
    /* Initialize XT_CP_ASA ptr to aligned save area: */ \
    thread_ptr->tx_thread_cp_state[1] = \
        ((((UINT)thread_ptr->tx_thread_cp_state)+8+XCHAL_TOTAL_SA_ALIGN-1) \
            & -XCHAL_TOTAL_SA_ALIGN);
#else
#define TX_THREAD_DELETE_EXTENSION(thread_ptr) \
        TX_CLIB_THREAD_DELETE_EXTENSION(thread_ptr);
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr) \
        TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr);
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr) \
        TX_CLIB_THREAD_EXIT_EXTENSION(thread_ptr);
#define TX_THREAD_CP_EXTENSION(thread_ptr)
#endif

#define TX_THREAD_CREATE_EXTENSION(thread_ptr) \
    /* _tx_thread_create has cleared *thread_ptr structure before initializing */ \
        TX_CLIB_THREAD_CREATE_EXTENSION(thread_ptr) \
    TX_THREAD_CP_EXTENSION(thread_ptr)


/* Define the ThreadX object creation extensions for the remaining objects.  */

#define TX_BLOCK_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_CREATE_EXTENSION(group_ptr)
#define TX_MUTEX_CREATE_EXTENSION(mutex_ptr)
#define TX_QUEUE_CREATE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_CREATE_EXTENSION(semaphore_ptr)
#define TX_TIMER_CREATE_EXTENSION(timer_ptr)


/* Define the ThreadX object deletion extensions for the remaining objects.  */

#define TX_BLOCK_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_DELETE_EXTENSION(group_ptr)
#define TX_MUTEX_DELETE_EXTENSION(mutex_ptr)
#define TX_QUEUE_DELETE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_DELETE_EXTENSION(semaphore_ptr)
#define TX_TIMER_DELETE_EXTENSION(timer_ptr)


/* Define ThreadX interrupt lockout and restore macros for protection on 
   access of critical kernel information.  The restore interrupt macro must 
   restore the interrupt posture of the running thread prior to the value 
   present prior to the disable macro.  In most cases, the save area macro
   is used to define a local function save area for the disable and restore
   macros.  */

extern unsigned int                 _tx_thread_interrupt_control(unsigned int new_posture);

#define TX_INTERRUPT_SAVE_AREA      register int interrupt_save;

#define TX_DISABLE                  interrupt_save = _tx_thread_interrupt_control(TX_INT_DISABLE);

#define TX_RESTORE                  _tx_thread_interrupt_control(interrupt_save);


/* Define the interrupt lockout macros for each ThreadX object.  */

#define TX_BLOCK_POOL_DISABLE                   TX_DISABLE
#define TX_BYTE_POOL_DISABLE                    TX_DISABLE
#define TX_EVENT_FLAGS_GROUP_DISABLE            TX_DISABLE
#define TX_MUTEX_DISABLE                        TX_DISABLE
#define TX_QUEUE_DISABLE                        TX_DISABLE
#define TX_SEMAPHORE_DISABLE                    TX_DISABLE

#if defined(AR6002_REV7)
extern void _txqc_init(void);
extern void _txqc_prep_threadx_scheduler(void);
extern int  _txqc_is_meta_scheduler_activity_pending(void);

#define TX_PORT_SPECIFIC_POST_INITIALIZATION            txqc_init();
#define TX_PORT_SPECIFIC_PRE_SCHEDULER_INITIALIZATION   txqc_prep_threadx_scheduler();
#define TX_TIMER_PROCESS_IN_ISR
#endif /* AR6002_REV7 */

#if defined(TXQC_CONFIG_USE_AR6K_TIMER)
#define TX_TIMER_INTERNAL_SYSTEM_INIT(x) _tx_timer_internal_system_init(x)
#else
#define TX_TIMER_INTERNAL_SYSTEM_INIT(x)
#endif

#if defined(TXQC_CONFIG_DYNAMIC_POOLS)
#define TX_POOL_CREATE_DYNAMIC ((VOID *)0xffffffff)
extern VOID *_txqc_pool_start(VOID *pool_start, ULONG pool_size);
extern VOID *_txqc_app_entry_get(VOID);
#endif /* TXQC_CONFIG_DYNAMIC_POOLS */


/* Define the version ID of ThreadX.  This may be utilized by the application.  */

#ifdef TX_THREAD_INIT
CHAR                            _tx_version_id[] = 
                                    "Copyright (c) 1996-2013 Express Logic Inc. * ThreadX Xtensa Version G5.6.5.7 SN: 3801-198-3201 *";
#else
extern  CHAR                    _tx_version_id[];
#endif

#endif /* #ifndef __ASSEMBLER__ */

#endif

