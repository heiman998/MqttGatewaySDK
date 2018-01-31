/* Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* 
 * Copyright (C) 2010--2016 Olaf Bergmann <bergmann@tzi.org>
 * 
 */
/*
 * For this file, which was received with alternative licensing options for  
 * distribution, Qualcomm Technologies, Inc. has selected the BSD license.
 */

#ifndef __QCOM_COAP_H__
#define __QCOM_COAP_H__
#include "basetypes.h"
#include "qcom_ssl.h"

#define COAP_TICKS_PER_SECOND 1000

#define COAP_DEFAULT_PORT      5683 /* CoAP default UDP port */
#define COAPS_DEFAULT_PORT     5684 /* CoAP default UDP port for secure transmission */
#define COAP_DEFAULT_MAX_AGE     60 /* default maximum object lifetime in seconds */


/* CoAP message types */
#define COAP_MESSAGE_CON       0 /* confirmable message (requires ACK/RST) */
#define COAP_MESSAGE_NON       1 /* non-confirmable message (one-shot message) */
#define COAP_MESSAGE_ACK       2 /* used to acknowledge confirmable messages */
#define COAP_MESSAGE_RST       3 /* indicates error in received messages */

/* CoAP request methods */
#define COAP_REQUEST_GET       1
#define COAP_REQUEST_POST      2
#define COAP_REQUEST_PUT       3
#define COAP_REQUEST_DELETE    4

/* CoAP option types (be sure to update check_critical when adding options */
#define COAP_OPTION_IF_MATCH        1 /* C, opaque, 0-8 B, (none) */
#define COAP_OPTION_URI_HOST        3 /* C, String, 1-255 B, destination address */
#define COAP_OPTION_ETAG            4 /* E, opaque, 1-8 B, (none) */
#define COAP_OPTION_IF_NONE_MATCH   5 /* empty, 0 B, (none) */
#define COAP_OPTION_URI_PORT        7 /* C, uint, 0-2 B, destination port */
#define COAP_OPTION_LOCATION_PATH   8 /* E, String, 0-255 B, - */
#define COAP_OPTION_URI_PATH       11 /* C, String, 0-255 B, (none) */
#define COAP_OPTION_CONTENT_FORMAT 12 /* E, uint, 0-2 B, (none) */
#define COAP_OPTION_CONTENT_TYPE COAP_OPTION_CONTENT_FORMAT
#define COAP_OPTION_MAXAGE         14 /* E, uint, 0--4 B, 60 Seconds */
#define COAP_OPTION_URI_QUERY      15 /* C, String, 1-255 B, (none) */
#define COAP_OPTION_ACCEPT         17 /* C, uint,   0-2 B, (none) */
#define COAP_OPTION_LOCATION_QUERY 20 /* E, String,   0-255 B, (none) */
#define COAP_OPTION_PROXY_URI      35 /* C, String, 1-1034 B, (none) */
#define COAP_OPTION_PROXY_SCHEME   39 /* C, String, 1-255 B, (none) */
#define COAP_OPTION_SIZE1          60 /* E, uint, 0-4 B, (none) */

/* option types from draft-ietf-coap-observe-09 */
#define COAP_OPTION_OBSERVE         6 /* E, empty/uint, 0 B/0-3 B, (none) */
#define COAP_OPTION_SUBSCRIPTION  COAP_OPTION_OBSERVE

/* selected option types from draft-core-block-04 */
#define COAP_OPTION_BLOCK2         23 /* C, uint, 0--3 B, (none) */
#define COAP_OPTION_BLOCK1         27 /* C, uint, 0--3 B, (none) */

/* selected option types from draft-tcs-coap-no-response-option-11 */
#define COAP_OPTION_NORESPONSE    284 /* N, uint, 0--1 B, 0 */
#define COAP_MAX_OPT            65535 /**< the highest option number we know */

/* CoAP result codes (HTTP-Code / 100 * 40 + HTTP-Code % 100) */
/* As of draft-ietf-core-coap-04, response codes are encoded to base
 * 32, i.e.  the three upper bits determine the response class while
 * the remaining five fine-grained information specific to that class.
 */
#define COAP_RESPONSE_CODE(N) (((N)/100 << 5) | (N)%100)

/* Determines the class of response code C */
#define COAP_RESPONSE_CLASS(C) (((C) >> 5) & 0xFF)

