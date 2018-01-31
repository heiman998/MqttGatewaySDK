/*
  * Copyright (c) 2013 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */
#if defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)

#include <osapi.h>
#include <nvram_api.h>
#include "threadx/tx_api.h"
#include "threadx/tx_semaphore.h"

#include <htc_api.h>
#include "htc_internal.h"
#include <AR6K/AR6K_soc.h>
#include <hw/analog_intf_athr_wlan_reg.h>
#include <AR6K_version.h>
#include <wlan_dev.h>
#include <storerecall_api.h>
#include <wps_api.h>
#include <ipport.h>
#include "socketLayer_api.h"
#include <wlan_framegen.h>
#include <p2p_api.h>
#include <argos.h>
#include <wmi.h>
#include <wmi_api.h>
#include <wlan_wmi.h>
#include <keymgmt_api.h>
//#include "qcom/wlan_wifi.h"

#include "host_proxy_iface.h"
#include <socketLayer.h>
#include <qcom_system.h>
#include "qcom_mem.h"
#include <threadxdmn_api.h>
#include <qcom_wlan.h>
#include <qcom_event.h>
#include "qcom_wps.h"
#include "tx_alloc_api.h"
#include "qcom_cdr.h"
#include "tunable_api.h"
#include "rng_api.h"
#include "qcom_internal_api.h"
#include "qcom_pwr.h"
#include "misc_cdr.h"
#include <cryptolib_api.h>
#include "hw/gpio_reg.h"

/* This application replaces to some extent the host during boot.
 * The application replaces several HTC api's to accomplish this
 * task as it is desired for the athwlan FW to behave normally so
 * far as not to be aware that it is actually communicating with an
 * application on-chip rather than a host driver.  After it completes
 * all of its duties this application will unload itself and return
 * the HTC api's to their original State, thus allowing the FW to
 * resume/start communication with the host.
 */

/* NOTE: The start address and the extended board data size overlap by 4-bits
 *  it is assumed that the lower 4 bits of the start_addr is always 0000. */
#define IN_PARAM_EXT_BD_DATA_MASK 0xfff
#define IN_PARAM_APP_START_ADDR_SHIFT 8

#define CHIP_SPECIFIC_RAM_RESERVE_SIZE       (1024) //(11264)
#define CHIP_SPECIFIC_BOARD_DATA_ADDR        (0x4de000)

extern devh_t *gdevp;
extern A_BOOL AR6K_host_present;

extern struct nvram_config_s nv_config_mio_quad1;
extern void qcom_phost_init();

typedef void (*type_HTC_module_Install) (struct htc_apis * pAPIs);

HTC_CONTEXT *pHTCInstance = NULL;
void (*real_HTC_ReturnBuffers) (void *pInstance, HTC_ENDPOINT_ID EndpointID, HTC_BUFFER * pBuffers);
void (*real_HTC_SendMsg) (void *pInstance, HTC_ENDPOINT_ID EndpointID, HTC_BUFFER * pBuffers);

static A_BOOL argos_feature_enabled = FALSE;    /*Flag to control argos feature intialization */

extern A_UINT8 gMccEnabled;
extern A_UINT8 gNumOfWlanDevices ;
extern volatile int assloop;
extern A_UINT8 scan_timeout;
unsigned long boot_time;
extern A_UINT64 cmnos_milliseconds(void);
extern char *reset_cause_string;

void sys_power_control(int sleepOn)
{
    if (sleepOn == 0)
       qcom_set_power_mode(0, MAX_PERF_POWER);
    else
       qcom_set_power_mode(0, REC_POWER);
}

void
_qcom_HTC_ReturnBuffers(void *pInstance, HTC_ENDPOINT_ID EndpointID, HTC_BUFFER * pBuffers)
{
    //NOP
}

void
_qcom_wmi_commands(void)
{
    union {
        WMI_SET_KEEPALIVE_CMD keepalive;
        WMI_POWER_MODE_CMD powermode;
        WMI_SET_HT_CAP_CMD htcap;
        WMI_ALLOW_AGGR_CMD aggrallow;
        //WMI_SCAN_PARAMS_CMD scanparams;
    } u;

    HTC_ENDPOINT *pEndpoint;
    A_UCHAR buffer[sizeof (WMI_CMD_HDR) + sizeof (u)];
    HTC_BUFFER htcHdr;
    WMI_CMD_HDR wmiHdr;
    int i;
    A_BOOL argos_enabled = 0;

    A_MEMZERO(&wmiHdr, sizeof (WMI_CMD_HDR));

    for (i = 0; i < ENDPOINT_MAX; i++) {
        if (pHTCInstance->Endpoints[i].pService->ServiceID == WMI_CONTROL_SVC) {
            pEndpoint = &pHTCInstance->Endpoints[i];
            break;
        }
    }

    A_ASSERT(i < ENDPOINT_MAX);
    /* hook HTC_ReturnBuffers() to catch the buffer recycle */
    real_HTC_ReturnBuffers = A_INDIR(htc._HTC_ReturnBuffers);
    A_INDIR(htc._HTC_ReturnBuffers) = _qcom_HTC_ReturnBuffers;
#if 0
    /* set pm params */

    htcHdr.buffer = buffer;
    A_MEMCPY(&buffer[0], wmiHdr, sizeof (WMI_CMD_HDR));
    htcHdr.actual_length =;
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
    /* set l preamble */
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
#endif
    /* set keep alive */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_SET_KEEPALIVE_CMD);
    wmiHdr.commandId = WMI_SET_KEEPALIVE_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    u.keepalive.keepaliveInterval = 60; //0; karl debug
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_SET_KEEPALIVE_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
    /* set power mode */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_POWER_MODE_CMD);
    wmiHdr.commandId = WMI_SET_POWER_MODE_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));

    u.powermode.powerMode = ((argos_enabled) ? REC_POWER : MAX_PERF_POWER);
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_POWER_MODE_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);

    /* set scan parameters */
