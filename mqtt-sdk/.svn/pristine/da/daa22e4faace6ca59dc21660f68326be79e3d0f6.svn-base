/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * 
//
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
//
//
 */
//


#ifndef QCOM_I2C_SLAVE_API_H_
#define QCOM_I2C_SLAVE_API_H_

//#include <osapi.h>

/* I2C slave api is the HAL for  I2C slave
 * hardware. I2C slave hardware provide three modes of
 * communication with an external I2C master. These modes are outlined 
 * below:
 *
 * ----------------------CSR Mode-----------------------------------------
 * -----------------------------------------------------------------------
 * Configuration and status mode - This mode allows the external I2C master to 
 * exchange 1 byte of information. Five bits out of the eight are available for
 * user definition and usage. The I2C slave API does not define on interpret
 * these bits. In addition, there are 2 more bits
 * which can be used by an external I2C master for following purposes-
 * 1) interrupt i2c slave host( bit0: 1 for enable interrupt
 * 2) request i2c slave host clock(bit1: 1 for request, 0 for no need to request
 *
 *----------------------REG mode--------------------------------------------
 *--------------------------------------------------------------------------
 * Register mode: This is similar to CSR mode except that there 
 * are up to 4 bytes that can be exchanged and there are  
 *  no special bits. Host can access/update the 4 bytes via the 
 * APB read/write. There are read complete and write complete interrupts
 * available
 *
 *
 * --------------------FIFO mode-----------------------------------------
 *  ---------------------------------------------------------------------
 *
 * FIFO: Similar to REG mode except that the in/out 
 * from memory procedes in the form of a FIFO. Thus, 
 * there will never be memory corruption due to bounds
 * exceding provided the memory location and the size 
 * specified at the time of service registration is 
 * correct. There is however a possibility of wraparound and stale data
 * There are various interrupts present to assist user software and notifies
 * start, end indications to the registered user
 * NOTE: master read operation will be failed without write operation before it
 * 
 * */


/***************** Macro definitions*****************************/

#define I2CS_CSR_USER_BIT_0         0x1
#define I2CS_CSR_USER_BIT_1         0x2
#define I2CS_CSR_USER_BIT_2         0x4
#define I2CS_CSR_USER_BIT_3         0x8
#define I2CS_CSR_USER_BIT_4         0x10
/*Interrupt mask macros. Each service has a 16 bitwide field which is used to
 * communicate interrupt related information between the service API and the
 * user software*/

/*Interrupt masks for CSR service.*/
#define I2CS_INTR_CSR                  0x1

/*Interrupt masks for REG service*/
#define I2CS_INTR_REG_READ_START       0x1                                          
#define I2CS_INTR_REG_READ_FINISH      0x2
#define I2CS_INTR_REG_WRITE_START      0x4
#define I2CS_INTR_REG_WRITE_FINISH     0x8

/*Interrupt masks for FIFO service*/
#define I2CS_INTR_FIFO_READ_START      0x1                                         
#define I2CS_INTR_FIFO_READ_FINISH     0x2
#define I2CS_INTR_FIFO_WRITE_START     0x4
#define I2CS_INTR_FIFO_WRITE_FINISH    0x8
#define I2CS_INTR_FIFO_READ_EMPTY      0x10
#define I2CS_INTR_FIFO_WRITE_FULL      0x20
typedef enum i2cs_pin_pair_t{
	I2CS_PIN_PAIR_0     = 0,
	I2CS_PIN_PAIR_1  ,
	I2CS_PIN_PAIR_MAX
}I2CS_PIN_PAIR;
typedef enum i2cs_fifo_size_t{
	I2CS_FIFO_SIZE_8 =0,       
	I2CS_FIFO_SIZE_16,      
	I2CS_FIFO_SIZE_32,      
	I2CS_FIFO_SIZE_64,      
	I2CS_FIFO_SIZE_128,     
	I2CS_FIFO_SIZE_256, 
	I2CS_FIFO_SIZE_512,     
	I2CS_FIFO_SIZE_1024    
}I2CS_FIFO_SIZE;
typedef enum i2cs_mode_type_t{
    I2CS_REG_MODE     = 0,
    I2CS_FIFO_MODE    = 1,
    I2CS_CSR_MODE      = 2,
    I2CS_MODE_MAX     = 3
}I2CS_MODE_TYPE;
/***************End Macro Definitions*********************************/


/****************** Enum TYPE definitions***********************************/
   
