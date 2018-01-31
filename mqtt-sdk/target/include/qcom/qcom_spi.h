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
#ifndef __CUST_SPI_H__
#define __CUST_SPI_H__
typedef enum {
	SPI_FREQ_6500K	=	2,
	SPI_FREQ_3250K	=	3,
	SPI_FREQ_1600K =	4,
	SPI_FREQ_800K 	=	5,
	SPI_FREQ_400K 	=	6,
	SPI_FREQ_200K 	=	7,
	SPI_FREQ_100K 	=	8,
	SPI_FREQ_50K 	=	9
}SPI_FREQ;
typedef enum {
	SPI_CS_1	=	0,
	SPI_CS_2	=	8
}SPI_CS_PIN;
typedef enum {
	SPI_CS_1_CFG	=	0x01,
	SPI_CS_2_CFG	=	0x05
}SPI_CS_PIN_CFGVALUE;

typedef enum {
	SPI_MOSI_1	=	1,
	SPI_MOSI_2	=	7,
}SPI_MOSI_PIN;
typedef enum {
	SPI_MOSI_1_CFG	=	0x01,
	SPI_MOSI_2_CFG	=	0x05,
}SPI_MOSI_PIN_CFGVALUE;

typedef enum {
	SPI_MISO_1	=	4,
	SPI_MISO_2	=	6,
}SPI_MISO_PIN;
typedef enum {
	SPI_MISO_1_CFG	=	0x01,
	SPI_MISO_2_CFG	=	0x05,
}SPI_MISO_PIN_CFGVALUE;

typedef enum {
	SPI_SCLK_1	=	5,
	SPI_SCLK_2	=	9,
}SPI_SCLK_PIN;
typedef enum {
	SPI_SCLK_1_CFG	=	0x01,
	SPI_SCLK_2_CFG	=	0x05,
}SPI_SCLK_PIN_CFGVALUE;


/* qcom_spi_init - initialize spi interface. */
/**
 * This function is used to initialize spi pins and frequency.
 *	
 * @param cs_pin		cs gpio pin(SPI_CS_PIN).
 * @param mosi_pin		mosi gpio pin(SPI_MOSI_PIN).
 * @param miso_pin		miso gpio pin(SPI_MISO_PIN).
 * @param sclk_pin		sclk gpio pin(SPI_SCLK_PIN).
 * @param freq		SPI lock frequency(SPI_FREQ).
 *
 * @return 			0 on success. else on error.
 */
int qcom_spi_init(SPI_CS_PIN cs_pin, SPI_MOSI_PIN mosi_pin, SPI_MISO_PIN miso_pin, SPI_SCLK_PIN sclk_pin, SPI_FREQ freq);
/* qcom_spi_fini - call it after finish write/read. */
/**
 * This function is used to restore configuration after used SPI device.
 *	
 */
void qcom_spi_fini(void);
/* qcom_spi_request - write device via spi interface. */
/**	
 * This function is used to write spi device.
 *	
 * @param req		pointer to chunk data.
 * @param rxcnt		length of read data.
 * @param rxcnt		length of write data.
 */
void qcom_spi_request(unsigned int *req, unsigned int rxcnt, unsigned int txcnt);
/* qcom_spi_response - write device via spi interface. */
/**
 * This function is used to write spi device.
 *	
 * @param buf		pointer to chunk data.
 * @param nbytes		length of read data.
 */
void qcom_spi_response(unsigned char *buf, unsigned int nbytes);

#endif
