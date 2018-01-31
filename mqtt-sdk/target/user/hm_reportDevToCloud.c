#include "qcom_common.h"
#include "qca_calendar.h"
#include "qca_json.h"
#include "hm_app.h"
#include "qca_base64.h"
#include "stdlib.h"
#include "hm_net_data_manage.h"
//#include "hm_reportDevToCloud.h"
#include "hm_zigbeeInfohand.h"

#include "hm_reportDevToCloud.h"
#include "hm_reportMsgToCloud.h"
#include "hm_network_data_manage.h"
#include "ServerDataHandle.h"




extern DEV_MYSELF_INFO hmIotsDevInfo;
extern DEV_NUM_INFO devNumInfo;
extern GWSTATFORTIME gwStateForRealTime;
REPORTTOCLOUD cloudBuf[MAX_REPORT_CLOUD_BUF_NUM];
ALARMTOCLOUD alarmBuf[MAX_ALARM_CLOUD_BUF_NUM];
extern char *alarm_report_data;
extern uint16_t alarm_report_len;
extern HM_GW_LIST gw_info;

//#define DEV_DATA_FOR_RECORD_TO_CLOUD 			1
//#define DEV_DATA_FOR_ALARM_TO_CLOUD				2

void userFillTheBufAlarmToCloud(char mode,char *ios,uint16_t iosLen,char *android,uint16_t androidLen)
{
	uint8_t i;
	uint8_t bufIndex = 0xff;
	if((ios == NULL) || (android == NULL))
	{
		printf("in alarm to cloud buf:the in put data point was null\r\n");
		return;
	}
	if((iosLen > MAX_ALARM_CLOUD_BUF_LEN) || (androidLen > MAX_ALARM_CLOUD_BUF_LEN))
	{
		printf("in up to cloud buf:in put data was too long\r\n ");
		return;
	}
	for(i = 0; i <MAX_ALARM_CLOUD_BUF_NUM; i ++)
	{
		if(alarmBuf[i].validFlag == 0)
		{
			bufIndex = i;
			break;
		}
	}
	if(bufIndex == 0xff)
	{
		printf("in alarm to cloud buf:the buf was full\r\n");
		return;
	}else{
		alarmBuf[bufIndex].validFlag = 1;
		alarmBuf[bufIndex].iosLen = iosLen;
		memcpy(alarmBuf[bufIndex].iosBuf,ios,iosLen);

		alarmBuf[bufIndex].androidLen = androidLen;
		memcpy(alarmBuf[bufIndex].androidBuf,android,androidLen);

		alarmBuf[bufIndex].alarmMode=mode;
	}
	
}

void userFillTheBufReportToCloud(char *data,uint16_t len)
{
	uint8_t i;
	uint8_t bufIndex = 0xff;
	if(data == NULL)
	{
		printf("in up to cloud buf:the in put data point was null\r\n");
		return;
	}
	if(len > MAX_REPROT_CLOUD_BUF_LEN)
	{
		printf("in up to cloud buf:in put data was too long\r\n ");
		return;
	}
	for(i = 0; i <MAX_REPORT_CLOUD_BUF_NUM; i ++)
	{
		if(cloudBuf[i].validFlag == 0)
		{
			bufIndex = i;
			break;
		}
	}
	if(bufIndex == 0xff)
	{
		printf("in up to cloud buf:the buf was full\r\n");
		return;
	}else{
		cloudBuf[bufIndex].validFlag = 1;
		cloudBuf[bufIndex].dataLen = len;
		memcpy(cloudBuf[bufIndex].data,data,len);
	}
}

json_t *userCreatWifiDevInfoJsObject(DEV_MYSELF_INFO *tempGwInfo)
{
	char tempData[18];
	json_t *info = json_create_object();
	char sign;
	uint16_t zones;
	memset(tempData,'\0',18);
	sprintf(tempData,"%02X%02X%02X%02X%02X%02X",tempGwInfo->wifiMac[0],tempGwInfo->wifiMac[1],tempGwInfo->wifiMac[2],tempGwInfo->wifiMac[3],
															tempGwInfo->wifiMac[4],tempGwInfo->wifiMac[5]);

															
	json_add_item_to_object(info, "WM", json_create_string(tempData));
	json_add_item_to_object(info, "FD", json_create_number(1));
	json_add_item_to_object(info, "WHV", json_create_string(HM_IOTS_HARD_VERSION));
	json_add_item_to_object(info, "WSV", json_create_string(HM_IOTS_SOFT_VERSION));

	memset(tempData,'\0',18);
	if(tempGwInfo->time_zones > 0)
	{
		sign = '+';
		zones = tempGwInfo->time_zones;
	}else{
		sign = '-';
		zones = tempGwInfo->time_zones * -1;
	}
	
	sprintf(tempData,"%c%d.%2d",sign,zones / 100,zones % 100);
	printf("the zone string :%s\r\n",tempData);
	json_add_item_to_object(info, "TZ", json_create_string(tempData));
	json_add_item_to_object(info, "TY", json_create_number(HM_IOTS_SMALL_GW_TYPE));
	json_add_item_to_object(info, "PV", json_create_number(HM_IOTS_PROTOCOL_VERSION));

	memset(tempData,'\0',18);
	sprintf(tempData,"%x.%x.0",(gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion >> 4)&0x0f,gwStateForRealTime.zigbeeZcBasicInfo.zcSoftVersion&0x0f);
	json_add_item_to_object(info, "ZSV", json_create_string(tempData));

	return info;
}

json_t *userCreatGWStaObject(DEV_MYSELF_INFO *tempGwInfo,DEVINFOOPTION devOption)
{
	json_t *state = json_create_object();
	
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "AL", json_create_number(tempGwInfo->sound_level));
		json_add_item_to_object(state, "BT", json_create_number(tempGwInfo->beep_alarm_times));
		if(tempGwInfo->app_language == LANGUAGE_ENGLISH)
		{
			json_add_item_to_object(state, "LG", json_create_string("US"));
		}else if(tempGwInfo->app_language == LANGUAGE_CHINESE){
			json_add_item_to_object(state, "LG", json_create_string("CN"));
		}
		
		json_add_item_to_object(state, "LL", json_create_number(tempGwInfo->light_level));
		json_add_item_to_object(state, "LO", json_create_number(tempGwInfo->light_onoff));


		json_add_item_to_object(state, "LT", json_create_number(tempGwInfo->light_on_times));
		json_add_item_to_object(state, "AT", json_create_number(tempGwInfo->sensor_arm_type));
		//json_add_item_to_object(state, "RD", json_create_string(tempGwInfo->romId));
	
	}else{
		if(devOption.GWStat.alarmlevel)
		{
			json_add_item_to_object(state, "AL", json_create_number(tempGwInfo->sound_level));
		}

		if(devOption.GWStat.betimer)
		{
			json_add_item_to_object(state, "BT", json_create_number(tempGwInfo->beep_alarm_times));
		}

		if(devOption.GWStat.gwlanguage)
		{
			if(tempGwInfo->app_language == LANGUAGE_ENGLISH)
			{
				json_add_item_to_object(state, "LG", json_create_string("US"));
			}else if(tempGwInfo->app_language == LANGUAGE_CHINESE){
				json_add_item_to_object(state, "LG", json_create_string("CN"));
			}
		}

		if(devOption.GWStat.gwlightlevel)
		{
			json_add_item_to_object(state, "LL", json_create_number(tempGwInfo->light_level));
		}

		if(devOption.GWStat.gwlightonoff)
		{
			json_add_item_to_object(state, "LO", json_create_number(tempGwInfo->light_onoff));
		}

		if(devOption.GWStat.lgtimer)
		{
			json_add_item_to_object(state, "LT", json_create_number(tempGwInfo->light_on_times));
		}

		if(devOption.GWStat.armtype)
		{
			json_add_item_to_object(state, "AT", json_create_number(tempGwInfo->sensor_arm_type));
		}

		if(devOption.GWStat.roomID)
		{
			//json_add_item_to_object(state, "RD", json_create_string(tempGwInfo->romId));
		}
	}
	
	return state;
}


json_t *userCreateWDSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(setting, "OF", json_create_number(pni->epList[0].devData.wdStateApp.action));
		json_add_item_to_object(setting, "DT", json_create_number(pni->epList[0].devData.wdStateApp.duration));
	}else{

		if(devOption.WDSet.onoff)
		{
			json_add_item_to_object(setting, "OF", json_create_number(pni->epList[0].devData.wdStateApp.action));
		}

		if(devOption.WDSet.duration)
		{
			json_add_item_to_object(setting, "DT", json_create_number(pni->epList[0].devData.wdStateApp.duration));
		}
	}
	
	return setting;
}

json_t *userCreateWDStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *state = json_create_object();
	

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.wdStateApp.action));
		json_add_item_to_object(state, "DT", json_create_number(pni->epList[0].devData.wdStateApp.duration));
	}else{
		if(devOption.WDState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}

		if(devOption.WDState.batteryPercent)
		{
			json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		}

		if(devOption.WDState.batteryAlarm)
		{
			json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		}

		if(devOption.WDState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}

		if(devOption.WDState.onoff)
		{
			json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.wdStateApp.action));
		}

		if(devOption.WDState.duration)
		{
			json_add_item_to_object(state, "DT", json_create_number(pni->epList[0].devData.wdStateApp.duration));
		}
	}
	
	return state;
}

