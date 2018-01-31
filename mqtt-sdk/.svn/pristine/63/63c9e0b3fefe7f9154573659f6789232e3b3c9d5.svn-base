/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
/****************************************************************************************
 * Certificate server/client test application
 * This is a simple certificate server and client application
 *
***************************************************************************************/

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

#define DEFAULT_PORT        1443


// ============================================================================
// Globals
// ============================================================================

/** Command-line options */
typedef struct
{
    /** Logging level */
    char verbose;
    /** True to use syslog for logging. Default is use console */
    char useSyslog;
    /** Log mask constructed from command-line arguments. TODO: Set to zero at final release  */
    int logMask;
    /** Log mask timestamp size */
    char *logTimestampFormat;

    char isServer;
    int port;

    char *host;
    char *url;
} arg_t;

static arg_t arg = {
    .verbose = 1,
    .logMask = 0,
    .logTimestampFormat = "%H:%M:%S ",

    .isServer = 0,
    .port = DEFAULT_PORT,

    .host = "localhost",
    .url = "kingfisher.crt.bin",
};


// ================================================================================================
// Globals
// ================================================================================================

#define CERT_HEADER_LEN sizeof(certHeader_t)

typedef struct certHeader {
	char id[4];
	uint32_t length;
	char data[0];
} certHeader_t;

static char idDefault[] = { 'C', 'R', 'T', '0', 0, 0, 0, 0 };


// ================================================================================================
// Utility Functions
// ================================================================================================

static int ipsend(int sd, const void *buf, int len)
{
    logDebug("ipsend(%d, buf, %d)\n", sd, len);
	return send(sd, buf, len, 0);
}

/**
 * Receive exactly size bytes
 * @return Number of bytes read on success, -1 on error
 */
int recvBytes(int sd, void *buf, int size)
{
	int len, lenDone = 0;
	char *p = buf;

	while (lenDone < size) {
		len = ipRecv(sd, &p[lenDone], size - lenDone, 0);
		if (len == 0)
			return 0;
		if (len < 0 && len != EAGAIN) {
			logError("ipRecv() %d\n", len);
			return len;
		}
		lenDone += len;
	}
	return lenDone;
}

/**
 * Read file, returning contents in buf
 *
 * @return file size on success, -1 on error
 */
static int readFile(const char *filename, char **buf)
{
    int fd, res;
    struct stat statBuf;

    logVerb("%s('%s')\n", __func__, filename);

    if ((fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR)) < 0) {
    	if (errno == ENOENT)
    		logWarn("File '%s' not found\n", filename);
        return fd;
    }

    if ((res = stat(filename, &statBuf)) < 0) {
        logSysError("%s.stat\n",  __func__);
        return res;
    }

    *buf = malloc(statBuf.st_size + 1);

    if (read(fd, *buf, statBuf.st_size) < 0)
        dieSysError("read()\n");

    close(fd);

    return statBuf.st_size;
}


// ================================================================================================
// Client
// ================================================================================================

/**
 * Read the CRT protocol header returning the length of the data field
 */
static int readHeader(int sd, void *buf)
{
	char *p = (char *) buf;

	int len;
	if ((len = recvBytes(sd, buf, CERT_HEADER_LEN)) <= 0)
		return len;

	if (p[0] != 'C' || p[1] != 'R' || p[2] != 'T') {
		logError("Bad MAGIC received in header\n");
		return -2;
	}
	if (p[3] != '0') {
		logError("Unknown protocol/request received in header\n");
		return -2;
	}
	certHeader_t *req = (certHeader_t *) buf;
	req->length = htonl(req->length);

	if (req->length == 0) {
		logWarn("Received header with data length = %d\n", req->length);
		return 0;
	}

	return req->length;
}

static int client()
{
	int sd = ipConnectPeer(arg.host, arg.port, 3);

	int filenameLen = strlen(arg.url);
	int reqLen = CERT_HEADER_LEN + filenameLen;
	certHeader_t *req = (certHeader_t *) malloc(reqLen);
	if (req == NULL)
		dieError("%s.malloc\n", __func__);

	req->id[0] = 'C';
	req->id[1] = 'R';
	req->id[2] = 'T';
	req->id[3] = '0';
	req->length = htonl(filenameLen);
	memcpy(&req->data[0], arg.url, filenameLen);
	logNorm("Requesting certificate '%s' (data len = %d)\n", arg.url, filenameLen);
	int len = ipsend(sd, req, reqLen);
	free(req);
	if (len < 0)
		return len;

	// Read the response
	char buf[1024];
	if ((len = readHeader(sd, buf)) < 0)
		return len;
	if (len == 0) {
		logError("Received error response (certificate not found?)\n");
		return -2;
	}

	void *certBuf = malloc(len);
	if (certBuf == NULL)
		dieError("%s.malloc\n", __func__);

	// Read the certificate
	int res;
	logNorm("Reading certificate of %d bytes\n", len);
	if ((res = recvBytes(sd, certBuf, len)) < 0)
		return res;
	hexdump(certBuf, 16, 16);

	close(sd);

	return 0;
}


// ============================================================================
// Server
// ============================================================================

