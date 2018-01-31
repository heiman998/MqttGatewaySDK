/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#define __WIN32__ 1
//#define __LINUX__ 1

#ifdef __WIN32__
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#endif

#ifdef __LINUX__
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#endif


#define	SA	struct sockaddr



#define QCA_OTA_PROTOCOL_ID 0xFF
#define QCA_OTA_MSG_BUF_SIZE 256
#define QCA_OTA_MAX_FILENAME_LEN 64

#define QCA_OTA_DEMO_PORT 6666

#define MD5_CHECKSUM_LEN 16

typedef enum{
	QCA_OTA_OK = 0,
	QCA_OTA_ERR_UNKOWN_MSG = 1000,
	QCA_OTA_ERR_IMAGE_NOT_FOUND = 1001,
	QCA_OTA_ERR_IMAGE_DOWNLOAD_FAIL = 1002,
	QCA_OTA_ERR_IMAGE_CHECKSUM_INCORRENT = 1003,
	QCA_OTA_ERR_SERVER_RSP_TIMEOUT = 1004,
	QCA_OTA_ERR_INVALID_FILENAME = 1005,
	QCA_OTA_ERR_UNSUPPORT_PROTOCOL = 1006,
	QCA_OTA_ERR_INVALID_PARTITION_INDEX = 1007,
	QCA_OTA_ERR_IMAGE_HDR_INCORRENT = 1008,	
}QCA_OTA_ERROR_CODE_t;


typedef enum{
	QCA_OTA_REQUEST = 0,
	QCA_OTA_RESPONSE,
}QCA_OTA_MSG_OPCODE_t;


typedef struct{
	char qca_code;
	char opcode;
	unsigned char length;
	char payload[0];
}__attribute__((packed)) QCA_OTA_MSG_HDR_t;

typedef struct{
	int server_ip;
	char file_name[QCA_OTA_MAX_FILENAME_LEN];
	unsigned char protocol;
	char partition_index;
	char ota_msg_checksum[MD5_CHECKSUM_LEN];
}__attribute__((packed)) QCA_OTA_MSG_REQUEST_t;

typedef struct{
	int response_code;
	char ota_image_checksum[MD5_CHECKSUM_LEN];
	char ota_msg_checksum[MD5_CHECKSUM_LEN];
}__attribute__((packed)) QCA_OTA_MSG_RESPONSE_t;


/* OTA daemon buffer */
char demo_ota_msg_rx_buf[QCA_OTA_MSG_BUF_SIZE] = {0};
char demo_ota_msg_tx_buf[QCA_OTA_MSG_BUF_SIZE] = {0};
char demo_ota_msg_checksum[MD5_CHECKSUM_LEN] = {0};
static int demo_ota_deamon_stop = 0;
static int demo_ota_daemon_ip = 0;
static int demo_ota_daemon_port = 0;


/**********************************************************************/
void _demo_packet_printf(unsigned char *data, int len)
{
	int count = 0;
	printf("_packet_printf:0x%p, len = %d\n",data,len);
	for (count = 0; count < len; count++){
		printf("%.2x ", data[count]);
		if (count && (15 == (count %16)))
			printf("\n");
	}
	printf("\n");

}


