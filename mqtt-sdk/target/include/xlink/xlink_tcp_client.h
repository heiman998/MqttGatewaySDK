

#ifndef XLINK_TCP_CLIENT_H_
#define XLINK_TCP_CLIENT_H_
#ifdef  __cplusplus
extern "C" {
#endif
#include "xlink_data.h"
#include "xlink_message.h"
#include "xlink_tcp_type.h"

extern XLINK_FUNC void XlinkTcpInit(XLINK_TCP_WORK_TYPE type);
extern XLINK_FUNC void XlinkTcpPing_Clear(void);
extern XLINK_FUNC void XlinkTcpLoop(xsdk_time_t c_time);
extern XLINK_FUNC void XlinkTcpNextWork(void);

//xlink_tcp_send
extern XLINK_FUNC x_int32 XlinkTcpSendData(unsigned char* data, x_uint16 datalen);

//xlink_tcp_send_sync
extern XLINK_FUNC void xsdk_tcp_SendSync(unsigned char* data, x_uint16 datalen);


extern XLINK_FUNC  void xlink_tcp_send_upgrade(unsigned char RetCode,unsigned short Upver,unsigned short currentVer) ;

#ifdef  __cplusplus
}
#endif
#endif /* XLINK_TCP_CLIENT_H_ */
