

/* Includes ------------------------------------------------------------------*/

#include "ltm3210.h"


#define UDP_SERVER_STACK_SIZE			1024	
#define UDP_SERVER_BUF_SIZE				1024

unsigned int udp_fd = -1;
struct sockaddr_in fromAddr;

void qca_udp_server_send(char *dat, int length)
{
	int ret;

	if ((udp_fd != -1) && (dat != NULL))
	{
		qcom_sendto(udp_fd, dat, length, 0, \
				(struct sockaddr *) &fromAddr, sizeof (fromAddr));
	}
}

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @fn static void qca_udp_server_handle(void)
* @brief udp server handle
* @param none
* @return none
*/
static void qca_udp_server_handle(void)
{
	unsigned int	recvBytes = 0;
//	unsigned int	sendBytes = 0;
	int				fromSize = 0;

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

		qca_handle_cmd(UDP_Server_Channel, recvBuff, recvBytes);
	}
}


/**
* @fn static void qca_udp_server_task(unsigned int arg)
* @brief udp sever task
* @param arg param
* @return none
*/
static void qca_udp_server_task(unsigned int arg)
{
	unsigned int		ret = 0;
	struct sockaddr_in	localAddr;
	t_save_param_type	*pParam = &t_save_param;

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
	localAddr.sin_port = htons(pParam->udp_server.port);

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
	qca_task_exit();
}



/**
* @fn void qca_udp_server(void)
* @brief udp server test
* @param none
* @return none
*/
void qca_udp_server(void)
{
	qca_task_start(qca_udp_server_task, 0, UDP_SERVER_STACK_SIZE);
}