static int processRequest(int sd)
{
	char buf[1024];
	int len;

	if ((len = readHeader(sd, buf)) < 0)
		return len;
	if (len == 0) {
		logWarn("File not found, sending zero response\n", len);
		ipsend(sd, &idDefault, CERT_HEADER_LEN);
		return 0;
	}

	logNorm("Received header. Receiving data of length = %d\n", len);
	certHeader_t *req = (certHeader_t *) buf;
	if ((len = recvBytes(sd, &req->data, len)) <= 0)
		return len;

	char *filename = &req->data[0];
	filename[len] = 0;
	char *fileBuf;
	logNorm("Reading certificate file '%s'\n", filename);
	if ((len = readFile(filename, &fileBuf)) < 0)
		return len;

	int rspLen = CERT_HEADER_LEN + len;
	certHeader_t *rsp = (certHeader_t *) malloc(rspLen);
	if (rsp == NULL)
		dieError("%s.malloc\n", __func__);

	memcpy(&rsp->id, idDefault, 4);
	rsp->length = htonl(len);
	memcpy(&rsp->data, fileBuf, len);
	hexdump(&rsp->data, 16, 16);
	logNorm("Sending certificate of size %d bytes\n", len);
	len = ipsend(sd, rsp, rspLen);

	free(fileBuf);
	free(rsp);

	return len;
}

static void serverLoop()
{
    int listensd = ipListenSocket(arg.port);

    int numRequestsServed = 0;

    logDebug("sizeof(certHeader_t) = %d\n", sizeof(certHeader_t));

	for (;;)
	{
		int sd = ipAcceptConnection(listensd);
		int res = processRequest(sd);
		logNorm("%s() %d served %d requests\n", __func__, res, ++numRequestsServed);

		close(sd);
	}
}

// ============================================================================
// Main and Command-line Parsing
// ============================================================================

static void printUsage(char *argv[])
{
    printf("Usage: %s [OPTION] HOST[:PORT][URL]\n", argv[0]);
    printf("SSL Certificate server, Copyright 2014, RTX A/S (" __DATE__ ", " __TIME__ ")\n");
    printf("Generate a binary certificate (for the server) with the command:\n");
    printf("  SharkSSLParseCert NAME.pem NAME.key -b NAME.cert.bin\n");
    printf("And a binary CAList with the command:\n");
    printf("  SharkSSLParseCAList NAME.pem -b NAME.calist.bin\n");
    printf("where NAME is some descriptive name and NAME.pem is a certificate\n");
    printf("for example generated with 'openssl x509/req'\n");
    printf("OPTIONS:\n");
    printf("  -s       Start as server\n");
    printf("  -p PORT  Port number (default is %d)\n", DEFAULT_PORT);
    printf("  -v       Be more verbose (can be repeated up to 4 times)\n");
    printf("  -q       Be more quiet (can be repeated up to 4 times)\n");
    printf("  -h       Help\n");
    printf("Client Examples:\n");
    printf("  %s localhost/cert-kingfisher\n", argv[0]);
    printf("Server Examples:\n");
    printf("  %s -s -p 1043 -vv\n", argv[0]);
    exit(0);
}

static char **argp;
static int argc;

/**
 * Parse command-line option with integer argument
 *
 * @param optionName
 * @param value = pointer to integer variable
 */
static void parseInt(const char *optionName, int *value)
{
    char *endp;

    if (argp[0][2] != '\0') {
        *value = (int) strtol(&argp[0][2], &endp, 10);
        if (*endp == '\0')
            return;
    }
    else if (argc > 1) {
        *value = (int) strtol(&argp[1][0], &endp, 10);
        argp++;
        argc--;
        if (*endp == '\0')
            return;
    }
    dieError("option '%s': integer expected\n", optionName);
}

/**
 * Parse command-line option with string argument
 *
 * @param optionName
 * @param value
 */
static void parseHostPortUrl(const char *optionName, char **host, int *port, char **url)
{
    char *q, *endp;
    endp = argp[0];

    // Only set host if host name was supplied
    if (endp[0] != ':' && endp[0] != '/')
        *host = argp[0];

    // Parse optional port number
    q = strchr(endp, ':');
    if (q != NULL ) {
        // Terminate host name
        *q++ = '\0';
        *port = (short) strtol(q, &endp, 10);
    }
    // Parse optional url
    q = strchr(endp, '/');
    if (q != NULL ) {
        int len = strlen(q);
        *url = malloc(len + 1);
        memcpy(*url, q, len + 1);
        // Terminate host name
        *q = '\0';
    }
}

static void parseCommandLine(int in_argc, char *argv[])
{
    char *p;
    argc = in_argc;
    argp = argv;
    // skip program name
    argp++;
    argc--;

    while (argc > 0)
    {
        if (argp[0][0] != '-') {
        	parseHostPortUrl(NULL, &arg.host, &arg.port, &arg.url);
        }
        else {
            switch (argp[0][1])
            {
                case 'p': parseInt("-p", &arg.port); break;
                case 's': arg.isServer = 1; break;
                case 'v':
                    p = &argp[0][1];
                    while (*p++ == 'v')
                        arg.verbose++;
                    break;
                case 'q':
                    p = &argp[0][1];
                    while (*p++ == 'q')
                        arg.verbose--;
                    break;
                case 'h':
                    printUsage(argv);
                    exit(0);
                    break;
                default:
                    printf("Bad option: '%c'\n", argp[0][1]);
                    printUsage(argv);
                    exit(0);
                    break;
            }
        }
        argp++;
        argc--;
    }
}

static int init()
{
    int res;

    if ((res = ipInit()) < 0)
        return res;

    return res;
}

int main(int argc, char *argv[])
{
    parseCommandLine(argc, argv);
    logInit("certserv", arg.useSyslog, arg.verbose, arg.logMask);
    //logNorm("arg.verbose = %d, arg.log_mask = %d\n", arg.verbose, arg.logMask);
    //logNorm("arg.chunks = %s\n", arg.chunks);

    if (init() < 0)
        dieError("init()\n");

	// Skip the slash
	if (arg.url[0] == '/')
		arg.url++;
    logVerb("host = '%s', url = '%s', port = %d\n", arg.host, arg.url, arg.port);

    if (arg.isServer)
        serverLoop();
    else
        client();

    return 0;
}
