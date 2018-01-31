/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>

#include "utility.h"
#include "libip.h"

//#define LOGPREFIX "libip: "
#define LOGPREFIX


// ============================================================================
// Globals
// ============================================================================

static int ipRecvChunkCount = 0;
static datachunk_t *ipRecvChunks = NULL;
static int ipSendChunkCount = 0;
static datachunk_t *ipSendChunks = NULL;


// ============================================================================
// Init, Listen and Accept
// ============================================================================

int ipInit(void)
{
    return 0;
}

int ipConnectPeer(const char* host, int port, int retries)
{
    struct sockaddr_in addr;
    int sd;

    logDebug(LOGPREFIX "connect('%s:%d')\n", host, port);

    struct hostent* hostInfo = gethostbyname(host);
    if (!hostInfo) {
        dieError(LOGPREFIX "gethostbyname('%s')\n", host);
        return -2;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ((struct in_addr *) hostInfo->h_addr)->s_addr;
    uint32_t ipAddr = htonl(addr.sin_addr.s_addr);
    logVerb(LOGPREFIX "connecting to %u.%u.%u.%u:%d\n",
        (ipAddr >> 24) & 0xFF, (ipAddr >> 16) & 0xFF, (ipAddr >> 8) & 0xFF, ipAddr & 0xFF, port);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        dieSysError(LOGPREFIX "socket()\n");
        return sd;
    }

    for (int i = 0; i < retries; i++) {
        int res = connect(sd, (struct sockaddr*) &addr, sizeof(addr));
        if (res == 0)
            break;
        if (errno == ECONNREFUSED && retries > 0) {
            logNorm(LOGPREFIX "connection failed, retrying %d of %d...\n", i, retries);
            usleep(500 * 1000);
            continue;
        }
        dieSysError(LOGPREFIX "connect()\n");
        return res;
    }
    logVerb(LOGPREFIX "connected to %u.%u.%u.%u:%d\n",
        (ipAddr >> 24) & 0xFF, (ipAddr >> 16) & 0xFF, (ipAddr >> 8) & 0xFF, ipAddr & 0xFF, port);

    return sd;
}

int ipListenSocket(int port)
{
    int fd;
    int opt = 1;
    struct sockaddr_in sinInterface;

    logDebug(LOGPREFIX "listenSocket(%d)\n", port);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        dieSysError(LOGPREFIX "Failed to create server socket\n");

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        dieSysError(LOGPREFIX "setsock() failed\n");

    sinInterface.sin_family = AF_INET;
    sinInterface.sin_addr.s_addr = INADDR_ANY;
    sinInterface.sin_port = htons(port);
    if (bind(fd, (struct sockaddr*) &sinInterface, sizeof(struct sockaddr_in)) == -1)
        dieSysError(LOGPREFIX "bind() failed\n");

    if (listen(fd, SOMAXCONN) == -1)
        dieSysError(LOGPREFIX "listen() failed\n");

    logNorm(LOGPREFIX "Listening on port %d\n", port);

    return fd;
}

int ipAcceptConnection(int serversd)
{
    struct sockaddr_in sinRemote;
    socklen_t nAddrSize = sizeof(struct sockaddr_in);
    int fd;

    fd = accept(serversd, (struct sockaddr*) &sinRemote, &nAddrSize);
    if (fd == -1) {
        perror(LOGPREFIX "accept failed");
        return -1;
    }

    logNorm(LOGPREFIX "Accepted connection from %s:%d\n", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

    if (0) {
        int opt = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1)
            perror(LOGPREFIX "setsockopt(SO_KEEPALIVE) failed");
    }
    int opts = fcntl(fd, F_GETFL);
    if (opts < 0)
        logSysError("fcntl(F_GETFL)");

    logDebug("Socket %d has blocking = %d\n", fd, opts & O_NONBLOCK);

    return fd;
}

void ipSetTcpOption(int sd, int optname, int optval)
{
    logDebug("%s(%d, %d, %d)\n", __func__, sd, optname, optval);

    int opts = setsockopt(sd, IPPROTO_TCP, optname, &optval, sizeof(optval));
    if (opts < 0)
        logSysError("setsockopt(%d)\n", optname);
}

int ipGetTcpOption(int sd, int option)
{
    logDebug("%s(%d, %d)\n", __func__, sd, option);

    int optval;
    socklen_t optlen = sizeof(optval);

    int res = getsockopt(sd, IPPROTO_TCP, option, &optval, &optlen);
    if (res < 0)
        logSysError("getsockopt(%d)\n", option);

    return optval;
}