json_t *userCreateCutRgbSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	json_t *js_ts1,*js_te1,*js_ts2,*js_te2,*js_ts3,*js_te3,*js_ts4,*js_te4,*onoffList,*brigList,*colorList;
	uint8_t i,s1Index,s2Index,s3Index,s4Index;
	s1Index = 0xff;
	s2Index = 0xff;
	s3Index = 0xff;
	s4Index = 0xff;
	printf("creat cut rgb seting\r\n");
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为第一路switch，EP 2默认为第二路switch
	{
		if(pni->epList[i].ep == 1)
		{
			s1Index = i;
		}else if(pni->epList[i].ep == 2){
			s2Index = i;
		}else if(pni->epList[i].ep == 3){
			s3Index = i;
		}else if(pni->epList[i].ep == 4){
			s4Index = i;
		}
	}

	if((s1Index == 0xff) || (s2Index == 0xff) || (s3Index == 0xff) || (s4Index == 0xff))
	{
		printf("the cut rgb ep list wos error\r\n");
		return setting;
	}
	
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(setting, "OFL", onoffList = json_create_array());
		json_add_item_to_object(setting, "LEL", brigList = json_create_array());
		json_add_item_to_object(setting, "CWL", colorList = json_create_array());

		json_add_item_toarray(onoffList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.onOffStatus));



		json_add_item_toarray(brigList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.brightness));


		json_add_item_toarray(colorList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.colorTemrature));

	

		js_ts1 = json_create_array();
		js_te1 = json_create_array();
	
		json_add_item_to_object(setting,"TS1",js_ts1);
		json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_month));
		json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_day));
		json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_hour));
		json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_min));
		json_add_item_to_object(setting,"TE1",js_te1);
		json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_month));
		json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_day));
		json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_hour));
		json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_min));
		json_add_item_to_object(setting, "WF1", json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.wk_flag));
		

		js_ts2 = json_create_array();
		js_te2 = json_create_array();
		
		json_add_item_to_object(setting,"TS2",js_ts2);
		json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_month));
		json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_day));
		json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_hour));
		json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_min));
		json_add_item_to_object(setting,"TE2",js_te2);
		json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_month));
		json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_day));
		json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_hour));
		json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_min));
		json_add_item_to_object(setting, "WF2", json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.wk_flag));
		

		js_ts3 = json_create_array();
		js_te3 = json_create_array();
		
		json_add_item_to_object(setting,"TS3",js_ts3);
		json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_month));
		json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_day));
		json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_hour));
		json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_min));
		json_add_item_to_object(setting,"TE3",js_te3);
		json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_month));
		json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_day));
		json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_hour));
		json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_min));
		json_add_item_to_object(setting, "WF3", json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.wk_flag));

		js_ts4 = json_create_array();
		js_te4 = json_create_array();
		
		json_add_item_to_object(setting,"TS4",js_ts4);
		json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_month));
		json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_day));
		json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_hour));
		json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_min));
		json_add_item_to_object(setting,"TE4",js_te4);
		json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_month));
		json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_day));
		json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_hour));
		json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_min));
		json_add_item_to_object(setting, "WF4", json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.wk_flag));
		
	}else{
		

		if((devOption.CUTRGBSet.onoffList1 == 1) || (devOption.CUTRGBSet.onoffList2 == 1) || (devOption.CUTRGBSet.onoffList3 == 1) || (devOption.CUTRGBSet.onoffList4 == 1))
		{
			json_add_item_to_object(setting, "OFL", onoffList = json_create_array());
			if(devOption.CUTRGBSet.onoffList1)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.onoffList2)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.onoffList3)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.onoffList4)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}
		}


		if((devOption.CUTRGBSet.brightnessList1 == 1) || (devOption.CUTRGBSet.brightnessList2 == 1) || (devOption.CUTRGBSet.brightnessList3 == 1) || (devOption.CUTRGBSet.brightnessList4 == 1))
		{
			json_add_item_to_object(setting, "LEL", brigList = json_create_array());
			if(devOption.CUTRGBSet.brightnessList1)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.brightnessList2)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.brightnessList3)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.brightnessList4)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}
		}

		if((devOption.CUTRGBSet.colorTempList1 == 1) || (devOption.CUTRGBSet.colorTempList2 == 1) || (devOption.CUTRGBSet.colorTempList3 == 1) || (devOption.CUTRGBSet.colorTempList4 == 1))
		{
			json_add_item_to_object(setting, "CWL", colorList = json_create_array());
			if(devOption.CUTRGBSet.colorTempList1)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.colorTempList2)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.colorTempList3)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBSet.colorTempList4)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}
		}


	


		if(devOption.CUTRGBSet.ts1)
		{
			js_ts1 = json_create_array();
			json_add_item_to_object(setting,"TS1",js_ts1);
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_month));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_day));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_hour));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.s_min));
		}

		if(devOption.CUTRGBSet.te1)
		{
			js_te1 = json_create_array();
			json_add_item_to_object(setting,"TE1",js_te1);
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_month));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_day));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_hour));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.e_min));
		}

		
		if(devOption.CUTRGBSet.wflag1)
		{
			json_add_item_to_object(setting, "WF1", json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.timer.wk_flag));
		}



		if(devOption.CUTRGBSet.ts2)
		{
			js_ts2 = json_create_array();
			json_add_item_to_object(setting,"TS2",js_ts2);
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_month));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_day));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_hour));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.s_min));
		}

		if(devOption.CUTRGBSet.te2)
		{
			js_te2 = json_create_array();
			json_add_item_to_object(setting,"TE2",js_te2);
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_month));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_day));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_hour));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.e_min));
		}

		if(devOption.CUTRGBSet.wflag2)
		{
			json_add_item_to_object(setting, "WF2", json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.timer.wk_flag));
		}
		


		if(devOption.CUTRGBSet.ts3)
		{
			js_ts3 = json_create_array();
			json_add_item_to_object(setting,"TS3",js_ts3);
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_month));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_day));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_hour));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.s_min));
		}

		if(devOption.CUTRGBSet.te3)
		{
			js_te3 = json_create_array();
			json_add_item_to_object(setting,"TE3",js_te3);
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_month));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_day));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_hour));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.e_min));
		}

		if(devOption.CUTRGBSet.wflag3)
		{
			json_add_item_to_object(setting, "WF3", json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.timer.wk_flag));
		}

		if(devOption.CUTRGBSet.ts4)
		{
			js_ts4 = json_create_array();
			json_add_item_to_object(setting,"TS4",js_ts4);
			json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_month));
			json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_day));
			json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_hour));
			json_add_item_toarray(js_ts4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.s_min));
		}

		if(devOption.CUTRGBSet.te4)
		{
			js_te4 = json_create_array();
			json_add_item_to_object(setting,"TE4",js_te4);
			json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_month));
			json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_day));
			json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_hour));
			json_add_item_toarray(js_te4, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.e_min));
		}

		if(devOption.CUTRGBSet.wflag4)
		{
			json_add_item_to_object(setting, "WF4", json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.timer.wk_flag));
		}
		

	}
	return setting;
}

json_t *userCreateCutRgbStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *state = json_create_object();
	json_t *onoffList,*brigList,*colorList;
	uint8_t i,s1Index,s2Index,s3Index,s4Index;
	s1Index = 0xff;
	s2Index = 0xff;
	s3Index = 0xff;
	s4Index = 0xff;
	
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为第一路switch，EP 2默认为第二路switch
	{
		if(pni->epList[i].ep == 1)
		{
			s1Index = i;
		}else if(pni->epList[i].ep == 2){
			s2Index = i;
		}else if(pni->epList[i].ep == 3){
			s3Index = i;
		}else if(pni->epList[i].ep == 4){
			s4Index = i;
		}
	}

	if((s1Index == 0xff) || (s2Index == 0xff) || (s3Index == 0xff) || (s4Index == 0xff))
	{
		printf("the cut rgb ep list wos error\r\n");
		return state;
	}

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "OFL", onoffList = json_create_array());
		json_add_item_to_object(state, "LEL", brigList = json_create_array());
		json_add_item_to_object(state, "CWL", colorList = json_create_array());
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));

		json_add_item_toarray(onoffList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.onOffStatus));
		json_add_item_toarray(onoffList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.onOffStatus));



		json_add_item_toarray(brigList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.brightness));
		json_add_item_toarray(brigList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.brightness));


		json_add_item_toarray(colorList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.colorTemrature));
		json_add_item_toarray(colorList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.colorTemrature));
		
	}else{
		if(devOption.CUTRGBState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}

		if((devOption.CUTRGBState.onoffList1 == 1) || (devOption.CUTRGBState.onoffList2 == 1) || (devOption.CUTRGBState.onoffList3 == 1) || (devOption.CUTRGBState.onoffList4 == 1))
		{
			json_add_item_to_object(state, "OFL", onoffList = json_create_array());
			if(devOption.CUTRGBState.onoffList1)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.onoffList2)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.onoffList3)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.onoffList4)
			{
				json_add_item_toarray(onoffList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.onOffStatus));
			}else{
				json_add_item_toarray(onoffList, json_create_number(-1));
			}
		}


		if((devOption.CUTRGBState.brightnessList1 == 1) || (devOption.CUTRGBState.brightnessList2 == 1) || (devOption.CUTRGBState.brightnessList3 == 1) || (devOption.CUTRGBState.brightnessList4 == 1))
		{
			json_add_item_to_object(state, "LEL", brigList = json_create_array());
			if(devOption.CUTRGBState.brightnessList1)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.brightnessList2)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.brightnessList3)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.brightnessList4)
			{
				json_add_item_toarray(brigList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.brightness));
			}else{
				json_add_item_toarray(brigList, json_create_number(-1));
			}
		}

		if((devOption.CUTRGBState.colorTempList1 == 1) || (devOption.CUTRGBState.colorTempList2 == 1) || (devOption.CUTRGBState.colorTempList3 == 1) || (devOption.CUTRGBState.colorTempList4 == 1))
		{
			json_add_item_to_object(state, "CWL", colorList = json_create_array());
			if(devOption.CUTRGBState.colorTempList1)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s1Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.colorTempList2)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s2Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.colorTempList3)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s3Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}

			if(devOption.CUTRGBState.colorTempList4)
			{
				json_add_item_toarray(colorList, json_create_number(pni->epList[s4Index].devData.cstRgbStateApp.colorTemrature));
			}else{
				json_add_item_toarray(colorList, json_create_number(-1));
			}
		}

	}
	return state;
}

json_t *userCreateAiSwitchSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	json_t *js_ts1,*js_te1,*js_ts2,*js_te2,*js_ts3,*js_te3;
	uint8_t i,s1Index = 0xff,s2Index = 0xff,s3Index = 0xff;
	s1Index = 0xff;
	s2Index = 0xff;
	s3Index = 0xff;
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为第一路switch，EP 2默认为第二路switch
	{
		if(pni->epList[i].ep == 1)
		{
			s1Index = i;
		}else if(pni->epList[i].ep == 2){
			s2Index = i;
		}else if(pni->epList[i].ep == 3){
			s3Index = i;
		}
	}
	printf("creat sw setting\r\n");
	if(devOption.modeByte == 0xffffffff)
	{
		if(s1Index != 0xff)
		{
			js_ts1 = json_create_array();
			js_te1 = json_create_array();
			json_add_item_to_object(setting, "OF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(setting,"TS1",js_ts1);
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_month));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_day));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_hour));
			json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_min));
			json_add_item_to_object(setting,"TE1",js_te1);
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_month));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_day));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_hour));
			json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_min));
			json_add_item_to_object(setting, "WF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.wk_flag));
		}
		if(s2Index != 0xff)
		{
			js_ts2 = json_create_array();
			js_te2 = json_create_array();
			json_add_item_to_object(setting, "OF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(setting,"TS2",js_ts2);
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_month));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_day));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_hour));
			json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_min));
			json_add_item_to_object(setting,"TE2",js_te2);
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_month));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_day));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_hour));
			json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_min));
			json_add_item_to_object(setting, "WF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.wk_flag));
		}
		if(s3Index != 0xff)
		{
			js_ts3 = json_create_array();
			js_te3 = json_create_array();
			json_add_item_to_object(setting, "OF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(setting,"TS3",js_ts3);
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_month));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_day));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_hour));
			json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_min));
			json_add_item_to_object(setting,"TE3",js_te3);
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_month));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_day));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_hour));
			json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_min));
			json_add_item_to_object(setting, "WF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.wk_flag));
		}
	}else{
		if(s1Index != 0xff)
		{
			if(devOption.SWSet.onoff1)
			{
				json_add_item_to_object(setting, "OF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWSet.ts1)
			{
				js_ts1 = json_create_array();
				json_add_item_to_object(setting,"TS1",js_ts1);
				json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_month));
				json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_day));
				json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_hour));
				json_add_item_toarray(js_ts1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.s_min));
			}

			if(devOption.SWSet.te1)
			{
				js_te1 = json_create_array();
				json_add_item_to_object(setting,"TE1",js_te1);
				json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_month));
				json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_day));
				json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_hour));
				json_add_item_toarray(js_te1, json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.e_min));
			}

			
			if(devOption.SWSet.wflag1)
			{
				json_add_item_to_object(setting, "WF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.timer.wk_flag));
			}
		}
		if(s2Index != 0xff)
		{
			if(devOption.SWSet.onoff2)
			{
				json_add_item_to_object(setting, "OF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWSet.ts2)
			{
				js_ts2 = json_create_array();
				json_add_item_to_object(setting,"TS2",js_ts2);
				json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_month));
				json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_day));
				json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_hour));
				json_add_item_toarray(js_ts2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.s_min));
			}

			if(devOption.SWSet.te2)
			{
				js_te2 = json_create_array();
				json_add_item_to_object(setting,"TE2",js_te2);
				json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_month));
				json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_day));
				json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_hour));
				json_add_item_toarray(js_te2, json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.e_min));
			}

			if(devOption.SWSet.wflag2)
			{
				json_add_item_to_object(setting, "WF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.timer.wk_flag));
			}
		}
		if(s3Index != 0xff)
		{
			if(devOption.SWSet.onoff3)
			{
				json_add_item_to_object(setting, "OF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWSet.ts3)
			{
				js_ts3 = json_create_array();
				json_add_item_to_object(setting,"TS3",js_ts3);
				json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_month));
				json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_day));
				json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_hour));
				json_add_item_toarray(js_ts3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.s_min));
			}

			if(devOption.SWSet.te3)
			{
				js_te3 = json_create_array();
				json_add_item_to_object(setting,"TE3",js_te3);
				json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_month));
				json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_day));
				json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_hour));
				json_add_item_toarray(js_te3, json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.e_min));
			}

			if(devOption.SWSet.wflag3)
			{
				json_add_item_to_object(setting, "WF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.timer.wk_flag));
			}
		}

	}
	return setting;
}

