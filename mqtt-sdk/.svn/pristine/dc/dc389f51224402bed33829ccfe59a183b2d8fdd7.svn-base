/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 *
 * Serial Interface support.  AR6K provides a SI which uses GPIO pins
 * in order to operate as either a SPI Master or I2C Master.  The
 * serial interface may be used to attach a Serial Flash or an EEPROM.
 */

#if defined(AR6002_REV4) || defined(AR6002_REV6) || defined(AR6002_REV7)/* { */

#include "osapi.h"
#include "hw/gpio_reg.h"
#include "hw/rtc_reg.h"
#include "hw/si_reg.h"

#include "efuse_api.h"
#include "otpstream_api.h"
#include "si_api.h"
#include "qcom_spi.h"

A_UINT32 si_pin_save[NVRAM_PINCOUNT_MAX];
A_UINT32 clock_control_reg_save;
A_UINT32 gpio_enable_reg_save;
A_UINT32 si_config_reg_save;
A_UINT32 sdio_reg_save;
#define SPI_PINCOUNT_MAX 4
struct spi_config_s {
    /* Serial Interface-connected SPI configuration. */
    A_UINT32    si_config_value;

    /* Selects which GPIO pin numbers are to be used */
    A_UINT8     si_pin_num[SPI_PINCOUNT_MAX];

    /* GPIO CONFIG value plus OUT indication */
    A_UINT8     si_pin_cfg[SPI_PINCOUNT_MAX];

    A_UINT8     si_pin_count; /* 2 for I2C, 4 for SPI */

    A_UINT8     si_sdio_value; /* 0-->using SDIO pins for SI */
}; 

#if defined (AR6002_REV4)

#define GET_REGISTER(_pin, _polarity) ((_polarity)? WLAN_GPIO_ENABLE_W1TS_ADDRESS : WLAN_GPIO_ENABLE_W1TC_ADDRESS)    
#define GET_MASK(_pin) (1<<(_pin))

#elif defined (AR6002_REV6)

#define GET_REGISTER(_pin, _polarity) \
    ((_polarity)? \
        (((_pin)>=32)? \
            WLAN_GPIO_ENABLE_W1TS_HIGH_ADDRESS \
        : \
            WLAN_GPIO_ENABLE_W1TS_LOW_ADDRESS) \
    : \
        (((_pin)>=32)? \
            WLAN_GPIO_ENABLE_W1TC_HIGH_ADDRESS \
        : \
            WLAN_GPIO_ENABLE_W1TC_LOW_ADDRESS))

#define GET_MASK(_pin) (((_pin)>=32)? (1<<((_pin)-32)) : (1<<(_pin)))

#elif defined (AR6002_REV7)

#define GET_REGISTER(_pin, _polarity) ((_polarity)? WLAN_GPIO_ENABLE_W1TS_LOW_ADDRESS : WLAN_GPIO_ENABLE_W1TC_LOW_ADDRESS)    
#define GET_MASK(_pin) (1<<(_pin))

