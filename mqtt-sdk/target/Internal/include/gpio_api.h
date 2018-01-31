/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#ifndef __GPIO_API_H__
#define __GPIO_API_H__

#include "gpio.h"

/* API for Target-side General Purpose I/O */

#if defined(AR6002_REV4)
#define GPIO_PIN_COUNT AR6003_GPIO_PIN_COUNT
#endif
#if defined(AR6002_REV6)
#define GPIO_PIN_COUNT AR6004_GPIO_PIN_COUNT
#endif
#if defined(AR6002_REV7)
#define GPIO_PIN_COUNT AR6006_GPIO_PIN_COUNT
#endif

#if GPIO_PIN_COUNT > 32
#define A_GPIO_MASK     A_UINT64
#else
#define A_GPIO_MASK     A_UINT32
#endif

/* Peripheral ID internally overloaded with ID and other info as follows */
#define GPIO_PERIPHERAL_ID_MASK         0xFFFF
#define GPIO_PERIPHERAL_PIN_NUM_MASK    0xFF000000

#define GPIO_PERIPHERAL_ID_SHIFT        0
#define GPIO_PERIPHERAL_PIN_NUM_SHIFT   24

/* GPIO peripheral IDs supported by Qualcomm hardware*/
#define GPIO_PERIPHERAL_ID_NONE     0
#define GPIO_PERIPHERAL_ID_UART_0   1
#define GPIO_PERIPHERAL_ID_UART_1   2
#define GPIO_PERIPHERAL_ID_I2C      3
#define GPIO_PERIPHERAL_ID_I2S      4
#define GPIO_PERIPHERAL_ID_I2S_SLAVE    5
#define GPIO_PERIPHERAL_ID_SPI_MASTER   6
#define GPIO_PERIPHERAL_ID_SPI_SLAVE    7
#define GPIO_PERIPHERAL_ID_NOT_BONDED   8
#define GPIO_PERIPHERAL_ID_PWM_HW	        0x9
#define GPIO_PERIPHERAL_ID_PWM_SW	        0xA
#define GPIO_PERIPHERAL_ID_GPIO             0xB
#define GPIO_PERIPHERAL_ID_JTAG             0xC
#define GPIO_PERIPHERAL_ID_I2C_SLAVE        0xD
#define GPIO_PERIPHERAL_ID_QUAD_SPI_MASTER  0xE
#define GPIO_PERIPHERAL_ID_UART_2           0xF
#define GPIO_PERIPHERAL_ID_MAX              0x10

/* GPIO interrupt handler, registered through gpio_intr_handler_register */
typedef void (* gpio_intr_handler_fn_t)(A_UINT32 intr_mask, void *arg);

typedef enum {
    GPIO_PIN_INT_NONE = 0,
    GPIO_PIN_INT_RISING_EDGE,
    GPIO_PIN_INT_FALLING_EDGE,
    GPIO_PIN_INT_BOTH_EDGE,    
    GPIO_PIN_INT_LEVEL_LOW,
    GPIO_PIN_INT_LEVEL_HIGH,  
} GPIO_PIN_INTERRUPT_MODE;

typedef enum {
    GPIO_PIN_PULLNONE = 0,
    GPIO_PIN_PULLUP, 
    GPIO_PIN_PULLDOWN 
} GPIO_PIN_PULL_TYPE;

#define GPIO_PIN_DIR_INPUT  TRUE
#define GPIO_PIN_DIR_OUTPUT FALSE

typedef struct _gpio_interrupt_info_t {
    int         pin;
    void        (*gpio_pin_int_handler_fn)(void *arg);
    void        *arg;
    A_UINT32    internal_flags;
} gpio_interrupt_info_t;

/*
 *  This file contains various structures that are exported outside of target domain
 */
typedef struct gpio_peripheral_pin_info_s
{
    A_UINT32    peripheral_gpio_pins[(GPIO_PIN_COUNT + 31) / 32];
} gpio_peripheral_pin_info_t;

