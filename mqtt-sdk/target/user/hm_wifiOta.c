#include "qcom_common.h"
#include "qca_json.h"
#include "qca_ota.h"
#include "qca_http_client.h"
#include "qca_task_manage.h"
#include "hm_app.h"
#include "hm_otaDefine.h"
#include "hm_wifiOta.h"
#include "hm_zigbeeStateDeal.h"
#include "hm_net_data_manage.h"
//#define OTA_URL "http://192.168.4.109/ltm3210_user_ota.bin"
#define OTA_URL "http://hmqq.free.ngrok.cc/firmwareFile/ltm3210_user_ota.bin"//http://192.158.1.136/firmwareFile/ltm3210_user_ota.bin"//"http://static.heimantech.com/7dcbfd4df7b238a7247d5cdc6d0bd224"
#define OTA_SERVER_PORT 8555//80//80//6001//80//80


static char tmp_buf[32];
unsigned int tmp_len = 0;
int flag = 0;
extern GWSTATFORTIME gwStateForRealTime;
extern uint8_t devOtaUpdateingFlag;
extern SUBOTARAM subOtaRam;
extern DEV_OTA_STATION otaStationInfo;
extern DEV_MYSELF_INFO hmIotsDevInfo;
static char subTmp_buf[32];
unsigned int subTmp_len = 0;
int subFlag = 0;
uint16_t OTA_HTTPSERVER_PORT=8555;//80;




static int userStartSubOta(void)
{
	int ret = 0;
	if(gwStateForRealTime.gwDevIsOta == 0)
	{
		subOtaRam.flashAddr = START_ADDR_FLASH_SUB_FIRMWARE_ADDR;
		qcom_sec_md5_init();
	}else{
		ret = -1;
	}
	return ret;
}
    

static int userWriteSubFirmwareToFlash(uint8_t *data,uint32_t len)
{
	int ret = 0;
	if(data == NULL)
	{
		printf("in write sub firmware: in put data point was null\r\n");
		return -1;
	}
	ret = qca_save_flash_params(subOtaRam.flashAddr,(char*)data,len);
	subOtaRam.flashAddr += len;
	qcom_sec_md5_update(data,len);
	return ret;
}

static int userFinishSubFirmwareOta(void)
{
	uint8_t md5Result[16];
	char md5String[40];
	uint8_t i;
	memset(md5String,'\0',40);
	qcom_sec_md5_final(md5Result);
	for(i = 0; i < 16; i ++)
	{
		sprintf(md5String,"%s%02x",md5String,md5Result[i]);
	}
	printf("check the md5:%s\r\n",md5String);
	if(memcmp(md5String, subOtaRam.subFileMd5, 32) == 0){
		printf("file check md5 succeed\n");
		otaStationInfo.firmwareValid = 1;
		printf("file check md5 otaStationInfo.firmwareValid:%d\n",otaStationInfo.firmwareValid);
		otaStationInfo.head = 0xaa;
		otaStationInfo.end = 0x55;
		otaStationInfo.devImageSize = subOtaRam.sufFileSize;
		otaStationInfo.devImageVersion = subOtaRam.subFileVersion;
		otaStationInfo.flashSaveImageType = subOtaRam.subFileType;
		qca_save_flash_params(START_ADDR_FLASH_SUB_FIRMWARE_INFO_ADDR,(char*)&otaStationInfo,sizeof(otaStationInfo));
		return 0;
	}else{
		printf("file check md5 failed\n");
		return -1;
	}
}


/*******************************************************************
flash固件与实际运行固件比对
**********************************************************************/
void userCheckUpdateSubFirmware(void)
{
	uint8_t i;
	uint8_t data = 0xff;
	if((otaStationInfo.head == 0xaa) && (otaStationInfo.end == 0x55))
	{
		if(otaStationInfo.firmwareValid == 1)
		{
			if(otaStationInfo.devImageVersion <= gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion)
			{	printf("----------------01 otaStationInfo,0xff====%d,%d\r\n",otaStationInfo.devImageVersion,gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion);
				memset((char*)&otaStationInfo,0xff,sizeof(otaStationInfo));
				otaStationInfo.head = 0xaa;
				otaStationInfo.end = 0x55;
				qca_save_flash_params(START_ADDR_FLASH_SUB_FIRMWARE_INFO_ADDR,(char*)&otaStationInfo,sizeof(otaStationInfo));
				for(i = 0; i < MAX_SUB_FIRMWARE_PAGE; i ++)
				{
					qca_save_flash_params(START_ADDR_FLASH_SUB_FIRMWARE_ADDR + i * 4096,(char*)&data,1);
				}
			}else{
				if((gwStateForRealTime.gwDevIsOta != 2) && (otaStationInfo.devImageSize > 0))//zc not otaing
				{
					printf("fill uart cmd to update zc firmware\r\n");
					userFillUartCmdForZCOta(otaStationInfo.devImageVersion);
				}
			}
		}
	}
}