#if 0
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_SCAN_PARAMS_CMD);
    wmiHdr.commandId = WMI_SET_SCAN_PARAMS_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    /*Set dwell time to IOT default (60 ms) to allow success in crowded environments */
    A_MEMZERO(&u.scanparams, sizeof (WMI_SCAN_PARAMS_CMD));
    u.scanparams.maxact_chdwell_time = 60;
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_SCAN_PARAMS_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
#endif
#if 0                           /* use default settings in athwlan */
    /* set HT caps 5GHz */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_SET_HT_CAP_CMD);
    wmiHdr.commandId = WMI_SET_HT_CAP_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    A_MEMZERO(&u.htcap, sizeof (WMI_SET_HT_CAP_CMD));
    u.htcap.band = A_BAND_5GHZ;
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_SET_HT_CAP_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
    /* set HT caps 2.4GHz */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_SET_HT_CAP_CMD);
    wmiHdr.commandId = WMI_SET_HT_CAP_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    A_MEMZERO(&u.htcap, sizeof (WMI_SET_HT_CAP_CMD));
    u.htcap.band = A_BAND_24GHZ;
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_SET_HT_CAP_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);
#endif

    /* set aggregation so that AMPDU aggregation is not allowed */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + sizeof (WMI_ALLOW_AGGR_CMD);
    wmiHdr.commandId = WMI_ALLOW_AGGR_CMDID;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    u.aggrallow.tx_allow_aggr = 0;
    u.aggrallow.rx_allow_aggr = 0;
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], &u, sizeof (WMI_ALLOW_AGGR_CMD));
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);

    /* unhook HTC_ReturnBuffers() */
    A_INDIR(htc._HTC_ReturnBuffers) = real_HTC_ReturnBuffers;

}

void
_qcom_wmi_wrap(WMI_COMMAND_ID cmdId, A_UINT8 * pCmd, int cmdLength)
{
    union {
        WMI_CONNECT_CMD conn;
        WMI_ADD_CIPHER_KEY_CMD addkey;
        WMI_SET_PASSPHRASE_CMD setPass;
        WMI_SOCKET_CMD sockCmd;
    } u;
    HTC_ENDPOINT *pEndpoint;
    A_UCHAR buffer[sizeof (WMI_CMD_HDR) + sizeof (u)];
    HTC_BUFFER htcHdr;
    WMI_CMD_HDR wmiHdr;
    int i;

    A_MEMZERO(&wmiHdr, sizeof (WMI_CMD_HDR));

    for (i = 0; i < ENDPOINT_MAX; i++) {
        if (pHTCInstance->Endpoints[i].pService->ServiceID == WMI_CONTROL_SVC) {
            pEndpoint = &pHTCInstance->Endpoints[i];
            break;
        }
    }

    /* hook HTC_ReturnBuffers() to catch the buffer recycle */
    real_HTC_ReturnBuffers = A_INDIR(htc._HTC_ReturnBuffers);
    A_INDIR(htc._HTC_ReturnBuffers) = _qcom_HTC_ReturnBuffers;

    /* set passphrase */
    htcHdr.buffer = buffer;
    htcHdr.actual_length = sizeof (WMI_CMD_HDR) + cmdLength;
    wmiHdr.commandId = cmdId;
    A_MEMCPY(&buffer[0], &wmiHdr, sizeof (WMI_CMD_HDR));
    A_MEMCPY(&buffer[sizeof (WMI_CMD_HDR)], pCmd, cmdLength);
    pEndpoint->pService->ProcessRecvMsg(pEndpoint->Eid, &htcHdr);

    /* unhook HTC_ReturnBuffers() */
    A_INDIR(htc._HTC_ReturnBuffers) = real_HTC_ReturnBuffers;
}

/* iotdebug_HTC_SendMsg() -- captures messages heading for the host and frees them back to HTC
 * as if the transmission had completed.  This is to prevent unwanted transmissions to the
 * host.  The host_proxy will return the HTC_SendMsg indirection to its original function
 * when it has finished its operations.
 */
void
_qcom_HTC_SendMsg(void *pInstance, HTC_ENDPOINT_ID EndpointID, HTC_BUFFER * pBuffers)
{
    /* store the HTC instance away for future reference. */
    if (pHTCInstance == NULL) {
        pHTCInstance = (HTC_CONTEXT *) pInstance;
    }

    // free the msg thereby preventing it from going to the host.
    HTCFreeMsgBuffer(pInstance, pBuffers);
}

