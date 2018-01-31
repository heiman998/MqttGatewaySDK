/*
    qca_adc test case

    Add the code below to you project to test

>    extern void test_adc(void);
>    test_adc();
 */
#include "sample_main.h"
#include "qca_gpio.h"
#include "qca_adc.h"

#define ADC_CHANNEL 1 

#define PIN_NUM 12 //PWM6
#define OUTPUT 0
#define HIGH 1

void test_adc(void)
{
    int ret;

    printf("acd function testing......\r\n");

    qca_adc_init(ADC_CHANNEL);
    
    qcom_thread_msleep(1000);

    qca_gpio_set_pin_dir(PIN_NUM, OUTPUT);
   
    printf("set pin num: %d status: HIGH\r\n", PIN_NUM);
    qca_gpio_set_pin_status(PIN_NUM, HIGH);

    ret = qca_adc_read(ADC_CHANNEL);
    printf("adc_%d value: %d\r\n", ADC_CHANNEL, ret);

    qcom_thread_msleep(1000);

    qca_adc_close();

    printf("adc function test done.\r\n");
}