json_t *userCreateAiSwitchStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *state = json_create_object();
	uint8_t i,s1Index,s2Index,s3Index;
	s1Index = 0xff;
	s2Index = 0xff;
	s3Index = 0xff;
	
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为第一路switch，EP 2默认为第二路switch
	{
		if(pni->epList[i].ep == 1)
		{
			s1Index = i;
		}else if(pni->epList[i].ep == 2){
			s2Index = i;
		}else if(pni->epList[i].ep == 3){
			s3Index = i;
		}
	}

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		if(s1Index != 0xff)
		{
			json_add_item_to_object(state, "OF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(state, "TM1", json_create_number(pni->epList[s1Index].times));
		}
		if(s2Index != 0xff)
		{
			json_add_item_to_object(state, "OF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(state, "TM2", json_create_number(pni->epList[s2Index].times));
		}
		if(s3Index != 0xff)
		{
			json_add_item_to_object(state, "OF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.onOffStatus));
			json_add_item_to_object(state, "TM3", json_create_number(pni->epList[s3Index].times));
		}
	}else{
		if(devOption.SWState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}
		if(s1Index != 0xff)
		{
			if(devOption.SWState.onoff1)
			{
				json_add_item_to_object(state, "OF1", json_create_number(pni->epList[s1Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWState.time1)
			{
				json_add_item_to_object(state, "TM1", json_create_number(pni->epList[s1Index].times));
			}
		}
		if(s2Index != 0xff)
		{
			if(devOption.SWState.onoff2)
			{
				json_add_item_to_object(state, "OF2", json_create_number(pni->epList[s2Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWState.time2)
			{
				json_add_item_to_object(state, "TM2", json_create_number(pni->epList[s2Index].times));
			}
		}
		if(s3Index != 0xff)
		{
			if(devOption.SWState.onoff3)
			{
				json_add_item_to_object(state, "OF3", json_create_number(pni->epList[s3Index].devData.witchStateApp.onOffStatus));
			}

			if(devOption.SWState.time3)
			{
				json_add_item_to_object(state, "TM3", json_create_number(pni->epList[s3Index].times));
			}
		}
	}
	return state;
}



json_t *userCreateSPlugSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	json_t *js_ts,*js_te;
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(setting, "RO", json_create_number(pni->epList[0].devData.splugStateApp.onOffStatus));
		js_ts = json_create_array();
		js_te = json_create_array();
		json_add_item_to_object(setting,"TS",js_ts);
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_month));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_day));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_hour));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_min));
		json_add_item_to_object(setting,"TE",js_te);
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_month));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_day));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_hour));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_min));
		json_add_item_to_object(setting, "WF", json_create_number(pni->epList[0].devData.splugStateApp.timer.wk_flag));
	}else{
		if(devOption.SPlugSet.relayOnoff)
		{
			json_add_item_to_object(setting, "RO", json_create_number(pni->epList[0].devData.splugStateApp.onOffStatus));
		}

		if(devOption.SPlugSet.ts)
		{
			js_ts = json_create_array();
			json_add_item_to_object(setting,"TS",js_ts);
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_month));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_day));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_hour));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.splugStateApp.timer.s_min));
		}

		if(devOption.SPlugSet.te)
		{
			js_te = json_create_array();
			json_add_item_to_object(setting,"TE",js_te);
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_month));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_day));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_hour));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.splugStateApp.timer.e_min));	
		}


		if(devOption.SPlugSet.wflag)
		{
			json_add_item_to_object(setting, "WF", json_create_number(pni->epList[0].devData.splugStateApp.timer.wk_flag));
		}
	}

	return setting;
}

json_t *userCreateSPlugStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{

	json_t *state = json_create_object();

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		json_add_item_to_object(state, "RO", json_create_number(pni->epList[0].devData.splugStateApp.onOffStatus));
		if((pni->onLine == 0) || (pni->epList[0].devData.splugStateApp.onOffStatus == 0))	//掉线或者继电器关闭
		{
			pni->epList[0].devData.splugStateApp.InstantaneousDemand = 0;
		}
		json_add_item_to_object(state, "PW", json_create_number(pni->epList[0].devData.splugStateApp.InstantaneousDemand));
		json_add_item_to_object(state, "ET", json_create_number(pni->epList[0].devData.splugStateApp.CurrentSummationDelivered));
	}else{
		if(devOption.SPlugState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}

		if(devOption.SPlugState.relayOnoff)
		{
			json_add_item_to_object(state, "RO", json_create_number(pni->epList[0].devData.splugStateApp.onOffStatus));
		}

		if((pni->onLine == 0) || (pni->epList[0].devData.splugStateApp.onOffStatus == 0))	//掉线或者继电器关闭
		{
			pni->epList[0].devData.splugStateApp.InstantaneousDemand = 0;
		}

		if(devOption.SPlugState.power_w)
		{
			json_add_item_to_object(state, "PW", json_create_number(pni->epList[0].devData.splugStateApp.InstantaneousDemand));
		}

		if(devOption.SPlugState.power_wh)
		{
			json_add_item_to_object(state, "ET", json_create_number(pni->epList[0].devData.splugStateApp.CurrentSummationDelivered));
		}

		if(devOption.SPlugState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}
	}
	return state;
}


json_t *userCreatePlugSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	json_t *js_tsr,*js_ter,*js_tsu,*js_teu;
	uint8_t i,relayIndex = 0xff;
	uint8_t usbIndex = 0xff;
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为继电器控制，EP 2默认为usb控制
	{
		if(pni->epList[i].ep == 1)
		{
			relayIndex = i;
		}else if(pni->epList[i].ep == 2){
			usbIndex = i;
		}
	}
	if(devOption.modeByte == 0xffffffff)
	{
		if(relayIndex != 0xff)
		{
			json_add_item_to_object(setting, "RO", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.onOffStatus));
			js_tsr = json_create_array();
			json_add_item_to_object(setting,"TSr",js_tsr);
			json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_month));
			json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_day));
			json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_hour));
			json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_min));

			js_ter = json_create_array();
			json_add_item_to_object(setting,"TEr",js_ter);
			json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_month));
			json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_day));
			json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_hour));
			json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_min));
			
			json_add_item_to_object(setting, "WFr", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.wk_flag));
		}

		if(usbIndex != 0xff)
		{
			json_add_item_to_object(setting, "UO", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.onOffStatus));
			js_tsu = json_create_array();
			json_add_item_to_object(setting,"TSu",js_tsu);
			json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_month));
			json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_day));
			json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_hour));
			json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_min));

			js_teu = json_create_array();
			json_add_item_to_object(setting,"TEu",js_teu);
			json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_month));
			json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_day));
			json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_hour));
			json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_min));
			json_add_item_to_object(setting, "WFu", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.wk_flag));
		}
	}else{
		if(relayIndex != 0xff)
		{
			if(devOption.UPlugSet.relayOnoff)
			{
				json_add_item_to_object(setting, "RO", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.onOffStatus));
			}

			if(devOption.UPlugSet.tsr)
			{
				js_tsr = json_create_array();
				json_add_item_to_object(setting,"TSr",js_tsr);
				json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_month));
				json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_day));
				json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_hour));
				json_add_item_toarray(js_tsr, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.s_min));
			}

			if(devOption.UPlugSet.ter)
			{
				js_ter = json_create_array();
				json_add_item_to_object(setting,"TEr",js_ter);
				json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_month));
				json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_day));
				json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_hour));
				json_add_item_toarray(js_ter, json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.e_min));
			}


			if(devOption.UPlugSet.wflag_r)
			{
				json_add_item_to_object(setting, "WFr", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.timer.wk_flag));
			}
		}

		if(usbIndex != 0xff)
		{
			if(devOption.UPlugSet.usbOnoff)
			{
				json_add_item_to_object(setting, "UO", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.onOffStatus));
			}

			if(devOption.UPlugSet.tsu)
			{
				js_tsu = json_create_array();
				json_add_item_to_object(setting,"TSu",js_tsu);
				json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_month));
				json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_day));
				json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_hour));
				json_add_item_toarray(js_tsu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.s_min));
			}

			if(devOption.UPlugSet.teu)
			{
				js_teu = json_create_array();
				json_add_item_to_object(setting,"TEu",js_teu);
				json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_month));
				json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_day));
				json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_hour));
				json_add_item_toarray(js_teu, json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.e_min));
			}

			if(devOption.UPlugSet.wflag_u)
			{
				json_add_item_to_object(setting, "WFu", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.timer.wk_flag));
			}
		}
	}
	return setting;
}
json_t *userCreatePlugStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{

	uint8_t i,relayIndex = 0xff;
	uint8_t usbIndex = 0xff;
	json_t *state = json_create_object();
	for(i = 0; i < pni->epNum; i ++)//ep 1 默认为继电器控制，EP 2默认为usb控制
	{
		if(pni->epList[i].ep == 1)
		{
			relayIndex = i;
		}else if(pni->epList[i].ep == 2){
			usbIndex = i;
		}
	}
	
	if(devOption.modeByte == 0xffffffff)
	{

		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		if(relayIndex != 0xff)
		{
			json_add_item_to_object(state, "RO", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.onOffStatus));
			json_add_item_to_object(state, "TMr", json_create_number(pni->epList[relayIndex].times));
		}

		if(usbIndex != 0xff)
		{
			json_add_item_to_object(state, "UO", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.onOffStatus));
			json_add_item_to_object(state, "TMu", json_create_number(pni->epList[usbIndex].times));
		}
	}else{
		if(devOption.UPlugState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}
		
		if(relayIndex != 0xff)
		{
			if(devOption.UPlugState.relayOnoff)
			{
				json_add_item_to_object(state, "RO", json_create_number(pni->epList[relayIndex].devData.onoffOutStateApp.onOffStatus));
			}

			if(devOption.UPlugState.time_r)
			{
				json_add_item_to_object(state, "TMr", json_create_number(pni->epList[relayIndex].times));
			}
		}

		if(usbIndex != 0xff)
		{
			if(devOption.UPlugState.usbOnoff)
			{
				json_add_item_to_object(state, "UO", json_create_number(pni->epList[usbIndex].devData.onoffOutStateApp.onOffStatus));
			}

			if(devOption.UPlugState.time_u)
			{
				json_add_item_to_object(state, "TMu", json_create_number(pni->epList[usbIndex].times));
			}
		}
	}
	return state;
}

