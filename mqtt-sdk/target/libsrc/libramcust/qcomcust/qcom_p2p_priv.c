/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc..
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */
#if defined(P2P_ENABLED)
#include <osapi.h>
#include <wlan_dev.h>
#include <wmi_api.h>
#include "threadx/tx_api.h"
#include "threadx/tx_queue.h"
#include "qcom/qcom_wlan.h"
#include "qcom_p2p_internal.h"
#include "wlan/p2p_fw/p2p_internal.h"
#include "wlan/include/p2p_api.h"
#include "qcom/qcom_wps.h"
#include "qcom/qcom_wps_priv.h"
#include "qcom/qcom_event.h"
#include "qcom/qcom_internal_api.h"

#define QCOM_P2P_DEBUG_PRINT_EN  0
#if (QCOM_P2P_DEBUG_PRINT_EN == 0)
#define QCOM_P2P_DEBUG_PRINTF(...)
#else
extern int cmnos_printf(const char * format, ...);
#define QCOM_P2P_DEBUG_PRINTF  cmnos_printf
#endif

#define QCOM_P2P_PRINT_EN  1
#if (QCOM_P2P_PRINT_EN == 0)
#define QCOM_P2P_PRINTF(...)
#else
extern int cmnos_printf(const char * format, ...);
#define QCOM_P2P_PRINTF  cmnos_printf
#endif

/* extern var */
extern A_UINT8  gMCCEnabled;

A_STATUS _qcom_p2p_func_set_channel_freq_cfg(A_UINT8 device_id, A_UINT16 usch_freq);

A_STATUS _qcom_p2p_get_dev_ctx_point(int dev_id, P2P_DEV_CTX **ppp2p_ctx)
{

    if ((dev_id >= gNumDev) || (NULL == ppp2p_ctx)) {
        return A_ERROR;
    }

    if (NULL == devArray[dev_id]->p2p_fw_ctx) {
        return A_DEVICE_NOT_FOUND;
    }

    *ppp2p_ctx = (P2P_DEV_CTX *)(devArray[dev_id]->p2p_fw_ctx);

    return A_OK;
}

A_STATUS _qcom_dev_get_sub_mode(int dev_id, A_UINT8 *psub_mod)
{
    if ((dev_id >= gNumDev) || (NULL == psub_mod)) {
        return A_ERROR;
    }

    *psub_mod = devArray[dev_id]->ic_subopmode;

    return A_OK;
}

void _qcom_p2p_gen_ssid(A_CHAR *ssid)
{
    if(ssid == NULL)
        return;

    A_MEMCPY(ssid,P2P_WILDCARD_SSID,P2P_WILDCARD_SSID_LEN);
    p2p_random((char *) &ssid[P2P_WILDCARD_SSID_LEN], 2);

}

A_BOOL _qcom_p2p_check_scc_mode(void)
{
    A_BOOL ret = FALSE;
    
    if ((gNumDev > 1) && (0 == gMCCEnabled)) {
        ret = TRUE;
    }

    return ret;
}

A_STATUS _qcom_p2p_get_peer_go_op_ch_freq(int dev_id, A_UINT8 *ppeer_addr, A_UINT16 *poper_freq)
{
    P2P_DEV_CTX *pp2p_ctx = NULL;
    struct node_p2p_dev *pp2p_node;

    if ((dev_id >= gNumDev) || (NULL == ppeer_addr)) {
        return A_ERROR;
    }

    if (A_OK != _qcom_p2p_get_dev_ctx_point(dev_id, &pp2p_ctx)) {
        return A_ERROR;
    }

    if (NULL == pp2p_ctx) {
        return A_ERROR;
    }

    //pp2p_node = p2p_get_device(pp2p_ctx, ppeer_addr);
    pp2p_node = p2p_get_device_intf_addrs(pp2p_ctx, ppeer_addr);
    if(NULL == pp2p_node){
        return A_ERROR;
    }

    *poper_freq = pp2p_node->dev.oper_freq;

    return A_OK;
}

