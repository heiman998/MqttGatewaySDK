

#ifndef _HMLINK_SYSTEM_H_
#define _HMLINK_SYSTEM_H_
#ifdef  __cplusplus
extern "C" {
#endif

struct _sockaddr_in {
	//uip_ipaddr_t  addr;
	int addr;
	int socket;
	unsigned short sin_port;
	unsigned int sin_family;
	struct in_address sin_addr;
};

extern void hmlink_process_udp_data(unsigned char *data, unsigned int datalength, _sockaddr_in *addr);
extern int hmlink_tcp_PushData(unsigned char * data, short datalen)

#ifdef  __cplusplus
}
#endif
#endif /* _HMLINK_SYSTEM_H_ */