json_t *userCreateRgbSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();	
	json_t *js_ts,*js_te;
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(setting, "OF", json_create_number(pni->epList[0].devData.colorStateApp.onOffStatus));
		json_add_item_to_object(setting, "LE", json_create_number(pni->epList[0].devData.colorStateApp.level));
		json_add_item_to_object(setting, "CR", json_create_number(pni->epList[0].devData.colorStateApp.rgb_r));
		json_add_item_to_object(setting, "CG", json_create_number(pni->epList[0].devData.colorStateApp.rgb_g));
		json_add_item_to_object(setting, "CB", json_create_number(pni->epList[0].devData.colorStateApp.rgb_b));
		
		js_ts = json_create_array();
		json_add_item_to_object(setting,"TS",js_ts);
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_month));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_day));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_hour));
		json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_min));

		js_te = json_create_array();
		json_add_item_to_object(setting,"TE",js_te);
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_month));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_day));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_hour));
		json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_min));
		
		json_add_item_to_object(setting, "WF", json_create_number(pni->epList[0].devData.colorStateApp.timer.wk_flag));
	}else{
		if(devOption.RGBSet.onoff)
		{
			json_add_item_to_object(setting, "OF", json_create_number(pni->epList[0].devData.colorStateApp.onOffStatus));
		}

		if(devOption.RGBSet.level)
		{
			json_add_item_to_object(setting, "LE", json_create_number(pni->epList[0].devData.colorStateApp.level));
		}

		if(devOption.RGBSet.colour_r)
		{
			json_add_item_to_object(setting, "CR", json_create_number(pni->epList[0].devData.colorStateApp.rgb_r));
		}

		if(devOption.RGBSet.colour_g)
		{
			json_add_item_to_object(setting, "CG", json_create_number(pni->epList[0].devData.colorStateApp.rgb_g));
		}

		if(devOption.RGBSet.colour_b)
		{
			json_add_item_to_object(setting, "CB", json_create_number(pni->epList[0].devData.colorStateApp.rgb_b));
		}

		if(devOption.RGBSet.ts)
		{
			js_ts = json_create_array();
			json_add_item_to_object(setting,"TS",js_ts);
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_month));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_day));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_hour));
			json_add_item_toarray(js_ts, json_create_number(pni->epList[0].devData.colorStateApp.timer.s_min));
		}

		if(devOption.RGBSet.te)
		{
			js_te = json_create_array();
			json_add_item_to_object(setting,"TE",js_te);
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_month));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_day));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_hour));
			json_add_item_toarray(js_te, json_create_number(pni->epList[0].devData.colorStateApp.timer.e_min));
		}
		

		if(devOption.RGBSet.wflag)
		{
			json_add_item_to_object(setting, "WF", json_create_number(pni->epList[0].devData.colorStateApp.timer.wk_flag));
		}
	}
	return setting;
}

json_t *userCreateRgbStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{

	json_t *state = json_create_object();
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.colorStateApp.onOffStatus));
		json_add_item_to_object(state, "LE", json_create_number(pni->epList[0].devData.colorStateApp.level));
		json_add_item_to_object(state, "CR", json_create_number(pni->epList[0].devData.colorStateApp.rgb_r));
		json_add_item_to_object(state, "CG", json_create_number(pni->epList[0].devData.colorStateApp.rgb_g));
		json_add_item_to_object(state, "CB", json_create_number(pni->epList[0].devData.colorStateApp.rgb_b));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
	}else{
		if(devOption.RGBState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}

		if(devOption.RGBState.onoff)
		{
			json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.colorStateApp.onOffStatus));
		}

		if(devOption.RGBState.level)
		{
			json_add_item_to_object(state, "LE", json_create_number(pni->epList[0].devData.colorStateApp.level));
		}

		if(devOption.RGBState.colour_r)
		{
			json_add_item_to_object(state, "CR", json_create_number(pni->epList[0].devData.colorStateApp.rgb_r));
		}

		if(devOption.RGBState.colour_g)
		{
			json_add_item_to_object(state, "CG", json_create_number(pni->epList[0].devData.colorStateApp.rgb_g));
		}

		if(devOption.RGBState.colour_b)
		{
			json_add_item_to_object(state, "CB", json_create_number(pni->epList[0].devData.colorStateApp.rgb_b));
		}

		if(devOption.RGBState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}
	}
	return state;
}

json_t *userCreateTHPSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	char strDot[5];
	char strAll[10];
	int16_t cpData;
	uint8_t i,tempEpIndex = 0xff;
	uint8_t humiEpIndex = 0xff;
	json_t *setting = json_create_object();
	for(i = 0; i < pni->epNum; i ++)	//默认EP 1为temperature，2为humility
	{
		if(pni->epList[i].ep == 1)
		{
			tempEpIndex = i;
		}else if(pni->epList[i].ep == 2){
			humiEpIndex = i;
		}
	}

	if(devOption.modeByte == 0xffffffff)
	{
		//temperature
		if(tempEpIndex != 0xff)
		{

			//check up
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[tempEpIndex].devData.tempStateApp.upTemp % 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d",pni->epList[tempEpIndex].devData.tempStateApp.upTemp / 100);
			json_add_item_to_object(setting, "TU", json_create_string(strAll));

			//check low
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[tempEpIndex].devData.tempStateApp.lowTemp % 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d",pni->epList[tempEpIndex].devData.tempStateApp.lowTemp / 100);
			json_add_item_to_object(setting, "TL", json_create_string(strAll));
			json_add_item_to_object(setting, "TA", json_create_number(pni->epList[tempEpIndex].devData.tempStateApp.checkEnable));
			
		}


		//humility
		if(humiEpIndex != 0xff)
		{
			//check up
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[humiEpIndex].devData.humiStateApp.upHumi% 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d",pni->epList[humiEpIndex].devData.humiStateApp.upHumi / 100);
			json_add_item_to_object(setting, "HU", json_create_string(strAll));

			//check low
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[humiEpIndex].devData.humiStateApp.lowHumi % 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d",pni->epList[humiEpIndex].devData.humiStateApp.lowHumi / 100);
			json_add_item_to_object(setting, "HL", json_create_string(strAll));
			json_add_item_to_object(setting, "HA", json_create_number(pni->epList[humiEpIndex].devData.humiStateApp.checkEnable));
			
		}
		
	}else{
		//temperature
		if(tempEpIndex != 0xff)
		{
			if(devOption.THPSet.tempUp)
			{
				//check up
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[tempEpIndex].devData.tempStateApp.upTemp % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d",pni->epList[tempEpIndex].devData.tempStateApp.upTemp / 100);
				json_add_item_to_object(setting, "TU", json_create_string(strAll));
			}

			if(devOption.THPSet.tempLow)
			{
				//check low
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[tempEpIndex].devData.tempStateApp.lowTemp % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d",pni->epList[tempEpIndex].devData.tempStateApp.lowTemp / 100);
				json_add_item_to_object(setting, "TL", json_create_string(strAll));
			}

			if(devOption.THPSet.t_checkEn)
			{
				json_add_item_to_object(setting, "TA", json_create_number(pni->epList[tempEpIndex].devData.tempStateApp.checkEnable));
			}
		}


		//humility
		if(humiEpIndex != 0xff)
		{
			if(devOption.THPSet.humiUp)
			{
				//check up
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[humiEpIndex].devData.humiStateApp.upHumi% 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d",pni->epList[humiEpIndex].devData.humiStateApp.upHumi / 100);
				json_add_item_to_object(setting, "HU", json_create_string(strAll));
			}

			if(devOption.THPSet.humiLow)
			{
				//check low
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[humiEpIndex].devData.humiStateApp.lowHumi % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d",pni->epList[humiEpIndex].devData.humiStateApp.lowHumi / 100);
				json_add_item_to_object(setting, "HL", json_create_string(strAll));
			}

			if(devOption.THPSet.h_checkEn)
			{
				json_add_item_to_object(setting, "HA", json_create_number(pni->epList[humiEpIndex].devData.humiStateApp.checkEnable));
			}
		}
	}
	

	
	return setting;
}


json_t *userCreateTHPStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	char strZBMac[18];
	char strDot[5];
	char strAll[10];
	int16_t cpData;
	uint8_t i,tempEpIndex = 0xff;
	uint8_t humiEpIndex = 0xff;
	json_t *state = json_create_object();
	
	for(i = 0; i < pni->epNum; i ++)	//默认EP 1为temperature，2为humility
	{
		if(pni->epList[i].ep == 1)
		{
			tempEpIndex = i;
		}else if(pni->epList[i].ep == 2){
			humiEpIndex = i;
		}
	}

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		if(tempEpIndex != 0xff)
		{
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage % 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d.%s",pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage / 100,strDot);
			json_add_item_to_object(state, "TP", json_create_string(strAll));

		}

		if(humiEpIndex != 0xff)
		{
			memset(strDot,'\0',5);
			memset(strAll,'\0',10);
			cpData = pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage % 100;
			if(cpData < 10)
			{
				sprintf(strDot, "0%d", cpData);
			}else{
				sprintf(strDot, "%d", cpData);
			}
			sprintf(strAll,"%d.%s",pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage / 100,strDot);
			json_add_item_to_object(state, "HY", json_create_string(strAll));
		}
	}else{
		if(devOption.THPState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}
		if(devOption.THPState.batteryPercent)
		{
			json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		}
		if(devOption.THPState.batteryAlarm)
		{
			json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		}
		if(devOption.THPState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}
		if(devOption.THPState.temp)
		{
			if(tempEpIndex != 0xff)
			{
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d.%s",pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage / 100,strDot);
				json_add_item_to_object(state, "TP", json_create_string(strAll));

			}
		}
		if(devOption.THPState.humi)
		{
			if(humiEpIndex != 0xff)
			{
				memset(strDot,'\0',5);
				memset(strAll,'\0',10);
				cpData = pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d.%s",pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage / 100,strDot);
				json_add_item_to_object(state, "HY", json_create_string(strAll));
			}
		}
	}	
	return state;
}


json_t *userCreateIasSettingObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *setting = json_create_object();
	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(setting, "AS", json_create_number(pni->epList[0].devData.zoneStateApp.armMode));
		if(pni->devType == NODE_INFO_TYPE_VIBRATION)
		{
			json_add_item_to_object(setting, "ST", json_create_number(pni->epList[0].devData.zoneStateApp.sensitivityLevel));
		}
	}else{
		if(devOption.IASSet.armMode)
		{
			json_add_item_to_object(setting, "AS", json_create_number(pni->epList[0].devData.zoneStateApp.armMode));
		}

		if(devOption.IASSet.sensitivityLevel)
		{
			json_add_item_to_object(setting, "ST", json_create_number(pni->epList[0].devData.zoneStateApp.sensitivityLevel));
		}
	}
	return setting;
}
json_t *userCreateIasStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	char strZBMac[18];
	json_t *state = json_create_object();

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		if(pni->devType != NODE_INFO_TYPE_GAS_SENSOR)//AC供电设备不需要电量上报
		{
			json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		}
		if(pni->devType == NODE_INFO_TYPE_VIBRATION)
		{
			json_add_item_to_object(state, "ST", json_create_number(pni->epList[0].devData.zoneStateApp.sensitivityLevel));
		}
		json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.zoneStateApp.alarms));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
	}else{
		if(devOption.IASState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}
		if(devOption.IASState.batteryPercent)
		{
			json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		}
		if(pni->devType != NODE_INFO_TYPE_GAS_SENSOR)//AC供电设备不需要电量上报
		{
			if(devOption.IASState.batteryAlarm)
			{
				json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
			}
		}
		if(devOption.IASState.onoff)
		{
			json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.zoneStateApp.alarms));
		}
		if(devOption.IASState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}
		if(devOption.IASState.sensitivityLevel)
		{
			json_add_item_to_object(state, "ST", json_create_number(pni->epList[0].devData.zoneStateApp.sensitivityLevel));
		}
	}

	return state;
}

json_t *userCreateAceStaObject(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	char strZBMac[18];
	json_t *state = json_create_object();

	if(devOption.modeByte == 0xffffffff)
	{
		json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.aceStateApp.station));
		json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
	}else{
		if(devOption.ACEState.online)
		{
			json_add_item_to_object(state, "OL", json_create_number(pni->onLine));
		}
		if(devOption.ACEState.batteryPercent)
		{
			json_add_item_to_object(state, "BP", json_create_number(pni->batteryRemain));
		}
		if(devOption.ACEState.batteryAlarm)
		{
			json_add_item_to_object(state, "BA", json_create_number(pni->batteryAlarm));
		}
		if(devOption.ACEState.onoff)
		{
			json_add_item_to_object(state, "OF", json_create_number(pni->epList[0].devData.aceStateApp.station));
		}
		if(devOption.ACEState.time)
		{
			json_add_item_to_object(state, "TM", json_create_number(pni->epList[0].times));
		}
	}

	return state;
}

