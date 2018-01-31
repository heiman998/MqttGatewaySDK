#ifndef _QCA_LTLINK_H_
#define _QCA_LTLINK_H_

#include "base.h"
#include "qcom_sec.h"

/**
* @fn typedef void(*ltlink_success_cb)(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt)
* @brief ltlink success callback function
* @param ssid		AP's ssid
* @param password	AP's password
* @param respond    link response
* @param auth		refer to WLAN_AUTH_MODE
* @param crypt		refer to WLAN_CRYPT_TYPE
*/
typedef void(*ltlink_success_cb)(char *ssid, char *password, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE crypt);

/**
* @fn int qca_ltlink_start(ltlink_success_cb cb)
* @brief start ltlink
* @param cb ltlink success callback function
*/
int qca_ltlink_start(ltlink_success_cb cb);

/**
* @fn int qca_ltlink_get_next_valid_channel(void)
* @brief get next valid channel to set channel
* @param none
* @return channel
*/
int qca_ltlink_get_next_valid_channel(void);

/**
* @fn void qca_ltlink_promiscuous_cb(unsigned char *buf, int length)
* @brief register promiscuous callback
* @param buf Data received in promiscuous mode
* @param length the length of data
* @return none
*/
void qca_ltlink_promiscuous_cb(unsigned char *buf, int length);

/**
* @fn int qca_ltlink_is_lock_channel(void)
* @brief Check if the channel has been locked 
* @param none
* @return 
*/
int qca_ltlink_is_lock_channel(void);

/**
* @fn int qca_ltlink_response(int port, char *data, int length, int count, int delayms)
* @brief respone to App after connected router
* @param port udp port
* @param data respond data buf
* @param length data length
* @param count send times
* @param delayms packet interval time
* @return 
*/
int qca_ltlink_response(int port, char *data, int length, int count, int delayms);

/**
* @fn int qca_ltlink_set_key(unsigned char* key, unsigned int length)
* @brief set encrypt key
* @param key key data
* @param length key len(len<=16 bytes)
* @return 0: success -1: failed
*/
int qca_ltlink_set_key(unsigned char* key, unsigned int length);

/**
* @fn int qca_ltlink_pause(void)
* @brief stop ltlink
* @param 0 resume, 1 pause 
* @return 0 success, -1 failed
*/
int qca_ltlink_pause(int state);

/**
* @fn int qca_ltlink_stop(void)
* @brief stop ltlink
* @param none
* @return 0 success, -1 failed
*/
int qca_ltlink_stop(void);

/**
* @fn int ltlink_lisence_verify(char *key)
* @brief verify the validity of the user
* @param key lisence data
* @return 0 lisence ok, -1 lisence err
*/
int ltlink_lisence_verify(char *key);

#endif	 

