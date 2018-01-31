#ifndef _PSRAM_PARAM_H_
#define _PSRAM_PARAM_H_

/** 
* @file 	qca_psram_param.h	
* @brief	psram driver.
* @author	
* @version	V0.1.0
* @date		2016.3.15
* @details	
* @copyright 
* @note		
*/

/**
* @fn int qca_psram_dset_write(unsigned char *buf, unsigned int len)
* @brief write psram
* @param *buf param buf
* @param len buf size(len < 0x2000)
* @return 0:successful, -1:failed, -2:create error
*/
int qca_psram_dset_write(unsigned char *buf, unsigned int len);

/**
* @fn int qca_psram_dset_read(unsigned char *buf, unsigned int len);
* @brief read psram
* @param *buf param buf
* @param len buf size
* @return 0:successful, -1:failed, -2:create error
*/
int qca_psram_dset_read(unsigned char *buf, unsigned int len);



#endif

