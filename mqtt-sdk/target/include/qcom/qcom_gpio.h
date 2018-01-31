/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __QCOM_GPIO_H__
#define __QCOM_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio_api.h" /* TBD: This does not belong here */

typedef enum {
    QCOM_GPIO_PIN_INT_NONE = 0,
    QCOM_GPIO_PIN_INT_RISING_EDGE,
    QCOM_GPIO_PIN_INT_FALLING_EDGE,
    QCOM_GPIO_PIN_INT_BOTH_EDGE,
    QCOM_GPIO_PIN_INT_LEVEL_LOW,
    QCOM_GPIO_PIN_INT_LEVEL_HIGH,
} QCOM_GPIO_PIN_INTERRUPT_MODE;

typedef enum {
    QCOM_GPIO_PIN_PULLNONE = 0,
    QCOM_GPIO_PIN_PULLUP,
    QCOM_GPIO_PIN_PULLDOWN
} QCOM_GPIO_PIN_PULL_TYPE;

/* Define macros for peripheral IDs for application */
#define QCOM_PERIPHERAL_ID_NONE         GPIO_PERIPHERAL_ID_NONE
#define QCOM_PERIPHERAL_ID_UART_0       GPIO_PERIPHERAL_ID_UART_0
#define QCOM_PERIPHERAL_ID_UART_1       GPIO_PERIPHERAL_ID_UART_1
#define QCOM_PERIPHERAL_ID_I2C          GPIO_PERIPHERAL_ID_I2C
#define QCOM_PERIPHERAL_ID_I2S          GPIO_PERIPHERAL_ID_I2S
#define QCOM_PERIPHERAL_ID_I2S_SLAVE    GPIO_PERIPHERAL_ID_I2S_SLAVE
#define QCOM_PERIPHERAL_ID_SPI_MASTER   GPIO_PERIPHERAL_ID_SPI_MASTER
#define QCOM_PERIPHERAL_ID_SPI_SLAVE    GPIO_PERIPHERAL_ID_SPI_SLAVE
#define QCOM_PERIPHERAL_ID_NOT_BONDED   GPIO_PERIPHERAL_ID_NOT_BONDED
#define QCOM_PERIPHERAL_ID_GPIO             GPIO_PERIPHERAL_ID_GPIO
#define QCOM_PERIPHERAL_ID_JTAG             GPIO_PERIPHERAL_ID_JTAG
#define QCOM_PERIPHERAL_ID_I2C_SLAVE        GPIO_PERIPHERAL_ID_I2C_SLAVE
#define QCOM_PERIPHERAL_ID_QUAD_SPI_MASTER  GPIO_PERIPHERAL_ID_QUAD_SPI_MASTER
#define QCOM_PERIPHERAL_ID_UART_2           GPIO_PERIPHERAL_ID_UART_2

/* GPIO peripheral IDs for GPIO mode */
/* (x + 1) is done here to avoid conflict between GPIO_PERIPHERAL_ID_GPIO and GPIO_PERIPHERAL_ID_GPIO_0. 
 * When getting the pin map for GPIO_PERIPHERAL_ID_GPIO_0, the obtained value is subtracted by 1 and pin 
 * number is returned */
#define QCOM_PERIPHERAL_ID_GPIOn(x)         (((x + 1) << GPIO_PERIPHERAL_PIN_NUM_SHIFT) | GPIO_PERIPHERAL_ID_GPIO)

typedef struct _qcom_gpio_interrupt_info_t {
    int         pin;
    void        (*gpio_pin_int_handler_fn)(void *arg);
    void        *arg;
    unsigned int internal_flags;
} qcom_gpio_interrupt_info_t;

/* Define macros gpio pin num of uart2 RTS for application */
#define GPIO_CONFIG_UART2_RTS_PIN     4

/* configure a GPIO pin's config value, direction and pullup/down type */
/* combo API, includes gpio_configure_mux/ direction /gpio_configure_pulltype */
//A_STATUS qcom_gpio_config_pin(A_UINT32 pin, A_UINT8 config_val, A_UINT8 input, QCOM_GPIO_PIN_PULL_TYPE pull_type) ;


#if 0
void qcom_gpio_pin_mux(A_UINT32 pin, A_UINT8 config_val);
void qcom_gpio_pin_pad(A_UINT32 pin, QCOM_GPIO_PIN_PULL_TYPE pull_type,
	                                       A_UINT8 strength, A_BOOL open_drain);

/*set the GPIO direction for specific PIN*/
void qcom_gpio_pin_dir(A_UINT32 pin, A_BOOL input) ;

/* set output high, note: pin must be configured as an output */
void qcom_gpio_pin_set(A_UINT32 pin,  	A_BOOL val);

/* get input state of pin, TRUE = H, FALSE = L */
A_BOOL qcom_gpio_pin_get(A_UINT32 pin);

void qcom_gpio_pin_source(A_UINT32 pin, A_BOOL pwm);

