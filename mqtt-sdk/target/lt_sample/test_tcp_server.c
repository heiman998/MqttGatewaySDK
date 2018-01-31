
/* Includes ------------------------------------------------------------------*/

#include "ltm3210.h"


/* Private define ------------------------------------------------------------*/
/**
* @brief: server ip port
*/
#define TCP_SERVER_PORT				8848

/**
* @brief: ��ջ
*/
#define TCP_SERVER_STACK_SIZE		1024
#define	TCP_SERVER_BUFFER_SIZE		1024



/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int  tcps_fd;


/**
* @fn void qca_tcp_server_handle(void)
* @brief tcp server handle
* @param none
* @return none
*/
void qca_tcp_server_handle(void)
{
	int 				recvBytes;
	int					sendBytes;
	int					fromSize = 0;
	struct sockaddr_in	fromAddr;
	unsigned char		*recvBuff = mem_alloc(TCP_SERVER_BUFFER_SIZE);
	q_fd_set			sockSet;
	struct timeval		tmo;
	unsigned int		fdAct = 0;
	int 				clientFd = tcps_fd;

	while (1)
	{
		memset(&fromAddr, 0, sizeof (struct sockaddr_in));
		fromSize = sizeof (struct sockaddr_in);

		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(clientFd, &sockSet);

		//== select time
		tmo.tv_sec = 3;
		tmo.tv_usec = 0;

		fdAct = qcom_select(clientFd + 1, &sockSet, NULL, NULL, &tmo);
		if (fdAct < 0)
		{
			printf("listen socket selece failed %d.\n", fdAct);
			break;
		}
		else if (fdAct == 0)
		{
			//			printf("listen socket select continue\n");
			continue;
		}

		printf("-->Mem : %d.\n", qcom_mem_heap_get_free_size());

		fdAct = FD_ISSET(tcps_fd, &sockSet);
		if (fdAct == 1)       
		{
			//== 5. accept a new socket connect
			fromSize = sizeof (struct sockaddr_in);
			clientFd = qcom_accept(tcps_fd, (struct sockaddr *) &fromAddr, &fromSize);
			if (clientFd < 0)
			{
				printf("Failed to accept socket %d.\n", clientFd);
				continue;
			}

			printf("Receiving from 0x%x Remote port:%d \r\n",
				(int)ntohl(fromAddr.sin_addr.s_addr), ntohs(fromAddr.sin_port));
		}
		else
		{
			fdAct = FD_ISSET(clientFd, &sockSet);
			if (fdAct)       
			{
				//== 6. receive data
				memset(recvBuff, 0, TCP_SERVER_BUFFER_SIZE);
				recvBytes = qcom_recv(clientFd, (char *)recvBuff, TCP_SERVER_BUFFER_SIZE, 0);
				if (recvBytes <= 0)
				{
					qcom_socket_close(clientFd);
					clientFd = tcps_fd;
				}

				printf("recv %d bytes: %s\n", recvBytes, recvBuff);

				//== send data
				sendBytes = qcom_send(clientFd, (char *)recvBuff, recvBytes, 0);
				if (sendBytes < 0)
				{
					printf("send error\n");
				}

			}
		}

	}

	mem_free(recvBuff);
	//== close listen socket
	qcom_socket_close(tcps_fd);

}

/**
* @fn void qca_tcp_server_task(unsigned int arg)
* @brief tcp server task
* @param arg task input
* @return none
*/
void qca_tcp_server_task(unsigned int arg)
{
	int	ret = 0;
	struct sockaddr_in	localAddr;

start:
	//== close save power
	qcom_power_set_mode(0, MAX_PERF_POWER);


	tcps_fd = qcom_socket(PF_INET, SOCK_STREAM, 0);
	if (tcps_fd < 0)
	{
		printf("open tcp server socket error.\r\n");
		goto QUIT;
	}

	//== ipv4 bind 
	memset(&localAddr, 0, sizeof (struct sockaddr_in));
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(TCP_SERVER_PORT);

	ret = qcom_bind(tcps_fd, (struct sockaddr *) &localAddr, sizeof (struct sockaddr_in));
	if (ret < 0) 
	{
		printf("bind failed\n");
		goto QUIT;
	}

	//== listen one socket
	ret = qcom_listen(tcps_fd, 1);
	if (ret < 0)
	{
		printf("listen failed\n");
		goto QUIT;
	}

	//== packet handle
	qca_tcp_server_handle();

QUIT:
	qcom_socket_close(tcps_fd);

	qcom_thread_msleep(2000);
	goto start;
	//qcom_task_exit();
}


/**
* @fn void qca_tcp_server_test(void)
* @brief tcp server test
* @param none
* @return none
*/
void qca_tcp_server_test(void)
{
	qca_task_start(qca_tcp_server_task, 0, TCP_SERVER_STACK_SIZE);
}

