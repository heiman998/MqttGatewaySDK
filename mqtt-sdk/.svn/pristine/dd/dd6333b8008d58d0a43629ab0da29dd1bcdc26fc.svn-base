
/* Includes ------------------------------------------------------------------*/

#include "ltm3210.h"


/* Private define ------------------------------------------------------------*/

/**
* @brief: ��ջ
*/
#define TCP_SERVER_STACK_SIZE		1024
#define	TCP_SERVER_BUFFER_SIZE		1024



/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int  tcps_fd;

int clientFd = -1;


/* Private functions ---------------------------------------------------------*/
/**
* @fn void qca_tcp_server_send(char *dat, int length)
* @brief tcp server send data
* @param dat data buf
* @param length data length
* @return none
*/
void qca_tcp_server_send(char *dat, int length)
{
	int ret;

	if ((clientFd != -1) && (dat != NULL))
	{
		ret = qcom_send(clientFd, dat, length, 0);
		if (ret <= 0)
		{

		}
	}
}


/**
* @fn static void qca_tcp_server_handle(void)
* @brief tcp server handle
* @param none
* @return none
*/
static void qca_tcp_server_handle(void)
{
	int 				recvBytes;
//	int					sendBytes;
	int					fromSize = 0;
	struct sockaddr_in	fromAddr;
	unsigned char		*recvBuff = mem_alloc(TCP_SERVER_BUFFER_SIZE);
	q_fd_set			sockSet;
	struct timeval		tmo;
	unsigned int		fdAct = 0;
	
	clientFd = tcps_fd;

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
				qca_handle_cmd(TCP_Server_Channel, recvBuff, recvBytes);
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
	t_save_param_type	*pParam = &t_save_param;

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
	localAddr.sin_port = htons(pParam->tcp_server.port);
	
	ret = qcom_bind(tcps_fd, (struct sockaddr *) &localAddr, sizeof (struct sockaddr_in));
	if (ret < 0) 
	{
		printf("bind failed\n");
		goto QUIT;
	}

	//== listen one socke
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

	qca_thread_msleep(2000);
	goto start;
	//qcom_task_exit();
}


/**
* @fn void qca_tcp_server(void)
* @brief tcp server test
* @param none
* @return none
*/
void qca_tcp_server(void)
{
	qca_task_start(qca_tcp_server_task, 0, TCP_SERVER_STACK_SIZE);
}