/* Structure to interpret GPIO configurations from 
 * tunable input */
typedef struct gpio_pin_config_s
{
    A_UINT32 source             : 1;
    A_UINT32 pull_override      : 1;
    A_UINT32 pad_driver         : 1;
    A_UINT32 pad_strength       : 2;
    A_UINT32 pad_pull_type      : 2;
    A_UINT32 interrupt_config   : 3;
    A_UINT32 is_wakeup          : 1;
    A_UINT32 functional_config  : 4;
    A_UINT32 reserved           : 14;
    A_UINT32 is_input           : 1;
    A_UINT32 output_config      : 1;
    A_UINT32 valid              : 1;
} gpio_pin_config_t;

/* Per pin, per active configuration structure */
typedef struct gpio_pin_peripheral_config_s
{
    A_UINT32       peripheral_id;
    A_UINT32       gpio_active_config;
} gpio_pin_peripheral_config_t;

/* This structure is the only node for GPIO head which contains all 
 * the possible active alternate configurations for a given pin */
typedef struct gpio_pin_multi_config_s
{
    gpio_pin_peripheral_config_t        *pConfigs;
    struct gpio_pin_multi_config_s      *pNext;
} gpio_pin_multi_config_t;

/* This is the HEAD structure for each pin */
typedef struct gpio_pin_config_head_s
{
    A_UINT32                        supported_peripheral_map;   /* Support for upto 32 peripherals */
    A_UINT32                        active_peripheral_id;
    A_UINT16                        num_dynamic_configs;    /* Number of dynamic configurations added */
    gpio_pin_peripheral_config_t    primary_config;
    gpio_pin_multi_config_t         pLink;
} gpio_pin_config_head_t;

#define A_ROUND_UP_GPIO(x, align)    (((int) (x) + ((align)-1)) & ~((align)-1))

/* This data structure contains information about GPIO store recall 
 * information */
typedef struct gpio_storerecall_data_s
{
    A_UINT32                    active_peripheral_map;
    gpio_peripheral_pin_info_t  interrupt_map;
    gpio_peripheral_pin_info_t  gpio_mode_pin_map;
    gpio_peripheral_pin_info_t  gpio_primary_config_pin_map;
    /* The &3 is for aligning the gpioData 4 byte aligned */
    A_UINT8                     num_dynamic_configs[A_ROUND_UP_GPIO(GPIO_PIN_COUNT, 4)];
    /* [0] to simplify usage of sizeof */
    A_UINT8                     gpioData[0];
} gpio_storerecall_data_t;

struct gpio_api {
    /*
     * Initialize GPIO.  This includes reading the GPIO Configuration DataSet
     * to initialize "output enables" and pin configurations for each gpio pin.
     * Must be called once during startup.
     */
    void (* _gpio_init)(void);

    /*
     * Change GPIO pin output by setting, clearing, or disabling pins.
     * In general, it is expected that a bit will be set in at most one
     * of these masks.  If a bit is clear in all masks, the output state
     * remains unchanged.
     *
     * There is no particular ordering guaranteed; so if the order of
     * writes is significant, calling code should divide a single call
     * into multiple calls.
     */
    void (* _gpio_output_set)(A_GPIO_MASK set_mask,
                              A_GPIO_MASK clear_mask,
                              A_GPIO_MASK enable_mask,
                              A_GPIO_MASK disable_mask);

    /*
     * Sample the value of GPIO input pins and returns a bitmask.
     */
    A_GPIO_MASK (* _gpio_input_get)(void);

    /*
     * Set the specified GPIO register to the specified value.
     * This is a very general and powerful interface that is not
     * expected to be used during normal operation.  It is intended
     * mainly for debug, or for unusual requirements.
     *
     * The reg_id parameter is a GPIO_ID_* value passed from the Host.
     * These values are defined in include/gpio.h and were historically
     * tied to hardware values.
     */
    void (* _gpio_register_set)(A_UINT32 reg_id, A_UINT32 value);

