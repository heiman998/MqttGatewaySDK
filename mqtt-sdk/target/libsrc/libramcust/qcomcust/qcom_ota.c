/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc..
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */
//#if defined(ENABLE_QCOM_API_OTA_ENABLE)
#include "qcom_common.h"
#include "misc_cdr.h"
#include "qcom_internal.h"
#include "qcom_network.h"
#include "qcom/socket_api.h"
#include "qcom/qcom_ota_api.h"
#include <qcom/select_api.h>


/* OTA daemon buffer */
unsigned char *qcom_ota_msg_buf;
unsigned char *qcom_ota_msg_checksum;
static int qcom_ota_deamon_stop;
static int qcom_ota_daemon_port;
static int qcom_ota_initted;
unsigned int   qcom_ota_listen_port_default=6666;
unsigned short qcom_ota_max_tftp_retry_times=3;

/* ota monitor listening socket */
int socket_serv;

void qcom_ota_request_daemon(A_UINT32 device_id, A_UINT32 *resp_code,A_UINT32 *length)
{
    qcom_ota_daemon_port=0;
    int ret = 0;
    struct sockaddr_in srv_addr;
    int socket_client = -1;
    struct sockaddr_in client_addr;
    int len = 0;
    q_fd_set fd_sockSet;
    int fd_act=0;
    int nSend=0, nRecv=0;
    QCOM_OTA_MSG_HDR_t *qcom_ota_hdr;
    unsigned char* pkt;
    QCOM_OTA_MSG_REQUEST_t *qcom_ota_request;
    QCOM_OTA_MSG_RESPONSE_t *qcom_ota_response;
    struct timeval timeout;
    int serverip;
    A_CHAR file_name[QCOM_OTA_MAX_FILENAME_LEN];
    int client_ip_addr;
    int client_l4_port;
    /* setup socket */
    socket_serv = qcom_socket(AF_INET, SOCK_STREAM, 0);
    if (socket_serv < 0) {
        return;
    }
    qcom_ota_daemon_port = qcom_ota_listen_port_default;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(qcom_ota_daemon_port);
    srv_addr.sin_family = AF_INET;

    /* bind the socket */
    ret = qcom_bind(socket_serv, (struct sockaddr *) &srv_addr, sizeof (struct sockaddr_in));
    if (ret < 0) {
        //A_PRINTF("%s:qcom_bind fail, ret = %d!\n",__func__, ret);
        goto exit;
    }

    /* start listening */
    ret = qcom_listen(socket_serv, 10);
    if (ret < 0) {
#if 0 /* comment below 2 lines out due to qcom_listen is changed to non-blocking mode, and may return -1 even when operation is successful */
        OTA_D_A_PRINTF("%s:qcom_listen fail, ret = %d!\n",__func__, ret);
        goto exit;
#endif
    }
    
    /* wait for somebody to connect me */
    for (;;) {
#if 1 /* add below lines due to socket API implementation change */
        while(1){
            FD_ZERO(&fd_sockSet);
            FD_SET(socket_serv, &fd_sockSet);
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            fd_act = qcom_select(socket_serv + 1, &fd_sockSet, NULL, NULL, &timeout);
            if (fd_act != 0) {
                break;
            }
        }
#endif
        socket_client = qcom_accept(socket_serv, (struct sockaddr *) &client_addr, &len);
        if (socket_client < 0) {
            if (qcom_ota_deamon_stop) {
                goto exit;
            }
            else {
                A_PRINTF("%s:qcom_accept fail, ret = %d!\n",__func__, socket_client);
                continue;
            }
        }
        client_ip_addr = ntohl(client_addr.sin_addr.s_addr);
        client_l4_port = ntohs(client_addr.sin_port);
        A_PRINTF("Accept connection from IP %x, Port %d\n", client_ip_addr, client_l4_port);
        while (1) {

            /* init fd_set */
            FD_ZERO(&fd_sockSet);
            FD_SET(socket_client, &fd_sockSet);
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            /* wait for input */

            fd_act = qcom_select(socket_client + 1, &fd_sockSet, NULL, NULL, &timeout);
            if (fd_act == 0) {
                //A_PRINTF("%s:qcom_select() timeout\n",__func__);
            }
            else {
		  int isset = FD_ISSET(socket_client, &fd_sockSet);
                if (isset>0) {
                    nRecv = qcom_recv(socket_client, (char *)qcom_ota_msg_buf, QCOM_OTA_MSG_BUF_SIZE, 0);
                    if (nRecv < 0) {
                        //A_PRINTF("%s:qcom_recv() error, ret = %d\n",__func__, nRecv);
                        continue;
                    }

                    /* handle OTA request message */
                    pkt = qcom_ota_msg_buf;

                    /* sanity check: QCOM OTA protocol ID */
                    if (pkt[0] != QCOM_OTA_PROTOCOL_ID) {
                        ret = QCOM_OTA_ERR_UNKOWN_MSG;
                        goto sendRsp;
                    }

                    qcom_ota_hdr = (QCOM_OTA_MSG_HDR_t *) pkt;

                    /* dispatch according to opcode */
                    switch (qcom_ota_hdr->opcode) {
                        case QCOM_OTA_REQUEST:
                            A_PRINTF("received QCOM_OTA_REQUEST\n");
                            qcom_ota_request = (QCOM_OTA_MSG_REQUEST_t *) qcom_ota_hdr->payload;
                            A_MEMCPY(&serverip, &(qcom_ota_request->server_ip), sizeof (int));
                            A_MEMSET(file_name,'\0',sizeof(file_name));
                            A_MEMCPY(&file_name, &(qcom_ota_request->file_name), strlen (qcom_ota_request->file_name));

                            qcom_ota_upgrade(device_id,serverip,file_name,qcom_ota_request->mode,qcom_ota_request->preserve_last,qcom_ota_request->protocol,resp_code,length);
                            break;
                        default:
                            ret = QCOM_OTA_ERR_UNKOWN_MSG;
                            break;
                    }


sendRsp:
                    /* prepare reponse message */
                    memset(qcom_ota_msg_buf, 0, QCOM_OTA_MSG_BUF_SIZE);

                    qcom_ota_hdr = (QCOM_OTA_MSG_HDR_t *)qcom_ota_msg_buf;
                    qcom_ota_hdr->qcom_code = QCOM_OTA_PROTOCOL_ID;
                    qcom_ota_hdr->opcode = QCOM_OTA_RESPONSE;
                    qcom_ota_hdr->length = sizeof (QCOM_OTA_MSG_RESPONSE_t);
                    qcom_ota_response = (QCOM_OTA_MSG_RESPONSE_t *) qcom_ota_hdr->payload;
                    qcom_ota_response->response_code = ret;

                    /* send response msg to remote controller */
                    nSend =
                        qcom_send(socket_client, (char *)qcom_ota_msg_buf,
                                sizeof (QCOM_OTA_MSG_HDR_t) + qcom_ota_hdr->length, 0);
                    if (nSend < 0) {
                        //A_PRINTF("%s:qcom_send() error, ret = %d\n",__func__, nSend);
                        continue;
                    }
                    break;
                }
                else if (isset<0){
                    break;
                }
            }
        }

        qcom_socket_close(socket_client);
    }
exit:
    A_PRINTF("qcom OTA server is turning down\n");
    FD_ZERO(&fd_sockSet); 
    return;
}

    int
qcom_ota_daemon_start(A_UINT32 device_id,A_UINT32 *resp_code,A_UINT32 *length)
{
    
    /*Dynamically allocate memory*/
    if(!qcom_ota_initted){
        qcom_ota_msg_buf=(unsigned char*)mem_alloc(QCOM_OTA_MSG_BUF_SIZE);
        qcom_ota_msg_checksum=(unsigned char*)mem_alloc(MD5_CHECKSUM_LEN);
        qcom_ota_initted=1; 
    }

    if(qcom_ota_msg_buf==NULL || qcom_ota_msg_checksum==NULL)
    {
        //A_PRINTF("%s:Insufficient Memory\n",__func__);
        mem_free(qcom_ota_msg_buf);
        mem_free(qcom_ota_msg_checksum); 
        qcom_ota_initted=0;
        return QCOM_OTA_ERR_INSUFFICIENT_MEMORY ;      
    }

    qcom_ota_request_daemon(device_id,resp_code,length);
    return 0;
}

    int
qcom_ota_daemon_stop(void)
{
    qcom_ota_deamon_stop = 1;
    qcom_socket_close(socket_serv);
    return 0;
}

//#endif

