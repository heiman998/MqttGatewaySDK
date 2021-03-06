#include "qcom_common.h"
#include "hm_app.h"
#include "hm_zigbeeInfohand.h"
#include "hm_timerEvent.h"
#include "qcom_security.h"



HM_GW_LIST gw_info;
DEV_NUM_INFO devNumInfo;
DEV_OTA_STATION otaStationInfo;
extern DEV_MYSELF_INFO hmIotsDevInfo;
extern GWSTATFORTIME gwStateForRealTime;

uint16_t findFreeDevIndex(void)
{
	uint8_t i;
	for(i = 0; i < MAX_INDEX_NUM; i ++)
	{
		if(devNumInfo.devIndex[i] == INVALID_DEV_INDEX)
		{
			break;
		}
	}
	if(i == MAX_INDEX_NUM)
	{
		return INVALID_DEV_INDEX;
	}
	printf("\n-----the findFreeDevIndex-----------i=%d\r\n",i);
	return i + BASIC_DEV_INDEX + (qcom_time_us() & 0xff) * 100;
}

uint8_t findFreeSceneIndex(void)
{
	uint8_t i;
	for(i = 3; i < MAX_SCENE_NUM; i ++)//0,1,2为固定场景
	{
		if(devNumInfo.sceneIndex[i] == INVALID_SCENE_LINKAGE_INDEX)
		{
			break;
		}
	}
	if(i == MAX_SCENE_NUM)
	{
		return INVALID_SCENE_LINKAGE_INDEX;
	}
	return i + BASIC_SCENE_INDEX + (qcom_time_us() & 0x0f) * 10;
}

uint8_t findFreeLinkageIndex(void)
{
	uint8_t i;
	for(i = 0; i < MAX_LINKAGE_NUM; i ++)
	{
		if(devNumInfo.linkageIndex[i] == INVALID_SCENE_LINKAGE_INDEX)
		{
			break;
		}
	}
	if(i == MAX_LINKAGE_NUM)
	{
		return INVALID_SCENE_LINKAGE_INDEX;
	}
	return i + BASIC_LINKAGE_INDEX +  (qcom_time_us() & 0x0f) * 10;
}

/***********************************************************
初始化设备定时器，用于GW检测系统时间有效时，
直接初始化网关的定时器
***********************************************************/
void userInitGwArmForTimerEvent(char *gwInfo)
{
	T_TIMER timers;
	OUTPUTINFO output;
	uint8_t i;
	DEV_MYSELF_INFO *gwPnl = (DEV_MYSELF_INFO*)gwInfo;
	output.devAction.modeByte = 0;
	memset((char*)&output.devData,0,sizeof(SCENE_LINKAGE_OUTPUT_STA));
	output.type = SCENE_LIST_GW_PAYLOAD;
	output.valid = 1;
	output.delay = 0;
	printf("user init gw info timer\r\n");
	if(gwPnl->sensor_arm_timer.armTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.armTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.armTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.armTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.armTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.armTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_ARM;

		userFillTimerEvent(timers,output);
	}

	if(gwPnl->sensor_arm_timer.disarmTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.disarmTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.disarmTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.disarmTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.disarmTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.disarmTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_DISARM;

		userFillTimerEvent(timers,output);
	}

	if(gwPnl->sensor_arm_timer.homeArmTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.homeArmTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.homeArmTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.homeArmTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.homeArmTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.homeArmTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_HOME_ARM;

		userFillTimerEvent(timers,output);
	}
}
/***********************************************************
更新GW撤布防定时器，用于app更新网关定时器时使用
***********************************************************/
void userUpdateGwArmForTimerEvent(char *gwInfo)
{
	T_TIMER timers;
	OUTPUTINFO output;
	uint8_t i;
	DEV_MYSELF_INFO *gwPnl = (DEV_MYSELF_INFO*)gwInfo;
	output.devAction.modeByte = 0;
	memset((char*)&output.devData,0,sizeof(SCENE_LINKAGE_OUTPUT_STA));
	output.type = SCENE_LIST_GW_PAYLOAD;
	output.valid = 1;
	output.delay = 0;
	printf("user update gw info timer\r\n");
	if(gwPnl->sensor_arm_timer.armTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.armTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.armTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.armTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.armTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.armTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_ARM;

		userUpdateTimerEvent(timers,output);
	}

	if(gwPnl->sensor_arm_timer.disarmTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.disarmTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.disarmTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.disarmTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.disarmTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.disarmTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_DISARM;

		userUpdateTimerEvent(timers,output);
	}

	if(gwPnl->sensor_arm_timer.homeArmTimer.wkflag & 0x80)
	{
		timers.dealFlag = 0;
		timers.tmonth = gwPnl->sensor_arm_timer.homeArmTimer.month;
		timers.tday = gwPnl->sensor_arm_timer.homeArmTimer.day;
		timers.thour = gwPnl->sensor_arm_timer.homeArmTimer.hour;
		timers.tmin = gwPnl->sensor_arm_timer.homeArmTimer.min;
		timers.twf =  gwPnl->sensor_arm_timer.homeArmTimer.wkflag;
		timers.tsec = 0;
		output.targetId = 0xffff;
		
		output.devAction.GWStat.armtype = 1;
		output.devData.gwOutput.armType = SENSOR_HOME_ARM;

		userUpdateTimerEvent(timers,output);
	}
}


/***********************************************************
初始化设备定时器，用于GW检测系统时间有效时，
直接初始化所有设备的定时器
***********************************************************/
void userInitTheDevTimerEvent(char *devInfo)
{
	T_TIMER timers;
	OUTPUTINFO output;
	uint8_t i;
	HM_DEV_RAM_LIST *pnl = (HM_DEV_RAM_LIST*)devInfo;
	output.devAction.modeByte = 0;
	memset((char*)&output.devData,0,sizeof(SCENE_LINKAGE_OUTPUT_STA));
	output.type = SCENE_LIST_ZB_DEV_PAYLOAD;
	output.valid = 1;
	output.delay = 0;
	printf("user init timer event\r\n");
	switch(pnl->devType)
	{
		case NODE_INFO_TYPE_LIGHT_RGB:
			if(pnl->epList[0].devData.colorStateApp.timer.wk_flag & 0x80)
			{
				if((pnl->epList[0].devData.colorStateApp.timer.s_hour == pnl->epList[0].devData.colorStateApp.timer.e_hour) &&
				    (pnl->epList[0].devData.colorStateApp.timer.s_min == pnl->epList[0].devData.colorStateApp.timer.e_min))
				{
					printf("the timer was not valid\r\n");
					break;
				}
				
				timers.dealFlag = 0;
				timers.tmonth = pnl->epList[0].devData.colorStateApp.timer.s_month;
				timers.tday = pnl->epList[0].devData.colorStateApp.timer.s_day;
				timers.thour = pnl->epList[0].devData.colorStateApp.timer.s_hour;
				timers.tmin = pnl->epList[0].devData.colorStateApp.timer.s_min;
				timers.twf =  pnl->epList[0].devData.colorStateApp.timer.wk_flag;
				timers.tsec = 0;
				output.targetId = pnl->index;
				
				output.devAction.RGBSet.onoff = 1;
				output.devData.rgbOutput.onoff = 1;

				userFillTimerEvent(timers,output);

				timers.tmonth = pnl->epList[0].devData.colorStateApp.timer.e_month;
				timers.tday = pnl->epList[0].devData.colorStateApp.timer.e_day;
				timers.thour = pnl->epList[0].devData.colorStateApp.timer.e_hour;
				timers.tmin = pnl->epList[0].devData.colorStateApp.timer.e_min;
				output.devAction.RGBSet.onoff = 1;
				output.devData.rgbOutput.onoff = 0;
				userFillTimerEvent(timers,output);
			}
			break;
		case NODE_INFO_TYPE_AI1SW:
		case NODE_INFO_TYPE_AI2SW:
		case NODE_INFO_TYPE_AI3SW:
			for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.witchStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.witchStateApp.timer.s_hour == pnl->epList[i].devData.witchStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.witchStateApp.timer.s_min == pnl->epList[i].devData.witchStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timers.dealFlag = 0;
					timers.tmonth = pnl->epList[i].devData.witchStateApp.timer.s_month;
					timers.tday = pnl->epList[i].devData.witchStateApp.timer.s_day;
					timers.thour = pnl->epList[i].devData.witchStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.witchStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.witchStateApp.timer.wk_flag;
					timers.tsec = 0;
					output.targetId = pnl->index;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.SWSet.onoff1 = 1;
						output.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.SWSet.onoff2 = 1;
						output.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.SWSet.onoff3 = 1;
						output.devData.switchOutput.onoff3 = 1;
					}


					userFillTimerEvent(timers,output);

					timers.tmonth= pnl->epList[i].devData.witchStateApp.timer.e_month;
					timers.tday= pnl->epList[i].devData.witchStateApp.timer.e_day;
					timers.thour = pnl->epList[i].devData.witchStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.witchStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.SWSet.onoff1 = 1;
						output.devData.switchOutput.onoff1 = 0;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.SWSet.onoff2 = 1;
						output.devData.switchOutput.onoff2 = 0;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.SWSet.onoff3 = 1;
						output.devData.switchOutput.onoff3 = 0;
					}
					userFillTimerEvent(timers,output);
				}
			}
			break;
		case NODE_INFO_TYPE_RELAY_CONTROL:
			/*for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timeMode = TIMER_EVENT_DEV_CONTROL;
					timers.dealFlag = 0;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					target = pnl->index;
					timers.data.devAction.modeByte = 0;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}


					userFillTimerEvent(timers,timeMode,target);

					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);
				}
			}*/
			break;
		case NODE_INFO_TYPE_THERMOSTAT:
			/*for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timeMode = TIMER_EVENT_DEV_CONTROL;
					timers.dealFlag = 0;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					target = pnl->index;
					timers.data.devAction.modeByte = 0;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);

					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);
				}
			}*/
			break;
		case NODE_INFO_TYPE_AI_SOCKET_USB:
			for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timers.dealFlag = 0;
					timers.tmonth = pnl->epList[i].devData.onoffOutStateApp.timer.s_month;
					timers.tday = pnl->epList[i].devData.onoffOutStateApp.timer.s_day;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					output.targetId = pnl->index;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.UPlugSet.relayOnoff = 1;
						output.devData.uplugOutput.onoff_r = 1;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.UPlugSet.usbOnoff = 1;
						output.devData.uplugOutput.onoff_u = 1;
					}

					userFillTimerEvent(timers,output);


					timers.tmonth = pnl->epList[i].devData.onoffOutStateApp.timer.e_month;
					timers.tday = pnl->epList[i].devData.onoffOutStateApp.timer.e_day;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.UPlugSet.relayOnoff = 1;
						output.devData.uplugOutput.onoff_r = 0;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.UPlugSet.usbOnoff = 1;
						output.devData.uplugOutput.onoff_u = 0;
					}
					userFillTimerEvent(timers,output);
				}
			}
			break;
		case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
			if(pnl->epList[0].devData.splugStateApp.timer.wk_flag & 0x80)
			{
				if((pnl->epList[0].devData.splugStateApp.timer.s_hour == pnl->epList[0].devData.splugStateApp.timer.e_hour) &&
				    (pnl->epList[0].devData.splugStateApp.timer.s_min == pnl->epList[0].devData.splugStateApp.timer.e_min))
				{
					printf("the timer was not valid\r\n");
					break;
				}
				timers.dealFlag = 0;
				timers.tmonth = pnl->epList[0].devData.splugStateApp.timer.s_month;
				timers.tday = pnl->epList[0].devData.splugStateApp.timer.s_day;
				timers.thour = pnl->epList[0].devData.splugStateApp.timer.s_hour;
				timers.tmin = pnl->epList[0].devData.splugStateApp.timer.s_min;
				timers.twf =  pnl->epList[0].devData.splugStateApp.timer.wk_flag;
				timers.tsec = 0;
				output.targetId = pnl->index;
	
				output.devAction.SPlugSet.relayOnoff = 1;
				output.devData.splugOutput.onoff = 1;

				userFillTimerEvent(timers,output);


				timers.tmonth = pnl->epList[0].devData.splugStateApp.timer.e_month;
				timers.tday = pnl->epList[0].devData.splugStateApp.timer.e_day;
				timers.thour = pnl->epList[0].devData.splugStateApp.timer.e_hour;
				timers.tmin = pnl->epList[0].devData.splugStateApp.timer.e_min;
				output.devAction.SPlugSet.relayOnoff = 1;
				output.devData.splugOutput.onoff = 0;
				userFillTimerEvent(timers,output);
			}
		
			break;
		default:
			break;
	}
}