void writeGWStateRecordToCloud(char *SID,char *TEID,DEVINFOOPTION devOption)
{
	json_t *jsMain,*jsSta,*message,*jsSetting;
	T_SYSTEM_TIME cale_time;
	uint8_t reportValid = 1;
	char *sendData;
	char strTime[25];
	char strMac[15];
	memset(strMac,'\0',15);
	qca_calendar_get_time(&cale_time);
	sprintf(strTime,"%d-%d-%d %d:%d:%d",cale_time.year,cale_time.month,cale_time.day,cale_time.hour,cale_time.minute,cale_time.second);
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X", hmIotsDevInfo.wifiMac[0],hmIotsDevInfo.wifiMac[1],hmIotsDevInfo.wifiMac[2],
												hmIotsDevInfo.wifiMac[3],hmIotsDevInfo.wifiMac[4],hmIotsDevInfo.wifiMac[5]);
	jsMain = json_create_object();

	
	json_add_item_to_object(jsMain, "WT", json_create_number(HM_IOTS_SMALL_GW_TYPE));
	json_add_item_to_object(jsMain, "WM", json_create_string(strMac));
	json_add_item_to_object(jsMain, "TM", json_create_string(strTime));
	jsSta = userCreatGWStaObject(&hmIotsDevInfo,devOption);
	json_add_item_to_object(jsMain, "MG", jsSta);
	json_add_item_to_object(jsSta,"UD",json_create_string(SID));
	json_add_item_to_object(jsSta,"UN",json_create_string(TEID));
	json_add_item_to_object(jsMain,"MT",1);
	sendData = json_PrintUnformatted(jsMain);
	if(sendData == NULL)
	{
		printf("in gw report cloud func:malloc the mem was error\r\n");
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
		return;
	}
	
	
	userFillTheBufReportToCloud(sendData,strlen(sendData));
	if(sendData != NULL)
	{
		mem_free(sendData);
		sendData = NULL;
	}
	
	if(jsMain != NULL)
	{
		json_delete(jsMain);
		jsMain = NULL;
	}
	
}

void writeNodeStateRecordToCloud(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	uint8_t reportValid = 1;
	char strTime[25];
	char strMac[25];
	uint8_t sent_buff[512]={0};
	uint16_t sent_buff_len;
	uint8_t alarm_mode;
	uint8_t tempdata,alarms_data,alarnState,valuedata;
	uint32_t convertdata;
	

	sent_buff_len=0;
	tempdata=SUB_INDEX_KEY;
	memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
	big_little_exchange(pni->index,&convertdata);
	memcpy(&sent_buff[sent_buff_len],&convertdata,2);sent_buff_len+=2;
	alarm_mode=ALARM_ACT;
	
	switch(pni->devType)
	{
		case NODE_INFO_TYPE_LIGHT_RGB:
			//jsSta = userCreateRgbStaObject(pni,devOption);
			userCreateSta_rgb_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_AI1SW:
			
			userCreateSta_AI1SW_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_AI2SW:
			userCreateSta_AI1SW_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_AI3SW:
			//jsSta = userCreateAiSwitchStaObject(pni,devOption);
			userCreateSta_AI3SW_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_ZB_IR_TRANSPONDER:
			break;
		case NODE_INFO_TYPE_RELAY_CONTROL:
			break;
		case NODE_INFO_TYPE_WATER_SENSOR:
		case NODE_INFO_TYPE_GAS_SENSOR:
		case NODE_INFO_TYPE_SMOKE_SENSOR:
		case NODE_INFO_TYPE_CO_SENSOR:
		case NODE_INFO_TYPE_SOS:
			reportValid = 0;
			break;
		case NODE_INFO_TYPE_REMOTE_CONTROL:
			//jsSta = userCreateAceStaObject(pni,devOption);
			
			userCreateAceSta_actual_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_MAGNET_DOOR:
		case NODE_INFO_TYPE_PIR_SENSOR:
		case NODE_INFO_TYPE_VIBRATION:
			if(pni->epList[0].devData.zoneStateApp.alarms!=NULL)
			alarnState=pni->epList[0].devData.zoneStateApp.alarms;
			//jsSta = userCreateIasStaObject(pni,devOption);
			if(alarnState == TAMPER_AND_ALARM_RESUME || alarnState == TAMPER_AND_ALARM || alarnState == ALARM_RESUME || \
				alarnState == ALARM)
			{
			tempdata=REMOVE_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			alarms_data=(alarnState>>1)&0x01;
			memcpy(&sent_buff[sent_buff_len],&alarms_data,1);sent_buff_len+=1;
			tempdata=OP_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			valuedata=(alarnState)&0x01;
			memcpy(&sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
			}
			break;
		case NODE_INFO_TYPE_TEMP_HUM_SENSOR:
			//jsSta = userCreateTHPStaObject(pni,devOption);
			userCreateIasSta_TEMPHUM_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_WARNING_DEV:
			//jsSta = userCreateWDStaObject(pni,devOption);
			userCreateIasSta_actual_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_ILLUMINATION:
			break;
		case NODE_INFO_TYPE_AIR_QUALITY:
			break;
		case NODE_INFO_TYPE_THERMOSTAT:
			break;
		case NODE_INFO_TYPE_AI_SOCKET_USB:
			//jsSta = userCreatePlugStaObject(pni,devOption);
			userCreateSta_SOCKETUSB_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
			//jsSta = userCreateSPlugStaObject(pni,devOption);
			userCreateSta_SOCKETMEASURE_pointdata(sent_buff,&sent_buff_len,pni,devOption);
			break;
		case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
			//jsSta = userCreateCutRgbStaObject(pni,devOption);
			userCreateSta_CUTRGB_pointdata(sent_buff,&sent_buff_len,pni,devOption);
		default:
			reportValid = 0;
			break;
	}
	userFillTheBufReportToCloud(sent_buff,sent_buff_len);
}

void writeNodeStateRecordToCloud_old(HM_DEV_RAM_LIST *pni,DEVINFOOPTION devOption)
{
	json_t *jsMain,*jsSta,*message,*jsSetting,*jsMg;
	uint8_t reportValid = 1;
	char *sendData;
	char strTime[25];
	char strMac[25];
	memset(strMac,'\0',25);
	memset(strTime,'\0',25);

	if(gwStateForRealTime.sysRealTimerValid == 1)
	{
		sprintf(strTime,"%d-%d-%d %d:%d:%d",gwStateForRealTime.sysRealTime.year,gwStateForRealTime.sysRealTime.month,gwStateForRealTime.sysRealTime.day,gwStateForRealTime.sysRealTime.hour,gwStateForRealTime.sysRealTime.minute,gwStateForRealTime.sysRealTime.second);
	}
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X", hmIotsDevInfo.wifiMac[0],hmIotsDevInfo.wifiMac[1],hmIotsDevInfo.wifiMac[2],
												hmIotsDevInfo.wifiMac[3],hmIotsDevInfo.wifiMac[4],hmIotsDevInfo.wifiMac[5]);
	jsMain = json_create_object();


	json_add_item_to_object(jsMain, "WT", json_create_number(HM_IOTS_SMALL_GW_TYPE));
	json_add_item_to_object(jsMain, "WM", json_create_string(strMac));
	json_add_item_to_object(jsMain, "TM", json_create_string(strTime));

	json_add_item_to_object(jsMain, "MG", jsMg = json_create_object());
	memset(strMac,'\0',25);
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X%02X%02X",pni->mac[0],pni->mac[1],pni->mac[2],pni->mac[3],
																pni->mac[4],pni->mac[5],pni->mac[6],pni->mac[7]);
	json_add_item_to_object(jsMg, "SM", json_create_string(strMac));
	json_add_item_to_object(jsMg, "ST", json_create_number(pni->devType));

	switch(pni->devType)
	{
		case NODE_INFO_TYPE_LIGHT_RGB:
			jsSta = userCreateRgbStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_AI1SW:
		case NODE_INFO_TYPE_AI2SW:
		case NODE_INFO_TYPE_AI3SW:
			jsSta = userCreateAiSwitchStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_ZB_IR_TRANSPONDER:
			break;
		case NODE_INFO_TYPE_RELAY_CONTROL:
			break;
		case NODE_INFO_TYPE_WATER_SENSOR:
		case NODE_INFO_TYPE_GAS_SENSOR:
		case NODE_INFO_TYPE_SMOKE_SENSOR:
		case NODE_INFO_TYPE_CO_SENSOR:
		case NODE_INFO_TYPE_SOS:
			reportValid = 0;
			break;
		case NODE_INFO_TYPE_REMOTE_CONTROL:
			jsSta = userCreateAceStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_MAGNET_DOOR:
		case NODE_INFO_TYPE_PIR_SENSOR:
		case NODE_INFO_TYPE_VIBRATION:
			jsSta = userCreateIasStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_TEMP_HUM_SENSOR:
			jsSta = userCreateTHPStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_WARNING_DEV:
			jsSta = userCreateWDStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_ILLUMINATION:
			break;
		case NODE_INFO_TYPE_AIR_QUALITY:
			break;
		case NODE_INFO_TYPE_THERMOSTAT:
			break;
		case NODE_INFO_TYPE_AI_SOCKET_USB:
			jsSta = userCreatePlugStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
			jsSta = userCreateSPlugStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
			jsSta = userCreateCutRgbStaObject(pni,devOption);
		default:
			reportValid = 0;
			break;
	}
	if((reportValid == 0) || (jsSta == NULL))
	{
		printf("in report cloud func:invalid data type\r\n");
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
		return;
	}

	json_add_item_to_object(jsMg, "SS", jsSta);
	json_add_item_to_object(jsMain,"MT",json_create_number(0));
	sendData = json_PrintUnformatted(jsMain);
	if(sendData == NULL)
	{
		printf("in report cloud func:malloc the mem was error\r\n");
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
		return;
	}
	

	userFillTheBufReportToCloud(sendData,strlen(sendData));
	if(sendData != NULL)
	{
		mem_free(sendData);
		sendData = NULL;
	}
	
	if(jsMain != NULL)
	{
		json_delete(jsMain);
		jsMain = NULL;
	}
	
	
}




