
#ifndef _QCA_OTA_H_
#define _QCA_OTA_H_


/** 
* @file 	qca_ota.h	
* @brief	ota upgrade driver.
* @author	
* @version	V0.1.0
* @date		2016.3.15
* @details	
* @copyright 
* @note		OTA process
* 1.qca_ota_start()
* 2.qca_ota_write(), loop
* 3.qca_ota_finish()
* 4.system restart
*/


/**
* @fn int qca_ota_start(void);
* @brief start OTA
* @param none
* @return 0£ºsuccessful, -1£ºfailed
*/
int qca_ota_start(void);


/**
* @fn int qca_ota_write(unsigned char *buf, unsigned int len);
* @brief write ota file data
* @param unsigned char *buf, unsigned int len
* @return 0	successful
*		-1	failed
*		-2	fw length overflow(<0x100000)
* @see write firmware the first packet must more than 24 byte
*		every packet 1024 byte is good
*/
int qca_ota_write(unsigned char *buf, unsigned int len);


/**
* @fn int qca_ota_finish(void)
* @brief ota finish
* @param none
* @return 0 succeed
*         -1 failed
* @warning	this API will check md5, so, you should check this API's return value
            in your code, if the return value is -1, it means ota failed
*/
int qca_ota_finish(void);

/**
* @fn int qca_ota_get_current_partition(void)
* @brief get current firmware running partition
* @param none
* @return 0 current firmware running in partition 0
*         1 current firmware running in partition 1
*         -1 get failed
*/
int qca_ota_get_current_partition(void);

#endif

