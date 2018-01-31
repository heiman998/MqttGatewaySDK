/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef LIBIP_H_
#define LIBIP_H_

typedef struct datachunk {
    int minsize;
    int maxsize;
    int count;
    int done;
} datachunk_t;


int ipInit(void);
int ipConnectPeer(const char* host, int port, int retries);
int ipListenSocket(int port);
int ipAcceptConnection(int serversd);
int ipChunksInit(char type, const char *csvChunkStr);
int ipRecv(int sd, void *buf, int size, int flags);
void ipSetTcpOption(int sd, int option, int enable);
int ipGetTcpOption(int sd, int option);

#endif // LIBIP_H_
