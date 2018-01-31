/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#ifndef __QCOM_NETWORK__
#define __QCOM_NETWORK__

#include <threadx/tx_api.h>
#include <qcom/qcom_ota_api.h>

#define NET_WORK_DEBUG_EN 0
#define HTTPC_MULTI_SESSION 1

/* HTTP Server socket command MACROS*/
#define HTTP_UNKNOWN		-1
#define HTTP_SERVER_STOP    0
#define HTTP_SERVER_START   1
#define HTTPS_SERVER_STOP   2
#define HTTPS_SERVER_START  3
#define HTTP_REDIRECTED_HTML_MAX_SIZE 1024

/* HTTP Server Get/Post/PUT/DELETE socket MACROS */
#define HTTP_GET_METHOD     0
#define HTTP_POST_METHOD    1
#define HTTP_PUT_METHOD		2
#define HTTP_DELETE_METHOD	3
#define HTTP_HEAD_METHOD	4

/*http client supported 4 sessions in default*/
#define DEFAULT_CLIENT_NUM_SUPPORTED 4
#define HTTPC_REQ_WAIT_FOREVER       0

#define ATH_MAX_ROUTES 3
#define IPV6_MAX_ROUTES 3

#define GET_TLV_TYPE(x,y)    (A_MEMCPY(&(y),(x),sizeof(A_UINT16)))
#define GET_TLV_LENGTH(x,y)  (A_MEMCPY(&(y),(x+2),sizeof(A_UINT16)))
#define GET_NEXT_TLV(x,y)    ((x)+ sizeof(A_UINT16) + sizeof(A_UINT16) + (y))
#define GET_TLV_VAL(x)       ((x)+ sizeof(A_UINT16) + sizeof(A_UINT16))

enum {
	/* Host command macros */
	HTTPC_CONNECT_CMD,
	HTTPC_GET_CMD,
	HTTPC_POST_CMD,
	HTTPC_DATA_CMD,
	HTTPC_DISCONNECT_CMD,
	HTTPC_CONNECT_SSL_CMD,
	HTTPC_HEADER_CMD,
	HTTPC_CLEAR_HEADER_CMD,
	HTTPC_BODY_CMD,
	HTTPC_PUT_CMD,
	HTTPC_PATCH_CMD
};

typedef struct IPv4Route {
A_UINT32 reserved;
A_UINT32 address;
A_UINT32 mask;
A_UINT32 gateway;
A_UINT32 ifIndex;
A_UINT32 prot;
} IPV4_ROUTE_T;

typedef struct IPv4RouteLists {
    A_UINT32 rtcount;
    IPV4_ROUTE_T route[ATH_MAX_ROUTES];
}IPV4_ROUTE_LIST_T;

typedef PREPACK struct {
    A_UINT32    resp_type ;
    A_UINT32    sock_handle ; //handle to open socket
    A_UINT32    result ;
    A_UINT32    rt_count;
    IPV4_ROUTE_T  route[ATH_MAX_ROUTES] ;
} POSTPACK WMI_IP4ROUTE_RESPONSE_EVENT;

enum{
    IPV6_ROUTE_ADD,
    IPV6_ROUTE_DEL,
    IPV6_ROUTE_SHOW,
    IPV6_ROUTEPREFIX_SHOW
};

typedef enum route_enum {
    ROUTE_ADD,
    ROUTE_DEL,
    ROUTE_SHOW,
    ROUTE_MAX
} ip_route;

typedef  struct sock_ipv6_route {
    A_UINT32 command ;
    A_UINT8  address[16] ;
    A_INT32  prefixlen ;
    A_UINT8  nexthop[16] ;
    A_UINT32 ifindex ;
    A_UINT32 prot ;
} IPV6_ROUTE_T;

typedef struct IPv6RouteLists {
    A_UINT32 rtcount;
    IPV6_ROUTE_T route[ATH_MAX_ROUTES];
}IPV6_ROUTE_LIST_T;

typedef  struct {
    A_UINT32    resp_type ;
    A_UINT32    sock_handle ; //handle to open socket
    A_UINT32    result ;
    A_UINT32    rt_count;
    IPV6_ROUTE_T  route[IPV6_MAX_ROUTES] ;
}  WMI_IP6ROUTE_RESPONSE_EVENT;

enum WI_HTTP_EVENT_TLV_TYPE{
    HTTP_TYPE_URI,
    HTTP_TYPE_NAME,
    HTTP_TYPE_VALUE,
    HTTP_TYPE_SESS_INDEX
};

typedef struct {
    A_UINT32 numTLV;   /*Number of TLVs encapsulated in the event*/
    A_UINT8 data[0];   /*Start of TLV data*/
}HTTP_EVENT_T;

typedef struct DnsSvrAddr
{
   A_UINT32 type ;
   A_UINT32 addr4 ;
   A_UINT8   addr6[16] ;
}  DnsSvrAddr_t;

typedef PREPACK struct {
    A_UINT32    length;
    A_UINT32    resp_code;
    A_UINT32    more;
    A_UINT8     data[1];
} POSTPACK HTTPC_RESPONSE;

/* Responds for the http client command*/
typedef PREPACK struct {
	A_UINT32    client;
    A_UINT32    sess;
    A_UINT32    length;
    A_UINT32    resp_code;
    A_UINT32    more;
    A_UINT8     data[1];;
} POSTPACK HTTPC_MULTI_SESSION_RESPONSE;


typedef void (*http_client_cb_t)(void* arg, A_INT32 state, void* value);

/* http client callback event */
typedef PREPACK struct HTTPC_CALLBACK_EVENT_S {
	struct HTTPC_CALLBACK_EVENT_S* next;
	A_UINT32 client;
    A_INT32  state;
    void*    value;
} POSTPACK HTTPC_CALLBACK_EVENT;

typedef enum
{
	HTTP_CLIENT_API_UNDEFINE_VERSION = 0,
	HTTP_CLIENT_API_SINGLE_VERSION = 1,
	HTTP_CLIENT_API_MULTI_VERSION = 2,
	HTTP_CLIENT_API_MAX_VERSION,
} HTTP_CLIENT_API_VERSION;

typedef enum {
	/*supported http client methods */
	HTTP_CLIENT_GET_CMD = 20,
	HTTP_CLIENT_POST_CMD,
	HTTP_CLIENT_PUT_CMD,
	HTTP_CLIENT_PATCH_CMD
} HTTPC_REQUEST_CMD_E;

#define HTTPCLIENT_MAX_SERVER_LENGTH 64

#define HTTPCLIENT_MAX_URL_LENGTH 256

#define HTTPCLIENT_MAX_HEADER_LENGTH 256

#define HTTPCLIENT_MAX_PARAM_LENGTH 256


#define HTTPC_SESS_CREATING 0xFFFFFFFF
#define HTTPC_REQ_WAIT_ON_QUEUE_MAX_TIMEOUT 60