#else
#error undefined GET_REGISTER() and GET_MASK() macros
#endif
#define SPI_PIN_OUT      0x80
#define SPI_PIN_CFG_MASK 0x7f
struct spi_config_s spi_config_p;
static struct spi_config_s spi_config_legacy =
{
    (SI_CONFIG_I2C_SET(0) | SI_CONFIG_POS_SAMPLE_MASK | SI_CONFIG_INACTIVE_DATA_MASK | SI_CONFIG_INACTIVE_CLK_MASK | SI_CONFIG_DIVIDER_SET(2)),
    {SPI_SCLK_1, SPI_CS_1, SPI_MOSI_1, SPI_MISO_1},
    {SPI_SCLK_1_CFG|SPI_PIN_OUT, SPI_CS_1_CFG|SPI_PIN_OUT, SPI_MOSI_1_CFG|SPI_PIN_OUT, SPI_MISO_1_CFG}, 
    4, //pin count
    1, //pins are not shared with sdio
};
int qcom_spi_init(SPI_CS_PIN cs_pin, SPI_MOSI_PIN mosi_pin, SPI_MISO_PIN miso_pin, SPI_SCLK_PIN sclk_pin, SPI_FREQ freq)
{
	if((freq<0)||(freq>9))
		return -1;
	int i;
	A_UINT32 reg, mask;
	A_UINT8 pin_config;

	A_MEMCPY(&spi_config_p, &spi_config_legacy, sizeof(struct spi_config_s));

	switch(cs_pin){
	default :
		return -1;
		break;
	case SPI_CS_2:
		pin_config = SPI_CS_2_CFG;		
		break;
	case SPI_CS_1:
		pin_config = SPI_CS_1_CFG;		
		break;
	}
	
	spi_config_p.si_pin_num[1]  = (A_UINT8)cs_pin;
	spi_config_p.si_pin_cfg[1] = pin_config|SPI_PIN_OUT;
	
	switch(mosi_pin){
	default :
		return -1;
		break;
	case SPI_MOSI_2:
		pin_config = SPI_MOSI_2_CFG;		
		break;
	case SPI_MOSI_1:
		pin_config = SPI_MOSI_1_CFG;		
		break;
	}

	spi_config_p.si_pin_num[2]  = (A_UINT8)mosi_pin;
	spi_config_p.si_pin_cfg[2] = pin_config|SPI_PIN_OUT;

	switch(miso_pin){
	default :
		return -1;
		break;
	case SPI_MISO_2:
		pin_config = SPI_MISO_2_CFG;		
		break;
	case SPI_MISO_1:
		pin_config = SPI_MISO_1_CFG;		
		break;
	}

	spi_config_p.si_pin_num[3]  = (A_UINT8)miso_pin;
	spi_config_p.si_pin_cfg[3] = pin_config;

	switch(sclk_pin){
	default :
		return -1;
		break;
	case SPI_SCLK_2:
		pin_config = SPI_SCLK_2_CFG;		
		break;
	case SPI_SCLK_1:
		pin_config = SPI_SCLK_1_CFG;		
		break;
	}

	spi_config_p.si_pin_num[0]  = (A_UINT8)sclk_pin;
	spi_config_p.si_pin_cfg[0] = pin_config|SPI_PIN_OUT;

    /* Enable SI clock */
	clock_control_reg_save = reg = A_RTC_SOC_REG_READ(CLOCK_CONTROL_ADDRESS);
	reg &= ~CLOCK_CONTROL_SI0_CLK_MASK;
	 A_RTC_SOC_REG_WRITE(CLOCK_CONTROL_ADDRESS, reg);

    /* Pull SI out of reset */
	reg = A_RTC_SOC_REG_READ(SOC_RESET_CONTROL_ADDRESS);
	reg &= ~RESET_CONTROL_SI0_RST_SET(1);
	A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS, reg);

    /* Handle any overlap between pins used for SI and for SDIO. */
	sdio_reg_save = A_GPIO_REG_READ(SDIO_ADDRESS);
	if(sclk_pin == SPI_SCLK_2)
	{
	    spi_config_p.si_sdio_value = 0;
	}

        if(!A_IS_HOST_PRESENT())
	    A_GPIO_REG_WRITE(SDIO_ADDRESS, 0x0);
	else
	    A_GPIO_REG_WRITE(SDIO_ADDRESS, (A_UINT32)spi_config_p.si_sdio_value);
	for (i=0; i<spi_config_p.si_pin_count; i++) {
	       si_pin_save[i] = A_GPIO_REG_READ(WLAN_GPIO_PIN0_ADDRESS + spi_config_p.si_pin_num[i]*4);

	       A_GPIO_REG_WRITE(WLAN_GPIO_PIN0_ADDRESS + spi_config_p.si_pin_num[i]*4,
	       /*(si_pin_save[i] & ~WLAN_GPIO_PIN0_CONFIG_MASK)*/ GPIO_PIN0_PAD_STRENGTH_SET(1)
	        | WLAN_GPIO_PIN0_CONFIG_SET(spi_config_p.si_pin_cfg[i] & SPI_PIN_CFG_MASK));
	}
    /* Configure each pin for input or output */
	gpio_enable_reg_save = A_GPIO_REG_READ(WLAN_GPIO_ENABLE_ADDRESS);
	for (i=0; i<spi_config_p.si_pin_count; i++) {
	        reg = GET_REGISTER(spi_config_p.si_pin_num[i], (spi_config_p.si_pin_cfg[i] & SPI_PIN_OUT));
	        mask = GET_MASK(spi_config_p.si_pin_num[i]);
	        A_GPIO_REG_WRITE(reg, mask);        
	}
	si_config_reg_save = A_SI_REG_READ(SI_CONFIG_ADDRESS);
	spi_config_p.si_config_value = (SI_CONFIG_I2C_SET(0) | SI_CONFIG_POS_SAMPLE_MASK | SI_CONFIG_INACTIVE_DATA_MASK | SI_CONFIG_INACTIVE_CLK_MASK | SI_CONFIG_DIVIDER_SET(freq));
	A_SI_REG_WRITE(SI_CONFIG_ADDRESS, spi_config_p.si_config_value);

	return 0;
}