void
_qcom_HTC_SendMsg2(void *pInstance, HTC_ENDPOINT_ID EndpointID, HTC_BUFFER * pBuffers)
{
    extern int user_bgrs_htc;

    /* call the original _HTC_SendMsg */
    real_HTC_SendMsg(pInstance, EndpointID, pBuffers);

    /* send a signal to do HTC house keeping */
    if (user_bgrs_htc > 0)
        A_BGR_SIGNAL_BGR(user_bgrs_htc);

}

void
_qcom_HTC_Housekeeping(void *data_ptr, int num)
{
    /* refer to _MboxHW_ISR(), clean up all message in the output queue. please fix me if you are a HTC expert */
    HTC_INTERN_API_FN(_HTCSendDoneHandler) (pHTCInstance, 0);   //mbox is always 0.

}

void
_qcom_HTCSetupComplete(HTC_CONTEXT * pHTCInst)
{
    pHTCInst->MaxMsgPerSendBundle = HTC_MAX_MSG_PER_BUNDLE;
    pHTCInst->StateFlags |= HTC_STATE_SETUP_COMPLETE;
    if (!(pHTCInst->StateFlags & HTC_STATE_BUFF_REALLOC)) {
        /* check and realloc ctrl buffers */
        CheckAndReallocCtrlBuffers(pHTCInst);
    }
    if (pHTCInst->SetupCompleteCb != NULL) {
        pHTCInst->SetupCompleteCb();
    }
    if (pHTCInst->SetupCompleteCbEx != NULL) {
        pHTCInst->SetupCompleteCbEx(pHTCInst->pSetupContext);
    }
}

void
_qcom_HTC_Postprocess(void)
{
    HTC_CONNECT_SERVICE_MSG connectMsg;

    /* Setup ENDPOINTS 1 thru 5 for WMI ctrl and 4 Data AC's */
    connectMsg.MessageID = 2;
    connectMsg.ServiceID = 0x0100;  /* service ID of the service to connect to */
    connectMsg.ConnectionFlags = 0; /* connection flags */
    connectMsg.ServiceMetaLength = 0;
    HTCProcessConnectMsg(pHTCInstance, &connectMsg);

    connectMsg.MessageID = 2;
    connectMsg.ServiceID = 0x0101;  /* service ID of the service to connect to */
    connectMsg.ConnectionFlags = 0x05;  /* connection flags */
    connectMsg.ServiceMetaLength = 0;
    HTCProcessConnectMsg(pHTCInstance, &connectMsg);

    connectMsg.MessageID = 2;
    connectMsg.ServiceID = 0x0102;  /* service ID of the service to connect to */
    connectMsg.ConnectionFlags = 0x05;  /* connection flags */
    connectMsg.ServiceMetaLength = 0;
    HTCProcessConnectMsg(pHTCInstance, &connectMsg);

    connectMsg.MessageID = 2;
    connectMsg.ServiceID = 0x0103;  /* service ID of the service to connect to */
    connectMsg.ConnectionFlags = 0x05;  /* connection flags */
    connectMsg.ServiceMetaLength = 0;
    HTCProcessConnectMsg(pHTCInstance, &connectMsg);

    connectMsg.MessageID = 2;
    connectMsg.ServiceID = 0x0104;  /* service ID of the service to connect to */
    connectMsg.ConnectionFlags = 0x05;  /* connection flags */
    connectMsg.ServiceMetaLength = 0;
    HTCProcessConnectMsg(pHTCInstance, &connectMsg);

    A_INDIR(htc._HTC_SendMsg) = _qcom_HTC_SendMsg2;  //real_HTC_SendMsg;
    /* unfortunately HTC setup complete cannot be initiated by calling
     * HTCProcessControlMsg() as that function will recycle the incoming
     * buffer back to the receive message queue. This can foul up the
     * credit report logic as the message did not originate from the host.
     * Instead this code performs the tasks that would normally be
     * performed by HTCProcessControlMsg(). A better fix would be to, within HTC,
     * separate the code that processes the setup complete message from the
     * code that returns the buffer to the message queue.
     */
    _qcom_HTCSetupComplete(pHTCInstance);
    _qcom_wmi_commands();
#if 0
    qcom_wlan_power_wakeup_start(60 * 60 * 1000);
#endif

#if 0
    qcom_wlan_unit_test();
#endif

}

void (*save_wlan_init_done_fn)(void) = NULL;

void qcom_wlan_init_done(void)
{
    /* execute previously-assigned override */
    if (save_wlan_init_done_fn) {
        save_wlan_init_done_fn();
    }
    RNG_Init();

    /* Needed for security APIs */
    crypto_mgmt_init();

    return;
}

#if defined(FPGA)
//HTC_CONTEXT *pHTCInstance = NULL;
//void (* real_HTC_SendMsg)(void *pInstance,HTC_ENDPOINT_ID EndpointID, HTC_BUFFER *pBuffers);
void fpga_HTC_SendMsg(void *pInstance,HTC_ENDPOINT_ID EndpointID, HTC_BUFFER *pBuffers)
{
    /* store the HTC instance away for future reference. */
    if(pHTCInstance == NULL){
        pHTCInstance = (HTC_CONTEXT *)pInstance;
    }

    //A_PRINTF_ALWAYS("temp_HTC_SendMsg: %d %d\n", EndpointID, pBuffers->actual_length );
    // free the msg thereby preventing it from going to the host.
    HTCFreeMsgBuffer(pInstance, pBuffers);
}

