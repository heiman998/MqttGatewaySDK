/*  
 * Copyright (c) 2013 Qualcomm Atheros, Inc..
 * All Rights Reserved.  
 * Qualcomm Atheros Confidential and Proprietary.
 */

#include "qcom_common.h"
#include "qcom_internal.h"
#include "qcom_p2p_api.h"
#include "qcom_wps.h"

//extern var
extern int qcom_wps_connect_flag;
//extern DEVICE_CONTEXT_T *gDeviceContextPtr[];
    
//extern function
extern A_STATUS _qcom_p2p_get_peer_mac(A_UINT8 *pmac);
extern A_STATUS _qcom_p2p_get_wps_method(A_UINT8 *wps_method);
extern void _qcom_p2p_get_wps_pin(char *ppin);
extern A_STATUS _qcom_dev_get_sub_mode(int dev_id, A_UINT8 *psub_mod);
extern void _qcom_p2p_set_persistent_peer_address(A_UINT8  * pEvtBuffer);


/* Moved qcom_wps_start_without_scan to go_neg_result WMI event handler
 * in the application. 
 * TODO: Need to understand why a 3 second delay is needed between
 * qcom_ap_start and wps_start_without_scan.
 */
#if 0
void _qcom_p2p_priv_connect_event_process(void *pContext,    /* app supplied context */
                            A_UINT16 EventID,  /* event ID */
                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
                              A_UINT8  * pEvtBuffer,  /* pointer to event data, if needed */
                            int Length)

{
    cmnos_printf("%s\n", __func__);
    return;
#if defined(P2P_ENABLED)
    #define SUB_OPMODE_FW_P2P_GO 8

    A_UINT8 device_id = info & 0xFF;
    A_UINT8 dev_mode;
    A_UINT8 wps_method=0;
    char szpin[WPS_PIN_LEN+1];

    memset(szpin, 0, sizeof(szpin));

    _qcom_dev_get_sub_mode(device_id, &dev_mode);
    if (dev_mode != SUB_OPMODE_FW_P2P_GO) { //make sure it's P2P GO device
        return;
    }
    
    _qcom_p2p_set_persistent_peer_address(pEvtBuffer);
    if (guigo_started != 1) {
        return;
    }

    guigo_started = 0;
    
    if (qcom_wps_connect_flag) {
        return;
    }

    _qcom_p2p_get_wps_method(&wps_method); 
    /*
    WPS_NOT_READY : 0
    WPS_PBC             : 4
    */
    if ((0 != wps_method) && (4 != wps_method)) {
        _qcom_p2p_get_wps_pin(szpin);
    }

    /*
    * When AP is started, a connecting event will be triggered.
    * When a station(p2p client) associates this AP, another connecting event will be triggered.
    * If we start a wps without scan procedure once receiving a connecting event, 
    * _ap_form_eapol_frame cannot find the connectiong context for the peer device.
    * Because, we will receive connecting event of AP starting first.
    * So we start wps with scan procedure here. 
    */
#if 0
    A_UINT8 role=0;
    A_UINT8 peer_addr[ATH_MAC_LEN]={0} ;
    A_STATUS _qcom_p2p_get_role(A_UINT8 *role);
    
    _qcom_p2p_get_peer_mac(peer_addr);
    _qcom_p2p_get_role(&role);
    extern void _qcom_wps_start_without_scan(int dev_id, int role, int mode, int channel, int connect, int time_out, char *pssid, char *pin, unsigned char *pmac);
    _qcom_wps_start_without_scan(device_id, role, wps_method, \
                        gDeviceContextPtr[device_id]->channel , 1, 30, (A_CHAR *)gDeviceContextPtr[device_id]->ssid, \
                        szpin,\
                        peer_addr);
#else
    if (4 == wps_method) {
        wps_method = 1; //PBC mode
    }
    else {
        wps_method = 0; //PIN mode
    }
    qcom_wps_start(device_id, 1, wps_method, szpin);
#endif
    #undef  SUB_OPMODE_FW_P2P_GO

#endif
    return;
}
#endif
