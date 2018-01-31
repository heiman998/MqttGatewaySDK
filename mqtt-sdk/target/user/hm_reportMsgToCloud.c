#include "hm_reportMsgToCloud.h"
#include "qcom_common.h"
#include "qca_calendar.h"
#include "qca_json.h"
#include "hm_app.h"
#include "qca_base64.h"
#include "stdlib.h"
#include "hm_network_data_manage.h"
#include "ServerDataHandle.h"

//#include "hm_reportDevToCloud.h"







/*




//主动上报zigbee设备报警pass发送，写入缓存//
void userAlarmMsgData_write(unsigned char* buff, int* bufflen,HM_DEV_RAM_LIST *pni)
{
	uint8_t ZT_valuedata;
	uint32_t converte_data=0,len;
	char strZBMac[18];
	uint8_t *ptr=buff;
	uint32_t rc=0x0001;
	//json_t *state = json_create_object();

	len=0;//bufflen=0;
	big_little_exchange(rc,&converte_data);
	memcpy(ptr,&converte_data,2);
	len+=2;
	big_little_exchange(pni->index,&converte_data);
	memcpy(ptr+len,&converte_data,2);
	len+=2;
	big_little_exchange(pni->devType,&converte_data);
	memcpy(ptr+len,&converte_data,2);
	len+=2;//mac[ZIGBEE_MAC_LEN];
	memcpy(ptr+len,pni->mac,8);
	len+=8;	
	pni->epList[0].devData.zoneStateApp.alarms=1;
	printf("pni->onLine=%x,pni->batteryAlarm=%x,pni->epList[0].devData.zoneStateApp.alarms=%x\n",pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	ZT_valuedata=userCreate_ZTdata(ONLINE_VALUE_TYPE,pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	memcpy(ptr,&ZT_valuedata,1);len+=1;
	memcpy(ptr+len,&pni->batteryRemain,1);len+=1;
	Int_biglittle_Convert(pni->epList[0].times,&converte_data);
	memcpy(ptr+len,&converte_data,4);
	len+=4;
	*bufflen=len;

	//Sent_pass_data(buff,bufflen);
//ZBINTOSS_CMD
}


void userUpdateMsgDataToCloud(void)
{
	uint8_t i;
	if(0){
	for(i = 0; i < 5; i ++)//MAX_ALARM_CLOUD_BUF_NUM
	{
		if(alarmBuf[i].validFlag == 1)
		{
			printf("alarm to cloud\r\n");
			//userForXlinkGetAllDataPoint((char*)&alarmBuf[i],DEV_DATA_FOR_ALARM_TO_CLOUD,alarm_report_data,&alarm_report_len);
			//Xlink_UpdateDataPoint(alarm_report_data,alarm_report_len,0);//0 开启推送，1 关闭推送
			memset((char*)&alarmBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}

	for(i = 0; i < 5; i ++)
	{
		if(cloudBuf[i].validFlag == 1)
		{
			printf("record to cloud\r\n");
			//userForXlinkGetAllDataPoint((char*)&cloudBuf[i],DEV_DATA_FOR_RECORD_TO_CLOUD,alarm_report_data,&alarm_report_len);
			//Xlink_UpdateDataPoint(alarm_report_data,alarm_report_len,1);//0 开启推送，1 关闭推送
			memset((char*)&cloudBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}
		}
}
*/



