/*
 * Copyright (c) 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$
 */

#ifndef __QCOM_CDR__H__
#define __QCOM_CDR__H__

#include "qcom/stdint.h"
#include "qcom/qcom_scan.h"

#ifdef PHOST_INDIRECT_ACCESS

typedef struct  {
	void
	(* _qcom_wlan_connect)(A_UINT8 device_id, NETWORK_TYPE netType, DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
	                  CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen, CRYPTO_TYPE groupCrypto,
	                  A_UINT8 groupCryptoLen, int ssidLength, A_UCHAR * ssid,
	                  A_UINT8 * bssid, A_UINT16 channel, A_UINT32 ctrl_flags);
	void
	(* _qcom_reconnect)(A_UINT8 device_id, A_UINT8 * bssid, A_UINT16 channel);
	void
	(* _qcom_wlan_disconnect)(A_UINT8 device_id);
	void
	(* _qcom_wifi_scan_start)(A_UINT8 device_id, qcom_start_scan_params_t *pScanParams);
                     /*
                                A_UINT8 scanType, A_BOOL forceFgScan,
                                A_UINT32 homeDwellTimeInMs, A_UINT32 forceScanIntervalInMs, 
                                A_UINT8 numChannels, A_UINT16* __qcom_in_ptr  channelList ); */
	void
	(* _qcom_set_scan_params)(A_UINT8 device_id, qcom_scan_params_t *pParams);

	void
	(* _qcom_set_bss_filter)(A_UINT8 device_id, int filter);
	void
	(* _qcom_set_probed_ssid)(A_UINT8 device_id,int index, int flag, char *ssid, int ssidLength);
	void
	(* _qcom_set_listen_time)(A_UINT8 device_id, A_UINT16 time);
	void
	(* _qcom_set_ap_beacon_interval)(A_UINT8 device_id, A_UINT16 beacon_interval);
	void
	(* _qcom_rssi_get)(A_UINT8 device_id, A_UINT16 * rssi);

	void
	(* _qcom_hw_set_phy_mode)(A_UINT8 device_id, int phyMode);
	void
	(* _qcom_set_power_mode)(A_UINT8 device_id, int powerMode);
	void
	(* _qcom_wmi_setPowerParams_cmd)(A_UINT8 device_id, A_UINT16 idlePeriod,
	                            A_UINT16 psPollNum, A_UINT16 dtimPolicy,
	                            A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
	                            A_UINT16 ps_fail_event_policy);
	void
	(* _qcom_add_cipher_key)(A_UINT8 device_id, A_UINT8 keyIndex, CRYPTO_TYPE keyType, A_UINT8 keyUsage, A_UINT8 keyLength,
	                    A_UINT8 * keyRSC, A_UINT8 * key, A_UINT8 key_op_ctrl, A_UINT8 * macAddr,
	                    int sync_flag);
	void
	(* _qcom_add_wep_key)(A_UINT8 device_id, int keyIndx, char *key, int keyLength);

	void
	(* _qcom_wlan_set_tx_power)(A_UINT8 device_id, int dbM);
	void
	(* _qcom_sta_wlan_abort_scan)(A_UINT8 device_id);
	void
	(* _qcom_ap_hidden_ssid)(A_UINT8 device_id, A_UINT8 hidden_ssid);
	void
	(* _qcom_ap_set_num_sta)(A_UINT8 device_id, A_UINT8 num_sta);
	void
	(* _qcom_ap_config_commit)(A_UINT8 device_id, A_UINT8 networkType,
                      A_UINT8 dot11AuthMode,
                      A_UINT8 authMode,
                      A_UINT8 pairwiseCryptoType,
                      A_UINT8 pairwiseCryptoLen,
                      A_UINT8 groupCryptoType,
                      A_UINT8 groupCryptoLen,
                      A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
                      A_UINT32 ctrl_flags);

	A_STATUS
	(* _qcom_set_ap_country)(A_UINT8 device_id, char *country_code);
	void
	(* _qcom_allow_aggr)(A_UINT8 device_id, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
	void
	(* _qcom_set_ht_cap)(A_UINT8 device_id, A_UINT8 band,
	                A_UINT8 enable,
	                A_UINT8 chan_width_40M_supported,
	                A_UINT8 short_GI_20MHz,
	                A_UINT8 short_GI_40MHz, A_UINT8 intolerance_40MHz, A_UINT8 max_ampdu_len_exp);
	void
	(* _qcom_set_pmk)(A_UINT8 device_id, A_UINT8 * pmk, A_UINT8 pmk_len);
	void
	(* _qcom_set_promiscuous_mode)(A_UINT8 enable);

	void
	(* _qcom_hw_set_channel)(A_UINT8 device_id, A_UINT16 channel);
	void
	(* _qcom_set_passphrase)(A_UINT8 device_id, A_UCHAR * ssid, A_UINT8 ssidLength, A_UINT8 * passphrase, A_UINT8 passLength);
	void
	(* _qcom_wlan_suspend_enable)(void);
	void
	(* _qcom_wlan_suspend_start)(A_UINT32 time);
	void
	(* _qcom_sock_stack_init)(A_UINT8 device_id);

	void
	(* _qcom_dns_get_request)(A_UINT32 mode, A_UINT32 domain, char *hostname);
//#ifdef BRIDGE_ENABLED
	void
	(* _qcom_enable_bridge_mode)(A_UINT16 bridgemode);
//#endif
	void
	(* _qcom_enable_dns_client)(A_INT32 command);
	void
	(* _qcom_dns_server)(A_UINT8 *serv_ip, A_UINT8 type, int mode);
	void
	(* _qcom_enable_dns_server)(A_INT32 command);
	void
	(* _qcom_dns_domain)(A_CHAR* name);
	void
	(* _qcom_dns_update)(A_INT32 flag, A_CHAR* hostname, A_UINT32 ipaddr);
	void
	(* _qcom_dns_6update)(A_INT32 flag, A_CHAR* hostname, A_UINT8* ip6addr);
	void
	(* _qcom_ip_config_request)(A_UINT8 device_id, A_UINT32 mode, A_UINT32 ipv4, A_UINT32 subnetMask, A_UINT32 gateway4);
	void
	(* _qcom_ip6_config_request)(A_UINT8 device_id);
	void
	(* _qcom_ipconfig_dhcp_pool)(A_UINT8 device_id, A_UINT32 startaddr, A_UINT32 endaddr, A_INT32 leasetime);

	A_STATUS
	(* _qcom_ping_request)(A_UINT32 ipv4_addr, A_UINT32 size);
	A_STATUS
	(* _qcom_ping6_request)(A_UINT8* ipv6_addr, A_UINT32 size);
	A_STATUS
	(* _qcom_ip6cfg_router_prefix)(A_UINT8 device_id, A_UINT8* addr, A_INT32 prefix_length, A_INT32 prefix_lifetime, A_INT32 valid_lifetime);
	void
	(* _qcom_get_bp_info)(bpool_info_t * bp_info);
	void
	(* _qcom_http_server_enable)(A_INT32 enable, void *ctx);

	void (* _qcom_http_server_cmd)(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8* value);
	void (* _qcom_http_client_cmd)(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, void *ctx);
	A_STATUS (* _qcom_wm_suspend)(A_UINT32 time, A_UINT32 flag);
	A_STATUS (* _qcom_wm_gpiowakeupcfg)(A_UINT32 options, A_UINT32 wake_msec,  A_UINT32 sleepOnGPIO_flag);
    void (*_qcom_set_ap_inact_time)(A_UINT8 device_id, A_UINT16 inact_time);
    void (*_qcom_ipconfig_release_dhcp_pool)(A_UINT8 device_id);
    void (*_qcom_ipconfig_set_exp_backoff)(A_UINT8 device_id, A_INT32 retry);
    A_STATUS
    (*_qcom_ip4_route_cmd)(A_UINT8 device_id,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    (*_qcom_ip6_route_cmd)(A_UINT8 device_id,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex);
    A_STATUS (*_qcom_ota_upgrade_cmd)(A_UINT8 device_id,void *cmd);
    A_STATUS (*_qcom_ota_read_ota_area_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_done_cmd)(void *cmd);
    A_STATUS (*_qcom_set_dhcp_hostname_cmd)(A_UINT8 device_id,void* cmd);
    void (* _qcom_lpl_force_enable_cmd)(A_UINT8 device_id, A_UINT8 enable);
    void (* _qcom_gtx_enable_cmd)(A_UINT8 device_id, A_UINT8 enable);
    void (* _qcom_rate_cmd)(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);
    A_STATUS (* _qcom_set_get_ebt_mac_filter_params_cmd)(A_UINT8 devid, A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);
    //TODO
	//A_STATUS (*_qcom_set_ipv6_status_cmd)(A_UINT8 device_id,void *cmd);
#if !defined(AR6002_REV76)
    A_STATUS (*_qcom_ota_session_start_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_partition_get_size_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_partition_erase_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_partition_erase_sectors_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_parse_image_hdr_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_partition_verify_checksum_cmd)(void *cmd);
    A_STATUS (*_qcom_ota_partition_write_data_cmd)(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    A_STATUS (*_qcom_ssdp_init_cmd)(A_UINT32 notifyPeriod, A_UINT32 localIpAddr, A_UCHAR *locationStr,
                        A_UCHAR *serverStr, A_UCHAR *searchTargetStr,
                        A_UCHAR *domainName, A_UCHAR *serviceType,
                        A_UCHAR *deviceType, A_UCHAR *uuidStr);	
    A_STATUS (*_qcom_ssdp_enable_cmd)(A_UINT32 enable);
    A_STATUS (*_qcom_ssdp_notify_change_cmd)(A_UINT32 notifyPeriod);
    
    A_STATUS (*_qcom_http_get_datasend_cmd)(A_UINT16 sess_index, A_UINT16 data_len, 
                       					A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
    A_STATUS (*qcom_http_set_custom_url_cmd)(const char* const *new_url);
    A_STATUS (*_qcom_restrict_http_request_cmd)(A_UINT8 device_id, A_UINT8 enable);
    A_STATUS (*_qcom_http_redirect_unknown_url_enable_cmd)(A_UINT8 enable);
    A_STATUS(* _qcom_http_server_add_redirected_page_cmd)(A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
    A_STATUS (*_qcom_http_set_redirected_url_cmd)(char * url);

    void (*_qcom_http_client_body_cmd)(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
    A_STATUS (*_qcom_set_bmiss_time)(A_UINT8 device_id, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
    A_STATUS(*_qcom_set_vendor_specific_ie_cmd)(A_UINT8 device_id, A_UINT8 mgmt_frm_type,A_UINT8 * buffer,A_UINT16 len);
	void (*_qcom_set_dset_maxblk_reclaim_onetime)(A_UINT32 maxblk_reclaim);
	void (*_qcom_ani_enable)(A_BOOL enable);
    A_STATUS (*_qcom_httpsvr_set_ota_uri_cmd)(const char* ota_uri);
    A_STATUS (*_qcom_httpsvr_set_ota_enable_cmd)(A_INT32 enable);
    A_STATUS (*_qcom_ant_div_set)(int div, int adjust);
    A_STATUS (*_qcom_ant_div_get)(void);
	A_STATUS (*_qcom_http_client_register_cb_cmd)(void* callback, void* arg);
#ifdef HTTPC_MULTI_SESSION
    A_STATUS (*_qcom_http_client_connect_cmd)(void* client, const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, A_BOOL cb_enabled);
    A_STATUS (*_qcom_http_client_disconnect_cmd)(A_UINT32 client, A_UINT32  session);
	A_STATUS (*_qcom_http_client_request_cmd)(A_UINT32 session, A_UINT32 cmd, const A_CHAR *url, A_CHAR *output);
	A_STATUS (*_qcom_http_client_set_body_cmd)(A_UINT32 session, const A_CHAR *body, A_UINT32 bodylen);
	A_STATUS (*_qcom_http_client_set_param_cmd)(A_UINT32 session, const A_CHAR *key, const A_CHAR *value);
	A_STATUS (*_qcom_http_client_add_header_cmd)(A_UINT32 session, const A_CHAR *header, const A_CHAR *value);
	A_STATUS (*_qcom_http_client_clear_header_cmd)(A_UINT32  session);
#endif
	A_STATUS (*_qcom_btcoex_scheme_set_cmd)(A_UINT8 scheme_id, void* data);

#endif
}QCOM_CDR_API_INDIRECTION_TABLE;


extern QCOM_CDR_API_INDIRECTION_TABLE qcom_cdr_api_indirect_table;
#define QCOM_CDR_API_FN(fn) qcom_cdr_api_indirect_table.fn


#else


	void
	_qcom_wlan_connect(A_UINT8 device_id, NETWORK_TYPE netType, DOT11_AUTH_MODE dot11AuthMode, AUTH_MODE authMode,
	                  CRYPTO_TYPE pairwiseCrypto, A_UINT8 pairwiseCryptoLen, CRYPTO_TYPE groupCrypto,
	                  A_UINT8 groupCryptoLen, int ssidLength, A_UCHAR * ssid,
	                  A_UINT8 * bssid, A_UINT16 channel, A_UINT32 ctrl_flags);
	void
	_qcom_reconnect(A_UINT8 device_id, A_UINT8 * bssid, A_UINT16 channel);
	void
	_qcom_wlan_disconnect(A_UINT8 device_id);
	void
	_qcom_wifi_scan_start(A_UINT8 device_id, qcom_start_scan_params_t *pScanParams);
	void
	_qcom_set_scan_params(A_UINT8 device_id, qcom_scan_params_t *pParams);

	void
	_qcom_set_bss_filter(A_UINT8 device_id, int filter);
	void
	_qcom_set_probed_ssid(A_UINT8 device_id,int index, int flag, char *ssid, int ssidLength);
	void
	_qcom_set_listen_time(A_UINT8 device_id, A_UINT16 time);
	void
	_qcom_set_ap_beacon_interval(A_UINT8 device_id, A_UINT16 beacon_interval);
	void
	_qcom_rssi_get(A_UINT8 device_id, A_UINT16 * rssi);

	void
	_qcom_hw_set_phy_mode(A_UINT8 device_id, int phyMode);
	void
	_qcom_set_power_mode(A_UINT8 device_id, int powerMode);
	void
	_qcom_wmi_setPowerParams_cmd(A_UINT8 device_id, A_UINT16 idlePeriod,
	                            A_UINT16 psPollNum, A_UINT16 dtimPolicy,
	                            A_UINT16 tx_wakeup_policy, A_UINT16 num_tx_to_wakeup,
	                            A_UINT16 ps_fail_event_policy);
	void
	_qcom_add_cipher_key(A_UINT8 device_id, A_UINT8 keyIndex, CRYPTO_TYPE keyType, A_UINT8 keyUsage, A_UINT8 keyLength,
	                    A_UINT8 * keyRSC, A_UINT8 * key, A_UINT8 key_op_ctrl, A_UINT8 * macAddr,
	                    int sync_flag);
	void
	_qcom_add_wep_key(A_UINT8 device_id, int keyIndx, char *key, int keyLength);

	void
	_qcom_wlan_set_tx_power(A_UINT8 device_id, int dbM);
	void
	_qcom_sta_wlan_abort_scan(A_UINT8 device_id);
	void
	_qcom_ap_hidden_ssid(A_UINT8 device_id, A_UINT8 hidden_ssid);
	void
	_qcom_ap_set_num_sta(A_UINT8 device_id, A_UINT8 num_sta);
	void
	_qcom_ap_config_commit(A_UINT8 device_id, A_UINT8 networkType,
                      A_UINT8 dot11AuthMode,
                      A_UINT8 authMode,
                      A_UINT8 pairwiseCryptoType,
                      A_UINT8 pairwiseCryptoLen,
                      A_UINT8 groupCryptoType,
                      A_UINT8 groupCryptoLen,
                      A_UINT8 ssidLength, A_UCHAR * ssid, A_UINT16 channel, A_UINT8 * bssid,
                      A_UINT32 ctrl_flags);

	A_STATUS
	_qcom_set_ap_country(A_UINT8 device_id, char *country_code);
    A_STATUS _qcom_set_sta_country_code(A_UINT8 device_id, A_CHAR *country);
	void
	_qcom_allow_aggr(A_UINT8 device_id, A_UINT16 tx_allow_aggr, A_UINT16 rx_allow_aggr);
	void
	_qcom_set_ht_cap(A_UINT8 device_id, A_UINT8 band,
	                A_UINT8 enable,
	                A_UINT8 chan_width_40M_supported,
	                A_UINT8 short_GI_20MHz,
	                A_UINT8 short_GI_40MHz, A_UINT8 intolerance_40MHz, A_UINT8 max_ampdu_len_exp);
	void
	_qcom_set_pmk(A_UINT8 device_id, A_UINT8 * pmk, A_UINT8 pmk_len);
	void
	_qcom_set_promiscuous_mode(A_UINT8 enable);

	void
	_qcom_hw_set_channel(A_UINT8 device_id, A_UINT16 channel);
	void
	_qcom_set_passphrase(A_UINT8 device_id, A_UCHAR * ssid, A_UINT8 ssidLength, A_UINT8 * passphrase, A_UINT8 passLength);
	void
	_qcom_wlan_suspend_enable(void);
	void
	_qcom_wlan_suspend_start(A_UINT32 time);
	void
	_qcom_sock_stack_init(A_UINT8 device_id);

	void
	_qcom_dns_get_request(A_UINT32 mode, A_UINT32 domain, char *hostname);
#ifdef BRIDGE_ENABLED
	void
	_qcom_enable_bridge_mode(A_UINT16 bridgemode);
#endif
	void
	_qcom_enable_dns_client(A_INT32 command);
	void
	_qcom_dns_server(A_UINT8 *serv_ip, A_UINT8 type, int mode);
	void
	_qcom_enable_dns_server(A_INT32 command);
	void
	_qcom_dns_domain(A_CHAR* name);

	void
	_qcom_dns_update(A_INT32 flag, A_CHAR* hostname, A_UINT32 ipaddr);
	void
	_qcom_dns_6update(A_INT32 flag, A_CHAR* hostname, A_UINT8* ip6addr);
	void
	_qcom_ip_config_request(A_UINT8 device_id, A_UINT32 mode, A_UINT32 ipv4, A_UINT32 subnetMask, A_UINT32 gateway4);
	void
	_qcom_ip6_config_request(A_UINT8 device_id);
	void
	_qcom_ipconfig_dhcp_pool(A_UINT8 device_id, A_UINT32 startaddr, A_UINT32 endaddr, A_INT32 leasetime);

	A_STATUS
	_qcom_ping_request(A_UINT32 ipv4_addr, A_UINT32 size);
	A_STATUS
	_qcom_ping6_request(A_UINT8* ipv6_addr, A_UINT32 size);
	A_STATUS
	_qcom_ip6cfg_router_prefix(A_UINT8 device_id, A_UINT8* addr, A_INT32 prefix_length, A_INT32 prefix_lifetime, A_INT32 valid_lifetime);
	void
	_qcom_get_bp_info(bpool_info_t * bp_info);
	void
	_qcom_http_server_enable(A_INT32 enable, void *ctx);

	void _qcom_http_server_cmd(A_INT32 cmd,A_UINT8 *pagename, A_UINT8 *objname, A_INT32 objtype, A_INT32 objlen, A_UINT8* value);
	void _qcom_http_client_cmd(A_UINT32 cmd, A_UINT8 *url, A_UINT8 *data, void *ctx);
	A_STATUS _qcom_wm_suspend(A_UINT32 time, A_UINT32 flag);
   	A_STATUS  _qcom_wm_gpiowakeupcfg(A_UINT32 options, A_UINT32 wake_msec,  A_UINT32 sleepOnGPIO_flag);
	void
	_qcom_set_ap_inact_time(A_UINT8 device_id, A_UINT16 inact_time);
    void
    _qcom_ipconfig_release_dhcp_pool(A_UINT8 device_id);
    void
    _qcom_ipconfig_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);
    A_STATUS
    _qcom_ip4_route_cmd(A_UINT8 device_id,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    _qcom_ip6_route_cmd(A_UINT8 device_id,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex);
    A_STATUS _qcom_ota_upgrade_cmd(A_UINT8 device_id,void *cmd);
   A_STATUS _qcom_ota_read_ota_area_cmd(void *cmd);
    A_STATUS _qcom_ota_done_cmd(void *cmd);
    A_STATUS _qcom_set_dhcp_hostname_cmd(A_UINT8 device_id,void* cmd);
    void _qcom_lpl_force_enable_cmd(A_UINT8 device_id, A_UINT8 enable);
    void _qcom_gtx_enable_cmd(A_UINT8 device_id, A_UINT8 enable);
    void _qcom_rate_cmd(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);
   A_STATUS _qcom_set_get_ebt_mac_filter_params_cmd(A_UINT8 devid, A_UINT8 action, A_UINT8 *ebt, A_UINT8 *mac_filter);

    A_STATUS _qcom_ota_session_start_cmd(void *cmd);
    A_STATUS _qcom_ota_partition_get_size_cmd(void *cmd);
    A_STATUS _qcom_ota_partition_erase_cmd(void *cmd);
    A_STATUS _qcom_ota_partition_erase_setors_cmd(void *cmd);
    A_STATUS _qcom_ota_parse_image_hdr_cmd(void *cmd);
    A_STATUS _qcom_ota_partition_verify_checksum_cmd(void *cmd);
    A_STATUS _qcom_ota_partition_write_data_cmd(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
    A_STATUS _qcom_ssdp_init_cmd(A_UINT32 notifyPeriod, A_UINT32 localIpAddr, A_UCHAR *locationStr,
                                 A_UCHAR *serverStr,  A_UCHAR *searchTargetStr,
                                 A_UCHAR *domainName, A_UCHAR *serviceType,
                                 A_UCHAR *deviceType, A_UCHAR *uuidStr);
    A_STATUS _qcom_ssdp_enable_cmd(A_UINT32 enable);
    A_STATUS _qcom_ssdp_notify_change_cmd(A_UINT32 notifyPeriod);
	void _qcom_http_client_body_cmd(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
    A_STATUS _qcom_set_bmiss_time(A_UINT8 device_id, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
    A_STATUS _qcom_set_vendor_specific_ie_cmd(A_UINT8 device_id, A_UINT8 * ie_info,A_UINT16 len);
    void _qcom_set_dset_maxblk_reclaim_onetime(A_UINT32 maxblk_reclaim);
	void _qcom_ani_enable(A_BOOL enable);
    A_STATUS _qcom_httpsvr_set_ota_uri_cmd(const char* ota_uri);
    A_STATUS _qcom_httpsvr_set_ota_enable_cmd(A_INT32 enable);

#define QCOM_CDR_API_FN(fn) fn

#endif


#define qcom_wlan_connect(_wmi, _type, _dot11AuthMode, _autoMode, _pwc, _pwcl, _gc, _gcl, _ssidl, _ssid, _bssid, _channel, _ctrl) \
	QCOM_CDR_API_FN(_qcom_wlan_connect((_wmi), (_type), (_dot11AuthMode), (_autoMode), (_pwc), (_pwcl), (_gc), (_gcl), (_ssidl), (_ssid), (_bssid), (_channel), (_ctrl)))

#define qcom_reconnect(_dev, _bssid, _channel) \
	QCOM_CDR_API_FN(_qcom_reconnect((_dev), (_bssid), (_channel)))

#define qcom_wlan_disconnect(_dev) \
	QCOM_CDR_API_FN(_qcom_wlan_disconnect((_dev)))

#define qcom_wifi_scan_start(_dev, _pStartScanParam) \
        QCOM_CDR_API_FN(_qcom_wifi_scan_start((_dev), (_pStartScanParam)))
	
#define qcom_set_scan_params(_dev, _pScanParam) \
	QCOM_CDR_API_FN(_qcom_set_scan_params((_dev), (_pScanParam)))
	
#define qcom_set_bss_filter(_dev, _f) \
	QCOM_CDR_API_FN(_qcom_set_bss_filter((_dev), (_f)))
	
#define qcom_set_probed_ssid(_dev, _index, _f, _ssid, _l) \
	QCOM_CDR_API_FN(_qcom_set_probed_ssid((_dev), (_index), (_f), (_ssid), (_l)))
	
#define qcom_set_listen_time(_dev, _t) \
	QCOM_CDR_API_FN(_qcom_set_listen_time((_dev), (_t)))

#define qcom_set_ap_beacon_interval(_dev, _t) \
	QCOM_CDR_API_FN(_qcom_set_ap_beacon_interval((_dev), (_t)))

#define qcom_rssi_get(_dev, _rssi) \
	QCOM_CDR_API_FN(_qcom_rssi_get((_dev), (_rssi)))

#define qcom_hw_set_phy_mode(_dev, _m) \
	QCOM_CDR_API_FN(_qcom_hw_set_phy_mode((_dev), (_m)))

#define qcom_set_power_mode(_dev, _m) \
	QCOM_CDR_API_FN(_qcom_set_power_mode((_dev), (_m)))

#define qcom_wmi_setPowerParams_cmd(_dev, _idle, _psPollNUm, _dtim, _txWake, _num, _failPolicy) \
	QCOM_CDR_API_FN(_qcom_wmi_setPowerParams_cmd((_dev), (_idle), (_psPollNUm), (_dtim), (_txWake), (_num), (_failPolicy)))

#define qcom_add_cipher_key(_dev, _index, _type, _usage, _len, _rsc, _key, _ctrl, _mac, _sync) \
	QCOM_CDR_API_FN(_qcom_add_cipher_key((_dev), (_index), (_type), (_usage), (_len), (_rsc), (_key), (_ctrl), (_mac), (_sync)))

#define qcom_add_wep_key(_dev, _index, _key, _len) \
	QCOM_CDR_API_FN(_qcom_add_wep_key((_dev), (_index), (_key), (_len)))

#define qcom_wlan_set_tx_power(_dev, _v) \
	QCOM_CDR_API_FN(_qcom_wlan_set_tx_power((_dev), (_v)))
	
#define qcom_sta_wlan_abort_scan(_dev) \
	QCOM_CDR_API_FN(_qcom_sta_wlan_abort_scan((_dev)))
	
#define qcom_ap_hidden_ssid(_dev, _v) \
	QCOM_CDR_API_FN(_qcom_ap_hidden_ssid((_dev), (_v)))
	
#define qcom_ap_set_num_sta(_dev, _v) \
	QCOM_CDR_API_FN(_qcom_ap_set_num_sta((_dev), (_v)))
	
#define qcom_ap_config_commit(_dev, _type, _dot11AuthMode, _authMode, _pwc, _pwcl, _gc, _gcl, _ssidl, _ssid, _channel, _bssid, _ctrl) \
	QCOM_CDR_API_FN(_qcom_ap_config_commit((_dev), (_type), (_dot11AuthMode), (_authMode), (_pwc), (_pwcl), (_gc), (_gcl), (_ssidl), (_ssid), (_channel), (_bssid), (_ctrl)))
	
#define qcom_set_ap_country(_dev, _v) \
	QCOM_CDR_API_FN(_qcom_set_ap_country((_dev), (_v)))

A_STATUS _qcom_set_sta_country_code(A_UINT8 device_id, A_CHAR *country);
#define qcom_set_sta_country_code(_dev, _v) \
    _qcom_set_sta_country_code((_dev), (_v))
	
A_STATUS _qcom_set_ipv6_status_cmd(A_UINT8 device_id,void *cmd);
#define qcom_set_ipv6_status_cmd(_dev,_v)\
      _qcom_set_ipv6_status_cmd((_dev), (_v))
#define qcom_allow_aggr(_dev, _tx, _rx) \
	QCOM_CDR_API_FN(_qcom_allow_aggr((_dev), (_tx), (_rx)))

#define qcom_set_ht_cap(_dev, _band, _en, _cw40, _sgi20, _sgi40, _intolerance, _max) \
	QCOM_CDR_API_FN(_qcom_set_ht_cap((_dev), (_band), (_en), (_cw40), (_sgi20), (_sgi40), (_intolerance), (_max)))

#define qcom_set_pmk(_dev, _v, _l) \
	QCOM_CDR_API_FN(_qcom_set_pmk((_dev), (_v), (_l)))

#define qcom_set_promiscuous_mode(_en) \
	QCOM_CDR_API_FN(_qcom_set_promiscuous_mode((_en)))

#define qcom_hw_set_channel(_dev, _ch) \
	QCOM_CDR_API_FN(_qcom_hw_set_channel((_dev), (_ch)))

#define qcom_set_passphrase(_dev, _ssid, _ssidl, _passphrase, _len) \
	QCOM_CDR_API_FN(_qcom_set_passphrase((_dev), (_ssid), (_ssidl), (_passphrase), (_len)))

#define qcom_wlan_suspend_enable() \
	QCOM_CDR_API_FN(_qcom_wlan_suspend_enable())

#define qcom_wlan_suspend_start(_dev, _time) \
	QCOM_CDR_API_FN(_qcom_wlan_suspend_start((_dev), (_time)))

#define qcom_sock_stack_init(_dev) \
	QCOM_CDR_API_FN(_qcom_sock_stack_init((_dev)))

#define qcom_dns_get_request(_dev, _dm, _host) \
	QCOM_CDR_API_FN(_qcom_dns_get_request((_dev), (_dm), (_host)))

#ifdef BRIDGE_ENABLED

#define qcom_enable_bridge_mode(_mode) \
	QCOM_CDR_API_FN(_qcom_enable_bridge_mode((_mode)))

#endif

#define qcom_enable_dns_client(_cmd) \
	QCOM_CDR_API_FN(_qcom_enable_dns_client((_cmd)))

#define qcom_dns_server(_ipa, _type, _mode) \
	QCOM_CDR_API_FN(_qcom_dns_server((_ipa),(_type), (_mode)))

#define qcom_enable_dns_server(_cmd) \
	QCOM_CDR_API_FN(_qcom_enable_dns_server((_cmd)))

#define qcom_dns_domain(_host) \
	QCOM_CDR_API_FN(_qcom_dns_domain((_host)))

#define qcom_dns_update(_dev, _host, _ipa) \
	QCOM_CDR_API_FN(_qcom_dns_update((_dev), (_host), (_ipa)))

#define qcom_dns_6update(_dev, _host, _ipa) \
	QCOM_CDR_API_FN(_qcom_dns_6update((_dev), (_host), (_ipa)))

#define qcom_ip_config_request(_dev, _mode, _ipa, _snmask, _gw) \
	QCOM_CDR_API_FN(_qcom_ip_config_request((_dev), (_mode), (_ipa), (_snmask), (_gw)))

#define qcom_ip6_config_request(_dev) \
	QCOM_CDR_API_FN(_qcom_ip6_config_request((_dev)))

#define qcom_ipconfig_dhcp_pool(_dev, _st, _ed, _time) \
	QCOM_CDR_API_FN(_qcom_ipconfig_dhcp_pool((_dev), (_st), (_ed), (_time)))

#define qcom_ping_request(_ipa, _size) \
	QCOM_CDR_API_FN(_qcom_ping_request((_ipa), (_size)))

#define qcom_ping6_request(_ipa, _size) \
	QCOM_CDR_API_FN(_qcom_ping6_request((_ipa), (_size)))

#define qcom_ip6cfg_router_prefix(_device, _addr, _plen, _ptime, _vtime) \
	QCOM_CDR_API_FN(_qcom_ip6cfg_router_prefix((_device), (_addr), (_plen), (_ptime), (_vtime)))

#define qcom_get_bp_info(_bp) \
	QCOM_CDR_API_FN(_qcom_get_bp_info((_bp)))

#define qcom_http_server_enable(_en, _ctx) \
	QCOM_CDR_API_FN(_qcom_http_server_enable((_en), (_ctx)))

#define qcom_http_server_cmd(_cmd, _par, _obj, _type, _len, _val) \
	QCOM_CDR_API_FN(_qcom_http_server_cmd((_cmd), (_par), (_obj), (_type), (_len), (_val)))

#define qcom_http_client_cmd(_cmd, _url, _data, _ctx) \
	QCOM_CDR_API_FN(_qcom_http_client_cmd((_cmd), (_url), (_data), (_ctx)))

#define qcom_wm_suspend(_time, _flag) \
	QCOM_CDR_API_FN(_qcom_wm_suspend((_time), (_flag)))

#define qcom_wm_gpiowakeupcfg(_options, _wake_msec, _sleepOnGPIO_flag) \
	QCOM_CDR_API_FN(_qcom_wm_gpiowakeupcfg((_options), (_wake_msec), (_sleepOnGPIO_flag)))

#if defined(AR6002_REV74) 

    void
	_qcom_set_ap_inact_time(A_UINT8 device_id, A_UINT16 inact_time);
    void
    _qcom_ipconfig_release_dhcp_pool(A_UINT8 device_id);
    void
    _qcom_ipconfig_set_exp_backoff(A_UINT8 device_id, A_INT32 retry);
    A_STATUS
    _qcom_ip4_route_cmd(A_UINT8 device_id,A_UINT32 cmd, A_UINT32 *addr, A_UINT32 *subnet, A_UINT32* gw, A_UINT32* ifindex);
    A_STATUS
    _qcom_ip6_route_cmd(A_UINT8 device_id,A_UINT32 cmd, A_UINT8 *ip6addr, A_UINT32 *prefixLen, A_UINT8* gw, A_UINT32* ifindex);
    A_STATUS _qcom_ota_upgrade_cmd(A_UINT8 device_id,void *cmd);
    A_STATUS _qcom_ota_read_ota_area_cmd(void *cmd);
    A_STATUS _qcom_ota_done_cmd(void *cmd);
    A_STATUS _qcom_set_dhcp_hostname_cmd(A_UINT8 device_id,void* cmd);
	void _qcom_lpl_force_enable_cmd(A_UINT8 device_id, A_UINT8 enable);
	void _qcom_gtx_enable_cmd(A_UINT8 device_id, A_UINT8 enable);
	void _qcom_rate_cmd(A_UINT8 device_id, A_UINT8 mcs, A_UINT8 rate);

    	
#define qcom_set_ap_inact_time(_dev, _t) \
		_qcom_set_ap_inact_time((_dev), (_t))

#define qcom_ipconfig_release_dhcp_pool(device_id) \
    _qcom_ipconfig_release_dhcp_pool((device_id))

#define qcom_ipconfig_set_exp_backoff(device_id, retry)  \
        _qcom_ipconfig_set_exp_backoff((device_id), (retry))

#define qcom_ip4_route_cmd(device_id,cmd, addr, subnet, gw, ifindex)  \
        _qcom_ip4_route_cmd((device_id),(cmd), (addr), (subnet), (gw), (ifindex))  

#define qcom_ip6_route_cmd(device_id, cmd, ip6addr, prefixLen, gw, ifindex) \
        _qcom_ip6_route_cmd((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex))

#define  qcom_ota_upgrade_cmd(device_id,cmd)\
            _qcom_ota_upgrade_cmd((device_id),(cmd))

#define qcom_ota_read_ota_area_cmd(cmd)\
           _qcom_ota_read_ota_area_cmd((cmd))

#define qcom_ota_done_cmd(cmd)\
           _qcom_ota_done_cmd((cmd))

#define qcom_lpl_force_enable_cmd(device_id, enable) \
			_qcom_lpl_force_enable_cmd(device_id, enable)
#define qcom_gtx_enable_cmd(device_id, enable) \
                _qcom_gtx_enable_cmd(device_id, enable)
#define qcom_rate_cmd(device_id, mcs, rate) \
                    _qcom_rate_cmd(device_id, mcs, rate)
#define qcom_set_dhcp_hostname_cmd(device_id,cmd)\
            _qcom_set_dhcp_hostname_cmd((device_id),(cmd))


#else //AR6002_REV74

       A_STATUS _qcom_tcp_conn_timeout_cmd(void *cmd);
#define qcom_tcp_conn_timeout_cmd(cmd)\
                   _qcom_tcp_conn_timeout_cmd((cmd))

       void _qcom_perform_ba(A_UINT8 device_id,A_UINT8 tid);
#define  qcom_perform_ba(device_id,tid)\
          _qcom_perform_ba((device_id),(tid))
                   
	void _qcom_wlan_set_roam_ctrl(A_UINT8 device_id, A_UINT8 roamCtrlType, A_UINT8 *buf);
#define qcom_wlan_set_roam_ctrl(device_id,roamCtrlType, buf)\
          _qcom_wlan_set_roam_ctrl((device_id),(roamCtrlType),(buf))

		A_STATUS _qcom_set_keep_alive_cmd(A_UINT8 device_id, A_UINT32 keep_alive_interval);
#define qcom_set_keepalive_cmd(device_id, keep_alive_interval) \
            _qcom_set_keep_alive_cmd((device_id), (keep_alive_interval))
            
#define qcom_set_ap_inact_time(_dev, _t) \
		QCOM_CDR_API_FN(_qcom_set_ap_inact_time((_dev), (_t)))
    
#define qcom_ipconfig_release_dhcp_pool(device_id) \
        QCOM_CDR_API_FN(_qcom_ipconfig_release_dhcp_pool((device_id)))

#define qcom_ipconfig_set_exp_backoff(device_id, retry)  \
        QCOM_CDR_API_FN(_qcom_ipconfig_set_exp_backoff((device_id), (retry)))

#define qcom_ip4_route_cmd(device_id,cmd, addr, subnet, gw, ifindex)  \
        QCOM_CDR_API_FN(_qcom_ip4_route_cmd((device_id),(cmd), (addr), (subnet), (gw), (ifindex)))

#define qcom_ip6_route_cmd(device_id, cmd, ip6addr, prefixLen, gw, ifindex) \
        QCOM_CDR_API_FN(_qcom_ip6_route_cmd((device_id), (cmd), (ip6addr), (prefixLen), (gw), (ifindex)))

#define qcom_ota_upgrade_cmd(device_id,cmd)\
            QCOM_CDR_API_FN(_qcom_ota_upgrade_cmd((device_id),(cmd)))

#define qcom_ota_read_ota_area_cmd(cmd)\
            QCOM_CDR_API_FN(_qcom_ota_read_ota_area_cmd((cmd)))

#define qcom_ota_done_cmd(cmd)\
               QCOM_CDR_API_FN(_qcom_ota_done_cmd((cmd)))

#define qcom_set_dhcp_hostname_cmd(device_id,cmd)\
          QCOM_CDR_API_FN(_qcom_set_dhcp_hostname_cmd((device_id),(cmd)))

#define qcom_lpl_force_enable_cmd(device_id, enable) \
		QCOM_CDR_API_FN(_qcom_lpl_force_enable_cmd(device_id, enable))
#define qcom_gtx_enable_cmd(device_id, enable) \
        QCOM_CDR_API_FN(_qcom_gtx_enable_cmd(device_id, enable))
#define qcom_rate_cmd(device_id, mcs, rate) \
        QCOM_CDR_API_FN(_qcom_rate_cmd(device_id, mcs, rate))

#if defined(AR6002_REV76)
A_STATUS _qcom_ota_session_start_cmd(void *cmd);
A_STATUS _qcom_ota_partition_get_size_cmd(void *cmd);
A_STATUS _qcom_ota_partition_erase_cmd(void *cmd);
A_STATUS _qcom_ota_partition_erase_sectors_cmd(void *cmd);
A_STATUS _qcom_ota_parse_image_hdr_cmd(void *cmd);
A_STATUS _qcom_ota_partition_verify_checksum_cmd(void *cmd);
A_STATUS _qcom_ota_partition_write_data_cmd(A_UINT32 offset, A_UINT8 *buf,A_UINT32 size);
A_STATUS _qcom_ssdp_init_cmd(A_UINT32 notifyPeriod, A_UINT32 localIpAddr, A_UCHAR *locationStr,
                                 A_UCHAR *serverStr,  A_UCHAR *searchTargetStr,
                                 A_UCHAR *domainName, A_UCHAR *serviceType,
                                 A_UCHAR *deviceType, A_UCHAR *uuidStr);
A_STATUS _qcom_ssdp_enable_cmd(A_UINT32 enable);
A_STATUS _qcom_ssdp_notify_change_cmd(A_UINT32 notifyPeriod);
A_STATUS _qcom_http_get_datasend_cmd(A_UINT16 sess_index, A_UINT16 data_len, 
										A_UINT8 *url_str, A_UINT8 *data, A_UINT16 header_len, A_UINT8 *header_data);
A_STATUS _qcom_http_set_custom_url_cmd(const char* const *new_url);
A_STATUS _qcom_restrict_http_request_cmd(A_UINT8 device_id, A_UINT8 enable);
A_STATUS _qcom_http_redirect_unknown_url_enable_cmd(A_UINT8 enable);
A_STATUS _qcom_http_server_add_redirected_page_cmd(A_UINT16 header_len, A_UINT8 *header, A_UINT16 data_len, char *data, char *url);
A_STATUS _qcom_http_set_redirected_url_cmd(char *url);
	
A_STATUS _qcom_param_set_cmd(A_UINT8 device_id,A_UINT32 which_param,void *data, A_UINT32 data_length);
A_STATUS _qcom_param_get_cmd(A_UINT8 device_id,A_UINT32 which_param);

void _qcom_http_client_body_cmd(A_UINT32 cmd, A_UINT8 *body, A_UINT32 bodylen);
A_STATUS _qcom_set_bmiss_time(A_UINT8 device_id, A_UINT16 bmiss_num, A_UINT16 bmiss_time);
A_STATUS _qcom_set_vendor_specific_ie_cmd(A_UINT8 device_id, A_UINT8 mgmt_frm_type,A_UINT8 * ie_info,A_UINT16 len);
void _qcom_set_dset_maxblk_reclaim_onetime(A_UINT32 maxblk_reclaim);
void _qcom_ani_enable(A_BOOL enable);
A_STATUS _qcom_httpsvr_set_ota_uri_cmd(const char* ota_uri);
A_STATUS _qcom_httpsvr_set_ota_enable_cmd(A_INT32 enable);
A_STATUS _qcom_ant_div_set(int div, int adjust);
void _qcom_ant_div_get(void);
A_STATUS _qcom_http_client_register_cb_cmd(void* callback, void* arg);
#ifdef HTTPC_MULTI_SESSION
A_STATUS _qcom_http_client_connect_cmd(void* client, const A_CHAR *server, A_UINT16 port, A_UINT32 timeout, void* ssl_ctx, A_BOOL cb_enabled);
A_STATUS _qcom_http_client_disconnect_cmd(A_UINT32 client, A_UINT32  session);
A_STATUS _qcom_http_client_request_cmd(A_UINT32 session, A_UINT32 cmd, const A_CHAR *url, A_CHAR *output);
A_STATUS _qcom_http_client_set_body_cmd(A_UINT32 session, const A_CHAR *body, A_UINT32 bodylen);
A_STATUS _qcom_http_client_set_param_cmd(A_UINT32 session, const A_CHAR *key, const A_CHAR *value);
A_STATUS _qcom_http_client_add_header_cmd(A_UINT32 session, const A_CHAR *header, const A_CHAR *value);
A_STATUS _qcom_http_client_clear_header_cmd(A_UINT32  session);
#endif
A_STATUS _qcom_btcoex_scheme_set_cmd(A_UINT8 scheme_id, void* data);

#define qcom_ota_session_start_cmd(cmd) \
	  _qcom_ota_session_start_cmd((cmd))

#define qcom_ota_partition_get_size_cmd(cmd) \
         _qcom_ota_partition_get_size_cmd((cmd))
         
#define qcom_ota_partition_erase_cmd(cmd) \
         _qcom_ota_partition_erase_cmd((cmd))
         
#define qcom_ota_partition_erase_sectors_cmd(cmd) \
         _qcom_ota_partition_erase_sectors_cmd((cmd))

#define qcom_ota_parse_image_hdr_cmd(cmd) \
        _qcom_ota_parse_image_hdr_cmd((cmd))

#define qcom_ota_partition_verify_checksum_cmd(cmd) \
         _qcom_ota_partition_verify_checksum_cmd((cmd))

#define qcom_ota_partition_write_data_cmd(offset, buf,size) \
        _qcom_ota_partition_write_data_cmd((offset),(buf),(size))

#define qcom_ssdp_init_cmd(notifyPeriod, localIpAddr, locationStr, serverStr,  searchTargetStr, domainName, serviceType, deviceType, uuidStr) \
        _qcom_ssdp_init_cmd((notifyPeriod), (localIpAddr), (locationStr), (serverStr), (searchTargetStr), (domainName), (serviceType), (deviceType), (uuidStr))

#define qcom_ssdp_enable_cmd(enable) \
	_qcom_ssdp_enable_cmd(enable)

#define qcom_ssdp_notify_change_cmd(notifyPeriod) \
	_qcom_ssdp_notify_change_cmd(notifyPeriod)

#define qcom_http_get_datasend_cmd(sess_index,  data_len,  url_str,  data, header_len, header_data) \
           _qcom_http_get_datasend_cmd(sess_index,  data_len,  url_str,  data, header_len, header_data)
#define qcom_http_set_custom_url_cmd(new_url)\
           _qcom_http_set_custom_url_cmd(new_url)

#define qcom_restrict_http_request_cmd(dev_id, enable)\
           _qcom_restrict_http_request_cmd(dev_id, enable)

#define qcom_http_redirect_unknown_url_enable_cmd(enable)\
           _qcom_http_redirect_unknown_url_enable_cmd(enable)

#define qcom_http_server_add_redirected_page_cmd(header_len, header, data_len, data, url) \
            _qcom_http_server_add_redirected_page_cmd(header_len, header, data_len, data, url)

#define qcom_http_set_redirected_url_cmd(url)\
           _qcom_http_set_redirected_url_cmd(url)
           
#define qcom_param_set_cmd(dev_id,which_param,data,data_length)\
            _qcom_param_set_cmd((dev_id),(which_param),(data),(data_length))
#define qcom_http_client_body_cmd(cmd, body, bodylen) \
	_qcom_http_client_body_cmd(cmd, body, bodylen)

#define qcom_set_bmiss_time(dev_id, bmiss_num, bmiss_time) \
    _qcom_set_bmiss_time((dev_id), (bmiss_num), (bmiss_time))

#define  qcom_set_vendor_specific_ie_cmd(device_id, mgmt_frm_type,ie_info, len)\
    _qcom_set_vendor_specific_ie_cmd((device_id),(mgmt_frm_type), (ie_info),(len))

#define qcom_param_get_cmd(dev_id,which_param)\
            _qcom_param_get_cmd((dev_id),(which_param))

#define qcom_ap_get_sta_info_cmd(dev_id)\
            _qcom_ap_get_sta_info_cmd((dev_id))

#define qcom_set_dset_maxblk_reclaim_onetime(maxblk_reclaim) \
    _qcom_set_dset_maxblk_reclaim_onetime((maxblk_reclaim))
	
#define qcom_ani_enable(enable) \
    _qcom_ani_enable((enable))
    
#define qcom_httpsvr_set_ota_uri_cmd(ota_uri)\
                   _qcom_httpsvr_set_ota_uri_cmd(ota_uri)

#define qcom_httpsvr_set_ota_enable_cmd(enable)\
                   _qcom_httpsvr_set_ota_enable_cmd(enable)

#define qcom_ant_div_set(div, adjust) \
    _qcom_ant_div_set((div), (adjust))	  

#define qcom_ant_div_get() \
    _qcom_ant_div_get()	  

#define qcom_http_client_register_cb_cmd(callback, arg) \
    _qcom_http_client_register_cb_cmd(callback, arg)

#ifdef HTTPC_MULTI_SESSION
#define qcom_http_client_connect_cmd(client, server, port, timeout, ssl_ctx, cb_enabled) \
     _qcom_http_client_connect_cmd(client, server, port, timeout, ssl_ctx, cb_enabled)

#define qcom_http_client_disconnect_cmd(client, session) \
     _qcom_http_client_disconnect_cmd(client, session)

#define qcom_http_client_request_cmd(session, cmd, url, output) \
     _qcom_http_client_request_cmd(session, cmd, url, output)

#define qcom_http_client_set_body_cmd(session, body, bodylen) \
     _qcom_http_client_set_body_cmd(session, body, bodylen)

#define qcom_http_client_set_param_cmd(session, key, value) \
     _qcom_http_client_set_param_cmd(session, key, value)

#define qcom_http_client_add_header_cmd(session, header, value) \
     _qcom_http_client_add_header_cmd(session, header, value)

#define qcom_http_client_clear_header_cmd(session) \
     _qcom_http_client_clear_header_cmd(session)
#endif

#define qcom_btcoex_scheme_set_cmd(scheme_id, data) \
    _qcom_btcoex_scheme_set_cmd(scheme_id, data)

#else //AR6002_REV76
#define qcom_ota_session_start_cmd(cmd) \
	    QCOM_CDR_API_FN(_qcom_ota_session_start_cmd((cmd)))

#define qcom_ota_partition_get_size_cmd(cmd) \
         QCOM_CDR_API_FN(_qcom_ota_partition_get_size_cmd((cmd)))
         
#define qcom_ota_partition_erase_cmd(cmd) \
         QCOM_CDR_API_FN(_qcom_ota_partition_erase_cmd((cmd)))
         
#define qcom_ota_partition_erase_sectors_cmd(cmd) \
         QCOM_CDR_API_FN(_qcom_ota_partition_erase_sectors_cmd((cmd)))

#define qcom_ota_parse_image_hdr_cmd(cmd) \
        QCOM_CDR_API_FN(_qcom_ota_parse_image_hdr_cmd((cmd)))

#define qcom_ota_partition_verify_checksum_cmd(cmd) \
         QCOM_CDR_API_FN(_qcom_ota_partition_verify_checksum_cmd((cmd)))	    

#define qcom_ota_partition_write_data_cmd(offset, buf,size) \
        QCOM_CDR_API_FN(_qcom_ota_partition_write_data_cmd((offset),(buf),(size)))

#define qcom_ssdp_init_cmd(notifyPeriod, localIpAddr, locationStr, serverStr,  searchTargetStr, domainName, serviceType, deviceType, uuidStr) \
        QCOM_CDR_API_FN(_qcom_ssdp_init_cmd((notifyPeriod), (localIpAddr), (locationStr), (serverStr), (searchTargetStr), (domainName), (serviceType), (deviceType), (uuidStr)))

#define qcom_ssdp_enable_cmd(enable) \
	QCOM_CDR_API_FN(_qcom_ssdp_enable_cmd((enable)))

#define qcom_ssdp_notify_change_cmd(notifyPeriod) \
	QCOM_CDR_API_FN(_qcom_ssdp_notify_change_cmd((notifyPeriod)))

	        
#define qcom_http_get_datasend_cmd(sess_index,  data_len,  url_str,  data, header_len, header_data) \
           QCOM_CDR_API_FN(_qcom_http_get_datasend_cmd((sess_index),  (data_len),  (url_str),  (data), (header_len), (header_data)))

#define qcom_http_set_custom_url_cmd(new_url)\
           QCOM_CDR_API_FN(_qcom_http_set_custom_url_cmd(new_url))

#define qcom_restrict_http_request_cmd(dev_id, enable)\
           QCOM_CDR_API_FN(_qcom_restrict_http_request_cmd((dev_id), (enable)))

#define qcom_http_redirect_unknown_url_enable_cmd(enable)\
           QCOM_CDR_API_FN(_qcom_http_redirect_unknown_url_enable_cmd(enable))

#define qcom_http_server_add_redirected_page_cmd(header_len, header, data_len, data, url) \
            QCOM_CDR_API_FN(_qcom_http_server_add_redirected_page_cmd((header_len), (header), (data_len), (data), (url)))

#define qcom_http_set_redirected_url_cmd(url) \
            QCOM_CDR_API_FN(_qcom_http_set_redirected_url_cmd((url)))
           
#define qcom_param_set_cmd(dev_id,which_param,data,data_length)\
    QCOM_CDR_API_FN(_qcom_param_set_cmd(dev_id, which_param, data, data_length))
        
#define qcom_param_get_cmd(dev_id,which_param)\
    QCOM_CDR_API_FN(_qcom_param_get_cmd(dev_id, which_param))

#define qcom_http_client_body_cmd(cmd, body, bodylen) \
	QCOM_CDR_API_FN(_qcom_http_client_body_cmd(cmd, body, bodylen))

#define qcom_set_bmiss_time(dev_id, bmiss_num, bmiss_time) \
    QCOM_CDR_API_FN(_qcom_set_bmiss_time((dev_id), (bmiss_num), (bmiss_time)))
#define qcom_set_vendor_specific_ie_cmd( device_id, mgmt_frm_type, ie_info,len)\
    QCOM_CDR_API_FN(_qcom_set_vendor_specific_ie_cmd((device_id), (mgmt_frm_type),(ie_info), (len))  

#define qcom_set_dset_maxblk_reclaim_onetime(maxblk_reclaim) \
    QCOM_CDR_API_FN(_qcom_set_dset_maxblk_reclaim_onetime((maxblk_reclaim)))
	
#define qcom_ani_enable(enable) \
    QCOM_CDR_API_FN(_qcom_ani_enable((enable)))	
    
#define qcom_httpsvr_set_ota_uri_cmd(ota_uri)\
               QCOM_CDR_API_FN(_qcom_httpsvr_set_ota_uri_cmd(ota_uri))

#define qcom_httpsvr_set_ota_enable_cmd(enable)\
               QCOM_CDR_API_FN(_qcom_httpsvr_set_ota_enable_cmd(enable))

#define qcom_ant_div_set(div, adjust) \
    QCOM_CDR_API_FN(_qcom_ant_div_set((div), (adjust)))  

#define qcom_ant_div_get() \
    QCOM_CDR_API_FN(_qcom_ant_div_get())

#define qcom_http_client_register_cb_cmd(callback, arg) \
    QCOM_CDR_API_FN(_qcom_http_client_register_cb_cmd(callback, arg))

#ifdef HTTPC_MULTI_SESSION  
#define qcom_http_client_connect_cmd(client, server, port, timeout, ssl_ctx, cb_enabled) \
     QCOM_CDR_API_FN(_qcom_http_client_connect_cmd(client, server, port, timeout, ssl_ctx, cb_enabled))

#define qcom_http_client_disconnect_cmd(client, session) \
     QCOM_CDR_API_FN(_qcom_http_client_disconnect_cmd(client, session))

#define qcom_http_client_request_cmd(session, cmd, url, output) \
     QCOM_CDR_API_FN(_qcom_http_client_request_cmd(session, cmd, url, output))

#define qcom_http_client_set_body_cmd(session, body, bodylen) \
     QCOM_CDR_API_FN(_qcom_http_client_set_body_cmd(session, body, bodylen))

#define qcom_http_client_set_param_cmd(session, key, value) \
     QCOM_CDR_API_FN(_qcom_http_client_set_param_cmd(session, key, value))

#define qcom_http_client_add_header_cmd(session, header, value) \
     QCOM_CDR_API_FN(_qcom_http_client_add_header_cmd(session, header, value))

#define qcom_http_client_clear_header_cmd(session) \
     QCOM_CDR_API_FN(_qcom_http_client_clear_header_cmd(session))
#endif

#define qcom_btcoex_scheme_set_cmd(scheme_id, data) \
    QCOM_CDR_API_FN(qcom_btcoex_scheme_set_cmd(scheme_id, data))

#endif // AR6002_REV76

#endif  //AR6002_REV74

#define qcom_set_get_ebt_mac_filter_params_cmd(devid, action, ebt, mac_filter) \
    QCOM_CDR_API_FN(_qcom_set_get_ebt_mac_filter_params_cmd(devid, action, ebt, mac_filter))

#endif
