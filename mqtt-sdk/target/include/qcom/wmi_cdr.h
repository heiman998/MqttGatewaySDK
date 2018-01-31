/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 *
 * This file contains the definitions of the WMI protocol specified in the
 * Wireless Module Interface (WMI).  It includes definitions of all the
 * commands and events. Commands are messages from the host to the WM.
 * Events and Replies are messages from the WM to the host.
 *
 * Ownership of correctness in regards to commands
 * belongs to the host driver and the WMI is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 */

#ifndef _WMI_CDR_H_
#define _WMI_CDR_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "qcom/stdint.h"
#include "cdr_api.h"
#include "wmi.h"

#ifndef CDR_WMI_SUPPORT
#define CDR_WMI_SUPPORT
#endif




#ifdef PHOST_INDIRECT_ACCESS

typedef struct  {
	void (* _WMI_CDR_initiate)(void *cmd, unsigned int cmd_length);

}WMI_CDR_INIT_API_INDIRECTION_TABLE;

extern WMI_CDR_INIT_API_INDIRECTION_TABLE wmi_cdr_init_api_indirect_table;

#define WMI_CDR_initiate(_cmd, _len) wmi_cdr_init_api_indirect_table._WMI_CDR_initiate((_cmd), (_len))
#else

void _WMI_CDR_initiate(void *cmd, unsigned int cmd_length);

#define WMI_CDR_initiate(_cmd, _len) _WMI_CDR_initiate((_cmd), (_len))

#endif



#define WMI_SYNC_FLAG int

typedef struct _wmi_device_cdr_s {
       A_UINT16  devId;
} WMI_STRUCT_CDR;

typedef int (*t_wmi_event_dispatch_handler)(void     *pContext,   /* app supplied context */
                                                                  A_UINT16  EventID,    /* event ID */
                                                                  A_UINT16  info,       /* the info param passed to WMI_SendEvent */
                                                                  A_UINT8  *pEvtBuffer, /* pointer to event data, if needed */
                                                                  int       Length);
extern void cdr_wmi_event_handler_init(t_wmi_event_dispatch_handler cb);




#ifdef PHOST_INDIRECT_ACCESS

