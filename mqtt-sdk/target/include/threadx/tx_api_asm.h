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

/**************************************************************************/
/*                                                                        */
/*  THREADX API INFO FOR ASM SOURCES                       RELEASE        */
/*                                                                        */
/*    tx_api_asm.h                                        Xtensa/GNU      */
/*                                                           5.7          */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ross Morley, Tensilica, Inc.                                        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains macro constants for ThreadX API structures       */
/*    and enums that need to be used in assembly coded port sources.      */
/*    Most of these constants are derived from definitions in tx_api.h.   */
/*    Only the constants that are needed are included here to reduce      */
/*    the maintenance required when the structures or enums change.       */
/*    Structure offsets depend on the compiler, so are tools-specific,    */
/*    which usually means port-specific since a compiler's struct         */
/*    packing rules depend on properties of the target architecture.      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-10-2006     Ross Morley              Initial Xtensa/GNU            */
/*                                            Support Version 5.0         */
/*  04-02-2007     Ross Morley              Modified comment(s),          */ 
/*                                            resulting in version 5.1    */ 
/*  06-18-2008     Ross Morley              Modified comment(s), and      */ 
/*                                            fixed bug whereby           */ 
/*                                            tx_thread_cp_state was off  */ 
/*                                            by 1 word and overlapped    */ 
/*                                            tx_thread_name if           */ 
/*                                            TX_THREAD_SAFE_NEWLIB was   */ 
/*                                            was not defined, resulting  */ 
/*                                            in version 5.2              */ 
/*  07-15-2009     Ross Morley              Modified comment(s),          */ 
/*                                            resulting in version 5.3    */ 
/*  05-01-2010     William E. Lamie         Modified comment(s),          */ 
/*                                            resulting in version 5.4    */ 
/*  01-28-2011     William E. Lamie         Modified comment(s),          */ 
/*                                            resulting in version 5.5    */ 
/*  07-15-2011     William E. Lamie         Modified comment(s),          */ 
/*                                            resulting in version 5.6    */ 
/*  10-01-2011     William E. Lamie         Modified comment(s),          */ 
/*                                            resulting in version 5.7    */ 
/*                                                                        */
/**************************************************************************/

#ifndef TX_API_ASM_H
#define TX_API_ASM_H

#include "txqc_indirection.h"


/* API input parameters and general constants.  */

#define TX_TRUE                         1
#define TX_FALSE                        0
#define TX_NULL                         0

/* ThreadX thread control block structure.  */
//  typedef struct TX_THREAD_STRUCT
//  {

#define tx_thread_run_count             0x04
#define tx_thread_stack_ptr             0x08
#define tx_thread_stack_end             0x10
#define tx_thread_time_slice            0x18
#define tx_thread_new_time_slice        0x1C
#define tx_thread_solicited             0x28

#ifdef TX_THREAD_SAFE_NEWLIB
#define tx_thread_clib_ptr              0x2C
#define tx_thread_cp_state              0x30
#else
#define tx_thread_cp_state              0x2C
#endif

//  }

#endif /* TX_API_ASM_H */
