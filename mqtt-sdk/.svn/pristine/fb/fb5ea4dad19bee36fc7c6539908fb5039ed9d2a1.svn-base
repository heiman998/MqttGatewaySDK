
#ifndef _QCA_AP_CONNECT_H_
#define _QCA_AP_CONNECT_H_

#include "base.h"
#include "qcom_sec.h"
#include "qcom_scan.h"

/**
* @brief: connect mode
*/
typedef enum 
{
	WITHOUT_SCAN = 0,		// connect AP without scan
	WITH_SCAN				// connect AP with scan
}t_connect_mode_type;

/**
* @brief: connect result
*/
typedef enum
{
	CONNECT_OK = 0,
	SSID_ERROR,
	SCAN_FAILED,
	CONNECT_FAILED,
	CONNECT_TIMEOUT,
	DHCP_TIMEOUT,
	PASSWORD_ERROR
}t_connect_result;

int qca_scan_ap(QCOM_BSS_SCAN_INFO** ptscan_info);
/**
***************************************************************************
*@brief		:	set the encryption mode
*@param[in]	:	auth_mode	authentication mode
*				crypt_type	encryption mode 
*@return	:	
*@warning	:
*@see		:
***************************************************************************
*/
void qca_ap_set_auth_encrypt(WLAN_AUTH_MODE auth_mode, WLAN_CRYPT_TYPE crypt_type);

/**
***************************************************************************
*@brief		:	start station mode
*@param[in]	:	
*				mode		WITH_SCAN connect with scan
*							WITHOUT_SCAN connect without scan
*				ssid		station ap ssid
*				password	station ap password
*			    timeout     timeout time
*@return	:	0			success
*				-1			failed
*				other		connect state, see struct t_connect_result for detail
*@warning	:
*@see		:	if mode set to WITHOUT_SCAN, qca_ap_set_auth_encrypt() have to
*      			be called to set the auth mode and encrypt mode first
*
***************************************************************************
*/
t_connect_result qca_start_station_mode(t_connect_mode_type mode, char *ssid, char *password, int timeout_ms);

/**
***************************************************************************
*@brief		:	get station ip config
*@param[out]:	ip   		buffer to store ip address
*				mask       buffer to store mask value
*				gateway    buffer to store gateway address
*@return	:	0			success
*				-1			failed
*@warning	:
*@see		:
***************************************************************************
*/
int qca_get_station_ip_config(unsigned int *ipAddress, unsigned int *submask, unsigned int *gateway);


#endif