typedef struct
{
	A_STATUS
	(* _CDR_wmi_dev_init)(void);
	WMI_STRUCT_CDR *
	(* _CDR_wmi_dev_get)(A_UINT16 devId);
	A_STATUS
	(* _CDR_wmi_connect_cmd)(WMI_STRUCT_CDR * wmip, NETWORK_TYPE netType,
	                    DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
	                    CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen,
	                    CRYPTO_TYPE groupCrypto, A_UINT8 groupCryptoLen,
	                    int ssidLength, A_UCHAR * ssid,
	                    A_UINT8 * bssid, A_UINT16 channel, A_UINT32 ctrl_flags);
	A_STATUS
	(* _CDR_wmi_listen_interval_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 listen_int);
	A_STATUS
	(* _CDR_wmi_ap_set_beacon_int_cmd)(WMI_STRUCT_CDR * wmip, A_UINT16 beacon_interval);

	A_STATUS
	(* _CDR_wmi_ap_hidden_ssid_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 hidden_ssid);
	A_STATUS
	(* _CDR_wmi_ap_set_num_sta_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 num_sta);
	A_STATUS
	(* _CDR_wmi_ap_set_country_cmd)(WMI_STRUCT_CDR * wmip, char *country_code);
	void
	(* _CDR_wmi_allow_aggr)(WMI_STRUCT_CDR * wmip, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
	A_STATUS
	(* _CDR_wmi_addKey_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 keyIndex, CRYPTO_TYPE keyType,
	               A_UINT8 keyUsage, A_UINT8 keyLength, A_UINT8 *keyRSC,
	               A_UINT8 *keyMaterial, A_UINT8 key_op_ctrl, A_UINT8 *macAddr,
	               WMI_SYNC_FLAG sync_flag);
	               
	A_STATUS
	(* _CDR_wmi_set_passphrase_cmd)(WMI_STRUCT_CDR * wmip, A_UCHAR * ssid, A_UINT8 ssidLength,
	                           A_UINT8 * passphrase, A_UINT8 passLength);
	A_STATUS
	(* _CDR_wmi_scanparams_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 fg_start_sec,
	                   A_UINT16 fg_end_sec, A_UINT16 bg_sec,
	                   A_UINT16 minact_chdw_msec, A_UINT16 maxact_chdw_msec,
	                   A_UINT16 pas_chdw_msec,
	                   A_UINT8 shScanRatio, A_UINT8 scanCtrlFlags,
	                   A_UINT32 max_dfsch_act_time, A_UINT16 maxact_scan_per_ssid);
	A_STATUS
	(* _CDR_wmi_bssfilter_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 filter, A_UINT32 ieMask);
	A_STATUS
	(* _CDR_wmi_probedSsid_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 index, A_UINT8 flag,
	                   A_UINT8 ssidLength, A_UCHAR *ssid);
	A_STATUS
	(* _CDR_wmi_startscan_cmd)(WMI_STRUCT_CDR *wmip, WMI_SCAN_TYPE scanType,
	                  A_BOOL forceFgScan, A_BOOL isLegacy,
	                  A_UINT32 homeDwellTime, A_UINT32 forceScanInterval,
	                  A_INT8 numChan, A_UINT16 *channelList);
	                  
	A_STATUS
	(* _CDR_wmi_disconnect_cmd)(WMI_STRUCT_CDR *wmip);
	A_STATUS
	(* _CDR_wmi_set_pmk_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 *pmk, A_UINT8 pmk_len);
	void
	(* _CDR_wmi_set_promiscuous_mode)(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
	A_STATUS
	(* _CDR_wmi_reconnect_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 * bssid, A_UINT16 channel);
	A_STATUS
	(* _CDR_wmi_set_txPwr_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 dbM);

	A_STATUS
	(* _CDR_wmi_set_channelParams_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 scanParam,
	                              WMI_PHY_MODE mode, A_INT8 numChan, A_UINT16 * channelList);
	A_STATUS
	(* _CDR_wmi_set_ht_cap_cmd)(WMI_STRUCT_CDR * wmip,
	                       A_UINT8 band,
	                       A_UINT8 enable,
	                       A_UINT8 chan_width_40M_supported,
	                       A_UINT8 short_GI_20MHz,
	                       A_UINT8 short_GI_40MHz, A_UINT8 intolerance_40MHz, A_UINT8 max_ampdu_len_exp);
	A_STATUS
	(* _CDR_wmi_set_channel_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 channel);
	A_STATUS
	(* _CDR_wmi_abort_scan_cmd)(WMI_STRUCT_CDR *wmip);
	A_STATUS
	(* _CDR_wmi_pmparams_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 idlePeriod,
	                 A_UINT16 psPollNum, A_UINT16 dtimPolicy,
	                 A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
	                 A_UINT16 ps_fail_event_policy);

	A_STATUS
	(* _CDR_wmi_powermode_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 powerMode);
	A_STATUS
	(* _CDR_wmi_susp_enable_cmd)(WMI_STRUCT_CDR *wmip);
	A_STATUS
	(* _CDR_wmi_susp_start_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 time);
	A_STATUS
	(* _CDR_wmi_wm_susp_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 time, A_UINT32 flag);
        A_STATUS
        (* _CDR_wmi_wm_gpiowakeupcfg_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 options, A_UINT32 wake_msec, A_UINT32 sleepOnGPIO_flag);
	A_STATUS
	(* _CDR_wmi_rssi_get_cmd)(WMI_STRUCT_CDR *wmip);

	A_STATUS
	(* _CDR_wmi_wps_start_cmd)(WMI_STRUCT_CDR *wmip, WMI_WPS_START_CMD *wps_start);
	void
	(* _CDR_wmi_ap_profile_commit)(WMI_STRUCT_CDR * wmip, A_UINT8 networkType,
	                          A_UINT8 dot11AuthMode,
	                          A_UINT8 authMode,
	                          A_UINT8 pairwiseCryptoType,
	                          A_UINT8 pairwiseCryptoLen,
	                          A_UINT8 groupCryptoType,
	                          A_UINT8 groupCryptoLen,
	                          A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
	                          A_UINT32 ctrl_flags);
	A_STATUS
	(* _CDR_wmi_sock_stack_init_cmd)(WMI_STRUCT_CDR * wmip);
	A_STATUS
	(* _CDR_wmi_dns_get_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 mode, A_UINT32 domain, char *hostname);
	A_STATUS
	(* _CDR_wmi_dns_en_cmd)(WMI_STRUCT_CDR * wmip, A_INT32 command);

	A_STATUS
	(* _CDR_wmi_dns_srv_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 *serv_ip, A_UINT8 type, int mode);
	A_STATUS
	(* _CDR_wmi_dns_srv_en_cmd)(WMI_STRUCT_CDR * wmip, A_INT32 command);
	A_STATUS
	(* _CDR_wmi_dns_domain_cmd)(WMI_STRUCT_CDR * wmip, A_CHAR* command);
	A_STATUS
	(* _CDR_wmi_dns_update_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 command, A_CHAR* hostname, A_UINT32 ipaddr);
	A_STATUS
	(* _CDR_wmi_dns_6update_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 command, A_CHAR* hostname, A_UINT8* ip6addr);
	A_STATUS
	(* _CDR_wmi_ip_config_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 mode, A_UINT32 ipv4, A_UINT32 subnetMask,
	                      A_UINT32 gateway4);

	A_STATUS
	(* _CDR_wmi_ip6_config_cmd)(WMI_STRUCT_CDR * wmip);
	A_STATUS
	(* _CDR_wmi_http_server)(WMI_STRUCT_CDR * wmip, A_INT32 enable, void *ctx);
	A_STATUS
	(* _CDR_wmi_http_server_cmd)(WMI_STRUCT_CDR * wmip, A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8* value);
	A_STATUS
	(* _CDR_wmi_http_client_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, void *ctx);
	A_STATUS
	(* _CDR_wmi_ipconfig_dhcp_pool_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 startaddr, A_UINT32 endaddr,
	                               A_INT32 leasetime);

	void (* _CDR_wmi_socket_cmd)(WMI_STRUCT_CDR *wmip, void *cmd, int length);
	A_STATUS
	(* _CDR_wmi_ping_cmd)(WMI_STRUCT_CDR * wmip, A_UINT32 ipv4_addr, A_UINT32 size);
	A_STATUS
	(* _CDR_wmi_ping6_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8* ipv6_addr, A_UINT32 size);
	A_STATUS
	(* _CDR_wmi_ip6_router_prefix_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8* addr, A_INT32 prefix_length, A_INT32 prefix_lifetime, A_INT32 valid_lifetime);
	A_STATUS
	(* _CDR_wmi_sntp_en_cmd)(WMI_STRUCT_CDR *wmip, void *sntp_en);

	A_STATUS
	(* _CDR_wmi_sntp_srv_cmd)(WMI_STRUCT_CDR *wmip, void *sntp_srv);
	A_STATUS
	(* _CDR_wmi_sntp_zone_cmd)(WMI_STRUCT_CDR *wmip, void *sntp_zone);
	A_STATUS
	(* _CDR_wmi_sntp_get_time_cmd)(WMI_STRUCT_CDR *wmip, void *sntp_get_time);
	A_STATUS
	(* _CDR_wmi_sntp_show_config)(WMI_STRUCT_CDR *wmip, void *sntp_show_config);
	A_STATUS
	(* _CDR_wmi_sntp_get_time_of_day)(WMI_STRUCT_CDR *wmip, void *sntp_get_time_of_day);

//#ifdef BRIDGE_ENABLED
	A_STATUS
	(* _CDR_wmi_bridge_mode_en_cmd)(WMI_STRUCT_CDR * wmip, A_UINT16 bridgemode);
//#endif
	A_STATUS
	(*_CDR_wmi_ap_set_inact_time_cmd)(WMI_STRUCT_CDR * wmip, A_UINT16 inact_time);
    A_STATUS
    (*_CDR_wmi_ipconfig_dhcp_release_cmd)(WMI_STRUCT_CDR * wmip); 
    A_STATUS
    (*_CDR_wmi_ipconfig_set_tcp_exp_backoff)(WMI_STRUCT_CDR * wmip, A_INT32 retry);
    A_STATUS
    (*_CDR_wmi_ip4_route_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    (*_CDR_wmi_ip6_route_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32*
        ifindex);

  A_STATUS (*_CDR_wmi_ota_upgrade_cmd)(WMI_STRUCT_CDR * wmip,void *cmd);
    A_STATUS (*_CDR_wmi_ota_read_ota_area_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_done_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_set_dhcp_hostname_cmd)(WMI_STRUCT_CDR * wmip,void *cmd);


    A_STATUS
    (*_CDR_wmi_pwm_ctrl_cmd)(WMI_STRUCT_CDR * wmip,A_BOOL module_select,A_BOOL enable,A_UINT32 port_id, A_INT32 * retp);
    A_STATUS
    (*_CDR_wmi_pwm_sdm_set_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 freq, A_UINT32 duty,A_UINT32 port_id, A_INT32 * retp);
    A_STATUS
    (*_CDR_wmi_pwm_port_set_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 freq,A_UINT32 duty,A_UINT32 phase,A_UINT32 port_id, A_UINT32 src_clk, A_INT32 * retp);
    A_STATUS
    (*_CDR_wmi_i2cm_init_cmd)(WMI_STRUCT_CDR * wmip,A_UINT8 pin_pair_id, A_INT8 freq, A_UINT32 timeout,A_INT32 * retp);
    A_STATUS
    (*_CDR_wmi_i2cm_ctrl_cmd)(WMI_STRUCT_CDR * wmip,A_UINT8 pin_pair_id,A_UCHAR *tx_data, A_INT32 tx_len, A_UCHAR *rx_data, A_INT32 rx_len, A_INT32 * retp);
    A_STATUS 
    (*_CDR_wmi_i2cs_csr_cmd)(WMI_STRUCT_CDR * wmip,I2CS_CSR_PARA *csr_params);
    A_STATUS 
	(*_CDR_wmi_i2cs_fifo_cmd)(WMI_STRUCT_CDR * wmip,I2CS_FIFO_PARA *fifo_params);
    A_STATUS 
	(*_CDR_wmi_i2cs_reg_cmd)(WMI_STRUCT_CDR * wmip,I2CS_REG_PARA *reg_params);
    A_STATUS 
	(*_CDR_wmi_i2cs_cmd_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 cmd,A_UINT32* data,A_INT32 * retp);
    A_STATUS 
	(*_CDR_wmi_i2cs_ctrl_cmd)(WMI_STRUCT_CDR * wmip,int enable, A_UINT32 pin_pair_id);

    A_STATUS (* _CDR_wmi_lpl_force_enable_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
    A_STATUS (* _CDR_wmi_gtx_enable_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
    A_STATUS (* _CDR_wmi_rate_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 mcs, A_UINT8 rate);
	A_STATUS (* _CDR_wmi_adc_cmd)(WMI_STRUCT_CDR *wmip, void *cmd, int length);
    A_STATUS (* _CDR_wmi_ebt_mac_filter_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);
    // To be added in next rom
    //A_STATUS (* _CDR_wmi_set_sta_country_code)(WMI_STRUCT_CDR *wmip, A_CHAR *country);
    //A_STATUS (*_CDR_wmi_wps_config_state_set_cmd)(WMI_STRUCT_CDR *wmip, WMI_WPS_CONFIG_SET_CMD *wpsConfigStateSet);
    //A_STATUS (*_CDR_wmi_set_ipv6_status_cmd)(WMI_STRUCT_CDR * wmip,void *cmd);
#if !defined(AR6002_REV76)
    A_STATUS (*_CDR_wmi_ota_session_start_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_partition_get_size_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_partition_erase_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_partition_erase_sectors_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_parse_image_hdr_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_partition_verify_checksum_cmd)(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS (*_CDR_wmi_ota_partition_write_data_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    A_STATUS (*_CDR_wmi_ssdp_init_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod,A_UINT32 localIpAddr,A_UCHAR *locationStr,
		A_UCHAR *serverStr,A_UCHAR *searchTargetStr,
		A_UCHAR *domainName,A_UCHAR *serviceType,
		A_UCHAR *deviceType,A_UCHAR *uuidStr);	
    A_STATUS (*_CDR_wmi_ssdp_enable_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 enable);
    A_STATUS (*_CDR_wmi_ssdp_notify_change_cmd)(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod);
    A_STATUS (*_CDR_wmi_http_get_datasend_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 sess_index, A_UINT16 data_len, 
                    A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
    A_STATUS (*_CDR_wmi_http_client_body_cmd)(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
    A_STATUS (*_CDR_wmi_http_set_custom_url_cmd)(WMI_STRUCT_CDR *wmip, const char* const *new_url);
    A_STATUS (*_CDR_wmi_httpsvr_set_ota_uri_cmd)(WMI_STRUCT_CDR *wmip, const char* ota_uri);  
    A_STATUS (*_CDR_wmi_httpsvr_set_ota_enable_cmd)(WMI_STRUCT_CDR *wmip, A_INT32 enable);
    A_STATUS (*_CDR_wmi_restrict_http_request_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
    A_STATUS (*_CDR_wmi_http_redirect_unknown_url_enable_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
    A_STATUS (*_CDR_wmi_http_server_add_redirected_page_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
    A_STATUS (*_CDR_wmi_http_set_redirected_url_cmd)(WMI_STRUCT_CDR *wmip, char *url);
    /* bmiss_num: num of beacon; bmiss_time: time in unit of TU (1024us) */
    A_STATUS (*_CDR_wmi_set_bmiss_time_cmd)(WMI_STRUCT_CDR *wmip, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
    A_STATUS (*_CDR_wmi_set_vendor_specific_ie_cmd)(WMI_STRUCT_CDR *wmip, A_UINT8 mgmt_frm_type,A_UINT8 *ie_info,A_UINT16 len);

	A_STATUS (*_CDR_wmi_ant_div_set_cmd)(WMI_STRUCT_CDR * wmip, int div, int adjust);
	A_STATUS (*_CDR_wmi_ant_div_get_cmd)(WMI_STRUCT_CDR * wmip);
    A_STATUS (*_CDR_wmi_http_client_register_cb_cmd)(WMI_STRUCT_CDR * wmip, void* callback, void* arg);
#ifdef HTTPC_MULTI_SESSION
    A_STATUS (*_CDR_wmi_http_client_connect_cmd)(WMI_STRUCT_CDR* wmip, void* client, const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, A_BOOL cb_enabled);
    A_STATUS (*_CDR_wmi_http_client_disconnect_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32 client, A_UINT32 session);
	A_STATUS (*_CDR_wmi_http_client_request_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32 session, A_UINT32 cmd, const A_CHAR *url, A_CHAR *output);
	A_STATUS (*_CDR_wmi_http_client_set_body_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *body, A_UINT32 bodylen);
	A_STATUS (*_CDR_wmi_http_client_set_param_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *key, const A_CHAR *value);
	A_STATUS (*_CDR_wmi_http_client_add_header_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *header, const A_CHAR *value);
	A_STATUS (*_CDR_wmi_http_client_clear_header_cmd)(WMI_STRUCT_CDR* wmip, A_UINT32  session);
#endif
	A_STATUS (*_CDR_wmi_btcoex_scheme_set_cmd)(WMI_STRUCT_CDR * wmip, A_UINT8 scheme_id, void* data);

#endif
}WMI_CDR_API_INDIRECT_TABLE;
extern WMI_CDR_API_INDIRECT_TABLE wmi_cdr_api_indirect_table;

#define WMI_CDR_API_FN(fn) wmi_cdr_api_indirect_table.fn


#else

	A_STATUS
	_CDR_wmi_dev_init(void);
	WMI_STRUCT_CDR *
	_CDR_wmi_dev_get(A_UINT16 devId);
	A_STATUS
	_CDR_wmi_connect_cmd(WMI_STRUCT_CDR * wmip, NETWORK_TYPE netType,
	                    DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
	                    CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen,
	                    CRYPTO_TYPE groupCrypto, A_UINT8 groupCryptoLen,
	                    int ssidLength, A_UCHAR * ssid,
	                    A_UINT8 * bssid, A_UINT16 channel, A_UINT32 ctrl_flags);
	A_STATUS
	_CDR_wmi_listen_interval_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 listen_int);
	A_STATUS
	_CDR_wmi_ap_set_beacon_int_cmd(WMI_STRUCT_CDR * wmip, A_UINT16 beacon_interval);

	A_STATUS
	_CDR_wmi_ap_hidden_ssid_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 hidden_ssid);
	A_STATUS
	_CDR_wmi_ap_set_num_sta_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 num_sta);
	A_STATUS
	_CDR_wmi_ap_set_country_cmd(WMI_STRUCT_CDR * wmip, char *country_code);
    A_STATUS _CDR_wmi_set_sta_country_code(WMI_STRUCT_CDR *wmip, A_CHAR *country);
	void
	_CDR_wmi_allow_aggr(WMI_STRUCT_CDR * wmip, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
	A_STATUS
	_CDR_wmi_addKey_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 keyIndex, CRYPTO_TYPE keyType,
	               A_UINT8 keyUsage, A_UINT8 keyLength, A_UINT8 *keyRSC,
	               A_UINT8 *keyMaterial, A_UINT8 key_op_ctrl, A_UINT8 *macAddr,
	               WMI_SYNC_FLAG sync_flag);
	               
	A_STATUS
	_CDR_wmi_set_passphrase_cmd(WMI_STRUCT_CDR * wmip, A_UCHAR * ssid, A_UINT8 ssidLength,
	                           A_UINT8 * passphrase, A_UINT8 passLength);
	A_STATUS
	_CDR_wmi_scanparams_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 fg_start_sec,
	                   A_UINT16 fg_end_sec, A_UINT16 bg_sec,
	                   A_UINT16 minact_chdw_msec, A_UINT16 maxact_chdw_msec,
	                   A_UINT16 pas_chdw_msec,
	                   A_UINT8 shScanRatio, A_UINT8 scanCtrlFlags,
	                   A_UINT32 max_dfsch_act_time, A_UINT16 maxact_scan_per_ssid);
	A_STATUS
	_CDR_wmi_bssfilter_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 filter, A_UINT32 ieMask);
	A_STATUS
	_CDR_wmi_probedSsid_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 index, A_UINT8 flag,
	                   A_UINT8 ssidLength, A_UCHAR *ssid);
	A_STATUS
	_CDR_wmi_startscan_cmd(WMI_STRUCT_CDR *wmip, WMI_SCAN_TYPE scanType,
	                  A_BOOL forceFgScan, A_BOOL isLegacy,
	                  A_UINT32 homeDwellTime, A_UINT32 forceScanInterval,
	                  A_INT8 numChan, A_UINT16 *channelList);
	                  
	A_STATUS
	_CDR_wmi_disconnect_cmd(WMI_STRUCT_CDR *wmip);
	A_STATUS
	_CDR_wmi_set_pmk_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 *pmk, A_UINT8 pmk_len);
	void
	_CDR_wmi_set_promiscuous_mode(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
	A_STATUS
	_CDR_wmi_reconnect_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 * bssid, A_UINT16 channel);
	A_STATUS
	_CDR_wmi_set_txPwr_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 dbM);

	A_STATUS
	_CDR_wmi_set_channelParams_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 scanParam,
	                              WMI_PHY_MODE mode, A_INT8 numChan, A_UINT16 * channelList);
	A_STATUS
	_CDR_wmi_set_ht_cap_cmd(WMI_STRUCT_CDR * wmip,
	                       A_UINT8 band,
	                       A_UINT8 enable,
	                       A_UINT8 chan_width_40M_supported,
	                       A_UINT8 short_GI_20MHz,
	                       A_UINT8 short_GI_40MHz, A_UINT8 intolerance_40MHz, A_UINT8 max_ampdu_len_exp);
	A_STATUS
	_CDR_wmi_set_channel_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 channel);
	A_STATUS
	_CDR_wmi_abort_scan_cmd(WMI_STRUCT_CDR *wmip);
	A_STATUS
	_CDR_wmi_pmparams_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 idlePeriod,
	                 A_UINT16 psPollNum, A_UINT16 dtimPolicy,
	                 A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
	                 A_UINT16 ps_fail_event_policy);

	A_STATUS
	_CDR_wmi_powermode_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 powerMode);
	A_STATUS
	_CDR_wmi_susp_enable_cmd(WMI_STRUCT_CDR *wmip);
	A_STATUS
	_CDR_wmi_susp_start_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 time);
	A_STATUS
	_CDR_wmi_wm_susp_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 time, A_UINT32 flag);
        A_STATUS
        _CDR_wmi_wm_gpiowakeupcfg_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 options, A_UINT32 wake_msec, A_UINT32 sleepOnGPIO_flag);
	A_STATUS
	_CDR_wmi_rssi_get_cmd(WMI_STRUCT_CDR *wmip);

	A_STATUS
	_CDR_wmi_wps_start_cmd(WMI_STRUCT_CDR *wmip, WMI_WPS_START_CMD *wps_start);
	void
	_CDR_wmi_ap_profile_commit(WMI_STRUCT_CDR * wmip, A_UINT8 networkType,
	                          A_UINT8 dot11AuthMode,
	                          A_UINT8 authMode,
	                          A_UINT8 pairwiseCryptoType,
	                          A_UINT8 pairwiseCryptoLen,
	                          A_UINT8 groupCryptoType,
	                          A_UINT8 groupCryptoLen,
	                          A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
	                          A_UINT32 ctrl_flags);
	A_STATUS
	_CDR_wmi_sock_stack_init_cmd(WMI_STRUCT_CDR * wmip);
	A_STATUS
	_CDR_wmi_dns_get_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 mode, A_UINT32 domain, char *hostname);
	A_STATUS
	_CDR_wmi_dns_en_cmd(WMI_STRUCT_CDR * wmip, A_INT32 command);

	A_STATUS
	_CDR_wmi_dns_srv_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 *serv_ip, A_UINT8 type, int mode);
	A_STATUS
	_CDR_wmi_dns_srv_en_cmd(WMI_STRUCT_CDR * wmip, A_INT32 command);
	A_STATUS
	_CDR_wmi_dns_domain_cmd(WMI_STRUCT_CDR * wmip, A_CHAR* name);

	A_STATUS
	_CDR_wmi_dns_update_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 command, A_CHAR* hostname, A_UINT32 ipaddr);
	A_STATUS
	_CDR_wmi_dns_6update_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 command, A_CHAR* hostname, A_UINT8* ip6addr);
	A_STATUS
	_CDR_wmi_ip_config_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 mode, A_UINT32 ipv4, A_UINT32 subnetMask,
	                      A_UINT32 gateway4);

	A_STATUS
	_CDR_wmi_ip6_config_cmd(WMI_STRUCT_CDR * wmip);
	A_STATUS
	_CDR_wmi_http_server(WMI_STRUCT_CDR * wmip, A_INT32 enable, void *ctx);
	A_STATUS
	_CDR_wmi_http_server_cmd(WMI_STRUCT_CDR * wmip, A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8* value);
	A_STATUS
	_CDR_wmi_http_client_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, void *ctx);
	A_STATUS
	_CDR_wmi_ipconfig_dhcp_pool_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 startaddr, A_UINT32 endaddr,
	                               A_INT32 leasetime);

	void _CDR_wmi_socket_cmd(WMI_STRUCT_CDR *wmip, void *cmd, int length);
	A_STATUS
	_CDR_wmi_ping_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 ipv4_addr, A_UINT32 size);
	A_STATUS
	_CDR_wmi_ping6_cmd(WMI_STRUCT_CDR * wmip, A_UINT8* ipv6_addr, A_UINT32 size);
	A_STATUS
	_CDR_wmi_ip6_router_prefix_cmd(WMI_STRUCT_CDR * wmip, A_UINT8* addr, A_INT32 prefix_length, A_INT32 prefix_lifetime, A_INT32 valid_lifetime);
	A_STATUS
	_CDR_wmi_sntp_en_cmd(WMI_STRUCT_CDR *wmip, void *sntp_en);

	A_STATUS
	_CDR_wmi_sntp_srv_cmd(WMI_STRUCT_CDR *wmip, void *sntp_srv);
	A_STATUS
	_CDR_wmi_sntp_zone_cmd(WMI_STRUCT_CDR *wmip, void *sntp_zone);
	A_STATUS
	_CDR_wmi_sntp_get_time_cmd(WMI_STRUCT_CDR *wmip, void *sntp_get_time);
	A_STATUS
	_CDR_wmi_sntp_show_config(WMI_STRUCT_CDR *wmip, void *sntp_show_config);
	A_STATUS
	_CDR_wmi_sntp_get_time_of_day(WMI_STRUCT_CDR *wmip, void *sntp_get_time_of_day);

