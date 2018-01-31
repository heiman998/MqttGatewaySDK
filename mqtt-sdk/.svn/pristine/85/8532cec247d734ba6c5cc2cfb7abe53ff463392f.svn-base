/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include <osapi.h>

A_INT32 sdkflash_main(A_UINT32 param);

A_INT32
app_start(A_UINT32 param)
{
    A_old_wdt_t old_wdt;

    A_INIT();

    A_WDT_DISABLE(&old_wdt);

    HOST_INTEREST->hi_refclk_hz = 40000000;
#ifndef  FPGA
    A_RTC_SOC_REG_WRITE(SOC_LPO_CAL_ADDRESS, SOC_LPO_CAL_ENABLE_MASK);
    A_CLOCK_INIT(HOST_INTEREST->hi_refclk_hz);
    A_CLOCKREGS_INIT();
    A_CPU_CLOCK_SELECT(AR6K_CPU_CLOCK_SELECT_CPU_PLL);
    HOST_INTEREST->hi_desired_baud_rate = 115200;
    HOST_INTEREST->hi_dbg_uart_txpin = 23;
#else
//    A_RTC_SOC_REG_WRITE(SOC_LPO_CAL_ADDRESS, SOC_LPO_CAL_ENABLE_MASK);
//    A_CLOCK_INIT(HOST_INTEREST->hi_refclk_hz);
//    A_CLOCKREGS_INIT();
//    A_CPU_CLOCK_SELECT(AR6K_CPU_CLOCK_SELECT_CPU_PLL);
    HOST_INTEREST->hi_desired_baud_rate = 115200; // 9600;
    HOST_INTEREST->hi_dbg_uart_txpin = 7; // 24;
#endif
    A_ENABLE_DEBUG_PRINT();

    A_PRINTF_ALWAYS("printf enabled\n");
    
    return sdkflash_main(param);
}

