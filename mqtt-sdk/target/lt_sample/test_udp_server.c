

/* Includes ------------------------------------------------------------------*/

#include "ltm3210.h"

/**
* @brief: UDP server ip port
*/
#define UDP_SERVER_PORT					8849

#define UDP_SERVER_STACK_SIZE			1024	
#define UDP_SERVER_BUF_SIZE				1024

static unsigned int udp_fd;

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @fn void qca_udp_server_handle(void)
* @brief udp server handle
* @param none
* @return none
*/
void qca_udp_server_handle(void)
{
	unsigned int	recvBytes = 0;
	unsigned int	sendBytes = 0;
	int				fromSize = 0;
	struct sockaddr_in fromAddr;

	unsigned char *recvBuff = mem_alloc(UDP_SERVER_BUF_SIZE);

	q_fd_set sockSet;
	struct timeval tmo;
	unsigned int fdAct = 0;


	while (1)
	{
		//== Prepare IP address & port 

		memset(&fromAddr, 0, sizeof (struct sockaddr_in));
		fromSize = sizeof (struct sockaddr_in);


		//== Init fd_set
		FD_ZERO(&sockSet);
		FD_SET(udp_fd, &sockSet);

		//== select time
		tmo.tv_sec = 3;
		tmo.tv_usec = 0;

		//== select
		fdAct = qcom_select(udp_fd + 1, &sockSet, NULL, NULL, &tmo);
		if (0 == fdAct)
		{
			continue;
		}

		if (FD_ISSET(udp_fd, &sockSet) == 0)
		{
			continue;
		}

		//== clear buffer
		memset(recvBuff, 0, UDP_SERVER_BUF_SIZE);

		//== receive
		
		recvBytes = qcom_recvfrom(udp_fd, (char*)recvBuff, UDP_SERVER_BUF_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize);
		if (recvBytes <= 0)
		{
			printf("socket close.\n");
			break;
		}

		printf("receive %d byte: %s\n", recvBytes, recvBuff);

		sendBytes = qcom_sendto(udp_fd, (char *)recvBuff, recvBytes, 0, (struct sockaddr*)&fromAddr, sizeof (struct sockaddr_in));
		printf("send %d byte\n", sendBytes);
	}
}


/**
* @fn void qca_udp_server_task(unsigned int arg)
* @brief udp sever task
* @param arg param
* @return none
*/
void qca_udp_server_task(unsigned int arg)
{
	unsigned int		ret = 0;
	struct sockaddr_in	localAddr;

start:

	qcom_power_set_mode(0, MAX_PERF_POWER);		// �˳��͹���ģʽ

	udp_fd = qcom_socket(PF_INET, SOCK_DGRAM, 0);
	if (udp_fd < 0)
	{
		printf("create udp server socket error.\r\n");
		goto QUIT;
	}


	/* Connect Socket */
	memset(&localAddr, 0, sizeof (struct sockaddr_in));
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_family = AF_INET;

	
	localAddr.sin_port = htons(UDP_SERVER_PORT);


	ret = qcom_bind(udp_fd, (struct sockaddr *) &localAddr, sizeof (struct sockaddr_in));
	if (ret < 0)
	{
		qcom_socket_close(udp_fd);
		goto QUIT;
	}

	/* Packet Handle */
	qca_udp_server_handle();

QUIT:
	qcom_thread_msleep(2000);
	goto start;
	//qcom_task_exit();
}



/**
* @fn void qca_udp_server_test(void)
* @brief udp server test
* @param none
* @return none
*/
void qca_udp_server_test(void)
{
	qca_task_start(qca_udp_server_task, 0, UDP_SERVER_STACK_SIZE);
}