#define COAP_RESPONSE_200      COAP_RESPONSE_CODE(200)  /* 2.00 OK */
#define COAP_RESPONSE_201      COAP_RESPONSE_CODE(201)  /* 2.01 Created */
#define COAP_RESPONSE_304      COAP_RESPONSE_CODE(203)  /* 2.03 Valid */
#define COAP_RESPONSE_400      COAP_RESPONSE_CODE(400)  /* 4.00 Bad Request */
#define COAP_RESPONSE_404      COAP_RESPONSE_CODE(404)  /* 4.04 Not Found */
#define COAP_RESPONSE_405      COAP_RESPONSE_CODE(405)  /* 4.05 Method Not Allowed */
#define COAP_RESPONSE_415      COAP_RESPONSE_CODE(415)  /* 4.15 Unsupported Media Type */
#define COAP_RESPONSE_500      COAP_RESPONSE_CODE(500)  /* 5.00 Internal Server Error */
#define COAP_RESPONSE_501      COAP_RESPONSE_CODE(501)  /* 5.01 Not Implemented */
#define COAP_RESPONSE_503      COAP_RESPONSE_CODE(503)  /* 5.03 Service Unavailable */
#define COAP_RESPONSE_504      COAP_RESPONSE_CODE(504)  /* 5.04 Gateway Timeout */
#define COAP_RESPONSE_X_242    COAP_RESPONSE_CODE(402)  /* Critical Option not supported */

/* CoAP media type encoding */
#define COAP_MEDIATYPE_TEXT_PLAIN                 0 /* text/plain (UTF-8) */
#define COAP_MEDIATYPE_APPLICATION_LINK_FORMAT   40 /* application/link-format */
#define COAP_MEDIATYPE_APPLICATION_XML           41 /* application/xml */
#define COAP_MEDIATYPE_APPLICATION_OCTET_STREAM  42 /* application/octet-stream */
#define COAP_MEDIATYPE_APPLICATION_RDF_XML       43 /* application/rdf+xml */
#define COAP_MEDIATYPE_APPLICATION_EXI           47 /* application/exi  */
#define COAP_MEDIATYPE_APPLICATION_JSON          50 /* application/json  */
#define COAP_MEDIATYPE_APPLICATION_CBOR          60 /* application/cbor  */


/*CoAP endpoint flags*/
#define COAP_ENDPOINT_NOSEC    0x0000
#define COAP_ENDPOINT_DTLS     0x0001
#define COAP_ENDPOINT_UDP     0x0002
#define COAP_ENDPOINT_TLS    0x0004
#define COAP_ENDPOINT_TCP    0x0008
#define COAP_ENDPOINT_IPV4    0x0010
#define COAP_ENDPOINT_IPV6    0x0020
#define COAP_ENDPOINT_CLIENT    0x0040
#define COAP_ENDPOINT_SERVER    0x0080
#define COAP_ENDPOINT_GROUP    0x0100

#define COAP_ENDPOINT_HAS_DATA 0x1000
#define COAP_FLAGS_BLOCK         0x2000
#define COAP_FLAGS_SUBSCRIBE    0x4000

/*CoAP resource flags*/
#define COAP_RESOURCE_FLAGS_RELEASE_URI 0x1
#define COAP_RESOURCE_FLAGS_NOTIFY_NON  0x0
#define COAP_RESOURCE_FLAGS_NOTIFY_CON  0x2

/* Definitions for Async Status Flags These flags can be used to control the
 * behaviour of asynchronous response generation.
 */
#define COAP_ASYNC_CONFIRM   0x01  /**< send confirmable response */
#define COAP_ASYNC_SEPARATE  0x02  /**< send separate response */
#define COAP_ASYNC_OBSERVED  0x04  /**< the resource is being observed */
/** release application data on destruction */
#define COAP_ASYNC_RELEASE_DATA  0x08

#define COAP_OBSERVE_ESTABLISH 0
#define COAP_OBSERVE_CANCEL 1
#define COAP_OBS_MAX_NON   5
#define COAP_OBS_MAX_FAIL  3