    /* Get the current value of the specified GPIO register. */
    A_UINT32 (* _gpio_register_get)(A_UINT32 reg_id);

    /*
     * Register an application-specific interrupt handler for GPIO pin
     * interrupts.  Once the interrupt handler is called, it will not 
     * be called again until after a call to gpio_intr_ack.  Any GPIO
     * interrupts that occur during the interim are masked.
     *
     * The application-specific handler is called with a mask of
     * pending GPIO interrupts.  After processing pin interrupts, the
     * application-specific handler may wish to use gpio_intr_pending
     * to check for any additional pending interrupts before it returns.
     */
    void (* _gpio_intr_handler_register)(gpio_intr_handler_fn_t fn, void *arg);

    /* Determine which GPIO interrupts are pending. */
    A_GPIO_MASK (* _gpio_intr_pending)(void);

    /*
     * Acknowledge GPIO interrupts.
     * Intended to be called from the gpio_intr_handler_fn.
     */
    void (* _gpio_intr_ack)(A_GPIO_MASK ack_mask);
    
    /* configure a GPIO pin's config value, direction and pullup/down type */
    void (* _gpio_configure_pin)(int pin, A_UINT8 config_val, A_BOOL Input, GPIO_PIN_PULL_TYPE pull_type);
    
    /* configure pullup or pulldown type */
    void (*_gpio_configure_pulltype)(int pin, GPIO_PIN_PULL_TYPE pull_type);
    
    /* configure pin mux setting */
    void (*_gpio_configure_mux)(int pin, A_UINT8 config_val);
    
    /* set output high, note: pin must be configured as an output */
    void (*_gpio_set_pin_high)(int pin);
    
    /* set output low note: pin must be configured as an output */
    void (*_gpio_set_pin_low)(int pin);
    
    /* get input state of pin, TRUE = H, FALSE = L */
    A_BOOL (*_gpio_get_pin_state)(int pin);
    
    /* install shared interrupt dispatcher for all GPIOs */
    void (*_gpio_install_int_dispatcher)(void);
    
    /* register for GPIO pin interrupts */
    A_STATUS (*_gpio_register_pin_interrupt)(gpio_interrupt_info_t *gpio_int_info);
    
    /* de-register previously registered interrupts */
    void (*_gpio_deregister_pin_interrupt)(gpio_interrupt_info_t *gpio_int_info);
    
    /* set the interrupting mode for the GPIO pin, must have a registered handler */
    void (*_gpio_set_interrupt_mode)(gpio_interrupt_info_t *gpio_int_info, GPIO_PIN_INTERRUPT_MODE Mode);
    
    /* called by interrupt handler to (re)arm gpio interrupt */
    void (*_gpio_arm_interrupt)(gpio_interrupt_info_t *gpio_int_info);

    /* configure gpio pad strength*/
    void (*_gpio_configure_padstrength)(A_UINT32 , A_UINT8 );

    /* configure gpio pad driver */
    void (*_gpio_configure_paddriver)(A_UINT32 , A_BOOL);

    /* configure gpio output source  */
    void (*_gpio_configure_source)(A_UINT32 , A_BOOL);

