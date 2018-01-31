#ifndef _QCA_GPIO_H_
#define _QCA_GPIO_H_

/**
* @fn void qca_gpio_set_pin_dir(int pin, A_BOOL Input)
* @brief set gpio direction
* @param pin GPIO pin to set
* @param Input 1 for input, 0 for output
* @return
*/
extern void qca_gpio_set_pin_dir(int pin, A_BOOL Input);

/**
* @fn void qca_gpio_set_pin_status(int pin, A_BOOL High)
* @brief set GPIO pin status
* @param pin GPIO pin to set
* @param High 1 for high level, 0 for low level
* @return
*/
extern void qca_gpio_set_pin_status(int pin, A_BOOL High);

/**
* @fn A_BOOL qca_gpio_get_pin_status(int pin)
* @brief get GPIO status
* @param pin GPIO pin to get
* @return 0 low level 
*         1 high level
*/
extern A_BOOL qca_gpio_get_pin_status(int pin);

/**
* @fn void qca_gpio_set_configuration(int pin, unsigned int value)
* @brief set GPIO tunable config
* @param pin GPIO to set
* @param value config value, see tunable document for detail
* @return
*/
extern void qca_gpio_set_configuration(int pin, unsigned int value);

#endif

