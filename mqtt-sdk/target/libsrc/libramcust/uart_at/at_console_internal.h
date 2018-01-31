/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

//
// @File:
//
// @Abstract: internal header file for AT module
//
// @Notes:
//
//

#ifndef AT_CONSOLE_INTERNAL_H_
#define AT_CONSOLE_INTERNAL_H_

#include "console_api.h"

#define ASCII_CARRIAGE_RETURN     0x0D
#define CONSOLE_RECV_BUFFER_SIZE        2000
#define CONSOLE_TRANSMIT_BUFFER_SIZE    1024
#define CONSOLE_DEF_MAX_LINE_CHARS      256
#define CONSOLE_DEF_MAX_HISTORY         4
#define CONSOLE_DEF_BAUDRATE            9600
#define CONSOLE_DEF_MAX_ARGC            16
#define CONSOLE_DEF_MAX_ARGV_LENGTH     32
#define CONSOLE_SLOW_POLL_INTERVAL_MS   1000
#define CONSOLE_FAST_POLL_INTERVAL_MS   10
#define CONSOLE_RECV_IDLE_TIMEOUT_MS    5000
#define MAX_ESCAPE_BYTES 4
#define CONSOLE_DEF_MAX_ARGC 16

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

#endif /*CONSOLE_INTERNAL_H_*/