#ifdef BRIDGE_ENABLED
	A_STATUS
	_CDR_wmi_bridge_mode_en_cmd(WMI_STRUCT_CDR * wmip, A_UINT16 bridgemode);
#endif

    A_STATUS
	_CDR_wmi_ap_set_inact_time_cmd(WMI_STRUCT_CDR * wmip, A_UINT16 inact_time);
    A_STATUS
    _CDR_wmi_ipconfig_dhcp_release_cmd(WMI_STRUCT_CDR * wmip); 
    A_STATUS
    _CDR_wmi_ipconfig_set_tcp_exp_backoff(WMI_STRUCT_CDR * wmip, A_INT32 retry);
    A_STATUS
    _CDR_wmi_ip4_route_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    _CDR_wmi_ip6_route_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32*
        ifindex);


    A_STATUS _CDR_wmi_ota_upgrade_cmd(WMI_STRUCT_CDR * wmip,void *cmd);
    A_STATUS _CDR_wmi_ota_read_ota_area_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS _CDR_wmi_ota_done_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS _CDR_wmi_set_dhcp_hostname_cmd(WMI_STRUCT_CDR * wmip,void *cmd);

   
            
A_STATUS
_CDR_wmi_pwm_ctrl_cmd(WMI_STRUCT_CDR * wmip,A_BOOL module_select,A_BOOL enable,A_UINT32 port_id, A_INT32 * retp);
A_STATUS
_CDR_wmi_pwm_sdm_set_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 freq, A_UINT32 duty,A_UINT32 port_id, A_INT32 * retp);	
A_STATUS
_CDR_wmi_pwm_port_set_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 freq,A_UINT32 duty,A_UINT32 phase,A_UINT32 port_id, A_UINT32 src_clk, A_INT32 * retp);
 A_STATUS