A_STATUS _qcom_p2p_show_status(int dev_id)
{
    A_STATUS ret;
    P2P_DEV_CTX *pstp2p_ctx;

    if (dev_id >= gNumDev) {
        return A_ERROR;
    }

    ret = _qcom_p2p_get_dev_ctx_point(dev_id, &pstp2p_ctx);
    if (A_DEVICE_NOT_FOUND == ret) {
        QCOM_P2P_DEBUG_PRINTF("P2p is not init on dev %d\n", dev_id);
    }
    else if (A_OK != ret) {
        QCOM_P2P_DEBUG_PRINTF("Failed to get p2p ctx of dev %d\n", dev_id);
    }
    else {
        A_UINT8 ucsub_mode;
        _qcom_dev_get_sub_mode(dev_id, &ucsub_mode);
        QCOM_P2P_PRINTF("name : %s\n", pstp2p_ctx->wpsInfo.device_name);
        QCOM_P2P_PRINTF("mode : %s\n", \
                         (ucsub_mode == SUB_OPMODE_P2PDEV)?"SUB_OPMODE_P2PDEV":\
                         (ucsub_mode == SUB_OPMODE_P2P_CLIENT)?"SUB_OPMODE_P2P_CLIENT":\
                         (ucsub_mode == SUB_OPMODE_P2P_GO)?"SUB_OPMODE_P2P_GO":\
                         (ucsub_mode == SUB_OPMODE_P2PDEV_DEDICATE)?"SUB_OPMODE_P2PDEV_DEDICATE":\
                         (ucsub_mode == SUB_OPMODE_FW_P2PDEV)?"SUB_OPMODE_FW_P2PDEV":\
                         (ucsub_mode == SUB_OPMODE_FW_P2P_CLIENT)?"SUB_OPMODE_FW_P2P_CLIENT":\
                         (ucsub_mode == SUB_OPMODE_FW_P2P_GO)? \
                         "SUB_OPMODE_FW_P2P_GO":"SUB_OPMODE_NONE");
        QCOM_P2P_PRINTF("country : %s\n", pstp2p_ctx->country);
        QCOM_P2P_PRINTF("listen-channel : 0x%x\n", pstp2p_ctx->channel);
        QCOM_P2P_PRINTF("operat-channel : 0x%x\n", pstp2p_ctx->op_channel);
        QCOM_P2P_PRINTF("cfg-go-intent : %d\n", pstp2p_ctx->cfg_go_intent);
        QCOM_P2P_PRINTF("state : %s\n", \
                         (pstp2p_ctx->state == P2P_IDLE)?"P2P_IDLE":\
                         (pstp2p_ctx->state == P2P_SEARCH)?"P2P_SEARCH":\
                         (pstp2p_ctx->state == P2P_CONNECT)?"P2P_CONNECT":\
                         (pstp2p_ctx->state == P2P_CONNECT_LISTEN)?"P2P_CONNECT_LISTEN":\
                         (pstp2p_ctx->state == P2P_GO_NEG)?"P2P_GO_NEG":\
                         (pstp2p_ctx->state == P2P_LISTEN_ONLY)?"P2P_LISTEN_ONLY":\
                         (pstp2p_ctx->state == P2P_WAIT_PEER_CONNECT)?"P2P_WAIT_PEER_CONNECT":\
                         (pstp2p_ctx->state == P2P_WAIT_PEER_IDLE)?"P2P_WAIT_PEER_IDLE":\
                         (pstp2p_ctx->state == P2P_SD_DURING_FIND)?"P2P_SD_DURING_FIND":\
                         (pstp2p_ctx->state == P2P_PROVISIONING)?"P2P_PROVISIONING":\
                         (pstp2p_ctx->state == P2P_PD_DURING_FIND)?"P2P_PD_DURING_FIND":\
                         (pstp2p_ctx->state == P2P_INVITE)?"P2P_INVITE":\
                         (pstp2p_ctx->state == P2P_INVITE_LISTEN)?"P2P_INVITE_LISTEN":\
                         (pstp2p_ctx->state == P2P_INVITATION_IND_TO_HOST)?\
                         "P2P_INVITATION_IND_TO_HOST":"UNKNOWN");
        QCOM_P2P_PRINTF("search state : %s\n", \
                         (pstp2p_ctx->search_state == SEARCH_LISTEN)?"SEARCH_LISTEN":\
                         (pstp2p_ctx->search_state == SEARCH_SCAN)?"SEARCH_SCAN":\
                         (pstp2p_ctx->search_state == SEARCH_SDPD)?"SEARCH_SDPD":\
                         (pstp2p_ctx->search_state == SEARCH_PD_FIND)?\
                         "SEARCH_PD_FIND":"UNKNOWN");
        QCOM_P2P_PRINTF("pending action state : %s\n", \
                         (pstp2p_ctx->pending_action_state == P2P_NO_PENDING_ACTION)?"P2P_NO_PENDING_ACTION":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_GO_NEG_REQUEST)?"P2P_PENDING_GO_NEG_REQUEST":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_GO_NEG_RESPONSE)?"P2P_PENDING_GO_NEG_RESPONSE":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_GO_NEG_RESPONSE_FAILURE)?"P2P_PENDING_GO_NEG_RESPONSE_FAILURE":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_GO_NEG_CONFIRM)?"P2P_PENDING_GO_NEG_CONFIRM":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_SD)?"P2P_PENDING_SD":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_PD)?"P2P_PENDING_PD":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_INVITATION_REQUEST)?"P2P_PENDING_INVITATION_REQUEST":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_INVITATION_RESPONSE)?"P2P_PENDING_INVITATION_RESPONSE":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_DEV_DISC_REQUEST)?"P2P_PENDING_DEV_DISC_REQUEST":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_DEV_DISC_RESPONSE)?"P2P_PENDING_DEV_DISC_RESPONSE":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_GO_DISC_REQ)?"P2P_PENDING_GO_DISC_REQ":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_PRESENCE_REQUEST)?"P2P_PENDING_PRESENCE_REQUEST":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_PRESENCE_RESPONSE)?"P2P_PENDING_PRESENCE_RESPONSE":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_SD_COMEBACK_REQ)?"P2P_PENDING_SD_COMEBACK_REQ":\
                         (pstp2p_ctx->pending_action_state == P2P_PENDING_SD_RESP)?\
                         "P2P_PENDING_SD_RESP":"UNKNOWN");
        QCOM_P2P_PRINTF("go-intend : %d\n", pstp2p_ctx->go_intent);
        QCOM_P2P_PRINTF("ssid : %s\n", pstp2p_ctx->ssid);
    }

    return A_OK;
}

