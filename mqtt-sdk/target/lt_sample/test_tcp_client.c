

/* Includes ------------------------------------------------------------------*/

#include "ltm3210.h"


/* Private define ------------------------------------------------------------*/
#define TCP_CLIENT_STACK_SIZE		1024		
#define TCP_CLIENT_BUF_SIZE			1024

#define TCP_SERVER_PORT				80
#define TCP_SERVER_ADDR				"192.168.0.41"

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int tcpc_fd;

static unsigned int qca_inet_addr(char *str)
{
	unsigned int ipaddr;
	unsigned int data[4];
	unsigned int ret;

	ret = sscanf(str, "%3d.%3d.%3d.%3d", data, data + 1, data + 2, data + 3);

	if (ret < 0) {
		return 0;
	}
	else {
		ipaddr = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
	}
	return ipaddr;
}

/* Private functions ---------------------------------------------------------*/

/**
* @fn void qca_tcp_client_send(char *dat, int length)
* @brief tcp client send data
* @param dat data buf
* @param length data length
* @return none
*/
void qca_tcp_client_send(char *dat, int length)
{
	int ret;

	if ((tcpc_fd != -1) && (dat != NULL))
	{
		ret = qcom_send(tcpc_fd, dat, length, 0);
		if (ret <= 0)
		{

		}
	}
}

/**
* @fn void qca_tcp_client_handle(void)
* @brief socket handle
* @param none
* @return none
*/
void qca_tcp_client_handle(void)
{
	int				recvBytes;
	int				sendBytes;
 	unsigned char	*recvBuff = mem_alloc(TCP_CLIENT_BUF_SIZE);
	int				fdAct;
	static q_fd_set sockSet;
	struct timeval	tmo;

	while (1)
	{
		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(tcpc_fd, &sockSet);
		if (tcpc_fd <= 0)
		{
			printf("socket closed\n");
			break;
		}

		//== select time
		tmo.tv_sec = 3;
		tmo.tv_usec = 0;

		//== select
		fdAct = qcom_select(tcpc_fd + 1, &sockSet, NULL, NULL, &tmo);
		if (fdAct == 0)
		{
			continue;
		}
		else if (fdAct == -1)
		{
			printf("selelt failed\n");
			break;
		}

		//== check event
		fdAct = FD_ISSET(tcpc_fd, &sockSet);
		if (fdAct == 0)
		{
			printf("FD_ISSET countinue\n");
			continue;
		}
		else if (fdAct == -1)
		{
			printf("FD_ISSET failed\n");
			break;
		}

		memset(recvBuff, 0, TCP_CLIENT_BUF_SIZE);
		//== receive
		recvBytes = qcom_recv(tcpc_fd, (char*)recvBuff, TCP_CLIENT_BUF_SIZE, 0);
		if (recvBytes <= 0) 
		{
			printf("socket close.\n");
			break;
		}
		
		printf("receive %d byte: %s\n", recvBytes, recvBuff);
		sendBytes = qcom_send(tcpc_fd, (char *)recvBuff, recvBytes, 0);

		printf("send %d byte\n", sendBytes);

	}
	printf("close socket\n");
	qcom_socket_close(tcpc_fd);
}

/**
* @fn void qca_tcp_client_task(unsigned int arg)
* @brief tcp client task
* @param arg task input
* @return 
*/
void qca_tcp_client_task(unsigned int arg)
{
	struct sockaddr_in	remoteAddr;
	int					ret = 0;
	int server_ip = 0;;

	while (1)
	{

		qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);			// �˳��͹���ģʽ

		tcpc_fd = qcom_socket(PF_INET, SOCK_STREAM, 0);
		if (tcpc_fd < 0)
		{
			printf("open tcp client socket error.\r\n");
			goto QUIT;
		}
		
		server_ip = qca_inet_addr(TCP_SERVER_ADDR);
		A_PRINTF("Server Addr:\n");
		printf("%d.%d.%d.%d\n", 
		(server_ip) & 0xFF, (server_ip) >> 8 & 0xFF,
		(server_ip) >> 16 & 0xFF, (server_ip) >> 24 & 0xFF);

		//== ipv4 connect 
		memset(&remoteAddr, 0, sizeof (struct sockaddr_in));
		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_addr.s_addr = htonl(server_ip);
		remoteAddr.sin_port = htons(TCP_SERVER_PORT);

		ret = qcom_connect(tcpc_fd, (struct sockaddr *) &remoteAddr, sizeof (struct sockaddr_in));
		if (ret < 0)
		{
			printf("Connect Failed\r\n");
			qcom_socket_close(tcpc_fd);
			goto QUIT;
		}

		printf("Connect %lu.%lu.%lu.%lu OK\r\n",
			(remoteAddr.sin_addr.s_addr) & 0xFF, (remoteAddr.sin_addr.s_addr) >> 8 & 0xFF,
			(remoteAddr.sin_addr.s_addr) >> 16 & 0xFF, (remoteAddr.sin_addr.s_addr) >> 24 & 0xFF);


		//== packet handle
		qca_tcp_client_handle();

QUIT:
		qcom_thread_msleep(5000);
	}

	qca_task_exit();
}



/**
* @fn void qca_tcp_client_test(void)
* @brief tcp client test
* @param none
* @return none
*/
void qca_tcp_client_test(void)
{
	qca_task_start(qca_tcp_client_task, 0, TCP_CLIENT_STACK_SIZE);
}

