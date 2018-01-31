/*
 *  * Copyright (c) 2015 Qualcomm Atheros, Inc..
 *  * All Rights Reserved.
 *  * Qualcomm Atheros Confidential and Proprietary.
 **/


#ifndef __QCOM_GPIO_INTERRUPTS_H__
#define __QCOM_GPIO_INTERRUPTS_H__

#ifdef AR6002_REV74
#define QCOM_GPIO_UART0_RXD                 0
#define QCOM_GPIO_UART0_TXD                 1
#define QCOM_GPIO_UART0_RTS                 2
#define QCOM_GPIO_UART0_CTS                 3

#define QCOM_GPIO_UART1_RXD                 4
#define QCOM_GPIO_UART1_TXD                 5
#define QCOM_GPIO_UART1_RTS                 6
#define QCOM_GPIO_UART1_CTS                 7

#define QCOM_GPIO_UART2_RXD                 8
#define QCOM_GPIO_UART2_TXD                 9
#define QCOM_GPIO_UART2_RTS                 10
#define QCOM_GPIO_UART2_CTS                 11

#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_0     12
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_1     13
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_2     14
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_3     15
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_0     16
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_1     17
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_2     18
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_3     19
#define QCOM_GPIO_QUAD_SPI_MASTER_SCLK      20

#define QCOM_GPIO_SPI_MASTER_MOSI           21
#define QCOM_GPIO_SPI_MASTER_SCLK           22
#define QCOM_GPIO_SPI_MASTER_CSn            23
#define QCOM_GPIO_SPI_MASTER_MISO           24

#define QCOM_GPIO_I2C_MASTER_SCK            25
#define QCOM_GPIO_I2C_MASTER_SDA            26

#define QCOM_GPIO_I2C_SLAVE_SCK             27
#define QCOM_GPIO_I2C_SLAVE_SDA             28

#define QCOM_GPIO_I2S0_MCK                  29
#define QCOM_GPIO_I2S0_BCK                  30
#define QCOM_GPIO_I2S0_WS                   31
#define QCOM_GPIO_I2S0_SDI                  32
#define QCOM_GPIO_I2S0_SDO                  33

#define QCOM_GPIO_I2S1_MCK                  34
#define QCOM_GPIO_I2S1_BCK                  35
#define QCOM_GPIO_I2S1_WS                   36
#define QCOM_GPIO_I2S1_SDI                  37
#define QCOM_GPIO_I2S1_SDO                  38

#define QCOM_GPIO_GPIO_START                39
#define QCOM_GPIO_GPIO_0                    (QCOM_GPIO_GPIO_START)
#define QCOM_GPIO_GPIO_1                    (QCOM_GPIO_GPIO_START + 1)
#define QCOM_GPIO_GPIO_2                    (QCOM_GPIO_GPIO_START + 2)
#define QCOM_GPIO_GPIO_3                    (QCOM_GPIO_GPIO_START + 3)
#define QCOM_GPIO_GPIO_4                    (QCOM_GPIO_GPIO_START + 4)
#define QCOM_GPIO_GPIO_5                    (QCOM_GPIO_GPIO_START + 5)
#define QCOM_GPIO_GPIO_6                    (QCOM_GPIO_GPIO_START + 6)
#define QCOM_GPIO_GPIO_7                    (QCOM_GPIO_GPIO_START + 7)
#define QCOM_GPIO_GPIO_8                    (QCOM_GPIO_GPIO_START + 8)
#define QCOM_GPIO_GPIO_9                    (QCOM_GPIO_GPIO_START + 9)
#define QCOM_GPIO_GPIO_10                   (QCOM_GPIO_GPIO_START + 10)
#define QCOM_GPIO_GPIO_11                   (QCOM_GPIO_GPIO_START + 11)
#define QCOM_GPIO_GPIO_12                   (QCOM_GPIO_GPIO_START + 12)
#define QCOM_GPIO_GPIO_13                   (QCOM_GPIO_GPIO_START + 13)
#define QCOM_GPIO_GPIO_14                   (QCOM_GPIO_GPIO_START + 14)
#define QCOM_GPIO_GPIO_15                   (QCOM_GPIO_GPIO_START + 15)
#define QCOM_GPIO_GPIO_16                   (QCOM_GPIO_GPIO_START + 16)
#define QCOM_GPIO_GPIO_17                   (QCOM_GPIO_GPIO_START + 17)
#define QCOM_GPIO_GPIO_18                   (QCOM_GPIO_GPIO_START + 18)
#define QCOM_GPIO_GPIO_19                   (QCOM_GPIO_GPIO_START + 19)
#define QCOM_GPIO_GPIO_20                   (QCOM_GPIO_GPIO_START + 20)
#define QCOM_GPIO_GPIO_21                   (QCOM_GPIO_GPIO_START + 21)
#define QCOM_GPIO_GPIO_22                   (QCOM_GPIO_GPIO_START + 22)
#define QCOM_GPIO_GPIO_23                   (QCOM_GPIO_GPIO_START + 23)
#define QCOM_GPIO_GPIO_24                   (QCOM_GPIO_GPIO_START + 24)
#define QCOM_GPIO_GPIO_25                   (QCOM_GPIO_GPIO_START + 25)
#define QCOM_GPIO_GPIO_26                   (QCOM_GPIO_GPIO_START + 26)
#define QCOM_GPIO_GPIO_27                   (QCOM_GPIO_GPIO_START + 27)
#define QCOM_GPIO_GPIO_28                   (QCOM_GPIO_GPIO_START + 28)
#define QCOM_GPIO_GPIO_29                   (QCOM_GPIO_GPIO_START + 29)
#define QCOM_GPIO_GPIO_END                  QCOM_GPIO_GPIO_29