/*CoAP response code*/
typedef enum {
    COAP_RESP_NONE=0,
    COAP_RESP_ACK_ONLY = 0x0001,
    COAP_RESP_ACK_DATA=0x0002,
    COAP_RESP_ACK_SUBSCRIBE = 0x0004,
    COAP_RESP_BLOCK2_MORE = 0x0008,
    COAP_RESP_BLOCK2_LAST = 0x0010,
    COAP_RESP_BLOCK1_MORE= 0x0020,
    COAP_RESP_BLOCK1_LAST = 0x0040,
    COAP_RESP_CLIENT_ERROR = 0x0080,
    COAP_RESP_SERVER_ERROR = 0x0100,
    COAP_RESP_TIMEOUT = 0x0200,
    COAP_RESP_NO_MATCH = 0x8000,
}COAP_RESP_CODE;

enum coap_uri_scheme_t {
  COAP_URI_SCHEME_COAP=0,
  COAP_URI_SCHEME_COAPS=1
};

typedef enum {
    COAP_SSL_NONE,    
    COAP_SSL_HANDSHAKING,
    COAP_SSL_HANDSHAKE_COMPLETE,
}COAP_SSL_STATE;

/** @enum
 * Return status values
 */
typedef enum {
    A_COAP_ERROR = -1,               /* Generic error return */
    A_COAP_OK = 0,                   /* success */
} A_COAP_STATUS;


#define COAP_TOKEN_MAX_LEN 8
#define COAP_MEDIATYPE_ANY   0xff /* any media type */
#define COAP_INVALID_TID -1
#define COAP_DROPPED_RESPONSE -2
#define COAP_TCP_MAX_CLIENT_NUM     5
#define NI_MAXSERV 32
#define NI_MAXHOST 32
#define COAP_MAX_BLOCK_SZX      4
/** The interval in seconds to check if resources have changed. */
#define COAP_RESOURCE_CHECK_TIME 2


/**
 * Abstract handle that is used to identify a local network interface.
 */
typedef int coap_if_handle_t;

typedef unsigned char method_t;
typedef unsigned char coap_key_t[4];
typedef unsigned long coap_tick_t;
typedef int coap_tid_t;
typedef unsigned char coap_opt_t;


typedef struct {
  size_t length;    /* length of string */
  unsigned char *s; /* string data */
} str;

typedef struct {
  unsigned char code;
  char *media_type;
} content_type_t;

typedef struct {
  unsigned short delta;
  size_t length;
  unsigned char *value;
} coap_option_t;

/** multi-purpose address abstraction */
typedef struct coap_address_t {
  socklen_t size;           /**< size of addr */
  union {
    struct sockaddr         sa;
    struct sockaddr_storage st;
    struct sockaddr_in      sin;
    struct sockaddr_in6     sin6;
  } addr;
} coap_address_t;

typedef struct {
  unsigned int num;       /**< block number */
  unsigned int m:1;       /**< 1 if more blocks follow, 0 otherwise */
  unsigned int szx:3;     /**< block size */
} coap_block_t;

typedef struct coap_list_t {
  struct coap_list_t *next;
  char data[];
} coap_list_t;

typedef struct {
  str host;             /**< host part of the URI */
  unsigned short port;  /**< The port in host byte order */
  str path;             /**< Beginning of the first path segment. 
                             Use coap_split_path() to create Uri-Path options */
  str query;            /**<  The query part if present */

  /** The parsed scheme specifier. */
  enum coap_uri_scheme_t scheme;
} coap_uri_t;

typedef struct {
  unsigned int token_length:4; /* length of Token */
  unsigned int type:2;         /* type flag */
  unsigned int version:2;      /* protocol version */
  unsigned int code:8;         /* request method (value 1--10) or response
                                  code (value 40-255) */
  unsigned short id;           /* transaction id (network byte order!) */
  unsigned char token[];       /* the actual token, if any */
} coap_hdr_t;

typedef struct {
  unsigned short key;           /* the option key (no delta coding) */
  unsigned int length;
} coap_option;

#define COAP_OPTION_KEY(option) (option).key
#define COAP_OPTION_LENGTH(option) (option).length
#define COAP_OPTION_DATA(option) ((unsigned char *)&(option) + sizeof(coap_option))
#define COAP_OPTION(node) ((coap_option *)(node)->options)


