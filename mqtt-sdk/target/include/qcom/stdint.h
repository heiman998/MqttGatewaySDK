/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef _STDINT_H_
#define _STDINT_H_

typedef unsigned short unshort;
typedef unsigned long ulong;
typedef unsigned int size_t;

/* now do it the way the CMU snmp code likes it: */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef unsigned long ip_addr;
typedef int socklen_t;

typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int     int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int   uint32_t;
/* typedef unsigned long   time_t;     ** OS-specific */
typedef volatile signed char     vint8_t;
typedef volatile signed short    vint16_t;
typedef volatile signed long     vint32_t;
typedef volatile unsigned char   vuint8_t;
typedef volatile unsigned short  vuint16_t;
typedef volatile unsigned long   vuint32_t;
#ifndef NULL
#define NULL            ((void *)0)
#endif

typedef int             bool_t;

#define SOCKTYPE        long
#define WP_SOCKTYPE     long

/* ASCII character codes */
#define NUL          '\000'   /* no character */
#define CTRL_C       '\003'   /* Control-C */
#define BS           '\010'   /* backspace */
#define TAB          '\t'     /* TAB */
#define CR           '\015'   /* carriage return */
#define LF           '\012'   /* line feed */
#define VT           '\013'   /* vertical tab */
#define FFEED        '\014'   /* form feed */
#define DC1          '\021'   /* Device Control 1 */
#define DC2          '\022'   /* Device Control 1 */
#define DC3          '\023'   /* Device Control 1 */
#define DC4          '\024'   /* Device Control 1 */
#define ESC          '\033'   /* ESCape */
#define DQUOTE       '\042'   /* Double Quote */
#define SQUOTE       '\047'   /* Single Quote */
#define SPACE        ' '      /* space */
/*#define EOF          '\377'   ** end-of-file */

#define NL           LF       /* newline == line feed */
#define XON          DC1
#define XOFF         DC3
#define DOUBLEQUOTE  DQUOTE
#define SINGLEQUOTE  SQUOTE

#endif  /* _STDINT_H_ */