void userCheckOtaSubFirmware(void)
{
	int ret = 0;
	uint8_t parm[4];
	if((otaStationInfo.head == 0xaa) && (otaStationInfo.end == 0x55))
	{
		if(otaStationInfo.firmwareValid == 1)
		{
			if(otaStationInfo.devImageVersion < subOtaRam.subFileVersion)
			{
				;
			}else{
				if(otaStationInfo.devImageVersion > gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion)
				{
					ret = 1;
				}
			}
		}
	}
	printf("the flash dev version:%d\r\n",otaStationInfo.devImageVersion);
	if(subOtaRam.subFileVersion < gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion)
	{
		ret = -1;
	}
	printf("version1:%d,version2:%d\r\n",subOtaRam.subFileVersion , gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion);
	if(ret == 0)
	{
		if(subOtaRam.sufFileSize < MAX_SUB_FIRMWARE_SIZE)
		{
			subOtaRam.subOtaStart = 1;
		}else{
			printf("the target sub firmware was too large to not down load\r\n");
			ret = -1;
		}
	}else if(ret == 1){
		printf("start update the flash firmware to ram\r\n");
		subOtaRam.subOtaStart = 2;
	}else{
		printf("the cloud dev firmware is not new them board\r\n");
	}
	if(ret == -1)
	{
		parm[0] = OTA_MODE_ZC;
		parm[1] = HM_IOTS_SMALL_GW_TYPE & 0xff;
		parm[2] = (HM_IOTS_SMALL_GW_TYPE >> 8) & 0xff;
		parm[3] = 0;
		hmSetIotsEvent(HFZ_EVENT_SOURCE_OTA, 0, (void *)&parm,4);
	}
}

static void userInitSubFirmwareOtaInfo(void)
{
	memset((char*)&subOtaRam,0,sizeof(subOtaRam));
}

static int user_ota_result_cb(user_ota_result_t result)
{
	static uint8_t faildNum = 0;
	uint8_t parm[4];
	if(result == USER_OTA_RESULT_SUCCEED){
		printf("ota succeed\r\n");
		parm[0] = OTA_MODE_WIFI;
		parm[1] = HM_IOTS_SMALL_GW_TYPE & 0xff;
		parm[2] = (HM_IOTS_SMALL_GW_TYPE >> 8) & 0xff;
		parm[3] = 1;
		hmSetIotsEvent(HFZ_EVENT_SOURCE_OTA, 0, (void *)&parm,4);
		qcom_thread_msleep(2000);
		userCheckSaveFlashAndReset(1);
	}else{
		printf("ota failed\r\n");
		faildNum ++;
		if(faildNum >= 2)
		{
			parm[0] = OTA_MODE_WIFI;
			parm[1] = HM_IOTS_SMALL_GW_TYPE & 0xff;
			parm[2] = (HM_IOTS_SMALL_GW_TYPE >> 8) & 0xff;
			parm[3] = 0;
			hmSetIotsEvent(HFZ_EVENT_SOURCE_OTA, 0, (void *)&parm,4);
			gwStateForRealTime.wifiSoftUpdate.appAllowSoftWareUpdate = 0;
			gwStateForRealTime.wifiSoftUpdate.haveNewestSoftWare = 0;
			memset(gwStateForRealTime.wifiSoftUpdate.softUrl,'\0',MAX_WOFTWARE_ADDR_LEN);
		}
	}
}

static void user_ota_value_reset()
{
	tmp_len = 0;
	flag = 0;
}

static void user_ota_get_cb(unsigned char *buf, unsigned int len)
{
	int ret = 0;

	// failed
	if(flag == 1){
		return;
	}

	// First block must more than 24 bytes
	if(tmp_len != 32){
		if(len < 32 - tmp_len){
			memcpy(tmp_buf + tmp_len, buf, len);
			tmp_len += len;
			return;
		}else{
			memcpy(tmp_buf + tmp_len, buf, 32 - tmp_len);

			len -= (32 - tmp_len);
			buf += (32 - tmp_len);
			tmp_len = 32;

			ret = qca_ota_write((unsigned char *)tmp_buf, tmp_len);
			if(ret != 0){
				printf("write first block failed(%d)\n", ret);
				flag = 1;
			}
		}
	}

	ret = qca_ota_write(buf, len);
	if(ret != 0){
		printf("write failed(%d)\n", ret);
		flag = 1;
	}
}

