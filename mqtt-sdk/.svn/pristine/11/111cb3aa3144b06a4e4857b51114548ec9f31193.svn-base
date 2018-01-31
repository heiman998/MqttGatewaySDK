/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include <osapi.h>
#include "console_api.h"

static struct console_apis console;

extern void at_console_module_install(struct console_apis *pAPIs);

void
at_console_setup(void)
{
    CONSOLE_CONFIG at_settings;

    SERP_MODULE_INSTALL();
    SERP_Init();
    UARTSERP_HW_MODULE_INSTALL();

    at_console_module_install(&console);

    /* use all defaults */
    A_MEMZERO(&at_settings, sizeof (at_settings));

    /* because the EEPROM dump is rather large, make sure output buffer is sized appropriately */
    at_settings.output_buffer_size = 4096;
    at_settings.phys_uart_id = 0;
    at_settings.baud = 115200;
    at_settings.flags |= CONSOLE_CONFIG_FLAGS_AWAKE_FOR_READ;
    at_settings.logical_port_name = (A_CHAR *) "UART0";

    console._init(&at_settings);

    console._start();
}