#define SET_CMD_PENDING(client, cmd)   ((client)->pending.cmd=0x1)
#define CLEAR_CMD_PENDING(client, cmd) ((client)->pending.cmd=0x0)
#define CMD_IS_PENDING(client, cmd)    ((client)->pending.cmd&0x1)

/*	bitmap used to indicate whether cmd is pending on queue*/
typedef struct cmd_pending_bitfiled {
    A_UINT32 connect:1;
    A_UINT32 disconnect:1;
    A_UINT32 request:1;
    A_UINT32 setbody:1;
    A_UINT32 addheader:1;
    A_UINT32 clearheader:1;
    A_UINT32 setparam:1;
    A_UINT32  resv:25;
} PENDING_T;

typedef struct srver_info
{
    A_UINT16 port;
	A_CHAR server[HTTPCLIENT_MAX_SERVER_LENGTH];
} server_info_t;

/*define http client structure*/
typedef struct http_client_s {
    A_UINT32 sess;
    TX_QUEUE queue;
    PENDING_T pending;
    A_INT32 result;
    //server_info_t* server_info;	
	void* ssl_ctx;
    A_UINT32 timeout;
    http_client_cb_t callback;
    void * arg;			
    A_UINT32 rsv;
} HCLIENT_T;


#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct  {
	void
	(* _qcom_ipconfig)(A_UINT8 device_id, int mode, A_UINT32 * ip_address, A_UINT32 * sn_mask, A_UINT32 * gateway);
	struct hostent *
	(* _qcom_dns_get)(A_UINT32 mode, A_UINT32 domain, char *hostname);
	A_STATUS
	(* _qcom_ip_address_set)(A_UINT8 device_id, A_UINT32 address, A_UINT32 submask, A_UINT32 gateway);
	A_STATUS
	(* _qcom_ip_address_get)(A_UINT8 device_id, A_UINT32 *paddress, A_UINT32 *psubmask, A_UINT32 *pgateway);