/* Restore registers that were changed by si_init */
void
qcom_spi_fini(void)
{
    int i;
    A_RTC_SOC_REG_WRITE(CLOCK_CONTROL_ADDRESS, clock_control_reg_save);
    A_GPIO_REG_WRITE(SDIO_ADDRESS, sdio_reg_save);
    for (i=0; i<spi_config_p.si_pin_count; i++) {
        A_GPIO_REG_WRITE(WLAN_GPIO_PIN0_ADDRESS + spi_config_p.si_pin_num[i]*4, si_pin_save[i]);
    }
    A_GPIO_REG_WRITE(WLAN_GPIO_ENABLE_ADDRESS, gpio_enable_reg_save);
    A_SI_REG_WRITE(SI_CONFIG_ADDRESS, si_config_reg_save);
}

/*
 * Wait for an operation to complete.
 * Returns 0 on success; non-0 on error
 */
A_UINT32
_qcom_spi_wait_done(void)
{
    A_UINT32 reg;

    /* Poll for CS_DONE_INT bit */
    do {
        reg = A_SI_REG_READ(SI_CS_ADDRESS);
    } while (!(reg & (SI_CS_DONE_INT_MASK | SI_CS_DONE_ERR_MASK)));
    if (reg & SI_CS_DONE_ERR_MASK) {
        return reg;
    }
    /* Clear CS_DONE_INT (W1TC) bit */
    A_SI_REG_WRITE(SI_CS_ADDRESS, reg);
    return 0;
}

void
qcom_spi_request(A_UINT32 *req, unsigned int rxcnt, unsigned int txcnt)
{
    if (txcnt) {
        A_SI_REG_WRITE(SI_TX_DATA0_ADDRESS, req[0]);
    }
    if (txcnt > 4) {
        A_SI_REG_WRITE(SI_TX_DATA1_ADDRESS, req[1]);
    }
    do {
        A_SI_REG_WRITE(SI_CS_ADDRESS, SI_CS_START_MASK
                                  | SI_CS_RX_CNT_SET(rxcnt)
                                  | SI_CS_TX_CNT_SET(txcnt));
    } while (_qcom_spi_wait_done());
    return;
}

/* 
 * Read up to 8 bytes from the attached device
 * into a byte-aligned buffer.
 */
void
qcom_spi_response(A_UINT8 *buf, unsigned int nbytes)
{
	int i;
	int which_word;
	A_UINT32 data;

	for (which_word=0; which_word < 2; which_word++) {
       data = A_SI_REG_READ(SI_RX_DATA0_ADDRESS+4*which_word);
       for (i=0; i<4; i++, buf++) {
		if (nbytes--) {
			*buf = ((A_UINT8 *)&data)[i];
		} else {
                return;
            }
        }
    }
}

#endif /* } AR6002_REV4 || defined(AR6002_REV6) */
#ifdef SPI_MASTER_TEST
void test_spi_flash_read(int cs_pin,unsigned char *addr, unsigned char *buf, int size)
{
	A_UINT32 req[1];
	req[0] = 0x03|(addr[0]<<8)|(addr[1]<<16)|(addr[2]<<24);
	if(qcom_spi_init(cs_pin, SPI_MOSI_1, SPI_MISO_1, SPI_SCLK_1, SPI_FREQ_400K)<0)
		return;
	qcom_spi_request(req,(unsigned int )size,4);
	qcom_spi_response(buf,(unsigned int )size);
	qcom_spi_fini();
}
#endif

