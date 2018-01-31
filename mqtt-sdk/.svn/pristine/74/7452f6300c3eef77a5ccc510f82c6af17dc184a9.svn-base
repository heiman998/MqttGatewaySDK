#include "sample_main.h"
#include "qcom_pwm_api.h"

#define  PWM5		5		//GPIO11
#define  PWM2		2		//GPIO8
#define  PWM0		0		//GPIO8
#define  PWM7		7		//GPIO8

void test_pwm(void)
{
	while(1)
	{
		qcom_pwm_port_set(100000, 5000, 0, PWM5, 2);
		qcom_pwm_port_set(100000, 5000, 0, PWM2, 2);
		qcom_pwm_port_set(100000, 5000, 0, PWM0, 2);
		qcom_pwm_port_set(100000, 5000, 0, PWM7, 2);
		qcom_pwm_control(0, 1, PWM_PORT_0|PWM_PORT_2|PWM_PORT_5|PWM_PORT_7);
		qcom_thread_msleep(1);
		qcom_pwm_control(0, 0, PWM_PORT_0|PWM_PORT_2|PWM_PORT_5|PWM_PORT_7);

		qcom_thread_msleep(100);
	}
}
