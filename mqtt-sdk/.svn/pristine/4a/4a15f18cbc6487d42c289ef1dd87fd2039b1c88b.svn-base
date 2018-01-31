/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
  
#ifndef __QCOM_PWR__
#define __QCOM_PWR__

#if 0
A_STATUS qcom_power_set_mode(A_UINT8 device_id, A_UINT32 mode);
A_STATUS qcom_power_get_mode(A_UINT8 device_id, A_UINT32 *pmode);
A_STATUS qcom_suspend_enable(A_BOOL enable);
A_STATUS qcom_suspend_start(A_UINT32 time);
A_STATUS qcom_power_set_parameters(A_UINT8 device_id, A_UINT16 idlePeriod, A_UINT16 psPollNum, 
    A_UINT16 dtimPolicy, A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup, 
    A_UINT16 ps_fail_event_policy);
int qcom_wlan_power_wakeup_start(A_UINT32 ms);
int qcom_wlan_power_wakeup_stop(void);
void qcom_wlan_wakeup_gpio_handler(void *pcontext);
A_STATUS qcom_wlan_wakeup_gpio_initconfig(void);
#endif

#ifdef LIBCUST_INDIRECT_ACCESS

typedef struct 
{
	A_STATUS
	(* _qcom_power_set_mode)(A_UINT8 device_id, A_UINT32 powerMode);
	A_STATUS
	(* _qcom_power_get_mode)(A_UINT8 device_id, unsigned int *powerMode);
	A_STATUS 
	(* _qcom_suspend_enable)(A_BOOL enable);
	A_STATUS 
	(* _qcom_suspend_start)(A_UINT32 time);
	A_STATUS 
	(* _qcom_power_set_parameters)(A_UINT8 device_id, A_UINT16 idlePeriod, A_UINT16 psPollNum, 
	    A_UINT16 dtimPolicy, A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup, 
	    A_UINT16 ps_fail_event_policy);
	    
    A_STATUS (*_qcom_pm_set_get_ebt_mac_filter_params)(A_UINT8 devid,
        A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);
	void
	(* _qcom_wlan_power_wakeup_timer_handler_sample)(unsigned int alarm, void *data);
	void
	(* _qcom_wlan_power_wakeup_timer_handler)(unsigned int alarm, void *data);
	int
	(* _qcom_wlan_power_wakeup_start)(A_UINT32 ms);
	int
	(* _qcom_wlan_power_wakeup_stop)(void);

	void
	(* _qcom_wlan_wakeup_gpio_handler)(void *pcontext);

	A_STATUS
	(* _qcom_wlan_wakeup_gpio_initconfig)(void);

    A_STATUS (* _qcom_suspend_restore_flag_get)(A_UINT8 *flg);

	A_STATUS
	(*_wmi_storerecall_event_handler)(A_UINT8 ucDeviceID, A_UINT16 uEventID, A_UINT8 * pEvtBuffer, A_UINT16 uLength);

#if !defined(AR6002_REV76)
	A_STATUS (*_qcom_set_keep_awake_time)(A_UINT32 time);
#endif

}QCOM_PWR_API_INDIRECTION_TABLE;
extern QCOM_PWR_API_INDIRECTION_TABLE qcomPwrApiIndirectionTable;
#define QCOM_PWR_API_FN(fn) qcomPwrApiIndirectionTable.fn

#else

	A_STATUS
	 _qcom_power_set_mode(A_UINT8 device_id, A_UINT32 powerMode);
	A_STATUS
	 _qcom_power_get_mode(A_UINT8 device_id, unsigned int *powerMode);
	A_STATUS 
	 _qcom_suspend_enable(A_BOOL enable);
	A_STATUS 
	 _qcom_suspend_start(A_UINT32 time);
	A_STATUS 
	 _qcom_power_set_parameters(A_UINT8 device_id, A_UINT16 idlePeriod, A_UINT16 psPollNum, 
	    A_UINT16 dtimPolicy, A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup, 
	    A_UINT16 ps_fail_event_policy);
	#if 0    
	static void
	 _qcom_wlan_power_wakeup_timer_handler_sample(unsigned int alarm, void *data);
	static void
	 _qcom_wlan_power_wakeup_timer_handler(unsigned int alarm, void *data);
	#endif
	int
	_qcom_wlan_power_wakeup_start(A_UINT32 ms);
	int
	 _qcom_wlan_power_wakeup_stop(void);
	void
	 _qcom_wlan_wakeup_gpio_handler(void *pcontext);

	A_STATUS
	 _qcom_wlan_wakeup_gpio_initconfig(void);

    A_STATUS _qcom_pm_set_get_ebt_mac_filter_params(A_UINT8 devid,
        A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);

    A_STATUS _qcom_set_keep_awake_time(A_UINT32 time);

	#define QCOM_PWR_API_FN(fn) fn