#if 0 
	A_STATUS
	(* _qcom_ip6_address_get)(A_UINT8 device_id, IP6_ADDR_T *v6Global, IP6_ADDR_T *v6Link, IP6_ADDR_T *v6DefGw,IP6_ADDR_T *v6GlobalExtd, A_INT32 *LinkPrefix,       A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#else
	A_STATUS
	(* _qcom_ip6_address_get)(A_UINT8 device_id, A_UINT8 *v6Global, A_UINT8 *v6Link, A_UINT8 *v6DefGw,A_UINT8 *v6GlobalExtd, A_INT32 *LinkPrefix,       A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#endif

	A_STATUS
	(* _qcom_dns_server_address_get)(DnsSvrAddr_t pdns[], A_UINT32* number);
	A_STATUS
	(* _qcom_ip_ping)(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
    A_STATUS
	(* _qcom_ip_ping6)(A_UINT8* host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
	A_STATUS (* _qcom_ip6config_router_prefix)(A_UINT8 device_id, A_UINT8 *addr,A_INT32 prefix_length,A_INT32 prefix_lifetime,A_INT32 valid_lifetime);
	A_STATUS
	(* _qcom_dhcpc_enable)(A_UINT8 device_id, A_BOOL enable);

	A_STATUS
	(* _qcom_dhcps_enable)(A_UINT8 device_id, A_BOOL enable);
	A_STATUS
	(* _qcom_dhcps_set_pool)(A_UINT8 device_id, A_UINT32 startip, A_UINT32 endip, A_UINT32 leasetime);
	A_STATUS
	(* _qcom_dhcps_get_pool)(A_UINT8 device_id, A_UINT32 *pstartip, A_UINT32 *pendip, A_UINT32 *pleasetime);
    A_STATUS (*_qcom_dhcps_release_pool)(A_UINT8 device_id);
    A_STATUS (*_qcom_tcp_set_exp_backoff)(A_UINT8 device_id, A_INT32 retry);

//#ifdef BRIDGE_ENABLED
	A_STATUS
	(* _qcom_bridge_mode_enable)(A_UINT16 bridgemode);
//#endif
	A_STATUS
	(* _qcom_dnsc_enable)(A_BOOL enable);
	A_STATUS
	(* _qcom_dnsc_add_server_address)(A_UINT8 *ipaddress, A_UINT8 type);
	A_STATUS
	(* _qcom_dnsc_del_server_address)(A_UINT8 *ipaddress, A_UINT8 type);
	A_STATUS
	(* _qcom_dnsc_get_host_by_name)(A_CHAR *pname, A_UINT32 *pipaddress);
	A_STATUS

	(* _qcom_dnss_enable)(A_BOOL enable);
	A_STATUS
	(* _qcom_dns_local_domain)(A_CHAR* local_domain);
	A_STATUS
	(* _qcom_dns_entry_create)(A_CHAR* hostname, A_UINT32 ipaddress);
	A_STATUS
	(* _qcom_dns_entry_delete)(A_CHAR* hostname, A_UINT32 ipaddress);
	A_STATUS
	(* _qcom_dns_6entry_create)(A_CHAR* hostname, A_UINT8* ip6addr);

	A_STATUS
	(* _qcom_dns_6entry_delete)(A_CHAR* hostname, A_UINT8* ip6addr);
	A_STATUS
	(* _qcom_dnsc_get_host_by_name2)(A_CHAR *pname, A_UINT32 *pipaddress, A_INT32 domain, A_UINT32 mode);
//#if defined HTTP_ENABLED
	A_STATUS
	(* _qcom_http_server)(A_INT32 enable, void *ctx);
	A_STATUS
	(* _qcom_http_server_method)(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8 *value);
	A_STATUS
	(* _qcom_http_client_method)(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, A_UINT8 *value, void *ctx);
//#endif

    A_STATUS (*_qcom_ping)(A_UINT32 host, A_UINT32 size);
    A_STATUS (*_qcom_ping6)(A_UINT8* host, A_UINT32 size);
    A_STATUS (*_qcom_ip4_route)(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
    A_STATUS (*_qcom_ip6_route)(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,IPV6_ROUTE_LIST_T* rtlist);
    A_STATUS (*_qcom_ota_upgrade)(A_UINT8 device_id,A_UINT32 ip_addr, const A_CHAR *source,A_UINT8 mode,A_UINT8 preserve_last,A_UINT8 protocol,A_UINT32 *resp_code,A_UINT32 *length);

    A_STATUS (*_qcom_read_ota_area)(A_UINT32 offset,A_UINT32 size,A_UINT8 *buffer,A_UINT32 *ret_len);
    A_STATUS (*_qcom_ota_done)(A_UINT32 good_image);
    A_STATUS (*_qcom_set_hostname)(A_UINT8 device_id,A_CHAR *pstr);
	void     (*_qcom_wmi_event_socket_process)(void *pevent_buff);
    A_STATUS (*_qcom_tcp_conn_timeout)(A_UINT32 timeout_val);
    
	//TODO
    //A_STATUS (*_qcom_set_ipv6_status)(A_UINT8 device_id,A_UINT8 flag);
    A_STATUS (*_qcom_http_set_post_cb)(void* cxt, void *callback);
    A_STATUS (*_qcom_dhcps_register_cb)(A_UINT8 device_id,void *callback);
    A_STATUS (*_qcom_dhcpc_register_cb)(A_UINT8 device_id,void *callback);
    A_STATUS (*_qcom_http_set_put_cb)(void* cxt, void *callback);
    A_STATUS (*_qcom_http_set_delete_cb)(void* cxt, void *callback);
    
#if !defined(AR6002_REV76)
    A_STATUS (*_qcom_autoip_register_cb)(A_UINT8 device_id,void *callback);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_session_start)(A_UINT32 flags, A_UINT32 partition_index);	
    A_UINT32 (*_qcom_ota_partition_get_size)(void);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_erase)();
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_erase_sectors)(A_UINT32 offset_next);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_parse_image_hdr)(A_UINT8 *header,A_UINT32 *offset);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_verify_checksum)(void);
    A_INT32 (*_qcom_ota_partition_read_data)(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_write_data)(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size,A_UINT32 *ret_size);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_session_end)(A_UINT32 good_image);
    QCOM_OTA_STATUS_CODE_t (*_qcom_ota_partition_format)(A_UINT32 partition_index);
    A_STATUS (*_qcom_ssdp_init)(A_UINT32 notifyPeriod,A_UINT32 localIpAddr,A_UCHAR *locationStr,
                                    A_UCHAR *serverStr, A_UCHAR *searchTargetStr,
                                    A_UCHAR *domainName, A_UCHAR *serviceType,
                                    A_UCHAR *deviceType, A_UCHAR *uuidStr);
    A_STATUS (*_qcom_ssdp_enable)(A_UINT32 enable);
    A_STATUS (*_qcom_ssdp_notify_change)(A_UINT32 notifyPeriod);    

    A_STATUS (*_qcom_http_set_get_cb)(void* cxt, void *callback);    
    A_STATUS (*_qcom_http_get_datasend)(A_UINT16 sess_index, A_UINT16 data_len, 
									    A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
    A_STATUS (*_qcom_set_http_custom_uri)(A_UINT8 device_id, const char * const *http_custom_uri);
    A_STATUS (*_qcom_set_httpsvr_ota_uri)(A_UINT8 device_id, const char * ota_uri);
    A_STATUS (*_qcom_httpsvr_ota_enable)(A_UINT8 device_id, A_INT32 enable);
    A_STATUS (*_qcom_httpsvr_ota_register_cb)(void *callback);
    A_STATUS (*_qcom_http_collect_scan_result)(A_UINT8 device_id, A_UINT8 **http_data, A_UINT16 *data_len);
    A_STATUS (*_qcom_http_server_add_redirected_page)(A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
    A_STATUS (*_qcom_restrict_http_request)(A_UINT8 device_id, A_UINT8 enable);
    A_STATUS (*_qcom_http_redirect_unknown_url_enable)(A_UINT8 device_id, A_UINT8 enable);
    A_STATUS (*_qcom_http_set_redirected_url)(A_UINT8 device_id, char * url);
    A_INT32 (*_qcom_get_autoip_status)(A_UINT8 device_id, A_UINT8 iface);
    void (*_qcom_dnc_clear)(A_UINT8 device_id);

    void (*_qcom_network_http_server_get_CB*)(void** ctx0, void** ctx1, void** ctx2, 
                                        void **post, void **put, void **del);
    void (*_qcom_network_http_server_set_CB*)(void* ctx0, void* ctx1, void* ctx2, 
                                        void *post, void *put, void *del);

    A_STATUS (*_qcom_http_client_body)(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
    A_STATUS (*_qcom_dhcpc_set_timeout)(A_UINT8 device_id,A_UINT32 timeout); 
	A_STATUS (*_qcom_http_client_register_cb)(http_client_cb_t callback, void* arg);
#ifdef HTTPC_MULTI_SESSION
	A_STATUS (*_http_client_api_check_version)(HTTP_CLIENT_API_VERSION version);
	A_STATUS (*_http_client_check_handler)(A_INT32 client, A_INT32 *index);
    A_INT32 (*_qcom_http_client_connect)(const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, http_client_cb_t callback, void* arg);
    A_STATUS (*_qcom_http_client_disconnect)(A_INT32 client);
    A_STATUS (*_qcom_http_client_request)(A_INT32 client, HTTPC_REQUEST_CMD_E cmd, const A_CHAR *url, A_CHAR *output);
    A_STATUS (*_qcom_http_client_set_body)(A_INT32 client, const A_CHAR *body, A_UINT32 bodylen);
    A_STATUS (*_qcom_http_client_set_param)(A_INT32 client, const A_CHAR *key, const A_CHAR *value);
    A_STATUS (*_qcom_http_client_add_header)(A_INT32 client, const A_CHAR *header, const A_CHAR *value);
    A_STATUS (*_qcom_http_client_clear_header)(A_INT32 client);
#endif
#endif	
}QCOM_NETWORK_API_INDIRECTION_TABLE;

extern QCOM_NETWORK_API_INDIRECTION_TABLE qcomNetworkApiIndirectionTable;
#define QCOM_NETWORK_API_FN(fn) qcomNetworkApiIndirectionTable.fn

#else

void
_qcom_ipconfig(A_UINT8 device_id, int mode, A_UINT32 * ip_address, A_UINT32 * sn_mask, A_UINT32 * gateway);
struct hostent *
_qcom_dns_get(A_UINT32 mode, A_UINT32 domain, char *hostname);
A_STATUS
_qcom_ip_address_set(A_UINT8 device_id, A_UINT32 address, A_UINT32 submask, A_UINT32 gateway);
A_STATUS
_qcom_ip_address_get(A_UINT8 device_id, A_UINT32 *paddress, A_UINT32 *psubmask, A_UINT32 *pgateway);
#if 1
#if 0
A_STATUS
_qcom_ip6_address_get(A_UINT8 device_id, IP6_ADDR_T *v6Global, IP6_ADDR_T *v6Link, IP6_ADDR_T *v6DefGw,IP6_ADDR_T *v6GlobalExtd, A_INT32 *LinkPrefix,
		      A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);
#else
A_STATUS
_qcom_ip6_address_get(A_UINT8 device_id, A_UINT8 *v6Global, A_UINT8 *v6Link, A_UINT8 *v6DefGw,A_UINT8 *v6GlobalExtd, A_INT32 *LinkPrefix,
		      A_INT32 *GlbPrefix, A_INT32 *DefgwPrefix, A_INT32 *GlbPrefixExtd);

#endif

#endif

A_STATUS
_qcom_dns_server_address_get(DnsSvrAddr_t pdns[], A_UINT32* number);
A_STATUS
_qcom_ip_ping(A_UINT32 host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
#if 1 
A_STATUS
_qcom_ip_ping6(A_UINT8* host, A_UINT32 size, A_UINT32 count, A_UINT32 interval);
A_STATUS _qcom_ip6config_router_prefix(A_UINT8 device_id, A_UINT8 *addr,A_INT32 prefix_length,A_INT32 prefix_lifetime,A_INT32 valid_lifetime);
#endif
A_STATUS
_qcom_dhcpc_enable(A_UINT8 device_id, A_BOOL enable);

A_STATUS
_qcom_dhcps_enable(A_UINT8 device_id, A_BOOL enable);
A_STATUS
_qcom_dhcps_set_pool(A_UINT8 device_id, A_UINT32 startip, A_UINT32 endip, A_UINT32 leasetime);
A_STATUS
_qcom_dhcps_get_pool(A_UINT8 device_id, A_UINT32 *pstartip, A_UINT32 *pendip, A_UINT32 *pleasetime);
#ifdef BRIDGE_ENABLED
A_STATUS
_qcom_bridge_mode_enable(A_UINT16 bridgemode);
#endif
A_STATUS
_qcom_dnsc_enable(A_BOOL enable);

A_STATUS
_qcom_dnsc_add_server_address(A_UINT8 *ipaddress, A_UINT8 type);
A_STATUS
_qcom_dnsc_del_server_address(A_UINT8 *ipaddress, A_UINT8 type);
A_STATUS
_qcom_dnsc_get_host_by_name(A_CHAR *pname, A_UINT32 *pipaddress);
A_STATUS
_qcom_dnss_enable(A_BOOL enable);
A_STATUS
_qcom_dns_local_domain(A_CHAR* local_domain);

A_STATUS
_qcom_dns_entry_create(A_CHAR* hostname, A_UINT32 ipaddress);
A_STATUS
_qcom_dns_entry_delete(A_CHAR* hostname, A_UINT32 ipaddress);
A_STATUS
_qcom_dns_6entry_create(A_CHAR* hostname, A_UINT8* ip6addr);
A_STATUS
_qcom_dns_6entry_delete(A_CHAR* hostname, A_UINT8* ip6addr);
A_STATUS
_qcom_dnsc_get_host_by_name2(A_CHAR *pname, A_UINT32 *pipaddress, A_INT32 domain, A_UINT32 mode);

#if defined HTTP_ENABLED
A_STATUS
_qcom_http_server(A_INT32 enable, void *ctx);
        
A_STATUS
_qcom_http_server_method(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8 *value);
A_STATUS
_qcom_http_client_method(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, A_UINT8 *value, void *ctx);
#endif
void
_qcom_wmi_event_socket_process(void *pevent_buff);

A_STATUS _qcom_ping(A_UINT32 host, A_UINT32 size);
A_STATUS _qcom_ping6(A_UINT8* host, A_UINT32 size);
A_STATUS _qcom_dhcps_release_pool(A_UINT8 device_id);
A_STATUS _qcom_tcp_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);

A_STATUS _qcom_ip4_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ip6_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,
        IPV6_ROUTE_LIST_T* rtlist);

A_STATUS _qcom_ota_upgrade(A_UINT8 device_id,A_UINT32 ,const A_CHAR *source,
		A_UINT8 mode, A_UINT8 preserve_last, A_UINT8 protocol,
		A_UINT32 *resp_code, A_UINT32 *length);
A_STATUS _qcom_read_ota_area(A_UINT32 offset,A_UINT32 size, A_UINT8 *buffer,A_UINT32 *ret_len);
A_STATUS _qcom_ota_done(A_UINT32 good_image);
A_STATUS _qcom_set_hostname(A_UINT8 device_id,A_CHAR* pstr);
A_STATUS _qcom_tcp_conn_timeout(A_UINT32 timeout_val);
A_STATUS _qcom_http_set_post_cb(void* cxt, void *callback);
A_STATUS _qcom_http_set_put_cb(void* cxt, void *callback);
A_STATUS _qcom_http_set_delete_cb(void* cxt, void *callback);
A_STATUS _qcom_dhcps_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_dhcpc_register_cb(A_UINT8 device_id,void *callback);
A_STATUS _qcom_autoip_register_cb(A_UINT8 device_id,void *callback);
QCOM_OTA_STATUS_CODE_t _qcom_ota_session_start(A_UINT32 flags, A_UINT32 partition_index);
A_UINT32 _qcom_ota_partition_get_size(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase();
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase_sectors(A_UINT32 offset_next);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_verify_checksum(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_parse_image_hdr(A_UINT8 *header,A_UINT32 *offset);
A_INT32 _qcom_ota_partition_read_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_write_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size,A_UINT32 *ret_size);
QCOM_OTA_STATUS_CODE_t  _qcom_ota_session_end(A_UINT32 good_image);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_format(A_UINT32 partition_index);
A_STATUS _qcom_ssdp_init(A_UINT32 notifyPeriod,A_UINT32 localIpAddr,A_UCHAR *locationStr,
                                    A_UCHAR *serverStr, A_UCHAR *searchTargetStr,
                                    A_UCHAR *domainName, A_UCHAR *serviceType,
                                    A_UCHAR *deviceType, A_UCHAR *uuidStr);
A_STATUS _qcom_ssdp_enable(A_UINT32 enable);
A_STATUS _qcom_ssdp_notify_change(A_UINT32 notifyPeriod);
A_STATUS _qcom_http_get_datasend(A_UINT16 sess_index, A_UINT16 data_len, 
                                    A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
A_STATUS _qcom_set_http_custom_uri(A_UINT8 device_id, const char* const *http_custom_uri);
A_STATUS _qcom_set_httpsvr_ota_uri(A_UINT8 device_id, const char* ota_uri);
A_STATUS _qcom_httpsvr_ota_enable(A_UINT8 device_id, A_INT32 enable);
A_STATUS _qcom_httpsvr_ota_register_cb(void *callback);
A_STATUS _qcom_http_collect_scan_result(A_UINT8 device_id, A_UINT8 **http_data, A_UINT16 *data_len);
A_STATUS _qcom_http_client_body(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
A_STATUS _qcom_dhcpc_set_timeout(A_UINT8 device_id,A_UINT32 timeout);
A_STATUS _qcom_http_server_add_redirected_page(A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
A_STATUS _qcom_restrict_http_request(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_http_redirect_unknown_url_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_http_set_redirected_url(A_UINT8 device_id, char * url);
A_INT32 _qcom_get_autoip_status(A_UINT8 device_id, A_UINT8 iface);
void _qcom_dnc_clear(A_UINT8 device_id);

#define QCOM_NETWORK_API_FN(fn) fn


#endif

A_STATUS _qcom_set_ping_id(A_UINT32 PingId);

#define  qcom_set_ping_id(PingId)\
    _qcom_set_ping_id((PingId))

A_STATUS _qcom_dns_set_timeout(int timeout);

#define  qcom_dns_set_timeout(timeout)\
    _qcom_dns_set_timeout((timeout))
	

#define qcom_ipconfig(id, mode, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ipconfig((id), (mode), (ip), (mask), (gate)))

#define qcom_dns_get(mode, domain, name) \
	QCOM_NETWORK_API_FN(_qcom_dns_get((mode), (domain), (name)))

#define qcom_ip_address_set(id, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ip_address_set((id), (ip), (mask), (gate)))

#define qcom_ip_address_get(id, ip, mask, gate) \
	QCOM_NETWORK_API_FN(_qcom_ip_address_get((id), (ip), (mask), (gate)))


#if 1 
#define qcom_ip6_address_get(id, global, link, gate, extd, linkPrefix, glbPrefix, gwPrefix, glbPrefixExtd) \
	QCOM_NETWORK_API_FN(_qcom_ip6_address_get((id), (global), (link), (gate), (extd), (linkPrefix), \
							(glbPrefix), (gwPrefix), (glbPrefixExtd)))
#endif

#define qcom_dns_server_address_get(dns, num) \
	QCOM_NETWORK_API_FN(_qcom_dns_server_address_get((dns), (num)))

#define qcom_ip_ping(host, size, cnt, interval) \
	QCOM_NETWORK_API_FN(_qcom_ip_ping((host), (size), (cnt), (interval)))

#if 1 

#define qcom_ip_ping6(host, size, cnt, interval) \
	QCOM_NETWORK_API_FN(_qcom_ip_ping6((host), (size), (cnt), (interval)))

#define qcom_ip6config_router_prefix(device_id, addr, len, prefixTime, validTime) \
	QCOM_NETWORK_API_FN(_qcom_ip6config_router_prefix((device_id), (addr), (len), (prefixTime), (validTime)))

#endif

#define qcom_dhcpc_enable(id, en) \
	QCOM_NETWORK_API_FN(_qcom_dhcpc_enable((id), (en)))

#define qcom_dhcps_enable(id, en) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_enable((id), (en)))

#define qcom_dhcps_set_pool(id, sip, eip, time) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_set_pool((id), (sip), (eip), (time)))

#define qcom_dhcps_get_pool(id, sip, eip, time) \
	QCOM_NETWORK_API_FN(_qcom_dhcps_get_pool((id), (sip), (eip), (time)))

#ifdef BRIDGE_ENABLED
#define qcom_bridge_mode_enable(mode) \
	QCOM_NETWORK_API_FN(_qcom_bridge_mode_enable((mode)))
#endif

#define qcom_dnsc_enable(en) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_enable((en)))

#define qcom_dnsc_add_server_address(ip, type) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_add_server_address((ip), (type)))

#define qcom_dnsc_del_server_address(ip, type) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_del_server_address((ip), (type)))

#define qcom_dnsc_get_host_by_name(name, addr) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_get_host_by_name((name), (addr)))

#define qcom_dnss_enable(en) \
	QCOM_NETWORK_API_FN(_qcom_dnss_enable((en)))

#define qcom_dns_local_domain(domain) \
	QCOM_NETWORK_API_FN(_qcom_dns_local_domain(domain))

#define qcom_dns_entry_create(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_entry_create((host), (ip)))

#define qcom_dns_entry_delete(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_entry_delete((host), (ip)))

#define qcom_dns_6entry_create(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_6entry_create((host), (ip)))

#define qcom_dns_6entry_delete(host, ip) \
	QCOM_NETWORK_API_FN(_qcom_dns_6entry_delete((host), (ip)))

#define qcom_dnsc_get_host_by_name2(name, ip, domain, mode) \
	QCOM_NETWORK_API_FN(_qcom_dnsc_get_host_by_name2((name), (ip), (domain), (mode)))

#if defined HTTP_ENABLED

#define qcom_http_server(en, ctx) \
	QCOM_NETWORK_API_FN(_qcom_http_server(en, ctx))

#define qcom_http_server_method(cmd, pagename, objname, type, len, val) \
	QCOM_NETWORK_API_FN(_qcom_http_server_method((cmd), (pagename), (objname), (type), (len), (val)))

#define qcom_http_client_method(cmd, url, data, val, ctx) \
	QCOM_NETWORK_API_FN(_qcom_http_client_method((cmd), (url), (data), (val), (ctx)))

#endif

#define qcom_wmi_event_socket_process(buf) \
	QCOM_NETWORK_API_FN(_qcom_wmi_event_socket_process(buf))

#define qcom_tcp_conn_timeout(timeout_val)\
    QCOM_NETWORK_API_FN(_qcom_tcp_conn_timeout((timeout_val)))

#define qcom_http_set_post_cb(cxt, callback)  \
    QCOM_NETWORK_API_FN(_qcom_http_set_post_cb((cxt), (callback)))

#define qcom_dhcps_register_cb(device_id,callback)\
    QCOM_NETWORK_API_FN(_qcom_dhcps_register_cb((device_id),(callback)))

#define qcom_dhcpc_register_cb(device_id,callback)\
	QCOM_NETWORK_API_FN(_qcom_dhcpc_register_cb((device_id),(callback)))
//TODO
A_STATUS _qcom_set_ipv6_status(A_UINT8 device_id,A_UINT8 flag);	
#define qcom_set_ipv6_status(device_id,flag)\
        _qcom_set_ipv6_status((device_id),(flag))


#ifdef AR6002_REV74

A_STATUS _qcom_ping(A_UINT32 host, A_UINT32 size);
A_STATUS _qcom_ping6(A_UINT8* host, A_UINT32 size);
A_STATUS _qcom_dhcps_release_pool(A_UINT8 device_id);
A_STATUS _qcom_tcp_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);

A_STATUS _qcom_ip4_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex,
        IPV4_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ip6_route(A_UINT8 device_id, A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex,
        IPV6_ROUTE_LIST_T* rtlist);
A_STATUS _qcom_ota_upgrade(A_UINT8 device_id,A_UINT32 ip_addr, const A_CHAR *source,
		A_UINT8 mode,A_UINT8 preserve_last,A_UINT8 protocol,
		A_UINT32 *resp_code,A_UINT32*length);
A_STATUS _qcom_read_ota_area(A_UINT32 offset,A_UINT32 size, A_UINT8 *buffer,A_UINT32 *ret_len);
A_STATUS _qcom_ota_done(A_UINT32 good_image);
A_STATUS _qcom_set_hostname(A_UINT8 device_id,A_CHAR* pstr);
//A_STATUS _qcom_tcp_conn_timeout(A_UINT32 timeout_val);
//A_STATUS _qcom_http_set_post_cb(void* cxt, void *callback);
//A_STATUS _qcom_http_set_put_cb(void* cxt, void *callback);
//A_STATUS _qcom_http_set_delete_cb(void* cxt, void *callback);

#define qcom_ping(host, size)  \
        _qcom_ping((host), (size))

#define qcom_ping6(host, size)  \
        _qcom_ping6((host), (size))

#define qcom_dhcps_release_pool(device_id)  \
    _qcom_dhcps_release_pool((device_id))

#define qcom_tcp_set_exp_backoff(device_id, retry)  \
    _qcom_tcp_set_exp_backoff((device_id), (retry))


#define qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist) \
    _qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist)

#define qcom_ip6_route(device_id, cmd, ip6addr, prefixLen, gw, ifindex, rtlist)  \
    _qcom_ip6_route((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex), (rtlist))

#define qcom_ota_upgrade(device_id,ip_addr,source,mode,preserve_last, protocol,resp_code,length)\
    _qcom_ota_upgrade((device_id),(ip_addr),(source),(mode),(preserve_last),( protocol),(resp_code),(length))

#define qcom_read_ota_area(offset,size,buffer,ret_len)\
    _qcom_read_ota_area((offset),(size),(buffer),(ret_len))

#define  qcom_ota_done(good_image)\
    _qcom_ota_done(good_image)
#define  qcom_set_hostname(device_id,pstr)\
    _qcom_set_hostname((device_id),(pstr))

#else //AR6002_REV74

#define qcom_ping(host, size)  \
    QCOM_NETWORK_API_FN(_qcom_ping((host), (size)))

#define qcom_ping6(host, size)  \
    QCOM_NETWORK_API_FN(_qcom_ping6((host), (size)))

#define qcom_dhcps_release_pool(device_id)  \
    QCOM_NETWORK_API_FN(_qcom_dhcps_release_pool((device_id)))

#define qcom_tcp_set_exp_backoff(device_id, retry)  \
    QCOM_NETWORK_API_FN(_qcom_tcp_set_exp_backoff((device_id), (retry)))

#define qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist) \
    QCOM_NETWORK_API_FN(_qcom_ip4_route(device_id,cmd,addr,subnet,gw,ifindex,rtlist))

#define qcom_ip6_route(device_id, cmd, ip6addr, prefixLen, gw, ifindex, rtlist)  \
    QCOM_NETWORK_API_FN(_qcom_ip6_route((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex), (rtlist)))

#define qcom_ota_upgrade(device_id,ip_addr,source,mode,preserve_last, protocol,resp_code,length)\
        QCOM_NETWORK_API_FN(_qcom_ota_upgrade((device_id),(ip_addr),(source),(mode),(preserve_last),( protocol),(resp_code),(length)))

#define qcom_read_ota_area(offset,size,buffer,ret_len)\
       QCOM_NETWORK_API_FN( _qcom_read_ota_area((offset),(size),(buffer),(ret_len)))

#define qcom_ota_done(good_image)\
    QCOM_NETWORK_API_FN(_qcom_ota_done(good_image))

#define  qcom_set_hostname(device_id,pstr)\
        QCOM_NETWORK_API_FN(_qcom_set_hostname((device_id),(pstr)))

#if defined(AR6002_REV76)
A_STATUS _qcom_autoip_register_cb(A_UINT8 device_id,void *callback);
QCOM_OTA_STATUS_CODE_t _qcom_ota_session_start(A_UINT32 flags, A_UINT32 partition_index);
A_UINT32 _qcom_ota_partition_get_size(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase();
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_erase_sectors(A_UINT32 offset_next);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_verify_checksum(void);
QCOM_OTA_STATUS_CODE_t _qcom_ota_parse_image_hdr(A_UINT8 *header,A_UINT32 *offset);
A_INT32 _qcom_ota_partition_read_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_write_data(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size,A_UINT32 *ret_size);
QCOM_OTA_STATUS_CODE_t  _qcom_ota_session_end(A_UINT32 good_image);
QCOM_OTA_STATUS_CODE_t _qcom_ota_partition_format(A_UINT32 partition_index);
A_STATUS _qcom_http_set_put_cb(void* cxt, void *callback);
A_STATUS _qcom_http_set_delete_cb(void* cxt, void *callback);
A_STATUS _qcom_ssdp_init(A_UINT32 notifyPeriod,A_UINT32 localIpAddr,A_UCHAR *locationStr,
                                    A_UCHAR *serverStr, A_UCHAR *searchTargetStr,
                                    A_UCHAR *domainName, A_UCHAR *serviceType,
                                    A_UCHAR *deviceType, A_UCHAR *uuidStr);
A_STATUS _qcom_ssdp_enable(A_UINT32 enable);
A_STATUS _qcom_ssdp_notify_change(A_UINT32 notifyPeriod);
A_STATUS _qcom_http_get_datasend(A_UINT16 sess_index, A_UINT16 data_len, 
                                    A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
A_STATUS _qcom_set_http_custom_uri(A_UINT8 device_id, const char* const *http_custom_uri);
A_STATUS _qcom_set_httpsvr_ota_uri(A_UINT8 device_id, const char* ota_uri);
A_STATUS _qcom_httpsvr_ota_enable(A_UINT8 device_id, A_INT32 enable);
A_STATUS _qcom_httpsvr_ota_register_cb(void *callback);
A_STATUS _qcom_http_collect_scan_result(A_UINT8 device_id, A_UINT8 **http_data, A_UINT16 *data_len);
A_STATUS _qcom_http_set_get_cb(void* cxt, void *callback);
A_STATUS _qcom_http_client_body(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
A_STATUS _qcom_dhcpc_set_timeout(A_UINT8 device_id,A_UINT32 timeout);
A_STATUS _qcom_http_server_add_redirected_page(A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
A_STATUS _qcom_restrict_http_request(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_http_redirect_unknown_url_enable(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_http_set_redirected_url(A_UINT8 device_id, char * url);
A_INT32 _qcom_get_autoip_status(A_UINT8 device_id, A_UINT8 iface);
void _qcom_dnc_clear(A_UINT8 device_id);
A_STATUS _qcom_http_client_register_cb(http_client_cb_t callback, void* arg);

#ifdef HTTPC_MULTI_SESSION
A_STATUS _http_client_api_check_version(HTTP_CLIENT_API_VERSION version);
A_STATUS _http_client_check_handler(A_INT32 client, A_INT32 *index);
A_INT32 _qcom_http_client_connect(const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, http_client_cb_t callback, void* arg);
A_STATUS _qcom_http_client_disconnect(A_INT32 client);
A_STATUS _qcom_http_client_request(A_INT32 client, HTTPC_REQUEST_CMD_E cmd, const A_CHAR *url, A_CHAR *output);
A_STATUS _qcom_http_client_set_body(A_INT32 client, const A_CHAR *body, A_UINT32 bodylen);
A_STATUS _qcom_http_client_set_param(A_INT32 client, const A_CHAR *key, const A_CHAR *value);
A_STATUS _qcom_http_client_add_header(A_INT32 client, const A_CHAR *header, const A_CHAR *value);
A_STATUS _qcom_http_client_clear_header(A_INT32 client);
#endif

#define qcom_http_set_put_cb(cxt, callback)  \
        _qcom_http_set_put_cb((cxt), (callback))

#define qcom_http_set_delete_cb(cxt, callback)  \
        _qcom_http_set_delete_cb((cxt), (callback))

#define qcom_http_set_get_cb(cxt, callback)  \
        _qcom_http_set_get_cb((cxt), (callback))

#define qcom_autoip_register_cb(device_id,callback)\
	_qcom_autoip_register_cb((device_id),(callback))

#define qcom_ota_session_start(flags, partition_index) \
    _qcom_ota_session_start((flags),(partition_index))

#define qcom_ota_partition_get_size() \
    _qcom_ota_partition_get_size()

#define qcom_ota_partition_erase() \
    _qcom_ota_partition_erase()

#define qcom_ota_partition_erase_sectors(offset_next) \
    _qcom_ota_partition_erase_sectors(offset_next)

#define qcom_ota_parse_image_hdr(header,offset) \
    _qcom_ota_parse_image_hdr((header),(offset))

#define qcom_ota_partition_verify_checksum() \
    _qcom_ota_partition_verify_checksum()


#define qcom_ota_partition_read_data(offset,buf,size) \
     _qcom_ota_partition_read_data((offset),(buf),(size))

#define qcom_ota_partition_write_data(offset,buf,size,ret_size) \
     _qcom_ota_partition_write_data((offset),(buf),(size),(ret_size))

#define qcom_ota_session_end(good_image) \
     _qcom_ota_session_end((good_image))

#define qcom_ota_partition_format(partition_index) \
    _qcom_ota_partition_format((partition_index))

#define qcom_ssdp_init(notifyPeriod ,localIpAddr,locationStr,serverStr,searchTargetStr,domainName,serviceType,deviceType,uuidStr) \
            _qcom_ssdp_init((notifyPeriod),(localIpAddr),(locationStr),(serverStr),(searchTargetStr),(domainName),(serviceType),(deviceType),(uuidStr))

#define qcom_ssdp_enable(enable) \
            _qcom_ssdp_enable((enable))		

#define qcom_ssdp_notify_change(notifyPeriod) \
            _qcom_ssdp_notify_change((notifyPeriod))

#define qcom_http_get_datasend(sess_index,  data_len, url_str,  data, header_len, header_data) \
            _qcom_http_get_datasend(sess_index,  data_len, url_str,  data, header_len, header_data)

#define qcom_set_http_custom_uri(currentDeviceId, http_custom_uri) \
			_qcom_set_http_custom_uri(currentDeviceId, http_custom_uri)
    
#define qcom_set_httpsvr_ota_uri(currentDeviceId, ota_uri) \
                _qcom_set_httpsvr_ota_uri(currentDeviceId, ota_uri)

#define qcom_httpsvr_ota_enable(currentDeviceId, enable) \
                _qcom_httpsvr_ota_enable(currentDeviceId, enable)
                
#define qcom_httpsvr_ota_register_cb(callback) \
                _qcom_httpsvr_ota_register_cb( (callback))

#define qcom_http_server_add_redirected_page(header_len, header, data_len, data, url) \
            _qcom_http_server_add_redirected_page(header_len, header, data_len, data, url)

#define qcom_restrict_http_request(currentDeviceId, enable) \
			_qcom_restrict_http_request(currentDeviceId, enable)

#define qcom_http_redirect_unknown_url_enable(currentDeviceId, enable) \
			_qcom_http_redirect_unknown_url_enable(currentDeviceId, enable)

#define qcom_http_set_redirected_url(currentDeviceId, url) \
			_qcom_http_set_redirected_url(currentDeviceId, url)

#define qcom_get_autoip_status(currentDeviceId, iface) \
			_qcom_get_autoip_status(currentDeviceId, iface)

#define qcom_http_collect_scan_result(device_id,  http_data, data_len) \
            _qcom_http_collect_scan_result(device_id,  http_data, data_len)
			
#define qcom_network_http_server_get_CB(ctx0, ctx1, ctx2, post, put, del) \
            _qcom_network_http_server_get_CB(ctx0, ctx1, ctx2, post, put, del)

#define qcom_network_http_server_set_CB(ctx0, ctx1, ctx2, post, put, del) \
            _qcom_network_http_server_set_CB(ctx0, ctx1, ctx2, post, put, del)

#define qcom_http_client_body(cmd, body, bodylen) \
            _qcom_http_client_body((cmd), (body), (bodylen))
            
#define qcom_dhcpc_set_timeout(device_id,flag)\
           _qcom_dhcpc_set_timeout((device_id),(flag))

#define qcom_dnc_clear(device_id)\
           _qcom_dnc_clear((device_id))

#define qcom_http_client_register_cb(callback, arg) \
           _qcom_http_client_register_cb(callback, arg)

#ifdef HTTPC_MULTI_SESSION
#define http_client_api_check_version(version) \
           _http_client_api_check_version(version)

#define http_client_check_handler(client, index) \
           _http_client_check_handler(client, index)

#define qcom_http_client_connect(server, port, timeout, ssl_ctx, callback, arg) \
           _qcom_http_client_connect(server, port, timeout, ssl_ctx, callback, arg)

#define qcom_http_client_disconnect(client) \
           _qcom_http_client_disconnect(client)

#define qcom_http_client_request(client, cmd, url, output) \
           _qcom_http_client_request(client, cmd, url, output)

#define qcom_http_client_set_body(client, body, bodylen) \
           _qcom_http_client_set_body(client, body, bodylen)

#define qcom_http_client_set_param(client, key, value) \
           _qcom_http_client_set_param(client, key, value)

#define qcom_http_client_add_header(client, header, value) \
           _qcom_http_client_add_header(client, header, value)

#define qcom_http_client_clear_header(client) \
           _qcom_http_client_clear_header(client)
#endif
#else  //AR6002_REV76

#define qcom_http_set_put_cb(cxt, callback)  \
        QCOM_NETWORK_API_FN(_qcom_http_set_put_cb((cxt), (callback)))

#define qcom_http_set_delete_cb(cxt, callback)  \
        QCOM_NETWORK_API_FN(_qcom_http_set_delete_cb((cxt), (callback)))

#define qcom_http_set_get_cb(cxt, callback)  \
        QCOM_NETWORK_API_FN(_qcom_http_set_get_cb((cxt), (callback)))      

#define qcom_autoip_register_cb(device_id,callback)\
	QCOM_NETWORK_API_FN(_qcom_autoip_register_cb((device_id),(callback)))

#define qcom_ota_session_start(flags, partition_index) \
	QCOM_NETWORK_API_FN(_qcom_ota_session_start((flags), (partition_index)))

#define qcom_ota_partition_get_size() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_get_size())

#define qcom_ota_partition_erase() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_erase())

#define qcom_ota_partition_erase_sectors(offset_next) \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_erase_sectors(offset_next))

#define qcom_ota_parse_image_hdr(header,offset) \
       QCOM_NETWORK_API_FN(_qcom_ota_parse_image_hdr((header),(offset)))

#define qcom_ota_partition_read_data(offset,buf,size) \
     QCOM_NETWORK_API_FN(_qcom_ota_partition_read_data((offset),(buf),(size)))

#define qcom_ota_partition_verify_checksum() \
      	QCOM_NETWORK_API_FN(_qcom_ota_partition_verify_checksum())

#define qcom_ota_partition_write_data(offset, buf,size,ret_size) \
        QCOM_NETWORK_API_FN(_qcom_ota_partition_write_data((offset),(buf),(size),(ret_size)))

#define qcom_ota_session_end(good_image) \
     QCOM_NETWORK_API_FN(_qcom_ota_session_end((good_image)))

#define qcom_ota_partition_format(partition_index) \
    QCOM_NETWORK_API_FN(_qcom_ota_partition_format((partition_index)))

#define qcom_ssdp_init(notifyPeriod ,localIpAddr,locationStr,serverStr,searchTargetStr,domainName,serviceType,deviceType,uuidStr) \
            QCOM_NETWORK_API_FN(_qcom_ssdp_init((notifyPeriod),(localIpAddr),(locationStr),(serverStr),(searchTargetStr),(domainName),(serviceType),(deviceType),(uuidStr)))

#define qcom_ssdp_enable(enable) \
            QCOM_NETWORK_API_FN(_qcom_ssdp_enable((enable)))		

#define qcom_ssdp_notify_change(notifyPeriod) \
            QCOM_NETWORK_API_FN(_qcom_ssdp_notify_change((notifyPeriod)))    

#define qcom_http_get_datasend(sess_index,  data_len, url_str,  data, heder_len, header_data) \
            QCOM_NETWORK_API_FN(_qcom_http_get_datasend((sess_index,  data_len, url_str,  data, header_len, header_data)))
            
#define qcom_set_http_custom_uri(currentDeviceId, http_custom_uri) \
			QCOM_NETWORK_API_FN(_qcom_set_http_custom_uri(currentDeviceId, http_custom_uri))
    
#define qcom_set_httpsvr_ota_uri(currentDeviceId, ota_uri) \
            QCOM_NETWORK_API_FN(_qcom_set_httpsvr_ota_uri(currentDeviceId, ota_uri))

#define qcom_httpsvr_ota_enable(currentDeviceId, enable) \
            QCOM_NETWORK_API_FN(_qcom_httpsvr_ota_enable(currentDeviceId, enable))
    
#define qcom_httpsvr_ota_register_cb(callback)  \
            QCOM_NETWORK_API_FN(_qcom_httpsvr_ota_register_cb((callback)))

#define qcom_http_server_add_redirected_page(header_len, header, data_len, data, url)\
            QCOM_NETWORK_API_FN(_qcom_http_server_add_redirected_page((header_len), (header), (data_len), (data), (url)))

#define qcom_restrict_http_request(currentDeviceId, enable) \
			QCOM_NETWORK_API_FN(_qcom_restrict_http_request(currentDeviceId, enable))

#define qcom_http_redirect_unknown_url_enable(currentDeviceId, enable) \
			QCOM_NETWORK_API_FN(_qcom_http_redirect_unknown_url_enable(currentDeviceId, enable))

#define qcom_http_set_redirected_url(currentDeviceId, url) \
			QCOM_NETWORK_API_FN(_qcom_http_set_redirected_url(currentDeviceId, url))

#define qcom_get_autoip_status(currentDeviceId, iface) \
			QCOM_NETWORK_API_FN(_qcom_get_autoip_status(currentDeviceId, iface))

#define qcom_http_collect_scan_result(device_id,  http_data, data_len) \
            QCOM_NETWORK_API_FN(_qcom_http_collect_scan_result((device_id,  http_data, data_len)))
#define qcom_network_http_server_get_CB(ctx0, ctx1, ctx2, post, put, del) \
            QCOM_NETWORK_API_FN(_qcom_network_http_server_get_CB(ctx0, ctx1, ctx2, post, put, del))

#define qcom_network_http_server_set_CB(ctx0, ctx1, ctx2, post, put, del) \
            QCOM_NETWORK_API_FN(_qcom_network_http_server_set_CB(ctx0, ctx1, ctx2, post, put, del))

#define qcom_http_client_body(cmd, body, bodylen) \
            QCOM_NETWORK_API_FN(_qcom_http_client_body((cmd), (body), (bodylen)))
            
#define qcom_dhcpc_set_timeout(device_id,flag)\
           _qcom_dhcpc_set_timeout((device_id),(flag))

#define qcom_dnc_clear(device_id)\
           QCOM_NETWORK_API_FN(_qcom_dnc_clear(device_id))

#define qcom_http_client_register_cb(callback, arg)\
           QCOM_NETWORK_API_FN(_qcom_http_client_register_cb(callback, arg))

#ifdef HTTPC_MULTI_SESSION
#define http_client_api_check_version(version) \
           QCOM_NETWORK_API_FN(_http_client_api_check_version(version))

#define http_client_check_handler(client, index) \
           QCOM_NETWORK_API_FN(_http_client_check_handler(client, index))

#define qcom_http_client_connect(server, port, timeout, ssl_ctx, callback, arg) \
           QCOM_NETWORK_API_FN(_qcom_http_client_connect(server, port, timeout, ssl_ctx, callback, arg))

#define qcom_http_client_disconnect(client) \
           QCOM_NETWORK_API_FN(_qcom_http_client_disconnect(client))

#define qcom_http_client_request(client, cmd, url, output) \
           QCOM_NETWORK_API_FN(_qcom_http_client_request(client, cmd, url, output))

#define qcom_http_client_set_body(client, body, bodylen) \
           QCOM_NETWORK_API_FN(_qcom_http_client_set_body(client, body, bodylen))

#define qcom_http_client_set_param(client, key, value) \
           QCOM_NETWORK_API_FN(_qcom_http_client_set_param(client, key, value))

#define qcom_http_client_add_header(client, header, value) \
           QCOM_NETWORK_API_FN(_qcom_http_client_add_header(client, header, value))

#define qcom_http_client_clear_header(client) \
           QCOM_NETWORK_API_FN(_qcom_http_client_clear_header(client))
#endif

#endif   // AR6002_REV76

#endif  //AR6002_REV74

#endif /* __QCOM_NETWORK__ */
