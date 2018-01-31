/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_CLI_H__
#define __QCOM_CLI_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _console_cmd {  
    struct _console_cmd *next;
    char    *name;                  /* command name */
    char    *description;           /* optional description */
    int       (*execute)(int argc, char *argv[]);  /* execute callback */
    void      *context;               /* context to pass to callback */
} console_cmd_t;                                                       

void console_setup(void);
void console_reg_cmds(console_cmd_t *cmds, int cmds_num);
int console_printf(const char *fmt, ...);

A_UINT32 qcom_console_get_free_txbuf_sz(void);

#ifdef __cplusplus
}
#endif

#endif

