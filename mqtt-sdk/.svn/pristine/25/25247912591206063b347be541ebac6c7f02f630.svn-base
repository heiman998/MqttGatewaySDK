/** 
* @file 	qca_at_command_test.c	
* @brief	
* @author	
* @version	V0.1.0
* @date		2017.3.01
* @details	
* @copyright 
* @note		
*/

/* Includes ----------------------------------------------------------------*/
#include "ltm3210.h"
#include "string.h"
#include "qca_uart.h"
#include "qca_adc.h"
#include "user_version.h"
#include "user_ota.h"
#include "qca_calendar.h"

#include "qca_tcp_client.h"
#include "qca_udp_server.h"
#include "qca_tcp_server.h"
#include "qca_at_command.h"


#define PARAM_MAX					10
#define AT							"AT+"                                                                                                                                                                                                      
#define OK							"+OK\r\n"

#define RST							"RST"
#define MODE						"MODE"
#define CONNSTATE					"CONNSTATE"
#define REMOTESTATE					"REMOTESTATE"
#define SAPINFO						"SAPINFO"
#define STAINFO						"STAINFO"
#define SAPCHANNEL					"SAPCHANNEL"
#define REMOTEINFO					"REMOTEINFO"
#define REMOTESW					"REMOTESW"
#define LOCALPORT					"LOCALPORT"
#define UID							"UID"
#define QMAC						"QMAC"
#define VER							"VER"
#define SCAN						"SCAN"
#define FLASHTEST					"FLASHTEST"
#define HTTP						"HTTP"
#define GPIO						"GPIO"
#define HELP						"HELP"
#define QOTA						"QOTA"
#define ADC 						"ADC"
#define SAVEPOWER                   "SAVEPOWER"
#define SUSPEND   					"SUSPEND"
#define CALENDAR					"CALENDAR"

#define FORMAT_ERROR        "+ERR=1\r\n" //command format error\r\n"
#define NONE_COMMAND        "+ERR=2\r\n" //unknow this command\r\n"
#define PARAM_ERROR         "+ERR=3\r\n" //param error\r\n"

/** 
* @struct command_handle_type
* @brief 
*/
typedef struct {
	char *command;
	char *description;
	void(*execute)(int channel,int argc, char *argv[]);
}command_handle_type;

#define COMMAND_SIZE		sizeof(command_handle) / sizeof(command_handle_type)

char AT_buf[512];

char qca_at_send(data_channel_type ch, unsigned char *buf, int length)
{
	switch(ch){
	case UART_Channel:
		qca_uart_send(buf, length);	
		break;
	case TCP_Client_Channel:
		qca_tcp_client_send(buf, length);
		break;
	case TCP_Server_Channel:
		qca_tcp_server_send(buf, length);
		break;
	case UDP_Server_Channel:
		qca_udp_server_send(buf, length);
		break;
	}
}

/** 
* @struct help_list
* @brief 
*/
static const char* help_list[] = {
	"COMMAND FORMAT :",

	"(1) AT+[COMMAND]\\r\\n",

	"(2) AT+[COMMAND]=[PARAM],[...]\\r\\n",

	"RST : Reset system",

    "MODE=[STATION_MODE/SOFTAP_MODE/LTLINK_MODE/CONFIG_MODE/SNIFFER_MODE/AIRKISS_MODE] : Module startup mode",

	"QMAC : MAC address",

	"VER : Frimware version",

	"CONNSTATE : Check the state of connect to ap",

	"REMOTESTATE : Check the state of connect to remote server",

	"REMOTESW=[ON/OFF] : Tcp client on/off switch",

	"REMOTEINFO=[ip],[port] : set remote tcp server ip and port",

	"SAPINFO=[ssid],[password] : Soft ap info",

	"SAPCHANNEL=[channel] : SoftAP channel, 1-11 for US, 1-13 for CN"

    "STAINFO=[ssid],[password] : Station info",

    "LOCALPORT=[UDP/TCP],[port] : Local UDP server and TCP server port",

    "UID=[user_id] : User id",

	"SCAN: Scan the wifi around",

	"GPIO=[pin],[dir],[value] : Control GPIO, [dir] could be 0 or 1, 0 for output, 1 for input",

	"ADC=[channel] : ADC sample Voltage, [channel] could be ADC 0-7",

	"SAVEPOWER=[ON/OFF] : Turn on/off save power mode, the uart will could not recv data after savepower(tcp/udp work normal)",

	"SUSPEND=[time] : Turn on suspend mode and set suspend time, unit is millisecond",

	"QOTA=[url],[port] : Start local ota, url format: http://192.168.0.21/ltm3210.bin, [port] is not necessary(default 80)",

	"CALENDAR=[year],[month],[day],[hour],[minute],[second] : Set the system time"
};

