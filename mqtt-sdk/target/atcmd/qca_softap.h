#ifndef _QCA_SOFTAAP_H_
#define _QCA_SOFTAAP_H_

/**
* @file 	
* @brief
* @author
* @version	V0.1.0
* @date		2016.3.01
* @details
* @copyright
* @note
*/


/**
* @brief: softap ip information
*/
#define SOFTAP_IP					"192.168.1.1"
#define SOFTAP_MASK					"255.255.255.0"
#define SOFTAP_GATEWAY				"192.168.1.1"
#define SOFTAP_IP_POOL_START		"192.168.1.100"
#define SOFTAP_IP_POOL_END			"192.168.1.255"
/**
* @fn int qca_start_softap_mode(char *ssid, char *password)
* @brief strat softap mode
* @param ssid softap ssid
* @param password softap password
* @return 
*/
int qca_start_softap_mode(char *ssid, char *password, char channel);

#endif
/***************** (C) COPYRIGHT 2017 longsys *****END OF FILE***************/
