/*
    qca_task_manage.c test case

    Add the code below to your project:
>   extern void test_lib_version(void);
>   test_lib_version();

 */
#include "sample_main.h"

#include "qca_lib_version.h"

#define VERSION_BUF_SIZE 64

void test_lib_version()
{
	char version[VERSION_BUF_SIZE];

	qca_get_lib_version(version);

	PRT_T_VERSION("QCA Library Version is %s\n", version);
}