typedef struct {
  size_t max_size;          /**< allocated storage for options and data */
  coap_hdr_t *hdr;          /**< Address of the first byte of the CoAP message.
                             *   This may or may not equal (coap_hdr_t*)(pdu+1)
                             *   depending on the memory management
                             *   implementation. */
  unsigned short max_delta; /**< highest option number */
  unsigned short length;    /**< PDU length (including header, options, data) */
  unsigned char *data;      /**< payload */
} coap_pdu_t;

typedef struct coap_tcp_server_peer_t{
    int clientFd;
    SSL *ssl;
    int ssl_state;
    coap_address_t peer_addr;
}coap_tcp_server_peer_t;

typedef struct coap_token_t{
    unsigned int length;
    unsigned char s[COAP_TOKEN_MAX_LEN];
}coap_token_t;


#define COAP_OPT_FILTER_SHORT 6
#define COAP_OPT_FILTER_LONG  2
#define COAP_OPT_FILTER_SIZE					\
  (((COAP_OPT_FILTER_SHORT + 1) >> 1) + COAP_OPT_FILTER_LONG) +1
typedef uint16_t coap_opt_filter_t[COAP_OPT_FILTER_SIZE];

typedef struct {
  size_t length;                /**< remaining length of PDU */
  unsigned short type;          /**< decoded option type */
  unsigned int bad:1;           /**< iterator object is ok if not set */
  unsigned int filtered:1;      /**< denotes whether or not filter is used */
  coap_opt_t *next_option;      /**< pointer to the unparsed next option */
  coap_opt_filter_t filter;     /**< option filter */
} coap_opt_iterator_t;

unsigned short coap_opt_length(const coap_opt_t *opt);
#define COAP_OPT_LENGTH(opt) coap_opt_length(opt)

unsigned char *coap_opt_value(coap_opt_t *opt);
#define COAP_OPT_VALUE(opt) coap_opt_value((coap_opt_t *)opt)

/**
 * Returns the value of the least significant byte of a Block option @p opt.
 * For zero-length options (i.e. num == m == szx == 0), COAP_OPT_BLOCK_LAST
 * returns @c NULL.
 */
#define COAP_OPT_BLOCK_LAST(opt) \
  (COAP_OPT_LENGTH(opt) ? (COAP_OPT_VALUE(opt) + (COAP_OPT_LENGTH(opt)-1)) : 0)

/** Returns the value of the More-bit of a Block option @p opt. */
#define COAP_OPT_BLOCK_MORE(opt) \
  (COAP_OPT_LENGTH(opt) ? (*COAP_OPT_BLOCK_LAST(opt) & 0x08) : 0)

/** Returns the value of the SZX-field of a Block option @p opt. */
#define COAP_OPT_BLOCK_SZX(opt)  \
  (COAP_OPT_LENGTH(opt) ? (*COAP_OPT_BLOCK_LAST(opt) & 0x07) : 0)

unsigned int coap_opt_block_num(const coap_opt_t *block_opt);
/** Returns the value of the num of a Block option @p opt. */
#define COAP_OPT_BLOCK_NUM(opt) \
      coap_opt_block_num(opt)


char *coap_response_phrase(unsigned char code);

typedef struct coap_endpoint_t {
  struct coap_endpoint_t *next;
  union {
    int fd;       /**< on POSIX systems */
    void *conn;   /**< opaque connection (e.g. uip_conn in Contiki) */
  } handle;       /**< opaque handle to identify this endpoint */

  struct coap_context_t *context;

  coap_address_t addr; /**< local interface address */
  coap_address_t dst; /**< remote interface address */
  int ifindex;
  int flags;
  int listen_fd;
} coap_endpoint_t;

typedef struct coap_queue_t {
  struct coap_queue_t *next;
  coap_tick_t t;                /**< when to send PDU for the next time */
  unsigned char retransmit_cnt; /**< retransmission counter, will be removed
                                 *    when zero */
  unsigned int timeout;         /**< the randomized timeout value */
  coap_endpoint_t local_if;     /**< the local address interface */
  coap_address_t remote;        /**< remote address */
  coap_tid_t id;                /**< unique transaction id */
  coap_pdu_t *pdu;              /**< the CoAP PDU to send */
} coap_queue_t;

/** Message handler that is used as call-back in coap_context_t */
typedef void (*coap_response_handler_t)(struct coap_context_t  *,
                                        const coap_endpoint_t *local_interface,
                                        const coap_address_t *remote,
                                        coap_pdu_t *sent,
                                        coap_pdu_t *received,
                                        const coap_tid_t id,
                                        unsigned int resp_code);

