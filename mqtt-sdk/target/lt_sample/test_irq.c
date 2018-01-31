
#include "qcom_common.h"
#include "qcom_gpio.h"

#include "qca_gpio.h"

#define NETWORK_BUTTON_GPIO 30
#define SHOW_GPIO 6

int led_val = 0;

static void network_button_ir_handler(void *arg)
{
    if(led_val == 0){
        led_val = 1;
    }else{
        led_val = 0;
    }
    qca_gpio_set_pin_status(SHOW_GPIO, led_val);
}

void test_irq()
{
    qcom_gpio_interrupt_info_t network_button;

    qca_gpio_set_configuration(SHOW_GPIO, 0x80000000);
    qca_gpio_set_pin_dir(SHOW_GPIO, 0);
    qca_gpio_set_pin_status(SHOW_GPIO, led_val);

    qcom_gpio_pin_pad(NETWORK_BUTTON_GPIO, QCOM_GPIO_PIN_PULLUP, 0, 0);

    network_button.pin = NETWORK_BUTTON_GPIO;
	network_button.gpio_pin_int_handler_fn = network_button_ir_handler;
	network_button.arg = NULL;
	qcom_gpio_interrupt_register(&network_button);
    qcom_gpio_pin_dir(NETWORK_BUTTON_GPIO, 1);
	qcom_gpio_interrupt_mode(&network_button, QCOM_GPIO_PIN_INT_FALLING_EDGE);
	qcom_gpio_interrupt_start(&network_button);
}