/***********************************************************
更新设备定时器，用于app更新设备定时器时使用
***********************************************************/
void userUpdateTheDevTimerEvent(char *devInfo)
{
	T_TIMER timers;
	OUTPUTINFO output;
	uint8_t i;
	HM_DEV_RAM_LIST *pnl = (HM_DEV_RAM_LIST*)devInfo;
	output.devAction.modeByte = 0;
	memset((char*)&output.devData,0,sizeof(SCENE_LINKAGE_OUTPUT_STA));
	output.type = SCENE_LIST_ZB_DEV_PAYLOAD;
	output.valid = 1;
	output.delay = 0;
	printf("start update timer\r\n");
	switch(pnl->devType)
	{
		case NODE_INFO_TYPE_LIGHT_RGB:
			if(pnl->epList[0].devData.colorStateApp.timer.wk_flag & 0x80)//定时器使能
			{
				if((pnl->epList[0].devData.colorStateApp.timer.s_hour == pnl->epList[0].devData.colorStateApp.timer.e_hour) &&
				    (pnl->epList[0].devData.colorStateApp.timer.s_min == pnl->epList[0].devData.colorStateApp.timer.e_min))
				{
					printf("the timer was not valid\r\n");
					break;
				}
				
				timers.dealFlag = 0;
				timers.tmonth = pnl->epList[0].devData.colorStateApp.timer.s_month;
				timers.tday = pnl->epList[0].devData.colorStateApp.timer.s_day;
				timers.thour = pnl->epList[0].devData.colorStateApp.timer.s_hour;
				timers.tmin = pnl->epList[0].devData.colorStateApp.timer.s_min;
				timers.twf =  pnl->epList[0].devData.colorStateApp.timer.wk_flag;
				timers.tsec = 0;
				output.targetId = pnl->index;
				
				output.devAction.RGBSet.onoff = 1;
				output.devData.rgbOutput.onoff = 1;

				userUpdateTimerEvent(timers,output);

				timers.tmonth = pnl->epList[0].devData.colorStateApp.timer.e_month;
				timers.tday = pnl->epList[0].devData.colorStateApp.timer.e_day;
				timers.thour = pnl->epList[0].devData.colorStateApp.timer.e_hour;
				timers.tmin = pnl->epList[0].devData.colorStateApp.timer.e_min;
				output.devAction.RGBSet.onoff = 1;
				output.devData.rgbOutput.onoff = 0;
				userUpdateTimerEvent(timers,output);
			}
			break;
		case NODE_INFO_TYPE_AI1SW:
		case NODE_INFO_TYPE_AI2SW:
		case NODE_INFO_TYPE_AI3SW:
			for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.witchStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.witchStateApp.timer.s_hour == pnl->epList[i].devData.witchStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.witchStateApp.timer.s_min == pnl->epList[i].devData.witchStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timers.dealFlag = 0;
					timers.tmonth = pnl->epList[i].devData.witchStateApp.timer.s_month;
					timers.tday = pnl->epList[i].devData.witchStateApp.timer.s_day;
					timers.thour = pnl->epList[i].devData.witchStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.witchStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.witchStateApp.timer.wk_flag;
					timers.tsec = 0;
					output.targetId = pnl->index;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.SWSet.onoff1 = 1;
						output.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.SWSet.onoff2 = 1;
						output.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.SWSet.onoff3 = 1;
						output.devData.switchOutput.onoff3 = 1;
					}


					userUpdateTimerEvent(timers,output);

					timers.tmonth = pnl->epList[i].devData.witchStateApp.timer.e_month;
					timers.tday = pnl->epList[i].devData.witchStateApp.timer.e_day;
					timers.thour = pnl->epList[i].devData.witchStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.witchStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.SWSet.onoff1 = 1;
						output.devData.switchOutput.onoff1 = 0;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.SWSet.onoff2 = 1;
						output.devData.switchOutput.onoff2 = 0;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.SWSet.onoff3 = 1;
						output.devData.switchOutput.onoff3 = 0;
					}
					userUpdateTimerEvent(timers,output);
				}
			}
			break;
		case NODE_INFO_TYPE_RELAY_CONTROL:
			/*for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timeMode = TIMER_EVENT_DEV_CONTROL;
					timers.dealFlag = 0;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					target = pnl->index;
					timers.data.devAction.modeByte = 0;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}


					userFillTimerEvent(timers,timeMode,target);

					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);
				}
			}*/
			break;
		case NODE_INFO_TYPE_THERMOSTAT:
			/*for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timeMode = TIMER_EVENT_DEV_CONTROL;
					timers.dealFlag = 0;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					target = pnl->index;
					timers.data.devAction.modeByte = 0;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);

					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						timers.data.devAction.SWSet.onoff1 = 1;
						timers.data.devData.switchOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						timers.data.devAction.SWSet.onoff2 = 1;
						timers.data.devData.switchOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						timers.data.devAction.SWSet.onoff3 = 1;
						timers.data.devData.switchOutput.onoff3 = 1;
					}
					userFillTimerEvent(timers,timeMode,target);
				}
			}*/
			break;
		case NODE_INFO_TYPE_AI_SOCKET_USB:
			for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.onoffOutStateApp.timer.s_hour == pnl->epList[i].devData.onoffOutStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.onoffOutStateApp.timer.s_min == pnl->epList[i].devData.onoffOutStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timers.dealFlag = 0;
					timers.tmonth = pnl->epList[i].devData.onoffOutStateApp.timer.s_month;
					timers.tday = pnl->epList[i].devData.onoffOutStateApp.timer.s_day;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.onoffOutStateApp.timer.wk_flag;
					timers.tsec = 0;
					output.targetId = pnl->index;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.UPlugSet.relayOnoff = 1;
						output.devData.uplugOutput.onoff_r = 1;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.UPlugSet.usbOnoff = 1;
						output.devData.uplugOutput.onoff_u = 1;
					}

					userUpdateTimerEvent(timers,output);

					timers.tmonth = pnl->epList[i].devData.onoffOutStateApp.timer.e_month;
					timers.tday = pnl->epList[i].devData.onoffOutStateApp.timer.e_day;
					timers.thour = pnl->epList[i].devData.onoffOutStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.onoffOutStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.UPlugSet.relayOnoff = 1;
						output.devData.uplugOutput.onoff_r = 0;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.UPlugSet.usbOnoff = 1;
						output.devData.uplugOutput.onoff_u = 0;
					}
					userUpdateTimerEvent(timers,output);
				}
			}
			break;
		case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
			if(pnl->epList[0].devData.splugStateApp.timer.wk_flag & 0x80)
			{
				if((pnl->epList[0].devData.splugStateApp.timer.s_hour == pnl->epList[0].devData.splugStateApp.timer.e_hour) &&
				    (pnl->epList[0].devData.splugStateApp.timer.s_min == pnl->epList[0].devData.splugStateApp.timer.e_min))
				{
					printf("the timer was not valid\r\n");
					break;
				}
				timers.dealFlag = 0;
				timers.tmonth = pnl->epList[0].devData.splugStateApp.timer.s_month;
				timers.tday = pnl->epList[0].devData.splugStateApp.timer.s_day;
				timers.thour = pnl->epList[0].devData.splugStateApp.timer.s_hour;
				timers.tmin = pnl->epList[0].devData.splugStateApp.timer.s_min;
				timers.twf =  pnl->epList[0].devData.splugStateApp.timer.wk_flag;
				timers.tsec = 0;
				output.targetId = pnl->index;
	
				output.devAction.SPlugSet.relayOnoff = 1;
				output.devData.splugOutput.onoff = 1;

				userUpdateTimerEvent(timers,output);

				timers.tmonth = pnl->epList[0].devData.splugStateApp.timer.e_month;
				timers.tday = pnl->epList[0].devData.splugStateApp.timer.e_day;
				timers.thour = pnl->epList[0].devData.splugStateApp.timer.e_hour;
				timers.tmin = pnl->epList[0].devData.splugStateApp.timer.e_min;
				output.devAction.SPlugSet.relayOnoff = 1;
				output.devData.splugOutput.onoff = 0;
				userUpdateTimerEvent(timers,output);
			}
		
			break;
		case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
			for(i = 0; i < pnl->epNum; i ++)
			{
				if(pnl->epList[i].devData.cstRgbStateApp.timer.wk_flag & 0x80)
				{
					if((pnl->epList[i].devData.cstRgbStateApp.timer.s_hour == pnl->epList[i].devData.cstRgbStateApp.timer.e_hour) &&
					    (pnl->epList[i].devData.cstRgbStateApp.timer.s_min == pnl->epList[i].devData.cstRgbStateApp.timer.e_min))
					{
						printf("the timer was not valid\r\n");
						break;
					}
					timers.dealFlag = 0;
					timers.tmonth = pnl->epList[i].devData.cstRgbStateApp.timer.s_month;
					timers.tday = pnl->epList[i].devData.cstRgbStateApp.timer.s_day;
					timers.thour = pnl->epList[i].devData.cstRgbStateApp.timer.s_hour;
					timers.tmin = pnl->epList[i].devData.cstRgbStateApp.timer.s_min;
					timers.twf =  pnl->epList[i].devData.cstRgbStateApp.timer.wk_flag;
					timers.tsec = 0;
					output.targetId = pnl->index;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.CUTRGBSet.onoffList1 = 1;
						output.devData.cutRgbOutput.onoff1 = 1;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.CUTRGBSet.onoffList2 = 1;
						output.devData.cutRgbOutput.onoff2 = 1;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.CUTRGBSet.onoffList3 = 1;
						output.devData.cutRgbOutput.onoff3 = 1;
					}else if(pnl->epList[i].ep == 4){
						output.devAction.CUTRGBSet.onoffList4 = 1;
						output.devData.cutRgbOutput.onoff4 = 1;
					}


					userUpdateTimerEvent(timers,output);

					timers.tmonth = pnl->epList[i].devData.cstRgbStateApp.timer.e_month;
					timers.tday = pnl->epList[i].devData.cstRgbStateApp.timer.e_day;
					timers.thour = pnl->epList[i].devData.cstRgbStateApp.timer.e_hour;
					timers.tmin = pnl->epList[i].devData.cstRgbStateApp.timer.e_min;
					if(pnl->epList[i].ep == 1)
					{
						output.devAction.CUTRGBSet.onoffList1 = 1;
						output.devData.cutRgbOutput.onoff1 = 0;
					}else if(pnl->epList[i].ep == 2){
						output.devAction.CUTRGBSet.onoffList2 = 1;
						output.devData.cutRgbOutput.onoff2 = 0;
					}else if(pnl->epList[i].ep == 3){
						output.devAction.CUTRGBSet.onoffList3 = 1;
						output.devData.cutRgbOutput.onoff3 = 0;
					}else if(pnl->epList[i].ep == 4){
						output.devAction.CUTRGBSet.onoffList4 = 1;
						output.devData.cutRgbOutput.onoff4 = 0;
					}
					userUpdateTimerEvent(timers,output);
				}
			}
			break;
		default:
			break;
	}
}



