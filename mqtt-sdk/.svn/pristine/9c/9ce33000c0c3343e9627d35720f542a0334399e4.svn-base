/*
 * v4.0 July 5, 2011 h/socket.h
 * Distributed per ref US2611-2
 */
/*
 * FILENAME: socket.h
 *
 * Copyright 1997 - 2000 By InterNiche Technologies Inc. All rights reserved
 *
 *
 * MODULE: TCP
 *
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. [rescinded 22 July 1999]
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef  __SOCKET_H__
#define  __SOCKET_H__


#define  SYS_SOCKETNULL -1    /* error return from sys_socket. */
#define  INVALID_SOCKET -1    /* WINsock-ish synonym for SYS_SOCKETNULL */
#define  SOCKET_ERROR   -1    /* error return from send(), sendto(), et.al. */
#define  SOL_SOCKET     -1    /* compatability parm for set/get sockopt */

/*
 * Types
 */
#define     SOCK_STREAM    1     /* stream socket */
#define     SOCK_DGRAM     2     /* datagram socket */
#define     SOCK_RAW       3     /* raw-protocol interface */

#ifndef NPPORT
#define     SOCK_RDM       4     /* reliably-delivered message */
#define     SOCK_SEQPACKET 5     /* sequenced packet stream */
#endif   /* NPPORT*/

/*
 * Option flags per-socket.
 */
#define     SO_DEBUG       0x00001     /* turn on debugging info recording */
#define     SO_ACCEPTCONN  0x00002     /* socket has had listen() */
#define     SO_REUSEADDR   0x00004     /* allow local address reuse */
#define     SO_KEEPALIVE   0x00008     /* keep connections alive */
#define     SO_DONTROUTE   0x00010     /* just use interface addresses */
#define     SO_BROADCAST   0x00020     /* permit sending of broadcast msgs */
#define     SO_USELOOPBACK 0x00040     /* bypass hardware when possible */
#define     SO_LINGER      0x00080     /* linger on close if data present */
#define     SO_OOBINLINE   0x00100     /* leave received OOB data in line */
#define     SO_TCPSACK     0x00200     /* Allow TCP SACK (Selective acknowledgment) */
#define     SO_WINSCALE    0x00400     /* Set scaling window option */ 
#define     SO_TIMESTAMP   0x00800     /* Set TCP timestamp option */ 
#define     SO_BIGCWND     0x01000     /* Large initial TCP Congenstion window */ 
#define     SO_HDRINCL     0x02000     /* user access to IP hdr for SOCK_RAW */
#define     SO_NOSLOWSTART 0x04000     /* suppress slowstart on this socket */
#define     SO_FULLMSS     0x08000     /* force packets to all be MAX size */

#define     SO_LOWLATENCY  0x10000     /* flush recved data to uper as soon as possible */
#define     SO_ACTIVE_CLOSE  0x20000     /* set socket is open by qcom_socket in active open state */

extern uint16_t socket_defaults;       /* default so_options fo new sockets */

/* for compatability with second-rate stacks: */
#define SO_EXPEDITE     SO_NOSLOWSTART

/*
 * Additional options, not kept in so_options.
 */
#define  SO_SNDBUF      0x1001      /* send buffer size */
#define  SO_RCVBUF      0x1002      /* receive buffer size */
#define  SO_SNDLOWAT    0x1003      /* send low-water mark */
#define  SO_RCVLOWAT    0x1004      /* receive low-water mark */
#define  SO_SNDTIMEO    0x1005      /* send timeout */
#define  SO_RCVTIMEO    0x1006      /* receive timeout */
#define  SO_ERROR       0x1007      /* get error status and clear */
#define  SO_TYPE        0x1008      /* get socket type */
#define  SO_HOPCNT      0x1009      /* Hop count to get to dst   */
#define  SO_MAXMSG      0x1010      /* get TCP_MSS (max segment size) */

#define  IP_ADD_MEMBERSHIP 12 /* ip_mreq; add an IP group membership */
#define  IP_MULTICAST_IF   9  /* u_char; set/get IP multicast i/f  */

/*IPv6*/
#define IPV6_MULTICAST_IF   80 /* unisgned int; set IF for outgoing MC pkts */
#define IPV6_MULTICAST_HOPS 81 /* int; set MC hopcount */
#define IPV6_MULTICAST_LOOP 82 /* unisgned int; set to 1 to loop back */
#define IPV6_JOIN_GROUP     83 /* ipv6_mreq; join MC group */
#define IPV6_LEAVE_GROUP    84 /* ipv6_mreq; leave MC group */
#define IPV6_V6ONLY         85 /* int; IPv6 only on this socket */

/* ...And some netport additions to setsockopt: */
#define  SO_RXDATA      0x1011      /* get count of bytes in sb_rcv */
#define  SO_TXDATA      0x1012      /* get count of bytes in sb_snd */
#define  SO_MYADDR      0x1013      /* return my IP address */
#define  SO_NBIO        0x1014      /* set socket into NON-blocking mode */
#define  SO_BIO         0x1015      /* set socket into blocking mode */
#define  SO_NONBLOCK    0x1016      /* set/get blocking mode via optval param */
#define  SO_CALLBACK    0x1017      /* set/get zero_copy callback routine */

#define  SO_SNDBUFCNT    0x1018      /* get count of mbuf in sock_snd buf chain */
#define  SO_RCVBUFCNT    0x1019      /* get count of mbuf in sock_rcv buf chain */
/*
 * TCP User-settable options (used with setsockopt).
 * TCP-specific socket options use the 0x2000 number space.
 */