   /* configure gpio wake up */
   void (* _gpio_configure_wakeup)(A_UINT32 , A_BOOL);

#ifdef GPIO_DEFAULT_PULL
    /*gpio pullup or pulldown by default setting*/
    void (*_gpio_default_pad_pull)();
#endif

#if  (CONFIG_GPIO != 1)
    void (*_gpio_all_pins_power_saving_set)(void);
    void (*_gpio_all_pins_restore_set)(void);
    void (*_gpio_pin_dir_set)(int pin, A_BOOL Input);
    void (*_gpio_set_pin_inout)(int pin, A_BOOL Input, A_BOOL val);
    void (*_gpio_all_pins_set)(A_UINT32  gpio_pin_config[], int nItems);
    void (*_gpio_active_pins_set)(gpio_pin_config_head_t gpio_pin_config[], int nItems);
    A_STATUS (*_gpio_get_peripheral_pin_map)(A_UINT32  peripheral_id, gpio_peripheral_pin_info_t *pGpioMap);
    A_STATUS (*_gpio_get_pin_inactive_config)(A_UINT16 pin_num, gpio_pin_config_t *pGpioConfig);
    A_STATUS (*_gpio_get_pin_active_config)(A_UINT16 pin_num, A_UINT32 peripheral_id, gpio_pin_config_t *pGpioConfig);
    void (*_gpio_init_pins)(void);
    A_STATUS (*_gpio_apply_configuration)(gpio_peripheral_pin_info_t *pin_map, A_UINT32 peripheral_id, A_BOOL low_power_mode);
    A_STATUS (*_gpio_add_alternate_configurations)(A_UINT32 pin, A_UINT32 num_configs, 
                                        gpio_pin_peripheral_config_t *pPinConfigs);
    A_UINT32 (*_gpio_get_active_peripheral_id)(A_UINT32 pin);
    A_STATUS (*_gpio_update_pin_active_config)(A_UINT16 pin_num, A_UINT32 peripheral_id,
                                              gpio_pin_config_t *pGpioConfig);
    A_INT32 (*_gpio_is_update_config_allowed)(A_UINT32 pin);
    void (*_gpio_get_active_peripherals)(gpio_peripheral_pin_info_t pin_map, A_UINT32 *pPeripheral_map);
    void (*_gpio_storerecall_init)(void);
    A_UINT32 (*_gpio_storerecall_wakeupmgr_cb)(int calltype, void *priv_data);
    A_STATUS (*_gpio_storerecall_fill_snapshot)(A_UINT8 *pBuffer, A_UINT32 *pLen);
    void (*_gpio_storerecall_recall)(A_UINT8 *pBuffer);
#endif
};

extern void gpio_module_install(struct gpio_api *api);

#define gpio_init()                                                     \
    A_INDIR(gpio._gpio_init())

#define gpio_output_set(set_mask, clear_mask, enable_mask, disable_mask)\
    A_INDIR(gpio._gpio_output_set((set_mask), (clear_mask),               \
                                (enable_mask), (disable_mask)))

#define gpio_input_get()                                                \
    A_INDIR(gpio._gpio_input_get())

#define gpio_register_set(reg_id, value)                                \
    A_INDIR(gpio._gpio_register_set((reg_id), (value)))

#define gpio_register_get(reg_id)                                       \
    A_INDIR(gpio._gpio_register_get(reg_id))

#define gpio_intr_handler_register(fn, arg)                             \
    A_INDIR(gpio._gpio_intr_handler_register((fn), (arg)))

#define gpio_intr_pending()                                             \
    A_INDIR(gpio._gpio_intr_pending(void))

#define gpio_intr_ack(ack_mask)                                         \
    A_INDIR(gpio._gpio_intr_ack(ack_mask))

#define gpio_configure_pin(p,c,d,t)                                     \
    A_INDIR(gpio._gpio_configure_pin((p),(c),(d),(t)))

#define gpio_configure_pulltype(p,t)                                    \
    A_INDIR(gpio._gpio_configure_pulltype((p),(t)))
   
#define gpio_configure_mux(p,v)                                         \
    A_INDIR(gpio._gpio_configure_mux((p),(v)))
     
#define gpio_set_pin_high(p)  A_INDIR(gpio._gpio_set_pin_high((p)))
#define gpio_set_pin_low(p)   A_INDIR(gpio._gpio_set_pin_low((p)))
#define gpio_get_pin_state(p) A_INDIR(gpio._gpio_get_pin_state((p)))       

#define gpio_install_int_dispatcher() \
    A_INDIR(gpio._gpio_install_int_dispatcher())
    
#define gpio_register_pin_interrupt(p)                     \
    A_INDIR(gpio._gpio_register_pin_interrupt((p)))