_CDR_wmi_i2cm_init_cmd(WMI_STRUCT_CDR * wmip,A_UINT8 pin_pair_id, A_INT8 freq,A_UINT32 timeout, A_INT32 * retp);
A_STATUS
_CDR_wmi_i2cm_ctrl_cmd(WMI_STRUCT_CDR * wmip,A_UINT8 pin_pair_id,A_UCHAR *tx_data, A_INT32 tx_len, A_UCHAR *rx_data, A_INT32 rx_len, A_INT32 * retp);
A_STATUS _CDR_wmi_i2cs_csr_cmd(WMI_STRUCT_CDR * wmip,I2CS_CSR_PARA *csr_params);
A_STATUS _CDR_wmi_i2cs_fifo_cmd(WMI_STRUCT_CDR * wmip,I2CS_FIFO_PARA *fifo_params);
A_STATUS _CDR_wmi_i2cs_reg_cmd(WMI_STRUCT_CDR * wmip,I2CS_REG_PARA *reg_params);
A_STATUS _CDR_wmi_i2cs_cmd_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd,A_UINT32* data,A_INT32 * retp);
A_STATUS _CDR_wmi_i2cs_ctrl_cmd(WMI_STRUCT_CDR * wmip,int enable, A_UINT32 pin_pair_id);

A_STATUS _CDR_wmi_adc_cmd(WMI_STRUCT_CDR *wmip, void *cmd, int length);
A_STATUS _CDR_wmi_ebt_mac_filter_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);

A_STATUS _CDR_wmi_ota_session_start_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_get_size_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_erase_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_erase_sectors_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_parse_image_hdr_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_verify_checksum_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_write_data_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
A_STATUS _CDR_wmi_ssdp_init_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod, A_UINT32 localIpAddr, A_UCHAR *locationStr,
				A_UCHAR *serverStr, A_UCHAR *searchTargetStr,
				A_UCHAR *domainName, A_UCHAR *serviceType,
				A_UCHAR *deviceType, A_UCHAR *uuidStr);
A_STATUS _CDR_wmi_ssdp_enable_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 enable);
A_STATUS _CDR_wmi_ssdp_notify_change_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod);
A_STATUS _CDR_wmi_http_get_datasend_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 sess_index, A_UINT16 data_len, 
                      A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 * header_data);
A_STATUS _CDR_wmi_http_client_body_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
A_STATUS _CDR_wmi_http_set_custom_url_cmd(WMI_STRUCT_CDR *wmip, const char* const *new_url);
A_STATUS _CDR_wmi_httpsvr_set_ota_uri_cmd(WMI_STRUCT_CDR *wmip, const char* ota_uri);
A_STATUS _CDR_wmi_httpsvr_set_ota_enable_cmd(WMI_STRUCT_CDR *wmip, A_INT32 enable);
A_STATUS _CDR_wmi_restrict_http_request_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
A_STATUS _CDR_wmi_http_redirect_unknown_url_enable_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
A_STATUS _CDR_wmi_http_server_add_redirected_page_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
A_STATUS _CDR_wmi_http_set_redirected_url_cmd(WMI_STRUCT_CDR *wmip, char *url);
A_STATUS _CDR_wmi_set_bmiss_time_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
A_STATUS _CDR_wmi_set_vendor_specific_ie_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 mgmt_frm_type,A_UINT8 *ie_info,A_UINT16 len);

A_STATUS _CDR_wmi_ant_div_set_cmd(WMI_STRUCT_CDR * wmip, int div, int adjust);
A_STATUS _CDR_wmi_ant_div_get_cmd(WMI_STRUCT_CDR * wmip);

#define WMI_CDR_API_FN(fn) fn


#endif



#define CDR_wmi_dev_init() \
	WMI_CDR_API_FN(_CDR_wmi_dev_init())

#define CDR_wmi_dev_get(_dev) \
	WMI_CDR_API_FN(_CDR_wmi_dev_get((_dev)))

#define CDR_wmi_connect_cmd(_wmip, _type, _dot11AuthMode, _authMode, _pc, _pcl, _gc, _gcl,  \
								_ssdil, _ssid, _bssid, _ch, _ctl) \
	WMI_CDR_API_FN(_CDR_wmi_connect_cmd((_wmip), (_type), (_dot11AuthMode), (_authMode), \
								(_pc), (_pcl), (_gc), (_gcl), (_ssdil), (_ssid), (_bssid), (_ch), (_ctl)))

#define CDR_wmi_listen_interval_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_listen_interval_cmd((_wmip), (_val)))

#define CDR_wmi_ap_set_beacon_int_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_ap_set_beacon_int_cmd((_wmip), (_val)))
	
#define CDR_wmi_ap_hidden_ssid_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_ap_hidden_ssid_cmd((_wmip), (_val)))

#define CDR_wmi_ap_set_num_sta_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_ap_set_num_sta_cmd((_wmip), (_val)))

#define CDR_wmi_ap_set_country_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_ap_set_country_cmd((_wmip), (_val)))

A_STATUS _CDR_wmi_set_sta_country_code(WMI_STRUCT_CDR *wmip, A_CHAR *country);
#define CDR_wmi_set_sta_country_code(_wmip, _val) \
    _CDR_wmi_set_sta_country_code((_wmip), (_val))

A_STATUS _CDR_wmi_wps_config_state_set_cmd(WMI_STRUCT_CDR *wmip, WMI_WPS_CONFIG_SET_CMD *wpsConfigStateSet);
#define CDR_wmi_wps_config_state_set_cmd(wmip, val) \
    _CDR_wmi_wps_config_state_set_cmd((wmip), (val))

A_STATUS _CDR_wmi_set_ipv6_status_cmd(WMI_STRUCT_CDR * wmip,void *cmd);
#define CDR_wmi_set_ipv6_status_cmd(wmip, val) \
    _CDR_wmi_set_ipv6_status_cmd((wmip), (val))
#define CDR_wmi_allow_aggr(_wmip, _tx, _rx) \
	WMI_CDR_API_FN(_CDR_wmi_allow_aggr((_wmip), (_tx), (_rx)))

#define CDR_wmi_addKey_cmd(_wmip, _index, _type, _usage, _len, _rsc, \
								_mt, _ctrl, _mac, _flag) \
	WMI_CDR_API_FN(_CDR_wmi_addKey_cmd((_wmip), (_index), (_type), (_usage), (_len), (_rsc), \
								(_mt), (_ctrl), (_mac), (_flag)))

#define CDR_wmi_set_passphrase_cmd(_wmip, _ssid, _ssidl, _passphrase, _passl) \
	WMI_CDR_API_FN(_CDR_wmi_set_passphrase_cmd((_wmip), (_ssid), (_ssidl), (_passphrase), (_passl)))