/* i2c slave command types*/
typedef enum i2cs_cmd_type_t{
    I2CS_CMD_CSR_ADDR_SET,
    I2CS_CMD_CSR_ADDR_GET,
    I2CS_CMD_CSR_DATA_READ ,
    I2CS_CMD_CSR_DATA_WRITE ,
    I2CS_CMD_REG_ADDR_SET,
    I2CS_CMD_REG_ADDR_GET,
    I2CS_CMD_REG_DATA_READ ,
    I2CS_CMD_REG_DATA_WRITE,
    I2CS_CMD_FIFO_ADDR_SET,
    I2CS_CMD_FIFO_ADDR_GET,
    I2CS_CMD_FIFO_READ_BASE_SET,
    I2CS_CMD_FIFO_READ_BASE_GET ,
    I2CS_CMD_FIFO_WRITE_BASE_SET,
    I2CS_CMD_FIFO_WRITE_BASE_GET,
    I2CS_CMD_READ_FIFO_WRITE_UPDATE,
    I2CS_CMD_WRITE_FIFO_READ_UPDATE,
}I2CS_CMD_TYPE;
/****************Structure definitions****************************/

/* All API related structures should be allocated and initialized
 by the user software registering with the service. The API will only 
 use it for initialization and later populate the fields during call backs
 */


/*interrupt call back args for CSR service*/
typedef struct i2cs_csr_cb_args_t{
    A_UINT16       intr;   /* CSR interrupts which resulted in this callback */
    A_UINT8       csr_data;
}I2CS_CSR_CB_ARGS;

/*interrupt call back args for REG service*/
typedef struct i2cs_reg_cb_args_t{
    A_UINT16       intr;
    A_UINT32        reg_data;
}I2CS_REG_CB_ARGS;

/*interrupt call back args for FIFO service*/
typedef struct i2cs_fifo_cb_args_t{
    A_UINT16       intr;
}I2CS_FIFO_CB_ARGS;

/* csr service registration parameters */
typedef struct i2cs_csr_param_t{
    A_UINT8 id;    /*i2c slave address */
    void (* cb)(I2CS_CSR_CB_ARGS *);/*call back upon interrupts or events*/
    A_UINT16 intr; /*interrupt interest*/
    I2CS_CSR_CB_ARGS * cb_args;   /*callback args populated here*/
}I2CS_CSR_PARA;

/*reg service registration parameters*/
typedef struct i2cs_reg_params_t{
    A_UINT8 id; 
    void (* cb)(I2CS_REG_CB_ARGS *);
    A_UINT8 intr;
    I2CS_REG_CB_ARGS * cb_args;
}I2CS_REG_PARA;

/*fifo service registration parameters*/
typedef struct i2cs_fifo_params_t{
    A_UINT8     id;
    A_UINT32    write_base;    
    I2CS_FIFO_SIZE    write_length;
    A_UINT32    read_base;    
    I2CS_FIFO_SIZE    read_length;
    A_UINT32    intr;
    void (* cb)(I2CS_FIFO_CB_ARGS *);
    I2CS_FIFO_CB_ARGS * cb_args;
}I2CS_FIFO_PARA;


#ifdef PHOST_INDIRECT_ACCESS

typedef struct 
{
	A_INT32 (*_qcom_i2cs_csr_init)(I2CS_CSR_PARA *csr_params);
	A_INT32 (*_qcom_i2cs_reg_init)(I2CS_REG_PARA *reg_params);
	A_INT32 (*_qcom_i2cs_fifo_init)(I2CS_FIFO_PARA *fifo_params);
	A_INT32 (*_qcom_i2cs_control)(int enable, A_UINT32 pin_pair_id);
	A_INT32 (*_qcom_i2cs_cmd)(A_UINT32 cmd,A_UINT32 *data );
}QCOM_I2CS_API_INDIRECTION_TABLE;
extern QCOM_I2CS_API_INDIRECTION_TABLE qcomI2csApiIndirectionTable;
#define QCOM_I2CS_API_FN(fn) qcomI2csApiIndirectionTable.fn

#else
A_INT32 _qcom_i2cs_csr_init(I2CS_CSR_PARA *csr_params);
A_INT32 _qcom_i2cs_reg_init(I2CS_REG_PARA *reg_params);
A_INT32 _qcom_i2cs_fifo_init(I2CS_FIFO_PARA *fifo_params);
A_INT32 _qcom_i2cs_control(int enable, A_UINT32 pin_pair_id);
A_INT32 _qcom_i2cs_cmd(A_UINT32 cmd,A_UINT32 *data );

#define QCOM_I2CS_API_FN(fn) fn

#endif

#define qcom_i2cs_csr_init(csr_params) \
	QCOM_I2CS_API_FN(_qcom_i2cs_csr_init((csr_params)))
#define qcom_i2cs_reg_init(reg_params) \
	QCOM_I2CS_API_FN(_qcom_i2cs_reg_init((reg_params)))
#define qcom_i2cs_fifo_init(fifo_params) \
	QCOM_I2CS_API_FN(_qcom_i2cs_fifo_init((fifo_params)))
#define qcom_i2cs_control(enable,pin_pair_id) \
	QCOM_I2CS_API_FN(_qcom_i2cs_control((enable),(pin_pair_id)))
#define qcom_i2cs_cmd(cmd, data) \
	QCOM_I2CS_API_FN(_qcom_i2cs_cmd((cmd), (data)))

#endif /* QCOM_I2C_SLAVE_API_H_ */