#define gpio_deregister_pin_interrupt(p)                   \
    A_INDIR(gpio._gpio_deregister_pin_interrupt((p)))
    
#define gpio_set_interrupt_mode(p,m)                       \
    A_INDIR(gpio._gpio_set_interrupt_mode((p),(m)))

#define gpio_arm_interrupt(p)                              \
    A_INDIR(gpio._gpio_arm_interrupt((p)))

#define gpio_configure_padstrength(pin, config_val)        \
    A_INDIR(gpio._gpio_configure_padstrength((pin), (config_val)))

#define gpio_configure_paddriver(pin, open_drain)        \
    A_INDIR(gpio._gpio_configure_paddriver((pin), (open_drain)))

#define gpio_configure_source(pin, pwm)        \
    A_INDIR(gpio._gpio_configure_source((pin), (pwm)))

#define gpio_configure_wakeup(pin, enable)        \
    A_INDIR(gpio._gpio_configure_wakeup((pin), (enable)))

#ifdef GPIO_DEFAULT_PULL
#define gpio_default_pad_pull()	\
    A_INDIR(gpio._gpio_default_pad_pull())
#endif

#if  (CONFIG_GPIO != 1)

#define gpio_pin_dir_set(pin, input)        \
    A_INDIR(gpio._gpio_pin_dir_set((pin), (input)))

#define gpio_set_pin_inout(pin, input, val)    \
    A_INDIR(gpio._gpio_set_pin_inout((pin), (input), (val)))

#define gpio_all_pins_power_saving_set()    \
    A_INDIR(gpio._gpio_all_pins_power_saving_set())

#define gpio_all_pins_restore_set()    \
    A_INDIR(gpio._gpio_all_pins_restore_set())

#define gpio_all_pins_set(pins,n)    \
    A_INDIR(gpio._gpio_all_pins_set((pins), (n)))

#define gpio_active_pins_set(pins,n)    \
    A_INDIR(gpio._gpio_active_pins_set((pins), (n)))

#define gpio_get_peripheral_pin_map(a, b) \
    A_INDIR(gpio._gpio_get_peripheral_pin_map((a), (b)))

#define gpio_get_pin_inactive_config(a, b) \
    A_INDIR(gpio._gpio_get_pin_inactive_config((a), (b)))

#define gpio_get_pin_active_config(a, b, c) \
    A_INDIR(gpio._gpio_get_pin_active_config((a), (b), (c)))

#define gpio_init_pins() \
    A_INDIR(gpio._gpio_init_pins())

#define gpio_apply_configuration(a, b, c) \
    A_INDIR(gpio._gpio_apply_configuration((a), (b), (c)))

#define gpio_add_alternate_configurations(a, b, c) \
    A_INDIR(gpio._gpio_add_alternate_configurations((a), (b), (c)))

#define gpio_get_active_peripheral_id(a) \
    A_INDIR(gpio._gpio_get_active_peripheral_id((a)))

#define gpio_update_pin_active_config(a, b, c) \
    A_INDIR(gpio._gpio_update_pin_active_config((a), (b), (c)))

#define gpio_is_update_config_allowed(a) \
    A_INDIR(gpio._gpio_is_update_config_allowed((a)))

#define gpio_get_active_peripherals(a, b) \
    A_INDIR(gpio._gpio_get_active_peripherals((a), (b)))

#define gpio_storerecall_init() \
    A_INDIR(gpio._gpio_storerecall_init())

#define gpio_storerecall_wakeupmgr_cb(a, b) \
    A_INDIR(gpio._gpio_storerecall_wakeupmgr_cb((a), (b)))

#define gpio_storerecall_fill_snapshot(a, b) \
    A_INDIR(gpio._gpio_storerecall_fill_snapshot((a), (b)))

#define gpio_storerecall_recall(a) \
    A_INDIR(gpio._gpio_storerecall_recall((a)))
#endif

#endif /* __GPIO_API_H__ */