void fpga_wlan_init_done(void)
{
    real_HTC_SendMsg = A_INDIR(htc._HTC_SendMsg);
    A_INDIR(htc._HTC_SendMsg) = fpga_HTC_SendMsg;
    //htc_patch_install();
    qcom_wlan_init_done();
}
#endif

/* Max Num of clients in AP Mode */
#define MAX_AP_CLIENT_NUM  10

void
_qcom_pre_start(void)
{
#if defined (FPGA)
    /* the ROM_REMAP feature is not yet integrated into the FPGA and because the ROM image is in development
     * the HTC_MODULE_INSTALL_ADDR will change with every new image.  so instead the host_proxy relies on
     * wlan_init_done_fn() which serves the same purpose of providing an appropriate opportunity to replace
     * _HTC_SendMsg(). */
    //DBG("FPGA\n");
    extern void (*wlan_init_done_fn)(void);
    wlan_init_done_fn = fpga_wlan_init_done;

    AR6K_host_present = 0;
    //extern A_BOOT_INFO AR6K_boot_info;
    //AR6K_boot_info.interconnect_type = A_HOST_IC_SDIO; //A_HOST_IC_NONE;
#else
#if 0
    struct nvram_state_s *nvram_state;
    nvram_state = (struct nvram_state_s *)(HOST_INTEREST->hi_nvram_state);

    A_PRINTF_ALWAYS("Build time(%s)\n", __TIME__);
    //A_PRINTF_ALWAYS("board_type = %d, hw_cfg = 0x%02x\n", board_type_get(), board_cfg_get());
    A_PRINTF_ALWAYS("NVM part current(0x%x)\n", nvram_state->_partition_start_current);
    A_PRINTF_ALWAYS("NVM part0 offset(0x%x)\n", nvram_state->_nvram_config.nvram_partition_offset[0]);
    A_PRINTF_ALWAYS("NVM part1 offset(0x%x)\n", nvram_state->_nvram_config.nvram_partition_offset[1]);
    A_PRINTF_ALWAYS("NVM part2 offset(0x%x)\n", nvram_state->_nvram_config.nvram_partition_offset[2]);
    A_PRINTF_ALWAYS("NVM part3 offset(0x%x)\n", nvram_state->_nvram_config.nvram_partition_offset[3]);
#endif
    A_PRINTF_ALWAYS("reset cause:%s\n", reset_cause_string);
    //fix issue CR725550
	nv_config_mio_quad1.si_sdio_value = 0;

    extern void (*wlan_init_done_fn)(void);
    extern void null_fn(void);
    /* check if this function has been assigned elsewhere */
    if (wlan_init_done_fn != null_fn) {
        save_wlan_init_done_fn = wlan_init_done_fn;
    }
    wlan_init_done_fn = qcom_wlan_init_done; 


    /* it is needed by v7.2 and v7.4 */
    AR6K_host_present = 0;
#endif

#ifdef CONFIG_TUNABLE
    /* Read tunables from a dataset and derive VALID bits */
    sys_tunables_init();
#endif /* CONFIG_TUNABLE */

    qcom_phost_init();

    if (HOST_INTEREST->hi_board_data) {
        HOST_INTEREST->hi_board_data_initialized = HI_BDATA_INITED; /* backwards compat */
    }

    regist_power_save_handler(uart_pre_sleep_handler, uart_post_awake_handler);

    return;
}

void
_qcom_pre_init(void)
{
#if !defined (REV74_TEST_ENV4)    
    STRRCL_INIT(gdevp);
#endif

    if (argos_feature_enabled == TRUE) {
        ARGOS_INIT(gdevp);
    } else {
        SOCKETLAYER_INIT(gdevp);
    }

    /* register WMI Event Filter */
    /*
    *Befor the user application is started, maybe there are
    *some events sent from firmware.
    *At this time, the CDR has not been initiated.
    *And those events cannot be process normally.
    *So we register a event filter to discard those events.
    *It will be canceled after user application is started.
    */
    extern void qcom_sta_register_callbacks();
    qcom_sta_register_callbacks();

    /* init device context */
    
    qcom_init_dev_context();

    qcom_init_ipstack();

    user_app_init();

    /* this call-back is called by APIs in ROM. */
#if defined(P2P_ENABLED)
#ifdef CONFIG_P2P_FEATURE            
    extern void qcom_p2p_func_install_cb(void);
    qcom_p2p_func_install_cb();
#endif
#endif    
    return;
}

/*
*The patch maybe needed by all ASIC version.
*/

void
_qcom_pre_patch(void)
{
#if defined(AR6002_REV72)
    extern void (*whalPreInitFn) (WHAL_STRUCT *);
    extern void whal_pre_init_callback(WHAL_STRUCT *);
    whalPreInitFn = whal_pre_init_callback;

    extern void (*wlan_patch_init_fn) (void);
    extern void install_patches(void);
    wlan_patch_init_fn = install_patches;
#else //AR6002_REV72
     extern void (*wlan_patch_init_fn) (void);
    extern void install_patches(void);
    wlan_patch_init_fn = install_patches;
#endif

    return;
}