int user_ota_start(char *url, unsigned short port, user_ota_result_cb_t cb)
{
	int ret = 0;
	char *pFind;
	static char url_without_domain[128];
	user_ota_result_t result = USER_OTA_RESULT_SUCCEED;

	do{
	    	pFind = strstr(url, "//");

		pFind = strchr(pFind != NULL ? pFind + 2 : url, '/');

		if (pFind == NULL){
			result = USER_OTA_RESULT_FAILED;
	        	break;
		}

		memset(url_without_domain, 0, sizeof(url_without_domain));
		strcpy(url_without_domain, pFind);
	    	*pFind = '\0';

		if(qca_ota_start() == -1){
			printf("qca ota start failed\n");
			return -1;
		}

			printf("OTA http_client_startup url:%s\n",url);
	    	ret = http_client_startup((unsigned char *)url, port);
		
		if(ret == 0){
			//add
			//memcpy(url_without_domain+strlen(url_without_domain),2,1);//del
			ret = http_client_get(url_without_domain, NULL, user_ota_get_cb);
			printf("the wifi http_client_get :%d,url_without_domain:%s\n",ret,url_without_domain);//-----
			if(ret == 0 && flag == 0){
				printf("HTTP finish.\n");
				ret = qca_ota_finish();
				if (ret == 0){
					printf("OTA success.\n");
					result = USER_OTA_RESULT_SUCCEED;
					break;
				}
			}else{
				if(flag == 0)
				{
					printf("user ota failed(get failed)\r\n");
				}else{
					printf("user ota failed(write failed)\r\n");
				}
				result = USER_OTA_RESULT_FAILED;
	            		break;
			}
		}else{
	        	printf("user ota failed(startup failed)\r\n");
			result = USER_OTA_RESULT_FAILED;
	       		break;
		}
	}while(0);

   	http_client_stop();
	user_ota_value_reset();
	ret = qca_ota_finish();
	if(ret != 0){
		result = USER_OTA_RESULT_FAILED;
	}else{
		result = USER_OTA_RESULT_SUCCEED;
	}

    	cb(result);

    	return -1;
}



static void userGetSubFirmwarmCb(unsigned char *buf, unsigned int len)
{
	int ret = 0;

	// failed
	if(subFlag == 1){
		return;
	}

	// First block must more than 24 bytes
	if(subTmp_len != 32){
		if(len < 32 - subTmp_len){
			memcpy(subTmp_buf + subTmp_len, buf, len);
			subTmp_len += len;
			return;
		}else{
			memcpy(subTmp_buf + subTmp_len, buf, 32 - subTmp_len);

			len -= (32 - subTmp_len);
			buf += (32 - subTmp_len);
			subTmp_len = 32;

			ret = userWriteSubFirmwareToFlash((unsigned char *)subTmp_buf, subTmp_len);
			if(ret != 0){
				printf("write first block failed(%d)\n", ret);
				subFlag = 1;
			}
		}
	}

	ret = userWriteSubFirmwareToFlash(buf, len);
	if(ret != 0){
		printf("write failed(%d)\n", ret);
		subFlag = 1;
	}
}
int userGetSubFirmwarmFromHttp(char *url, unsigned short port)
{
	int ret = 0;
	char *pFind;
	static char url_without_domain[128];
	char temp_url[64],i;
	user_ota_result_t result = USER_OTA_RESULT_SUCCEED;

	do{
	    	pFind = strstr(url, "//");

		pFind = strchr(pFind != NULL ? pFind + 2 : url, '/');

		if (pFind == NULL){
			result = USER_OTA_RESULT_FAILED;
	        	break;
		}

		memset(url_without_domain, 0, sizeof(url_without_domain));
		strcpy(url_without_domain, pFind);
		/*strcpy(temp_url, pFind);
		for(i=0;i<20;i++)
			{
			if(temp_url[i]==':')
				{
					printf("sub temp_url:%d\n",i);
					break;
				}
			}
		memcpy(url_without_domain,temp_url,i);
		printf("url_without_domain:%s,temp_url:%s\n",url_without_domain,temp_url);*/
		
	    	*pFind = '\0';

		if(userStartSubOta() == -1){
			printf("sub file ota start failed\n");
			return -1;
		}

		printf("sub http_client_startup :%s\n",url);
		printf("sub ret:%d\n",ret);
	    ret = http_client_startup((unsigned char *)url, port);
		
		if(ret == 0){
			//add
			//memcpy(url_without_domain+strlen(url_without_domain),2,1);//-----------del
			ret = http_client_get(url_without_domain, NULL, userGetSubFirmwarmCb);
			printf("the sub http_client_get :%d,url_without_domain:%s\n",ret,url_without_domain);//-----
			if(ret == 0 && subFlag == 0){
				printf("HTTP finish.\n");
				ret = userFinishSubFirmwareOta();
				if (ret == 0){
					printf("sub file OTA success.\n");
					result = USER_OTA_RESULT_SUCCEED;
					break;
				}
			}else{
				if(subFlag == 0)
				{
					printf("user sub file ota failed(get failed)\r\n");
				}else{
					printf("user sub file ota failed(write failed)\r\n");
				}
				result = USER_OTA_RESULT_FAILED;
	            		break;
			}
		}else{
	        	printf("user sub file ota failed(startup failed)\r\n");
			result = USER_OTA_RESULT_FAILED;
	       		break;
		}
	}while(0);
	printf("the api http client stop\r\n");
   	http_client_stop();
   	userInitSubFirmwareOtaInfo();
	if(result == USER_OTA_RESULT_SUCCEED)
	{
		return 0;
	}
}