/**
* @fn void qca_restart_handle(int channel,int argc, char *argv[])
* @brief system restart
* @param 
* @return 
*/
void qca_restart_handle(int channel,int argc3, char *argv[])
{
	A_PRINTF("System restart\r\n" OK);

	AT_PRINTF(channel, "NT:0012\r\n");
	qcom_sys_reset();
}

/**
* @fn void qca_mode_handle(int channel,int argc, char *argv[])
* @brief working mode
* @param 
* @return 
*/
void qca_mode_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		switch (pParam->work_mode)
		{
		case SOFTAP_MODE:
			AT_PRINTF(channel, "MODE: SOFTAP_MODE\r\n" OK);
			
			break;

		case STATION_MODE:
			AT_PRINTF(channel, "MODE: STATION_MODE\r\n" OK);
			break;
		case SNIFFER_MODE:
			AT_PRINTF(channel, "MODE: SNIFFER_MODE\r\n" OK);
			break;
		case AIRKISS_MODE:
			AT_PRINTF(channel, "MODE: AIRKISS_MODE\r\n" OK);
			break;
		case CONFIG_MODE:
			AT_PRINTF(channel, "MODE: CONFIG_MODE\r\n" OK);
			break;
		case LTLINK_MODE:
			AT_PRINTF(channel, "MODE: LTLINK_MODE\r\n" OK);
			break;
		default:
			AT_PRINTF(channel, "MODE: NULL_MODE\r\n" OK);
			break;
		
		}
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "STATION_MODE") == 0)
		{
			pParam->work_mode = STATION_MODE;
		}
		else if (strcmp(argv[1], "SOFTAP_MODE") == 0)
		{
			pParam->work_mode = SOFTAP_MODE;
		}
		else if (strcmp(argv[1], "SNIFFER_MODE") == 0)
		{
			pParam->work_mode = SNIFFER_MODE;
		}
		else if (strcmp(argv[1], "AIRKISS_MODE") == 0)
		{
			pParam->work_mode = AIRKISS_MODE;
		}
		else if (strcmp(argv[1], "CONFIG_MODE") == 0)
		{
			pParam->work_mode = CONFIG_MODE;
		}
		else if (strcmp(argv[1], "LTLINK_MODE") == 0)
		{
			pParam->work_mode = LTLINK_MODE;
		}
		else
		{
			return;
		}
		qca_save_flash(pParam);

		AT_PRINTF(channel, OK);
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_connstate_handle(int channel,int argc, char *argv[])
* @brief the sate of connect router
* @param 
* @return 
*/
void qca_connstate_handle(int channel,int argc, char *argv[])
{
	if (argc == 1)
	{
		if (tGlobal_param.connect_state)
		{
			AT_PRINTF(channel, "CONNSTATE: SUCCESS\r\n" OK);
		}
		else
		{
			AT_PRINTF(channel, "CONNSTATE: FAIL\r\n" OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_remotestate_handle(int channel,int argc, char *argv[])
* @brief the state of connect server
* @param 
* @return 
*/
void qca_remotestate_handle(int channel,int argc, char *argv[])
{
	if (argc == 1)
	{
		if (tGlobal_param.remote_state)
		{
			AT_PRINTF(channel, "REMOTESTATE: SUCCESS\r\n" OK);
		}
		else
		{
			AT_PRINTF(channel, "REMOTESTATE: FAIL\r\n" OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_sapinfo_handle(int channel,int argc, char *argv[])
* @brief softap info
* @param 
* @return 
*/
void qca_sapinfo_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		AT_PRINTF(channel, "SSID: %s\r\nPWD: %s\r\n" OK, pParam->sap_ssid, pParam->sap_password);
	}
	else if (argc < 4)
	{
		if (strcmp(argv[1],"?") == 0)
		{
			AT_PRINTF(channel, "SSID: %s\r\nPWD: %s\r\n" OK, pParam->sap_ssid, pParam->sap_password);
		}
		else  if (argc == 2)
		{
			strcpy(pParam->sap_ssid, argv[1]);

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
		else if (argc == 3)
		{
			strcpy(pParam->sap_ssid, argv[1]);
			strcpy(pParam->sap_password, argv[2]);

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}
#ifdef SAPCHANNEL
void qca_sapchannel_handle(int channel, int argc, char *argv[])
{
	t_save_param_type *pParam = &t_save_param;
	unsigned char sap_channel;

	if (argc == 1)
	{
		AT_PRINTF(channel, SAPCHANNEL ": %d\r\n" OK, pParam->sap_channel);
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "?") == 0)
		{
			AT_PRINTF(channel, SAPCHANNEL ": %d\r\n" OK, pParam->sap_channel);
		}
		else
		{
			sap_channel = atoi(argv[1]);
			if ((sap_channel > 0) && (sap_channel < 14))
			{
				pParam->sap_channel = sap_channel;

				qca_save_flash(pParam);
				AT_PRINTF(channel, "SAPCHANNEL: SUCCESS\r\n" OK);
			}
		}
	}
	else
	{
		AT_PRINTF(channel, FORMAT_ERROR);
	}
}
#endif
/**
* @fn void qca_stainfo_handle(int channel,int argc, char *argv[])
* @brief station info
* @param 
* @return 
*/
void qca_stainfo_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		AT_PRINTF(channel, "SSID: %s\r\nPWD: %s\r\n", pParam->sta_ssid, pParam->sta_password);

	}
	else if (argc < 4)
	{
		if (strcmp(argv[1],"?") == 0)
		{
			AT_PRINTF(channel, "SSID: %s\r\nPWD: %s\r\n", pParam->sta_ssid, pParam->sta_password);
		}
		else  if (argc == 2)
		{
			strcpy(pParam->sta_ssid, argv[1]);
			memset(pParam->sta_password, 0, sizeof(pParam->sta_password));

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
		else if (argc == 3)
		{
			strcpy(pParam->sta_ssid, argv[1]);
			strcpy(pParam->sta_password, argv[2]);

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_remoteinfo_handle(int channel,int argc, char *argv[])
* @brief remote server info
* @param 
* @return 
*/
void qca_remoteinfo_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{	
		AT_PRINTF(channel, "REMOTE IP: %d.%d.%d.%d\r\n" "REMOTE PORT: %d\r\n", 
			pParam->tcp_client.ip & 0xFF, 
			(pParam->tcp_client.ip >> 8) & 0xFF,
			(pParam->tcp_client.ip >> 16) & 0xFF,
			(pParam->tcp_client.ip >> 24) & 0xFF,
			pParam->tcp_client.port);
	}
	else if (argc < 4)
	{
		if (strcmp(argv[1],"?") == 0)
		{
			AT_PRINTF(channel, "REMOTE IP: %d.%d.%d.%d\r\n" "REMOTE PORT: %d\r\n", 
				pParam->tcp_client.ip & 0xFF, 
				(pParam->tcp_client.ip >> 8) & 0xFF,
				(pParam->tcp_client.ip >> 16) & 0xFF,
				(pParam->tcp_client.ip >> 24) & 0xFF,
				pParam->tcp_client.port);
		}
		else if (argc == 3)
		{
			pParam->tcp_client.ip = _inet_addr(argv[1]);
			pParam->tcp_client.port = atoi(argv[2]);

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_remotesw_handle(int channel,int argc, char *argv[])
* @brief connect remote server switch
* @param 
* @return 
*/
void qca_remotesw_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;;

	if (argc == 1)
	{
		if (pParam->remotesw)
		{
			AT_PRINTF(channel, REMOTESW ": ON\r\n" OK);
		}
		else
		{
			AT_PRINTF(channel, REMOTESW ": OFF\r\n" OK);
		}
	}
	else if ((argc == 2) || (argc == 3))
	{
		if (strcmp(argv[1], "ON") == 0)
		{
			if ((pParam->tcp_client.ip == 0) || (pParam->tcp_client.port == 0))
			{
				AT_PRINTF(channel, "Please set remote info first.\n");
			}
			else
			{
				pParam->remotesw = 1;
				qca_save_flash(pParam);
				AT_PRINTF(channel, OK);
			}		
		}
		else if (strcmp(argv[1], "OFF") == 0)
		{
			pParam->remotesw = 0;
			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
		else if (strcmp(argv[1], "?") == 0)
		{
			if (pParam->remotesw)
			{
				AT_PRINTF(channel, REMOTESW ": ON\r\n" OK);
			}
			else
			{
				AT_PRINTF(channel, REMOTESW ": OFF\r\n" OK);
			}
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}


/**
* @fn void qca_localport_handle(int channel,int argc, char *argv[])
* @brief local port
* @param 
* @return 
*/
void qca_localport_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		AT_PRINTF(channel, "UDP PORT: %d\r\nTCP PORT: %d\r\n" OK, pParam->udp_server.port, pParam->tcp_server.port);
	}
	else if (argc < 4)
	{
		if ((strcmp(argv[1],"?") == 0) && (argc == 2 ))
		{
			AT_PRINTF(channel, "UDP PORT: %d\r\nTCP PORT: %d\r\n" OK, pParam->udp_server.port, pParam->tcp_server.port);
		}
		else if (argc == 3)
		{
			if (strcmp(argv[1], "UDP") == 0)
			{
				pParam->udp_server.port = atoi(argv[2]);
			}
			else if (strcmp(argv[1], "TCP") == 0)
			{
				pParam->tcp_server.port = atoi(argv[2]);
			}
			else
			{
				return ;
			}

			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_uid_handle(int channel,int argc, char *argv[])
* @brief user uid
* @param 
* @return 
*/
void qca_uid_handle(int channel,int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		AT_PRINTF(channel, "UID: %s\r\n", pParam->uid);
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1],"?") == 0)
		{
			AT_PRINTF(channel, "UID: %s\r\n" OK, pParam->uid);
		}
		else
		{
			strcpy(pParam->uid, argv[1]);
			qca_save_flash(pParam);
			AT_PRINTF(channel, OK);
		}
	}
	else
	{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}

/**
* @fn void qca_qmac_handle(int channel,int argc, char *argv[])
* @brief mac address
* @param 
* @return 
*/
void qca_qmac_handle(int channel,int argc, char *argv[])
{
	unsigned char	macAddr[6];

	qcom_mac_get(DEVICE_ID, macAddr);

	AT_PRINTF(channel, "MAC: %02x:%02x:%02x:%02x:%02x:%02x\r\n" OK,
		macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);

}

/**
* @fn void qca_ver_handle(int channel,int argc, char *argv[])
* @brief FW version
* @param 
* @return 
*/
void qca_ver_handle(int channel,int argc, char *argv[])
{
	static char date[20];
	static char time[20];
	static char ver[20];
	static char cl[20];
	static char lib_ver[20];

// 	memset(date, 0, 20);
// 	memset(time, 0, 20);
	memset(ver, 0, 20);
//	memset(cl, 0, 20);
	qcom_firmware_version_get(date, time, ver, cl);

	qca_get_lib_version(lib_ver);

	A_PRINTF("Host version			: Hostless\n");
	A_PRINTF("Target version		: QCM\n");
	A_PRINTF("Firmware version		: %s\n", ver);
	A_PRINTF("Firmware changelist	: %s\n", cl);
	A_PRINTF("Interface version	    : EBS\n");
	A_PRINTF(" - built on %s %s\n", date, time);
	A_PRINTF("lt lib version		: %s\n", lib_ver);
	A_PRINTF("Demo version          : V"VERSION_FMT"\n", VERSION(USER_VERSION_SOFTWARE));

	AT_PRINTF(channel, "LIB VER: %s\n", lib_ver)
	AT_PRINTF(channel, "FW  VER: V"VERSION_FMT"\n"OK, VERSION(USER_VERSION_SOFTWARE));
}

/**
* @fn void qca_scan_handle(int channel,int argc, char *argv[])
* @brief scan test
* @param 
* @return 
*/
void qca_scan_handle(int channel,int argc, char *argv[])
{
	QCOM_BSS_SCAN_INFO* ptscan_info;
	int		counts;
	int		i;

	if (argc <= 2)
	{
		counts = qca_scan_ap(&ptscan_info);

		for (i = 0; i < counts; i++)
		{
			if (argc == 2)
			{
				if (strcmp(argv[1], "?") != 0)
				{
					if (strcmp(argv[1], (char *)ptscan_info[i].ssid) == 0)
					{
						AT_PRINTF(channel, "SSID:%s, BSSID:%02x-%02x-%02x-%02x-%02x-%02x, CHANNEL:%d, RSSI:%d\r\n",
							ptscan_info[i].ssid,
							ptscan_info[i].bssid[0], ptscan_info[i].bssid[1], ptscan_info[i].bssid[2], ptscan_info[i].bssid[3], ptscan_info[i].bssid[4], ptscan_info[i].bssid[5],
							ptscan_info[i].channel,
							ptscan_info[i].rssi);
					}
					continue;
				}
			}

			AT_PRINTF(channel, "%02d, SSID:%s, BSSID:%02x-%02x-%02x-%02x-%02x-%02x, CHANNEL:%d, RSSI:%d\r\n",
				i,
				ptscan_info[i].ssid,
				ptscan_info[i].bssid[0], ptscan_info[i].bssid[1], ptscan_info[i].bssid[2], ptscan_info[i].bssid[3], ptscan_info[i].bssid[4], ptscan_info[i].bssid[5],
				ptscan_info[i].channel,
				ptscan_info[i].rssi);
//			qcom_thread_msleep(10);
		}

		AT_PRINTF(channel, OK);
	}

}

/**
* @fn void qca_flash_test(int channel,int argc, char *argv[])
* @brief flash test
* @param 
* @return 
*/
void qca_flash_test(int channel,int argc, char *argv[])
{
//	qca_flash_param_test();
}

/**
* @fn void qca_httpc_handle(int channel,int argc, char *argv[])
* @brief http client test
* @param 
* @return 
*/
void qca_httpc_handle(int channel,int argc, char *argv[])
{
//	qca_http_client_test();
}

/**
* @fn void qca_gpio_handle(int channel,int argc, char *argv[])
* @brief GPIO control
* @param 
* @return 
*/
void qca_gpio_handle(int channel,int argc, char *argv[])
{
	char pin;
	char mode;
	char value;

	if (argc == 3)
	{
		pin = atoi(argv[1]);
		mode = atoi(argv[2]);
		
		if (mode == 1)
		{
			qca_gpio_set_pin_dir(pin, 1);
			value = qca_gpio_get_pin_status(pin);

			AT_PRINTF(channel, "GPIO%d get value:%d\n", pin, value);
		}

	}
	else if (argc == 4)
	{
		pin = atoi(argv[1]);
		mode = atoi(argv[2]);
		value = atoi(argv[3]);

		if (mode == 0)
		{
			qca_gpio_set_configuration(pin, 0x80000000);
			qca_gpio_set_pin_dir(pin, 0);
			qca_gpio_set_pin_status(pin, value);

			AT_PRINTF(channel, "GPIO%d set value:%d\n", pin, value);
		}
	}
}

/**
* @fn void qca_help_handle(int channel,int argc, char *argv[])
* @brief help list print
* @param 
* @return 
*/
void qca_help_handle(int channel,int argc, char *argv[])
{
	int i;
	for (i = 0; i < sizeof(help_list) / sizeof(*help_list); i++)
	{
		AT_PRINTF(channel, "%s\r\n\r\n", help_list[i]);
	}

}

#ifdef ADC
void qca_adc_handle(int channel, int argc, char *argv[])
{
	int adc_ch;
	unsigned int value;
	float result = 0.0;

	if (argc == 2)
	{
		adc_ch = atoi(argv[1]);
		
		qca_adc_init(adc_ch);
		value = qca_adc_read(adc_ch);
		qca_adc_close();

		
		value = (value - 980);
		result = value * 3.3;
		result = result / 2000;

		value = result * 1000;

		AT_PRINTF(channel, "%d.%d\r\n"OK, value / 1000, value % 1000);
	}
	else
	{
		
	}
}
#endif
#ifdef SAVEPOWER
void qca_savepower_handle(int channel, int argc, char *argv[])
{
	t_save_param_type	*pParam = &t_save_param;

	if (argc == 1)
	{
		AT_PRINTF(channel, "SAVEPOWER:%s\r\n"OK, (pParam->savepower ? "ON":"OFF"));
	}else if (argc == 2){
		if(strcmp(argv[1],"?") == 0){
			AT_PRINTF(channel, "SAVEPOWER:%s\r\n"OK, (pParam->savepower ? "ON":"OFF"));	
		}else if(strcmp(argv[1],"ON") == 0){
			if(pParam->savepower == 0){
				qcom_set_power_mode(DEVICE_ID, REC_POWER);
        		cmnos_power_syssleep_control(1);
			}
			pParam->savepower = 1;

			qca_save_flash(pParam);
		}else if(strcmp(argv[1],"OFF") == 0){
			if(pParam->savepower == 1){
				qcom_set_power_mode(DEVICE_ID, MAX_PERF_POWER);
				cmnos_power_syssleep_control(0);
			}
			pParam->savepower = 0;

			qca_save_flash(pParam);			
		}
	}

}
#endif
#ifdef SUSPEND
void qca_suspend_handle(int channel, int argc, char *argv[])
{
	int times = 0;

	if(argc == 2){
		times = atoi(argv[1]);

		AT_PRINTF(channel, OK);
		qcom_suspend_enable(1);

		qcom_nvram_init();
		qcom_nvram_fini();

		qcom_suspend_start(times);

	}else{
		AT_PRINTF(channel, PARAM_ERROR);
	}
}
#endif
#ifdef QOTA
int user_ota_cb(user_ota_result_t result)
{
	if(result == USER_OTA_RESULT_SUCCEED){
		AT_PRINTF(UART_Channel, "NT:0022\r\n");

		qcom_sys_reset();
	}else{
		AT_PRINTF(UART_Channel, "NT:0021\r\n");		
	}

	return 0;
}
void qca_ota_handle(int channel, int argc, char *argv[])
{
	int ret;
	unsigned short port;

	if ((argc == 2) || (argc == 3))
	{
		if((argc == 2)){
			port = 80;
		}else{
			port = atoi(argv[2]);
		}

		AT_PRINTF(channel, "NT:0020\r\n");
		ret = user_ota_start(argv[1], port, user_ota_cb);
	}else{
		AT_PRINTF(channel, FORMAT_ERROR);
	}
}
#endif
void qca_calendar_handle(int channel,int argc, char *argv[])
{
	T_SYSTEM_TIME		cale_time;

	if (argc == 1)
	{
		qca_calendar_get_time(&cale_time);

		AT_PRINTF(channel, "%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
		AT_PRINTF(channel, "%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1],"?") == 0)
		{
			qca_calendar_get_time(&cale_time);

			AT_PRINTF(channel, "%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
			AT_PRINTF(channel, "%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);
		}	
	}
	else if (argc == 7)
	{

		cale_time.year		=  atoi(argv[1]);	// year
		cale_time.month		=  atoi(argv[2]);	// month
		cale_time.day		=  atoi(argv[3]);	// day
		cale_time.hour		=  atoi(argv[4]);	// hour
		cale_time.minute	=  atoi(argv[5]);	// minute
		cale_time.second	=  atoi(argv[6]);	// second

		qca_calendar_init_time(&cale_time);

		qca_calendar_get_time(&cale_time);
		AT_PRINTF(channel, "%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
		AT_PRINTF(channel, "%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);
	}
}
/** 
* @struct command handle
* @brief 
*/
const command_handle_type command_handle[]= {
	{ 
		.command = RST,
		.description = "restart system.",
		.execute = qca_restart_handle,
	},

	{ 
        .command = MODE,
        .description = "check/set system wifi mode.",
        .execute = qca_mode_handle,
    },

    {
        .command = CONNSTATE,
        .description = "connect state.",
        .execute = qca_connstate_handle,
    },

    {
        .command = REMOTESTATE,
        .description = "connect to remote server state",
        .execute = qca_remotestate_handle,
    },

    {
        .command = SAPINFO,
        .description = "check/set Soft-Ap info.",
        .execute = qca_sapinfo_handle,
    },
#ifdef SAPCHANNEL
	{
        .command = SAPCHANNEL,
        .description = "check/set Soft-Ap channel.",
        .execute = qca_sapchannel_handle,
    },
#endif
    {
        .command = STAINFO,
        .description = "check/set Station info",
        .execute = qca_stainfo_handle,
    },

    {
        .command = REMOTEINFO,
        .description = "set the remote tcp server.",
        .execute = qca_remoteinfo_handle,
    },

    {
        .command = REMOTESW,
        .description = "remote switch",
        .execute = qca_remotesw_handle,
    },

    {
        .command = LOCALPORT,
        .description = "check the local tcp server and udp server port.",
        .execute = qca_localport_handle,
    },

    {
        .command = UID,
        .description = "check the unique identification.",
        .execute = qca_uid_handle,
    },

    {
        .command = QMAC,
        .description = "get the MAC address.",
        .execute = qca_qmac_handle,
    },

    {
        .command = VER,
        .description = "get the version.",
        .execute = qca_ver_handle,
    },

    {
        .command = SCAN,
        .description = "scan the wifi around",
        .execute = qca_scan_handle,
    },
	
//     {
//         .command = FACTORYSETTINGS,
//         .description = "restore factory settings",
//         .execute = qca_factory_settings_handle,
//     },

	{
        .command = GPIO,
        .description = "GPIO",
        .execute = qca_gpio_handle,
    },

	{
        .command = HELP,
        .description = "HELP",
        .execute = qca_help_handle,
    },
#ifdef QOTA
	{
        .command = QOTA,
        .description = "QOTA",
        .execute = qca_ota_handle,
    },
#endif
#ifdef ADC	
	{
        .command = ADC,
        .description = "ADC",
        .execute = qca_adc_handle,
    },
#endif
#ifdef SAVEPOWER	
	{
        .command = SAVEPOWER,
        .description = "SAVEPOWER",
        .execute = qca_savepower_handle,
    },
#endif
#ifdef SUSPEND
	{
        .command = SUSPEND,
        .description = "SUSPEND",
        .execute = qca_suspend_handle,
    },
#endif
#ifdef CALENDAR
	{
        .command = CALENDAR,
        .description = "CALENDAR",
        .execute = qca_calendar_handle,
    },
#endif
};


/**
* @fn int qca_handle_cmd(int channel, unsigned char *pdata)
* @brief resolve data
* @param 
* @return 
*/
int qca_handle_cmd(int channel, unsigned char *pdata, int len)
{
	int					i, j;
	char				*ptemp1, *ptemp2;
	char				*argv[PARAM_MAX] = {NULL};
	int					argc = 0;
	char				*cmd = (char *)pdata;

	while (1)
	{
		cmd = strstr(cmd, AT);
		if (cmd != NULL)
		{
			cmd += 3;

			ptemp1 = strchr(cmd, '\r');
			ptemp2 = strchr(cmd, '\n');

			if(ptemp1 != NULL)
			{
				*ptemp1 = '\0';
			}
			else if (ptemp2 != NULL)
			{
				*ptemp2 = '\0';
			}
			else
			{
				AT_PRINTF(channel, "command err 1.\n");
				break;
			}

			// escape special characters such as "=" ","
			ptemp1 = cmd;
			ptemp2 = cmd;
			while (*ptemp1 != '\0')
			{
				if (*ptemp1++ == '\\')
				{
					*ptemp1 -= 1;
				}
			}
			
			argv[argc] = strtok_ext(cmd, "=\r\n");
			if (argv[argc] == NULL)
			{
				AT_PRINTF(channel, "command err 2.\n");
				break;
			}
		
			i = 0;
			while (i < COMMAND_SIZE)
			{
				if (strcmp(cmd, command_handle[i].command) == 0)
				{
					A_PRINTF("at cmd:%s\n", cmd);

					while (1)
					{
						argc++;

						argv[argc] = strtok_ext(NULL, ",");
						if (argv[argc] == NULL)
						{
							break;
						}

						if (argc >= PARAM_MAX)
						{
							AT_PRINTF(channel, "command err.3\n");
							return -1;
						}
					}

					for (j=0; j<argc; j++)
					{
						ptemp1 = argv[j];

						while (*ptemp1 != '\0')
						{
							if (*ptemp1++ == '\\')
							{
								*ptemp1 += 1;

								memmove(ptemp1-1, ptemp1, strlen(ptemp1)+1);
							}
						}
					}

					command_handle[i].execute(channel, argc, argv);
					break;

				}

				i++;
			}

			if (i>= COMMAND_SIZE)
			{
				AT_PRINTF(channel, "not at command\n");
			}
		}
		else
		{
			break;
		}

		cmd = ptemp2 +1;
		return AT_COMMAND;
	}

	return NONE_AT_COMMAND;
}




/***************** (C) COPYRIGHT 2016 longsys *****END OF FILE***************/
