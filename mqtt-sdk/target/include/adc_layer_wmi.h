/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
//#include "wlan_defs.h"
#ifndef _ADC_LAYER_WMI_H
#define _ADC_LAYER_WMI_H


typedef PREPACK struct {
    A_UINT32  cmd_type;   
    A_UINT32  length;
} POSTPACK WMI_ADC_HDR;



typedef enum 
{
	GAIN_SCALE_1V8=0,
	GAIN_SCALE_3V3=1,
}adc_gain_scale_t;


typedef enum {
    ADC_FREQ_UNKNOWN   = -1, /* Unsupported sampling frequency -- use PLL Bypass */
	ADC_FREQ_31P25_KHZ   = 0,
	ADC_FREQ_1_MHZ,
    ADC_FREQ_500_KHZ,
    ADC_FREQ_250_KHZ,
    ADC_FREQ_125_KHZ,
    ADC_FREQ_100_KHZ,
    ADC_FREQ_50_KHZ,
    ADC_FREQ_25_KHZ,
    ADC_FREQ_10_KHZ,
} adc_freq_t;


typedef enum {
    SINGLE_ENDED = 0,  // single ended input
    DIFFERENTIAL = 1,  // differential input 
    MIXED = 2,
}adc_input_mode_t;

#define IS_ADC_INPUT_MODE(MODE) (((MODE) == SINGLE_ENDED) || ((MODE) == DIFFERENTIAL) || ((MODE) == MIXED))



typedef enum {
    SW_TRIGGER = 0,
    HW_TRIGGER_TIMER = 1,
    HW_TRIGGER_PIN = 2
}adc_trigger_mode_t;

typedef enum {
    TIME_UNIT_CLOCK = 0,   //unit is clock period(CLK=66M)  
    TIME_UNIT_1US = 1,   //1us 
    TIME_UNIT_1MS = 2,   //1ms
    TIME_UNIT_10MS = 3    //10ms
}time_unit_mode_t;



typedef struct adc_init
{
	A_UINT8 gain_scale;
	A_UINT8 accuracy;
	A_UINT8 data_rate;
	A_UINT8 input_type;
	A_BOOL stream_bit;	
}ADC_INIT_T;


typedef struct adc_channel_config
{
  A_UINT16 input_type;		/* single ended input or differential input*/

  A_UINT16 adch;               /*!< Specifies the number of ADC channels that will be converted
                                               using the sequencer for regular channel group.
                                               This parameter must range from 0 to 7. */
}ADC_CHAN_CFG_T;


/*if trigger_mode is timer trigger, this parameter should be set*/ 
typedef struct timer_trigger_config{
    A_UINT32 time_start;
    A_UINT8 time_unit;
}ADC_TIMER_CFG_T;

typedef struct adc_config{
    
    A_UINT8 scan_mode;      /*Specifies whether the conversion is performed in
                               Scan (multichannels) or Single (one channel) mode.
                            */                                  

    A_UINT8 scan_cnt;       /*Specifies the number of ADC channels that will be converted
                                This parameter must range from 1 to 8. 
                               */
    A_UINT8 trigger_mode;  /*Conversion Trigger Select 
                            0: software trigger(write ADC_CFG0)                                                      
                            1: hardware timer trigger
                            2: hardware pin trigger
                          */                                    
   
    A_UINT8 adco_mode;  /*Specifies whether the conversion is performed in
                         Continuous or Single mode. 
                         */
                         
    ADC_CHAN_CFG_T* adc_channel;
    
}ADC_CFG_T;


typedef struct dma_channel_config{
    A_UINT32 dma_memory_base_address;
    A_UINT32 dma_buf_size;
}DMA_CHAN_CFG_T;

typedef struct dma_interrupt_config{
  A_UINT32 threshold;       /*all dma channel have the same threshold*/
  A_UINT32 data_cnt;
} DMA_CFG_T;


typedef struct adc_calib{
	A_UINT8 accuracy;
	A_UINT8 input_type;
}ADC_CALIB_T;

typedef struct adc_conver{
    A_BOOL start;
}ADC_CONV_T;


typedef struct adc_read{
    A_UINT8 channel;
	int ret;
}ADC_READ_T;

typedef struct adc_close{
	int ret;
}ADC_CLOSE_T;



typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_INIT_T adc_it;
} WMI_ADC_INIT_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_CFG_T adc_cfg;
} WMI_ADC_CFG_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_TIMER_CFG_T timer_cfg;
} WMI_ADC_TIMER_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    DMA_CFG_T dma_cfg;
} WMI_ADC_DMA_CFG_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_CALIB_T ac;
} WMI_ADC_CAL_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_CONV_T conv;
} WMI_ADC_CONV_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_READ_T ar;
} WMI_ADC_READ_CMD;

typedef PREPACK struct {
    WMI_ADC_HDR hdr;
    ADC_CLOSE_T adcClose;
} WMI_ADC_CLOSE_CMD;

typedef PREPACK struct {
    A_UINT32    resp_type;
    A_INT32    result;
    A_UINT8     data[1];
} POSTPACK WMI_ADC_RESPONSE_EVENT;

typedef struct adc_read_data_info
{   
	A_UINT16 channel;
    A_UINT16 len;
    A_UINT32 ret_len;
    A_UINT32 more;
    A_UINT32 done;
    A_UINT8  data[1];
}ADC_READ_DATA_RESP_T;

typedef PREPACK struct adc_read_data_response_s{
    A_UINT32   resp_type ;
    A_UINT32   result ;
    ADC_READ_DATA_RESP_T  data ;
}POSTPACK WMI_ADC_READ_DATA_RESPONSE_EVENT;



typedef PREPACK struct {
    A_UINT16    channel;
	A_UINT16	channel_num;
    A_INT32    len;
    A_UINT8     data[1];
} POSTPACK ADC_DMA_DATA;


enum ADC_CMDS
{
	ADC_INIT_CMD = 0,
    ADC_CFG_CMD,
    ADC_TIMER_CMD,
    ADC_DMA_CFG_CMD,
    ADC_CAL_CMD,
    ADC_CONV_CMD,
    ADC_READ_CMD,
    ADC_CLOSE_CMD,
};

enum ADC_EVENTS
{
    ADC_INIT_EVENT = 0,
    ADC_CFG_EVENT,
    ADC_TIMER_EVENT,
    ADC_DMA_CFG_EVENT,
    ADC_CAL_EVENT,
    ADC_CONV_EVENT,
    ADC_READ_EVENT,
    ADC_INTQ_EVENT,
};


enum ADC_ERRORS
{
	ADC_ERROR=-1,
	ADC_DUTY_ERROR=-2,	// data rate are not fit with accuracy
	ADC_CAL_ERROR=-3,
	ADC_READ_ERROR=-4,

};








#endif