#define CDR_wmi_scanparams_cmd(_wmip, _start, _end, _bg, _min, _max, _pas, \
								_scanRatio, _scanCtrl, \
								_max_dfsch_act_time, _maxact_scan_per_ssid) \
	WMI_CDR_API_FN(_CDR_wmi_scanparams_cmd((_wmip), (_start), (_end), (_bg), (_min), (_max), (_pas), \
								(_scanRatio), (_scanCtrl), \
								(_max_dfsch_act_time), (_maxact_scan_per_ssid)))

#define CDR_wmi_bssfilter_cmd(_wmip, _filter, _mask) \
	WMI_CDR_API_FN(_CDR_wmi_bssfilter_cmd((_wmip), (_filter), (_mask)))

#define CDR_wmi_probedSsid_cmd(_wmip, _index, _flag, _len, _ssid) \
	WMI_CDR_API_FN(_CDR_wmi_probedSsid_cmd((_wmip), (_index), (_flag), (_len), (_ssid)))

#define CDR_wmi_startscan_cmd(_wmip, _type, _force, _legacy, _time, _interval, _num, _chanList) \
	WMI_CDR_API_FN(_CDR_wmi_startscan_cmd((_wmip), (_type), (_force), (_legacy), (_time), \
											(_interval), (_num), (_chanList)))

#define CDR_wmi_disconnect_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_disconnect_cmd((_wmip)))

#define CDR_wmi_set_pmk_cmd(_wmip, _pmk, _len) \
	WMI_CDR_API_FN(_CDR_wmi_set_pmk_cmd((_wmip), (_pmk), (_len)))

#define CDR_wmi_set_promiscuous_mode(_wmip, _en) \
	WMI_CDR_API_FN(_CDR_wmi_set_promiscuous_mode((_wmip), (_en)))

#define CDR_wmi_reconnect_cmd(_wmip, _bssid, _chan) \
	WMI_CDR_API_FN(_CDR_wmi_reconnect_cmd((_wmip), (_bssid), (_chan)))


#define CDR_wmi_set_txPwr_cmd(_wmip, _dbM) \
	WMI_CDR_API_FN(_CDR_wmi_set_txPwr_cmd((_wmip), (_dbM)))

#define CDR_wmi_set_channelParams_cmd(_wmip, _scanParam, _mode, _num, _chanList) \
	WMI_CDR_API_FN(_CDR_wmi_set_channelParams_cmd((_wmip), (_scanParam), (_mode), \
														(_num), (_chanList)))

#define CDR_wmi_set_ht_cap_cmd(_wmip, _band,_en, _cw40, _sgi20, _sgi40, _intolerance40, _maxLen) \
	WMI_CDR_API_FN(_CDR_wmi_set_ht_cap_cmd((_wmip), (_band),(_en), (_cw40), (_sgi20), (_sgi40), \
												(_intolerance40), (_maxLen)))

#define CDR_wmi_set_channel_cmd(_wmip, _chan) \
	WMI_CDR_API_FN(_CDR_wmi_set_channel_cmd((_wmip), (_chan)))

#define CDR_wmi_abort_scan_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_abort_scan_cmd((_wmip)))

#define CDR_wmi_pmparams_cmd(_wmip, _idle, _psPollNum, _dtim, _txWakeupPolicy, _num, _failPoicy) \
	WMI_CDR_API_FN(_CDR_wmi_pmparams_cmd((_wmip), (_idle), (_psPollNum), (_dtim), \
												(_txWakeupPolicy), (_num), (_failPoicy)))

#define CDR_wmi_powermode_cmd(_wmip, _mode) \
	WMI_CDR_API_FN(_CDR_wmi_powermode_cmd((_wmip), (_mode)))

#define CDR_wmi_susp_enable_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_susp_enable_cmd((_wmip)))

#define CDR_wmi_susp_start_cmd(_wmip, _time) \
	WMI_CDR_API_FN(_CDR_wmi_susp_start_cmd((_wmip), (_time)))

#define CDR_wmi_wm_susp_cmd(_wmip, _time, _flag) \
	WMI_CDR_API_FN(_CDR_wmi_wm_susp_cmd((_wmip), (_time), (_flag)))

#define CDR_wmi_wm_gpiowakeupcfg_cmd(_wmip, _options, _wake_msec, _sleepOnGPIO_flag) \
	WMI_CDR_API_FN(_CDR_wmi_wm_gpiowakeupcfg_cmd((_wmip), (_options), (_wake_msec), (_sleepOnGPIO_flag)))

#define CDR_wmi_rssi_get_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_rssi_get_cmd((_wmip)))

#define CDR_wmi_wps_start_cmd(_wmip, _start) \
	WMI_CDR_API_FN(_CDR_wmi_wps_start_cmd((_wmip), (_start)))

#define CDR_wmi_ap_profile_commit(_wmip, _type, _dot11AuthMode, _authMode, _pct, _pcl, _gct, _gcl, \
									_ssidl, _ssid, _channel, _bssid, _ctrl) \
	WMI_CDR_API_FN(_CDR_wmi_ap_profile_commit((_wmip), (_type), (_dot11AuthMode), (_authMode), \
									(_pct), (_pcl), (_gct), (_gcl), (_ssidl), (_ssid), (_channel), (_bssid), (_ctrl)))

#define CDR_wmi_sock_stack_init_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_sock_stack_init_cmd((_wmip)))

#define CDR_wmi_dns_get_cmd(_wmip, _mode, _dm, _host) \
	WMI_CDR_API_FN(_CDR_wmi_dns_get_cmd((_wmip), (_mode), (_dm), (_host)))

#define CDR_wmi_dns_en_cmd(_wmip, _cmd) \
	WMI_CDR_API_FN(_CDR_wmi_dns_en_cmd((_wmip), (_cmd)))

#define CDR_wmi_dns_srv_cmd(_wmip, _ip, _type, _mode) \
	WMI_CDR_API_FN(_CDR_wmi_dns_srv_cmd((_wmip), (_ip), (_type), (_mode)))

#define CDR_wmi_dns_domain_cmd(_wmip, _name) \
		WMI_CDR_API_FN(_CDR_wmi_dns_domain_cmd((_wmip), (_name)))

#define CDR_wmi_dns_srv_en_cmd(_wmip, _cmd) \
	WMI_CDR_API_FN(_CDR_wmi_dns_srv_en_cmd((_wmip), (_cmd)))

#define CDR_wmi_dns_update_cmd(_wmip, _cmd, _host, _ipa) \
	WMI_CDR_API_FN(_CDR_wmi_dns_update_cmd((_wmip), (_cmd), (_host), (_ipa)))

#define CDR_wmi_dns_6update_cmd(_wmip, _cmd, _host, _ipa) \
	WMI_CDR_API_FN(_CDR_wmi_dns_6update_cmd((_wmip), (_cmd), (_host), (_ipa)))

#define CDR_wmi_ip_config_cmd(_wmip, _mode, _ipa, _mask, _gw) \
	WMI_CDR_API_FN(_CDR_wmi_ip_config_cmd((_wmip), (_mode), (_ipa), (_mask), (_gw)))

#define CDR_wmi_ip6_config_cmd(_wmip) \
	WMI_CDR_API_FN(_CDR_wmi_ip6_config_cmd((_wmip)))

#define CDR_wmi_http_server(_wmip, _en, _ctx) \
	WMI_CDR_API_FN(_CDR_wmi_http_server((_wmip), (_en), (_ctx)))

#define CDR_wmi_http_server_cmd(_wmip, _cmd, _page, _obj, _type, _len, _val) \
	WMI_CDR_API_FN(_CDR_wmi_http_server_cmd((_wmip), (_cmd), (_page), (_obj), \
												(_type), (_len), (_val)))

#define CDR_wmi_http_client_cmd(_wmip, _cmd, _url, _data, _ctx) \
	WMI_CDR_API_FN(_CDR_wmi_http_client_cmd((_wmip), (_cmd), (_url), (_data), (_ctx)))

#define CDR_wmi_ipconfig_dhcp_pool_cmd(_wmip, _start, _end, _time) \
	WMI_CDR_API_FN(_CDR_wmi_ipconfig_dhcp_pool_cmd((_wmip), (_start), (_end), (_time)))

#define CDR_wmi_socket_cmd(_wmip, _cmd, _len) \
	WMI_CDR_API_FN(_CDR_wmi_socket_cmd((_wmip), (_cmd), (_len)))

#define CDR_wmi_ping_cmd(_wmip, _ipa, _len) \
	WMI_CDR_API_FN(_CDR_wmi_ping_cmd((_wmip), (_ipa), (_len)))

#define CDR_wmi_ping6_cmd(_wmip, _ipa, _len) \
	WMI_CDR_API_FN(_CDR_wmi_ping6_cmd((_wmip), (_ipa), (_len)))

#define CDR_wmi_ip6_router_prefix_cmd(_wmip, _addr, _len, _preLifeTime, _validLifeTime) \
	WMI_CDR_API_FN(_CDR_wmi_ip6_router_prefix_cmd((_wmip), (_addr), (_len), (_preLifeTime), \
														(_validLifeTime)))

#define CDR_wmi_sntp_en_cmd(_wmip, _en) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_en_cmd((_wmip), (_en)))

#define CDR_wmi_sntp_srv_cmd(_wmip, _sntp_srv) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_srv_cmd((_wmip), (_sntp_srv)))

#define CDR_wmi_sntp_zone_cmd(_wmip, _sntp_zone) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_zone_cmd((_wmip), (_sntp_zone)))