void
_qcom_sys_dbg_uart_set(A_UINT32 uiinput_param)
{
#if defined (AR6002_REV7)

#if defined(FPGA)

    /* the GPIO pins used by debug UART are diffrent from FPGA and ASIC board. */
    HOST_INTEREST->hi_desired_baud_rate = 115200;
    HOST_INTEREST->hi_dbg_uart_txpin = 23; 
    HOST_INTEREST->hi_dbg_uart_rxpin = 24; 
    A_ENABLE_DEBUG_PRINT();
    
#else  /* else of defined (FPGA) */

    HOST_INTEREST->hi_desired_baud_rate = HI_OPTION_BAUDRATE;
    //HOST_INTEREST->hi_dbg_uart_txpin = HI_UART_DBG_TXPIN;
    /*This is for Ruby HW7.5 HSUART, fixme later when using other UART*/
    HOST_INTEREST->hi_dbg_uart_txpin = 29;//23;//29;
    HOST_INTEREST->hi_dbg_uart_rxpin = 28;//24;//28;
    if(0 == (uiinput_param & 0x04)) {
        A_ENABLE_DEBUG_PRINT();
    }
    
#endif  /* end of defined (FPGA) */

#endif  /* end of defined (AR6002_REV7) */

    return;
}

void
_qcom_sys_clock_set(void)
{
    static int config_flag = 0;
    
    if (0 == config_flag)
    {
        config_flag = 1;
#if defined(FPGA)

        HOST_INTEREST->hi_refclk_hz = 26000000;
        A_CLOCK_INIT(HOST_INTEREST->hi_refclk_hz);
        A_CLOCKREGS_INIT();
        //A_CPU_CLOCK_SELECT(AR6K_CPU_CLOCK_SELECT_CPU_PLL);
        A_CPU_CLOCK_SELECT(AR6K_CPU_CLOCK_SELECT_REFCLK);
        
#else  /* else of defined (FPGA) */
        /* for kingfisher the host must set the refclk freq. Default ref clock for Kingfisher is 40000000. */
        HOST_INTEREST->hi_refclk_hz = HI_REF_CLOCK;
        //A_RTC_SOC_REG_WRITE(CPU_CLOCK_ADDRESS, CPU_CLOCK_STANDARD_SET(0));
        A_RTC_SOC_REG_WRITE(SOC_LPO_CAL_ADDRESS, SOC_LPO_CAL_ENABLE_MASK);
        A_CLOCK_INIT(HOST_INTEREST->hi_refclk_hz);
        A_CLOCKREGS_INIT();
        A_CPU_CLOCK_SELECT(AR6K_CPU_CLOCK_SELECT_CPU_PLL);       
#endif  /* end of defined (FPGA) */
    }

    return;
}

void
_qcom_sys_device_config(A_UINT32 uidev_num)
{
    if (0 == uidev_num) {
        gNumOfWlanDevices = 1;
    }
    else {
        gNumOfWlanDevices = uidev_num;
    }

    if (2 == uidev_num) {
#ifdef P2P_ENABLED
        /* Set number of Device and device mode */
        /*clear dev-num*/
        HOST_INTEREST->hi_option_flag &= ~(HI_OPTION_NUM_DEV_MASK << HI_OPTION_NUM_DEV_SHIFT);
        /*clear fw-mode*/
        HOST_INTEREST->hi_option_flag &= ~(HI_OPTION_ALL_FW_MODE_MASK << HI_OPTION_FW_MODE_SHIFT);
        /*clear fw-sub_mode*/
        HOST_INTEREST->hi_option_flag &= ~(0xFF << HI_OPTION_FW_SUBMODE_SHIFT);

        /*
        * For concurrent mode, the device 0 is designed for P2P device and device 1 is for legacy
        * station.
        */
        /* device num */
        HOST_INTEREST->hi_option_flag |= (2 << HI_OPTION_NUM_DEV_SHIFT);
        /* device mode */
        HOST_INTEREST->hi_option_flag |= (((HI_OPTION_FW_MODE_BSS_STA<<2 | HI_OPTION_FW_MODE_BSS_STA)) << \
                                                                  HI_OPTION_FW_MODE_SHIFT);
        /* sub-device mode */
        HOST_INTEREST->hi_option_flag |= (((HI_OPTION_FW_SUBMODE_NONE << HI_OPTION_FW_SUBMODE_BITS) | \
                                                                   HI_OPTION_FW_SUBMODE_NONE ) << \
                                                                   HI_OPTION_FW_SUBMODE_SHIFT);

        HOST_INTEREST->hi_option_flag3 |= HI_OPTION_USE_OFFLOAD_P2P;
#else
        /* device 0 - Soft AP; device 1 - IEEE STA */
        HOST_INTEREST->hi_option_flag |= (2 << HI_OPTION_NUM_DEV_SHIFT) | \
            (((HI_OPTION_FW_MODE_BSS_STA<<2 | HI_OPTION_FW_MODE_AP)) << HI_OPTION_FW_MODE_SHIFT) | \
            (((HI_OPTION_FW_SUBMODE_NONE << HI_OPTION_FW_SUBMODE_BITS) | HI_OPTION_FW_SUBMODE_NONE ) << HI_OPTION_FW_SUBMODE_SHIFT);
#endif
    }
    else {
        /*station mode */
        HOST_INTEREST->hi_option_flag |= (1 << HI_OPTION_NUM_DEV_SHIFT) |\
                                                               (HI_OPTION_FW_MODE_BSS_STA << HI_OPTION_FW_MODE_SHIFT);
    }
    
    return;
}