/******************************************************************
app控制设备云端记录上报；
devmode 用于区分是控制的zb设备还是GW；
当控制的设备为zb设备时，index才有效；
******************************************************************/
void writeUserControlDevStateToCloud(uint8_t devMode,uint16_t index,DEVINFOOPTION devOption,char* sid,char*teid)
{
	json_t *jsMain,*jsMg,*jsSe;
	uint8_t reportValid = 1;
	char *sendData;
	char strTime[25];
	char strMac[25];
	HM_DEV_RAM_LIST *pni;
	memset(strTime,'\0',25);
	memset(strMac,'\0',25);
	if(gwStateForRealTime.sysRealTimerValid == 1)
	{
		sprintf(strTime,"%d-%d-%d %d:%d:%d",gwStateForRealTime.sysRealTime.year,gwStateForRealTime.sysRealTime.month,gwStateForRealTime.sysRealTime.day,gwStateForRealTime.sysRealTime.hour,gwStateForRealTime.sysRealTime.minute,gwStateForRealTime.sysRealTime.second);
	}
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X", hmIotsDevInfo.wifiMac[0],hmIotsDevInfo.wifiMac[1],hmIotsDevInfo.wifiMac[2],
												hmIotsDevInfo.wifiMac[3],hmIotsDevInfo.wifiMac[4],hmIotsDevInfo.wifiMac[5]);
	jsMain = json_create_object();

	
	json_add_item_to_object(jsMain, "WT", json_create_number(HM_IOTS_SMALL_GW_TYPE));
	json_add_item_to_object(jsMain, "WM", json_create_string(strMac));
	json_add_item_to_object(jsMain, "TM", json_create_string(strTime));


	json_add_item_to_object(jsMain, "MG", jsMg = json_create_object());
	json_add_item_to_object(jsMain,"MT",json_create_number(1));

	json_add_item_to_object(jsMg, "UD", json_create_string(teid));
	json_add_item_to_object(jsMg, "UN", json_create_string(sid));
	

	if(USER_CONRTOL_DEV_TYPE_ZB == devMode)
	{
		pni = findInDevListByIndex(index);
		if(pni == NULL)
		{
			printf("the index was not in dev list\r\n");
			json_add_item_to_object(jsMg, "SM", json_create_string("invalid index"));
		}else{
			memset(strMac,'\0',25);
			sprintf(strMac,"%02X%02X%02X%02X%02X%02X%02X%02X",pni->mac[0],pni->mac[1],pni->mac[2],pni->mac[3],
																		pni->mac[4],pni->mac[5],pni->mac[6],pni->mac[7]);
			json_add_item_to_object(jsMg, "SM", json_create_string(strMac));
			json_add_item_to_object(jsMg, "ST", json_create_number(pni->devType));
			switch(pni->devType)
			{
				case NODE_INFO_TYPE_LIGHT_RGB:
					jsSe = userCreateRgbSettingObject(pni,devOption);
					break;
				case NODE_INFO_TYPE_AI1SW:
				case NODE_INFO_TYPE_AI2SW:
				case NODE_INFO_TYPE_AI3SW:
					jsSe = userCreateAiSwitchSettingObject(pni,devOption);
					break;
				case NODE_INFO_TYPE_ZB_IR_TRANSPONDER:
					break;
				case NODE_INFO_TYPE_RELAY_CONTROL:
					break;
				case NODE_INFO_TYPE_PIR_SENSOR:
				case NODE_INFO_TYPE_MAGNET_DOOR:
				case NODE_INFO_TYPE_VIBRATION:
					jsSe = userCreateIasSettingObject(pni,devOption);
					break;
				case NODE_INFO_TYPE_WATER_SENSOR:
				case NODE_INFO_TYPE_GAS_SENSOR:
				case NODE_INFO_TYPE_SMOKE_SENSOR:
				case NODE_INFO_TYPE_CO_SENSOR:
				case NODE_INFO_TYPE_REMOTE_CONTROL:
				case NODE_INFO_TYPE_SOS:
					break;
				case NODE_INFO_TYPE_TEMP_HUM_SENSOR:
					jsSe = userCreateTHPSettingObject(pni,devOption);
					break;
				
				case NODE_INFO_TYPE_WARNING_DEV:
					jsSe = userCreateWDSettingObject(pni,devOption);
					break;

				case NODE_INFO_TYPE_ILLUMINATION:
					break;
				case NODE_INFO_TYPE_AIR_QUALITY:
					break;
				case NODE_INFO_TYPE_THERMOSTAT:
					break;
				case NODE_INFO_TYPE_AI_SOCKET_USB:
					jsSe = userCreatePlugSettingObject(pni,devOption);
					break;
				case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
					jsSe = userCreateSPlugSettingObject(pni,devOption);
					break;
				case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
					jsSe = userCreateCutRgbSettingObject(pni,devOption);
					break;					
				default:
					break;
			}
		}
	}else if(USER_CONTROL_DEV_TYPE_GW == devMode){
		jsSe = userCreatGWStaObject(&hmIotsDevInfo,devOption);
	}
	if(jsSe == NULL)
	{
		json_add_item_to_object(jsMg, "SE", jsSe = json_create_object());
	}else{
		json_add_item_to_object(jsMg, "SE", jsSe);
	}
	
	sendData = json_PrintUnformatted(jsMain);
	if(sendData == NULL)
	{
		printf("in user control cloud func:malloc the mem was error\r\n");
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
		return;
	}
	
	
	userFillTheBufReportToCloud(sendData,strlen(sendData));
	if(sendData != NULL)
	{
		mem_free(sendData);
		sendData = NULL;
	}
	
	if(jsMain != NULL)
	{
		json_delete(jsMain);
		jsMain = NULL;
	}
}


/******************************************************************
app控制设备云端记录上报；
app控制场景执行的上报
******************************************************************/
void writeUserControlSceneStateToCloud(uint8_t sceneId,uint8_t exeSta,char * sid,char *teid)
{
	json_t *jsMain,*jsMg,*jsSe;
	uint8_t reportValid = 1;
	char *sendData;
	char strTime[25];
	char strMac[25];
	HM_GW_LIST *pgw_info=&gw_info;
	uint8_t tmIndex = sceneId % 10;
	memset(strTime,'\0',25);
	memset(strMac,'\0',25);
	printf("scene control save to cloude\r\n");
	if(gwStateForRealTime.sysRealTimerValid == 1)
	{
		sprintf(strTime,"%d-%d-%d %d:%d:%d",gwStateForRealTime.sysRealTime.year,gwStateForRealTime.sysRealTime.month,gwStateForRealTime.sysRealTime.day,gwStateForRealTime.sysRealTime.hour,gwStateForRealTime.sysRealTime.minute,gwStateForRealTime.sysRealTime.second);
	}
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X", hmIotsDevInfo.wifiMac[0],hmIotsDevInfo.wifiMac[1],hmIotsDevInfo.wifiMac[2],
												hmIotsDevInfo.wifiMac[3],hmIotsDevInfo.wifiMac[4],hmIotsDevInfo.wifiMac[5]);
	jsMain = json_create_object();

	
	json_add_item_to_object(jsMain, "WT", json_create_number(HM_IOTS_SMALL_GW_TYPE));
	json_add_item_to_object(jsMain, "WM", json_create_string(strMac));
	json_add_item_to_object(jsMain, "TM", json_create_string(strTime));


	json_add_item_to_object(jsMain, "MG", jsMg = json_create_object());
	json_add_item_to_object(jsMain,"MT",json_create_number(2));

	json_add_item_to_object(jsMg, "UD", json_create_string(teid));
	json_add_item_to_object(jsMg, "UN", json_create_string(sid));
	json_add_item_to_object(jsMg, "IS", json_create_number(exeSta));
	json_add_item_to_object(jsMg, "SD", json_create_number(sceneId));
	json_add_item_to_object(jsMg, "SN", json_create_string(pgw_info->scene_list[tmIndex].sceneName));
	
	sendData = json_PrintUnformatted(jsMain);
	if(sendData == NULL)
	{
		printf("in user control cloud func:malloc the mem was error\r\n");
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
		return;
	}
	
	
	userFillTheBufReportToCloud(sendData,strlen(sendData));
	if(sendData != NULL)
	{
		mem_free(sendData);
		sendData = NULL;
	}
	
	if(jsMain != NULL)
	{
		json_delete(jsMain);
		jsMain = NULL;
	}
}


/*void userUpdateDevListBasicToApp(char* teid,char* sid,uint32_t sn)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	uint8_t i;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	uint8_t devNum = devNumInfo.devNum;
	uint8_t packageCount = 0;
	uint8_t packageNum = devNum / 8 + 1;
	json_t *jsOb = NULL;
	if (pni==NULL)
	{
		printf("the ram is empty\r\n");
		userCreateJsplToBuf(jsOb,1,MESSAGE_RSP_GETINFO,sid,teid,sn,1);
		return;
	}
	do{
		if (pni_tmp->index == index)
			return pni_tmp;
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);

	void userCreateJsplToBuf(json_t* json_newPL,int8_t rc,uint16_t cid,char *sid,char *teid,uint32_t sn,uint8_t security)
}

void userUpdateDevListStateToApp(char* teid,char* sid,uint32_t sn)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	uint8_t i;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	uint8_t devNum = devNumInfo.devNum;
	uint8_t packageCount = 0;
	uint8_t packageNum = devNum / 8 + 1;
	json_t *jsOb = NULL;
	if (pni==NULL)
	{
		printf("the ram is empty\r\n");
		userCreateJsplToBuf(jsOb,1,MESSAGE_RSP_GETINFO,sid,teid,sn,1);
		return;
	}
	do{
		if (pni_tmp->index == index)
			return pni_tmp;
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);

	void userCreateJsplToBuf(json_t* json_newPL,int8_t rc,uint16_t cid,char *sid,char *teid,uint32_t sn,uint8_t security)
}*/



