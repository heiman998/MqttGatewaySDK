/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "utility.h"


// ================================================================================================
//
// ================================================================================================

/** Print a hexdump from given address and length with indicated number of columns */
void hexdump(const void *mem, unsigned int len, unsigned char columns)
{
    unsigned int i, j;
    unsigned int len_roundedup = columns * ((len + columns - 1) / columns);
    // len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS);

    for (i = 0; i < len_roundedup; i++)
    {
        // print offset
        if (i % columns == 0)
            printf("%04x ", i);

        // print hex data
        if (i < len)
            printf("%02x ", ((char*) mem)[i] & 0xFF);
        else
            printf("   "); // filler spaces

        // print ASCII dump when we have printed the last byte on this line
        if (i % columns == (columns - 1)) {
            for (j = i - (columns - 1); j <= i; j++) {
                if (j >= len) // end of block, not really printing
                    putchar(' ');
                else if (isprint(((char*) mem)[j])) // printable char
                    putchar(0xFF & ((char*) mem)[j]);
                else // other char
                    putchar('.');
            }
            putchar('\n');
        }
    }
}

/** Convert byte buffer to hex string.
 *
 * @param buf = destination buffer to store hex string
 * @param size = size of destination buffer
 * @param addr = address of byte buffer
 * @param len = length of byte buffer
 * @param flags = bit-0: space should be inserted between each hex value
 *                bit-1: ASCII representation should be appended
 * @return Number of bytes converted (value will be less than len if the buffer was too small)
 */
int toHexString(char *buf, size_t bufSize, const void *addr, size_t len, char flags)
{
    static const char *hexdigitsUpper = "0123456789ABCDEF";
    static const char *hexdigitsLower = "0123456789abcdef";
    const char *hexdigits = (flags & (1 << 2) ? hexdigitsUpper : hexdigitsLower);

    if (buf == NULL)
        return -1;

    if (bufSize < 3)
        return 0;

    // Compute maximum number of bytes that can be printed to buf
    int maxLen = 0;
    if (flags & 2)
        // We need 3 or 4 bytes when also appending ASCII
        // In the case of no-spaces we have one byte less for the space between hex and ASCII and one byte less for NUL
        maxLen = (flags & 1 ? (bufSize - 1) / 4 : (bufSize - 2) / 3);
    else
        // We need 2 or 3 bytes when NOT appending ASCII
        maxLen = (flags & 1 ? bufSize / 3 : (bufSize - 1) / 2);

    if (maxLen == 0)
        return 0;

    // Flag that we need to terminate hex string with ellipsis
    // This decrease in number of bytes is slightly conservative
    int makeEllipsis = 0;
    if (len > maxLen) {
        makeEllipsis = 1;
        len = maxLen - 1;
    }

    char *pbuf = buf;
    for (int i = 0; i < len; i++)
    {
        char value = ((char *) addr)[i];
        *pbuf++ = hexdigits[(value >> 4) & 0xF];
        *pbuf++ = hexdigits[value & 0xF];
        if (flags & 1)
            *pbuf++ = ' ';
    }
    if (makeEllipsis) {
        *pbuf++ = '.';
        *pbuf++ = '.';
    }

    // Optionally append ASCII representation
    if (flags & 2) {
        if (pbuf[-1] != ' ')
            *pbuf++ = ' ';
        for (int i = 0; i < len; i++)
        {
            char value = ((char *) addr)[i];
            if (value < ' ' || value > 126)
                value = '.';
            *pbuf++ = value;
        }
        if (flags & 1)
            *pbuf++ = ' ';
    }
    if (flags & 1) {
        // Prepare to replace last space with the NUL terminator
        pbuf--;
    }
    *pbuf++ = 0;

    return len;
}

/**
 * Convert hexstr to byte buffer
 * hexstr is a hexdump string of hexadecimal digit pairs: "3E 0F BA" or "3E0FBA"
 *
 * @param buf = destination byte buffer
 * @param size = size of destination byte buffer
 * @param hexstr = hex dump string, possibly with spaces interlaving the hex digit pairs
 * @return Number of bytes inserted into destination byte buffer. On syntax error (bad hex digit),
 *         a negative number is returned, whose absolute value indicates what byte was being convereted
 */