static void ota_task(unsigned int arg)
{
	int part = 0;
	uint8_t forNum;
	uint8_t parm[0];
	uint16_t checkFlashDevFirmwareTimes = 0;
	while(1)
	{
		forNum ++;
		if(forNum > 250)
		{
			forNum = 0;
			part = qca_ota_get_current_partition();
			printf("Current partition is %d\r\n", part);
		}
		if((gwStateForRealTime.wifiSoftUpdate.haveNewestSoftWare == 1) && (gwStateForRealTime.wifiSoftUpdate.appAllowSoftWareUpdate == 1))
		{	printf("ota_task.... gwStateForRealTime\n");
			gwStateForRealTime.gwDevIsOta = 1;//
			printf("OTA start, url: %s, port: %d\r\n",gwStateForRealTime.wifiSoftUpdate.softUrl, OTA_SERVER_PORT);//gwStateForRealTime.wifiSoftUpdate.softUrl
			user_ota_start(gwStateForRealTime.wifiSoftUpdate.softUrl, OTA_SERVER_PORT, user_ota_result_cb);//
			gwStateForRealTime.gwDevIsOta = 0;
		}
		if(devOtaUpdateingFlag == 1)//zc OTA
		{
			if(otaStationInfo.firmwareValid == 1)
			{
				printf("start zigbee zc ota\r\n");
				gwStateForRealTime.gwDevIsOta = 2;
				qcom_watchdog(0,0);
				userXmodemTransport();
				qcom_watchdog(1,15);
				gwStateForRealTime.gwDevIsOta = 0;
				checkFlashDevFirmwareTimes = 36000;
			}
			devOtaUpdateingFlag = 0;
		}

		if(subOtaRam.subOtaStart == 1)	//http down load sub firmware
		{
			printf("sub firmware ota start:%s\r\n",subOtaRam.subFileUrl);
			//OTA_SERVER_PORT=8080;
			if(userGetSubFirmwarmFromHttp(subOtaRam.subFileUrl,OTA_SERVER_PORT) == 0)//OTA_SERVER_PORT
			{//最新sub 固件下载完成，并校验无误
				qcom_thread_msleep(5000);
				subOtaRam.subOtaStart = 2;
			}else{
				subOtaRam.subOtaStart = 0;
				printf("the subOtaRam.subOtaStart =0\r\n");
			}
		}else if(subOtaRam.subOtaStart == 2){
			checkFlashDevFirmwareTimes = 36000;
			subOtaRam.subOtaStart = 0;
		}

		// 2个小时检测一次flash中的固件和正在运行的固件版本，如果flash中的固件
		//比正在运行的高，则开始发送OTA指令，如果低，则清除flash中的固件
		checkFlashDevFirmwareTimes ++;
		if(checkFlashDevFirmwareTimes >= 36000)
		{
			//add
			otaStationInfo.devImageVersion=0x26;//0x24
			userFillUartCmdForZCOta(otaStationInfo.devImageVersion);
			
			checkFlashDevFirmwareTimes = 0;
			userCheckUpdateSubFirmware();//检测是否需要通过串口update到dev中
			printf("the sorf ram dev firmware:%d,flash firmware:%d\r\n",gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion,otaStationInfo.devImageVersion);
			
		}
		qcom_thread_msleep(200);
	}
	
}

void userOtaAndHttpTaskInit(void)
{
	printf("start ota task\r\n");
	qca_task_start(ota_task, 0, 2048);
}