/*
#define QCOM_GPIO_JTAG_START                (QCOM_GPIO_GPIO_END + 1)
#define QCOM_GPIO_JTAG_TDI                  QCOM_GPIO_JTAG_START
#define QCOM_GPIO_JTAG_TMS                  (QCOM_GPIO_JTAG_START + 1)
#define QCOM_GPIO_JTAG_TCK                  (QCOM_GPIO_JTAG_START + 2)
#define QCOM_GPIO_JTAG_TDO                  (QCOM_GPIO_JTAG_START + 3)
*/
#endif /* AR6002_REV74 */

#ifdef AR6002_REV76
#define QCOM_GPIO_UART0_RXD                 0
#define QCOM_GPIO_UART0_TXD                 1
#define QCOM_GPIO_UART0_RTS                 2
#define QCOM_GPIO_UART0_CTS                 3

#define QCOM_GPIO_UART1_RXD                 4
#define QCOM_GPIO_UART1_TXD                 5
#define QCOM_GPIO_UART1_RTS                 6
#define QCOM_GPIO_UART1_CTS                 7

#define QCOM_GPIO_UART2_RXD                 8
#define QCOM_GPIO_UART2_TXD                 9
#define QCOM_GPIO_UART2_RTS                 10
#define QCOM_GPIO_UART2_CTS                 11

#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_0     12
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_1     13
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_2     14
#define QCOM_GPIO_QUAD_SPI_MASTER_CSn_3     15
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_0     16
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_1     17
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_2     18
#define QCOM_GPIO_QUAD_SPI_MASTER_DIO_3     19
#define QCOM_GPIO_QUAD_SPI_MASTER_SCLK      20

#define QCOM_GPIO_SPI_MASTER_MOSI           21
#define QCOM_GPIO_SPI_MASTER_SCLK           22
#define QCOM_GPIO_SPI_MASTER_CSn            23
#define QCOM_GPIO_SPI_MASTER_MISO           24

#define QCOM_GPIO_I2C_MASTER_SCK            25
#define QCOM_GPIO_I2C_MASTER_SDA            26

#define QCOM_GPIO_I2C_SLAVE_SCK             27
#define QCOM_GPIO_I2C_SLAVE_SDA             28

#define QCOM_GPIO_I2S0_MCK                  29
#define QCOM_GPIO_I2S0_BCK                  30
#define QCOM_GPIO_I2S0_WS                   31
#define QCOM_GPIO_I2S0_SDI                  32
#define QCOM_GPIO_I2S0_SDO                  33

#define QCOM_GPIO_I2S1_MCK                  34
#define QCOM_GPIO_I2S1_BCK                  35
#define QCOM_GPIO_I2S1_WS                   36
#define QCOM_GPIO_I2S1_SDI                  37
#define QCOM_GPIO_I2S1_SDO                  38