typedef struct coap_req_t{
    struct coap_req_t *next;
    coap_tid_t tid;
    coap_block_t block;
    unsigned int flags;
    coap_uri_t uri;
    coap_list_t *optlist;
    coap_token_t token;
    str payload;
    method_t method;
    unsigned char msgtype;
    unsigned short reserved;
}coap_req_t;

struct coap_packet_t {
  coap_if_handle_t hnd;	      /**< the interface handle */
  coap_address_t src;	      /**< the packet's source address */
  coap_address_t dst;	      /**< the packet's destination address */
  const coap_endpoint_t *interface;

  int ifindex;
  void *session;		/**< opaque session data */

  size_t length;		/**< length of payload */
  unsigned char payload[];	/**< payload */
};

typedef struct coap_packet_t coap_packet_t;

/** The CoAP stack's global state is stored in a coap_context_t object */
typedef struct coap_context_t {
  coap_opt_filter_t known_options;
  struct coap_resource_t *resources; /**< hash table or list of known resources */

  /* list of asynchronous transactions */
  struct coap_async_state_t *async_state;

  /*The time stamp in the first element of the sendqeue is relative
    * to sendqueue_basetime. */
  coap_tick_t sendqueue_basetime;
  coap_queue_t *sendqueue;
  coap_endpoint_t *endpoint;      /**< the endpoint used for listening  */

  /**
   * The last message id that was used is stored in this field. The initial
   * value is set by coap_new_context() and is usually a random value. A new
   * message id can be created with coap_new_message_id().
   */
  unsigned short message_id;
  unsigned short reserved;

  /**
   * The next value to be used for Observe. This field is global for all
   * resources and will be updated when notifications are created.
   */
  unsigned int observe;

  coap_response_handler_t response_handler;

  int (*network_send)(struct coap_context_t *context,
                          const coap_endpoint_t *local_interface,
                          const coap_address_t *dst,
                          unsigned char *data, size_t datalen);

  int (*network_read)(coap_endpoint_t *ep, coap_packet_t **packet);

  SSL_CTX *sslCtx;
  SSL*  ssl; 
  coap_req_t request;
  coap_tcp_server_peer_t peer_info[COAP_TCP_MAX_CLIENT_NUM]; //for tcp server
  int peer_index;
  void *coap_config;      /*the other config which could be defined by users*/
} coap_context_t;

//uthash.h
typedef struct UT_hash_bucket {
   struct UT_hash_handle *hh_head;
   unsigned count;
   unsigned expand_mult;

} UT_hash_bucket;

typedef struct UT_hash_table {
   UT_hash_bucket *buckets;
   unsigned num_buckets, log2_num_buckets;
   unsigned num_items;
   struct UT_hash_handle *tail; /* tail hh in app order, for fast append    */
   ptrdiff_t hho; /* hash handle offset (byte pos of hash handle in element */

   /* in an ideal situation (all buckets used equally), no bucket would have
    * more than ceil(#items/#buckets) items. that's the ideal chain length. */
   unsigned ideal_chain_maxlen;

   /* nonideal_items is the number of items in the hash whose chain position
    * exceeds the ideal chain maxlen. these items pay the penalty for an uneven
    * hash distribution; reaching them in a chain traversal takes >ideal steps */
   unsigned nonideal_items;

   /* ineffective expands occur when a bucket doubling was performed, but
    * afterward, more than half the items in the hash had nonideal chain
    * positions. If this happens on two consecutive expansions we inhibit any
    * further expansion, as it's not helping; this happens when the hash
    * function isn't a good fit for the key domain. When expansion is inhibited
    * the hash will still work, albeit no longer in constant time. */
   unsigned ineff_expands, noexpand;

   uint32_t signature; /* used only to find hash tables in external analysis */
} UT_hash_table;

typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;                       /* prev element in app order      */
   void *next;                       /* next element in app order      */
   struct UT_hash_handle *hh_prev;   /* previous hh in bucket order    */
   struct UT_hash_handle *hh_next;   /* next hh in bucket order        */
   void *key;                        /* ptr to enclosing struct's key  */
   unsigned keylen;                  /* enclosing struct's key len     */
   unsigned hashv;                   /* result of hash-fcn(key)        */
} UT_hash_handle;


