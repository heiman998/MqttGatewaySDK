#ifndef __QCA_SSL_CLIENT_H
#define __QCA_SSL_CLIENT_H

/**
***************************************************************************
*@brief     :   Start a ssl client
*@param[in] :   cert     certificate buffer
*               cert_len len of certificate buffer
*@return    :   A_OK: succeed
*               A_ERROR: failed
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_client_start(unsigned char *cert, unsigned short cert_len);

/**
***************************************************************************
*@brief     :   Stop a started ssl client
*@param[in] :   none
*@return    :   A_OK: succeed
*               A_ERROR: failed
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_client_stop(void);

/**
***************************************************************************
*@brief     :   Create a socket
*@param[in] :   none
*@return    :   -1: failed
*               other: socket fd
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_socket(void);

/**
***************************************************************************
*@brief     :   Connect to a ssl server
*@param[in] :   fd   socket fd created by qca_ssl_socket()
*               ip   ssl server ip
*               port ssl server port
*@return    :   A_OK: succeed
*               A_ERROR: failed
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_connect(int* fd, unsigned int ip, int port);

/**
***************************************************************************
*@brief     :   Write chunk data to a ssl server
*@param[in] :   pbuf data buffer
*               size data length
*@return    :   1: succeed
*               negative: failed(error code)
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_write(signed char* pbuf, int size);

/**
***************************************************************************
*@brief     :   Read chunk data from a ssl server
*@param[in] :   pbuf data buffer
*               size data length
*@return    :   >1: succeed(byte count of the read data)
*               negative: failed(error code)
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_read(signed char* pbuf, int size);

/**
***************************************************************************
*@brief     :   select the fd to listen
*@param[in] :   fd fd set to listen
*@return    :   0: succeed
*               other: failed
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_select(int* fd);

/**
***************************************************************************
*@brief     :   Close the fd that create by qca_ssl_socket()
*@param[in] :   fd the fd that create by qca_ssl_socket()
*@return    :   A_OK: succeed
*               A_ERROR: failed
*@warning   :
*@see       :
***************************************************************************
*/
int qca_ssl_close(int* fd);

#endif