int fromHexString(void *outBuf, size_t size, const char *hexstr)
{
    char *buf = (char *) outBuf;
    char *bufStart = outBuf;

    while (*hexstr != 0)
    {
        unsigned char msb, lsb;

        while (*hexstr == ' ')
            hexstr++;

        if ( (msb = *hexstr++) == 0 )
            break;
        msb = toupper(msb);
        if ('A' <= msb)
            msb -= 'A' - 10;
        if (msb >= '0')
            msb -= '0';
        if (msb > 15)
            return -(buf - bufStart);

        if ( (lsb = *hexstr++) == 0 )
            break;
        lsb = toupper(lsb);
        if ('A' <= lsb)
            lsb -= 'A' - 10;
        if (lsb >= '0')
            lsb -= '0';
        if (lsb > 15)
            return -(buf - bufStart);

        unsigned char value = msb << 4 | lsb;
        *buf++ = value;
    };
    return buf - bufStart;
}

void strToUpper(char *str)
{
    while (*str != '\0') {
        *str = toupper(*str);
        str++;
    }
}

void strToLower(char *str)
{
    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}


// ================================================================================================
// Logging
// ================================================================================================

static logConfig_t log = {
    .level = 0,
    .useSyslog = 0,
    .mask = 0,
    .syslogName = NULL,
    .timeStampFormat = "%Y-%m-%dT%H:%M:%S ",
    .errorFilename = NULL,
};

/** Set the log level to the given value: 0 = notice, 1 = normal, 2 = verbose, 3 = debug, 4 = trace
 *
 * @param timeStampFormat "%Y-%m-%d %H:%M:%S "
 */
void logInit(const char *appName, char useSyslog, char level, int mask)
{
    log.useSyslog = useSyslog;
    log.level = level;
    log.mask = mask;
    if (log.useSyslog)
        openlog(appName, LOG_CONS, LOG_USER);
    setvbuf(stdout, NULL, _IONBF, 0);
}

void logGetConfig(logConfig_t *logConfig)
{
    memcpy(logConfig, &log, sizeof(logConfig_t));
}

void logSetConfig(const logConfig_t *logConfig)
{
    log.level = logConfig->level;
    log.mask = logConfig->mask;

    if (logConfig->timeStampFormat != NULL)
        log.timeStampFormat = (char *) logConfig->timeStampFormat;
    if (logConfig->errorFilename != NULL)
        log.errorFilename = (char *) logConfig->errorFilename;

    log.useSyslog = logConfig->useSyslog;
    if (logConfig->syslogName != NULL)
        log.syslogName = (char *) logConfig->syslogName;
    if (log.useSyslog)
        openlog(log.syslogName, LOG_CONS, LOG_USER);
}

int logGetMask(void)
{
    return log.mask;
}

#if 0
static int snprinttime(char *buf, size_t size)
{
    if (log.timeStampFormat == NULL)
        return 0;

    time_t now = time(NULL);
    return strftime(buf, size, log.timeStampFormat, localtime(&now));
}
#endif

/**
 * Exit after a system error. Print the system errno and corresponding message (with strerror())
 *
 * @param format
 */
void dieSysError(const char *format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char msg[100];
    int index = 0;
    //index += snprinttime(&msg[index], sizeof(msg) - index);
    index += snprintf(&msg[index], sizeof(msg) - index, "FATAL (errno = %d, %s): ", errno, strerror(errno));
    index += vsnprintf(&msg[index], sizeof(msg) - index, format, argptr);

    if (log.useSyslog)
        syslog(LOG_ERR, "%s", msg);

    fprintf(stderr, "%s", msg);

    va_end(argptr);
    exit(1);
}

/**
 * Exit, printing error message.
 *
 * @param format
 */
void dieError(const char *format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char msg[100];
    int index = 0;
    //index += snprinttime(&msg[index], sizeof(msg) - index);
    index += snprintf(&msg[index], sizeof(msg) - index, "FATAL: ");
    index += vsnprintf(&msg[index], sizeof(msg) - index, format, argptr);

    if (log.useSyslog)
        syslog(LOG_ERR, "%s", msg);

    fprintf(stderr, "%s", msg);

    va_end(argptr);
    exit(1);
}

/** Log error */
void logError(const char *format, ...)
{
    // Never log anything if log.level is less than -1
    if (log.level < -1)
        return;

    va_list argptr;
    va_start(argptr, format);

    char msg[100];
    int index = 0;
    //index += snprinttime(&msg[index], sizeof(msg) - index);
    index += snprintf(&msg[index], sizeof(msg) - index, "ERROR: ");
    index += vsnprintf(&msg[index], sizeof(msg) - index, format, argptr);

    if (log.useSyslog)
        syslog(LOG_ERR, "%s", msg);

    fprintf(stderr, "%s", msg);

    va_end(argptr);
}

