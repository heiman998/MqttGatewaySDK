/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
#include "base.h"
#include "threadx/tx_api.h"
#include "qcom_system.h"
#include "threadxdmn_api.h"
#include "qca_uart.h"

#define PRINTF_ENBALE 1

#define BYTE_POOL_SIZE (8*1024 + 256 )
#define PSEUDO_HOST_STACK_SIZE (8 * 1024 )   /* small stack for pseudo-Host thread */

TX_THREAD		host_thread;
TX_BYTE_POOL	pool;


/**
* @fn void user_entry(ULONG which_thread)
* @brief 
* @param 
* @return 
*/
void user_at_entry(ULONG which_thread)
{
    extern void user_pre_init(void);
    user_pre_init();

	qcom_watchdog(1,5000);

	qcom_enable_print(PRINTF_ENBALE);

	extern void LTM3210_run(void);
	LTM3210_run();

}

/**
* @fn void qca_user_main(void)
* @brief user main
* @param none
* @return none
*/
void qca_at_main(void)
{   
	char *pointer;

    tx_byte_pool_create(&pool, "cdrtest pool", TX_POOL_CREATE_DYNAMIC, BYTE_POOL_SIZE);

	tx_byte_allocate(&pool, (VOID **) & pointer, PSEUDO_HOST_STACK_SIZE, TX_NO_WAIT);

	tx_thread_create(&host_thread, "cdrtest thread", user_at_entry,
		0, pointer, PSEUDO_HOST_STACK_SIZE, 16, 16, 4, TX_AUTO_START);

    cdr_threadx_thread_init();
}

