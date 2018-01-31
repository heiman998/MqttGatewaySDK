
#ifndef _QCA_TASK_MANAGE_H_
#define _QCA_TASK_MANAGE_H_


//#include "threadx/tx_api.h"

/**
***************************************************************************
*@brief		:	create the user task
*@param[in]	:	void (*fn) (unsigned int)	entry function
*				arg							entry input
*			:	stk_size					task stack
*@return	:	0	success
*				-1	failed
*@see		:
***************************************************************************
*/
int qca_task_start(void(*fn) (unsigned int), unsigned int arg, int stk_size);

/***************************************************************************
*@brief		:	exit current thread
*@param[in]	:	none
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_task_exit(void);

/**
***************************************************************************
*@brief		:	get current thread pointer
*@param[in]	:	none
*@return	:	thread_ptr	thread pointer
*@warning	:
*@see		:
***************************************************************************
*/
TX_THREAD * qca_get_task_thread(void);

/**
***************************************************************************
*@brief		:	suspend current thread
*@param[in]	:	none
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_task_suspend(void);

/**
***************************************************************************
*@brief		:	resume thread
*@param[in]	:	thread_ptr	thread pointer
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_task_resume(TX_THREAD *thread_ptr);

/**
***************************************************************************
*@brief		:	kill all task
*@param[in]	:	none
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_task_kill_all(void);


#endif