void qcom_gpio_interrupt_wakeup(A_UINT32 pin, A_BOOL enable);

/* register for GPIO pin interrupts */
A_STATUS qcom_gpio_interrupt_register(qcom_gpio_interrupt_info_t * gpio_interrupt_info);

/* de-register previously registered interrupts */
void qcom_gpio_interrupt_deregister(qcom_gpio_interrupt_info_t *gpio_interrupt_info);

/* set the interrupting mode for the GPIO pin, must have a registered handler */
void qcom_gpio_interrupt_mode(qcom_gpio_interrupt_info_t *gpio_int_info, QCOM_GPIO_PIN_INTERRUPT_MODE Mode);

/*enable GPIO interrupts on specific GPIO pin*/
void qcom_gpio_interrupt_start(qcom_gpio_interrupt_info_t *gpio_int_info);
#endif 


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {
	void (*_qcom_gpio_pin_mux)(A_UINT32 pin, A_UINT8 config_val);
	A_STATUS (*_qcom_gpio_pin_pad)(A_UINT32 pin, QCOM_GPIO_PIN_PULL_TYPE pull_type,
		                                       A_UINT8 strength, A_BOOL open_drain);
	A_STATUS (*_qcom_gpio_pin_dir)(A_UINT32 pin, A_BOOL input);
	A_STATUS (*_qcom_gpio_pin_set)(A_UINT32 pin,  	A_BOOL val);
	A_BOOL (*_qcom_gpio_pin_get)(A_UINT32 pin);
	A_STATUS (*_qcom_gpio_pin_source)(A_UINT32 pin, A_BOOL pwm);

	void (*_qcom_gpio_interrupt_wakeup)(A_UINT32 pin, A_BOOL enable);
	A_STATUS (*_qcom_gpio_interrupt_register)(qcom_gpio_interrupt_info_t * gpio_interrupt_info);
	void (*_qcom_gpio_interrupt_deregister)(qcom_gpio_interrupt_info_t *gpio_interrupt_info);
	A_STATUS (*_qcom_gpio_interrupt_mode)(qcom_gpio_interrupt_info_t *gpio_int_info, 
								QCOM_GPIO_PIN_INTERRUPT_MODE Mode);
	void (*_qcom_gpio_interrupt_start)(qcom_gpio_interrupt_info_t *gpio_int_info);
#ifndef AR6002_REV74
    A_STATUS (*_qcom_gpio_apply_peripheral_configuration)(A_UINT32 id, A_BOOL low_power_mode);

#endif
	A_STATUS (*_qcom_gpio_add_alternate_configurations)(A_UINT32 pin_num, A_UINT32 num_configs, gpio_pin_peripheral_config_t* pPinConfigs);
    A_STATUS (*_qcom_gpio_get_interrupt_pin_num)(A_UINT32 interrupt_num, A_UINT32 *pPinNum);
    A_STATUS (*_qcom_gpio_peripheral_pin_conflict_check)(A_UINT32 peripheral_id, A_UINT32 *pPeripheralMap);
#ifndef AR6002_REV76
    A_STATUS (*_qcom_gpio_apply_configuration_by_pin_peripheral_id)(A_UINT32 pin_num, A_UINT32 peripheral_id, A_BOOL low_power_mode);
#endif
}QCOM_GPIO_API_INDIRECTION_TABLE;

extern QCOM_GPIO_API_INDIRECTION_TABLE qcomGpioApiIndirectionTable;
#define QCOM_GPIO_API_FN(fn) qcomGpioApiIndirectionTable.fn

#else /* LIBCUST_INDIRECT_ACCESS */
void _qcom_gpio_pin_mux(A_UINT32 pin, A_UINT8 config_val);
A_STATUS _qcom_gpio_pin_pad(A_UINT32 pin, QCOM_GPIO_PIN_PULL_TYPE pull_type,
		                                       A_UINT8 strength, A_BOOL open_drain);
A_STATUS _qcom_gpio_pin_dir(A_UINT32 pin, A_BOOL input);
A_STATUS _qcom_gpio_pin_set(A_UINT32 pin,  	A_BOOL val);
A_BOOL _qcom_gpio_pin_get(A_UINT32 pin);
A_STATUS _qcom_gpio_pin_source(A_UINT32 pin, A_BOOL pwm);

void _qcom_gpio_interrupt_wakeup(A_UINT32 pin, A_BOOL enable);
A_STATUS _qcom_gpio_interrupt_register(qcom_gpio_interrupt_info_t * gpio_interrupt_info);
void _qcom_gpio_interrupt_deregister(qcom_gpio_interrupt_info_t *gpio_interrupt_info);
A_STATUS _qcom_gpio_interrupt_mode(qcom_gpio_interrupt_info_t *gpio_int_info, 
								QCOM_GPIO_PIN_INTERRUPT_MODE Mode);
void _qcom_gpio_interrupt_start(qcom_gpio_interrupt_info_t *gpio_int_info);

