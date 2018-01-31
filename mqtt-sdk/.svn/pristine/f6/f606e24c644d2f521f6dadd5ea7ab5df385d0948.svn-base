/*
 * xsdk_config.h
 *
 *  Created on: 2015骞�8鏈�2鏃�
 *      Author: XT800
 */

#ifndef XSDK_CONFIG_H_
#define XSDK_CONFIG_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "Xlink_Head_Adaptation.h"

#define XLINK_CONFIG_BUFFER_SIZE__ 180
#define XLINK_DOMAIN_IP_MAX_LEN  72

extern XLINK_FUNC int xsdk_config_init(char *proID,char *proKey,unsigned char *mac);
extern XLINK_FUNC int xsdk_write_config(unsigned int index, char *p_data, unsigned int p_datalen);
extern XLINK_FUNC int xsdk_read_config(unsigned int index, char *p_Buffer, unsigned int p_datalen);
extern XLINK_FUNC int xsdk_config_save(void);
extern XLINK_FUNC int * xlink_get_share_key_ptr(void);
extern XLINK_FUNC void XlinkResetSDK(void);
//code size:16bytes
extern XLINK_FUNC void XlinkGetAuthCode(char * code);

typedef struct XLINK_TIMER{
	xsdk_time_t  time;
	unsigned int outtime;
}xlink_timer;

extern XLINK_FUNC void xsdk_timer_set(xlink_timer *timer,unsigned int outtime);
extern XLINK_FUNC int xsdk_tiemr_timerout(xlink_timer *timer);

extern XLINK_FUNC void XlinkSetACK(int accesskey);
extern XLINK_FUNC int XlinkGetACK(void);

//#define TICKS_DIFF(cur, prev) ((cur) >= (prev)) ? ((cur)-(prev)) : ((0xFFFFFFFF-(prev))+(cur)+1)
//extern XLINK_FUNC int xlink_ticks_diff(xsdk_time_t current,xsdk_time_t prev);

#define __XLINK_LOCAL_PORT__ 5987

#define XSDK_DEBUG_DEBUG(info,y...)  do{ if (g_xlink_user_config->DebugPrintf) {g_xlink_user_config->DebugPrintf("\r\nXSDK debug =>fun(%s)line:%04d:="info,__FUNCTION__,__LINE__,##y);}}while(0)
#define XSDK_DEBUG_WARING(info,y...) do{ if (g_xlink_user_config->DebugPrintf) {g_xlink_user_config->DebugPrintf("\r\nXSDK waring=>fun(%s)line %04d:="info,__FUNCTION__,__LINE__,##y);}}while(0)
#define XSDK_DEBUG_ERROR(info,y...)  do{ if (g_xlink_user_config->DebugPrintf) { g_xlink_user_config->DebugPrintf("\r\nXSDK error =>fun(%s)line %04d:="info,__FUNCTION__,__LINE__,##y);}}while(0)

#ifdef  __cplusplus
}
#endif
#endif /* XSDK_CONFIG_H_ */