int main(int argc, char **argv)
{
	int ret;
	#ifdef __WIN32__
	SOCKET sockfd;
	#else
	int	sockfd;
	#endif
	struct sockaddr_in	servaddr;
	unsigned char* pkt;
	int i;
	QCA_OTA_MSG_HDR_t *qca_ota_hdr;
	QCA_OTA_MSG_REQUEST_t *qca_ota_request;
	QCA_OTA_MSG_RESPONSE_t *qca_ota_response;
    struct timeval timeout;
    fd_set fd_sockSet;
    int nSend=0, nRecv=0;
    int fd_act;	
    WSADATA Ws;

	
	
	if (argc != 5)
	{
		printf("usage: otaTrigger <ota-daemon-ip> <ota-server-ip> <image-name> <protocol>\n");

		return -1;
	}

	#ifdef __WIN32__
	/* init windows socket */
	if(WSAStartup(MAKEWORD(2,2),&Ws)!=0){
		printf("Init Windows Socket Failed\n");
	}
	#endif

	/* create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* configure OTA daemon information */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	//servaddr.sin_addr.s_addr = inet_addr("192.168.1.10");
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(QCA_OTA_DEMO_PORT);

	/* connect to OTA request daemon */
	printf("connecting to OTA request daemon\n");
	ret = connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	if(ret < 0)
	{
		printf("Fail: Unable connect to OTA deamon, please make sure the IP of OTA daemon is correct!\n");
		exit(-1);
	}
	printf("connected!\n");

	/* Send OTA request message to OTA request Daemon */
	memset(demo_ota_msg_tx_buf, 0, QCA_OTA_MSG_BUF_SIZE);

	qca_ota_hdr = (QCA_OTA_MSG_HDR_t *)demo_ota_msg_tx_buf;
	qca_ota_hdr->qca_code = QCA_OTA_PROTOCOL_ID;
	qca_ota_hdr->opcode = QCA_OTA_REQUEST;
	qca_ota_hdr->length = sizeof(QCA_OTA_MSG_REQUEST_t);	
	qca_ota_request = (QCA_OTA_MSG_REQUEST_t *)qca_ota_hdr->payload;
	
	qca_ota_request->server_ip = htonl(inet_addr(argv[2]));
	memcpy(qca_ota_request->file_name, argv[3], strlen(argv[3]));
	qca_ota_request->protocol = atoi(argv[4]);
	qca_ota_request->partition_index = -1;
	
	//_demo_packet_printf(demo_ota_msg_tx_buf, sizeof(QCA_OTA_MSG_HDR_t)+ qca_ota_hdr->length);

	ret = send(sockfd, demo_ota_msg_tx_buf, sizeof(QCA_OTA_MSG_HDR_t)+ qca_ota_hdr->length, 0);
	if(ret < 0)
	{
		printf("send error, ret = %d\n", ret);
	}
	
	/* wait for response from OTA request Deamon  */
   	while (1)
   	{
	        /* init fd_set */
	    	FD_ZERO(&fd_sockSet);
	       	FD_SET(sockfd, &fd_sockSet);
	       	timeout.tv_sec = 10;
	       	timeout.tv_usec = 0;
	
	        /* wait for input */
	        fd_act = 0;
	        fd_act = select(sockfd+1, &fd_sockSet, NULL, NULL, &timeout);
	        if(fd_act == 0)
	        {
	        	printf("select() timeout\n");
	        }
	        else 
	        {
	        	if(FD_ISSET(sockfd, &fd_sockSet))
			{
				memset(demo_ota_msg_rx_buf, 0, 100);
	            		nRecv = recv(sockfd, demo_ota_msg_rx_buf, 20, 0);
	                	if(nRecv < 0)
	                	{
					printf("recv error, ret = %d\n", nRecv);
	         			continue;
	                	}
	
				/* DEBUG: dump received OTA request message */
				//_demo_packet_printf(demo_ota_msg_rx_buf, nRecv);
					
				pkt = (unsigned char *)demo_ota_msg_rx_buf;

				/* sanity check: QCA OTA protocol ID */
				if(pkt[0] != QCA_OTA_PROTOCOL_ID)
				{
					printf("error: unknown message\n");
					goto exit;
				}
	
				/* handle OTA request message */
				qca_ota_hdr = (QCA_OTA_MSG_HDR_t *)pkt;
					
				/* todo: calcualte checksum for rx message */
	
	
				/* handle msg according to opcode */				
				qca_ota_response = (QCA_OTA_MSG_RESPONSE_t *)qca_ota_hdr->payload;
	
				
				printf("responsecode = %d\n", qca_ota_response->response_code);
				/* print response code */
				switch(qca_ota_response->response_code)
				{
					case QCA_OTA_OK:
						printf("OTA upgrade finish successfully!\n");
						break;
	
					case QCA_OTA_ERR_IMAGE_NOT_FOUND:
						printf("Fail: image not found!\n");						
						break;
	
					case QCA_OTA_ERR_IMAGE_DOWNLOAD_FAIL:
						printf("Fail: image download fail1, please make sure the file name is correct\n");
						break;
	
					case QCA_OTA_ERR_IMAGE_CHECKSUM_INCORRENT:
						printf("Fail: image checksum incorrect fail1\n");
						break;
							
					case QCA_OTA_ERR_SERVER_RSP_TIMEOUT:
						printf("Fail: OTA server response timeout, please make sure IP of OTA server is correct\n");
						break;
	
					case QCA_OTA_ERR_UNSUPPORT_PROTOCOL:
						printf("Fail: upsupported protocol, only 0(TFTP) is supported!\n");
						break;
	
					case QCA_OTA_ERR_IMAGE_HDR_INCORRENT:
						printf("Fail: image header incorrect!\n");
						break;
	
					case QCA_OTA_ERR_INVALID_PARTITION_INDEX:
						printf("Fail: invalid partition index!\n");
	
					default :
						break;
				}
	
				/* exit */
				goto exit;
	        	}//end if
	        }//end else

   	}//end while
exit:

	return 0;
}


