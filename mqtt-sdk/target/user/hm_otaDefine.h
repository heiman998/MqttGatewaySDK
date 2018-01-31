#ifndef _HM_OTA_DEFINE_H
#define _HM_OTA_DEFINE_H
#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18	// Cancel
#define CTRLZ 		0x1A
#define XUSER_WAIT_COMMUNITE_ACK 0x80
#define XUSER_WAIT_END_ACK 	0x81
#define XUSER_START				0x82
#define XUSER_END				0x83
#define XUSER_RESTART_APP		0x84
#define XUSER_NONE_STA	0
#define USE_1K_XMODEM 0  /* 1 for use 1k_xmodem, 0 for xmodem */
#if (USE_1K_XMODEM)	 //for 1K xmodem protocol
#define XMODEM_DATA_SIZE 1024
#define XMODEM_HEAD 0x02

#else				//for Xmodem protocol
#define XMODEM_DATA_SIZE 128
#define XMODEM_HEAD 0x01

#endif

void userXmodemTransport(void);
void uartSendXmodemStartOTA(void);
void uartSendXmodemExApp(void);
#endif
