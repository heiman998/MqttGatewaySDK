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
#ifndef __QCOM_I2S_API_H
#define __QCOM_I2S_API_H

typedef enum {
	I2S_FREQ_44_1K	 = 0,
	I2S_FREQ_48K	= 1,
	I2S_FREQ_32K 	= 2,
	I2S_FREQ_96K	= 3,
	I2S_FREQ_16K	= 4,
	I2S_FREQ_MAX	= 5,
}I2S_FREQ;
typedef enum {
	I2S_MASTER = 1,
	I2S_SLAVE = 0
}I2S_SIM;
typedef enum {
    I2S_CLK_CPU = 0,
    I2S_CLK_BB = 1
} I2S_CLK;

typedef enum {
	I2S_RCV_START = 0,
	I2S_RCV_STOP = 1, 	
}I2S_RCV_CONTROL;

typedef struct{
    A_UINT8 port;
    I2S_FREQ freq;
    A_UINT8 dsize;
    A_UINT16 i2s_buf_size;
    A_UINT8 num_rx_desc;
    A_UINT8 num_tx_desc;
    A_UINT8 i2s_mode; //master:1   slave:0
}i2s_api_config_s;

typedef void (*i2s_rec_cb)(unsigned char * ,unsigned int );

#if 0
/* qcom_i2s_init - initialize i2s interface. */
/**
 * This function is used to initialize i2s frequency etc.
 * audio bit wide is 32 bits, sampling frequency is WS, KF can be used in master or slave mode.
 *	
 * @param freq		WS frequency.
 * @param src_clk		source clock.
 * @param mode		KF is used in master or slave mode.
 *
 * @return 			1 on success. else on error.
 */
A_INT32  qcom_i2s_init(A_UINT8 port, I2S_FREQ freq,A_UINT8 data_size,
						A_UINT8 tx_buf_cnt, A_UINT8 rx_buf_cnt, A_UINT16 buf_size, 
						void *cb_rx);

/* qca_i2s_write - write data through i2s. */
/**
 * This function is used to write audio data.
 *	
 * @param buf		pointer to chunk data.
 * @param count		length of chunk data.
 *
 * @return 			1 on success. else on error.
 */
A_UINT32 qcom_i2s_rcv_data(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen, A_UINT32 *len);
A_UINT32 qcom_i2s_xmt_data(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen);

void qcom_i2s_rcv_control(A_UINT8 port, I2S_RCV_CONTROL control);
#endif

#ifdef PHOST_INDIRECT_ACCESS

typedef struct  {
	A_INT32  (*_qcom_i2s_init)(i2s_api_config_s *i2s_api_config, void *intr_cb_txcomp, void *intr_cb_rx);
	A_INT32  (* _qcom_i2s_config)(A_UINT8 port, I2S_FREQ freq , A_UINT8 data_size);
	A_UINT32 (*_qcom_i2s_xmt_data)(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen);
	A_UINT32 (*_qcom_i2s_rcv_data)(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen, A_UINT32 *len);
	void (*_qcom_i2s_rcv_control)(A_UINT8 port, I2S_RCV_CONTROL control);
	//A_UINT32 (*_qcom_i2s_mode)(A_UINT8 port, A_UINT32 mode);

}QCOM_I2S_API_INDIRECTION_TABLE;

extern QCOM_I2S_API_INDIRECTION_TABLE qcomI2sApiIndirectionTable;
#define QCOM_I2S_API_FN(fn) qcomI2sApiIndirectionTable.fn

#else

	A_INT32  _qcom_i2s_init(i2s_api_config_s *i2s_api_config, void *intr_cb_txcomp, void *intr_cb_rx);
	A_INT32  _qcom_i2s_config(A_UINT8 port, I2S_FREQ freq , A_UINT8 data_size);
	A_UINT32 _qcom_i2s_xmt_data(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen);
	A_UINT32 _qcom_i2s_rcv_data(A_UINT8 port, A_UINT8 * data, A_UINT32 bufLen, A_UINT32 *len);
	void _qcom_i2s_rcv_control(A_UINT8 port, I2S_RCV_CONTROL control);
	A_UINT32 _qcom_i2s_mode(A_UINT8 port, A_UINT32 mode);

#define QCOM_I2S_API_FN(fn) fn

#endif

#define qcom_i2s_init(_config, _p, _q) \
    QCOM_I2S_API_FN(_qcom_i2s_init((_config), (_p), (_q)))

#define qcom_i2s_config(_port, _freq,  _size) \
    QCOM_I2S_API_FN(_qcom_i2s_config((_port), (_freq), (_size)))

#define qcom_i2s_xmt_data(_port, _data,  _len) \
    QCOM_I2S_API_FN(_qcom_i2s_xmt_data((_port), (_data), (_len)))

#define qcom_i2s_rcv_data(_port, _data,  _len, _retLen) \
    QCOM_I2S_API_FN(_qcom_i2s_rcv_data((_port), (_data), (_len), (_retLen)))

#define qcom_i2s_rcv_control(_port, _ctrl) \
    QCOM_I2S_API_FN(_qcom_i2s_rcv_control((_port), (_ctrl)))
/*TO DO, fix in next ROM*/ 
A_UINT32 _qcom_i2s_mode(A_UINT8 port, A_UINT32 mode);
#define qcom_i2s_mode(_port, _mode) \
		_qcom_i2s_mode((_port), (_mode))


#endif

