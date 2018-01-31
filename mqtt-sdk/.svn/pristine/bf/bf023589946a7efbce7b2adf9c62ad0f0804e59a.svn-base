/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_ADC_API_H__
#define __QCOM_ADC_API_H__

#include <qcom/stdint.h>
#include <../adc_layer_wmi.h>
#include "cdr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int initResult;
    int calibResult;
    int convDone;
    int convAbort;
    int convErr;
    int evtReg;
    int channelCnt;

    //A_INT32 channel;
    //A_UINT8* data;
}adc_context_t;

typedef struct {
    void *event;
    int completed;
    void (*set_complete_flag)(struct cdr_req_s *cdr_req);
} cdr_adc_completed_t;




#ifdef PHOST_INDIRECT_ACCESS

typedef struct 
{
    void        (* _wmi_adc_cmd)(void *wmip, A_UINT32 cmd_type, void* pData, A_UINT32 length);
    void        (* _cdr_adc_common_completed)(void *arg, struct cdr_req_s *cdr_req);
    void        (* _BGR_ADC_CDR_initiate)(void *request, unsigned int req_length);
    void        (*_qcom_adc_event_handler)(void* pEvtBuffer);
    A_STATUS    (*_qcom_adc_event_register)(void);
    void        (*_qcom_data_dump_hex)(A_UINT8 *data, A_UINT32 len);    	
    int         (*_qcom_adc_init)(A_UINT8 gain_scale,A_UINT8 accuracy, A_UINT8 data_rate, A_UINT8 input_type,A_BOOL stream_bit);
    int         (* _qcom_adc_config)(A_UINT8 scan_mode, A_UINT8 trigger_mode, A_UINT8 adco_mode,ADC_CHAN_CFG_T adc_channel[],A_UINT8 channel_cnt);
    int         (*_qcom_adc_timer_config)(A_UINT32 time_start, A_UINT8 time_unit);

    int         (* _qcom_adc_dma_config)(A_UINT32 threshold,A_UINT32 data_cnt);
    int         (* _qcom_adc_calibration)(A_UINT8 accuracy, A_UINT8 input_type);
    int         (* _qcom_adc_conversion)(A_BOOL enable);
    int         (* _qcom_adc_read_data)(A_UINT8 channel, A_UINT8* data, A_UINT32 bufLen, A_UINT32* len);
    int         (*_qcom_adc_recv_data)(A_UINT8 channel, A_UINT8* buffer, A_UINT32 bufLen, A_UINT32* ret_len,A_UINT8* more, A_UINT8* done);
    int         (*_qcom_adc_intq_process)(A_INT32 intq);
    int         (*_qcom_adc_close)();

}QCOM_ADC_API_INDIRECT_TABLE;

extern QCOM_ADC_API_INDIRECT_TABLE qcom_adc_api_indirect_table;
#define QCOM_ADC_API_FN(fn) qcom_adc_api_indirect_table.fn

#else

void        _wmi_adc_cmd(void *wmip, A_UINT32 cmd_type, void* pData, A_UINT32 length);
void        _cdr_adc_common_completed(void *arg, struct cdr_req_s *cdr_req);
void        _BGR_ADC_CDR_initiate(void *request, unsigned int req_length);
void        _qcom_adc_event_handler(void* pEvtBuffer);
A_STATUS    _qcom_adc_event_register(void);
void        _qcom_data_dump_hex(A_UINT8 *data, A_UINT32 len);


int         _qcom_adc_init(A_UINT8 gain_scale,A_UINT8 accuracy, A_UINT8 data_rate, A_UINT8 input_type,A_BOOL stream_bit);
int         _qcom_adc_config(A_UINT8 scan_mode, A_UINT8 trigger_mode, A_UINT8 adco_mode, ADC_CHAN_CFG_T adc_channel[],A_UINT8 channel_cnt);
int         _qcom_adc_timer_config(A_UINT32 time_start, A_UINT8 time_unit);
int         _qcom_adc_dma_config(A_UINT32 threshold,A_UINT32 data_cnt);
int         _qcom_adc_calibration(A_UINT8 accuracy, A_UINT8 input_type);
int         _qcom_adc_conversion(A_BOOL enable);
int         _qcom_adc_read_data(A_UINT8 channel, A_UINT8* data, A_UINT32 bufLen, A_UINT32* len);
int         _qcom_adc_recv_data(A_UINT8 channel, A_UINT8* buffer, A_UINT32 bufLen, A_UINT32* ret_len,A_UINT8* more, A_UINT8* done);
int         _qcom_adc_intq_process(A_INT32 intq);
int         _qcom_adc_close();


#define QCOM_ADC_API_FN(fn) fn

#endif


#define wmi_adc_cmd(_wmip, _type, _data, _len) \
    QCOM_ADC_API_FN(_wmi_adc_cmd((_wmip), (_type), (_data), (_len)))

#define cdr_adc_common_completed(_arg, _cdr) \
    QCOM_ADC_API_FN(_cdr_adc_common_completed((_arg), (_cdr)))

#define BGR_ADC_CDR_initiate(_req, _len) \
    QCOM_ADC_API_FN(_BGR_ADC_CDR_initiate((_req), (_len)))

#define qcom_adc_event_handler(_ev1) \
    QCOM_ADC_API_FN(_qcom_adc_event_handler(_ev1))

#define qcom_adc_event_register() \
    QCOM_ADC_API_FN(_qcom_adc_event_register())

#define qcom_data_dump_hex(data, len) \
    QCOM_ADC_API_FN(_qcom_data_dump_hex((data),(len)))


#define qcom_adc_init(gain_scale, accuracy, data_rate, input_type, stream_bit) \
    QCOM_ADC_API_FN(_qcom_adc_init((gain_scale),(accuracy),(data_rate),(input_type),(stream_bit)))

#define qcom_adc_config(scan_mode, trigger_mode, adco_mode,adc_channel, channel_cnt) \
    QCOM_ADC_API_FN(_qcom_adc_config((scan_mode), (trigger_mode),(adco_mode),(adc_channel),(channel_cnt)))

#define qcom_adc_timer_config(time_start, time_unit) \
    QCOM_ADC_API_FN(_qcom_adc_timer_config((time_start), (time_unit)))

#define qcom_adc_dma_config(threshold,data_cnt) \
    QCOM_ADC_API_FN(_qcom_adc_dma_config((threshold),(data_cnt)))

#define qcom_adc_calibration(accuracy, input_type) \
    QCOM_ADC_API_FN(_qcom_adc_calibration((accuracy),(input_type)))

#define qcom_adc_conversion(enable) \
    QCOM_ADC_API_FN(_qcom_adc_conversion((enable)))

#define qcom_adc_read_data(channel, data, bufLen, ret_len) \
    QCOM_ADC_API_FN(_qcom_adc_read_data((channel),(data),(bufLen),(ret_len)))

#define qcom_adc_recv_data(channel, buffer, bufLen, ret_len,more,done) \
    QCOM_ADC_API_FN(_qcom_adc_recv_data((channel),(buffer),(bufLen),(ret_len),(more),(done)))

#define qcom_adc_intq_process(intq) \
    QCOM_ADC_API_FN(_qcom_adc_intq_process((intq)))

#define qcom_adc_close() \
    QCOM_ADC_API_FN(_qcom_adc_close())

#ifdef __cplusplus
}
#endif

#endif

