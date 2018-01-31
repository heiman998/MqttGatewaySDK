/*
    qca_ssl_client test case

    Add the code below to you project to test

>    extern void test_qca_spi(void);
>    test_qca_spi();
 */
#include "qcom_common.h"
#include "qca_spi_driver.h"
#include "qca_gpio.h"
#include "qcom_spi.h"

void test_qca_spi()
{
    unsigned int i;
    unsigned char buf[64];

    qca_spi_set(SPI_0, SPI_MODE2, SPI_6500K);

    for(i = 0; i < 4; i++){

        qca_spi_cs(SPI_ENABLE);
        memset(buf, 'a', 64);
        qca_spi_write(buf, 64);

        qca_spi_cs(SPI_DISABLE);
    }
}
