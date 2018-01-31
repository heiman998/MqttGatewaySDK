
#ifndef _QCA_USER_CORE_H_
#define _QCA_USER_CORE_H_



/* Typedef -------------------------------------------------------------------*/


/**
* @brief: enable/disable
*/
typedef enum
{
	DISABLE = 0,
	ENABLE = 1
}tIpv6Enable_Type;


/* Define --------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/

typedef struct
{
	A_UINT32 ipAddress;				// ip address
	IP6_ADDR_T ip6Address;			// IP v6 address
	A_UINT32 mcAddress;				// multicast address for traffic receive only
	IP6_ADDR_T mcastIp6;
	IP6_ADDR_T localIp6;
	A_UINT32 port;					// port
	A_UINT32 pktSize;				// packet size
	A_UINT8* pktBuff;				// packet buffer
	A_UINT32 seconds;				// time seconds
	A_UINT32 numOfPkts;				// number of packets
	A_UINT32 delay;					// delay in ms
	A_UINT32 local_ipAddress;		// for raw tx
	A_UINT32 ip_hdr_inc;			// for raw tx - optional
	A_UINT32 rawmode;				// for raw rx ETH/IP mode
	A_UINT8 mcast_enabled;
} STREAM_PARA_t;

typedef struct
{
	A_INT32  socketLocal;
	A_INT32  clientFd;
	A_INT32  max_fd;
	q_fd_set sockSet;
	STREAM_PARA_t param;
	void *pfd_set;
} STREAM_CXT_t;

/* Export variable -----------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
* @brief: check null pointer
*/
#define QCA_PTR_NULL_CHK(para)		do {if (NULL == (para)) return;} while(0)

/**
* @brief: correction time
*/
#define qca_thread_msleep(time)		qcom_thread_msleep(time*1024/1000)
#define qca_time_ms()				(time_ms()*1000/1024)

/**
***************************************************************************
*@brief		:	converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure
*@param[in]	:	A NULL-terminated character string representing a number expressed in the Internet standard ".'' (dotted) notation
*@return	:	An unsigned long value containing a suitable binary representation of the Internet address given
*@warning	:
*@see		:
***************************************************************************
*/
unsigned int _inet_addr(char *str);

/**
***************************************************************************
*@brief		:	converts an (Ipv4) Internet network address into an ASCII string in Internet standard dotted-decimal format
*@param[in]	:	an in_addr structure that represents an internet host address
*@return	:	if no error occurs, inet_ntoa returns a character pointer to a static buffer containing the text address in standard ".'' notation. Otherwise, it returns NULL
*@warning	:
*@see		:
***************************************************************************
*/
char * _inet_ntoa(unsigned int ip);

/**
***************************************************************************
*@brief		:	set ipv6 enable status
*@param[in]	:	state	ENABLE/DISABLE
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_set_ipv6_enable(tIpv6Enable_Type state);

/**
***************************************************************************
*@brief		:	get ipv6 enable status
*@param[in]	:	state	ENABLE/DISABLE
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
tIpv6Enable_Type qca_get_ipve6_enable(void);

/**
***************************************************************************
*@brief		:	socket close
*@param[in]	:	pCxtPara
*@return	:	none
*@warning	:
*@see		:
***************************************************************************
*/
void qca_socket_close(STREAM_CXT_t * pCxtPara);




#endif