A_STATUS _qcom_p2p_init(int dev_id)
{
    devh_t *pdev = NULL;
    static int p2p_init_flag = 0;
    
    if (p2p_init_flag) {
        return A_OK;
    }

    if (A_OK != _qcom_dev_get_dev_point(dev_id, &pdev)) {
        return A_ERROR;
    }

    if (NULL == pdev) {
        return A_ERROR;
    }

    wmi_switch_mode(pdev, WHAL_M_STA, SUB_OPMODE_FW_P2PDEV);

    P2P_INIT(pdev, NULL);
    p2p_init_flag = 1;

    return A_OK;
}

A_STATUS _qcom_p2p_ps_clean_go_schedules(int dev_id)
{
    P2P_DEV_CTX *p2p_dev_ctx = NULL;

    if (dev_id >= gNumDev) {
        return A_ERROR;
    }

    if (A_OK != _qcom_p2p_get_dev_ctx_point(dev_id, &p2p_dev_ctx)) {
        return A_ERROR;
    }

    if (NULL == p2p_dev_ctx)
    {
        return A_ERROR;
    }
    
     if (p2p_dev_ctx->noa_initialized) {
        A_UNTIMEOUT(&p2p_dev_ctx->go_schedules->h_tsftimer);
        p2p_go_schedule_setup(devArray[dev_id], NULL, 0, NULL);
        p2p_dev_ctx->noa.ctwindow = 0;
        p2p_dev_ctx->noa.num_descriptors = 0;
        p2p_dev_ctx->noa_initialized = FALSE;
        p2p_dev_ctx->go_schedules->tsftimer_started = FALSE;
        p2p_dev_ctx->go_schedules->go_present = FALSE;
        p2p_dev_ctx->go_schedules->callback_active = FALSE;
        p2p_dev_ctx->go_schedules->paused = FALSE;
        p2p_dev_ctx->go_schedules->num_schedules = 0;
    }

    return A_OK;
}

A_STATUS _qcom_p2p_event_node_list(A_UINT8 device_id, A_UINT8 * pEvtBuffer, int len)
{
    if (p2p_msg_area_ptr) {
        A_UINT32 status;
        if (pEvtBuffer) {
            /* Copy to common message area and then call tx_queue_send to wake up the
             * thread waiting on receive. No need for locking the message area. Race
             * condition between application threads is prevented by having a check on
             * tx_queue_create. If queue is already created (and in use), API will
             * return an error
             */
            A_MEMCPY(p2p_msg_area_ptr, pEvtBuffer, NODELIST_MSG_BYTES);
            status = 1;
        }
        else {
            status = 0;
        }

        if (p2p_msg_queue.tx_queue_id ==  TX_QUEUE_ID) {
            tx_queue_send(&p2p_msg_queue, &status, TX_NO_WAIT);
        }
        return A_OK;
    }
    return A_ERROR;
}

A_STATUS _qcom_p2p_event_persistent_list(A_UINT8 device_id, A_UINT8 * pEvtBuffer, int len)
{
    if (p2p_msg_area_ptr) {
        A_UINT32 status;
        if (pEvtBuffer) {
            /* Copy to common message area and then call tx_queue_send to wake up the
             * thread waiting on receive. No need for locking the message area. Race
             * condition between application threads is prevented by having a check on
             * tx_queue_create. If queue is already created (and in use), API will
             * return an error
             */
            A_MEMCPY(p2p_msg_area_ptr, pEvtBuffer, P2P_NETWORK_LIST_SIZE);
            status = 1;
        }
        else {
            status = 0;
        }

        if (p2p_msg_queue.tx_queue_id ==  TX_QUEUE_ID) {
            tx_queue_send(&p2p_msg_queue, &status, TX_NO_WAIT);
        }
        
        return A_OK;
    }
    return A_ERROR;
}