void
_qcom_enable_mcc(A_UINT8 mcc_en)
{
    /* enable MCC */
    if(mcc_en) {
        HOST_INTEREST->hi_option_flag2 |= HI_OPTION_MCC_ENABLE;
    }

    return;
}

void
_qcom_sys_buffer_set(void)
{
    extern int ath_rxbuf;
    extern int ath_txbuf;
    extern int ath_mgmtbuf;

#if defined (REV74_TEST_ENV4)  
    ath_txbuf = 8;
    ath_rxbuf = 8;
    ath_mgmtbuf = 8;
#else
    /* FIXME: Needs to be removed since this is now handled by tunables */
    ath_txbuf = ATH_TX_BUF_NUM;
    ath_rxbuf = ATH_RX_BUF_NUM;
    ath_mgmtbuf = ATH_MGMT_BUF_NUM;
#endif
    
    return;
}

void
_qcom_sys_board_data_address_set(void)
{
    /* Handled via tunable */
}

void
_qcom_sys_dset_address_set(void)
{
    /* Handled via tunable */
}

void
_qcom_sys_switchable_ram_config(void)
{
    /* Handled via tunable */
}

void
_qcom_sys_reserved_ram_size_set(void)
{
    /* Handled via tunable */
}

void
_qcom_sys_amsdu_set(void)
{
    HOST_INTEREST->hi_option_flag2 |= HI_OPTION_OFFLOAD_AMSDU;
    return;
}

void
_qcom_sys_ap_client_num_set(void)
{
    #define MAX_AP_CLIENT_NUM  10
    HOST_INTEREST->hi_option_flag2 |= (MAX_AP_CLIENT_NUM << HI_OPTION_AP_CLIENT_CNT_SHIFT);

    return;
}

void
_qcom_sys_dtim_set(void)
{
    HOST_INTEREST->hi_option_flag2 |= (1 << HI_OPTION_AP_DTIM_VAL_SHIFT);
    return;
}

void
_qcom_sys_fw_bridge_set(void)
{
    HOST_INTEREST->hi_option_flag |= (0x01 << HI_OPTION_FW_BRIDGE_SHIFT);
    return;
}

void
_qcom_sys_bmi_skip(void)
{
    A_UINT32 val;
    
    /* To skip BMI phase set this flag */
    HOST_INTEREST->hi_pwr_save_flags = 0x31;
    val = A_MBOX_REG_READ(LOCAL_SCRATCH_ADDRESS);
    val |= AR6K_OPTION_BMI_DISABLE;
    A_MBOX_REG_WRITE(LOCAL_SCRATCH_ADDRESS, val);
    return;
}

void
_qcom_host_less_boot_config(A_UINT32 input_param)
{
/*
* For Asic mode, all the configuring are done in sdk_proxy.
* For HOST-LESS FPGA mode, we do not load and run sdk_proxy.
* So do some necessary configuration here.
*/

#if defined(FPGA) || defined(AR6002_REV75) || defined(AR6002_REV76)
    _qcom_sys_clock_set();// fixed issue that mess up UART output
    _qcom_enable_mcc(gMccEnabled);
    _qcom_sys_device_config(gNumOfWlanDevices);
    _qcom_sys_dbg_uart_set(input_param);
#ifndef FPGA
    //HOST_INTEREST->hi_dset_list_head = AR6006_REV5_DATASET_PATCH_ADDRESS;
#endif
    /* IOT driver relies on mbox credit reporting so we enable it here. */
    HOST_INTEREST->hi_option_flag2 |= HI_OPTION_USE_MBOX_CREDIT_REPORT;

    /* Set HTC Protocol Version */
    HOST_INTEREST->hi_app_host_interest = HTC_PROTOCOL_VERSION;

    /* IOT driver does not use debug logs so disable dbglogs */
    HOST_INTEREST->hi_option_flag |= HI_OPTION_DISABLE_DBGLOG;
    HOST_INTEREST->hi_pktlog_num_buffers = HI_PKTLOG_BUF_NUM;
    HOST_INTEREST->hi_hci_bridge_flags = HI_HCI_BRIGE_DISABLE;
    /* set block size */
    HOST_INTEREST->hi_mbox_io_block_sz = HI_MBOX_IO_SIZE;    //32;
    /* set yield limit */
    HOST_INTEREST->hi_mbox_isr_yield_limit = HI_MBOX_ISR_YIELD_LIMIT;
    /* provide indication that board data is initialized. The actual load
     * of bddata to RAM is performed separately by ROM NVRAM loader. */

    /* conditionally trim txrx buffers for IOT use. this will provide more RAM for
     * heap which is used by WPS and ipstack. also, by assigning these here at this
     * point we allow for other code to override these later before they are used by
     * MAC_DMA_AllocBuffer().
     */
    /* _qcom_sys_buffer_set(); */
    _qcom_sys_board_data_address_set();
    _qcom_sys_switchable_ram_config();
    _qcom_sys_dset_address_set();

    /* enable AMSDU slicing on-chip */
    _qcom_sys_amsdu_set();
    
#if defined(FPGA)
    HOST_INTEREST->hi_option_flag2 |= HI_OPTION_RADIO_RETENTION_DISABLE;
#endif

    /* Max Num of clients in AP Mode */
    _qcom_sys_ap_client_num_set();

    /* DTIM val in AP Mode */
    _qcom_sys_dtim_set();

    /* prevent regulatory scan from wlan_main_part2() */
    HOST_INTEREST->hi_option_flag |= HI_OPTION_SKIP_REG_SCAN;   //HI_OPTION_INIT_REG_SCAN;
    /* set ABI version that will be sent to host in the WMI_READY_EVENT
     * this allows us to avoid patching the code that sends the event. */
    HOST_INTEREST->hi_abi_version_override = HI_ABI_VER_OVERRIDE;
   _qcom_sys_fw_bridge_set();
    /* set up data set patch support
     * the dset patch will be loaded automatically by the NVRAM module. */
    _qcom_sys_reserved_ram_size_set();
#endif
    /* To skip BMI phase */
    _qcom_sys_bmi_skip();
}