void userInitTheLinkageTimerEvent(char *linkInfo)
{
	T_TIMER timers;
	OUTPUTINFO output;
	LINKAGEINFO *linkagePni = (LINKAGEINFO*)linkInfo ;
	if(linkagePni->inputInfo.type == SCENE_LIST_TIMER_PAYLOAD)//时间触发
	{
		if(1)//时间使能
		{
			output.valid = 1;
			output.type = SCENE_LIST_LINKAGE_PAYLOAD;
			timers.tmonth = linkagePni->inputInfo.IN.timeIn.month;
			timers.tday =  linkagePni->inputInfo.IN.timeIn.day;
			timers.thour =  linkagePni->inputInfo.IN.timeIn.hour;
			timers.tmin =  linkagePni->inputInfo.IN.timeIn.min;
			timers.tsec = 0;
			timers.twf =  linkagePni->inputInfo.IN.timeIn.wkflag;
			timers.dealFlag = 0;
			output.targetId = linkagePni->linkageId;
			userFillTimerEvent(timers,output);
		}
	}
}

/**************************************************************************
该函数用于增加一个延时的定时器，
temptarget 为目标index，或场景、联动ID
mode区分定时器为设备驱动、场景驱动、联动驱动
delays 延时时间，单位s

***************************************************************************/
void userAddDelayToTimerEvent(uint16_t delays,OUTPUTINFO output)
{
	T_TIMER timers;
	HMTM delayTime;
	int32_t zoneTime;
	
	if(gwStateForRealTime.sysRealTimerValid == 1)//系统时间有效
	{
		//系统存储的为格林威治时间，在fill timer中减去了时区时间，故在延时设置时，需要加上这个时区时间
		zoneTime = (hmIotsDevInfo.time_zones / 100) * 3600 + (hmIotsDevInfo.time_zones % 100) * 60;
		delayTime = userChangeSTimeToYears(gwStateForRealTime.sysRealTimeFors + zoneTime + delays);
		timers.dealFlag = 0;
		timers.tmonth = delayTime.tm_mon;
		timers.tday = delayTime.tm_mday;
		timers.thour = delayTime.tm_hour;
		timers.tmin = delayTime.tm_min;
		timers.tsec = delayTime.tm_sec;
		timers.twf = 0x80;//最高位为时间有效位
		userFillTimerEvent(timers,output);
	}
}

void initOtaStationInfo(void)
{
	uint32_t tempaddr = START_ADDR_FLASH_SUB_FIRMWARE_INFO_ADDR;
	qca_load_flash_params(tempaddr,(uint8_t *)&otaStationInfo,sizeof(otaStationInfo));
	if((otaStationInfo.head != 0xaa) || (otaStationInfo.end != 0x55))
	{
		otaStationInfo.head = 0xaa;
		otaStationInfo.end = 0x55;
		otaStationInfo.firmwareValid = 0;
		otaStationInfo.devImageSize = 0;
		otaStationInfo.devImageVersion = 0;
		otaStationInfo.devUpdateNeedFlag = NO_DEV_NEED_UPDATE_IMAGE;
		otaStationInfo.flashSaveImageType = NO_IMAGE_SAVE_FLASH;
		qca_save_flash_params(tempaddr,(uint8_t *)&otaStationInfo,sizeof(otaStationInfo));
	}
}