#define CDR_wmi_sntp_get_time_cmd(_wmip, _time) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_get_time_cmd((_wmip), (_time)))

#define CDR_wmi_sntp_show_config(_wmip, _sntp_show_config) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_show_config((_wmip), (_sntp_show_config)))

#define CDR_wmi_sntp_get_time_of_day(_wmip, _sntp_get_time_of_day) \
	WMI_CDR_API_FN(_CDR_wmi_sntp_get_time_of_day((_wmip), (_sntp_get_time_of_day)))

#ifdef BRIDGE_ENABLED
#define CDR_wmi_bridge_mode_en_cmd(_wmip, _bridgemode) \
	WMI_CDR_API_FN(_CDR_wmi_bridge_mode_en_cmd((_wmip), (_bridgemode)))
#endif


#if defined(AR6002_REV74)

   	A_STATUS
	_CDR_wmi_ap_set_inact_time_cmd(WMI_STRUCT_CDR * wmip, A_UINT16 inact_time);
    A_STATUS
    _CDR_wmi_ipconfig_dhcp_release_cmd(WMI_STRUCT_CDR * wmip); 
    A_STATUS
    _CDR_wmi_ipconfig_set_tcp_exp_backoff(WMI_STRUCT_CDR * wmip, A_INT32 retry);
    A_STATUS
    _CDR_wmi_ip4_route_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    _CDR_wmi_ip6_route_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32*
        ifindex);
   A_STATUS _CDR_wmi_ota_upgrade_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
   A_STATUS _CDR_wmi_ota_read_ota_area_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
   A_STATUS _CDR_wmi_ota_done_cmd(WMI_STRUCT_CDR * wmip,void * cmd);

   A_STATUS _CDR_wmi_set_dhcp_hostname_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
    A_STATUS _CDR_wmi_lpl_force_enable_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
    A_STATUS _CDR_wmi_gtx_enable_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 enable);
    A_STATUS _CDR_wmi_rate_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 mcs, A_UINT8 rate);
         
#define CDR_wmi_ap_set_inact_time_cmd(_wmip, _val) \
	_CDR_wmi_ap_set_inact_time_cmd((_wmip), (_val))

#define CDR_wmi_ipconfig_dhcp_release_cmd(wmip)  \
   _CDR_wmi_ipconfig_dhcp_release_cmd((wmip))

#define CDR_wmi_ipconfig_set_tcp_exp_backoff(wmip, retry) \
        _CDR_wmi_ipconfig_set_tcp_exp_backoff((wmip), (retry))

#define CDR_wmi_ip4_route_cmd(wmip,cmd, addr, subnet, gw, ifindex) \
    _CDR_wmi_ip4_route_cmd(wmip,cmd, addr, subnet, gw, ifindex)

#define CDR_wmi_ip6_route_cmd(wmip,cmd, ip6addr, prefixLen, gw, ifindex)  \
       WMI_CDR_API_FN( _CDR_wmi_ip6_route_cmd((wmip),(cmd), (ip6addr), (prefixLen), (gw), (ifindex)))

#define  CDR_wmi_ota_upgrade_cmd(wmip,cmd)\
        _CDR_wmi_ota_upgrade_cmd((wmip),(cmd))

#define  CDR_wmi_ota_read_ota_area_cmd(wmip,cmd)\
        _CDR_wmi_ota_read_ota_area_cmd((wmip),(cmd))

#define  CDR_wmi_ota_done_cmd(wmip,cmd)\
            _CDR_wmi_ota_done_cmd((wmip),(cmd))
    
#define CDR_wmi_lpl_force_enable_cmd(wmip, enable) \
		_CDR_wmi_lpl_force_enable_cmd((wmip), (enable))
   
#define CDR_wmi_gtx_enable_cmd(wmip, enable) \
            _CDR_wmi_gtx_enable_cmd((wmip), (enable))

#define CDR_wmi_rate_cmd(wmip, mcs, rate) \
            _CDR_wmi_rate_cmd((wmip), (mcs), (rate))
#define  CDR_wmi_set_dhcp_hostname_cmd(wmip,cmd)\
        _CDR_wmi_set_dhcp_hostname_cmd((wmip), (cmd))

#else

	A_STATUS _CDR_wmi_tcp_conn_timeout_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
#define  CDR_wmi_tcp_conn_timeout_cmd(wmip,cmd)\
         _CDR_wmi_tcp_conn_timeout_cmd((wmip), (cmd))
       void _CDR_wmi_perform_ba_cmd(WMI_STRUCT_CDR * wmip, A_UINT8  tid);
#define  CDR_wmi_perform_ba_cmd(wmip,tid) \
         _CDR_wmi_perform_ba_cmd((wmip), (tid))
	A_STATUS
	_CDR_wmi_set_roam_ctrl_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 roamCtrlType, A_UINT8 *buf);
#define CDR_wmi_set_roam_ctrl_cmd(wmip, mcs, rate) \
				_CDR_wmi_set_roam_ctrl_cmd((wmip), (mcs), (rate))
	A_STATUS _CDR_wmi_set_keep_alive_cmd(WMI_STRUCT_CDR * wmip, A_UINT32 keepAliveInterval);
#define  CDR_wmi_set_keep_alive_cmd(wmip, keepAliveInterval)\
        _CDR_wmi_set_keep_alive_cmd((wmip), (keepAliveInterval))		
        
#define CDR_wmi_ap_set_inact_time_cmd(_wmip, _val) \
	WMI_CDR_API_FN(_CDR_wmi_ap_set_inact_time_cmd((_wmip), (_val)))

#define CDR_wmi_ipconfig_dhcp_release_cmd(wmip)  \
    WMI_CDR_API_FN(_CDR_wmi_ipconfig_dhcp_release_cmd((wmip)))

#define CDR_wmi_ipconfig_set_tcp_exp_backoff(wmip, retry) \
    WMI_CDR_API_FN( _CDR_wmi_ipconfig_set_tcp_exp_backoff((wmip), (retry)))

#define CDR_wmi_ip4_route_cmd(wmip,cmd, addr, subnet, gw, ifindex) \
    WMI_CDR_API_FN(_CDR_wmi_ip4_route_cmd(wmip,cmd, addr, subnet, gw, ifindex))

#define CDR_wmi_ip6_route_cmd(wmip,cmd, ip6addr, prefixLen, gw, ifindex)  \
    WMI_CDR_API_FN(_CDR_wmi_ip6_route_cmd((wmip),(cmd), (ip6addr), (prefixLen), (gw), (ifindex)))

#define  CDR_wmi_ota_upgrade_cmd(wmip,cmd)\
           WMI_CDR_API_FN(_CDR_wmi_ota_upgrade_cmd((wmip),(cmd)))

#define  CDR_wmi_ota_read_ota_area_cmd(wmip,cmd)\
           WMI_CDR_API_FN(_CDR_wmi_ota_read_ota_area_cmd((wmip),(cmd))) 

#define  CDR_wmi_ota_done_cmd(wmip,cmd)\
               WMI_CDR_API_FN(_CDR_wmi_ota_done_cmd((wmip),(cmd))) 
               
#if defined(AR6002_REV76)

A_STATUS _CDR_wmi_ota_session_start_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_get_size_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_erase_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_erase_sectors_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_parse_image_hdr_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_verify_checksum_cmd(WMI_STRUCT_CDR * wmip,void * cmd);
A_STATUS _CDR_wmi_ota_partition_write_data_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
A_STATUS _CDR_wmi_ssdp_init_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod, A_UINT32 localIpAddr, A_UCHAR *locationStr,\
				A_UCHAR *serverStr, A_UCHAR *searchTargetStr, \
				A_UCHAR *domainName, A_UCHAR *serviceType, \
				A_UCHAR *deviceType, A_UCHAR *uuidStr);
A_STATUS _CDR_wmi_ssdp_enable_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 enable);
A_STATUS _CDR_wmi_ssdp_notify_change_cmd(WMI_STRUCT_CDR *wmip, A_UINT32 notifyPeriod);
A_STATUS _CDR_wmi_http_get_datasend_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 sess_index, A_UINT16 data_len, 
                      A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8* header_data);