void logWarn(const char *format, ...)
{
    // Never log anything if log.level is less than -1
    if (log.level < -1)
        return;

    va_list argptr;
    va_start(argptr, format);

    char msg[100];
    int index = 0;
    //index += snprinttime(&msg[index], sizeof(msg) - index);
    index += snprintf(&msg[index], sizeof(msg) - index, "WARNING: ");
    index += vsnprintf(&msg[index], sizeof(msg) - index, format, argptr);

    if (log.useSyslog)
        syslog(LOG_WARNING, "%s", msg);

    fprintf(stderr, "%s", msg);

    va_end(argptr);
}

/** Log system error. Similar to perror() */
void logSysError(const char *format, ...)
{
    // Never log anything if log.level is less than -1
    if (log.level < -1)
        return;

    va_list argptr;
    va_start(argptr, format);

    char msg[100];
    int index = 0;
    //index += snprinttime(&msg[index], sizeof(msg) - index);
    index += snprintf(&msg[index], sizeof(msg) - index, "FATAL (errno = %d, %s): ", errno, strerror(errno));
    index += vsnprintf(&msg[index], sizeof(msg) - index, format, argptr);

    if (log.useSyslog)
        syslog(LOG_ERR, "%s", msg);

    fprintf(stderr, "%s", msg);

    va_end(argptr);
}

/** Log message if the given flag is enabled in the log mask */
void logMasked(int flag, const char *format, ...)
{
    //printf("log.mask = 0x%x, flag = 0x%x\n", log.mask, flag);
    if ((log.mask & flag) || (flag == 0xFF && log.level >= 0))
    {
        va_list argptr;
        va_start(argptr, format);
        if (log.useSyslog)
            vsyslog(LOG_NOTICE, format, argptr);
        else
            vfprintf(stdout, format, argptr);
        va_end(argptr);
    }
}

/** Log message with highest priority (logNote, logNorm, logVerb, logDebug, logTrace) */
void logNote(const char *format, ...)
{
    if (0 > log.level)
        return;

    va_list argptr;
    va_start(argptr, format);
    if (log.useSyslog)
        vsyslog(LOG_NOTICE, format, argptr);
    else
        vfprintf(stdout, format, argptr);
    va_end(argptr);
}

/** Log message with 2. highest priority (logNote, logNorm, logVerb, logDebug, logTrace) */
void logNorm(const char *format, ...)
{
    if (1 > log.level)
        return;

    va_list argptr;
    va_start(argptr, format);
    if (log.useSyslog)
        vsyslog(LOG_NOTICE, format, argptr);
    else
        vfprintf(stdout, format, argptr);
    va_end(argptr);
}

/** Log message with 3. highest priority (logNote, logNorm, logVerb, logDebug, logTrace) */
void logVerb(const char *format, ...)
{
    if (2 > log.level)
        return;

    va_list argptr;
    va_start(argptr, format);
    if (log.useSyslog)
        vsyslog(LOG_NOTICE, format, argptr);
    else
        vfprintf(stdout, format, argptr);
    va_end(argptr);
}

/** Log message with 4. priority (logNote, logNorm, logVerb, logDebug, logTrace) */
void logDebug(const char *format, ...)
{
    if (3 > log.level)
        return;

    va_list argptr;
    va_start(argptr, format);
    if (log.useSyslog)
        vsyslog(LOG_NOTICE, format, argptr);
    else
        vfprintf(stdout, format, argptr);
    va_end(argptr);
}

/** Log message with lowest priority (logNote, logNorm, logVerb, logDebug, logTrace) */
void logTrace(const char *format, ...)
{
    if (4 > log.level)
        return;

    va_list argptr;
    va_start(argptr, format);
    if (log.useSyslog)
        vsyslog(LOG_NOTICE, format, argptr);
    else
        vfprintf(stdout, format, argptr);
    va_end(argptr);
}

void logHexStr(int level, const char *message, const void *buf, unsigned int len)
{
    if (level > log.level)
        return;

    char hexStrBuf[400] = "";
    toHexString(hexStrBuf, sizeof(hexStrBuf), buf, len, 3);
    const char *format = "%s (%2d) %s\n";

    if (log.useSyslog)
        syslog(LOG_NOTICE, format, message, len, hexStrBuf);
    else
        printf(format, message, len, hexStrBuf);
}