#define  TCP_ACKDELAYTIME 0x2001    /* Set time for delayed acks */
#define  TCP_NOACKDELAY   0x2002    /* suppress delayed ACKs */
#define  TCP_MAXSEG       0x2003    /* set maximum segment size */
#define  TCP_NODELAY      0x2004    /* Disable Nagle Algorithm */
#define  TCP_SND_BACKOFF    0x2005    /* tcp tx pending buf per socket */


/*
 * Structure used for manipulating linger option.
 */
struct   linger
{
   int   l_onoff;    /* option on/off */
   int   l_linger;   /* linger time */
};


/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr
{
	u_short     sa_family;     /* address family */

	union
	{
		char     data4[14];      /* up to 14 bytes of direct address */
		char	 data6[32];	   /* big enough for unpacked sockaddr_in6 */
	}sa;
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto
{
   u_short     sp_family;     /* address family */
   u_short     sp_protocol;   /* protocol */
};

#if defined(INCLUDE_SNMP)
struct conn
{
   ip_addr laddr;  /* local IPv4 address (TCP or UDP) */
   ip_addr raddr;  /* remote IPv4 address (TCP only) */
   uint16_t lport; /* local port (TCP or UDP) */
   uint16_t rport; /* remote port (TCP only) */
   uint8_t state;  /* TCP state identifier */
};
#endif
/*
 * RFC 2553: protocol-independent placeholder for socket addresses
 */
#define _SS_MAXSIZE     32
#define _SS_ALIGNSIZE   sizeof(unsigned long)  /* 4-byte alignment desired 
                                                  for structure */
#define _SS_PAD1SIZE    (_SS_ALIGNSIZE - sizeof(u_short))
#define _SS_PAD2SIZE    (_SS_MAXSIZE - sizeof(u_short) - \
                            _SS_PAD1SIZE - _SS_ALIGNSIZE)

struct sockaddr_storage
{
   u_short         ss_family;               /* address family */
   char            __ss_pad1[_SS_PAD1SIZE];
   unsigned long   __ss_align;              /* force desired structure alignment */
   char            __ss_pad2[_SS_PAD2SIZE];
};

struct in_addr
{
   unsigned long  s_addr;
};

#define  INADDR_ANY     0L


/* Berkeley style "Socket address" */

struct sockaddr_in
{
   short    sin_family;
   u_short  sin_port;
   struct   in_addr  sin_addr;
   char     sin_zero[8];
};

typedef struct ip6addr{
    char addr[16];
}ip6addr_t;
struct sockaddr_in6
{
   u_short        sin6_family;   /* AF_INET6 */
   u_short        sin6_port;     /* transport layer port # */
   u_long         sin6_flowinfo; /* IPv6 flow information */
   ip6addr_t       sin6_addr;     /* IPv6 address */
   int            sin6_scope_id; /* set of interfaces for a scope */
};

/* Support for Large initial congestion window */
#ifdef TCP_BIGCWND
extern   int      use_default_cwnd;    /* Flag to use this on all sockets */
extern   u_long   default_cwnd;        /* initial cwnd value to use */
#endif   /* TCP_BIGCWND */

#define  AF_INET     2     /* internetwork: UDP, TCP, etc. */
#define  AF_INET6    3     /* IPv6 */
#define  AF_INET_ALL 255   /* only used by CLI code to indicate */
                           /* all domains */
#define  PF_INET6    AF_INET6
#define  AF_UNSPEC   0

/*
 * Protocol families, same as address families for now.
 */
#define  PF_UNSPEC   AF_UNSPEC
#define  PF_UNIX     AF_UNIX
#define  PF_INET     AF_INET
#define  PF_IMPLINK  AF_IMPLINK
#define  PF_PUP      AF_PUP
#define  PF_CHAOS    AF_CHAOS
#define  PF_NS       AF_NS
#define  PF_NBS      AF_NBS
#define  PF_ECMA     AF_ECMA
#define  PF_DATAKIT  AF_DATAKIT
#define  PF_CCITT    AF_CCITT
#define  PF_SNA      AF_SNA
#define  PF_DECnet   AF_DECnet
#define  PF_DLI      AF_DLI
#define  PF_LAT      AF_LAT
#define  PF_HYLINK   AF_HYLINK
#define  PF_APPLETALK   AF_APPLETALK


/*
 * Maximum queue length specifiable by listen.
 */
#define     SOMAXCONN   5

#define     MSG_OOB        0x1      /* process out-of-band data */
#define     MSG_PEEK       0x2      /* peek at incoming message */
#define     MSG_DONTROUTE  0x4      /* send without using routing tables */
#define     MSG_NEWPIPE    0x8      /* New pipe for recvfrom call   */
#define     MSG_EOR        0x10     /* data completes record */
#define     MSG_DONTWAIT   0x20     /* this message should be nonblocking */

#ifndef lswap
#define lswap(n)	\
	((((n) & 0xff) << 24) | (((n) & 0xff00) << 8) | (((n) & 0xff0000UL) >> 8) | (((n) & 0xff000000UL) >> 24))
#endif

#ifndef htonl
#define htonl(l) lswap(l)
#endif

#ifndef ntohl
#define ntohl(l) lswap(l)
#endif

/* Amazingly, some compilers really need all these u_short casts: */
#define htons(s) ((u_short)((u_short)(s) >> 8) | (u_short)((u_short)(s) << 8))
#define ntohs(s) htons(s)

/* utility functions defined in misclib\parseip.c */
//extern int inet46_addr(char *str, struct sockaddr *address); 
//extern void inet_setport(struct sockaddr *addr,int port);
u_long inet_addr(char * str);
char *inet_ntoa(unsigned long ip);
#endif   /* __SOCKET_H__ */