A_STATUS _wmi_p2p_priv_event_handler(void *pContext,
                            A_UINT16 EventID,
                            A_UINT16 info,
                            A_UINT8 * pEvtBuffer,
                            int Length)
{
    P2P_EVENT_ID app_evt_id;
	A_UINT8 device_id = info & 0xF;

    if (device_id >= gNumDev) {
        return A_ERROR;
    }

    /* Private events */
    if (EventID == WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID) {
        qcom_p2p_event_persistent_list(device_id, pEvtBuffer, Length);
    }
    else if (EventID == WMI_P2P_NODE_LIST_EVENTID) {
        qcom_p2p_event_node_list(device_id, pEvtBuffer, Length);
    }
    else {
        switch (EventID) {
            case WMI_P2P_GO_NEG_RESULT_EVENTID:
                app_evt_id = P2P_GO_NEG_RESULT_EVENTID;
                break;
            case WMI_P2P_GO_NEG_REQ_EVENTID:
                app_evt_id = P2P_GO_NEG_REQ_EVENTID;
                break;
            case WMI_P2P_INVITE_REQ_EVENTID:
                app_evt_id = P2P_INVITE_REQ_EVENTID;
                break;
            case WMI_P2P_INVITE_RCVD_RESULT_EVENTID:
                app_evt_id = P2P_INVITE_RCVD_RESULT_EVENTID;
                break;
            case WMI_P2P_INVITE_SENT_RESULT_EVENTID:
                app_evt_id = P2P_INVITE_SENT_RESULT_EVENTID;
                break;
            case WMI_P2P_PROV_DISC_REQ_EVENTID:
                app_evt_id = P2P_PROV_DISC_REQ_EVENTID;
                break;
            case WMI_P2P_PROV_DISC_RESP_EVENTID:
                app_evt_id = P2P_PROV_DISC_RESP_EVENTID;
                break;
            case WMI_P2P_START_SDPD_EVENTID:
                app_evt_id = P2P_START_SDPD_EVENTID;
                break;
            case WMI_P2P_SDPD_RX_EVENTID:
                app_evt_id = P2P_SDPD_RX_EVENTID;
                break;
            case WMI_P2P_REQ_TO_AUTH_EVENTID:
                app_evt_id = P2P_REQ_TO_AUTH_EVENTID;
                break;
            default:
                return -1;
        }
        /* call the application callback function */
        if (NULL != p2p_event_handler) {
            p2p_event_handler(device_id, app_evt_id, pEvtBuffer, Length, p2p_event_handler_arg);
        }
    }
    return 0;
}

void _qcom_p2p_priv_discconnect_event_process(void *pContext,    /* app supplied context */
                            A_UINT16 EventID,  /* event ID */
                            A_UINT16 info, /* the info param passed to WMI_SendEvent */
                              A_UINT8  * pEvtBuffer,  /* pointer to event data, if needed */
                            int Length)
{
    A_UINT8 device_id = info & 0xFF;
    _qcom_p2p_ps_clean_go_schedules(device_id);

    return;
}

A_STATUS _qcom_p2p_event_register(void)
{
#ifdef LIBCUST_INDIRECT_ACCESS
    extern QCOM_EVENT_API_INDIRECTION_TABLE qcomEventApiIndirectionTable;
    qcomEventApiIndirectionTable._qcom_p2p_event_process = QCOM_P2P_INT_FN(_wmi_p2p_priv_event_handler);
#endif

    extern void (* _qcom_p2p_disconnect_event_process)(void *pContext,A_UINT16 EventID,A_UINT16 info,A_UINT8 * pEvtBuffer,int Length);
    _qcom_p2p_disconnect_event_process = _qcom_p2p_priv_discconnect_event_process;

    return;
}

#ifdef LIBCUST_INDIRECT_ACCESS
QCOM_P2P_INT_INDIRECTION_TABLE qcomP2pFuncIntIndirectionTable = {
    _wmi_p2p_priv_event_handler,
    _qcom_p2p_func_set_channel_freq_cfg,
    _qcom_p2p_event_register,
    _qcom_p2p_init,
    _qcom_p2p_check_scc_mode,
    _qcom_p2p_get_peer_go_op_ch_freq,
    _qcom_p2p_gen_ssid,
    _qcom_p2p_event_persistent_list,
    _qcom_p2p_event_node_list,
    _qcom_p2p_show_status,
};
#endif
#endif