#define QCOM_GPIO_GPIO_START                39
#define QCOM_GPIO_GPIO_0                    (QCOM_GPIO_GPIO_START)
#define QCOM_GPIO_GPIO_1                    (QCOM_GPIO_GPIO_START + 1)
#define QCOM_GPIO_GPIO_2                    (QCOM_GPIO_GPIO_START + 2)
#define QCOM_GPIO_GPIO_3                    (QCOM_GPIO_GPIO_START + 3)
#define QCOM_GPIO_GPIO_4                    (QCOM_GPIO_GPIO_START + 4)
#define QCOM_GPIO_GPIO_5                    (QCOM_GPIO_GPIO_START + 5)
#define QCOM_GPIO_GPIO_6                    (QCOM_GPIO_GPIO_START + 6)
#define QCOM_GPIO_GPIO_7                    (QCOM_GPIO_GPIO_START + 7)
#define QCOM_GPIO_GPIO_8                    (QCOM_GPIO_GPIO_START + 8)
#define QCOM_GPIO_GPIO_9                    (QCOM_GPIO_GPIO_START + 9)
#define QCOM_GPIO_GPIO_10                   (QCOM_GPIO_GPIO_START + 10)
#define QCOM_GPIO_GPIO_11                   (QCOM_GPIO_GPIO_START + 11)
#define QCOM_GPIO_GPIO_12                   (QCOM_GPIO_GPIO_START + 12)
#define QCOM_GPIO_GPIO_13                   (QCOM_GPIO_GPIO_START + 13)
#define QCOM_GPIO_GPIO_14                   (QCOM_GPIO_GPIO_START + 14)
#define QCOM_GPIO_GPIO_15                   (QCOM_GPIO_GPIO_START + 15)
#define QCOM_GPIO_GPIO_16                   (QCOM_GPIO_GPIO_START + 16)
#define QCOM_GPIO_GPIO_17                   (QCOM_GPIO_GPIO_START + 17)
#define QCOM_GPIO_GPIO_18                   (QCOM_GPIO_GPIO_START + 18)
#define QCOM_GPIO_GPIO_19                   (QCOM_GPIO_GPIO_START + 19)
#define QCOM_GPIO_GPIO_20                   (QCOM_GPIO_GPIO_START + 20)
#define QCOM_GPIO_GPIO_21                   (QCOM_GPIO_GPIO_START + 21)
#define QCOM_GPIO_GPIO_22                   (QCOM_GPIO_GPIO_START + 22)
#define QCOM_GPIO_GPIO_23                   (QCOM_GPIO_GPIO_START + 23)
#define QCOM_GPIO_GPIO_24                   (QCOM_GPIO_GPIO_START + 24)
#define QCOM_GPIO_GPIO_25                   (QCOM_GPIO_GPIO_START + 25)
#define QCOM_GPIO_GPIO_26                   (QCOM_GPIO_GPIO_START + 26)
#define QCOM_GPIO_GPIO_27                   (QCOM_GPIO_GPIO_START + 27)
#define QCOM_GPIO_GPIO_28                   (QCOM_GPIO_GPIO_START + 28)
#define QCOM_GPIO_GPIO_29                   (QCOM_GPIO_GPIO_START + 29)
#define QCOM_GPIO_GPIO_30                   (QCOM_GPIO_GPIO_START + 30)
#define QCOM_GPIO_GPIO_31                   (QCOM_GPIO_GPIO_START + 31)
#define QCOM_GPIO_GPIO_32                   (QCOM_GPIO_GPIO_START + 32)
#define QCOM_GPIO_GPIO_33                   (QCOM_GPIO_GPIO_START + 33)
#define QCOM_GPIO_GPIO_34                   (QCOM_GPIO_GPIO_START + 34)
#define QCOM_GPIO_GPIO_35                   (QCOM_GPIO_GPIO_START + 35)
#define QCOM_GPIO_GPIO_36                   (QCOM_GPIO_GPIO_START + 36)
#define QCOM_GPIO_GPIO_37                   (QCOM_GPIO_GPIO_START + 37)
#define QCOM_GPIO_GPIO_38                   (QCOM_GPIO_GPIO_START + 38)
#define QCOM_GPIO_GPIO_39                   (QCOM_GPIO_GPIO_START + 39)
#define QCOM_GPIO_GPIO_40                   (QCOM_GPIO_GPIO_START + 40)
#define QCOM_GPIO_GPIO_41                   (QCOM_GPIO_GPIO_START + 41)
#define QCOM_GPIO_GPIO_END                  QCOM_GPIO_GPIO_41

/*
#define QCOM_GPIO_JTAG_START                (QCOM_GPIO_GPIO_END + 1)
#define QCOM_GPIO_JTAG_TDI                  QCOM_GPIO_JTAG_START
#define QCOM_GPIO_JTAG_TMS                  (QCOM_GPIO_JTAG_START + 1)
#define QCOM_GPIO_JTAG_TCK                  (QCOM_GPIO_JTAG_START + 2)
#define QCOM_GPIO_JTAG_TDO                  (QCOM_GPIO_JTAG_START + 3)
*/
#endif /* AR6002_REV76 */




#endif /* __QCOM_GPIO_INTERRUPTS_H__ */
