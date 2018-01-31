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
#ifndef __QCOM_I2C_MASTER_H__
#define __QCOM_I2C_MASTER_H__
typedef enum {
	I2C_FREQ_3200K 	=	3,
	I2C_FREQ_1600K 	=	4,
	I2C_FREQ_800K 	=	5,
	I2C_FREQ_400K 	=	6,
	I2C_FREQ_200K 	=	7,
	I2C_FREQ_100K 	=	8,
	I2C_FREQ_50K 	=	9
}I2CM_FREQ;

/*
	I2CM_PAIR {SDA,SCK}
	
	I2CM_PAIR_0 {1,5}//default SPI master interface to flash, not recommend
	I2CM_PAIR_1{11,10}
	I2CM_PAIR_2{15,14}//conflicted with I2CM_PAIR_1
	I2CM_PAIR_3{21,23}
	I2CM_PAIR_4{33,32} //conflicted with I2CM_PAIR_4
 
*/
typedef enum i2cm_pin_pair_t{
	I2CM_PIN_PAIR_0     = 0,
	I2CM_PIN_PAIR_1  ,
	I2CM_PIN_PAIR_2  ,
	I2CM_PIN_PAIR_3  ,
	I2CM_PIN_PAIR_4 ,
	I2CM_PIN_PAIR_MAX
}I2CM_PIN_PAIR;

#ifdef PHOST_INDIRECT_ACCESS

typedef struct 
{
	A_INT32 (*_qcom_i2cm_init)(I2CM_PIN_PAIR pin_pair_id, I2CM_FREQ freq,A_UINT32 timeout);
	A_INT32 (*_qcom_i2cm_fini)(I2CM_PIN_PAIR pin_pair_id);
	A_INT32 (*_qcom_i2cm_read)(I2CM_PIN_PAIR pin_pair_id,A_UINT8 dev_addr , A_UINT32 word_addr, A_UINT8 addr_len, A_UCHAR *data, A_UINT32 data_len);
	A_INT32 (*_qcom_i2cm_write)(I2CM_PIN_PAIR pin_pair_id,A_UINT8 dev_addr, A_UINT32 word_addr, A_UINT8 addr_len, A_UCHAR *data, A_UINT32 data_len);

}QCOM_I2CM_API_INDIRECTION_TABLE;
extern QCOM_I2CM_API_INDIRECTION_TABLE qcomI2cmApiIndirectionTable;
#define QCOM_I2CM_API_FN(fn) qcomI2cmApiIndirectionTable.fn

#else
_qcom_i2cm_init(I2CM_PIN_PAIR pin_pair_id, I2CM_FREQ freq,A_UINT32 timeout);
_qcom_i2cm_fini(I2CM_PIN_PAIR pin_pair_id);
_qcom_i2cm_read(I2CM_PIN_PAIR pin_pair_id,A_UINT8 dev_addr , A_UINT32 word_addr, A_UINT8 addr_len, A_UCHAR *data, A_UINT32 data_len);
_qcom_i2cm_write(I2CM_PIN_PAIR pin_pair_id,A_UINT8 dev_addr, A_UINT32 word_addr, A_UINT8 addr_len, A_UCHAR *data, A_UINT32 data_len);



#define QCOM_I2CM_API_FN(fn) fn

#endif

#define qcom_i2cm_init(pin_pair_id, freq,timeout) \
	QCOM_I2CM_API_FN(_qcom_i2cm_init((pin_pair_id), (freq),(timeout)))
#define qcom_i2cm_fini(pin_pair_id) \
	QCOM_I2CM_API_FN(_qcom_i2cm_fini((pin_pair_id)))
#define qcom_i2cm_read(pin_pair_id,dev_addr,word_addr,addr_len,data,data_len) \
	QCOM_I2CM_API_FN(_qcom_i2cm_read((pin_pair_id), (dev_addr), (word_addr),(addr_len),(data),(data_len)))
#define qcom_i2cm_write(pin_pair_id,dev_addr,word_addr,addr_len,data,data_len) \
	QCOM_I2CM_API_FN(_qcom_i2cm_write((pin_pair_id), (dev_addr), (word_addr),(addr_len),(data),(data_len)))

#endif