#if 0
static void ipSetBlockingMode(int sd, int enableBlocking)
{
    logDebug("ipSetBlockingMode(%d, %d)\n", sd, enableBlocking);

    int opts = fcntl(sd, F_GETFL);
    if (opts < 0)
        logSysError("fcntl(F_GETFL)");

    if (enableBlocking)
        opts = opts | O_NONBLOCK;
    else
        opts = opts & (~O_NONBLOCK);

    if (fcntl(sd, F_SETFL, opts) < 0)
        logSysError("fcntl(F_GETFL)");
}
#endif

// ============================================================================
// Receive
// ============================================================================

/**
 * FIXME: We should allow specifying min and max size of chunks
 * Examples:
 *   1-1418,2000-2114,51,1000*20
 * @param type
 * @param csvChunkStr
 * @return
 */
int ipChunksInit(char type, const char *csvChunkStr)
{
    datachunk_t *pchunks;
    int numChunks;

    logDebug(LOGPREFIX "ipChunksInit(%s)\n", csvChunkStr);

    if (csvChunkStr == NULL) {
        numChunks = 0;
        pchunks = 0;
    }
    else {
        // First count number of chunks
        numChunks = 1;
        for (int i = 0; i < strlen(csvChunkStr); i++) {
            if (csvChunkStr[i] == ',')
                numChunks++;
        }
        pchunks = malloc(numChunks * sizeof(datachunk_t));
        char *endp = (char *) csvChunkStr;
        for (int i = 0; i < numChunks; i++) {
            char *p = endp;
            int val = (int) strtol(p, &endp, 10);
            if (endp == p)
                return -2; // not a number
            pchunks[i].minsize = val;
            pchunks[i].maxsize = val;
            pchunks[i].done = 0;
            pchunks[i].count = 1;
            // Is there a range character '-'?
            if (*endp == '-') {
                endp++;
                p = endp;
                int val = (int) strtol(p, &endp, 10);
                if (endp == p)
                    return -2; // not a number
                pchunks[i].maxsize = val;
            }
            // Is there also a count?
            if (*endp == '*' || *endp == 'x') {
                endp++;
                p = endp;
                val = (int) strtol(p, &endp, 10);
                if (endp == p)
                    return -2; // not a number
                pchunks[i].count = val;
            }
            if (*endp != ',' && *endp != '\0')
                return -3; // syntax error
            if (*endp == ',')
                endp++;
        }
    }
    if (type == 'r') {
        if (ipRecvChunks != NULL)
            return -1;
        ipRecvChunkCount = numChunks;
        ipRecvChunks = pchunks;
    }
    else if (type == 's') {
        if (ipSendChunks != NULL)
            return -1;
        ipSendChunkCount = numChunks;
        ipSendChunks = pchunks;
    }
    for (int i = 0; i < numChunks; i++)
        logDebug("chunk %d: size = %5d..%5d, count = %3d\n", i, pchunks[i].minsize, pchunks[i].maxsize, pchunks[i].count);

    return numChunks;
}

int ipRecv(int sd, void *buf, int size, int flags)
{
    // The current chunkIndex
    static int chunkIndex = 0;
    datachunk_t *chunk = &ipRecvChunks[chunkIndex];
    char chunkLogStr[100] = { 0 };

    int minLen = size;
    int maxLen = size;

    // When we have run out of chunks, skip chunking and just receive normally
    if (chunkIndex < ipRecvChunkCount) {
        if (chunk->maxsize < maxLen)
            maxLen = chunk->maxsize;
            minLen = chunk->minsize;
    }

    int len = 0;
    do {
        if (minLen == maxLen)
            logDebug(LOGPREFIX "ipRecv(%d, rem = %d)\n", minLen, minLen - len);
        else
            logDebug(LOGPREFIX "ipRecv(%d..%d, rem = %d..%d)\n", minLen, maxLen, minLen - len, maxLen - len);

        int lenRead = recv(sd, &((char *) buf)[len], maxLen - len, 0);
        if (lenRead <= 0) {
            logDebug(LOGPREFIX "ipRecv() %d\n", len);
            return lenRead;
        }
        len += lenRead;
        // Only loop when we run in chunked mode
    } while (chunkIndex < ipRecvChunkCount && len < minLen);

    if (len > 0 && chunkIndex < ipRecvChunkCount) {
        chunk->done += len;
        // Move to next chunk
        //if (chunk->size * chunk->count >= chunk->done)
            chunkIndex++;
        sprintf(chunkLogStr, " (%d of chunk %d)", len, chunkIndex);
    }
    logDebug(LOGPREFIX "ipRecv() %d%s\n", len, chunkLogStr);

    return len;
}