void getValidDevNameAndDevType(HM_DEV_RAM_LIST *pni,uint8_t *payload,uint8_t len)
{
	char mac_addr[10];
	uint8_t devType = NODE_INFO_TYPE_UNKNOWN;
	uint8_t temp_arm = 1;
	memset(mac_addr, 0, 10);
	int i;
	for(i=0; i<3; i++)
	{
		if(pni->mac[i] < 0x10)
			sprintf(mac_addr, "%s0%X", mac_addr, pni->mac[i]);
		else
			sprintf(mac_addr, "%s%X", mac_addr, pni->mac[i]);
	}
	switch(pni->epList[0].deviceId)
	{
		case DEVICE_ID_ON_OFF_SWITCH:
			pni->batteryRemain = 0xff;//不支持电量，用0xff表示
			 if(strstr((char*)pni->modeId,"PLUG") != NULL)
			{
				devType =  NODE_INFO_TYPE_AI_SOCKET_USB;
				sprintf((char *)pni->name, "%s%s", NODE_INFO_TYPE_AI_SOCKET_USB_STRING,mac_addr);
				if(len == 1)
				{
					pni->epList[0].devData.onoffOutStateApp.onOffStatus = payload[0];
				}else{
					printf("the len is allow with dev type:%d\r\n",len);
				}
			}
			break;
		case DEVICE_ID_LEVEL_CONTROL_SWITCH:
			break;
		case DEVICE_ID_ON_OFF_OUTPUT:
			pni->batteryRemain = 0xff;//不支持电量，用0xff表示

			if(strstr((char*)pni->modeId,"SW") != NULL)
			{
				if(strstr((char*)pni->modeId,"SW1A") != NULL)
				{
					devType =  NODE_INFO_TYPE_AI1SW;
					sprintf((char *)pni->name, "%s%s", NODE_INFO_TYPE_AI1SW_STRING,mac_addr);
				}else if(strstr((char*)pni->modeId,"SW2A") != NULL){
					devType =  NODE_INFO_TYPE_AI2SW;
					sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_AI2SW_STRING,mac_addr);
				}else if(strstr((char*)pni->modeId,"SW3A") != NULL){
					devType =  NODE_INFO_TYPE_AI3SW;
					sprintf((char *)pni->name, "%s%s", NODE_INFO_TYPE_AI3SW_STRING,mac_addr);
					printf("----------the type sw3a---------\r\n");
				}//NODE_INFO_TYPE_AI_SOCKET_USB 跟2路开关有冲突
				//sprintf((char *)node_name, "ZPlug_%s", mac_addr);
				if(len == 1)
				{
					pni->epList[0].devData.witchStateApp.onOffStatus = payload[0];
				}else{
					printf("the len is allow with dev type:%d\r\n",len);
				}
			}else if(strstr((char*)pni->modeId,"HS2RL") != NULL){
				devType =  NODE_INFO_TYPE_RELAY_CONTROL;
				sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_RELAY_CONTROL_STRING, mac_addr);
				if(len == 1)
				{
					pni->epList[0].devData.onoffOutStateApp.onOffStatus = payload[0];
				}else{
					printf("the len is allow with dev type:%d\r\n",len);
				}
			}else if(strstr((char*)pni->modeId,"CUTRGB4") != NULL){
				devType =  NODE_INFO_TYPE_CUSTOM_RGB_AREA;
				sprintf((char *)pni->name, "%s%s", NODE_INFO_TYPE_CUSTOM_RGB_AREA_STRING,mac_addr);
				if(len == 4)
				{
					pni->epList[0].devData.cstRgbStateApp.onOffStatus = payload[0];
					pni->epList[0].devData.cstRgbStateApp.brightness = payload[1];
					pni->epList[0].devData.cstRgbStateApp.colorTemrature = payload[2] + ((uint16_t)payload[3] << 8);
				}else{
					printf("the len is allow with dev type:%d\r\n",len);
				}
			}
			break;
		case DEVICE_ID_LEVEL_CONTROL_OUTPUT:
			break;
		case DEVICE_ID_SCENE_SELECTOR:
		case DEVICE_ID_CONFIG_TOOL:
		case DEVICE_ID_REMOTE_CONTROL:
		case DEVICE_ID_COMBINED_INTERFACE:
		case DEVICE_ID_RANGE_EXTENDER:
		case DEVICE_ID_MAINS_POWER_OUTLET:
		case DEVICE_ID_DOOR_LOCK:
		case DEVICE_ID_DOOR_LOCK_CONTROLLER:
		case DEVICE_ID_SIMPLE_SENSOR:
		case DEVICE_ID_CONSUMPTION_AWARENESS_DEVICE:
			break;
		case DEVICE_ID_COLOR_DIMMABLE_LIGHT:
			pni->batteryRemain = 0xff;//不支持电量，用0xff表示
			devType =  NODE_INFO_TYPE_LIGHT_RGB;
			sprintf((char *)pni->name, "%s%s", NODE_INFO_TYPE_LIGHT_RGB_STRING,mac_addr);
			if(len == 9)
			{
				pni->epList[0].devData.colorStateApp.onOffStatus = payload[0];
				pni->epList[0].devData.colorStateApp.level = payload[1];
				//pni->epList[0].devData.colorStateApp.hue = payload[2];
				pni->epList[0].devData.colorStateApp.color_x = payload[3] + ((uint16_t)payload[4] << 8);
				pni->epList[0].devData.colorStateApp.color_y = payload[5] + ((uint16_t)payload[6] << 8);
				//pni->epList[0].devData.colorStateApp.color_temp = payload[7] + ((uint16_t)payload[8] << 8);
				hmzColorXYToRgb(pni->epList[0].devData.colorStateApp.color_x,pni->epList[0].devData.colorStateApp.color_y,pni->epList[0].devData.colorStateApp.level,
								&pni->epList[0].devData.colorStateApp.rgb_r,&pni->epList[0].devData.colorStateApp.rgb_g,&pni->epList[0].devData.colorStateApp.rgb_b);
			}else{
				printf("the len is allow with dev type:%d\r\n",len);
			}
			break;
		case DEVICE_ID_SMART_PLUG:
			pni->batteryRemain = 0xff;//不支持电量，用0xff表示
			devType =  NODE_INFO_TYPE_AI_SOCKET_MEASURE;
			sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_AI_SOCKET_MEASURE_STRING, mac_addr);
			if(len == 15)
			{
				pni->epList[0].devData.splugStateApp.onOffStatus = payload[0];
				pni->epList[0].devData.splugStateApp.rmsVoltage = payload[1] + ((uint16_t)payload[2] << 8);
				pni->epList[0].devData.splugStateApp.InstantaneousDemand = payload[3] + (((uint32_t)payload[4] << 8)) +
																		   ((uint32_t)payload[5] << 16) + ((uint32_t)payload[6] << 24);
				pni->epList[0].devData.splugStateApp.CurrentSummationDelivered =  ((uint64_t)payload[7] << 0) + ((uint64_t)payload[8] << 8) +
																				((uint64_t)payload[9] << 16) + ((uint64_t)payload[10] << 24) +
																			   ((uint64_t)payload[11] << 32) + ((uint64_t)payload[12] << 40) +
																			    ((uint64_t)payload[13] << 48) + ((uint64_t)payload[14] << 56) ;	
			}else{
				printf("the len is allow with dev type:%d\r\n",len);
			}
			break;
		case DEVICE_ID_WHITE_GOODS:
			break;
		case DEVICE_ID_METER_INTERFACE:
			break;
		case DEVICE_ID_THERMOSTAT:
			break;
		case DEVICE_ID_TEMPERATURE_SENSOR:
			devType =  NODE_INFO_TYPE_TEMP_HUM_SENSOR;
			if(len == 2)
			{
				if(pni->epNum >1)
				{
					if(pni->epList[0].ep == 1)
					{
						pni->epList[0].devData.tempStateApp.tempMeterage = payload[0] + ((uint16_t)payload[1] << 8);
						printf("the DEVICE_ID_TEMPERATURE_SENSOR tempMeterage:%x\r\n",pni->epList[0].devData.tempStateApp.tempMeterage);
					}else{
						pni->epList[0].devData.humiStateApp.humiMeterage = payload[0] + ((uint16_t)payload[1] << 8);
						printf("the DEVICE_ID_TEMPERATURE_SENSOR humiMeterage:%x\r\n",pni->epList[0].devData.humiStateApp.humiMeterage);
					}
				}else{
					pni->epList[0].devData.tempStateApp.tempMeterage = payload[0] + ((uint16_t)payload[1] << 8);
				}
			}else{
				printf("the len is allow with dev type:%d\r\n",len);
			}
			sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_TEMP_HUM_SENSOR_STRING, mac_addr);
			break;
		case DEVICE_ID_IAS_ANCILLARY_CONTROL:
			devType =  NODE_INFO_TYPE_REMOTE_CONTROL;
			sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_REMOTE_CONTROL_STRING, mac_addr);
			if(len == 2)
			{
				pni->epList[0].devData.aceStateApp.comId = payload[0];
				pni->epList[0].devData.aceStateApp.modeId = payload[1];
			}else{
				printf("the len is allow with dev type:%d\r\n",len);
			}
			break;
		case DEVICE_ID_IAS_ZONE:	
			if(len == 5)
			{
				pni->epList[0].devData.zoneStateApp.zoneId = payload[0];
				pni->epList[0].devData.zoneStateApp.zoneType = payload[1] + ((uint16_t)payload[2] << 8);
				pni->epList[0].devData.zoneStateApp.zoneStatus = payload[3] + ((uint16_t)payload[4] << 8);
				if (pni->epList[0].devData.zoneStateApp.zoneStatus & 0x08)
				{
					pni->batteryAlarm = 1;
				}
				switch(pni->epList[0].devData.zoneStateApp.zoneType)
				{
					case EMBER_ZCL_IAS_ZONE_TYPE_MOTION_SENSOR:
						if((hmIotsDevInfo.sensor_arm_type == SENSOR_DISARM) || (hmIotsDevInfo.sensor_arm_type == SENSOR_HOME_ARM))
						{
							temp_arm = 0;//撤防
						}
						devType = NODE_INFO_TYPE_PIR_SENSOR;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_PIR_SENSOR_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_CONTACT_SWITCH:
						if(hmIotsDevInfo.sensor_arm_type == SENSOR_DISARM)
						{
							temp_arm = 0;//撤防
						}
						devType = NODE_INFO_TYPE_MAGNET_DOOR;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_MAGNET_DOOR_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_FIRE_SENSOR:
						devType = NODE_INFO_TYPE_SMOKE_SENSOR;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_SMOKE_SENSOR_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_WATER_SENSOR:
						devType = NODE_INFO_TYPE_WATER_SENSOR;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_WATER_SENSOR_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_GAS_SENSOR:
						if(pni->zbSubType == ZIGBEE_ZED_RECEIVE_OFF_IN_IDLE)
						{
							devType = NODE_INFO_TYPE_CO_SENSOR;
							sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_CO_SENSOR_STRING, mac_addr);
						}else{
							devType = NODE_INFO_TYPE_GAS_SENSOR;
							sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_GAS_SENSOR_STRING, mac_addr);
						}
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_PERSONAL_EMERGENCY_DEVICE:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_VIBRATION_MOVEMENT_SENSOR:
						if(hmIotsDevInfo.sensor_arm_type == SENSOR_DISARM)
						{
							temp_arm = 0;//撤防
						}
						devType = NODE_INFO_TYPE_VIBRATION;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_VIBRATION_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_REMOTE_CONTROL:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_KEY_FOB:
						devType = NODE_INFO_TYPE_SOS;
						sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_SOS_STRING, mac_addr);
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_KEYPAD:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_STANDARD_WARNING_DEVICE:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_GLASS_BREAK_SENSOR:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_CARBON_MONOXIDE_SENSOR:
						break;
					case EMBER_ZCL_IAS_ZONE_TYPE_SECURITY_REPEATER:
						break;
					default:
						break;
				}
				pni->epList[0].devData.zoneStateApp.armMode = temp_arm;
				printf("------pni->epList[0].devData.zoneStateApp.armMode:%d\n",temp_arm);
			}else{
				printf("the len is not valid with ias zone dev:%d\r\n",len);
			}
			break;
		case DEVICE_ID_IAS_WARNING:
			if((pni->batteryRemain < 30) || (pni->epList[0].devData.zoneStateApp.zoneStatus & 0x08))
			{
				pni->batteryAlarm = 1;
			}
			devType =  NODE_INFO_TYPE_WARNING_DEV;
			pni->epList[0].devData.wdStateApp.duration = 60;//WD 默认响应时间60s
			sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_WARNING_DEV_STRING, mac_addr);
			break;
		default:
			break;
	}
	if(devType==NODE_INFO_TYPE_UNKNOWN)
	{
              sprintf((char *)pni->name, "%s%s",NODE_INFO_TYPE_UNKNOWN_STRING, mac_addr);
	}
	pni->devType = devType;
	printf("the dev type:%d,and the dev name:%s",devType,pni->name);		
}