/** Subscriber information */
typedef struct coap_subscription_t {
  struct coap_subscription_t *next; /**< next element in linked list */
  coap_endpoint_t local_if;         /**< local communication interface */
  coap_address_t subscriber;        /**< address and port of subscriber */

  unsigned int non_cnt:4;  /**< up to 15 non-confirmable notifies allowed */
  unsigned int fail_cnt:2; /**< up to 3 confirmable notifies can fail */
  unsigned int dirty:1;    /**< set if the notification temporarily could not be
                            *   sent (in that case, the resource's partially
                            *   dirty flag is set too) */
  size_t token_length;     /**< actual length of token */
  unsigned char token[8];  /**< token used for subscription */
  coap_pdu_t *request;  /**< copy of subscription's request */
} coap_subscription_t;

typedef void (*coap_method_handler_t)
  (coap_context_t  *,
   struct coap_resource_t *,
   const coap_endpoint_t *,
   coap_address_t *,
   coap_pdu_t *,
   str * /* token */,
   coap_pdu_t * /* response */);

typedef struct coap_attr_t {
  struct coap_attr_t *next;
  str name;
  str value;
  int flags;
} coap_attr_t;

typedef struct coap_resource_t {
  unsigned int dirty:1;          /**< set to 1 if resource has changed */
  unsigned int partiallydirty:1; /**< set to 1 if some subscribers have not yet
                                  *   been notified of the last change */
  unsigned int observable:1;     /**< can be observed */
  unsigned int cacheable:1;      /**< can be cached */

  /**
   * Used to store handlers for the four coap methods @c GET, @c POST, @c PUT,
   * and @c DELETE. coap_dispatch() will pass incoming requests to the handler
   * that corresponds to its request method or generate a 4.05 response if no
   * handler is available.
   */
  coap_method_handler_t handler[4];

  coap_key_t key;                /**< the actual key bytes for this resource */

  UT_hash_handle hh;

  coap_attr_t *link_attr; /**< attributes to be included with the link format */
  coap_subscription_t *subscribers;  /**< list of observers for this resource */

  /**
   * Request URI for this resource. This field will point into the static
   * memory.
   */
  str uri;
  int flags;

} coap_resource_t;

typedef enum {
  LOG_NONE=0,
  LOG_ALERT,
  LOG_WARNING,
  LOG_DEBUG
} coap_log_t;

extern int app_printf(const char * format, ...);

#define COAP_ALERT(fmt,args...) do{if(qcom_coap_get_log_level()>=LOG_ALERT) {app_printf(fmt,##args);}}while(0)
#define COAP_WARNING(fmt,args...) do{if(qcom_coap_get_log_level()>=LOG_WARNING) {app_printf(fmt,##args);}}while(0)
#define COAP_DEBUG(fmt,args...) do{if(qcom_coap_get_log_level()>=LOG_DEBUG) {app_printf(fmt,##args);}}while(0)

//async.h

typedef struct coap_async_state_t {
  unsigned char flags;  /**< holds the flags to control behaviour */

  /**
   * Holds the internal time when the object was registered with a
   * resource. This field will be updated whenever
   * coap_register_async() is called for a specific resource.
   */
  coap_tick_t created;

  /**
   * This field can be used to register opaque application data with the
   * asynchronous state object.
   */
  void *appdata;
  unsigned short message_id;       /**< id of last message seen */
  coap_tid_t id;                   /**< transaction id */
  struct coap_async_state_t *next; /**< internally used for linking */
  coap_address_t peer;             /**< the peer to notify */
  size_t tokenlen;                 /**< length of the token */
  unsigned char token[];           /**< the token to use in a response */
} coap_async_state_t;


void
qcom_coap_ticks(coap_tick_t *t);

coap_log_t 
qcom_coap_get_log_level(void);

void
qcom_coap_set_log_level(coap_log_t level);

int
qcom_coap_uri_scheme_is_secure(const coap_uri_t *uri);

coap_context_t *
qcom_coap_new_context();

coap_list_t *
qcom_coap_new_option(unsigned short key, unsigned int length, unsigned char *data);

int
qcom_coap_insert_option(coap_list_t **head, coap_list_t *node);

size_t
qcom_coap_add_option(coap_pdu_t *pdu, unsigned short type, unsigned int len, const unsigned char *data);