A_STATUS _qcom_gpio_apply_peripheral_configuration(A_UINT32 peripheral_id, A_BOOL low_power_mode);
A_STATUS _qcom_gpio_add_alternate_configurations(A_UINT32 pin_num, A_UINT32 num_configs, qcom_pin_peripheral_config_t *pPinConfigs);
A_STATUS _qcom_gpio_get_interrupt_pin_num(A_UINT32 interrupt_num, A_UINT32 *pPinNum);
A_STATUS _qcom_gpio_peripheral_pin_conflict_check(A_UINT32 peripheral_id, A_UINT32 *pPeripheralMap);

#ifndef AR6002_REV76
A_STATUS _qcom_gpio_apply_configuration_by_pin_peripheral_id(A_UINT32 pin_num, A_UINT32 peripheral_id, A_BOOL low_power_mode);
#endif

#define QCOM_GPIO_API_FN(fn) (fn)

#endif /* LIBCUST_INDIRECT_ACCESS */

#define qcom_gpio_pin_mux(_p, _v) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_mux((_p), (_v)))

#define qcom_gpio_pin_pad(_p, _t, _s, _od) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_pad((_p), (_t), (_s), (_od)))

#define qcom_gpio_pin_dir(_p, _i) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_dir((_p), (_i)))

#define qcom_gpio_pin_set(_p, _v) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_set((_p), (_v)))

#define qcom_gpio_pin_get(_p) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_get((_p)))

#define qcom_gpio_pin_source(_p, _pwm) \
    QCOM_GPIO_API_FN(_qcom_gpio_pin_source((_p), (_pwm)))

#define qcom_gpio_interrupt_wakeup(_p, _e) \
    QCOM_GPIO_API_FN(_qcom_gpio_interrupt_wakeup((_p), (_e)))

#define qcom_gpio_interrupt_register(_pi) \
    QCOM_GPIO_API_FN(_qcom_gpio_interrupt_register((_pi)))

#define qcom_gpio_interrupt_deregister(_pi) \
    QCOM_GPIO_API_FN(_qcom_gpio_interrupt_deregister((_pi)))

#define qcom_gpio_interrupt_mode(_pi, _m) \
    QCOM_GPIO_API_FN(_qcom_gpio_interrupt_mode((_pi), (_m)))

#define qcom_gpio_interrupt_start(_pi) \
    QCOM_GPIO_API_FN(_qcom_gpio_interrupt_start((_pi)))

#ifndef AR6002_REV74

#define qcom_gpio_apply_peripheral_configuration(id, low_pwr_mode) \
    QCOM_GPIO_API_FN(_qcom_gpio_apply_peripheral_configuration((id), (low_pwr_mode)))    

#else   /* AR6002_REV74 */

void _qcom_gpio_apply_peripheral_configuration(A_UINT16 id, A_BOOL low_power_mode);


#define qcom_gpio_apply_peripheral_configuration(id, low_pwr_mode) \
            (_qcom_gpio_apply_peripheral_configuration((id), (low_pwr_mode)))
#endif
    

#define qcom_gpio_add_alternate_configurations(pin_num, num_configs, pPinConfigs) \
    QCOM_GPIO_API_FN(_qcom_gpio_add_alternate_configurations((pin_num), (num_configs), (pPinConfigs)))

#define qcom_gpio_get_interrupt_pin_num(interrupt_num, pPinNum) \
    QCOM_GPIO_API_FN(_qcom_gpio_get_interrupt_pin_num((interrupt_num), (pPinNum)))

#define qcom_gpio_peripheral_pin_conflict_check(peripheral_id, pPeripheralMap) \
    QCOM_GPIO_API_FN(_qcom_gpio_peripheral_pin_conflict_check((peripheral_id), (pPeripheralMap)))

A_STATUS _qcom_gpio_apply_configuration_by_pin(A_UINT32 pin_num, A_BOOL low_power_mode);
#define qcom_gpio_apply_configuration_by_pin(pin_num, low_pwr_mode) \
            (_qcom_gpio_apply_configuration_by_pin((pin_num), (low_pwr_mode)))

#ifndef AR6002_REV76
#define qcom_gpio_apply_configuration_by_pin_peripheral_id(id, peripheral_id, low_pwr_mode) \
    QCOM_GPIO_API_FN(_qcom_gpio_apply_configuration_by_pin_peripheral_id((id), (peripheral_id), (low_pwr_mode)))    
#else   /* AR6002_REV76 */
A_STATUS _qcom_gpio_apply_configuration_by_pin_peripheral_id(A_UINT32 pin_num, A_UINT32 peripheral_id, A_BOOL low_power_mode);
#define qcom_gpio_apply_configuration_by_pin_peripheral_id(id, peripheral_id, low_pwr_mode) \
            (_qcom_gpio_apply_configuration_by_pin_peripheral_id((id), (peripheral_id), (low_pwr_mode)))
#endif

#ifdef __cplusplus
}
#endif

#endif