void userFlashWriteDevInfoFunc(uint8_t* data,uint32_t addr)
{
    uint32_t eraseaddr;
	uint32_t ret;
    if(data == NULL)
    {
        printf("the input data was null");
        return;
    }
    eraseaddr = (addr  /0x1000) * 0x1000;
    printf("erase addr:%4x",eraseaddr);
    ret=qca_save_flash_params(eraseaddr,data,sizeof(HM_DEV_FLASH_INFO));
	if(ret==0)
		 printf("\n userFlashWriteDevInfoFunc success-----------ok \n");
	else{
		ret=qca_save_flash_params(eraseaddr,data,sizeof(HM_DEV_FLASH_INFO));
	}
}


void userFlashWriteSceneLinkageInfoFunc(uint8_t* data,uint32_t addr,uint16_t dataLen)
{
    uint32_t eraseaddr;

    
    if(data == NULL)
    {
        printf("the input data was null");
        return;
    }
    eraseaddr = (addr  /0x1000) * 0x1000;
    printf("SceneLink erase addr:%4x",eraseaddr);
    qca_save_flash_params(eraseaddr,data,dataLen); 
}

void userFlashWriteNumFunc(uint8_t* data)
{
    uint32_t writeAddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_NUM_OFFSET;
    if(data == NULL)
    {
        printf("the input data was null");
        return;
    }
    qca_save_flash_params(writeAddr,data,sizeof(DEV_NUM_INFO)); 
}

void userFlashWriteSysInfoFunc(uint8_t* data)
{
    DEV_MYSELF_INFO tempData;
    uint32_t writeAddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_DEV_MYSELF_INFO_OFFSET;
    if(data == NULL)
    {
        printf("the input data was null");
        return;
    }
    tempData = *((DEV_MYSELF_INFO*)data);
    tempData.light_onoff = 0;//网关的RGB灯状态不记录在flash中，即重启后网关的RGB灯是灭的
    qca_save_flash_params(writeAddr,(uint8_t*)&tempData,sizeof(DEV_MYSELF_INFO)); 
}

DEV_NUM_INFO userFlashReadNumFunc(void)
{
	DEV_NUM_INFO temp;
	uint32_t readAddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_NUM_OFFSET;
	qca_load_flash_params(readAddr,(uint8_t *)&temp,sizeof(DEV_NUM_INFO));
	return temp;
}

HM_DEV_FLASH_INFO userFlashReadDevInfoFunc(uint32_t addr)
{
	HM_DEV_FLASH_INFO temp;
	if((addr >= START_ADDR_FLASH_NODE_INFO_ADDR) && (addr < START_ADDR_FLASH_SCENE_INFO_ADDR))
	{
		qca_load_flash_params(addr,(uint8_t *)&temp,sizeof(HM_DEV_FLASH_INFO));
	}else{
		printf("the addr was invalid\r\n");
	}
	return temp;
}

void userFlashDeleteOneNodeInfo(uint32_t addr)
{
    uint32_t eraseaddr;
    uint8_t data = 0xff;
    eraseaddr = (addr  /0x1000) * 0x1000;
  
    printf("Delete erase addr:%4x",eraseaddr);
    qca_save_flash_params(eraseaddr,&data,1); //写flash的时候，内部自动擦除该页数据
}

void user_flash_delete_all_node_info(void)//删除所有设备、scene、linkage，backup
{
	uint8_t data = 0xff;
	uint8_t i;
	uint32_t eraseaddr;
	for(i = 0; i < MAX_INDEX_NUM + MAX_SCENE_NUM + MAX_LINKAGE_NUM; i ++)
	{
		eraseaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_NUM_OFFSET + i * BASIC_NODE_INFO_SIZE;
		qca_save_flash_params(eraseaddr,&data,1); //写flash的时候，内部自动擦除该页数据
	}

	//delete linkage to flash
	for(i = 0; i < MAX_LINKAGE_NUM;i ++)
	{
		eraseaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_LINKAGE_OFFSET+ i * BASIC_LINKAGE_INFO_SIZE;
		qca_save_flash_params(eraseaddr,&data,1); //写flash的时候，内部自动擦除该页数据
	}

	//delete scene to flash
	for(i = 0; i < MAX_SCENE_NUM;i ++)
	{
		eraseaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET+ i * BASIC_LINKAGE_INFO_SIZE;
		qca_save_flash_params(eraseaddr,&data,1); //写flash的时候，内部自动擦除该页数据
	}

	eraseaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_DEV_MYSELF_INFO_OFFSET;
    	qca_save_flash_params(eraseaddr,&data,1); //写flash的时候，内部自动擦除该页数据

}




/***************************************************************
函数名： sourceFlashDataIsCorrent
输入参数： 无
输出参数： 0 读取flash有误，1 check 成功
函数说明： 校验原flash num跟info是否匹配
***************************************************************/
uint8_t sourceFlashDataIsCorrent(void)
{
    uint8_t i;
    uint8_t flashValidxNum = 0;
    uint32_t tempaddr;
    DEV_NUM_INFO xNumInfo;
    HM_DEV_FLASH_INFO devFlashInfo;
    SCENEINFO sceneFlashInfo;
    LINKAGEINFO linkageFlashInfo;
    tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_NUM_OFFSET;
    qca_load_flash_params(tempaddr,(uint8_t *)&xNumInfo,sizeof(xNumInfo));

    ///////dev check
    for(i = 0,tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_OFFSET; i < MAX_INDEX_NUM; i ++,tempaddr += BASIC_NODE_INFO_SIZE)//dev num
    {
	 qca_load_flash_params(tempaddr,(uint8_t *)&devFlashInfo,sizeof(devFlashInfo));
        if((devFlashInfo.head == 0xaa) && (devFlashInfo.end == 0x55) && (xNumInfo.devIndex[i] != INVALID_DEV_INDEX))
        {
            flashValidxNum ++; 
        }
    }
    if(xNumInfo.devNum != flashValidxNum)
    {
        printf("flash source check was faild");
        return 0;//ignore
    }
    
    /////scene check
    flashValidxNum = 0;
    for(i = 0,tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET; i < MAX_SCENE_NUM; i ++,tempaddr += BASIC_SCENE_INFO_SIZE)
    {
    	 qca_load_flash_params(tempaddr,(uint8_t *)&sceneFlashInfo,sizeof(sceneFlashInfo));
        if((sceneFlashInfo.head == 0xaa) && (sceneFlashInfo.end == 0x55) && (xNumInfo.sceneIndex[i] != INVALID_SCENE_LINKAGE_INDEX))
        {
            flashValidxNum ++; 
        }
    }
    if(xNumInfo.sceneNum != flashValidxNum)
    {
        printf("flash source check was faild");
        return 0;//ignore
    }
    
        /////linkage check
    flashValidxNum = 0;
    for(i = 0,tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_LINKAGE_OFFSET; i < MAX_LINKAGE_NUM; i ++,tempaddr += BASIC_LINKAGE_INFO_SIZE)
    {
    	 qca_load_flash_params(tempaddr,(uint8_t *)&linkageFlashInfo,sizeof(linkageFlashInfo));
        if((linkageFlashInfo.head == 0xaa) && (linkageFlashInfo.end == 0x55) && (xNumInfo.linkageIndex[i] != INVALID_SCENE_LINKAGE_INDEX))
        {
            flashValidxNum ++; 
        }
    }
    if(xNumInfo.linkageNum != flashValidxNum)
    {
        printf("flash source check was faild");
        return 0;//ignore
    }
    
    return 1;
    
}