unsigned int
qcom_coap_encode_var_bytes(unsigned char *buf, unsigned int val);

coap_opt_t *
qcom_coap_check_option(coap_pdu_t *pdu, unsigned short type, coap_opt_iterator_t *oi);

int
qcom_coap_resolve_address(const str *server, struct coap_address_t *dst, int v6_enabled);

int
qcom_coap_add_data(coap_pdu_t *pdu, unsigned int len, const unsigned char *data);

int 
qcom_coap_add_block(coap_pdu_t *pdu, unsigned int len, const unsigned char *data, unsigned int block_num, unsigned char block_szx);

int
qcom_coap_get_data(coap_pdu_t *pdu, size_t *len, unsigned char **data);

int 
qcom_coap_fls(unsigned int i);

coap_tid_t
qcom_coap_send_confirmed(coap_context_t *context, const coap_endpoint_t *local_interface, const coap_address_t *dst, coap_pdu_t *pdu);

coap_tid_t 
qcom_coap_send(coap_context_t *context, const coap_endpoint_t *local_interface, const coap_address_t *dst, coap_pdu_t *pdu);

coap_pdu_t *
qcom_coap_pdu_init(unsigned char type, unsigned char code, unsigned short id, unsigned int size);

void
qcom_coap_pdu_clear(coap_pdu_t *pdu, unsigned int size);

void
qcom_coap_delete_pdu(coap_pdu_t *pdu);

int
qcom_coap_run_once(coap_context_t *ctx, unsigned int timeout_ms);

void
qcom_coap_free_context(coap_context_t *context);

void
qcom_coap_parse_uri(char *arg, coap_uri_t *uri) ;

void
qcom_coap_add_uri(coap_uri_t *uri, coap_list_t **optlist);

void
qcom_coap_register_option(coap_context_t *ctx, unsigned char type);

void
qcom_coap_set_blocksize(method_t method, coap_block_t *block, str *payload, coap_list_t **optlist);

coap_pdu_t *
qcom_coap_new_request(coap_context_t *ctx,
                 method_t m,
                 unsigned char msgtype,
                 coap_token_t *token,
                 coap_list_t **options);

int
qcom_coap_request_payload(coap_context_t *ctx,
                 coap_pdu_t *pdu,
                 unsigned char *data,
                 size_t length,
                 coap_block_t *block,
                unsigned int flags);


int
qcom_coap_client_can_exit( coap_context_t *context );

coap_tid_t
qcom_coap_clear_obs(coap_context_t *ctx,
          const coap_endpoint_t *local_interface,
          const coap_address_t *remote,
          unsigned char msg_type,
          coap_token_t *token,
          coap_list_t *optlist);

void
qcom_coap_delete_list(coap_list_t **queue);

A_COAP_STATUS
qcom_coap_conext_init(coap_context_t *ctx, coap_address_t *addr, unsigned int flags, coap_response_handler_t handler);

int
qcom_coap_client_connect(coap_context_t *ctx, SSL_CTX* sslCtx, coap_address_t* dst, unsigned int flags);


coap_resource_t *
qcom_coap_resource_init(const unsigned char *uri, size_t len, int flags);

void
qcom_coap_register_handler(coap_resource_t *resource, unsigned char method, coap_method_handler_t handler);

coap_attr_t *
qcom_coap_add_attr(coap_resource_t *resource, const unsigned char *name, size_t nlen, const unsigned char *val, size_t vlen, int flags);

void
qcom_coap_add_resource(coap_context_t *context, coap_resource_t *resource);

int
qcom_coap_server_setup(coap_context_t *ctx, SSL_CTX* sslCtx, char* group, unsigned int flags);

void
qcom_coap_check_async(coap_context_t *ctx,  const coap_endpoint_t *local_if);

void
qcom_coap_check_notify(coap_context_t *context);

coap_subscription_t *
qcom_coap_find_observer(coap_resource_t *resource, const coap_address_t *peer, const str *token);

coap_async_state_t *
qcom_coap_register_async(coap_context_t *context, coap_address_t *peer, coap_pdu_t *request, unsigned char flags, void *data);

int
qcom_coap_remove_async(coap_context_t *context, coap_tid_t id, coap_async_state_t **s);

void 
qcom_coap_free_async(coap_async_state_t *s);
#endif
