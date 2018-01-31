/*
    qca_aes test case

    Add the code below to you project to test

>    extern void test_aes(void);
>    test_aes();
 */
#include "sample_main.h"
#include "qca_gpio.h"

#define INPUT 1
#define OUTPUT 0

#define HIGH 1
#define LOW 0

#define PIN_NUM_1 11 //PWM5_I2CS_SDA0
#define PIN_NUM_2 6 //PWM0

void test_gpio(void)
{
    A_BOOL flag;

    printf("gpio testing......\r\n");

    printf("set pin num: %d direction: INPUT\r\n", PIN_NUM_1);
    qca_gpio_set_pin_dir(PIN_NUM_1, INPUT);

    printf("set pin num: %d direction: OUTPUT\r\n", PIN_NUM_2);
    qca_gpio_set_pin_dir(PIN_NUM_2, OUTPUT);

    printf("set pin num: %d status: LOW\r\n", PIN_NUM_2);
    qca_gpio_set_pin_status(PIN_NUM_2, LOW);

    flag = qca_gpio_get_pin_status(PIN_NUM_1);
    if(flag == 1){
        printf("get pin num: %d status: HIGH\r\n", PIN_NUM_1);
    }
    else if(flag == 0){
        printf("get pin num :%d status: LOW\r\n", PIN_NUM_1);
    }
    else{
        printf("get pin num :%d status err.\r\n", PIN_NUM_1);
    }

    printf("gpio test done.\r\n");
}

