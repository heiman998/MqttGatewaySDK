 /*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_PWM_API_H__
#define __QCOM_PWM_API_H__
/*PWM port group bit map for port_group in _qcom_pwm_control*/
#define PWM_PORT_0         0x1
#define PWM_PORT_1         0x2
#define PWM_PORT_2         0x4
#define PWM_PORT_3         0x8
#define PWM_PORT_4         0x10
#define PWM_PORT_5         0x20
#define PWM_PORT_6         0x40
#define PWM_PORT_7         0x80
#define PWM_PORT_MAX     0xff
#define PWM_PORT_ID_MAX 7
#define PWM_PRECISION_MAX 10000
#ifdef FPGA
#define PWM_SRC_CLK_0_MAX 440000000
#define PWM_SRC_CLK_0_MIN 66
#define PWM_SRC_CLK_1_MAX 260000000
#define PWM_SRC_CLK_1_MIN 39
#define PWM_SRC_CLK_2_MAX 20000000
#define PWM_SRC_CLK_2_MIN 3
#else
#define PWM_SRC_CLK_0_MAX 1300000000
#define PWM_SRC_CLK_0_MIN 195
#define PWM_SRC_CLK_1_MAX 400000000
#define PWM_SRC_CLK_1_MIN 60
#define PWM_SRC_CLK_2_MAX 20000000
#define PWM_SRC_CLK_2_MIN 3
#endif
#ifdef PHOST_INDIRECT_ACCESS

typedef struct 
{
	int (*_qcom_pwm_control)(A_BOOL module_select,A_BOOL enable,A_UINT32 port_group);
	int (*_qcom_pwm_sdm_set)(A_UINT32 freq, A_UINT32 duty,A_UINT32 port_id);
	int (*_qcom_pwm_port_set)(A_UINT32 freq,A_UINT32 duty,A_UINT32 phase,A_UINT32 port_id, A_UINT32 src_clk);

}QCOM_PWM_API_INDIRECTION_TABLE;
extern QCOM_PWM_API_INDIRECTION_TABLE qcomPwmApiIndirectionTable;
#define QCOM_PWM_API_FN(fn) qcomPwmApiIndirectionTable.fn

#else
int _qcom_pwm_port_set(A_UINT32 freq,A_UINT32 duty,A_UINT32 phase,A_UINT32 port_id, A_UINT32 src_clk);
int _qcom_pwm_sdm_set(A_UINT32 freq, A_UINT32 duty,A_UINT32 port_id);
int _qcom_pwm_control(A_BOOL module_select,A_BOOL enable,A_UINT32 port_group);

#define QCOM_PWM_API_FN(fn) fn

#endif

#define qcom_pwm_port_set(freq, duty, phase,port_id,src_clk) \
	QCOM_PWM_API_FN(_qcom_pwm_port_set((freq), (duty), (phase),(port_id),(src_clk)))
#define qcom_pwm_sdm_set(freq,duty,port_id) \
	QCOM_PWM_API_FN(_qcom_pwm_sdm_set((freq), (duty), (port_id)))
#define qcom_pwm_control(module_select, enable, port_group) \
	QCOM_PWM_API_FN(_qcom_pwm_control((module_select), (enable), (port_group)))


#endif

