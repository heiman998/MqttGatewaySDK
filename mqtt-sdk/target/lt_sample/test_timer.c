#include "qcom_common.h"
#include "qcom_gpio.h"

qcom_timer_t task_timer;

static void task_timer_handle(unsigned int alarm, void *arg)
{
    printf("timer wake up\n");
}


void test_timer()
{
    qcom_timer_init(&task_timer, task_timer_handle, NULL, 3000, ONESHOT);
    qcom_timer_start(&task_timer);

    qcom_thread_msleep(5000);
    qcom_timer_stop(&task_timer);
}