void (*user_start_func) (void);

void
_qcom_app_start(void *unused)
{
    if (user_start_func) {
        user_start_func();
    }

    return;
}

extern int qcom_nvram_dset_reinit(void);
void
qcom_sys_start(void (*fn) (void), QCOM_SYS_START_PARA_t *pSysStartPara)
{

    if (!A_INIT()) {
        A_PRINTF("A_INIT() FAILED.\n");
        return;
    }

    /*
     * Calculate the pool size of the last arena
     * This should work for both 1.5MB and 1.0MB chips
     */
    #define TARG_DRAM_MAX (AR6K_RAM_START + TARG_RAM_SZ)
    HOST_INTEREST->hi_allocram_sz = TARG_DRAM_MAX - HOST_INTEREST->hi_allocram_start 
                                                  - HOST_INTEREST->hi_end_RAM_reserve_sz;
    /* subtract switchable IRAM banks */
    HOST_INTEREST->hi_allocram_sz -= ((HOST_INTEREST->hi_reserved_3 & 0x00FF0000) >> 16) * AR6K_SWITCHABLE_RAM_BANK_SZ;
    #undef TARG_DRAM_MAX
    
    {
        A_old_wdt_t old_wdt;

        /*
         * During debug, enable this section or set AR6K_OPTION_WDT_DISABLE;
         * otherwise, you may trigger a WatchDog Timer while at a breakpoint.
         */
        A_WDT_DISABLE(&old_wdt);
    }
	
    /*
   * restore the watchdog by setting assloop to zero while ASSERT happen.
   */
#ifndef FPGA /* NOT restore on fpga */
   assloop = 0;
#endif
    boot_time = cmnos_milliseconds();
    /*
	* System sleep is set in athos_init_part2.
	* Here we disable that.
	*/
    extern void cmnos_power_syssleep_control(int sleepOK);
    cmnos_power_syssleep_control(0);
//    extern void sys_power_control(int sleepOK);
//    sys_power_control(0);
      
    gNumOfWlanDevices = pSysStartPara->numOfVdev;
    gMccEnabled = pSysStartPara->isMccEnabled;

    _qcom_host_less_boot_config(0);
   

    _qcom_pre_start();
    _qcom_pre_patch();    

#if ((defined(AR6002_REV74) || defined(AR6002_REV75) || defined(AR6002_REV76)) && defined(P2P_ENABLED))
#ifdef CONFIG_P2P_FEATURE            
    extern void p2p_module_install(void);
    p2p_module_install();
#endif
#endif
    extern void wlan_main(void);
    wlan_main();
    _qcom_pre_init();

#if defined(AR6002_REV72)
    void patch_domain_install(void);
    patch_domain_install();
#endif    

	/*Disable SDIO interface in hostless mode. This will enable GPIO0~13 pin in GPIO mode if configured on Ruby. */
	A_GPIO_REG_WRITE(SDIO_ADDRESS, 0x0);
    /* Fix: because some OTP will use hosted mode in sdio value 
     * SDIO register will be mis-enabled and will cause concurrency uart tx bytes messy
     */
    nvram_configp->si_sdio_value = 0; 
    user_start_func = fn;
    ((struct rtos_s *) HOST_INTEREST->hi_rtos)->entry_host = _qcom_app_start;
    ((struct rtos_s *) HOST_INTEREST->hi_rtos)->malloc =QCOM_MEM_API_FN(_qcom_mem_alloc);
    ((struct rtos_s *) HOST_INTEREST->hi_rtos)->free = QCOM_MEM_API_FN(_qcom_mem_free);

    rtos_is_threadx();

    qcom_nvram_dset_reinit(); //for ota img, re-initialize nvram dset
    
#if defined(AR6002_REV72)
    extern void _tx_initialize_kernel_enter();
    _tx_initialize_kernel_enter();
#endif    
    return;
}

void
qcom_sys_reset()
{

    A_RTC_SOC_REG_WRITE(SOC_RESET_CONTROL_ADDRESS, SOC_RESET_CONTROL_COLD_RST_MASK);
    A_RTC_SOC_REG_READ(SOC_RESET_CONTROL_ADDRESS);

}


/*fn- qcom_watchdog_feed- API to allow application to reset the watchdog*/
void qcom_watchdog_feed()
{
    A_WDT_RESET(); 
}

#define WDT_EXPIRE_INTERVAL       (10) /* 10 seconds @ 32KHz WDT */
extern A_UINT8 wdt_usr_flag;
extern A_UINT32 cmnos_timer_wdt_timeout_secs;

