/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: internal header file for CLI module
//
// @Notes:
//
//

#ifndef CONSOLE_INTERNAL_H_
#define CONSOLE_INTERNAL_H_

#include "console_api.h"

#define CONSOLE_PROMPT           "shell> "
#define ASCII_ESCAPE              0x1B
#define ASCII_CARRIAGE_RETURN     0x0D
#define ASCII_BACK_SPACE          0x08
#define ASCII_BELL                "\x07"
#define ANSI_VT100_CURSOR_FWD     "\x1b[C" 
#define ANSI_VT100_CURSOR_BACK    "\x1b[D"
#define ANSI_VT100_CLEAR_LINE     "\x1b[2K"
#define CONSOLE_PROMPT_LINE       "\n"CONSOLE_PROMPT
#define CURSOR_FORWARD            TRUE
#define CURSOR_BACKWARD           FALSE
#define MIN_ESCAPE_BYTES                3
#define MAX_ESCAPE_BYTES                4
#define CONSOLE_RECV_BUFFER_SIZE        64
#define CONSOLE_TRANSMIT_BUFFER_SIZE    1024
#define CONSOLE_DEF_MAX_LINE_CHARS      256
#define CONSOLE_DEF_MAX_HISTORY         4
#define CONSOLE_DEF_BAUDRATE            9600
#define CONSOLE_DEF_MAX_ARGC            24
#define CONSOLE_DEF_MAX_ARGV_LENGTH     32
#define CONSOLE_SLOW_POLL_INTERVAL_MS   1000
#define CONSOLE_FAST_POLL_INTERVAL_MS   10
#define CONSOLE_RECV_IDLE_TIMEOUT_MS    5000

typedef enum {
    MEMORY_ACCESS_NONE = 0,
    MEMORY_ACCESS_BYTE = 1,
    MEMORY_ACCESS_SHORT,
    MEMORY_ACCESS_WORD    
} MEMORY_ACCESS_WIDTH;

typedef struct  {
    MEMORY_ACCESS_WIDTH     width;
    A_UINT32                flags;
    A_UINT32                address;
    A_UINT32                write_val;
    A_UINT32                count;
} MEMORY_ACCESS_OPTIONS;

#define ADDRESS_OPTION       (1 << 0)
#define WRITE_VALUE_OPTION   (1 << 1)

typedef struct _CONSOLE_INFO {    
    CONSOLE_COMMAND *command_list;
    void            *port;
    A_CHAR          *history_buffer;
    int             history_length;
    int             history_write_index;
    int             history_read_index;
    CONSOLE_CONFIG  config;
    A_CHAR          *current_line;
    int             chars_avail;
    int             cursor_pos;
    A_CHAR          escape_buffer[MAX_ESCAPE_BYTES];
    int             current_escape_pos;
    A_CHAR          *argv_buffers[CONSOLE_DEF_MAX_ARGC];
    A_UINT32        flags;
    A_timer_t       recv_idle_timer;
} CONSOLE_INFO;

#define CONSOLE_FLAGS_RX_SUSPENDED     (1 << 0)
#define CONSOLE_FLAGS_ALLOW_RX_SUSPEND (1 << 1)
#define CONSOLE_FLAGS_USE_HW_RX_SUSP   (1 << 2)
#define CONSOLE_FLAGS_RX_WAKE_ENABLED  (1 << 3)

void    _refresh_console_line(void);
A_BOOL  _insert_console_char(A_CHAR character);
A_BOOL  _process_console_backspace(void);
A_BOOL  _process_console_delete(void);
A_BOOL  _move_console_cursor(A_BOOL forward);
A_CHAR  *_get_console_history_buf(int index);
void    _process_console_line(void);
void    _show_console_history(A_BOOL Up);
void    _console_process_char(void);
int     _do_memory_op(void *pAddress, A_BOOL read, A_UINT32 write_value, MEMORY_ACCESS_WIDTH width);
int     _get_mem_access_options(int argc, A_CHAR *argv[], MEMORY_ACCESS_OPTIONS *options);

typedef struct UART_SERP_INTERNAL_APIS {
   void    (*_refresh_console_line)(void);
   A_BOOL  (*_insert_console_char)(A_CHAR character);
   A_BOOL  (*_process_console_backspace)(void);
   A_BOOL  (*_process_console_delete)(void);
   A_BOOL  (*_move_console_cursor)(A_BOOL forward);
   A_CHAR  *(*_get_console_history_buf)(int index);
   void    (*_process_console_line)(void);
   void    (*_show_console_history)(A_BOOL Up);
   void    (*_console_process_char)(void);
   int     (*_do_memory_op)(void *pAddress, A_BOOL read, A_UINT32 write_value, MEMORY_ACCESS_WIDTH width);
    int    (*_get_mem_access_options)(int argc, A_CHAR *argv[], MEMORY_ACCESS_OPTIONS *options);
} CONSOLE_INTERNAL_APIS;

extern CONSOLE_INTERNAL_APIS ConsoleIndirectionTable;
   
#define CALL_CONSOLE_INDIR_FN(fn) ConsoleIndirectionTable.fn
#define GET_CONSOLE_INDIR_FN(fn)  ConsoleIndirectionTable.fn

#define refresh_console_line()          CALL_CONSOLE_INDIR_FN(_refresh_console_line())
#define insert_console_char(c)          CALL_CONSOLE_INDIR_FN(_insert_console_char(c))
#define process_console_backspace()     CALL_CONSOLE_INDIR_FN(_process_console_backspace())
#define process_console_delete()        CALL_CONSOLE_INDIR_FN(_process_console_delete())
#define move_console_cursor(f)          CALL_CONSOLE_INDIR_FN(_move_console_cursor(f))
#define get_console_history_buf(i)      CALL_CONSOLE_INDIR_FN(_get_console_history_buf(i))
#define process_console_line()          CALL_CONSOLE_INDIR_FN(_process_console_line())   
#define show_console_history(u)         CALL_CONSOLE_INDIR_FN(_show_console_history(u))  
#define console_process_char()          CALL_CONSOLE_INDIR_FN(_console_process_char())
#define do_memory_op(p,r,w,t)           CALL_CONSOLE_INDIR_FN(_do_memory_op(p,r,w,t))
#define get_mem_access_options(c,v,o)   CALL_CONSOLE_INDIR_FN(_get_mem_access_options(c,v,o))


#endif /*CONSOLE_INTERNAL_H_*/