A_STATUS _CDR_wmi_http_set_custom_url_cmd(WMI_STRUCT_CDR *wmip, const char* const *new_url);
A_STATUS _CDR_wmi_httpsvr_set_ota_uri_cmd(WMI_STRUCT_CDR *wmip, const char* ota_uri);
A_STATUS _CDR_wmi_httpsvr_set_ota_enable_cmd(WMI_STRUCT_CDR *wmip, A_INT32 enable);
A_STATUS _CDR_wmi_restrict_http_request_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
A_STATUS _CDR_wmi_http_redirect_unknown_url_enable_cmd(WMI_STRUCT_CDR *wmip, A_UINT8 enable);
A_STATUS _CDR_wmi_http_server_add_redirected_page_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
A_STATUS _CDR_wmi_http_set_redirected_url_cmd(WMI_STRUCT_CDR *wmip, char *url);
A_STATUS _CDR_wmi_param_set_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 which_param,void* data,A_UINT32 data_length);
A_STATUS _CDR_wmi_param_get_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 which_param);
A_STATUS _CDR_wmi_http_client_body_cmd(WMI_STRUCT_CDR * wmip,A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
A_STATUS _CDR_wmi_set_bmiss_time_cmd(WMI_STRUCT_CDR *wmip, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
A_STATUS _CDR_wmi_set_vendor_specific_ie_cmd(WMI_STRUCT_CDR *wmip,A_UINT8 mgmt_frm_type,A_UINT8 *ie_info,A_UINT16 len);
A_STATUS _CDR_wmi_ant_div_set_cmd(WMI_STRUCT_CDR * wmip, int div, int adjust);
A_STATUS _CDR_wmi_ant_div_get_cmd(WMI_STRUCT_CDR * wmip);
A_STATUS _CDR_wmi_http_client_register_cb_cmd(WMI_STRUCT_CDR * wmip, void* callback, void* arg);

#ifdef HTTPC_MULTI_SESSION
A_STATUS _CDR_wmi_http_client_connect_cmd(WMI_STRUCT_CDR* wmip, void* client, const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, A_BOOL cb_enabled);
A_STATUS _CDR_wmi_http_client_disconnect_cmd(WMI_STRUCT_CDR* wmip, A_UINT32 client, A_UINT32 session);
A_STATUS _CDR_wmi_http_client_request_cmd(WMI_STRUCT_CDR* wmip, A_UINT32 session, A_UINT32 cmd, const A_CHAR *url, A_CHAR *output);
A_STATUS _CDR_wmi_http_client_set_body_cmd(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *body, A_UINT32 bodylen);
A_STATUS _CDR_wmi_http_client_set_param_cmd(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *key, const A_CHAR *value);
A_STATUS _CDR_wmi_http_client_add_header_cmd(WMI_STRUCT_CDR* wmip, A_UINT32 session, const A_CHAR *header, const A_CHAR *value);
A_STATUS _CDR_wmi_http_client_clear_header_cmd(WMI_STRUCT_CDR* wmip, A_UINT32  session);
#endif

A_STATUS _CDR_wmi_btcoex_scheme_set_cmd(WMI_STRUCT_CDR * wmip, A_UINT8 scheme_id, void* data);

#define  CDR_wmi_ota_session_start_cmd(wmip,cmd) \
            _CDR_wmi_ota_session_start_cmd((wmip),(cmd))    

#define  CDR_wmi_ota_partition_get_size_cmd(wmip,cmd) \
            _CDR_wmi_ota_partition_get_size_cmd((wmip),(cmd))
            
#define  CDR_wmi_ota_partition_erase_cmd(wmip,cmd) \
            _CDR_wmi_ota_partition_erase_cmd((wmip),(cmd))
            
#define  CDR_wmi_ota_partition_erase_sectors_cmd(wmip,cmd) \
            _CDR_wmi_ota_partition_erase_sectors_cmd((wmip),(cmd))

#define  CDR_wmi_ota_parse_image_hdr_cmd(wmip,cmd) \
            _CDR_wmi_ota_parse_image_hdr_cmd((wmip),(cmd))

#define  CDR_wmi_ota_partition_verify_checksum_cmd(wmip,cmd) \
            _CDR_wmi_ota_partition_verify_checksum_cmd((wmip),(cmd))

#define  CDR_wmi_ota_partition_write_data_cmd(wmip,offset,buf,size) \
            _CDR_wmi_ota_partition_write_data_cmd((wmip),(offset),(buf),(size))

#define CDR_wmi_ssdp_init_cmd(wmip, notifyPeriod, localIpAddr, locationStr, serverStr, searchTargetStr, domainName, serviceType, deviceType, uuidStr) \
            _CDR_wmi_ssdp_init_cmd((wmip), (notifyPeriod), (localIpAddr), (locationStr), (serverStr), (searchTargetStr), (domainName), (serviceType), (deviceType), (uuidStr))

#define  CDR_wmi_ssdp_enable_cmd(wmip, enable) \
            _CDR_wmi_ssdp_enable_cmd((wmip), (enable))	

#define  CDR_wmi_ssdp_notify_change_cmd(wmip, notifyPeriod) \
            _CDR_wmi_ssdp_notify_change_cmd((wmip), (notifyPeriod))

#define CDR_wmi_http_get_datasend_cmd(wmip, sess_index, data_len, url_str, data, header_len, header_data) \
         _CDR_wmi_http_get_datasend_cmd(wmip, sess_index,  data_len, url_str, data, header_len, header_data)

#define CDR_wmi_http_set_custom_url_cmd(wmip, new_url) \
         _CDR_wmi_http_set_custom_url_cmd(wmip, new_url)

#define CDR_wmi_httpsvr_set_ota_uri_cmd(wmip, ota_uri) \
         _CDR_wmi_httpsvr_set_ota_uri_cmd(wmip, ota_uri)

#define CDR_wmi_httpsvr_set_ota_enable_cmd(wmip, enable) \
         _CDR_wmi_httpsvr_set_ota_enable_cmd(wmip, enable)

#define CDR_wmi_restrict_http_request_cmd(wmip, enable) \
         _CDR_wmi_restrict_http_request_cmd(wmip, enable)

#define CDR_wmi_http_redirect_unknown_url_enable_cmd(wmip, enable) \
         _CDR_wmi_http_redirect_unknown_url_enable_cmd(wmip, enable)

#define CDR_wmi_http_server_add_redirected_page_cmd(wmip, header_len, header, data_len, data, url) \
        _CDR_wmi_http_server_add_redirected_page_cmd(wmip, header_len, header, data_len, data, url)

#define CDR_wmi_http_set_redirected_url_cmd(wmip, url) \
        _CDR_wmi_http_set_redirected_url_cmd(wmip, url)

#define  CDR_wmi_param_set_cmd(wmip,which_param,data,data_length)\
         _CDR_wmi_param_set_cmd((wmip),(which_param),(data),(data_length))

#define  CDR_wmi_param_get_cmd(wmip,which_param)\
         _CDR_wmi_param_get_cmd((wmip), (which_param))

#define  CDR_wmi_ap_sta_info_get_cmd(wmip)\
         _CDR_wmi_ap_sta_info_get_cmd((wmip))          
 
#define  CDR_wmi_http_client_body_cmd(wmip, cmd, body, bodylen) \
         _CDR_wmi_http_client_body_cmd((wmip), (cmd), (body), (bodylen))

#define CDR_wmi_set_bmiss_time_cmd(wmip, bmiss_num, bmiss_time) \
    _CDR_wmi_set_bmiss_time_cmd((wmip), (bmiss_num), (bmiss_time))
    
#define  CDR_wmi_set_vendor_specific_ie_cmd(wmip, mgmt_frm_type,ie_info,len)\
     _CDR_wmi_set_vendor_specific_ie_cmd((wmip),(mgmt_frm_type),(ie_info),(len))
  
#define CDR_wmi_ant_div_set_cmd(_wmip, div, adjust) \
         _CDR_wmi_ant_div_set_cmd((_wmip), (div), (adjust))

#define CDR_wmi_ant_div_get_cmd(_wmip) \
         _CDR_wmi_ant_div_get_cmd((_wmip))

#define CDR_wmi_http_client_register_cb_cmd(_wmip, callback, arg) \
         _CDR_wmi_http_client_register_cb_cmd(_wmip, callback, arg)

#ifdef HTTPC_MULTI_SESSION
#define CDR_wmi_http_client_connect_cmd(wmip, client, server, port, timeout, ssl_ctx, cb_enabled) \
         _CDR_wmi_http_client_connect_cmd(wmip, client, server, port, timeout, ssl_ctx, cb_enabled)

#define CDR_wmi_http_client_disconnect_cmd(wmip, client, session) \
         _CDR_wmi_http_client_disconnect_cmd(wmip, client, session)

#define CDR_wmi_http_client_request_cmd(wmip, session, cmd, url, output) \
         _CDR_wmi_http_client_request_cmd(wmip, session, cmd, url, output)

#define CDR_wmi_http_client_set_body_cmd(wmip, session, body, bodylen) \
         _CDR_wmi_http_client_set_body_cmd(wmip, session, body, bodylen)

#define CDR_wmi_http_client_set_param_cmd(wmip, session, key, value) \
         _CDR_wmi_http_client_set_param_cmd(wmip, session, key, value)

#define CDR_wmi_http_client_add_header_cmd(wmip, session, header, value) \
         _CDR_wmi_http_client_add_header_cmd(wmip, session, header, value)

#define CDR_wmi_http_client_clear_header_cmd(wmip, session) \
         _CDR_wmi_http_client_clear_header_cmd(wmip, session)
#endif

#define CDR_wmi_btcoex_scheme_set_cmd(wmip, scheme_id, data) \
         _CDR_wmi_btcoex_scheme_set_cmd(wmip, scheme_id, data) 

#else // AR6002_REV76

#define  CDR_wmi_ota_session_start_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_session_start_cmd((wmip),(cmd))) 

#define  CDR_wmi_ota_partition_get_size_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_partition_get_size_cmd((wmip),(cmd)))
            
#define  CDR_wmi_ota_partition_erase_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_partition_erase_cmd((wmip),(cmd)))
            
#define  CDR_wmi_ota_partition_erase_sectors_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_partition_erase_sectors_cmd((wmip),(cmd)))

#define  CDR_wmi_ota_parse_image_hdr_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_parse_image_hdr_cmd((wmip),(cmd)))

#define  CDR_wmi_ota_partition_verify_checksum_cmd(wmip,cmd) \
    WMI_CDR_API_FN(_CDR_wmi_ota_partition_verify_checksum_cmd((wmip),(cmd)))

#define  CDR_wmi_ota_partition_write_data_cmd(wmip,offset,buf,size) \
    WMI_CDR_API_FN(_CDR_wmi_ota_partition_write_data_cmd((wmip),(offset),(buf),(size)))

#define CDR_wmi_ssdp_init_cmd(wmip, notifyPeriod, localIpAddr, locationStr, serverStr, searchTargetStr, domainName, serviceType, deviceType, uuidStr) \
	WMI_CDR_API_FN(_CDR_wmi_ssdp_init_cmd((wmip), (notifyPeriod), (localIpAddr), (locationStr), (serverStr), (searchTargetStr), (domainName), (serviceType), (deviceType), (uuidStr))) \

#define  CDR_wmi_ssdp_enable_cmd(wmip, enable) \
      	WMI_CDR_API_FN(_CDR_wmi_ssdp_enable_cmd((wmip), (enable)))	

#define  CDR_wmi_ssdp_notify_change_cmd(wmip, notifyPeriod) \
      WMI_CDR_API_FN(_CDR_wmi_ssdp_notify_change_cmd((wmip), (notifyPeriod)))

#define CDR_wmi_http_get_datasend_cmd(wmip, sess_index, data_len, url_str, data, header_len, header_data) \
             WMI_CDR_API_FN(*_CDR_wmi_http_get_datasend_cmd((wmip, sess_index,  data_len, url_str, data, header_len, header_data))

#define CDR_wmi_http_set_custom_url_cmd(wmip, new_url) \
         WMI_CDR_API_FN(_CDR_wmi_http_set_custom_url_cmd(wmip, new_url))
    
#define CDR_wmi_httpsvr_set_ota_uri_cmd(wmip, new_uri) \
             WMI_CDR_API_FN(_CDR_wmi_httpsvr_set_ota_uri_cmd(wmip, ota_uri))

#define CDR_wmi_httpsvr_set_ota_enable_cmd(wmip, enable) \
                 WMI_CDR_API_FN(_CDR_wmi_httpsvr_set_ota_enable_cmd(wmip, enable))

#define CDR_wmi_restrict_http_request_cmd(wmip, enable) \
         WMI_CDR_API_FN(_CDR_wmi_restrict_http_request_cmd(wmip, enable))

#define CDR_wmi_http_redirect_unknown_url_enable_cmd(wmip, enable) \
         WMI_CDR_API_FN(_CDR_wmi_http_redirect_unknown_url_enable_cmd(wmip, enable))

#define CDR_wmi_http_server_add_redirected_page_cmd(wmip, header_len, header, data_len, data, url) \
            WMI_CDR_API_FN(_CDR_wmi_http_server_add_redirected_page_cmd((wmip), (header_len), (header), (data_len), (data), (url)))

#define CDR_wmi_http_set_redirected_url_cmd(wmip, url) \
            WMI_CDR_API_FN(_CDR_wmi_http_set_redirected_url_cmd((wmip), (url)))

#define  CDR_wmi_param_set_cmd(wmip,which_param,data,data_length)\
        WMI_CDR_API_FN(_CDR_wmi_param_set_cmd((wmip), (which_param),(data),(data_length)))

#define  CDR_wmi_param_get_cmd(wmip,which_param)\
        WMI_CDR_API_FN(_CDR_wmi_param_get_cmd((wmip), (which_param)))

#define  CDR_wmi_http_client_body_cmd(wmip, cmd, body, bodylen) \
        WMI_CDR_API_FN(_CDR_wmi_http_client_body_cmd((wmip), (cmd), (body), (bodylen)))

#define CDR_wmi_set_bmiss_time_cmd(wmip, bmiss_num, bmiss_time) \
    WMI_CDR_API_FN(_CDR_wmi_set_bmiss_time_cmd((wmip), (bmiss_num), (bmiss_time)))
    
#define  CDR_wmi_set_vendor_specific_ie_cmd(wmip, mgmt_frm_type,ie_info,len)\
     _CDR_wmi_set_vendor_specific_ie_cmd((wmip),(mgmt_frm_type),(ie_info),(len))

#define CDR_wmi_ant_div_set_cmd(_wmip, div, adjust) \
         WMI_CDR_API_FN(_CDR_wmi_ant_div_set_cmd((_wmip), (div), (adjust)))

#define CDR_wmi_ant_div_get_cmd(_wmip) \
         WMI_CDR_API_FN(_CDR_wmi_ant_div_get_cmd((_wmip)))

#define CDR_wmi_http_client_register_cb_cmd(_wmip, callback, arg) \
         WMI_CDR_API_FN(_CDR_wmi_http_client_register_cb_cmd(_wmip, callback, arg))

#ifdef HTTPC_MULTI_SESSION
#define CDR_wmi_http_client_connect_cmd(wmip, client, server, port, timeout, ssl_ctx, cb_enabled) \
         WMI_CDR_API_FN(_CDR_wmi_http_client_connect_cmd(wmip, client, server, port, timeout, ssl_ctx, cb_enabled))

#define CDR_wmi_http_client_disconnect_cmd(wmip, client, session) \
         WMI_CDR_API_FN(_CDR_wmi_http_client_disconnect_cmd(wmip, client, session))
         
#define CDR_wmi_http_client_request_cmd(wmip, session, cmd, url, output) \
		WMI_CDR_API_FN(_CDR_wmi_http_client_request_cmd(wmip, session, cmd, url, output))
	
#define CDR_wmi_http_client_set_body_cmd(wmip, session, body, bodylen) \
		WMI_CDR_API_FN(_CDR_wmi_http_client_set_body_cmd(wmip, session, body, bodylen))

#define CDR_wmi_http_client_set_param_cmd(wmip, session, key, value) \
		WMI_CDR_API_FN(_CDR_wmi_http_client_set_param_cmd(wmip, session, key, value))
	
#define CDR_wmi_http_client_add_header_cmd(wmip, session, header, value) \
		WMI_CDR_API_FN(_CDR_wmi_http_client_add_header_cmd(wmip, session, header, value))
	
#define CDR_wmi_http_client_clear_header_cmd(wmip, session) \
		WMI_CDR_API_FN(_CDR_wmi_http_client_clear_header_cmd(wmip, session))
#endif

#define CDR_wmi_btcoex_scheme_set_cmd(wmip, scheme_id, data) \
		WMI_CDR_API_FN(_CDR_wmi_btcoex_scheme_set_cmd(wmip, scheme_id, data))

#endif // AR6002_REV76

#define  CDR_wmi_set_dhcp_hostname_cmd(wmip,cmd)\
               WMI_CDR_API_FN(_CDR_wmi_set_dhcp_hostname_cmd((wmip),(cmd)))

#define CDR_wmi_lpl_force_enable_cmd(wmip, enable) \
		WMI_CDR_API_FN(_CDR_wmi_lpl_force_enable_cmd((wmip), (enable)))
   
#define CDR_wmi_gtx_enable_cmd(wmip, enable) \
            WMI_CDR_API_FN(_CDR_wmi_gtx_enable_cmd((wmip), (enable)))

#define CDR_wmi_rate_cmd(wmip, mcs, rate) \
            WMI_CDR_API_FN(_CDR_wmi_rate_cmd((wmip), (mcs), (rate)))

#endif
#define CDR_wmi_pwm_ctrl_cmd(wmip, module_select,enable,port_id,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_pwm_ctrl_cmd((wmip),(module_select), (enable), (port_id), (retp)))
 #define CDR_wmi_pwm_sdm_set_cmd(wmip, freq,duty,port_id,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_pwm_sdm_set_cmd((wmip),(freq), (duty), (port_id), (retp)))
 #define CDR_wmi_pwm_port_set_cmd(wmip, freq,duty,phase,port_id,src_clk,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_pwm_port_set_cmd((wmip),(freq), (duty),(phase), (port_id),(src_clk), (retp)))
#define CDR_wmi_i2cm_init_cmd(wmip, pin_pair_id,freq,timeout,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cm_init_cmd((wmip),(pin_pair_id), (freq),(timeout),(retp)))
 #define CDR_wmi_i2cm_ctrl_cmd(wmip, pin_pair_id,tx_data,tx_len,rx_data,rx_len,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cm_ctrl_cmd((wmip),(pin_pair_id), (tx_data), (tx_len),(rx_data), (rx_len),  (retp)))

#define CDR_wmi_i2cs_csr_cmd(wmip, csr_params)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cs_csr_cmd((wmip),(csr_params)))
 #define CDR_wmi_i2cs_fifo_cmd(wmip, fifo_params)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cs_fifo_cmd((wmip),(fifo_params)))
 #define CDR_wmi_i2cs_reg_cmd(wmip, reg_params)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cs_reg_cmd((wmip),(reg_params)))
#define CDR_wmi_i2cs_cmd_cmd(wmip, cmd,data,retp)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cs_cmd_cmd((wmip),(cmd), (data),(retp)))
 #define CDR_wmi_i2cs_ctrl_cmd(wmip, enable, pin_pair_id)  \
    WMI_CDR_API_FN(_CDR_wmi_i2cs_ctrl_cmd((wmip),(enable),( pin_pair_id)))
	
#define CDR_wmi_adc_cmd(_wmip, _cmd, _len) \
		WMI_CDR_API_FN(_CDR_wmi_adc_cmd((_wmip), (_cmd), (_len)))

#define CDR_wmi_ebt_mac_filter_cmd(_wmip, action, ebt, mac_filter) \
    WMI_CDR_API_FN(_CDR_wmi_ebt_mac_filter_cmd((_wmip), (action), (ebt), (mac_filter)))

#ifdef __cplusplus
}
#endif


#endif /* _WMI_H_ */

