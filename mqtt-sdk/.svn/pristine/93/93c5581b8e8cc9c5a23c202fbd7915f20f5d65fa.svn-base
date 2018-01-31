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
#ifndef __QCOM_I2C_H__
#define __QCOM_I2C_H__
typedef enum {
	I2C_FREQ_400K 	=	6,
	I2C_FREQ_200K 	=	7,
	I2C_FREQ_100K 	=	8,
	I2C_FREQ_50K 	=	9
}I2C_FREQ;
typedef enum {
	I2C_SCK_1	=	4,
	I2C_SCK_2	=	8,
	I2C_SCK_3	=	12,
	I2C_SCK_4	=	18,
	I2C_SCK_5	=	20//currently used
}I2C_SCK_PIN;
typedef enum {
	I2C_SDA_1	=	3,
	I2C_SDA_2	=	9,
	I2C_SDA_3	=	13,
	I2C_SDA_4	=	19,//currently used
	I2C_SDA_5    =	6
}I2C_SDA_PIN;


#if 0
/* qcom_i2c_init - initialize i2c interface. */
/**
 * This function is used to initialize i2c pins and frequency.
 *	
 * @param sck_pin		sck gpio pin(4, 8, 12, 18, 20).
 * @param sda_pin		sda gpio pin(3, 9, 13, 19).
 * @param freq		i2c lock frequency(50, 100, 200, 400).
 */
A_INT32 qcom_i2c_init(I2C_SCK_PIN sck_pin, I2C_SDA_PIN sda_pin, I2C_FREQ freq);

/* qcom_i2c_read - i2c read devicess. */
/**
 * This function is used to read.
 *	
 * @param devAddr		device address.
 * @param addr			reg address.
 * @param addrLen		the length of the register, 1~4 in unit of bytes.
 * @param data			data buffer pointer.
 * @param length			the data legnth, 1~4 bytes.
 */
A_INT32 qcom_i2c_read(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);

/* qcom_i2c_write - i2c write devicess. */
/**
 * This function is used to write.
 *	
 * @param devAddr		device address.
 * @param addr			reg address.
 * @param addrLen		the length of the register, 1~4 in unit of bytes.
 * @param data			data buffer pointer.
 * @param length			the data legnth, 1~4 bytes.
 */
A_INT32 qcom_i2c_write(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);

/* qcom_i2c_init - i2c read/write control. */
/**
 * This function is used to set i2c read/write control.
 *	
 * @param tx_data		pointer to tx chunk data.
 * @param tx_len		length of tx chunk data, 1~8 byte is preferred 
 * @param rx_data		pointer to rx chunk data.
 * @param rx_len          length of rx chunk data, 0~8 byte is preferred 
 *
 * @return 			1 on success. else on error.
 */
int qcom_i2c_ctrl(unsigned char *tx_data, int tx_len, unsigned char *rx_data, int rx_len);
#endif


/*
 * Constant
 */
#define SI_OK       1
#define SI_ERR      0

#ifdef PHOST_INDIRECT_ACCESS

typedef struct  {
	A_INT32 (*_qcom_i2c_init)(I2C_SCK_PIN sck_pin, I2C_SDA_PIN sda_pin, I2C_FREQ freq);
	A_INT32 (*_qcom_i2c_ctrl)(A_UCHAR *tx_data, A_INT32 tx_len, A_UCHAR *rx_data, A_INT32 rx_len);
	A_INT32 (*_qcom_i2c_read)(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);
	A_INT32 (*_qcom_i2c_write)(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);
}QCOM_I2C_API_INDIRECTION_TABLE;

extern QCOM_I2C_API_INDIRECTION_TABLE qcomI2cApiIndirectionTable;
#define QCOM_I2C_API_FN(fn) qcomI2cApiIndirectionTable.fn

#else

A_INT32 _qcom_i2c_init(I2C_SCK_PIN sck_pin, I2C_SDA_PIN sda_pin, I2C_FREQ freq);
A_INT32 _qcom_i2c_ctrl(A_UCHAR *tx_data, A_INT32 tx_len, A_UCHAR *rx_data, A_INT32 rx_len);
A_INT32 _qcom_i2c_read(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);
A_INT32 _qcom_i2c_write(A_UINT8 devAddr , A_UINT32 addr, A_UINT8 addrLen, A_UCHAR *data, A_UINT32 length);

#define QCOM_I2C_API_FN(fn) fn

#endif


#define qcom_i2c_init(_sck,_sda, _freq) \
    QCOM_I2C_API_FN(_qcom_i2c_init((_sck), (_sda), (_freq)))

#define qcom_i2c_ctrl(_tx, _len1, _rx, _len2) \
    QCOM_I2C_API_FN(_qcom_i2c_ctrl((_tx), (_len1), (_rx), (_len2)))

#define qcom_i2c_read(_devAddr, _addr, _addrLen, _data, _len) \
    QCOM_I2C_API_FN(_qcom_i2c_read((_devAddr), (_addr), (_addrLen), (_data), (_len)))

#define qcom_i2c_write(_devAddr, _addr, _addrLen, _data, _len) \
    QCOM_I2C_API_FN(_qcom_i2c_write((_devAddr), (_addr), (_addrLen), (_data), (_len)))



#endif
