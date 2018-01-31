/*
    qca_task_manage.c test case

    Add the code below to your project:
>   extern void test_task_manage(void);
>   test_task_manage();

 */
#include "sample_main.h"
#include "qca_task_manage.h"

#define TASK_NUM 3
#define TAST_STACK_SIZE 1024

TX_THREAD *thread_point[TASK_NUM];

static void task_handle(unsigned int arg)
{
	printf("in task %d\n", arg);

	/* Store current thread's id */
	thread_point[arg] = qca_get_task_thread();

	/* Suspend it until other thread resume it */
	printf("task %d suspend\n", arg);
	qca_task_suspend();
	/* Resume by main thread */
	printf("task %d resume\n", arg);

	if(arg == 0){
		printf("task %d exit by itself\n",arg);
		qca_task_exit();
		printf("task %d exit failed\n", arg);
	}

	/* Else continue */
	while(1){
		printf("task %d continue\n", arg);

		qcom_thread_msleep(1000);
	}
}

void test_task_manage(void)
{
	int i;

	/* Create 3 tasks */
	for(i = 0; i < TASK_NUM; i++){
		qca_task_start(task_handle, i, TAST_STACK_SIZE);
		qcom_thread_msleep(1000);
	}

	qcom_thread_msleep(2000);

	/* Resume all task */
	for(i = 0; i < TASK_NUM; i++){
		qca_task_resume(thread_point[i]);
	}

	qcom_thread_msleep(2000);

	/* Kill all task */
	printf("Kill all task\n");
	qca_task_kill_all();
}
