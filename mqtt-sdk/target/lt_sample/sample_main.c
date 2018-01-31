#include "sample_main.h"
#include "qca_task_manage.h"
#include "qca_gpio.h"
#include "qca_softap.h"
#include "qca_ap_connect.h"
#include "qca_adc.h"
#include "qca_i2s.h"
#include "qca_aes.h"
#include "qca_flash_param.h"
#include "qca_i2c_driver.h"
#include "qca_infrared.h"
#include "qca_json.h"
#include "qca_lib_version.h"
#include "qca_ltlink.h"
#include "qca_psram_param.h"
#include "qca_sniffer.h"
#include "qca_spi_driver.h"
#include "qca_ssl_client.h"
#include "qca_uart.h"
#include "qca_http_client.h"
#include "qca_ota.h"

/* Test function on/off */
#define TEST_LIB_VERSION 1
/* TEST_STATION_MODE and TEST_SOFTAP_MODE is alternative */

#define TEST_STATION_MODE 0

#define TEST_SOFTAP_MODE 0

#define TEST_TASK_MANAGE 0
#define TEST_SSL_CLIENT 0
#define TEST_INFRARED 0
#define TEST_I2C_DRIVER 0
#define TEST_SPI 0
#define TEST_AES 0
#define TEST_MD5 0
#define TEST_BASE64 0
#define TEST_ADC 0
#define TEST_I2S 0
#define TEST_UART 0
#define TEST_PSRAM_PARAM 0
#define TEST_FLASH_PARAM 1
#define TEST_GPIO 0
#define TEST_LTLINK 0
#define TEST_SNIFFER 0
#define TEST_JSON 0
#define TEST_OTA 0
#define TEST_PWM 0
#define TEST_TIMER 0
#define TEST_IRQ 0
#define TEST_TCP_CLIENT 0
#define TEST_TCP_SERVER 0
#define TEST_UDP 0
#define TEST_HTTP_CLIENT 0
#define TEST_CALENDAR 0

int sample_main(void)
{
#if TEST_LIB_VERSION
	extern void test_lib_version(void);
	test_lib_version();
#endif

#if TEST_STATION_MODE
	extern void test_ap_connect(void);
	test_ap_connect();
#endif

#if TEST_SOFTAP_MODE
    extern void test_softap(void);
    test_softap();
#endif

#if TEST_TASK_MANAGE
	extern void test_task_manage(void);
	test_task_manage();
#endif

#if TEST_SSL_CLIENT
	extern void test_ssl_client(void);
    test_ssl_client();
#endif

#if TEST_INFRARED
    extern void test_infrared_task(unsigned int arg);
    test_infrared_task(0);
#endif

#if TEST_I2C_DRIVER
    extern void test_i2c_driver(void);
    test_i2c_driver();
#endif

#if TEST_SPI
    extern void test_qca_spi(void);
    test_qca_spi();
#endif

#if TEST_AES
    extern void test_aes(void);
    test_aes();
#endif

#if TEST_MD5
    extern void test_md5(void);
    test_md5();
#endif

#if TEST_BASE64
    extern void test_base64(void);
    test_base64();
#endif

#if TEST_ADC
    extern void test_adc(void);
    test_adc();
#endif

#if TEST_I2S
    extern void test_i2s(void);
    test_i2s();
#endif

#if TEST_UART
    extern void test_uart(void);
    test_uart();
#endif

#if TEST_PSRAM_PARAM
    extern void test_psram_param(void);
    test_psram_param();
#endif

#if TEST_FLASH_PARAM
    extern void test_flash_param(void);
    test_flash_param();
#endif

#if TEST_GPIO
    extern void test_gpio(void);
    test_gpio();
#endif

#if TEST_LTLINK
    extern void test_ltlink(void);
    test_ltlink();
#endif

#if TEST_SNIFFER
    extern void test_sniffer(void);
    test_sniffer();
#endif

#if TEST_JSON
    extern void test_json(void);
    test_json();
#endif

#if TEST_OTA 
    extern void test_ota(void);
    test_ota();
#endif

#if TEST_PWM
    extern void test_pwm(void);
    test_pwm();
#endif

#if TEST_TIMER
    void test_timer();
    test_timer();
#endif

#if TEST_IRQ
    void test_irq();
    test_irq();
#endif

#if TEST_HTTP_CLIENT
    extern void qca_http_client_test();
    qca_http_client_test();
#endif

#if TEST_TCP_CLIENT
    extern void qca_tcp_client_test(void);
    qca_tcp_client_test();
#endif

#if TEST_TCP_SERVER
    extern void qca_tcp_server_test(void);
    qca_tcp_server_test();
#endif

#if TEST_UDP
    void qca_udp_server_test(void);
    qca_udp_server_test();
#endif

#if TEST_CALENDAR
    extern void test_calendar();
    test_calendar();
#endif

    while(1){
        qcom_thread_msleep(1000);
    }

	return 0;
}