void removeNodeList(HM_DEV_RAM_LIST *pni)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pnext;
	HM_DEV_RAM_LIST *pprev;
	if((pni==NULL)||((HM_DEV_RAM_LIST *)pgw_info->pnode_list==NULL))
            return;
	if (pni==(HM_DEV_RAM_LIST *)pni->pnext_dev_info)//only one dev
            pgw_info->pnode_list= 0;//NULL
	else
	{
		if(pni == (HM_DEV_RAM_LIST *)pgw_info->pnode_list)//delete first node
			pgw_info->pnode_list = pni->pnext_dev_info;
		
		pnext=(HM_DEV_RAM_LIST *)pni->pnext_dev_info;
		pprev=(HM_DEV_RAM_LIST *)pni->pprev_dev_info;
		pnext->pprev_dev_info=pni->pprev_dev_info;
		pprev->pnext_dev_info=pni->pnext_dev_info;
	}
        if(pni != NULL)
        {
            mem_free(pni);//链表的成员都为动态内存，删除后需要释放
            pni = NULL;
        }
}

void putNiInList(HM_DEV_RAM_LIST *pni_ram)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pnl=(HM_DEV_RAM_LIST *)(pgw_info->pnode_list);
	HM_DEV_RAM_LIST *oldNext;
	if (pnl== NULL)
	{
		printf("the list was null\r\n");
		pgw_info->pnode_list=(uint32_t)pni_ram;
		pnl=pni_ram;
		pnl->pnext_dev_info=(uint32_t)pnl;
		pnl->pprev_dev_info=(uint32_t)pnl;
	}else{
		printf("the list was not null\r\n");
		oldNext=(HM_DEV_RAM_LIST *)pnl->pnext_dev_info;
		pni_ram->pnext_dev_info=(uint32_t)oldNext;
		pnl->pnext_dev_info=(uint32_t)pni_ram;
		oldNext->pprev_dev_info=(uint32_t)pni_ram;
		pni_ram->pprev_dev_info=(uint32_t)pnl;
	}
}

HM_DEV_RAM_LIST *findInDevListByIndex(uint16_t index)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	if (pni==NULL)
	{
		printf("the ram is empty\r\n");
		return NULL;
	}
	do{
		if (pni_tmp->index == index)
			return pni_tmp;
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);
	printf("the index node is not in ram\r\n");
	return NULL;
}

HM_DEV_RAM_LIST *findInDevListByMac(uint8_t *zclAdd)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	//printf("-------------findInDevListByMac00\r\n");
	if (pni==NULL)
		return NULL;
	//printf("-------------findInDevListByMac01\r\n");
	HM_DEV_RAM_LIST *pni_tmp = pni;
	do{
		if (memcmp(pni_tmp->mac, zclAdd, ZIGBEE_MAC_LEN)== 0)
			return pni_tmp;
		//printf("-------------findInDevListByMac pni_tmp->mac %02x %02x %02x %02x %02x %02x %02x %02x\r\n",\
		//pni_tmp->mac[0],pni_tmp->mac[1],pni_tmp->mac[2],pni_tmp->mac[3],pni_tmp->mac[4],pni_tmp->mac[5],pni_tmp->mac[6],pni_tmp->mac[7]);
					
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);
        printf("the mac node is not in ram\r\n");
	return NULL;
}

HM_DEV_RAM_LIST *findInDevListByNwkadd(uint16_t shortAdd)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	if (pni==NULL)
		return NULL;
	HM_DEV_RAM_LIST *pni_tmp = pni;
	do{
		//printf("the short addr node is pni_tmp->shortAddr:%x,%d\r\n",pni_tmp->shortAddr,pni_tmp->shortAddr);
		if (pni_tmp->shortAddr == shortAdd)
			return pni_tmp;
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);
        printf("the short addr node is not in ram\r\n");
	return NULL;
}

