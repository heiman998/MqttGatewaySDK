#ifndef _QCA_I2C_DRIVER_H_SKLFJKSLJDFKLSJDKFSD
#define _QCA_I2C_DRIVER_H_SKLFJKSLJDFKLSJDKFSD

/**
* @enum tI2CM_Freq
* @brief The I2C master clock frequency
*/
typedef enum {
    I2CM_4000K  =   3,
    I2CM_2000K  =   4,
    I2CM_1000K   =  5,
    I2CM_500K   =   6,
    I2CM_250K   =   7,
    I2CM_125K   =   8,
    I2CM_63K    =   9
}tI2CM_Freq;

/**
* @fn int qca_i2cm_init(tI2CM_Freq freq, int write_wait_cnt)
* @brief Init the I2C master.
* @param freq The I2C master clock frequency.
* @param write_wait_cnt The I2C master write try count, 0 is default(0xfff)
* @return 0: succeed other error code: failed
* @note: when the I2C master receive nack from slave, the i2c master will re-send the byte,
         write_wait_cnt is set the re-send try times.
         if write_wait_cnt == 0xfff, the block time is about 350ms,
         if write_wait_cnt == 0xff, the block time is about 2.41ms
*/
int qca_i2cm_init(tI2CM_Freq freq, int write_wait_cnt);
/**
* @fn int qca_i2cm_fini(void)
* @brief Init the I2C master.
* @param freq The I2C master clock frequency.
* @return 0: succeed other error code: failed
*/
int qca_i2cm_fini(void);
/**
* @fn int qca_i2cm_read(unsigned char dev_addr, unsigned int word_addr, unsigned char word_addr_len, unsigned char *data, unsigned int data_len)
* @brief Init the I2C master.
* @param dev_addr I2C device ID (unique 7-bit address)
* @param word_addr word/reg address of I2C device to read, Ignore it in simple read format
* @param word_addr_len length of word_addr data, 1-6  bytes is preferred to combined format, 0 for simple read format
* @param data The buffer pointer to the Rx data
* @param data_len length of Rx chunk data 0-8 bytes is preferred
* @return 0 succeed -1 failed
* @warning word_addr_len + data_len must <= 7
*/
int qca_i2cm_read(unsigned char dev_addr, unsigned int word_addr, unsigned char word_addr_len, unsigned char *data, unsigned int data_len);
/**
* @fn int qca_i2cm_write(unsigned char dev_addr, unsigned int word_addr, unsigned char word_addr_len, unsigned char *data, unsigned int data_len)
* @brief Init the I2C master.
* @param dev_addr I2C device ID (unique 7-bit address)
* @param word_addr word/reg address of I2C device to write, Ignore it in simple write format
* @param word_addr_len length of word_addr data, 1-7  bytes is preferred to combined format, 0 for simple write format
* @param data The buffer pointer to the Tx data
* @param data_len length of Tx chunk data 1-8 bytes is preferred
* @return 0 succeed -1 failed
* @warning word_addr_len + data_len must <= 7
*/
int qca_i2cm_write(unsigned char dev_addr, unsigned int word_addr, unsigned char word_addr_len, unsigned char *data, unsigned int data_len);

#endif