/*fn- qcom_watchdog- API to control watchdog behavior.
  Possible states-
  APP_WDT_DISABLE- disable watchdog
  APP_WDT_ENABLE-  enable watchdog, it will fire at specified interval
  APP_WDT_USER_DEF- enable watchdog, set timeout, but disable Target's reset timer. Application should
  call qcom_watchdog_feed to reset the watchdog preiodically*/
int qcom_watchdog(int enable, int timeout)
{
    A_old_wdt_t old_wdt;

    switch(enable){
        case APP_WDT_DISABLE:
            A_WDT_DISABLE(&old_wdt);
            break;
        case APP_WDT_ENABLE:            
            /*Enable the watchdog and set the timeout*/
             /*fix IR-119288: generate interrupt when watchdog tiimeout*/
            A_WDT_ENABLE(A_WDT_CONTROL_INTR);
            extern void cmnos_wdt_isr_patch(void);
            cmnos_wdt_isr_patch();            
            //A_WDT_EXPIRE(timeout);
            cmnos_timer_wdt_timeout_secs = timeout;
			wdt_usr_flag = 0;
            break;

        case APP_WDT_USER_DEF:
            /*Enable the watchdog and set the timeout*/
			/*fix IR-119288: generate interrupt when watchdog tiimeout*/
            A_WDT_ENABLE(A_WDT_CONTROL_INTR);
            extern void cmnos_wdt_isr_patch(void);
            cmnos_wdt_isr_patch();            
            //A_WDT_EXPIRE(timeout);
            cmnos_timer_wdt_timeout_secs = timeout;  //let the value of user set valid
			wdt_usr_flag = 1;
            /*User wants to use application defined reset timeout, it will reset the watchdog from the application.
            */
            /* To-Do- Disable Internal WDT timer? */
            break;
        case APP_WDT_USER_DBG:
            /*Enable the watchdog and set the timeout*/
            A_WDT_ENABLE(A_WDT_CONTROL_INTR);
            extern void cmnos_wdt_isr_patch(void);
            cmnos_wdt_isr_patch();
            cmnos_timer_wdt_timeout_secs = timeout;  //let the value of user set valid
            wdt_usr_flag = 1;
            break;
			
        default:
            return -1;
    }
    return 0;
}


/*
 * This function is used to :
 * 1)initiate serial 
 * 2)enable or disable consol printf.
 * This function should be called at the beginning of user application.
 */
void qcom_enable_print(int enable)
{
    cdr_dbg_print(enable);
    return;
}

void
qcom_sys_set_current_devid(int devid)
{
    if(devid)
        HOST_INTEREST->hi_device_status_info |= HI_DEVICE_STATUS_CURRENT_DEVID;
    else
        HOST_INTEREST->hi_device_status_info &= ~HI_DEVICE_STATUS_CURRENT_DEVID;

}

void qcom_set_time(tRtcTime time)
{
    A_UINT32 date_regval = 0;
    A_UINT32 time_regval = 0;

    A_RTC_SOC_REG_WRITE(RTC_CONTROL_ADDRESS,0x00000000);//Disable RTC
    A_RTC_SOC_REG_WRITE(RTC_CONFIG_ADDRESS,0x00000000);

	if (time.wday == 0)
	{
		time.wday = 7; // Sun.
	}
	
    time_regval  = time.Sec  & RTC_SECS_MASK;
    time_regval |= ((time.min  << 8 ) & RTC_MIN_MASK);
    time_regval |= ((time.hour << 16 )& RTC_HOUR_MASK);
    time_regval |= ((time.wday << 24 )& RTC_WDAY_MASK);
    A_RTC_SOC_REG_WRITE(RTC_SET_TIME_ADDRESS,time_regval);

    date_regval  = ((time.yday ) & RTC_YDAY_MASK);
    date_regval |= (((time.mon + 1) << 8) & RTC_MON_MASK);
    date_regval |= (((time.year % 100) << 16) & RTC_YEAR_MASK);

    A_RTC_SOC_REG_WRITE(RTC_SET_DATE_ADDRESS,date_regval);
    A_RTC_SOC_REG_WRITE(RTC_CONTROL_ADDRESS,0x00000006);//Load and Enable RTC

    return;
}

void qcom_set_scan_timeout(A_UINT8 sec)
{
   if (sec == 0x0) {
      return;
   }
   scan_timeout = sec;
}

A_UINT32 _sem_flag_cleared = 1;
    
unsigned int qcom_tx_semaphore_put(void *semaphore_ptr)
{
	A_UINT32 _sem_send_ret = TX_SUCCESS;
	if (_sem_flag_cleared) {
		_sem_flag_cleared = 0;
		_sem_send_ret = tx_semaphore_put((TX_SEMAPHORE*)semaphore_ptr);
	}
	return _sem_send_ret;
}
unsigned int qcom_tx_semaphore_get(void *semaphore_ptr, unsigned int wait_option)
{
	A_old_intr_t old;
	
	A_INTR_DISABLE(&old);
	_sem_flag_cleared = 1;
	A_INTR_RESTORE(old);

	return tx_semaphore_get((TX_SEMAPHORE*)semaphore_ptr,wait_option);
}

#else
#error Unsupported platform!
#endif                          /* AR6002_REV7 */
