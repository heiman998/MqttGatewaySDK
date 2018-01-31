/*
    qca_i2c_driver test case

    Add the code below to you project to test

>    extern void test_i2c_driver(void);
>    test_i2c_driver();
 */
#include "sample_main.h"
#include "qca_i2c_driver.h"
#include "qca_gpio.h"
#include "qcom_i2c_master.h"
#include "qcom_nvram.h"

#define I2C_FREQ I2CM_500K

/* Link eeprom(M24C32-RMB6TP) to GPIO14(SDA) and GPIO15(SCL) */
void test_i2c_driver(void)
{
    static unsigned char buf1[8] = {0,};
    static unsigned char buf2[8] = {0,};
    int i;

    //unsigned char buf2 = 0;
    int ret = 0;
    ret = qca_i2cm_init(I2C_FREQ, 0xff);
    if(ret != 0){
        printf("I2C master init failed\n");
        return;
    }

    buf1[0] = 0xFE;
    buf1[1] = 0x80;
    buf1[2] = 0xC0;
    buf1[3] = 0xE0;
    buf1[4] = 0x78;
    buf1[5] = 0x11;
    buf1[6] = 0x12;
    buf1[7] = 0x13;

    ret = qca_i2cm_write(0xa0, 0xfc, 0, buf1, 4);

    qca_i2cm_read(0xa0, 0xfc, 0, buf1, 4);

    qca_i2cm_fini();

    if(buf1 == buf2)
        printf("I2C master OK\n");
    else
        printf("I2C master error\n");
}
