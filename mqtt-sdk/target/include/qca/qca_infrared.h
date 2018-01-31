
#ifndef _QCA_INFRARED_H_
#define _QCA_INFRARED_H_

/**
* @brief: infrared sensor start/stop receive data
* @note: when the infrared sensor is receiving data we need suspend all other threads, because interrupt is not preempt.
*		 INFRARED_START -> suspend all other threads
*		 INFRARED_END -> resume all other threads
*/
typedef enum 
{
	INFRARED_START	= 0,	
	INFRARED_END
}t_infrared_state_type;


/**
* @brief: infrared sensor callback function
* @param: INFRARED_START -> suspend all other threads
*		  INFRARED_END -> resume all other threads
*/
/**
***************************************************************************
*@brief		:	infrared sensor callback function
*@param[in]	:	state	INFRARED_START -> suspend all other threads
*						INFRARED_END -> resume all other threads
*@return	:	none
*@see		:
***************************************************************************
*/
typedef void(*infared_func)(t_infrared_state_type state);

/**
***************************************************************************
*@brief		:	install infrared sensor driver 
*@param[in]	:	pin				gpio pin nember
*				cache_len		cache length(int type)
*				idle_time_ms	the time interval between the two frame data
*				mode			interrupt mode
*				cb				the callback function
*@return	:	0	success
*				-1	failed
*@see		:	after idle_time_ms when received data, we can parse it security
***************************************************************************
*/
int qca_infared_install(int pin, int cache_len, int idle_time_ms, QCOM_GPIO_PIN_INTERRUPT_MODE mode, infared_func cb);

/**
***************************************************************************
*@brief		:	uninstall infrared sensor driver
*@param[in]	:	none
*@return	:	none
*@see		:
***************************************************************************
*/
void qca_infared_uninstall(void);

/**
***************************************************************************
*@brief		:	get infrared sensor received data
*@param[in]	:	buf		data buffer
*@return	:	data length
*@note		:	when debugging you can use printf, but release it must be disabled
***************************************************************************
*/
int qca_infared_get_data(unsigned int *buf);

#endif
