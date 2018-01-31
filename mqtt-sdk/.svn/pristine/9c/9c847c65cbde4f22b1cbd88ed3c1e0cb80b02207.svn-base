#ifndef _QCA_SPI_DRIVER_H_SFJSKLJDFKLSDJFSDFD
#define _QCA_SPI_DRIVER_H_SFJSKLJDFKLSDJFSDFD


/** 
* @enum tSPI_Freq
* @brief spi frequency
*/
typedef enum {
    SPI_26000K = 0, //!< 26MHZ
    SPI_13000K = 1,  //!< 13MHZ
    SPI_6500K = 2,  //!< 6500KHZ
    SPI_3250K = 3,  //!< 3250KHZ
	SPI_1600K = 4,  //!< 1600KHZ
	SPI_800K = 5,  //!< 800KHZ
	SPI_400K = 6,  //!< 400KHZ
	SPI_200K = 7,  //!< 200KHZ
	SPI_100K = 8, //!< 100KHZ
	SPI_50K = 9  //!< 50KHZ
}tSPI_Freq;


/** 
* @enum tSPI_Mode
* @brief spi mode
* Mode 0: CPOL = 0, CPHA = 0
* Mode 1: CPOL = 0, CPHA = 1
* Mode 2: CPOL = 1, CPHA = 0
* Mode 3: CPOL = 1, CPHA = 1
* (CPOL = 0 means inactive clock is low)
* (CPOL = 1 means inactive clock is high)
* (CPHA = 0 means Leading Edge sample data, and Trailing Edge setup data)
* (CPHA = 1 means Leading Edge setup data, and Trailing Edge sample data)
*
* @warning QCA4004 and QCA4010 SPI mode1 and mode3 have a bug
*/
typedef enum {
    SPI_MODE0 = 0, //!< Spi mode 0
    SPI_MODE1 = 1, //!< Spi mode 1
    SPI_MODE2 = 2, //!< Spi mode 2
    SPI_MODE3 = 3, //!< Spi mode 3
}tSPI_Mode;

/** 
* @enum tSPI_Line
* @brief spi line
*/
typedef enum {
    SPI_0 = 0,  //!< the first SPI line
}tSPI_Line;

/** 
* @enum tSPI_Enable
* @brief Enable spi or disable spi
*/
typedef enum {
	SPI_ENABLE = 0,  //!< enable spi
	SPI_DISABLE,  //!< disable spi
}tSPI_Enable;

/** 
* @fn void qca_spi_set(tSPI_Line line, tSPI_Mode mode, tSPI_Freq freq)
* @brief Set SPI_s parameters, and if use SPI_2 then init it four pin.
* @param  line  determines use which spi line
* @param  mode  determines which mode the spi use
* @param  freq  determines which freq the spi use
* @return none
*/
void qca_spi_set(tSPI_Line line, tSPI_Mode mode, tSPI_Freq freq);	

/** 
* @fn void qca_spi_cs(tSPI_Enable enable)
* @brief Enable SPI or disable SPI(make CS up or down).
* @param  enable  set to SPI_ENABLE to enable the SPI(CS down),
*                 set to SPI_DISABLE to disable the SPI(CS up)
* @return none
*/
void qca_spi_cs(tSPI_Enable enable);

/** 
* @fn int qca_spi_write(unsigned char *buf, unsigned int len)
* @brief SPI_write chunk data.
* @param  buf  store the data writen to slave device
* @param  len: len of the data
* @return 0  succeed
*         -1  error
*/
int qca_spi_write(unsigned char *buf, unsigned int len);

/** 
* @fn int qca_spi_read(unsigned char *buf, unsigned int len)
* @brief SPI_read chunk data.
* @param  buf  buf store the data read from the slave device
* @param  len: len of the data
* @return 0  succeed
*         -1  error
*/
int qca_spi_read(unsigned char *buf, unsigned int len);

#endif

