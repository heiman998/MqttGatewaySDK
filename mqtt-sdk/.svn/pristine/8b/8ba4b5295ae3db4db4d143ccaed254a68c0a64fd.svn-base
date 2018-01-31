/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

//#include "qcom_system.h"
#include "qcom_common.h"
#include "threadx/tx_api.h"
#include "qcom_cli.h"
#include "threadxdmn_api.h"
#include "qcom_wps.h"
#include "qcom_gpio.h"
#include "qcom_gpio_interrupts.h"


TX_THREAD host_thread;
#ifdef REV74_TEST_ENV4

#define BYTE_POOL_SIZE (2*1024 + 128 )
#define PSEUDO_HOST_STACK_SIZE (2 * 1024 )   /* small stack for pseudo-Host thread */

#else

#define BYTE_POOL_SIZE (4*1024 + 256 )
#define PSEUDO_HOST_STACK_SIZE (4 * 1024 )   /* small stack for pseudo-Host thread */

#endif
TX_BYTE_POOL pool;

void
shell_host_entry(ULONG which_thread)
{
    #define PRINTF_ENBALE 1
    extern void user_pre_init(void);
    user_pre_init();
    qcom_enable_print(PRINTF_ENBALE);

    extern int main(void);
    main();
}


void user_main(void)
{   
    extern void task_execute_cli_cmd();
    tx_byte_pool_create(&pool, "cdrtest pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);

    {
        CHAR *pointer;
        tx_byte_allocate(&pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

        tx_thread_create(&host_thread, "cdrtest thread", shell_host_entry,
                         0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
    }

    cdr_threadx_thread_init();
}