void userFlashInitToRam(void)
{
    uint8_t i,needSaveNumToFlash = 0;
    uint8_t flashValidxNum = 0;
    uint32_t tempaddr,tempDataLen;
    HM_DEV_FLASH_INFO devFlashInfo;
    SCENEINFO sceneFlashInfo;
    LINKAGEINFO linkageFlashInfo;
    T_TIMER timers;
    uint8_t timeMode;
    
    tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_NUM_OFFSET;
    
    if( (tempaddr,(uint8_t *)&devNumInfo,sizeof(devNumInfo)) == 0)
    {
    	printf("\n now no no userFlashInitToRam-------------------0\n");
		if((devNumInfo.head != 0xaa) || (devNumInfo.end != 0x55))
		{
			printf("the devNumInfo was not valid in flash data,init now\r\n");
			memset((char*)&devNumInfo,0xff,sizeof(devNumInfo));
			devNumInfo.sceneNum = 3;
			devNumInfo.sceneIndex[0] = 20;
			devNumInfo.sceneIndex[1] = 21;
			devNumInfo.sceneIndex[2] = 22;
			
			devNumInfo.linkageNum = 0;
			devNumInfo.devNum = 0;
			devNumInfo.head = 0xaa;
			devNumInfo.end = 0x55;
			userFlashWriteNumFunc((uint8_t *)&devNumInfo);
		}
		printf("the scene num:%d,linkage num:%d,dev num:%d\r\n",devNumInfo.sceneNum,devNumInfo.linkageNum,devNumInfo.devNum);
    }
	printf("\n now run userFlashInitToRam-------------------1\r\n");
    tempDataLen = sizeof(devFlashInfo);
    for(i = 0; i < MAX_INDEX_NUM; i ++)
    {
        if(devNumInfo.devIndex[i] != INVALID_DEV_INDEX)
        {	
            tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_NODE_OFFSET+ i * BASIC_NODE_INFO_SIZE;
			printf("\n  userFlashInitToRam devNumInfo.devIndex[%d],tempaddr=%x -------------------1\r\n",i,tempaddr);
	     qca_load_flash_params(tempaddr,(uint8_t *)&devFlashInfo,tempDataLen);

            HM_DEV_RAM_LIST *pin_ram = (HM_DEV_RAM_LIST *)mem_alloc(sizeof(HM_DEV_RAM_LIST));
            if(pin_ram == NULL)
            {
                printf("not malloc mem at userFlashInitToRam\r\n");
                continue;
            }
            if((devFlashInfo.head == 0xaa) && (devFlashInfo.end == 0x55))
            {
                printf("init to ram dev list :%d\r\n",devFlashInfo.index);
            	  pin_ram->batteryAlarm = devFlashInfo.batteryAlarm;
                pin_ram->batteryRemain = devFlashInfo.batteryRemain;
                pin_ram->devType       = devFlashInfo.devType;
                pin_ram->epNum         = devFlashInfo.epNum;
                pin_ram->flashAddr     = tempaddr;
                pin_ram->index         = devFlashInfo.index;
                pin_ram->onLine        = devFlashInfo.onLine;
                pin_ram->shortAddr     = devFlashInfo.shortAddr;
				printf("\n pin_ram->shortAddr --------%x,%d\r\n",pin_ram->shortAddr,pin_ram->shortAddr);
                pin_ram->zbSubType     = devFlashInfo.zbSubType;
                pin_ram->softVersion 	  = devFlashInfo.softVersion;
		  pin_ram->httpState 	  = devFlashInfo.httpState;
		  memcpy(pin_ram->name,devFlashInfo.name,NAME_MAX_LEN);
		  memcpy(pin_ram->roomId,devFlashInfo.roomId,ROOM_ID_MAX_LEN);
		  memcpy(pin_ram->modeId,devFlashInfo.modeId,ZIGBEE_MODEID_LEN);
                memcpy((uint8_t *)(pin_ram->mac),(uint8_t *)(devFlashInfo.mac),ZIGBEE_MAC_LEN);
                memcpy((uint8_t *)(pin_ram->epList),(uint8_t *)(devFlashInfo.epList),sizeof(EP_INFO) * SUPORT_MAX_EP_NUM);
                
                putNiInList(pin_ram);
                
               // userInitTheDevTimerEvent((char*)pin_ram);//timer init
            }else{
               devNumInfo.devIndex[i] =  INVALID_DEV_INDEX;
               devNumInfo.devNum --;
               needSaveNumToFlash = 1;
            }
        }
    }
    
    HM_GW_LIST *pgw_info=&gw_info;
    pgw_info->scene_cnt = devNumInfo.sceneNum;
    pgw_info->linkage_cnt = devNumInfo.linkageNum;
    tempDataLen = sizeof(sceneFlashInfo);
    
    for(i = 0; i < MAX_SCENE_NUM; i ++)
    {
        if(devNumInfo.sceneIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
        {
            tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET+ i * BASIC_SCENE_INFO_SIZE;
	     qca_load_flash_params(tempaddr,(uint8_t *)&sceneFlashInfo,tempDataLen);
            if((sceneFlashInfo.head == 0xaa) && (sceneFlashInfo.end == 0x55))
            {
                memcpy((uint8_t *)(&pgw_info->scene_list[i]),(uint8_t *)&sceneFlashInfo,tempDataLen); 
            }else{

			if(i == 0)
			{
				pgw_info->scene_list[i].sceneId = 20;
				memcpy(pgw_info->scene_list[i].sceneName,"LeavingMode",sizeof("LeavingMode"));
			}else if(i == 1)
			{
				pgw_info->scene_list[i].sceneId = 21;
				memcpy(pgw_info->scene_list[i].sceneName,"HomeMode",sizeof("HomeMode"));
			}else if(i == 2)
			{
				pgw_info->scene_list[i].sceneId = 22;
				memcpy(pgw_info->scene_list[i].sceneName,"SleepingMode",sizeof("SleepingMode"));
			}else{
				devNumInfo.sceneIndex[i] = INVALID_SCENE_LINKAGE_INDEX;
				devNumInfo.sceneNum -- ;
			}
			needSaveNumToFlash = 1;
            }
        }
    }
    
    tempDataLen = sizeof(linkageFlashInfo);
    
    for(i = 0; i < MAX_LINKAGE_NUM; i ++)
    {
        if(devNumInfo.linkageIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
        {
            tempaddr = FLASH_BASIC_ADDR + START_ADDR_FLASH_LINKAGE_OFFSET+ i * BASIC_LINKAGE_INFO_SIZE;
	     qca_load_flash_params(tempaddr,(uint8_t *)&linkageFlashInfo,tempDataLen);
            if((linkageFlashInfo.head == 0xaa) && (linkageFlashInfo.end == 0x55))
            {
                memcpy((uint8_t *)(&pgw_info->linkage_list[i]),(uint8_t *)&linkageFlashInfo,tempDataLen); 
            }else{
               devNumInfo.linkageIndex[i] = INVALID_SCENE_LINKAGE_INDEX ;
               devNumInfo.linkageNum --;
               needSaveNumToFlash = 1;
            }
        }
    }
    if(needSaveNumToFlash == 1)
    {
        userFlashWriteNumFunc((uint8_t *)&devNumInfo);
    }
    initOtaStationInfo();
}

void userSaveAllInfoToFlash(void)
{
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	HM_DEV_RAM_LIST *pni_tmp;
	HM_DEV_FLASH_INFO tempFlashData;
	uint16_t tempDataLen;
	uint8_t i;
	//save dev num to flash
	userFlashWriteNumFunc((uint8_t *)&devNumInfo);
	
	//save dev info to flash()
	//normal
	/*if(pni != NULL)
	{
		pni_tmp = pni;
		do{
			if(pni_tmp->batteryAlarm==NULL)
			 {printf("0 -----error\n");
				pni_tmp->batteryAlarm=0;
				}
			else
				printf("x0 -----error=%d\n",pni_tmp->batteryAlarm);
			tempFlashData.batteryAlarm = pni_tmp->batteryAlarm;
			//else
				//printf("1 -----error\n");
			if(pni_tmp->batteryRemain!=NULL)
		 	tempFlashData.batteryRemain = pni_tmp->batteryRemain;
			else
				printf("2 -----error\n");
			if(pni_tmp->devType)
		 	tempFlashData.devType = pni_tmp->devType;
			else
				printf("3 -----error\n");
			if(pni_tmp->epNum)
		 	tempFlashData.epNum = pni_tmp->epNum;
			else
				printf("4 -----error\n");
		 	memcpy((uint8_t *)tempFlashData.epList,(uint8_t *)pni_tmp->epList,sizeof(EP_INFO) * SUPORT_MAX_EP_NUM);
		 	if(pni_tmp->index)
		 	tempFlashData.index = pni_tmp->index;
			else
				printf("5 -----error\n");
			if(pni_tmp->onLine!=NULL)
		 	tempFlashData.onLine = pni_tmp->onLine;
			else
				printf("6 -----error\n");
			if(pni_tmp->shortAddr)
		 	tempFlashData.shortAddr = pni_tmp->shortAddr;
			else
				printf("7 -----error\n");
			if(pni_tmp->zbSubType)
		 	tempFlashData.zbSubType = pni_tmp->zbSubType;
			else
				printf("8 -----error\n");
		 	memcpy((uint8_t *)tempFlashData.mac,(uint8_t *)pni_tmp->mac,8);

			//--2--//
		 	//memcpy((uint8_t *)tempFlashData.name,(uint8_t *)pni_tmp->name,NAME_MAX_LEN);
		 	//tempFlashData.softVersion = pni_tmp->softVersion;
			//tempFlashData.httpState = pni_tmp->httpState;
			//memcpy(tempFlashData.roomId,pni_tmp->roomId,ROOM_ID_MAX_LEN);
			//--2--//
			memcpy(tempFlashData.modeId,pni_tmp->modeId,ZIGBEE_MODEID_LEN);
		 	tempFlashData.head = 0xaa;
		 	tempFlashData.end = 0x55;
			userFlashWriteDevInfoFunc((uint8_t *)&tempFlashData,pni_tmp->flashAddr);

			pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
		}while (pni_tmp != pni);
	}*/
	if(pni != NULL)
	{
		pni_tmp = pni;
		do{
			if(pni_tmp->batteryAlarm==NULL)
			 {printf("0 -----error\n");
				pni_tmp->batteryAlarm=0;
				}
			else
				printf("x0 -----error=%d\n",pni_tmp->batteryAlarm);
			tempFlashData.batteryAlarm = pni_tmp->batteryAlarm;
			//else
				//printf("1 -----error\n");
			if(pni_tmp->batteryRemain!=NULL)
		 	tempFlashData.batteryRemain = pni_tmp->batteryRemain;
			else
				printf("2 -----error\n");
			if(pni_tmp->devType)
		 	tempFlashData.devType = pni_tmp->devType;
			else
				printf("3 -----error\n");
			if(pni_tmp->epNum)
		 	tempFlashData.epNum = pni_tmp->epNum;
			else
				printf("4 -----error\n");
		 	memcpy((uint8_t *)tempFlashData.epList,(uint8_t *)pni_tmp->epList,sizeof(EP_INFO) * SUPORT_MAX_EP_NUM);
		 	if(pni_tmp->index!= NULL)
		 	tempFlashData.index = pni_tmp->index;
			else
				printf("5 -----error\n");
			if(pni_tmp->onLine!=NULL)
		 	tempFlashData.onLine = pni_tmp->onLine;
			else
				printf("6 -----error\n");
			if(pni_tmp->shortAddr)
		 	tempFlashData.shortAddr = pni_tmp->shortAddr;
			else
				printf("7 -----error\n");
			if(pni_tmp->zbSubType)
		 	tempFlashData.zbSubType = pni_tmp->zbSubType;
			else
				printf("8 -----error\n");
		 	memcpy((uint8_t *)tempFlashData.mac,(uint8_t *)pni_tmp->mac,8);

			//--2--//
		 	//memcpy((uint8_t *)tempFlashData.name,(uint8_t *)pni_tmp->name,NAME_MAX_LEN);
		 	//tempFlashData.softVersion = pni_tmp->softVersion;
			//tempFlashData.httpState = pni_tmp->httpState;
			//memcpy(tempFlashData.roomId,pni_tmp->roomId,ROOM_ID_MAX_LEN);
			//--2--//
			memcpy(tempFlashData.modeId,pni_tmp->modeId,ZIGBEE_MODEID_LEN);
		 	tempFlashData.head = 0xaa;
		 	tempFlashData.end = 0x55;
			userFlashWriteDevInfoFunc((uint8_t *)&tempFlashData,pni_tmp->flashAddr);

			pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
		}while (pni_tmp != pni);
	}
	//save linkage to flash
	tempDataLen = sizeof(LINKAGEINFO);
	for(i = 0; i < MAX_LINKAGE_NUM;i ++)
	{
		if(devNumInfo.linkageIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
		{
			userFlashWriteSceneLinkageInfoFunc((uint8_t*) &pgw_info->linkage_list[i],FLASH_BASIC_ADDR + START_ADDR_FLASH_LINKAGE_OFFSET+ i * BASIC_LINKAGE_INFO_SIZE,tempDataLen);
		}
	}

	//save scene to flash
	tempDataLen = sizeof(SCENEINFO);
	for(i = 0; i < MAX_SCENE_NUM;i ++)
	{
		if(devNumInfo.sceneIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
		{
			userFlashWriteSceneLinkageInfoFunc((uint8_t*) &pgw_info->scene_list[i],FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET+ i * BASIC_SCENE_INFO_SIZE,tempDataLen);
		}
	}

	//save sys info to flash
	userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);
}

void userCheckSaveFlashAndReset(uint8_t flag)
{
	
	if(flag == 1)
	{
		userSaveAllInfoToFlash();
		us_delay(10000);
	}
	qcom_sys_reset();
}

void userUpdateDevInfoDataToFlash(HM_DEV_RAM_LIST *pni)
{
	HM_DEV_FLASH_INFO tempFlashData;
	if(pni == NULL)
	{
		printf("the input point was null\r\n");
		return;
	}
	tempFlashData.batteryAlarm = pni->batteryAlarm;
	tempFlashData.batteryRemain = pni->batteryRemain;
	tempFlashData.devType = pni->devType;
	tempFlashData.epNum = pni->epNum;
	memcpy(tempFlashData.epList,pni->epList,sizeof(EP_INFO) * SUPORT_MAX_EP_NUM);
	tempFlashData.index = pni->index;
	memcpy(tempFlashData.mac,pni->mac,8);
	memcpy(tempFlashData.name,pni->name,NAME_MAX_LEN);
	tempFlashData.onLine = pni->onLine;
	tempFlashData.shortAddr = pni->shortAddr;
	tempFlashData.zbSubType = pni->zbSubType;
	tempFlashData.softVersion = pni->softVersion;
	tempFlashData.httpState = pni->httpState;
	memcpy(tempFlashData.roomId,pni->roomId,ROOM_ID_MAX_LEN);
	memcpy(tempFlashData.modeId,pni->modeId,ZIGBEE_MODEID_LEN);
	tempFlashData.head = 0xaa;
	tempFlashData.end = 0x55;
	userFlashWriteDevInfoFunc((uint8_t *)&tempFlashData,pni->flashAddr);
	printf("\n ------userUpdateDevInfoDataToFlash --pni->flashAddr:%x\r\n",pni->flashAddr);
}

void userAddDevNumToNumInfo(uint16_t index)
{
	uint8_t tempIndex;
	if((index >= BASIC_DEV_INDEX) && (index < 30000))
	{
		tempIndex = index % 100;
		devNumInfo.devNum ++;
		devNumInfo.devIndex[tempIndex] = index;
		devNumInfo.head = 0xaa;
		devNumInfo.end = 0x55;
		userFlashWriteNumFunc((uint8_t *)&devNumInfo);
	}else{
		printf("the index was not valid\r\n");
	}
}

void userDeleteOneDevFromRamAndFlash(HM_DEV_RAM_LIST *pni)
{
	uint8_t tempIndex;
	if(pni == NULL)
	{
		printf("delete dev point was null\r\n");
		return;
	}
	tempIndex = pni->index % 100;
	userFlashDeleteOneNodeInfo(pni->flashAddr);
	removeNodeList(pni);
	devNumInfo.devNum -- ;
	devNumInfo.devIndex[tempIndex] = INVALID_DEV_INDEX;
	devNumInfo.head = 0xaa;
	devNumInfo.end = 0x55;
	userFlashWriteNumFunc((uint8_t *)&devNumInfo);
}

void userDeleteDevFromLightNightAndHomeArm(uint16_t index)
{
	uint8_t i;
	uint8_t needSaveFlash = 0;
	printf("delete dev in light night and home arm\r\n");
	for(i = 0; i < MAX_HOME_ARM_FOR_SENSOR_NUM; i ++)
	{
		if(hmIotsDevInfo.gwLightForNight.index[i] == index)
		{
			hmIotsDevInfo.gwLightForNight.index[i] = 0;
			needSaveFlash = 1;
			break;
		}
	}

	for(i = 0; i < MAX_HOME_ARM_FOR_SENSOR_NUM; i ++)
	{
		if(hmIotsDevInfo.home_arm_for_dev.index[i] == index)
		{
			hmIotsDevInfo.home_arm_for_dev.index[i] = 0;
			needSaveFlash = 1;
			break;
		}
	}

	if(needSaveFlash == 1)
	{
		userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);
	}
}

void userDeleteDevFromWDLinkList(uint16_t index)
{
	uint8_t i;
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	printf("delete dev in wd dev link list\r\n");
	if (pni==NULL)
	{
		printf("in dev link wd:the ram is empty\r\n");
		return;
	}
	do{
		if (pni_tmp->devType == NODE_INFO_TYPE_WARNING_DEV)
		{
			for(i = 0; i < MAX_DEV_LINK_WD_NUM; i ++)
			{
				if(pni_tmp->epList[0].devData.wdStateApp.devLinkWd.zLinkWd[i].zIndex == index)
				{
					pni_tmp->epList[0].devData.wdStateApp.devLinkWd.zLinkWd[i].zIndex = 0;
					pni_tmp->epList[0].devData.wdStateApp.devLinkWd.zLinkWd[i].zAlarmType = 0;

					userUpdateDevInfoDataToFlash(pni_tmp);
					break;
				}
			}
		}
			
		pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
	}while (pni_tmp != pni);	
}

void userDeleteDevFromSceneAndLinkage(uint8_t index)
{

	uint8_t i,j;
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni_temp;
	uint8_t outputNum = 0;
	uint8_t sceneInfoSaveToFlash = 0; 
	uint8_t linkInfoSaveToFlash = 0;
	uint8_t sceneNum = 0;
	printf("delete dev in scene and linkage\r\n");
	for(i = 0; i < MAX_SCENE_NUM; i ++)
	{
		if(devNumInfo.sceneIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
		{
			printf("the scene id:%d\r\n",devNumInfo.sceneIndex[i]);
			for(j = 0; j < MAX_OUTPUT_NUM; j ++)
			{
				if (pgw_info->scene_list[i].outDevInfo[j].valid == 1)
				{

					if(pgw_info->scene_list[i].outDevInfo[j].devAction.modeByte != 0)
					{
						if(pgw_info->scene_list[i].outDevInfo[j].type == SCENE_LIST_ZB_DEV_PAYLOAD)//dev output
						{
							if(pgw_info->scene_list[i].outDevInfo[j].targetId == index)
							{
								memset((char*)&pgw_info->scene_list[i].outDevInfo[j],0,sizeof(OUTPUTINFO));
								sceneInfoSaveToFlash = 1;
							}
						}
					} 		
				}
			} 
			if(sceneInfoSaveToFlash == 1)
			{
				sceneInfoSaveToFlash = 0;
				userFlashWriteSceneLinkageInfoFunc((uint8_t*) &pgw_info->scene_list[i],FLASH_BASIC_ADDR + START_ADDR_FLASH_SCENE_OFFSET+ i * BASIC_SCENE_INFO_SIZE,sizeof(SCENEINFO));
			}
			
		}
	}

	
	 //遍历整个联动表
	for(i = 0; i < MAX_LINKAGE_NUM; i ++)
	{ 
		if(devNumInfo.linkageIndex[i] != INVALID_SCENE_LINKAGE_INDEX)
		{
			printf("the linkage id:%d\r\n",devNumInfo.linkageIndex[i]); 
			//联动输出缓存有效
			if(pgw_info->linkage_list[i].outputNum > 0)
			{ 
				//遍历输出表
				for(j = 0; j < MAX_OUTPUT_NUM; j ++)
				{ 
					if(pgw_info->linkage_list[i].outputInfo[j].valid == 1)
					{
						if(pgw_info->linkage_list[i].outputInfo[j].type == SCENE_LIST_ZB_DEV_PAYLOAD)//dev output
						{
							if(pgw_info->linkage_list[i].outputInfo[j].targetId == index)
							{
								memset((char*)&pgw_info->linkage_list[i].outputInfo[j],0,sizeof(OUTPUTINFO));
								linkInfoSaveToFlash = 1;
							}
						}  
							
					}
				} 
			} 


			//触发条件读取，如果联动没有触发条件，则删除该联动
			 
			if(pgw_info->linkage_list[i].inputInfo.valid == 1)
			{
				if(pgw_info->linkage_list[i].inputInfo.type == SCENE_LIST_ZB_DEV_PAYLOAD)
				{
					if(pgw_info->linkage_list[i].inputInfo.IN.devIn.targetId == index)
					{
						memset((char*) &pgw_info->linkage_list[i],0,sizeof(LINKAGEINFO));
						linkInfoSaveToFlash = 1;
					}
				} 
			}
 

			//遍历检测条件缓存，检测条件不是必须条件，联动中可以没有检测条件
			 
			if(pgw_info->linkage_list[i].checkNum > 0)
			{
				for(j = 0; j < MAX_CHECK_NUM; j ++)
				{ 
					if(pgw_info->linkage_list[i].checkInfo[j].valid == 1)
					{
						if(pgw_info->linkage_list[i].checkInfo[j].type == SCENE_LIST_ZB_DEV_PAYLOAD)
						{
							if(pgw_info->linkage_list[i].checkInfo[j].check.devCheck.targetId == index)
							{
								memset((char*)&pgw_info->linkage_list[i].checkInfo[j],0,sizeof(CHCEKINFO));
								linkInfoSaveToFlash = 1;
							}
						} 
					 }
		 		}
			} 
			if(linkInfoSaveToFlash == 1)
			{
				linkInfoSaveToFlash = 0;
				userFlashWriteSceneLinkageInfoFunc((uint8_t*) &pgw_info->linkage_list[i],FLASH_BASIC_ADDR + START_ADDR_FLASH_LINKAGE_OFFSET+ i * BASIC_SCENE_INFO_SIZE,sizeof(LINKAGEINFO));
			}
			
		}
	}

}




