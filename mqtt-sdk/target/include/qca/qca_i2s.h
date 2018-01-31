#ifndef _QCA_I2S_H_KSJDKLJSDFKLJKLDSF_
#define _QCA_I2S_H_KSJDKLJSDFKLJKLDSF_

/**
* @brief: i2s interrupt callback, use for Tx done or Rx ready
*/
typedef void (*qca_i2s_intr_cb_t)(void *);

/**
* @brief: I2S bus 0/1
*/
typedef enum{
    QCA_I2S_0 = 0,
    QCA_I2S_1 = 1,
}qca_i2s_t;

/**
* @brief: I2S bus frequency
*/
typedef enum{
	QCA_I2S_FREQ_44_1K	 = 0,
	QCA_I2S_FREQ_48K	= 1,
	QCA_I2S_FREQ_32K 	= 2,
	QCA_I2S_FREQ_96K	= 3,
	QCA_I2S_FREQ_16K	= 4,
	QCA_I2S_FREQ_MAX	= 5,    
}qca_i2s_freq_t;

/**
* @brief: I2S bus word size
*/
typedef enum{
    QCA_I2S_WORD_SIZE_8 = 8,
    QCA_I2S_WORD_SIZE_16 = 16,
    QCA_I2S_WORD_SIZE_24 = 24,
    QCA_I2S_WORD_SIZE_32 = 32,
}qca_i2s_word_size_t;

/**
* @brief: I2S bus mode
*/
typedef enum{
    QCA_I2S_MODE_SLAVE = 0,
    QCA_I2S_MODE_MASTER = 1,
}qca_i2s_mode_t;

/**
* @brief: I2S bus config
*/
typedef struct{
    qca_i2s_freq_t freq;
    qca_i2s_word_size_t word_size;
    unsigned short buf_size;
    unsigned char dma_buf_count; // Tx(master) or Rx(slave) DMA buffer count 
    qca_i2s_mode_t mode;
    qca_i2s_intr_cb_t cb; // Tx done callback when in master mode, Rx ready callback when in slave mode
}qca_i2s_config_t;

/**
* @fn int qca_i2s_init(qca_i2s_t port, qca_i2s_config_t *config)
* @brief init i2s bus
* @param port i2s bus to init
* @param config i2s bus config
* @return  0: success
*         -1: i2s init error
*         -2: i2s bus busy
*         -3: mode not found(direction have be QCA_I2S_DIR_RECV or QCA_I2S_DIR_SEND)
*/
int qca_i2s_init(qca_i2s_t port, qca_i2s_config_t *config);

/**
* @fn int qca_i2s_recv_start(qca_i2s_t port)
* @brief recv start
* @param port i2s bus to start recv
* @return -1 i2s bus have not been init
*         0 success
*/
int qca_i2s_recv_start(qca_i2s_t port);

/**
* @fn int qca_i2s_recv_stop(qca_i2s_t port)
* @brief recv stop(must call qca_i2s_recv_start() first)
* @param port i2s bus to stop recv
* @return -1 i2s bus have not been init
*         0 success
*/
int qca_i2s_recv_stop(qca_i2s_t port);

/**
* @fn int qca_i2s_recv(qca_i2s_t port, unsigned char *buf, unsigned int *size)
* @brief i2s recv data
* @param port i2s bus to recv
* @param buf buffer to store recv data
* @param size recv buffer size
* @return 0 No data remaining to be recv
*         1 have Data remaining to be recv
*        -1 i2s bus have not been init
*/
int qca_i2s_recv(qca_i2s_t port, unsigned char *buf, unsigned int *size);

/**
* @fn int qca_i2s_send(qca_i2s_t port, unsigned char *buf, unsigned int size)
* @brief i2s send data
* @param port i2s bus to send
* @param buf buffer to send
* @param size buffer size
* @return >0 Data remaining to be send
*          0 No data remaining to be send
*         -1 i2s bus have not been init
*/
int qca_i2s_send(qca_i2s_t port, unsigned char *buf, unsigned int size);

/**
* @fn void qca_i2s_stop(qca_i2s_t port)
* @brief i2s bus stop
* @param port i2s bus to stop
* @return
*/
void qca_i2s_stop(qca_i2s_t port);

#endif