/**************************************************
writeNodeStateAlarnToCloud: 设备报警推送写缓存，
alarnState为报警类型，具体需要在调用该函数
前判断该类型，然后输入，类型为ALARMTYPE
**************************************************/
void writeNodeStateAlarnToCloud(HM_DEV_RAM_LIST *pni,uint8_t alarnState)
{
	json_t *jsMainAn,*jsStaAn,*jsArrayAn;
	json_t *jsMainIos,*jsStaIos,*jsArrayIos,*jsArgIos;
	uint8_t reportValid = 1;
	char *iosData;
	char *androidData;
	char strTime[25];
	char strMac[15];
	char str[30];
	char strDot[5];
	char strAll[10];
	int16_t cpData;
	uint8_t i,tempEpIndex = 0xff;
	uint8_t humiEpIndex = 0xff,tempdata,valuedata;
	char sent_buff[512]={0};
	uint16_t sent_buff_len=0;
	uint32_t convertdata;
	uint8_t tamper_data,alarm_mode;

	memset(strTime,'\0',25);
	memset(strMac,'\0',15);
	if(gwStateForRealTime.sysRealTimerValid == 1)
	{
		sprintf(strTime,"%d-%d-%d %02d:%02d:%02d",gwStateForRealTime.sysRealTime.year,gwStateForRealTime.sysRealTime.month,gwStateForRealTime.sysRealTime.day,gwStateForRealTime.sysRealTime.hour,gwStateForRealTime.sysRealTime.minute,gwStateForRealTime.sysRealTime.second);
	}
	sprintf(strMac,"%02X%02X%02X%02X%02X%02X%02X%02X", pni->mac[0],pni->mac[1],pni->mac[2],pni->mac[3],
														pni->mac[4],pni->mac[5],pni->mac[6],pni->mac[7]);
	//jsMainIos = json_create_object();
	//jsMainAn = json_create_object();

	memset(str,'\0',30);
	memset(strDot,'\0',5);
	memset(strAll,'\0',10);
	//ios
	/*
	json_add_item_to_object(jsMainIos, "aps", jsStaIos = json_create_object());
	json_add_item_to_object(jsStaIos, "alert", jsArgIos = json_create_object());
	json_add_item_to_object(jsArgIos, "loc-args", jsArrayIos = json_create_array());
	json_add_item_toarray(jsArrayIos,json_create_string(strTime));
	json_add_item_toarray(jsArrayIos,json_create_string(pni->name));
	json_add_item_toarray(jsArrayIos,json_create_string(strMac));
	

	//android 	
	json_add_item_to_object(jsMainAn, "notification", jsStaAn = json_create_object());
	json_add_item_to_object(jsStaAn, "title", json_create_string(pni->name));
	json_add_item_to_object(jsStaAn, "body_loc_args", jsArrayAn = json_create_array());
	json_add_item_toarray(jsArrayAn,json_create_string(strTime));
	json_add_item_toarray(jsArrayAn,json_create_string(strMac));*/

	tempdata=SUB_INDEX_KEY;
	memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
	big_little_exchange(pni->index,&convertdata);
	memcpy(&sent_buff[sent_buff_len],&convertdata,2);sent_buff_len+=2;
	alarm_mode=ALARM_ACT;
	switch(pni->devType)
	{
		case NODE_INFO_TYPE_LIGHT_RGB:
			 if(SUB_DEV_OFFLINE == alarnState)//
			 {
				sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
				tempdata=OL_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_AI1SW:
		case NODE_INFO_TYPE_AI2SW:
		case NODE_INFO_TYPE_AI3SW:
			 if(SUB_DEV_OFFLINE == alarnState)
			 {
				sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
				tempdata=OL_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_ZB_IR_TRANSPONDER:
			break;
		case NODE_INFO_TYPE_RELAY_CONTROL:
			break;
		case NODE_INFO_TYPE_MAGNET_DOOR:
		case NODE_INFO_TYPE_WATER_SENSOR:
		case NODE_INFO_TYPE_PIR_SENSOR:
		case NODE_INFO_TYPE_VIBRATION:
			
			if(alarnState == TAMPER_AND_ALARM_RESUME || alarnState == TAMPER_AND_ALARM || alarnState == ALARM_RESUME || \
				alarnState == ALARM)
			{
			tempdata=REMOVE_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			tamper_data=(alarnState>>1)&0x01; //1
			if(tamper_data)
				printf("\n ----------report_zb_alarmMsg REMOVE_KEY=== 1\r\n");
			//alarm_data=alarm_data>>1;
			memcpy(&sent_buff[sent_buff_len],&tamper_data,1);sent_buff_len+=1;
			tempdata=OP_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			valuedata=(alarnState)&0x01;
			if(valuedata)
				printf("\n ----------report_zb_alarmMsg alarm_data=== 01\r\n");
			memcpy(&sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
			}
			if(TAMPER_AND_ALARM_RESUME == alarnState)
			{
				alarm_mode=EVENT;
				sprintf(str,"%s%d",TAMPER_ALARM_RESUME_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
				
				
			}else if(TAMPER_AND_ALARM == alarnState){
				sprintf(str,"%s%d",TAMPER_ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
				
				
			}else if(ALARM_RESUME == alarnState)
			{
				alarm_mode=EVENT;
				sprintf(str,"%s%d",ALARM_RESUME_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS

				
			}else if(ALARM == alarnState){
				sprintf(str,"%s%d",ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS

				
			}else if(BATTERY_LOW_ALARM == alarnState){
				sprintf(str,"%s%d",BATTERY_LOW_ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS

				tempdata=BA_KEY;
				memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=1;
				memcpy(&sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
			}else if(SUB_DEV_OFFLINE == alarnState){
				sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS

				tempdata=OL_KEY;
				memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(&sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
			}
			//android 
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));			
			//ios
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_SMOKE_SENSOR:
		case NODE_INFO_TYPE_GAS_SENSOR:
		case NODE_INFO_TYPE_CO_SENSOR:
		case NODE_INFO_TYPE_SOS://-----------------------------------sos
			if(alarnState == ALARM_RESUME || alarnState == ALARM )
			{
				tempdata=OP_KEY;
				memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=(alarnState)&0x01;
				if(valuedata)
					printf("\n ----------report_zb_alarmMsg alarm_data=== 01\r\n");
				memcpy(&sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
			}
			if(ALARM_RESUME == alarnState)
			{
				alarm_mode=EVENT;
				sprintf(str,"%s%d",ALARM_RESUME_STRING,pni->devType);
								//android 
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));

				//ios
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));
				printf("ias dev ALARM RESUME---------------\r\n");

				//tempdata=OP_KEY;
				//memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				//valuedata=0;
				//memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}else if(ALARM == alarnState){
				sprintf(str,"%s%d",ALARM_STRING,pni->devType);
				//android
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));

				//ios
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));
				printf("ias dev ALARM  ---------------\r\n");

				//tempdata=OP_KEY;
				//memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				//valuedata=1;
				//memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}else if(BATTERY_LOW_ALARM == alarnState){
				//sprintf(str,"%s%d",BATTERY_LOW_ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS

				tempdata=BA_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=1;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}else if(SUB_DEV_OFFLINE == alarnState){
				//sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS

				tempdata=OL_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
			}
			//android
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));

			//ios
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));

			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_TEMP_HUM_SENSOR:
			for(i = 0; i < pni->epNum; i ++)	//默认EP 1为temperature，2为humility
			{
				if(pni->epList[i].ep == 1)
				{
					tempEpIndex = i;
				}else if(pni->epList[i].ep == 2){
					humiEpIndex = i;
				}
			}
			if(tempEpIndex != 0xff)
			{
				cpData = pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d.%s",pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage / 100,strDot);
				if(TEMP_UP_ALARM == alarnState){
					tempdata=TEMPBA_KEY;
					memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
					valuedata=1;
					memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					tempdata=TP_KEY;
					memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
					memcpy(sent_buff[sent_buff_len],&pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage,1);sent_buff_len+=1;
				
					
					sprintf(str,"%s%d",TEMP_UP_ALARM_STRING,pni->devType);
					//android
					//json_add_item_toarray(jsArrayAn,json_create_string(strAll));
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));


					//ios
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));
					//ios
					//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				}else if(TEMP_LOW_ALARM == alarnState){
					tempdata=TEMPBA_KEY;
					memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
					valuedata=0;
					memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					tempdata=TP_KEY;
					memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
					memcpy(sent_buff[sent_buff_len],&pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage,2);sent_buff_len+=2;
				
				
					sprintf(str,"%s%d",TEMP_LOW_ALARM_STRING,pni->devType);
					//android
					//json_add_item_toarray(jsArrayAn,json_create_string(strAll));
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));
					////ios
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));
					//ios
					//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				}else if(BATTERY_LOW_ALARM == alarnState){
					tempdata=BA_KEY;
					memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
					valuedata=1;
					memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					
					sprintf(str,"%s%d",BATTERY_LOW_ALARM_STRING,pni->devType);
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
				}else if(SUB_DEV_OFFLINE == alarnState){
					tempdata=OL_KEY;
					memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
					valuedata=1;
					memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					
					//sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
				}
			}
			if(humiEpIndex != 0xff)
			{
				cpData = pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage % 100;
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				sprintf(strAll,"%d.%s",pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage / 100,strDot);
				if(HUMI_UP_ALARM == alarnState){
					tempdata=HUMPBA_KEY;
					memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
					valuedata=1;
					memcpy(sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
					tempdata=HU_KEY;
					memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
					memcpy(sent_buff[sent_buff_len],&pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage,2);sent_buff_len+=2;
				
					sprintf(str,"%s%d",HUMI_UP_ALARM_STRING,pni->devType);
					//android
					//json_add_item_toarray(jsArrayAn,json_create_string(strAll));
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));


					//ios
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));
					//ios
					//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				}else if(HUMI_LOW_ALARM == alarnState){
					tempdata=HUMPBA_KEY;
					memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
					valuedata=0;
					memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					tempdata=HU_KEY;
					memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
					memcpy(sent_buff[sent_buff_len],&pni->epList[humiEpIndex].devData.humiStateApp.humiMeterage,2);
					sent_buff_len+=2;			
					
					sprintf(str,"%s%d",HUMI_LOW_ALARM_STRING,pni->devType);
					//android
					//json_add_item_toarray(jsArrayAn,json_create_string(strAll));
					//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));
					//ios
					//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));
					//ios
					//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				}
			}
			//android
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_WARNING_DEV:
			 if(SUB_DEV_OFFLINE == alarnState)
			 {
			 	tempdata=OL_KEY;
				memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
				
				//sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
			}else if(BATTERY_LOW_ALARM == alarnState){
				tempdata=BA_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=1;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
					
				//sprintf(str,"%s%d",BATTERY_LOW_ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}
			//android 
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));			
			//ios
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		
		case NODE_INFO_TYPE_ILLUMINATION:
			break;
		case NODE_INFO_TYPE_AIR_QUALITY:
			break;
		case NODE_INFO_TYPE_THERMOSTAT:
			break;
		

			break;
		case NODE_INFO_TYPE_REMOTE_CONTROL:
			if(ALARM == alarnState)
			{
				tempdata=OP_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=3;//valuedata=1;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
				
				//sprintf(str,"%s%d",ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}else if(BATTERY_LOW_ALARM == alarnState){
				tempdata=BA_KEY;
				memcpy(sent_buff+sent_buff_len,&tempdata,1);sent_buff_len+=1;
				valuedata=1;
				memcpy(sent_buff+sent_buff_len,&valuedata,1);sent_buff_len+=1;
				
				//sprintf(str,"%s%d",BATTERY_LOW_ALARM_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}else if(SUB_DEV_OFFLINE == alarnState){
				tempdata=OL_KEY;
				memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
				
				//sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
			}
			//android 
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));

			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));

			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_AI_SOCKET_USB:
			 if(SUB_DEV_OFFLINE == alarnState)
			 {
				tempdata=OL_KEY;
				memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
				
				//sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS
			}
			//json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));
			//json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
			if(POWER_W_OVER_ALARM == alarnState)
			{
				cpData = pni->epList[0].devData.splugStateApp.InstantaneousDemand % 10;
		
				//sprintf(strDot, "%d", cpData);
				//sprintf(strAll,"%d.%s",pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage / 10,strDot);
				//android
				//json_add_item_toarray(jsArrayAn,json_create_string(strAll));

				//ios
				//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				//sprintf(str,"%s%d",POWER_W_UP_STRING,pni->devType);


			//	json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
			//	json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}else if(VOLTAGE_OVER_ALARM == alarnState){
				cpData = pni->epList[0].devData.splugStateApp.rmsVoltage % 100;
		
				if(cpData < 10)
				{
					sprintf(strDot, "0%d", cpData);
				}else{
					sprintf(strDot, "%d", cpData);
				}
				//sprintf(strAll,"%d.%s",pni->epList[tempEpIndex].devData.tempStateApp.tempMeterage / 100,strDot);
				//android
				//json_add_item_toarray(jsArrayAn,json_create_string(strAll));

				//ios
				//json_add_item_toarray(jsArrayIos,json_create_string(strAll));
				//sprintf(str,"%s%d",VOLTAGE_UP_STRING,pni->devType);

				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}else if(CURRENT_OVER_ALARM == alarnState){
				//sprintf(str,"%s%d",CURRENT_UP_STRING,pni->devType);
				//json_add_item_to_object(jsStaAn, "sound",json_create_string("alarm.mp3"));//android
				//json_add_item_to_object(jsStaIos, "sound", json_create_string("alarm.avf"));//IOS
			}else if(SUB_DEV_OFFLINE == alarnState){
				tempdata=OL_KEY;
				memcpy(sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
				valuedata=0;
				memcpy(sent_buff[sent_buff_len],&valuedata,1);sent_buff_len+=1;
				
				/*sprintf(str,"%s%d",SUB_DEV_OFFLINE_STRING,pni->devType);
				json_add_item_to_object(jsStaAn, "sound",json_create_string("message.mp3"));//android
				json_add_item_to_object(jsStaIos, "sound", json_create_string("message.avf"));//IOS */
			}
			//android 
			/*json_add_item_to_object(jsStaAn, "body_loc_key", json_create_string(str));
			json_add_item_to_object(jsArgIos, "loc-key", json_create_string(str));*/
			tempdata=TM_KEY;
			memcpy(&sent_buff[sent_buff_len],&tempdata,1);sent_buff_len+=1;
			Int_biglittle_Convert(pni->epList[0].times,&convertdata);
			memcpy(&sent_buff[sent_buff_len],&convertdata,4);sent_buff_len+=4;
			break;
		default:
			return;
	}
	




	/*iosData = json_PrintUnformatted(jsMainIos);
	if(iosData == NULL)
	{
		printf("in alarm cloud func:malloc the mem was error\r\n");
		if(jsMainIos != NULL)
		{
			json_delete(jsMainIos);
			jsMainIos = NULL;
		}
		return;
	}*/

	/*androidData = json_PrintUnformatted(jsMainAn);
	if(androidData == NULL)
	{
		printf("in alarm cloud func:malloc the mem was error\r\n");
		if(jsMainAn != NULL)
		{
			json_delete(jsMainAn);
			jsMainAn = NULL;
		}
		
		if(jsMainIos != NULL)
		{
			json_delete(jsMainIos);
			jsMainIos = NULL;
		}

		if(iosData != NULL)
		{
			mem_free(iosData);
			iosData = NULL;
		}
		return;
	}*/
	
	
	//userFillTheBufAlarmToCloud(iosData,strlen(iosData),androidData,strlen(androidData));
	//old
	//userAlarmMsgData_write(sent_buff,&sent_buff_len,pni,alarnState);
	//printf("in alarm cloud userFillTheBufAlarmToCloud\r\n");
	//userFillTheBufAlarmToCloud(sent_buff,sent_buff_len,sent_buff,sent_buff_len);//hardy add
	//old
	printf("in alarm cloud userFillTheBufAlarmToCloud %x\r\n",sent_buff_len);
	userFillTheBufAlarmToCloud(alarm_mode,sent_buff,sent_buff_len,sent_buff,sent_buff_len);
	
	/*if(iosData != NULL)
	{
		mem_free(iosData);
		iosData = NULL;
	}
	
	if(jsMainIos != NULL)
	{
		json_delete(jsMainIos);
		jsMainIos = NULL;
	}

	if(androidData != NULL)
	{
		mem_free(androidData);
		androidData = NULL;
	}
	
	if(jsMainAn != NULL)
	{
		json_delete(jsMainAn);
		jsMainAn = NULL;
	}*/
}


