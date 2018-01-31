

#ifndef XLINK_SYSTEM_H_
#define XLINK_SYSTEM_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "Xlink_Head_Adaptation.h"
#include "xlink_message.h"
#if __ALL_DEVICE__
    enum TCP_ {
        E_TCP_SUCCESS, E_TCP_CONTINUE, //
        E_TCP_HEAD_ERROR, //
        E_TCP_NO_MEM,
        E_TCP_READ_MEM_ERROR,
    };
#endif

extern XLINK_FUNC char *XlinkSystemVersion(void);

extern XLINK_FUNC x_bool XlinkSystemInit(char * product_id, char * product_key, XLINK_USER_CONFIG *config);

extern XLINK_FUNC void XlinkSystemSetWifiStatus(unsigned char status);

extern XLINK_FUNC void XlinkSystemLoop(xsdk_time_t c_time, x_int32 timeout_ms);

extern XLINK_FUNC void XlinkSystemSetLocalScanStatus(x_bool status);

extern XLINK_FUNC void XlinkSystemSetPassword(char *password);

extern XLINK_FUNC void XlinkSystemClearPassword(char *password);

extern XLINK_FUNC void XlinkSystemSetDeviceName(char *NameStr);
extern XLINK_FUNC void XlinkSystemSetDomain(const char *domain);

extern XLINK_FUNC void XlinkSystemReSet(void);

extern XLINK_FUNC void XlinkNetInit(void);

/*
 * 说明：发送透传数据到指定APP
 *参数：
 *     * data:数据指针
 *     datalen:数据长度
 *     to_id:APP ID
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendTcpPipe(const unsigned char * data,const unsigned int datalen, x_uint32 to_id);
/*
 * 说明：发送透传数据到指定APP，有回调响应
 *参数：
 *     handle:包句柄，用于对应回调函数对应响应
 *     * data:数据指针
 *     datalen:数据长度
 *     to_id:APP ID
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendTcpPipe_cb(unsigned short handle,const unsigned char * data, const unsigned int datalen, x_uint32 to_id);

/*
 * 说明：发送广播透传数据到APP
 *参数：
 *     * data:数据指针
 *     datalen:数据长度
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendTcpPipe2(const unsigned char * data,const unsigned int datalen);
/*
 * 说明：发送广播透传数据到APP，有回调响应
 *参数：
 *     handle:包句柄，用于对应回调函数对应响应
 *     * data:数据指针
 *     datalen:数据长度
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendTcpPipe2_cb(unsigned short handle,const unsigned char * data,const unsigned int datalen);
/*
 * 说明：发送透传数据到所有APP
 *参数：
 *     * data:数据指针
 *     datalen:数据长度
 *     to_id:APP ID
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendUdpPipe(const unsigned char *data, const unsigned int datalen, xlink_addr *fromAddr);
/*
 * 说明：发送透传数据到所有APP，有回调响应
 *参数：
 *     handle:包句柄，用于对应回调函数对应响应
 *     * data:数据指针
 *     datalen:数据长度
 *     to_id:APP ID
 *     return:大于0表示成功，小于等于0表示失败
*/
extern XLINK_FUNC x_int32 XlinkSendUdpPipe_cb(unsigned short handle,const unsigned char *data, const unsigned int datalen, xlink_addr *fromAddr);

extern XLINK_FUNC void XlinkGetServerTime(void);
extern XLINK_FUNC int XlinkGetSystemTime(XLINK_SYS_TIME *pTime);

extern XLINK_FUNC void XlinkSystemInitEvent(XLINK_SELECT *event);

extern XLINK_FUNC int XlinkSystemTcpLoop(void);
extern XLINK_FUNC int xsdk_system_getMacStr(char *RetMacBuffer, const int bufflen);

extern XLINK_FUNC int XlinkProTestSendData(const char *data,const unsigned char datalen,const unsigned char data_id,xlink_addr *toAddr);

extern XLINK_FUNC int XlinkProTestSendCloseRequest(xlink_addr *toAddr);

extern XLINK_FUNC int XlinkSystemProtesting(void);
extern XLINK_FUNC int XlinkSystemGetDeviceID(void);

extern XLINK_FUNC void Xlink_UpdateDataPoint(unsigned char* data, x_uint16 datalen,unsigned char flag);
extern XLINK_FUNC int Xlink_UpdateDataPoint_udp(unsigned short handle,unsigned char* data, x_uint16 datalen,unsigned char flag);
extern XLINK_FUNC int Xlink_UpdateDataPoint_tcp(unsigned short handle,unsigned char* data, x_uint16 datalen,unsigned char flag);

extern XLINK_FUNC void XlinkSetACK(int accesskey);
extern XLINK_FUNC int XlinkGetACK(void);

extern XLINK_FUNC  void xlink_tcp_send_upgrade(unsigned char RetCode,unsigned short Upver,unsigned short currentVer) ;

extern XLINK_FUNC void Xlink_Close_Net(void);

extern XLINK_FUNC int XlinkProcessTCPData(void);
extern XLINK_FUNC int XlinkPushData(unsigned char * data, x_int16 datalen);
extern XLINK_FUNC void XlinkProcessUdpData( unsigned char * Buffer, unsigned int BufferLen, xlink_addr *addr);

#define XLINK_USE_DEFAUT_PORT  0
extern int XlinkProcessUdpScan_V3_DefaultPort(xlink_Message * header,unsigned char * Buffer, unsigned int BufferLen,int socket, xlink_addr * addrBro,int addrBrolen);
//code size:16bytes
extern XLINK_FUNC void XlinkGetAuthCode(char * code);
#if __ALL_DEVICE__
extern void XLINK_FUNC setServerStatus_(unsigned char stat,unsigned char is80Port);
extern XLINK_FUNC void XlinkInitData(void);
#endif
#ifdef  __cplusplus
}
#endif
#endif /* XLINK_SYSTEM_H_ */