#endif

A_STATUS 
_qcom_suspend_restore_flag_get(A_UINT8 *flg);

#define qcom_power_set_mode(id, mode) \
	QCOM_PWR_API_FN(_qcom_power_set_mode((id), (mode)))
#define qcom_power_get_mode(id, mode) \
	QCOM_PWR_API_FN(_qcom_power_get_mode((id), (mode)))
#define qcom_suspend_enable(en) \
	QCOM_PWR_API_FN(_qcom_suspend_enable((en)))
#define qcom_suspend_start(time) \
	QCOM_PWR_API_FN(_qcom_suspend_start((time)))
#define qcom_power_set_parameters(id, period, psPollNum, dtimPolicy, wakeupPolicy, numTx, failPolicy) \
	QCOM_PWR_API_FN(_qcom_power_set_parameters((id), (period), (psPollNum), (dtimPolicy), (wakeupPolicy),  \
						(numTx), (failPolicy)))
#define qcom_wlan_power_wakeup_timer_handler_sample(alarm, data) \
	QCOM_PWR_API_FN(_qcom_wlan_power_wakeup_timer_handler_sample((alarm), (data)))
#define qcom_wlan_power_wakeup_timer_handler(alarm, data) \
	QCOM_PWR_API_FN(_qcom_wlan_power_wakeup_timer_handler((alarm), (data)))

#define qcom_wlan_power_wakeup_start(ms) \
	QCOM_PWR_API_FN(_qcom_wlan_power_wakeup_start((ms)))
#define qcom_wlan_power_wakeup_stop() \
	QCOM_PWR_API_FN(_qcom_wlan_power_wakeup_stop(()))
#define qcom_wlan_wakeup_gpio_handler(pcontext) \
	QCOM_PWR_API_FN(_qcom_wlan_wakeup_gpio_handler((pcontext)))
#define qcom_wlan_wakeup_gpio_initconfig() \
	QCOM_PWR_API_FN(_qcom_wlan_wakeup_gpio_initconfig())
#define qcom_suspend_restore_flag_get(flg) \
	QCOM_PWR_API_FN(_qcom_suspend_restore_flag_get(flg))
#define qcom_pm_set_get_ebt_mac_filter_params(id, action, ebt, mac_filter) \
    QCOM_PWR_API_FN(_qcom_pm_set_get_ebt_mac_filter_params((id), (action), (ebt), (mac_filter)))

#define wmi_storerecall_event_handler(ucDeviceID, uEventID, pEvtBuffer, uLength) \
   QCOM_PWR_API_FN(_wmi_storerecall_event_handler((ucDeviceID), (uEventID), (pEvtBuffer), (uLength)))

//ruby uart wakeup 
#define MAX_PWR_FN 0x04 
typedef void (*power_save_fn)(void); 

//extern power_save_fn gPreSleepHandler[MAX_PWR_FN]; 
//extern power_save_fn gPostAwakeHandler[MAX_PWR_FN]; 

void regist_power_save_handler(void (*pre_sleep_fn) (void), void (*post_awake_fn)(void)); 

#if defined(AR6002_REV76)

A_STATUS _qcom_set_keep_awake_time(A_UINT32 time);

#define qcom_set_keep_awake_time(time)					_qcom_set_keep_awake_time(time)

#else // !AR6002_REV76

#define qcom_set_keep_awake_time(time)					QCOM_PWR_API_FN(_qcom_set_keep_awake_time(time))

#endif // AR6002_REV76



#endif /* __QCOM_PWR__ */