char userForXlinkGetAllDataPoint(char* inData,uint8_t dataMode,char* outData,uint16_t *outDataLen)
{
	REPORTTOCLOUD recordP;
	ALARMTOCLOUD alarmP;	
	int i = 0;
	if((inData == NULL) || (outData == NULL))
	{
		printf("in get data point:the data point was null\r\n");
		return -1;
	}
	if(dataMode == DEV_DATA_FOR_RECORD_TO_CLOUD)//record
	{
		recordP = *((REPORTTOCLOUD*)inData);
		outData[i++] = 2;
		outData[i++] = DP_STRING;
		outData[i++] = recordP.dataLen;
		memcpy(&outData[i],recordP.data,recordP.dataLen);
		i += recordP.dataLen;
		*outDataLen = i;
	}else if(dataMode == DEV_DATA_FOR_ALARM_TO_CLOUD){//alarm
		alarmP = *((ALARMTOCLOUD*)inData);
		//ios
		outData[i++] = 0;
		outData[i++] = DP_STRING;
		outData[i++] = alarmP.iosLen;
		memcpy(&outData[i],alarmP.iosBuf,alarmP.iosLen);
		i += alarmP.iosLen;

		//android
		outData[i++] = 1;
		outData[i++] = DP_STRING;
		outData[i++] = alarmP.androidLen;
		memcpy(&outData[i],alarmP.androidBuf,alarmP.androidLen);
		i += alarmP.androidLen;
		*outDataLen = i;
	}
}
void userUpdateDevDataToCloud(void)
{
	uint8_t i;
	for(i = 0; i < MAX_ALARM_CLOUD_BUF_NUM; i ++)
	{
		if(alarmBuf[i].validFlag == 1)
		{
			printf("alarm to cloud\r\n");
			userForXlinkGetAllDataPoint((char*)&alarmBuf[i],DEV_DATA_FOR_ALARM_TO_CLOUD,alarm_report_data,&alarm_report_len);
			//Xlink_UpdateDataPoint(alarm_report_data,alarm_report_len,0);//0 开启推送，1 关闭推送
			memset((char*)&alarmBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}

	for(i = 0; i < MAX_REPORT_CLOUD_BUF_NUM; i ++)
	{
		if(cloudBuf[i].validFlag == 1)
		{
			printf("record to cloud\r\n");
			userForXlinkGetAllDataPoint((char*)&cloudBuf[i],DEV_DATA_FOR_RECORD_TO_CLOUD,alarm_report_data,&alarm_report_len);
			//Xlink_UpdateDataPoint(alarm_report_data,alarm_report_len,1);//0 开启推送，1 关闭推送
			memset((char*)&cloudBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}
}

json_t* userReadHomeArmForDevList(void)
{
	json_t *js_obj,*js_ds;
	uint8_t i;
	int ret = 0;
	uint16_t tempIndex;
	HM_DEV_RAM_LIST *pni_temp;
	js_obj = json_create_object();
	json_add_item_to_object(js_obj, "DS", js_ds = json_create_array());
	if(hmIotsDevInfo.home_arm_for_dev.indexNum > 0)
	{
		for(i = 0; i < MAX_HOME_ARM_FOR_SENSOR_NUM; i ++)
		{
			tempIndex = hmIotsDevInfo.home_arm_for_dev.index[i];
			if(tempIndex != 0)
			{
				pni_temp = findInDevListByIndex(tempIndex);
				if((pni_temp != NULL) && ((pni_temp->devType == NODE_INFO_TYPE_MAGNET_DOOR) || (pni_temp->devType == NODE_INFO_TYPE_PIR_SENSOR) ||(pni_temp->devType == NODE_INFO_TYPE_VIBRATION)))
				{
					json_add_item_toarray(js_ds,json_create_number(hmIotsDevInfo.home_arm_for_dev.index[i]));
				}else{
					hmIotsDevInfo.home_arm_for_dev.index[i] = 0;
					hmIotsDevInfo.home_arm_for_dev.indexNum --;
				}
			}
		}
	}
	printf("the valid home arm dev num:%d\r\n",hmIotsDevInfo.home_arm_for_dev.indexNum);
	return js_obj;
	
}

json_t* userReadArmStateAndTimerFunc(void)
{
	json_t *js_obj,*js_am,*js_ts,*js_child;
	uint8_t i;
	int ret = 0;
	js_obj = json_create_object();
	json_add_item_to_object(js_obj, "AT",json_create_number(hmIotsDevInfo.sensor_arm_type));
	json_add_item_to_object(js_obj, "AM", js_am = json_create_array());

	js_child = json_create_object();
	json_add_item_to_object(js_child, "AS", json_create_number(SENSOR_ARM));
	json_add_item_to_object(js_child, "WF", json_create_number(hmIotsDevInfo.sensor_arm_timer.armTimer.wkflag));
	json_add_item_to_object(js_child, "TS", js_ts = json_create_array());
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.armTimer.month));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.armTimer.day));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.armTimer.hour));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.armTimer.min));
	json_add_item_toarray(js_am,js_child);

	js_child = json_create_object();
	json_add_item_to_object(js_child, "AS", json_create_number(SENSOR_DISARM));
	json_add_item_to_object(js_child, "WF", json_create_number(hmIotsDevInfo.sensor_arm_timer.disarmTimer.wkflag));
	json_add_item_to_object(js_child, "TS", js_ts = json_create_array());
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.disarmTimer.month));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.disarmTimer.day));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.disarmTimer.hour));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.disarmTimer.min));
	json_add_item_toarray(js_am,js_child);

	js_child = json_create_object();
	json_add_item_to_object(js_child, "AS", json_create_number(SENSOR_HOME_ARM));
	json_add_item_to_object(js_child, "WF", json_create_number(hmIotsDevInfo.sensor_arm_timer.homeArmTimer.wkflag));
	json_add_item_to_object(js_child, "TS", js_ts = json_create_array());
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.homeArmTimer.month));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.homeArmTimer.day));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.homeArmTimer.hour));
	json_add_item_toarray(js_ts,json_create_number(hmIotsDevInfo.sensor_arm_timer.homeArmTimer.min));
	json_add_item_toarray(js_am,js_child);

	return js_obj;
	
}


//主动上报zigbee设备报警pass发送，写入缓存//
void userAlarmMsgData_write(unsigned char* buff, int* bufflen,HM_DEV_RAM_LIST *pni,char alarm_data)
{
	uint8_t ZT_valuedata;
	uint32_t converte_data=0,len;
	char strZBMac[18];
	uint8_t *ptr=buff;
	uint32_t rc=0x0001;
	

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
	//pni->epList[0].devData.zoneStateApp.alarms=1;//alarm_data
	pni->epList[0].devData.zoneStateApp.alarms=alarm_data;
	printf("pni->onLine=%x,pni->batteryAlarm=%x,pni->epList[0].devData.zoneStateApp.alarms=%x\n",pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	ZT_valuedata=userCreate_ZTdata(ONLINE_VALUE_TYPE,pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	printf("ZT_valuedata====%x",ZT_valuedata);
	memcpy(ptr+len,&ZT_valuedata,1);len+=1;
	memcpy(ptr+len,&pni->batteryRemain,1);len+=1;
	Int_biglittle_Convert(pni->epList[0].times,&converte_data);
	memcpy(ptr+len,&converte_data,4);
	len+=4;
	*bufflen=len;

	//xiugai 
	/*len=0;
	big_little_exchange(rc,&converte_data);
	memcpy(ptr,&converte_data,2);
	len+=2;
	big_little_exchange(pni->index,&converte_data);
	memcpy(ptr+len,&converte_data,2);
	len+=2;
	pni->epList[0].devData.zoneStateApp.alarms=1;
	printf("pni->onLine=%x,pni->batteryAlarm=%x,pni->epList[0].devData.zoneStateApp.alarms=%x\n",pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	ZT_valuedata=userCreate_ZTdata(ONLINE_VALUE_TYPE,pni->onLine,pni->batteryAlarm,pni->epList[0].devData.zoneStateApp.alarms);
	printf("ZT_valuedata====%x",ZT_valuedata);
	memcpy(ptr+len,&ZT_valuedata,1);len+=1;
	Int_biglittle_Convert(pni->epList[0].times,&converte_data);
	memcpy(ptr+len,&converte_data,4);
	len+=4;
	*bufflen=len;*/

//ZBINTOSS_CMD
}

extern char TCPSocket_connectd;
void userUpdateMsgDataToCloud(void)
{
	uint8_t i;
	if(TCPSocket_connectd==1){//printf("to cloud ---userUpdateMsgDataToCloud--\n");
	//if(MQTT_is_connected()==0)
		//return;
	for(i = 0; i < MAX_ALARM_CLOUD_BUF_NUM; i ++)
	{
		if(alarmBuf[i].validFlag == 1)
		{
			printf("\n alarm to cloud ---userUpdateMsgDataToCloud\n");
			report_zb_alarmMsg(alarmBuf[i].alarmMode,alarmBuf[i].androidBuf,alarmBuf[i].androidLen);
			//userForXlinkGetAllDataPoint((char*)&alarmBuf[i],DEV_DATA_FOR_ALARM_TO_CLOUD,alarm_report_data,&alarm_report_len);
			memset((char*)&alarmBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}

	for(i = 0; i < 5; i ++)
	{
		if(cloudBuf[i].validFlag == 1)
		{
			printf("\n record to cloud ---userUpdateMsgDataToCloud\n");
			report_zb_recordMsg(cloudBuf[i].data,cloudBuf[i].dataLen);
			//report_zb_alarmMsg(alarmBuf[i].androidBuf,alarmBuf[i].androidLen);			
			//userForXlinkGetAllDataPoint((char*)&cloudBuf[i],DEV_DATA_FOR_RECORD_TO_CLOUD,alarm_report_data,&alarm_report_len);
			memset((char*)&cloudBuf[i],0,sizeof(ALARMTOCLOUD));
		}
	}
		}
}



