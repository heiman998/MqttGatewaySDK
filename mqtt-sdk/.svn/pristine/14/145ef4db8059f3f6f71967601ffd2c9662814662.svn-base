#ifndef _QCA_HTTP_CLIENT_H_
#define _QCA_HTTP_CLIENT_H_

/** 
* @file 	qca_http_client.h	
* @brief	http client
* @author	
* @version	V0.1.1
* @date		2017.5.15
* @details	
* @copyright 
* @note		HTTP client
*/

/**
* @enum typedef http_client_connection_t
* @brief header connection feild.
*/
typedef enum {
    KEEP_ALIVE = 0,
    CLOSE = 1,
}http_client_connection_t;

/**
* @struct typedef http_client_header_t
* @brief header struct that will use when POST or GET.
*/
typedef struct {
    char *content_type; // default is application/octet-stream
    http_client_connection_t connection; // default is keep-alive
}http_client_header_t;

/**
* @fn typedef void (*get_callback_t)(unsigned char *buf, unsigned int len)
* @brief response data get callback function, user can get the response data in this function .
* @param buf buffer store response data.
* @param len length of data.
*/
typedef void (*get_callback_t)(unsigned char *buf, unsigned int len);

/**
* @fn typedef void (*post_callback_t)(unsigned char **send_buf, unsigned int *buf_len)
* @brief POST callback function, user should give the data that want to post in this function.
* @param send_buf a pointer to pointer to the send buffer.
* @param buf_len a pointer to get the send buffer length.
* in this function, user should define a buf, just like
* >    char buf[64]
* then define the function:
* > void post_cb(unsigned char **send_buf, unsigned int *buf_len);
* and then, in the callback funtion, point send_buf to the buf define above:
* >    *send_buf = buf
* and give the buf length:
* >    *buf_len = 10;
*/
typedef void (*post_callback_t)(unsigned char **send_buf, unsigned int *buf_len);


/**
* @fn int http_client_startup(unsigned char *domain, unsigned short port)
* @brief Start a http client, connect to domain:port.
* @param domain host name or host ip of the http server.
* @param port port number of the http server.
* @return 0 success, -1 failed
*/
int http_client_startup(unsigned char *domain, unsigned short port);

/**
* @fn int http_client_post(char *url, unsigned int content_length, http_client_header_t header, post_callback_t post_cb, get_callback_t get_cb)
* @brief Post data to the url.
* @param url POST url.
* @param content_length content length.
* @param header post request header info
* @param post_cb post callback function, user should give the data that want to post in this function
* @param get_cb response data get callback function, user can get the response data in this function
* @return 0 success, 
*         -1 failed
*         -2 get response failed(select fd error)
*         -3 get response failed(read timeout)
*         -4 get response failed(socket close by remote)
*         > 1, HTTP status code(200 OK will return 0), see HTTP status code for detail, for example:
*           404   Not Found
*           500   Internal Server Error
*/
int http_client_post(char *url, unsigned int content_length
                        , http_client_header_t *header, post_callback_t post_cb, get_callback_t respond_cb);

/**
* @fn int http_client_put(char *url, unsigned int content_length, http_client_header_t header, post_callback_t post_cb, get_callback_t get_cb)
* @brief http client PUT method.
* @param url PUT url.
* @param content_length content length.
* @param header put request header info
* @param post_cb put callback function, user should give the data that want to put in this function
* @param get_cb response data get callback function, user can get the response data in this function
* @return 0 success, 
*         -1 failed
*         -2 get response failed(select fd error)
*         -3 get response failed(read timeout)
*         -4 get response failed(socket close by remote)
*         > 1, HTTP status code(200 OK will return 0), see HTTP status code for detail, for example:
*           404   Not Found
*           500   Internal Server Error
*/
int http_client_put(char *url, unsigned int content_length
                        , http_client_header_t *header, post_callback_t post_cb, get_callback_t respond_cb);

/**
* @fn int http_client_get(char *url, http_client_header_t header, get_callback_t get_cb)
* @brief get data or page from the url.
* @param url GET url.
* @param header get request header info
* @param get_cb response data get callback function, user can get the response data in this function.
* @return 0 success, 
*         -1 failed
*         -2 get response failed(select fd error)
*         -3 get response failed(read timeout)
*         -4 get response failed(socket close by remote)
*         > 1, HTTP status code(200 OK will return 0), see HTTP status code for detail, for example:
*           404   Not Found
*           500   Internal Server Error
*/
int http_client_get(char *url, http_client_header_t *header, get_callback_t get_cb);

/**
* @fn int http_client_delete(char *url, http_client_header_t header, get_callback_t get_cb)
* @brief http client DELETE method.
* @param url DELETE url.
* @param header delete request header info
* @param get_cb response data get callback function, user can get the response data in this function.
* @return 0 success, 
*         -1 failed
*         -2 get response failed(select fd error)
*         -3 get response failed(read timeout)
*         -4 get response failed(socket close by remote)
*         > 1, HTTP status code(200 OK will return 0), see HTTP status code for detail, for example:
*           404   Not Found
*           500   Internal Server Error
*/
int http_client_delete(char *url, http_client_header_t *header, get_callback_t get_cb);

/**
* @fn int http_client_stop(void)
* @brief Stop the http client, free the memory.
* @param none.
* @return 0 success, -1 failed
*/
int http_client_stop(void);


/* HTTP Status Code
100   Continue
101   Switching Protocols

200   OK
201   Created
202   Accepted
203   Non-Authoritative Information
204   No Content
205   Reset Content
206   Partial Content

300   Multiple Choices
301   Moved Permanently
302   Found
303   See Other
304   Not Modified
305   Use Proxy
306   Unused
307   Temporary Redirect

400   Bad Request
401   Unauthorized
402   Payment Required
403   Forbidden
404   Not Found
405   Method Not Allowed
406   Not Acceptable
407   Proxy Authentication Required
408   Request Timeout
409   Conflict
410   Gone
411   Length Required
412   Precondition Failed
413   Request Entity Too Large
414   Request-URI Too Long
415   Unsupported Media Type
416   Requested Range Not Satisfiable
417   Expectation Failed

500   Internal Server Error
501   Not Implemented
502   Bad Gateway
503   Service Unavailable
504   Gateway Timeout
505   HTTP Version Not Supported
*/


#endif
