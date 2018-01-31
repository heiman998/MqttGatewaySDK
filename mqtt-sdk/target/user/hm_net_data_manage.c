#include "qcom_common.h"
#include "qca_json.h"
#include "hm_app.h"
#include "qca_base64.h"
#include "stdlib.h"
#include "hm_net_data_manage.h"
#include "hm_keyAndSmartLink.h"
#include "xlink_All_Device.h"
#include "Xlink_Head_Adaptation.h"
#include "hm_httpEvent.h"
#include "hm_zigbeeInfohand.h"
#include "hm_reportDevToCloud.h"
#include "hm_zigbeeStateDeal.h"
#include "hm_halBeep.h"
#include "hm_sceneAndLinkage.h"

#include "hm_network_data_manage.h"


extern HM_GW_LIST gw_info;
extern BEEP_INFO beep_info;
extern DEV_NUM_INFO devNumInfo;
extern TX_SEMAPHORE beep_semaphore_ptr;
extern GWSTATFORTIME gwStateForRealTime;
extern HTTPSSENDBUF httpsSendBuf[MAX_HTTPS_SEND_BUF_NUM];
NETSENDINFO netSendInfo;
NETRCVINFO   netRcvInfo;
HMIOTS_STRUCT zigbeeFuncToNetBuf[MAX_IOTS_BUF_SIZE];
extern DEV_MYSELF_INFO hmIotsDevInfo;
extern tGlobal_net_type tGlobal_net;
WIFICMDWATIRSP wifiCmdWaitRspBuf[SPECIALCMDNUM];
extern qcom_timer_t gwNightLightTimer;

extern unsigned char clear_zigbeenet_flag;


uint16_t user_aes128_encode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key)
{
	
 	uint8_t *tempdst = NULL;
	uint16_t dstlen,base64_len;
	uint16_t block,i;
	if((src == NULL) )//|| (key == NULL)
	{
		printf("user_aes_encode:input the src data was null");
		return 0;
	}
	if((key == NULL))
	{
		printf("key user_aes_encode:input the src data was null");
		return 0;
	}
	if(dst == NULL)
	{
		printf("user_aes_encode:input the dst data was null");
		return 0;
	}
	//Encrypt(lpIn,nExpKey,lpOut);
	
	block = srclen >> 4;
	dstlen = (block << 4) + 16;
	
	printf("need malloc size:%d\r\n",dstlen);
	tempdst = mem_alloc(dstlen);
	if(tempdst == NULL)
	{
		printf("user_aes_encode:malloc tempdst erron\r\n ");
		return 0;
	}

	printf(" Express data-----------:\n");
	for(i=0;i<srclen;i++)
	{
		printf("%02x ",src[i]);
	}
	dstlen=EncryptBuf((char*)src, (uint32_t)srclen,(char*)dst,(char*)key);
	//printf("\n ----the ace and  data len:%d,string:%s\r\n",dstlen,dst); 


	printf("\n Encrypted data-----------:\n");
	for(i=0;i<dstlen;i++)
	{
		printf("%02x ",dst[i]);
	}
	//base64_len = base64_encode(tempdst,dstlen,(char *)dst);//实际返回长度有有效长度，不包含==
	//printf("the ace and base64 data len:%d,string:%s\r\n",base64_len,dst); 
	//dst=&tempdst;
	//printf("----the ace and  data len:%d,string:%s\r\n",srclen+16,dst); 
	if(tempdst != NULL)
	{
		mem_free(tempdst);
		tempdst = NULL;
	}
	return dstlen;
}


uint16_t user_aes128_decode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key)
{
	int8_t *tempsrc = NULL;
	uint16_t templen,base64_len,i;
	if((src == NULL) || (key == NULL))
	{
		printf("user_aes_encode:input the src data was null");
		return 0;
	}
	if(dst == NULL)
	{
		printf("user_aes_encode:input the dst data was null");
		return 0;
	}
	tempsrc = mem_alloc(srclen);
	if(tempsrc == NULL)
	{
		printf("user_aes_encode:malloc tempsrc erron\r\n ");
		return 0;
	}
	
	//base64_len = base64_decode(src,tempsrc);
	{
		//printf("the base 64 decode len:%d\r\n",base64_len);
		printf("\n Encrypted data-----------:\n");
		for(i=0;i<srclen;i++)
		{
			printf("%02x ",src[i]);
		}
		//templen = DecryptBuf((char*)tempsrc, (uint32_t)base64_len, (char*)dst,(char*)key);
		templen = DecryptBuf((char*)src, (uint32_t)srclen, (char*)dst,(char*)key);
		if(templen != 0)
		{
		
			dst[templen] = '\0';

			printf("the ace decode data len:%d,string:%s\r\n",templen,dst); 
		}else{
			printf("aes decode was error\r\n");
			templen = 0;
		}
	}
	printf(" Express data-----------:\n");
	for(i=0;i<templen;i++)
	{
		printf("%02x ",dst[i]);
	}
	
	if(tempsrc != NULL)
	{
		mem_free(tempsrc);
		tempsrc = NULL;
	}
	return templen;
}

uint16_t user_aes_encode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key)
{
	
 	uint8_t *tempdst = NULL;
	uint16_t dstlen,base64_len;
	uint16_t block;
	if((src == NULL) || (key == NULL))
	{
		printf("user_aes_encode:input the src data was null");
		return 0;
	}
	if(dst == NULL)
	{
		printf("user_aes_encode:input the dst data was null");
		return 0;
	}
	block = srclen >> 4;
	dstlen = (block << 4) + 16;
	
	printf("need malloc size:%d\r\n",dstlen);
	tempdst = mem_alloc(dstlen);
	if(tempdst == NULL)
	{
		printf("user_aes_encode:malloc tempdst erron\r\n ");
		return 0;
	}



	if(dstlen != EncryptBuf((char*)src, (uint32_t)srclen,(char*)tempdst,(char*)key))
	{
		printf("aes en code lenth was error\r\n");
		return 0;
	}
	base64_len = base64_encode(tempdst,dstlen,(char *)dst);//实际返回长度有有效长度，不包含==
	printf("the ace and base64 data len:%d,string:%s\r\n",base64_len,dst); 

	if(tempdst != NULL)
	{
		mem_free(tempdst);
		tempdst = NULL;
	}
	return base64_len;
}


uint16_t user_aes_decode(uint8_t *src,uint16_t srclen,uint8_t *dst,uint8_t *key)
{
	int8_t *tempsrc = NULL;
	uint16_t templen,base64_len;
	if((src == NULL) || (key == NULL))
	{
		printf("user_aes_encode:input the src data was null");
		return 0;
	}
	if(dst == NULL)
	{
		printf("user_aes_encode:input the dst data was null");
		return 0;
	}
	tempsrc = mem_alloc(srclen);
	if(tempsrc == NULL)
	{
		printf("user_aes_encode:malloc tempsrc erron\r\n ");
		return 0;
	}
	
	base64_len = base64_decode(src,tempsrc);
	if(base64_len != 0)
	{
		printf("the base 64 decode len:%d\r\n",base64_len);
		
		templen = DecryptBuf((char*)tempsrc, (uint32_t)base64_len, (char*)dst,(char*)key);
		if(templen != 0)
		{
		
			dst[templen] = '\0';

			printf("the ace decode data len:%d,string:%s\r\n",templen,dst); 
		}else{
			printf("aes decode was error\r\n");
			templen = 0;
		}
	}else{
		printf("base64 decode was error\r\n");
		templen = 0;
	}
	if(tempsrc != NULL)
	{
		mem_free(tempsrc);
		tempsrc = NULL;
	}
	return templen;
}

/**************************************************************
//发送给app数据的缓存填充，包括加密处理
//appDevId为0xffffffff时表示app全部透传
***************************************************************/
int userPkSendBuffer(char *data,unsigned int len,uint8_t rcvType,uint32_t appDevId,unsigned int cid,uint8_t encodeFlag)
{
		uint16_t encodeLen;
		if((data == NULL) || (len == 0) || (len > 800))
		{
			printf("pk send data was null!\n\r");
			return -1;
		}
		if(netSendInfo.netsendnum >= NET_SEND_PK_NUM)
		{
			printf("the send buf is full\r\n");
			return -1;
		}

		if(encodeFlag)
		{
			user_aes_encode(data,len,&netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC + 1],hmIotsDevInfo.aesKey);
			encodeLen = strlen(&netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC + 1]);
			netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC] = '^';
			netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC + encodeLen + 1]  = '&';
			netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC + encodeLen + 2]  = '\0';
			#ifdef  HM_DEBUG
			printf("the encode len:%d,encode data:%s",encodeLen,&netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC]);
			#endif
		}else{
			memcpy(&netSendInfo.netsendbuf[netSendInfo.netsendnum * NET_SEND_BUF_BASIC],data,len);
		}
		
		netSendInfo.netappinfo[netSendInfo.netsendnum].sendCid = cid;
	
		netSendInfo.netappinfo[netSendInfo.netsendnum].dealType = rcvType;
		netSendInfo.netappinfo[netSendInfo.netsendnum].appDevId = appDevId;
		netSendInfo.netsendnum ++;
		netSendInfo.netsendflag = 1;

		return 0;
}




/**********************************************************
json_newPL : json PL字段
RC - 返回状态值，在主动上报的时候忽略该值
CID - 设备上报指令，具体区分为设置回复、获取回复、主动上报
SID- app id值
TEID- app 用户名称
sn-设备通信编号，回复指令时应与下发指令sn一致，上报指令时可以忽略该值
security -是否加密，1为加密，0为非加密
函数说明:网关与app通信时的封包指令，并写入缓存中
*********************************************************/
void userCreateJsplToBuf(json_t *json_newPL,int8_t rc,uint16_t cid,char *sid,char *teid,uint32_t sn,uint8_t security,uint8_t rcvType,uint32_t appDevId)
{
	char *payload=NULL;
	json_t *jsMain;
	jsMain = json_create_object();
	if(jsMain == NULL)
	{
		printf("create js was error\r\n");
		return;
	}
	
	do
	{
		if((cid == MESSAGE_DEV_STATE_UP) || (cid == MESSAGE_RSP_GETINFO))
		{
			
			if(json_newPL ==  NULL)
			{
				json_add_item_to_object(jsMain,"PL",json_create_string("{}"));
			}else{
				json_add_item_to_object(jsMain,"PL",json_newPL);
			}
			
			json_add_item_to_object(jsMain,"ENCRYPT",json_create_number(security));
			
			json_add_item_to_object(jsMain,"RC",json_create_number(rc));
			
			json_add_item_to_object(jsMain,"SN",json_create_number(sn));
			
			json_add_item_to_object(jsMain,"CID",json_create_number(cid));
			

			payload = json_PrintUnformatted(jsMain);
			if(payload == NULL)
			{
				printf("js printf payload was error\r\n");
				break;
			}
			printf("test pl:%s\r\n",payload);
		}else{
			if(json_newPL ==  NULL)
			{
				json_add_item_to_object(jsMain,"PL",json_create_string("{}"));
			}else{
				json_add_item_to_object(jsMain,"PL",json_newPL);
			}
			json_add_item_to_object(jsMain,"ENCRYPT",json_create_number(security));
			json_add_item_to_object(jsMain,"RC",json_create_number(rc));
			json_add_item_to_object(jsMain,"SN",json_create_number(sn));
			json_add_item_to_object(jsMain,"CID",json_create_number(cid));
			json_add_item_to_object(jsMain,"TEID",json_create_string(teid));
			json_add_item_to_object(jsMain,"SID",json_create_string(sid));
			payload = json_PrintUnformatted(jsMain);
			if(payload == NULL)
			{
				printf("js printf payload was error\r\n");
				break;
			}
		}
		#ifdef  HM_DEBUG
		printf("the paylaod:%s\r\n",payload);
		#endif
		userPkSendBuffer(payload,strlen(payload),rcvType,appDevId,cid,security);
		if(payload != NULL)
		{
			mem_free(payload);
			payload = NULL;
		}
	}while(0);
	

	if(jsMain != NULL)
	{
		json_delete(jsMain);
		jsMain = NULL;
	}
}

json_t* userCreatNodeBasicListInfo(HM_DEV_RAM_LIST *pni)
{
	json_t *jsMain;
	char tempMac[20];
	memset(tempMac,'\0',20);
	sprintf(tempMac,"%02X%02X%02X%02X%02X%02X%02X%02X", pni->mac[0],pni->mac[1],pni->mac[2],
												pni->mac[3],pni->mac[4],pni->mac[5],pni->mac[6],pni->mac[7]);
	jsMain = json_create_object();
	json_add_item_to_object(jsMain,"ZX",json_create_number(pni->index));
	json_add_item_to_object(jsMain,"ZT",json_create_number(pni->devType));
	json_add_item_to_object(jsMain,"ZM",json_create_string(tempMac));
	json_add_item_to_object(jsMain,"ZN",json_create_string(pni->name));
	//json_add_item_to_object(jsMain,"RD",json_create_string(pni->roomId));
	return jsMain;
}

json_t* userCreatNodeStatListInfo(HM_DEV_RAM_LIST *pni)
{
	json_t *jsSta = NULL;
	json_t *jsMain = NULL;
	DEVINFOOPTION devOption;
	devOption.modeByte = ALL_DEV_STATE_ACTION;
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
		case NODE_INFO_TYPE_MAGNET_DOOR:
		case NODE_INFO_TYPE_PIR_SENSOR:
		case NODE_INFO_TYPE_VIBRATION:
			jsSta = userCreateIasStaObject(pni,devOption);
			break;
		case NODE_INFO_TYPE_REMOTE_CONTROL:
			jsSta = userCreateAceStaObject(pni,devOption);
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
			break;
		default:
			break;
	}
	if(jsSta == NULL)
	{
		jsSta = json_create_object();
	}
	jsMain = json_create_object();
	json_add_item_to_object(jsMain,"SS",jsSta);
	json_add_item_to_object(jsMain,"ZX",json_create_number(pni->index));
	return jsMain;
}

void userSendAllNodeBasicListJs(uint32_t sn,uint8_t rcvType,uint32_t appDevId)
{
	json_t *jsMain,*jsArray,*jsOid;
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	uint8_t allDevPk = devNumInfo.devNum / 8 + 1;//总数据帧数量
	uint8_t devListPkNum = 0;	//第多少帧
	uint8_t cpDevListPkNum = 0xff;
	uint8_t inPutNum = 0;//帧dev设备数量
	if (pni==NULL)
	{
		printf("the ram is empty\r\n");
		jsMain = json_create_object();
		jsArray = json_create_array();
		jsOid = json_create_object();
		devListPkNum ++;
		inPutNum = 0;
		json_add_item_to_object(jsOid,"DEV",jsArray);
		json_add_item_to_object(jsOid,"PG",json_create_number(1));
		json_add_item_to_object(jsOid,"NU",json_create_number(1));
		json_add_item_to_object(jsMain,GW_GET_NODE_LIST_OID,jsOid);
		userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
	}else{
		do{
			if(cpDevListPkNum != devListPkNum)
			{
				cpDevListPkNum = devListPkNum;
				jsArray = json_create_array();
			}
			inPutNum ++;
			json_add_item_toarray(jsArray,userCreatNodeBasicListInfo(pni_tmp));
			if(inPutNum == MAX_DEV_BASIC_LIST_NUM_FOR_PK)
			{
				jsMain = json_create_object();
				jsOid = json_create_object();
				devListPkNum ++;
				inPutNum = 0;
				json_add_item_to_object(jsOid,"DEV",jsArray);
				json_add_item_to_object(jsOid,"PG",json_create_number(allDevPk));
				json_add_item_to_object(jsOid,"NU",json_create_number(devListPkNum));
				json_add_item_to_object(jsMain,GW_GET_NODE_LIST_OID,jsOid);
				userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
				if(jsMain != NULL)
				{
					json_delete(jsMain);
					jsMain = NULL;
				}
			}
			pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
		}while (pni_tmp != pni);

		if(inPutNum != 0)
		{
			jsMain = json_create_object();
			jsOid = json_create_object();
			devListPkNum ++;
			inPutNum = 0;
			json_add_item_to_object(jsOid,"DEV",jsArray);
			json_add_item_to_object(jsOid,"PG",json_create_number(allDevPk));
			json_add_item_to_object(jsOid,"NU",json_create_number(devListPkNum));
			json_add_item_to_object(jsMain,GW_GET_NODE_LIST_OID,jsOid);
			userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
			if(jsMain != NULL)
			{
				json_delete(jsMain);
				jsMain = NULL;
			}
		}
	}
	
}


void userSendAllNodeStatListJs(uint32_t sn,uint8_t rcvType,uint32_t appDevId)
{
	json_t *jsMain,*jsArray,*jsOid;
	HM_GW_LIST *pgw_info=&gw_info;
	HM_DEV_RAM_LIST *pni= (HM_DEV_RAM_LIST *)pgw_info->pnode_list;
	HM_DEV_RAM_LIST *pni_tmp=pni;
	uint8_t allDevPk = devNumInfo.devNum / 8 + 1;
	uint8_t devListPkNum = 0;
	uint8_t cpDevListPkNum = 0xff;
	uint8_t inPutNum = 0;
	if (pni==NULL)
	{
		printf("the ram is empty\r\n");
		jsMain = json_create_object();
		jsArray = json_create_array();
		jsOid = json_create_object();
		devListPkNum ++;
		inPutNum = 0;
		json_add_item_to_object(jsOid,"DEV",jsArray);
		json_add_item_to_object(jsOid,"PG",json_create_number(1));
		json_add_item_to_object(jsOid,"NU",json_create_number(1));
		json_add_item_to_object(jsMain,GW_APP_GET_DEV_STAT_LIST_OID,jsOid);
		userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
		if(jsMain != NULL)
		{
			json_delete(jsMain);
			jsMain = NULL;
		}
	}else{
		do{
			if(cpDevListPkNum != devListPkNum)
			{
				cpDevListPkNum = devListPkNum;
				jsArray = json_create_array();
			}
			inPutNum ++;
			json_add_item_toarray(jsArray,userCreatNodeStatListInfo(pni_tmp));
			if(inPutNum == MAX_DEV_STAT_LIST_NUM_FOR_PK)
			{
				jsMain = json_create_object();
				jsOid = json_create_object();
				devListPkNum ++;
				inPutNum = 0;
				json_add_item_to_object(jsOid,"DEV",jsArray);
				json_add_item_to_object(jsOid,"PG",json_create_number(allDevPk));
				json_add_item_to_object(jsOid,"NU",json_create_number(devListPkNum));
				json_add_item_to_object(jsMain,GW_APP_GET_DEV_STAT_LIST_OID,jsOid);
				userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
				if(jsMain != NULL)
				{
					json_delete(jsMain);
					jsMain = NULL;
				}
			}
			pni_tmp=(HM_DEV_RAM_LIST *)pni_tmp->pnext_dev_info;
		}while (pni_tmp != pni);

		if(inPutNum != 0)
		{
			jsMain = json_create_object();
			jsOid = json_create_object();
			devListPkNum ++;
			inPutNum = 0;
			json_add_item_to_object(jsOid,"DEV",jsArray);
			json_add_item_to_object(jsOid,"PG",json_create_number(allDevPk));
			json_add_item_to_object(jsOid,"NU",json_create_number(devListPkNum));
			json_add_item_to_object(jsMain,GW_APP_GET_DEV_STAT_LIST_OID,jsOid);
			userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,sn,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
			if(jsMain != NULL)
			{
				json_delete(jsMain);
				jsMain = NULL;
			}
		}
	}
	
}


json_t* userCreatNodeStateJsFunc(HM_DEV_RAM_LIST *pni,uint8_t stateMode,DEVINFOOPTION devOption)
{
	json_t *jsMain = NULL;
	json_t *jsUpToMode = NULL;
	char oid[25];
	uint8_t lastAlarmOid[] = ".31";
	uint8_t lastGetSetOid[] = ".32";

	memset(oid,'\0',25);
	jsMain = json_create_object();
	if(jsMain != NULL)
	{
		switch(pni->devType)
		{
			case NODE_INFO_TYPE_LIGHT_RGB:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_RGB_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateRgbStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_RGB_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateRgbStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_RGB_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateRgbSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateRgbStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_AI1SW:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_AISW1_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW1_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW1_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateAiSwitchSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateAiSwitchStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_AI2SW:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_AISW2_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW2_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW2_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateAiSwitchSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateAiSwitchStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_AI3SW:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_AISW3_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW3_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateAiSwitchStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_AISW3_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateAiSwitchSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateAiSwitchStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_ZB_IR_TRANSPONDER:
				break;
			case NODE_INFO_TYPE_RELAY_CONTROL:
				break;
			case NODE_INFO_TYPE_MAGNET_DOOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_DOOR_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_DOOR_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_DOOR_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_WATER_SENSOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_WATER_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_WATER_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_WATER_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_PIR_SENSOR:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_PIR_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_PIR_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_PIR_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_SMOKE_SENSOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_SMOKE_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_SMOKE_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_SMOKE_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}

				break;
			case NODE_INFO_TYPE_GAS_SENSOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_GAS_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_GAS_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_GAS_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_CO_SENSOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_CO_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_CO_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_CO_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
				
				break;
			case NODE_INFO_TYPE_VIBRATION:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_VIBRATION_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_VIBRATION_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_VIBRATION_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
				
				break;
			case NODE_INFO_TYPE_SOS:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_EB_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_EB_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateIasStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_EB_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateIasStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_TEMP_HUM_SENSOR:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_THP_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateTHPStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_THP_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateTHPStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_THP_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateTHPSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateTHPStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_WARNING_DEV:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_WD_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateWDStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_WD_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateWDStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_WD_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateWDSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateWDStaObject(pni,devOption);
				}
				break;
			case NODE_INFO_TYPE_ILLUMINATION:
				break;
			case NODE_INFO_TYPE_AIR_QUALITY:
				break;
			case NODE_INFO_TYPE_THERMOSTAT:
				break;
			

				break;
			case NODE_INFO_TYPE_REMOTE_CONTROL:

				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_ACERC_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateAceStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_ACERC_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateAceStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_ACERC_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateIasSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateAceStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_AI_SOCKET_USB:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_USB_PLUG_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreatePlugStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_USB_PLUG_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreatePlugStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_USB_PLUG_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreatePlugSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreatePlugStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_SMART_PLUG_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateSPlugStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_SMART_PLUG_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateSPlugStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_SMART_PLUG_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateSPlugSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateSPlugStaObject(pni,devOption);
				}
			
				break;
			case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
				if((stateMode == CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID) || (CREATE_DEV_STATE_FOR_UPDATE_STATE_OID == stateMode))
				{
					sprintf(oid,"%s%d",GW_APP_COM_CUTRGB4_OID,pni->index);
					json_add_item_to_object(jsMain,"SS",userCreateCutRgbStaObject(pni,devOption));
				}else if(stateMode == CREATE_dEV_STATE_FOR_ALARM_OID){
					sprintf(oid,"%s%d%s",GW_APP_COM_CUTRGB4_OID,pni->index,lastAlarmOid);
					json_add_item_to_object(jsMain,"SS",userCreateCutRgbStaObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID == stateMode){
					sprintf(oid,"%s%d%s",GW_APP_COM_CUTRGB4_OID,pni->index,lastGetSetOid);
					json_add_item_to_object(jsMain,"SE",userCreateCutRgbSettingObject(pni,devOption));
				}else if(CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID == stateMode){
					return userCreateCutRgbStaObject(pni,devOption);
				}
			
				break;			default:
				break;
		}

	}

	jsUpToMode = json_create_object();
	json_add_item_to_object(jsUpToMode,oid,jsMain);
	return jsUpToMode;
		
}


/************************************************************************
获取设备状态、设置信息，并组合成pl child的json  object，
该函数用于新设备上报(app 透传)、设备状态改变(app udp)，
获取设备状态/设置信息(app 透传)
************************************************************************/

json_t* userCreateNodeJsPl(HM_DEV_RAM_LIST *pni,uint8_t stateMode)
{
	//char strData[5];
	json_t *jsMain = NULL;
	DEVINFOOPTION devOption;
	char tempMac[20];
	memset(tempMac,'\0',20);
	sprintf(tempMac,"%02X%02X%02X%02X%02X%02X%02X%02X", pni->mac[0],pni->mac[1],pni->mac[2],
												pni->mac[3],pni->mac[4],pni->mac[5],pni->mac[6],pni->mac[7]);
	devOption.modeByte = ALL_DEV_STATE_ACTION;
	if(stateMode == CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID)
	{
		jsMain = json_create_object();
		//sprintf(strData,"%02x",pni->softVersion);
		//json_add_item_to_object(jsMain,"swVersion",json_create_string(strData));
		json_add_item_to_object(jsMain,"ZT",json_create_number(pni->devType));
		//json_add_item_to_object(jsMain,"RD",json_create_string(pni->roomId));
		json_add_item_to_object(jsMain,"ZM",json_create_string(tempMac));
		json_add_item_to_object(jsMain,"ZX",json_create_number(pni->index));
		json_add_item_to_object(jsMain,"ZN",json_create_string(pni->name));
		json_add_item_to_object(jsMain,"SS",userCreatNodeStateJsFunc(pni,CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID,devOption));
	}else{
		jsMain = userCreatNodeStateJsFunc(pni,stateMode,devOption);
	}
	
	return jsMain;
}

json_t* userCreateOTAJsPl(uint8_t otaMode,uint16_t devType,uint8_t status)
{
	char oid[15];
	json_t *jsMain = NULL;
	json_t *jsOb = NULL;

	if((otaMode == OTA_MODE_WIFI) || (otaMode == OTA_MODE_ZC) || (otaMode == OTA_MODE_ZED))
	{
		jsMain = json_create_object();

		memset(oid,'\0',15);
		sprintf(oid,"%s%d",GW_APP_OTA_OID,otaMode);
		json_add_item_to_object(jsMain,oid,jsOb = json_create_object());
		json_add_item_to_object(jsOb,"TY",json_create_number(devType));	
		json_add_item_to_object(jsOb,"RT",json_create_number(status));
	}else{
		printf("in create ota js:unknow ota mode\r\n");
	}
	return jsMain;
}


void userNetMemInit(void)
{
	netSendInfo.netsendflag = 0;
	netSendInfo.netsendnum  = 0;
	netSendInfo.netsendbuf = (char *)mem_alloc(NET_SEND_BUF_BASIC * NET_SEND_PK_NUM);
	if(netSendInfo.netsendbuf == NULL)
	{
		printf("net send buf malloc faild\r\n");
		userCheckSaveFlashAndReset(0);
	}
	netRcvInfo.netrcvflag = 0;
	netRcvInfo.netrcvnum  = 0;
	netRcvInfo.netrcvbuf = (char *)mem_alloc(NET_RCV_BUF_BASIC * NET_RCV_PK_NUM);
	if(netRcvInfo.netrcvbuf == NULL)
	{
		printf("net receive buf malloc faild\r\n");
		userCheckSaveFlashAndReset(0);
	}
}

int hmiotsDoRecvGetMibCallback(json_t *request, json_t *reply, int SN,uint8_t *encodeFlag,int8_t *rc,uint8_t rcvType,uint32_t appDevId)
{
	uint8_t *tempData;
	uint16_t tempLen;
	DEVINFOOPTION devOption;
	uint8_t key[16];
	json_t *jsOb,*jsArray;
	uint8_t dotNum;
	uint16_t intOid,lastOid;
	HM_DEV_RAM_LIST *pni_temp;
	devOption.modeByte = 0;
	printf("get teest\r\n");
	if((request == NULL) || (reply == NULL) || (encodeFlag == NULL))
	{
		printf("in do recv:the input data point was null\r\n");
		return 0;
	}
	if (strcmp(WIFI_DEV_BASIC_STATE_OID, request->valuestring) == 0)
	{
		json_add_item_to_object(reply,request->valuestring,userCreatWifiDevInfoJsObject(&hmIotsDevInfo));
		*rc = APP_CMD_RC_SUCCESS;
		*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		return 1;
	}else if (strcmp(WIFI_DEV_BASIC_AES_OID, request->valuestring) == 0){//get aes key

		tempData = mem_alloc(64);
		if(tempData == NULL)
		{
			printf("in do rcv net data:malloc mem was error\r\n");
			*rc = APP_CMD_RC_ERROR_UNKNOW;
			*encodeFlag = SEND_TO_APP_DATA_NOT_NEED_ENCODE;
			return 1;
		}
		memcpy(key,XLINK_PRODUCT_KEY,16);
		user_aes_encode(hmIotsDevInfo.aesKey,16,&tempData[1],key);
		tempLen = strlen(&tempData[1]);
		tempData[0] = '^';
		tempData[1 + tempLen] = '&';
		tempData[2 + tempLen] = '\0';
		printf("encode len:%d,encode data:%s",tempLen,tempData);
		*encodeFlag = SEND_TO_APP_DATA_NOT_NEED_ENCODE;
		*rc = APP_CMD_RC_SUCCESS;
		jsOb = json_create_object();
		json_add_item_to_object(jsOb,"key",json_create_string(tempData));
		json_add_item_to_object(reply,request->valuestring,jsOb);
		mem_free(tempData);
		return 1;
	}else if (strcmp(WIFI_DEV_BASIC_NAME_OID, request->valuestring) == 0){
		printf("test for get gw name\r\n");
		jsOb = json_create_object();
		json_add_item_to_object(jsOb,"name",json_create_string(hmIotsDevInfo.gwName));
		json_add_item_to_object(reply,request->valuestring,jsOb);
		*rc = APP_CMD_RC_SUCCESS;
		*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		return 1;
	}else if (strcmp(WIFI_DEV_GW_BASIC_INFO_OID, request->valuestring) == 0){
		printf("test for gw basic info\r\n");
		devOption.modeByte = ALL_DEV_STATE_ACTION;
		json_add_item_to_object(reply,request->valuestring,userCreatGWStaObject(&hmIotsDevInfo,devOption));
		*rc = APP_CMD_RC_SUCCESS;
		*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		return 1;
	}else if (strcmp(GW_BASIC_STATE_LIGHT_SET_OID, request->valuestring) == 0){
		printf("test for get gw light night list\r\n");
		userReadDevLinkGWLightNightFunc(SN,rcvType,appDevId);
		return 0;
	}else if (strcmp(GW_GET_NODE_LIST_OID, request->valuestring) == 0){
		printf("test for get dev list\r\n");
		userSendAllNodeBasicListJs(SN,rcvType,appDevId);
		return 0;
	}else if (strcmp(GW_APP_GET_DEV_STAT_LIST_OID, request->valuestring) == 0){
		printf("test for get dev state\r\n");
		userSendAllNodeStatListJs(SN,rcvType,appDevId);
		return 0;
	}else if (memcmp(GW_APP_LINKAGE_ACTION_OID, request->valuestring,strlen(GW_APP_LINKAGE_ACTION_OID)) == 0){
		intOid = get_last_id_from_OID(request->valuestring);
		if(intOid == 0)
		{
			printf("get linkage oid\r\n");
			userReadLinkageListFunc(SN,rcvType,appDevId);
			return 0;
		}else{
			*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
			*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		}
		return 1;
	}else if (memcmp(GW_FUNC_ACTION_ARM_MODE_OID, request->valuestring,strlen(GW_FUNC_ACTION_ARM_MODE_OID)) == 0){//场景控制		intOid = get_last_id_from_OID(request->valuestring);
		printf("get arm state oid\r\n");
		intOid = get_last_id_from_OID(request->valuestring);
		if(intOid == 2)
		{
			printf("get home arm dev index list\r\n");
			json_add_item_to_object(reply,request->valuestring,userReadHomeArmForDevList());
			*rc = APP_CMD_RC_SUCCESS;
		}else if(intOid == 5){
			printf("get arm type timer\r\n");
			json_add_item_to_object(reply,request->valuestring,userReadArmStateAndTimerFunc());
			*rc = APP_CMD_RC_SUCCESS;
		}else{
			*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
		}
		*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		return 1;
	}else if (memcmp(GW_APP_SCENE_ACTION_OID, request->valuestring,strlen(GW_APP_SCENE_ACTION_OID)) == 0){//场景控制		intOid = get_last_id_from_OID(request->valuestring);
		intOid = get_last_id_from_OID(request->valuestring);
		if(intOid == 0)
		{
			printf("get scene oid\r\n");
			userReadSceneListFunc(SN,rcvType,appDevId);
			return 0;
		}else{
			*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
			*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		}
		return 1;
	}else if (memcmp("2.1.1.0.", request->valuestring,strlen("2.1.1.0.")) == 0){//dev list dev state get
		printf("get oid for zb dev info\r\n");
		dotNum = get_dot_num_from_OID(request->valuestring);
		intOid = get_id_from_OID(request->valuestring,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if(pni_temp == NULL)//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
			return 1;
		}
		if(dotNum == 6)//get node sta list
		{
			printf("get zb dev ss\r\n");
			jsOb = userCreateNodeJsPl(pni_temp,CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID);
			userCreateJsplToBuf(jsOb,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,SN,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
			if(jsOb != NULL)
			{
				json_delete(jsOb);
				jsOb = NULL;
			}
			return 0;
		}else if(dotNum == 7){
			printf("get zb dev se\r\n");
			lastOid = get_last_id_from_OID(request->valuestring);
			if(lastOid == 32)// get dev setting info
			{
				printf("test  11111\r\n");
				devOption.modeByte = ALL_DEV_STATE_ACTION;
				jsOb = userCreatNodeStateJsFunc(pni_temp,CREATE_DEV_STATE_FOR_GET_NODE_SETTING_OID,devOption);
				userCreateJsplToBuf(jsOb,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,SN,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);
				if(jsOb != NULL)
				{
					json_delete(jsOb);
					jsOb = NULL;
				}
				return 0;
			}else if(lastOid < 32){//get dev signal stat
				devOption.modeByte |= (1 << lastOid);
				printf("read the signal stat:%d\r\n",devOption.modeByte);
				jsOb = userCreatNodeStateJsFunc(pni_temp,CREATE_DEV_STATE_FOR_GET_NODE_STATE_OID,devOption);
				userCreateJsplToBuf(jsOb,APP_CMD_RC_SUCCESS,MESSAGE_RSP_GETINFO,NULL,NULL,SN,SEND_TO_APP_DATA_NEED_ENCODE,rcvType,appDevId);	
				if(jsOb != NULL)
				{
					json_delete(jsOb);
					jsOb = NULL;
				}
				return 0;
			}else{
				if (memcmp(GW_APP_COM_WD_OID, request->valuestring,strlen(GW_APP_COM_WD_OID)) == 0)
				{
					if(lastOid == 41)
					{
						if(userReadDevLinkWdFunc(intOid,SN,rcvType,appDevId) == 0)
						{
							return 0;
						}else{
							*rc = APP_CMD_RC_ERROR_UNKNOW;
							*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
							return 1;
						}
					}else{
						*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
						*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
						return 1;
					}
				}
			}
		}
	}else{
		printf("unknow oid:%s\r\n",request->valuestring);
		*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
		*encodeFlag = SEND_TO_APP_DATA_NEED_ENCODE;
		return 1;
	}
}


void userDoNetAppGetNodeState(json_t *json,uint8_t rcvType,uint32_t appDevId)
{
	json_t *json_pl,*json_cid,*json_teid,*json_sid,*json_sn,*json_oid;
	json_t *json_rsp_pl,*jstempOid;
	int oidCount,i;
	int8_t rc;
	uint8_t encodeFlag = 1;
	char *sendData,*payload;
	json_pl = json_get_object_item(json,"PL");
	json_cid = json_get_object_item(json,"CID");
	json_sn = json_get_object_item(json,"SN");
	json_oid = json_get_object_item(json_pl,"OID");
	if((json_pl == NULL) ||(json_cid == NULL) ||(json_sn == NULL) ||(json_oid == NULL))
	{
		printf("get:the rec app json data was not valid\r\n");
		return;
	}
	oidCount = json_get_array_size(json_oid);

	sendData = mem_alloc(1024);
	if(sendData == NULL)
	{
		printf("in get oid:malloc mem was error\r\n");
		return;
	}
	printf("oid num:%d\r\n",oidCount);
	for(i = 0; i < oidCount; i ++)
	{
		json_rsp_pl = json_create_object();
		if(json_rsp_pl == NULL)
		{
			printf("mem the json was error\r\n");
			if(sendData != NULL)
			{
				mem_free(sendData);
				sendData = NULL;
			}
			return;
		}

		jstempOid = json_get_array_item(json_oid,i);
		if(hmiotsDoRecvGetMibCallback(jstempOid,json_rsp_pl,json_sn->valueint,&encodeFlag,&rc,rcvType,appDevId))
		{
			/*payload = json_PrintUnformatted(json_rsp_pl);
			if(payload == NULL)
			{
				printf("in get oid:json to string was error\r\n");
				if(sendData != NULL)
				{
					mem_free(sendData);
					sendData = NULL;
				}
				if(json_rsp_pl != NULL)
				{
					json_delete(json_rsp_pl);
					json_rsp_pl = NULL;
				}
				return;
			}
			sprintf(sendData, "{\"PL\":%s,\"TEID\":\"%s\",\"RC\":1,\"SN\":%d,\"ENCRYPT\":%d,\"CID\":%d,\"SID\":\"%s\"}\r\n", 
				payload, json_teid->valuestring, json_sn->valueint, encodeFlag,MESSAGE_RSP_GETINFO,json_sid->valuestring);//teid 建议填写设备的device id(云端给设备分配的ID)
			userPkSendBuffer(sendData,strlen(sendData),json_sid->valuestring,MESSAGE_RSP_GETINFO,encodeFlag);
			mem_free(payload);*/
			userCreateJsplToBuf(json_rsp_pl,rc,MESSAGE_RSP_GETINFO,NULL,NULL,json_sn->valueint,encodeFlag,rcvType,appDevId);
			if(json_rsp_pl != NULL)
			{
				json_delete(json_rsp_pl);
				json_rsp_pl = NULL;
			}
		}else{
			if(json_rsp_pl != NULL)
			{
				json_delete(json_rsp_pl);
				json_rsp_pl = NULL;
			}
		}
	}

	if(sendData != NULL)
	{
		mem_free(sendData);
		sendData = NULL;
	}
}

/*************************************************************************
网络数据接收后，最终处理函数，该函数只处理对网关
或zigbee的setting的指令
**************************************************************************/
int hmiotsDoRecvSetMibCallback(json_t *request, char *TEID, int SN,char *app_id,int8_t *rc)
{
	int16_t zones,vFlag;
	uint8_t tempString[48];
	uint8_t tempLen;
	json_t *objs,*js_t,*js_value;
	uint32_t tempData;
	float tempF;
	uint16_t intOid,lastOid,oidCount;
	uint16_t epIndex1= 0xff, epIndex2 = 0xff, epIndex3 = 0xff,epIndex4 = 0xff;
	uint8_t i;
	uint8_t colorChange = 0;
	uint8_t wdAction = 0xff;
	int sceneId;
	int linkageId;
	uint8_t enable;
	uint8_t needSaveFlashFlag = 0;
	HM_DEV_RAM_LIST *pni_temp;
	DEVINFOOPTION devOption;
	devOption.modeByte = 0;
	int ret = 0;
	TIMEIN tempTimer;

	printf("do app set oid\r\n");
	if (memcmp(WIFI_DEV_BASIC_ZONES_OID, request->string, strlen(WIFI_DEV_BASIC_ZONES_OID)) == 0){//set zone
		if(request->child->valuestring == NULL)
		{
			*rc = 0;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		
		printf("in set zone:rcv sting:%s",request->child->valuestring);
		memcpy(tempString,request->child->valuestring,strlen(request->child->valuestring));
		if(tempString[0] == '+')
		{
			vFlag = 1;			
		}else if(tempString[0] == '-'){
			vFlag = -1;
		}
		tempF = Atof(&tempString[1]);
		zones = (int)(vFlag * tempF * 100);
		if(hmIotsDevInfo.time_zones != zones)
		{
			hmIotsDevInfo.time_zones = zones;
			needSaveFlashFlag = 1;
		}
		printf("the zone:%d\r\n",zones);
		if(needSaveFlashFlag == 1)
		{
			userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);	
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_BASIC_STATE_LIGHT_SET_OID, request->string, strlen(GW_BASIC_STATE_LIGHT_SET_OID)) == 0){//GW LIGHT SET 
		printf("set gw light for night func\r\n");
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_UNKNOW;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}

		while(objs != NULL)
		{
			if(strcmp(objs->string,"EB") == 0)
			{
				if(objs->valueint)
				{
					if(hmIotsDevInfo.gwLightForNight.enable != 1)
					{
						hmIotsDevInfo.gwLightForNight.enable = 1;
						needSaveFlashFlag = 1;
					}
				}else{
					if(hmIotsDevInfo.gwLightForNight.enable != 0)
					{
						hmIotsDevInfo.gwLightForNight.enable = 0;
						needSaveFlashFlag = 1;
					}
				}
			}else if(strcmp(objs->string,"DS") == 0){
				oidCount = json_get_array_size(objs);
				hmIotsDevInfo.gwLightForNight.indexNum = 0;
				for(i = 0; i < oidCount; i ++)
				{
					js_value = json_get_array_item(objs,i);
					pni_temp = findInDevListByIndex(js_value->valueint);
					if(pni_temp != NULL)
					{
						if((pni_temp->devType == NODE_INFO_TYPE_MAGNET_DOOR) || (pni_temp->devType == NODE_INFO_TYPE_PIR_SENSOR) || (pni_temp->devType == NODE_INFO_TYPE_VIBRATION))
						{
							hmIotsDevInfo.gwLightForNight.index[hmIotsDevInfo.gwLightForNight.indexNum] = js_value->valueint;
							hmIotsDevInfo.gwLightForNight.indexNum ++;
						}
					}
				}
				needSaveFlashFlag = 1;
			}
			objs = objs->next;
		}
		if(needSaveFlashFlag == 1)
		{
			userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);	
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(WIFI_DEV_BASIC_NAME_OID, request->string, strlen(WIFI_DEV_BASIC_NAME_OID)) == 0){//set gw name
		if(request->child->valuestring == NULL)
		{
			*rc = APP_CMD_RC_ERROR_UNKNOW;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		*rc = APP_CMD_RC_SUCCESS;
		tempLen = strlen(request->child->valuestring);
		if(tempLen >= NAME_MAX_LEN)
		{
			tempLen = NAME_MAX_LEN - 1;
		}
		memset(hmIotsDevInfo.gwName,'\0',NAME_MAX_LEN);
		memcpy(hmIotsDevInfo.gwName,request->child->valuestring,tempLen);
		hmIotsDevInfo.gwName[tempLen] = '\0';
		userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);
		gwStateForRealTime.echoNeedUpdateFlag = 1;
		return 1;
	}else if (memcmp(WIFI_DEV_GW_BASIC_INFO_OID, request->string, strlen(WIFI_DEV_GW_BASIC_INFO_OID)) == 0){//set gw basic value
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_UNKNOW;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		while(objs != NULL)
		{
			if(strcmp(objs->string,"AT") == 0)
			{
				if(objs->valueint == SENSOR_DISARM){
					hmSetAllSensorDisarm();
					if(beep_info.work_flag == 0 )
					 {
						 beep_info.work_type = BEEP_DISARM;
						 beep_info.work_flag = 1;
						 tx_semaphore_put(&beep_semaphore_ptr);
					 }
					 if(hmIotsDevInfo.sensor_arm_type != SENSOR_DISARM)
					 {
						 hmIotsDevInfo.sensor_arm_type = SENSOR_DISARM;
						 needSaveFlashFlag = 1;
					 }
				}else if(objs->valueint == SENSOR_HOME_ARM){
					hmSetAllSensorHomeArm();
					if((beep_info.work_flag == 0) && ((beep_info.work_type != BEEP_ALARM) && (beep_info.work_type != BEE_SOUND_LOCAL_REGULA)) )
					 {
						 beep_info.work_type = BEEP_HOME_ARM;
						 beep_info.work_flag = 1;
						 tx_semaphore_put(&beep_semaphore_ptr);
					 }
					if(hmIotsDevInfo.sensor_arm_type != SENSOR_HOME_ARM)
					 {
						 hmIotsDevInfo.sensor_arm_type = SENSOR_HOME_ARM;
						 needSaveFlashFlag = 1;
					 }
				}else if(objs->valueint == SENSOR_ARM){
					hmSetAllSensorArm();
					if((beep_info.work_flag == 0) && ((beep_info.work_type != BEEP_ALARM) && (beep_info.work_type != BEE_SOUND_LOCAL_REGULA)))
					 {
						 beep_info.work_type = BEEP_ARM;
						 beep_info.work_flag = 1;
						 tx_semaphore_put(&beep_semaphore_ptr);
					 }
					if(hmIotsDevInfo.sensor_arm_type != SENSOR_ARM)
					 {
						 hmIotsDevInfo.sensor_arm_type = SENSOR_ARM;
						 needSaveFlashFlag = 1;
					 }
				}else{
					printf("the arm mode is not valid\r\n");
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
				hmIotsDevInfo.sensor_arm_type = objs->valueint;
				devOption.GWStat.armtype = 1;
			}else if(strcmp(objs->string,"AL") == 0){
				if(hmIotsDevInfo.sound_level != objs->valueint)
				{
					hmIotsDevInfo.sound_level = objs->valueint;
					needSaveFlashFlag = 1;
				}
				if(beep_info.work_flag == 0) 
				 {
				 	 beep_info.value = objs->valueint;
					 beep_info.work_type = BEEP_SOUND_REGULA;
					 beep_info.work_flag = 1;
					 tx_semaphore_put(&beep_semaphore_ptr);
				 }
				 
				 devOption.GWStat.alarmlevel = 1;
			}else if(strcmp(objs->string,"BT") == 0){
				if(hmIotsDevInfo.beep_alarm_times != objs->valueint)
				{
					hmIotsDevInfo.beep_alarm_times = objs->valueint;
					needSaveFlashFlag = 1;
				}
				if(beep_info.work_flag == 0) 
				 {
					 beep_info.work_type = BEEP_ALARM_TIMER_CHANGE;
					 beep_info.work_flag = 1;
					 tx_semaphore_put(&beep_semaphore_ptr);
				 }
				 
				 devOption.GWStat.betimer = 1;
			}else if(strcmp(objs->string,"LG") == 0){
				if(strcmp(objs->valuestring,"US") == 0)
				{
					if(hmIotsDevInfo.app_language != LANGUAGE_ENGLISH)
					{
						hmIotsDevInfo.app_language = LANGUAGE_ENGLISH;
						needSaveFlashFlag = 1;
					}
				}else if(strcmp(objs->valuestring,"CN") == 0){
					if(hmIotsDevInfo.app_language != LANGUAGE_CHINESE)
					{
						hmIotsDevInfo.app_language = LANGUAGE_CHINESE;
						needSaveFlashFlag = 1;
					}
				}else{
					*rc = APP_CMD_RC_ERROR_UNKNOW;
					return 1;
				}
				if((beep_info.work_flag == 0) && ((beep_info.work_type != BEEP_ALARM) && (beep_info.work_type != BEE_SOUND_LOCAL_REGULA)))
				 {
					 beep_info.work_type = BEEP_LANGUAGE_SET;
					 beep_info.work_flag = 1;
					 tx_semaphore_put(&beep_semaphore_ptr);
				 }
				 devOption.GWStat.gwlanguage = 1;
			}else if(strcmp(objs->string,"LL") == 0){
				if(hmIotsDevInfo.light_level != objs->valueint)
				{
					hmIotsDevInfo.light_level = objs->valueint;
					needSaveFlashFlag = 1;
				}
				userControlGWLedFunc(1,3);
				devOption.GWStat.gwlightlevel = 1;
			}else if(strcmp(objs->string,"LO") == 0){
				hmIotsDevInfo.light_onoff = objs->valueint;
				userControlGWLedFunc(hmIotsDevInfo.light_onoff,0);
				devOption.GWStat.gwlightonoff = 1;
			}else if(strcmp(objs->string,"LT") == 0){
				if(hmIotsDevInfo.light_on_times != objs->valueint)
				{
					hmIotsDevInfo.light_on_times = objs->valueint;
					qcom_timer_delete(&gwNightLightTimer);
					qcom_timer_init(&gwNightLightTimer, lightNight_timerS_handle, NULL, hmIotsDevInfo.light_on_times * 1000, ONESHOT);
					needSaveFlashFlag = 1;
				}
				devOption.GWStat.lgtimer = 1;
			}else if(strcmp(objs->string,"RD") == 0){
				tempLen = strlen(objs->valuestring);
				if(tempLen >=  ROOM_ID_MAX_LEN)
				{
					tempLen = ROOM_ID_MAX_LEN - 1;
				}
				memset(hmIotsDevInfo.romId,'\0',ROOM_ID_MAX_LEN);
				memcpy(hmIotsDevInfo.romId,objs->valuestring,tempLen);
				needSaveFlashFlag = 1;
				devOption.GWStat.roomID = 1;
			}
			objs = objs->next;
		}
		if(needSaveFlashFlag == 1)
		{
			userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);	
		}
		*rc = APP_CMD_RC_SUCCESS;
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONTROL_DEV_TYPE_GW,0,devOption,app_id,TEID);
		}
		return 1;
	}else if (memcmp(GW_FUNC_ACTION_SET_ZC_JOIN_OID, request->string, strlen(GW_FUNC_ACTION_SET_ZC_JOIN_OID)) == 0){//set zc to joining
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_UNKNOW;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		tempData = objs->valueint;
		if(tempData == 1)
		{
			if((gwStateForRealTime.gwZbIsJoining == 0) && (gwStateForRealTime.gwIsAlarning == 0))//没有处于加网状态，且未处于报警状态
			{
				userFillUartCmdForPermitJoin(DEFUALT_ZIGBEE_JOINING_TIMERS);
				//gwStateForRealTime.gwZbIsJoining = 1;
			}
		}else{
			if((gwStateForRealTime.gwZbIsJoining != 0) && (gwStateForRealTime.gwIsAlarning == 0))
			{
				userFillUartCmdForPermitJoin(0);
				//gwStateForRealTime.gwZbIsJoining = 0;
			}
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_OTA_OID, request->string, strlen(GW_APP_OTA_OID)) == 0){//allow dev ota
		lastOid = get_last_id_from_OID(request->string);
		if(OTA_MODE_WIFI == lastOid)
		{
			if(request->child->valueint == 2)//允许升级
			{
				gwStateForRealTime.wifiSoftUpdate.appAllowSoftWareUpdate = 1;		
			}else{
				gwStateForRealTime.wifiSoftUpdate.appAllowSoftWareUpdate = 0;
			}
		}else if(OTA_MODE_ZC == lastOid){
			if(request->child->valueint == 2)//允许升级
			{
				gwStateForRealTime.zigbeeZcBasicInfo.appAllowSoftWareUpdate = 1;
			}else{
				gwStateForRealTime.zigbeeZcBasicInfo.appAllowSoftWareUpdate = 0;
			}
			*rc = APP_CMD_RC_SUCCESS;
			return 1;
		}
	}else if (memcmp(GW_FUNC_ACTION_ARM_MODE_OID, request->string,strlen(GW_FUNC_ACTION_ARM_MODE_OID)) == 0){//场景控制		intOid = get_last_id_from_OID(request->valuestring);
		printf("set arm mode oid\r\n");
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_last_id_from_OID(request->string);
		if(intOid == 2)//在家布防设置关联设备
		{
			printf("set home arm dev index list\r\n");
			oidCount = json_get_array_size(objs);
			hmIotsDevInfo.home_arm_for_dev.indexNum = 0;
			for(i = 0; i < oidCount; i ++)
			{
				js_value = json_get_array_item(objs,i);
				pni_temp = findInDevListByIndex(js_value->valueint);
				if(pni_temp != NULL)
				{
					if((pni_temp->devType == NODE_INFO_TYPE_MAGNET_DOOR) || (pni_temp->devType == NODE_INFO_TYPE_PIR_SENSOR) ||(pni_temp->devType == NODE_INFO_TYPE_VIBRATION))
					{
						hmIotsDevInfo.home_arm_for_dev.index[hmIotsDevInfo.home_arm_for_dev.indexNum] = js_value->valueint;
						hmIotsDevInfo.home_arm_for_dev.indexNum ++;
					}
				}
			}
			needSaveFlashFlag = 1;
			*rc = APP_CMD_RC_SUCCESS;
		}else if(intOid == 5){//设置撤布防定时器
			objs = objs->child;
			printf("set arm type timer\r\n");
			do{
				js_value = json_get_object_item(objs,"AS");
				if(js_value == NULL)
				{
					printf("the json not have arm state\r\n");
					ret = -1;
					break;
				}
				tempData = js_value->valueint;
				js_t = json_get_object_item(objs,"TS");
				if(js_t == NULL)
				{
					printf("the json not have timer start\r\n");
					ret = -1;
					break;
				}
				oidCount = json_get_array_size(js_t);
				if(oidCount == 4)
				{
					for(i = 0; i < oidCount; i ++)
					{
						js_value = json_get_array_item(js_t,i);
						if(js_value !=  NULL)
						{
							if(i == 0)
							{
								tempTimer.month = js_value->valueint;
							}else if(i == 1){
								tempTimer.day = js_value->valueint;
							}else if(i == 2){
								tempTimer.hour = js_value->valueint;
							}else if(i == 3){
								tempTimer.min = js_value->valueint;
							}
						}else{
							printf("the time start was error\r\n");
							ret = -1;
							break;
						}
					}
				}else{
					printf("the js count was error\r\n ");
					ret = -1;
					break;
				}
				js_value = json_get_object_item(objs,"WF");
				if(js_value == NULL)
				{
					printf("the timer not have wf\r\n");
					ret = -1;
					break;
				}
				tempTimer.wkflag = js_value->valueint;
				if(tempData == SENSOR_DISARM)
				{
					printf("test for disarm\r\n");
					hmIotsDevInfo.sensor_arm_timer.disarmTimer = tempTimer;
				}else if(tempData == SENSOR_ARM){
					printf("test for arm\r\n");
					hmIotsDevInfo.sensor_arm_timer.armTimer = tempTimer;
				}else if(tempData == SENSOR_HOME_ARM){
					printf("test for hom arm\r\n");
					hmIotsDevInfo.sensor_arm_timer.homeArmTimer = tempTimer;
				}else{
					printf("the json arm state value was not valid\r\n");
					ret = -1;
					break;
				}
			}while(0);
			if(ret == 0)
			{
				needSaveFlashFlag = 1;
				*rc = APP_CMD_RC_SUCCESS;
				userUpdateGwArmForTimerEvent((char*)&hmIotsDevInfo);//更新网关撤布防定时器
				printf("set arm timer was success\r\n");
			}else{
				*rc = APP_CMD_RC_ERROR_PARAMETER;
				printf("set arm timer was faild\r\n");
			}
		}else{
			*rc = APP_CMD_RC_ERROR_UNKNOW;
		}

		if(needSaveFlashFlag == 1)
		{
			userFlashWriteNumFunc((uint8_t *)&devNumInfo);
		}
		return 1;
		//dai wan cheng timer ,cloud report
	}else if (memcmp(GW_APP_LINKAGE_ACTION_OID, request->string, strlen(GW_APP_LINKAGE_ACTION_OID)) == 0){//set linkage
		printf("in linkage oid\r\n");
		intOid = get_id_from_OID(request->string,5);
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid == 0)//添加联动
		{
			linkageId = userAddLinkageFunc(request,INVALID_SCENE_LINKAGE_INDEX);
			if(linkageId >= 0)
			{
				objs = json_create_object();
				if(objs != NULL)
				{
					json_add_item_to_object(request,"LD",json_create_number(linkageId));
					json_add_item_to_object(objs,"2.1.1.3.0",request);
				}else{
					printf("create json obs was error\r\n");
					*rc = APP_CMD_RC_ERROR_UNKNOW;
					return 1;
				}
				*rc = APP_CMD_RC_SUCCESS;
				userCreateJsplToBuf(objs,*rc,MESSAGE_RSP_STATUS_CHANGE,app_id,TEID,SN,1,DATA_SEND_RCV_TYPE_ALL,0xffffffff);
				if(objs != NULL)
				{
					json_delete(objs);
					objs = NULL;
				}
				return 0;
			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
				return 1;
			}
		}else if (lastOid == 1){//联动使能
			
			enable = request->child->valueint;
			printf("enable linkage ,the linkage id:%d,value:%d\r\n",intOid,enable);
			if(userSetEnableLinkageFunc(intOid,enable) == 0)
			{
				*rc = APP_CMD_RC_SUCCESS;
			}else{
				printf("set enable the linkage was faild\r\n");
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}else if(lastOid == 254){//删除联动
			printf("delete the linkage id:%d\r\n",intOid);
			if(userDeleteLinkageFromId(intOid) == 0)
			{
				*rc = APP_CMD_RC_SUCCESS;
			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}else if(intOid == lastOid){//修改联动
			printf("update linkage,linkage id:%d\r\n",intOid);
			linkageId = userAddLinkageFunc(request,intOid);
			if(linkageId >= 0)
			{
				*rc = APP_CMD_RC_SUCCESS;

			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}
		return 1;
	}else if (memcmp(GW_APP_SCENE_ACTION_OID, request->string, strlen(GW_APP_SCENE_ACTION_OID)) == 0){//set scene
		printf("in scene oid\r\n");
		intOid = get_id_from_OID(request->string,5);
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid == 0)
		{
			sceneId = userAddSceneFunc(request,INVALID_SCENE_LINKAGE_INDEX);
			if(sceneId >= 0)
			{
				objs = json_create_object();
				if(objs != NULL)
				{
					json_add_item_to_object(request,"SD",json_create_number(sceneId));
					json_add_item_to_object(objs,"2.1.1.2.0",request);
				}else{
					printf("create json obs was error\r\n");
					*rc = APP_CMD_RC_ERROR_UNKNOW;
					return 1;
				}
				*rc = APP_CMD_RC_SUCCESS;
				gwStateForRealTime.echoNeedUpdateFlag = 1;
				userCreateJsplToBuf(objs,*rc,MESSAGE_RSP_STATUS_CHANGE,app_id,TEID,SN,1,DATA_SEND_RCV_TYPE_ALL,0xffffffff);
				if(objs != NULL)
				{
					json_delete(objs);
					objs = NULL;
				}
				return 0;
			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
				return 1;
			}
		}else if (lastOid == 1){//执行场景
			printf("exe scene ,the scene id:%d\r\n",intOid);
			if(userCheckSceneIdWasList(intOid) == 0)
			{
				if(userFillSceneExeBuf(intOid,0) == 0)
				{
					*rc = APP_CMD_RC_SUCCESS;
					writeUserControlSceneStateToCloud(intOid,APP_CMD_RC_SUCCESS,app_id,TEID);
				}else{
					*rc = APP_CMD_RC_ERROR_UNKNOW;
				}
			}else{
				printf("the scene id was not valid\r\n");
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}else if(lastOid == 254){//删除场景
			printf("delete the scene id:%d\r\n",intOid);
			if(userDeleteSceneFromId(intOid) == 0)
			{
				*rc = APP_CMD_RC_SUCCESS;
				gwStateForRealTime.echoNeedUpdateFlag = 1;
			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}else if(intOid == lastOid){//修改场景
			printf("update scene,scene id:%d\r\n",intOid);
			sceneId = userAddSceneFunc(request,intOid);
			if(sceneId >= 0)
			{
				*rc = APP_CMD_RC_SUCCESS;
				gwStateForRealTime.echoNeedUpdateFlag = 1;//可能会有修改场景名称的操作，所以在此一并需要上传云端
			}else{
				*rc = APP_CMD_RC_ERROR_UNKNOW;
			}
		}
		return 1;
	}else if ((memcmp(GW_APP_COM_DOOR_OID, request->string, strlen(GW_APP_COM_DOOR_OID)) == 0) ||
		      (memcmp(GW_APP_COM_PIR_OID, request->string, strlen(GW_APP_COM_PIR_OID)) == 0)){//set door arm mode
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_PIR_SENSOR) || (pni_temp->devType != NODE_INFO_TYPE_MAGNET_DOOR))//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		while(objs != NULL)
		{
			if(strcmp(objs->string,"AS") == 0)
			{
				if(pni_temp->epList[0].devData.zoneStateApp.armMode != objs->valueint)
				{
					pni_temp->epList[0].devData.zoneStateApp.armMode = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.IASSet.armMode = 1;
			}
			objs = objs->next;
		}
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
		}
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_COM_WD_OID, request->string, strlen(GW_APP_COM_WD_OID)) == 0){//control wd
		printf("oid set for wd\r\n");
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
			
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (NODE_INFO_TYPE_WARNING_DEV != pni_temp->devType))//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid == 32)
		{
			while(objs != NULL)
			{
				if(strcmp(objs->string,"OF") == 0)
				{
					if(objs->valueint)
					{
						wdAction = 1;
					}else{
						wdAction = 0;
					}
					devOption.WDSet.onoff = 1;
				}else if(strcmp(objs->string,"DT") == 0){
					printf("wd set duration value:%d\r\n",objs->valueint);
					if(pni_temp->epList[0].devData.wdStateApp.duration != objs->valueint)
					{
						pni_temp->epList[0].devData.wdStateApp.duration = objs->valueint;
						needSaveFlashFlag = 1;
					}
					devOption.WDSet.duration = 1;
				}
				objs = objs->next;
			}
			if(wdAction != 0xff)
			{
				if(pni_temp->onLine == 1)
				{
					userAppControlWDFunc(pni_temp->shortAddr,pni_temp->epList[0].ep,wdAction,pni_temp->epList[0].devData.wdStateApp.duration);
				}else{
					printf("the wd was off lin\r\n");
					*rc= APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}
			if(needSaveFlashFlag == 1)
			{
				userUpdateDevInfoDataToFlash(pni_temp);
			}
			
			if(devOption.modeByte != 0)
			{
				writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
			}
			*rc = APP_CMD_RC_SUCCESS;
			return 1;
		}else if(lastOid == 41){
			if(userCreatDevLinkWdFunc(request,pni_temp) == 0)
			{
				*rc = APP_CMD_RC_SUCCESS;
				return 1;
			}else{
				*rc= APP_CMD_RC_ERROR_UNKNOW;
				return 1;
			}
		}else{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
	}else if (memcmp(GW_APP_COM_THP_OID, request->string, strlen(GW_APP_COM_THP_OID)) == 0){//set thp alarn value
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_TEMP_HUM_SENSOR))//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		for(i = 0; i < pni_temp->epNum; i ++)
		{
			if(pni_temp->epList[i].ep == 1)
			{
				epIndex1 = i;
			}else if(pni_temp->epList[i].ep == 2){
				epIndex2 = i;
			}
		}
		if((epIndex1 == 0xff) || (epIndex2 == 0xff))
		{
			printf("the dev info was error\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		while(objs != NULL)
		{
			if(strcmp(objs->string,"HU") == 0)
			{
				memcpy(tempString,objs->valuestring,strlen(objs->valuestring));
				tempF = Atof(&tempString[0]);
				if(pni_temp->epList[epIndex2].devData.humiStateApp.upHumi != (uint16_t)(tempF * 100))
				{
					pni_temp->epList[epIndex2].devData.humiStateApp.upHumi = (uint16_t)(tempF * 100);
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.humiUp = 1;
			}else if(strcmp(objs->string,"HL") == 0){
				memcpy(tempString,objs->valuestring,strlen(objs->valuestring));
				tempF = Atof(&tempString[0]);
				if(pni_temp->epList[epIndex2].devData.humiStateApp.lowHumi != (uint16_t)(tempF * 100))
				{
					pni_temp->epList[epIndex2].devData.humiStateApp.lowHumi = (uint16_t)(tempF * 100);
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.humiLow = 1;
			}else if(strcmp(objs->string,"HA") == 0){
				if(pni_temp->epList[epIndex2].devData.humiStateApp.checkEnable != objs->valueint)
				{
					pni_temp->epList[epIndex2].devData.humiStateApp.checkEnable = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.h_checkEn = 1;
			}else if(strcmp(objs->string,"TU") == 0){
				printf("in set temp up:rcv sting:%s\r\n",objs->valuestring);
				memcpy(tempString,objs->valuestring,strlen(objs->valuestring));
				if(tempString[0] == '-')
				{
					vFlag = -1;	
					tempF = Atof(&tempString[1]);
				}else {
					vFlag = 1;
					tempF = Atof(&tempString[0]);;
				}

				if(pni_temp->epList[epIndex1].devData.tempStateApp.upTemp != (int)(vFlag * tempF * 100))
				{
					pni_temp->epList[epIndex1].devData.tempStateApp.upTemp = (int)(vFlag * tempF * 100);
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.tempUp = 1;
			}else if(strcmp(objs->string,"TL") == 0){
				printf("in set temp low:rcv sting:%s\r\n",objs->valuestring);
				memcpy(tempString,objs->valuestring,strlen(objs->valuestring));
				if(tempString[0] == '-')
				{
					vFlag = -1;	
					tempF = Atof(&tempString[1]);
				}else {
					vFlag = 1;
					tempF = Atof(&tempString[0]);;
				}
				if(pni_temp->epList[epIndex1].devData.tempStateApp.lowTemp != (int)(vFlag * tempF * 100))
				{
					pni_temp->epList[epIndex1].devData.tempStateApp.lowTemp = (int)(vFlag * tempF * 100);
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.tempLow = 1;
			}else if(strcmp(objs->string,"TA") == 0){
				if(pni_temp->epList[epIndex1].devData.tempStateApp.checkEnable != objs->valueint)
				{
					pni_temp->epList[epIndex1].devData.tempStateApp.checkEnable = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.THPSet.t_checkEn = 1;
			}
			objs = objs->next;
		}
		
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_COM_VIBRATION_OID, request->string, strlen(GW_APP_COM_VIBRATION_OID)) == 0){//set vibration sensitivity level
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_VIBRATION) )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		while(objs != NULL)
		{
			if(strcmp(objs->string,"ST") == 0)
			{
				if(userSetNodeSensitivityLevel(pni_temp->shortAddr,pni_temp->epList[0].ep,objs->valueint,SN) == 0)
				{
					devOption.IASSet.sensitivityLevel = 1;
				}else{
					printf("set dev sensitivity level fail\r\n");
				}
			}
			objs = objs->next;
		}
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
			return 0;
		}
		*rc = APP_CMD_RC_ERROR_UNKNOW;
		return 1;
	}else if (memcmp(GW_APP_COM_RGB_OID, request->string, strlen(GW_APP_COM_RGB_OID)) == 0){//control RGB
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_LIGHT_RGB) )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		
		while(objs != NULL)
		{
			if(strcmp(objs->string,"OF") == 0)
			{
				pni_temp->epList[0].devData.colorStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[0].ep,objs->valueint);
				devOption.RGBSet.onoff = 1;
			}else if(strcmp(objs->string,"TS") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[0].devData.colorStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[0].devData.colorStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[0].devData.colorStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[0].devData.colorStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.RGBSet.ts = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[0].devData.colorStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[0].devData.colorStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[0].devData.colorStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[0].devData.colorStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.RGBSet.te = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF") == 0){
				if(pni_temp->epList[0].devData.colorStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[0].devData.colorStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.RGBSet.wflag = 1;
			}else if(strcmp(objs->string,"CR") == 0){
				colorChange = 1;
				pni_temp->epList[0].devData.colorStateApp.rgb_r = objs->valueint;
				devOption.RGBSet.colour_r = 1;
			}else if(strcmp(objs->string,"CG") == 0){
				colorChange = 1;
				pni_temp->epList[0].devData.colorStateApp.rgb_g = objs->valueint;
				devOption.RGBSet.colour_g = 1;
			}else if(strcmp(objs->string,"CB") == 0){
				colorChange = 1;
				pni_temp->epList[0].devData.colorStateApp.rgb_b = objs->valueint;
				devOption.RGBSet.colour_b = 1;
			}else if(strcmp(objs->string,"LE") == 0){
				pni_temp->epList[0].devData.colorStateApp.level= objs->valueint;
				userFillUartCmdForLevel(pni_temp->shortAddr,pni_temp->epList[0].ep,MOVE_TO_LEVEL_WTONOFF,
									pni_temp->epList[0].devData.colorStateApp.level,0);
				devOption.RGBSet.level = 1;
			}
			objs = objs->next;
		}
		if(colorChange == 1)
		{
			printf("control color r:%d,g:%d,b:%d\r\n",pni_temp->epList[0].devData.colorStateApp.rgb_r,pni_temp->epList[0].devData.colorStateApp.rgb_g,pni_temp->epList[0].devData.colorStateApp.rgb_b);
			if((pni_temp->epList[0].devData.colorStateApp.onOffStatus == 1) && (pni_temp->onLine == 1))
			{
				userFillUartCmdForColorControl(pni_temp->shortAddr,pni_temp->epList[0].ep,pni_temp->epList[0].devData.colorStateApp.rgb_r,
											pni_temp->epList[0].devData.colorStateApp.rgb_g,pni_temp->epList[0].devData.colorStateApp.rgb_b);
			}else{
				printf("the rgb was not control color status\r\n");
				*rc = APP_CMD_RC_ERROR_PARAMETER;
				return 1;
			}
		}
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
			userUpdateTheDevTimerEvent((char*)pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if ((memcmp(GW_APP_COM_AISW2_OID, request->string, strlen(GW_APP_COM_AISW2_OID)) == 0) ||
	 	      (memcmp(GW_APP_COM_AISW1_OID, request->string, strlen(GW_APP_COM_AISW1_OID)) == 0) ||
	 	      (memcmp(GW_APP_COM_AISW3_OID, request->string, strlen(GW_APP_COM_AISW3_OID)) == 0)){//control sw2
		printf("the sw control\r\n");
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || ((pni_temp->devType != NODE_INFO_TYPE_AI2SW) && (pni_temp->devType != NODE_INFO_TYPE_AI1SW) && (pni_temp->devType != NODE_INFO_TYPE_AI3SW)))//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		for(i = 0; i < pni_temp->epNum; i ++)
		{
			if(pni_temp->epList[i].ep == 1)
			{
				epIndex1 = i;
			}else if(pni_temp->epList[i].ep == 2){
				epIndex2 = i;
			}else if(pni_temp->epList[i].ep == 3){
				epIndex3 = i;
			}
		}
		if(pni_temp->devType == NODE_INFO_TYPE_AI2SW)
		{
			if((epIndex1 == 0xff) || (epIndex2 == 0xff))
			{
				printf("the dev info was error\r\n");
				*rc = APP_CMD_RC_ERROR_PARAMETER;
				return 1;
			}
		}else if(pni_temp->devType == NODE_INFO_TYPE_AI3SW){
			if((epIndex1 == 0xff) || (epIndex2 == 0xff) || (epIndex3 == 0xff))
			{
				printf("the dev info was error\r\n");
				*rc = APP_CMD_RC_ERROR_PARAMETER;
				return 1;
			}
		}else if(pni_temp->devType == NODE_INFO_TYPE_AI1SW){
			if(epIndex1 == 0xff)
			{
				printf("the dev info was error\r\n");
				*rc = APP_CMD_RC_ERROR_PARAMETER;
				return 1;
			}
		}
		while(objs != NULL)
		{
			if(strcmp(objs->string,"OF1") == 0)
			{
				pni_temp->epList[epIndex1].devData.witchStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex1].ep,objs->valueint);
				devOption.SWSet.onoff1 = 1;
			}else if(strcmp(objs->string,"TS1") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.ts1 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE1") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.witchStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.te1 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF1") == 0){
				if(pni_temp->epList[epIndex1].devData.witchStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex1].devData.witchStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.SWSet.wflag1 = 1;
			}else if(strcmp(objs->string,"OF2") == 0){
				pni_temp->epList[epIndex2].devData.witchStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex2].ep,objs->valueint);
				devOption.SWSet.onoff2 = 1;
			}else if(strcmp(objs->string,"TS2") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.ts2 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE2") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.witchStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.te2 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF2") == 0){
				if(pni_temp->epList[epIndex2].devData.witchStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex2].devData.witchStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.SWSet.wflag2 = 1;
			}else if(strcmp(objs->string,"OF3") == 0){
				pni_temp->epList[epIndex3].devData.witchStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex3].ep,objs->valueint);
				devOption.SWSet.onoff3 = 1;
			}else if(strcmp(objs->string,"TS3") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.ts3 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE3") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex3].devData.witchStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SWSet.te3 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF3") == 0){
				if(pni_temp->epList[epIndex3].devData.witchStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex3].devData.witchStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.SWSet.wflag3 = 1;
			}
			objs = objs->next;
			
		}
		
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
			userUpdateTheDevTimerEvent((char*)pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		
		return 1;
	}else if ((memcmp(GW_APP_COM_CUTRGB4_OID, request->string, strlen(GW_APP_COM_CUTRGB4_OID)) == 0) ){//control custom rgb4
		printf("the custom rgb4 control\r\n");
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_CUSTOM_RGB_AREA ))//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		for(i = 0; i < pni_temp->epNum; i ++)
		{
			if(pni_temp->epList[i].ep == 1)
			{
				epIndex1 = i;
			}else if(pni_temp->epList[i].ep == 2){
				epIndex2 = i;
			}else if(pni_temp->epList[i].ep == 3){
				epIndex3 = i;
			}else if(pni_temp->epList[i].ep == 4){
				epIndex4 = i;
			}
		}
		
		while(objs != NULL)
		{
			if(strcmp(objs->string,"OFL") == 0)
			{
				do{
					oidCount = json_get_array_size(objs);
					if(oidCount == 4)
					{
						for(i = 0; i < oidCount; i ++)
						{
							js_value = json_get_array_item(objs,i);
							if(js_value !=  NULL)
							{
								if(i == 0)
								{
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.onOffStatus = js_value->valueint;
										userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex1].ep,js_value->valueint);
										devOption.CUTRGBSet.onoffList1 = 1;
									}
								}else if(i == 1){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.onOffStatus = js_value->valueint;
										userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex2].ep,js_value->valueint);
										devOption.CUTRGBSet.onoffList2 = 1;
									}
								}else if(i == 2){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.onOffStatus = js_value->valueint;
										userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex3].ep,js_value->valueint);
										devOption.CUTRGBSet.onoffList3 = 1;
									}
								}else if(i == 3){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.onOffStatus = js_value->valueint;
										userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex4].ep,js_value->valueint);
										devOption.CUTRGBSet.onoffList4 = 1;
									}
								}
							}else{
								printf("the time start was error\r\n");
								ret = -1;
							}
						}
					}else{
						printf("the js count was error\r\n ");
						ret = -1;
					}
				}while(0);
				if(ret == -1)
				{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"LEL") == 0){
				do{
					oidCount = json_get_array_size(objs);
					if(oidCount == 4)
					{
						for(i = 0; i < oidCount; i ++)
						{
							js_value = json_get_array_item(objs,i);
							if(js_value !=  NULL)
							{
								if(i == 0)
								{
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.brightness = js_value->valueint;
										userFillUartCmdForBrightness(pni_temp->shortAddr,pni_temp->epList[epIndex1].ep,js_value->valueint);
										devOption.CUTRGBSet.brightnessList1 = 1;
									}
								}else if(i == 1){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.brightness = js_value->valueint;
										userFillUartCmdForBrightness(pni_temp->shortAddr,pni_temp->epList[epIndex2].ep,js_value->valueint);
										devOption.CUTRGBSet.brightnessList2 = 1;
									}
								}else if(i == 2){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.brightness = js_value->valueint;
										userFillUartCmdForBrightness(pni_temp->shortAddr,pni_temp->epList[epIndex3].ep,js_value->valueint);
										devOption.CUTRGBSet.brightnessList3 = 1;
									}
								}else if(i == 3){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.brightness = js_value->valueint;
										userFillUartCmdForBrightness(pni_temp->shortAddr,pni_temp->epList[epIndex4].ep,js_value->valueint);
										devOption.CUTRGBSet.brightnessList4 = 1;
									}
								}
							}else{
								printf("the time start was error\r\n");
								ret = -1;
							}
						}
					}else{
						printf("the js count was error\r\n ");
						ret = -1;
					}
				}while(0);
				if(ret == -1)
				{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"CWL") == 0){
				do{
					oidCount = json_get_array_size(objs);
					if(oidCount == 4)
					{
						for(i = 0; i < oidCount; i ++)
						{
							js_value = json_get_array_item(objs,i);
							if(js_value !=  NULL)
							{
								if(i == 0)
								{
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.colorTemrature = js_value->valueint;
										userFillUartCmdForColorTemperature(pni_temp->shortAddr,pni_temp->epList[epIndex1].ep,js_value->valueint);
										devOption.CUTRGBSet.colorTempList1 = 1;
									}
								}else if(i == 1){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.colorTemrature = js_value->valueint;
										userFillUartCmdForColorTemperature(pni_temp->shortAddr,pni_temp->epList[epIndex2].ep,js_value->valueint);
										devOption.CUTRGBSet.colorTempList2 = 1;
									}
								}else if(i == 2){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.colorTemrature = js_value->valueint;
										userFillUartCmdForColorTemperature(pni_temp->shortAddr,pni_temp->epList[epIndex3].ep,js_value->valueint);
										devOption.CUTRGBSet.colorTempList3 = 1;
									}
								}else if(i == 3){
									if(js_value->valueint != -1)
									{
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.colorTemrature = js_value->valueint;
										userFillUartCmdForColorTemperature(pni_temp->shortAddr,pni_temp->epList[epIndex4].ep,js_value->valueint);
										devOption.CUTRGBSet.colorTempList4 = 1;
									}
								}
							}else{
								printf("the time start was error\r\n");
								ret = -1;
							}
						}
					}else{
						printf("the js count was error\r\n ");
						ret = -1;
					}
				}while(0);
				if(ret == -1)
				{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TS1") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.ts1 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE1") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.te1 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF1") == 0){
				if(pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex1].devData.cstRgbStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.CUTRGBSet.wflag1 = 1;
			}else if(strcmp(objs->string,"TS2") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.ts2 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE2") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.te2 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF2") == 0){
				if(pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex2].devData.cstRgbStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.CUTRGBSet.wflag2 = 1;
			}else if(strcmp(objs->string,"TS3") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.ts3 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE3") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.te3 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF3") == 0){
				if(pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex3].devData.cstRgbStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.CUTRGBSet.wflag3 = 1;
			}else if(strcmp(objs->string,"TS4") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.ts4 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE4") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.CUTRGBSet.te4 = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF4") == 0){
				if(pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex4].devData.cstRgbStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.CUTRGBSet.wflag3 = 1;
			}
			objs = objs->next;
			
		}
		
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
			userUpdateTheDevTimerEvent((char*)pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		
		return 1;
	}else if (memcmp(GW_APP_COM_USB_PLUG_OID, request->string, strlen(GW_APP_COM_USB_PLUG_OID)) == 0){//control usb plug
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_AI_SOCKET_USB) )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		for(i = 0; i < pni_temp->epNum; i ++)
		{
			if(pni_temp->epList[i].ep == 1)
			{
				epIndex1 = i;
			}else if(pni_temp->epList[i].ep == 2){
				epIndex2 = i;
			}
		}
	
		if((epIndex1 == 0xff) || (epIndex2 == 0xff))
		{
			printf("the dev info was error\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		
		while(objs != NULL)
		{
			if(strcmp(objs->string,"RO") == 0)
			{
				pni_temp->epList[epIndex1].devData.onoffOutStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex1].ep,objs->valueint);
				devOption.UPlugSet.relayOnoff = 1;
			}else if(strcmp(objs->string,"TSr") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.UPlugSet.tsr = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TEr") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.UPlugSet.ter = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WFr") == 0){
				if(pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex1].devData.onoffOutStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.UPlugSet.wflag_r = 1;
			}else if(strcmp(objs->string,"UO") == 0){
				pni_temp->epList[epIndex2].devData.onoffOutStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[epIndex2].ep,objs->valueint);
				devOption.UPlugSet.usbOnoff = 1;
			}else if(strcmp(objs->string,"TSu") == 0){
				js_t = objs->child;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.UPlugSet.tsu = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TEu") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.UPlugSet.teu = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WFu") == 0){
				if(pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[epIndex2].devData.onoffOutStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.UPlugSet.wflag_u = 1;
			}
			objs = objs->next;
		}
		if(needSaveFlashFlag == 1)
		{
			userUpdateDevInfoDataToFlash(pni_temp);
			userUpdateTheDevTimerEvent((char*)pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_COM_SMART_PLUG_OID, request->string, strlen(GW_APP_COM_SMART_PLUG_OID)) == 0){//control smart plug
		objs = request->child;
		if(objs == NULL)
		{
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			printf("the json data was not allow with protocol\r\n");
			return 1;
		}
		intOid = get_id_from_OID(request->string,7);//index
		pni_temp = findInDevListByIndex(intOid);
		if((pni_temp == NULL) || (pni_temp->devType != NODE_INFO_TYPE_AI_SOCKET_MEASURE) )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		lastOid = get_last_id_from_OID(request->string);
		if(lastOid != 32)
		{
			printf("the control oid was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		
		while(objs != NULL)
		{
			if(strcmp(objs->string,"RO") == 0)
			{
				pni_temp->epList[0].devData.splugStateApp.onOffStatus = objs->valueint;
				userFillUartCmdForOnOff(pni_temp->shortAddr,pni_temp->epList[0].ep,objs->valueint);
				devOption.SPlugSet.relayOnoff = 1;
			}else if(strcmp(objs->string,"TS") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[0].devData.splugStateApp.timer.s_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[0].devData.splugStateApp.timer.s_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[0].devData.splugStateApp.timer.s_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[0].devData.splugStateApp.timer.s_min = js_value->valueint;
									}
								}else{
									printf("the time start was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SPlugSet.ts = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"TE") == 0){
				js_t = objs;
				do{
					if(js_t != NULL)
					{
						
						oidCount = json_get_array_size(js_t);
						if(oidCount == 4)
						{
							for(i = 0; i < oidCount; i ++)
							{
								js_value = json_get_array_item(js_t,i);
								if(js_value !=  NULL)
								{
									if(i == 0)
									{
										pni_temp->epList[0].devData.splugStateApp.timer.e_month = js_value->valueint;
									}else if(i == 1){
										pni_temp->epList[0].devData.splugStateApp.timer.e_day = js_value->valueint;
									}else if(i == 2){
										pni_temp->epList[0].devData.splugStateApp.timer.e_hour = js_value->valueint;
									}else if(i == 3){
										pni_temp->epList[0].devData.splugStateApp.timer.e_min = js_value->valueint;
									}
								}else{
									printf("the time end was error\r\n");
									ret = -1;
								}
							}
						}else{
							printf("the js count was error\r\n ");
							ret = -1;
						}
					}else{
						printf("the js for time was null\r\n");
						ret = -1;
					}
				}while(0);
				if(ret == 0)
				{
					needSaveFlashFlag = 1;
					devOption.SPlugSet.te = 1;
				}else{
					*rc = APP_CMD_RC_ERROR_PARAMETER;
					return 1;
				}
			}else if(strcmp(objs->string,"WF") == 0){
				printf("fwg:%d,ewg:%d\r\n",pni_temp->epList[0].devData.splugStateApp.timer.wk_flag , objs->valueint);
				if(pni_temp->epList[0].devData.splugStateApp.timer.wk_flag != objs->valueint)
				{
					pni_temp->epList[0].devData.splugStateApp.timer.wk_flag = objs->valueint;
					needSaveFlashFlag = 1;
				}
				devOption.SPlugSet.wflag = 1;
			}
			objs = objs->next;
		}
		
		if(needSaveFlashFlag == 1)
		{
			printf("the dev data was change,start save to flash\r\n");
			userUpdateDevInfoDataToFlash(pni_temp);
			userUpdateTheDevTimerEvent((char*)pni_temp);
		}
		
		if(devOption.modeByte != 0)
		{
			writeUserControlDevStateToCloud(USER_CONRTOL_DEV_TYPE_ZB,intOid,devOption,app_id,TEID);
		}
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_COM_DEV_CHANGE_ROOMID_OID, request->string, strlen(GW_APP_COM_DEV_CHANGE_ROOMID_OID)) == 0){//rename zigbee dev

		*rc = 1;
		return 1;
	}else if (memcmp(GW_APP_COM_DEV_RENAME_OID, request->string, strlen(GW_APP_COM_DEV_RENAME_OID)) == 0){//rename zigbee dev
		objs = request->child;
		lastOid = get_last_id_from_OID(request->string);
		pni_temp = findInDevListByIndex(lastOid);
		if(pni_temp == NULL )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		tempLen = strlen(objs->valuestring);
		if(tempLen >=  NAME_MAX_LEN)
		{
			tempLen = NAME_MAX_LEN - 1;
		}
		memset(pni_temp->name,'\0',NAME_MAX_LEN);
		memcpy(pni_temp->name,objs->valuestring,tempLen);
		userUpdateDevInfoDataToFlash(pni_temp);
		*rc = APP_CMD_RC_SUCCESS;
		gwStateForRealTime.echoNeedUpdateFlag = 1;
		return 1;
	}else if (memcmp(GW_APP_COM_DEV_DELETE_OID, request->string, strlen(GW_APP_COM_DEV_DELETE_OID)) == 0){//delete zigbee dev
		lastOid = get_last_id_from_OID(request->string);
		pni_temp = findInDevListByIndex(lastOid);
		if(pni_temp == NULL )//not find dev from index
		{
			printf("the control dev was not valid\r\n");
			*rc = APP_CMD_RC_ERROR_PARAMETER;
			return 1;
		}
		userFillUartCmdForLeaveDev(pni_temp->shortAddr);//在uart tx函数中清除ram和flash
		
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else if (memcmp(GW_APP_COM_CONTROL_ALL_ZB_LIGHT, request->string, strlen(GW_APP_COM_CONTROL_ALL_ZB_LIGHT)) == 0){//一键全开\全关zigbee灯
		printf("all light control\r\n");
		objs = request->child;
		userFillUartCmdForOnOffByGroupsOrBroadcast(ALL_LIGHT_GROUPS,GROUPS_ADDR,objs->valueint);
		*rc = APP_CMD_RC_SUCCESS;
		return 1;
	}else{
		printf("unknow oid:%s\r\n",request->string);
		*rc = APP_CMD_RC_ERROR_UNKNOW_OID;
		return 1;
	}
	*rc = 1;
	return 1;
}
void userDoNetAppChangeNodeState(json_t *json)
{
	json_t *json_pl,*json_cid,*json_teid,*json_sid,*json_sn,*json_pl_cmd;
	json_t *json_rsp_pl,*jstempOid;
	int oidCount,i;
	uint8_t encodeFlag = 1;
	char *sendData,*payload;
	int8_t rc;
	json_pl = json_get_object_item(json,"PL");
	json_cid = json_get_object_item(json,"CID");
	json_teid = json_get_object_item(json,"TEID");
	json_sid = json_get_object_item(json,"SID");
	json_sn = json_get_object_item(json,"SN");
	if((json_pl == NULL) ||(json_cid == NULL) ||(json_teid == NULL) ||(json_sid == NULL) ||(json_sn == NULL))
	{
		printf("set:the rec app json data was not valid\r\n");
		return;
	}
	json_pl_cmd = json_pl->child;

	if(json_pl_cmd != NULL)
	{
		if(hmiotsDoRecvSetMibCallback(json_pl_cmd,json_teid->valuestring,json_sn->valueint,json_sid->valuestring,&rc))
		{
			userCreateJsplToBuf(json_pl,rc,MESSAGE_RSP_STATUS_CHANGE,json_sid->valuestring,json_teid->valuestring,json_sn->valueint,encodeFlag,DATA_SEND_RCV_TYPE_ALL,0xffffffff);
		}
	}

}

void userDoEchoCmdControlSubDev(HM_DEV_RAM_LIST *pni,uint8_t ep,char *cmd,uint8_t value)
{
	uint8_t level;
	uint8_t j,tmEp = 0xff;

	if((pni == NULL) || (cmd == NULL))
	{
		printf("the cmd was not valid\r\n");
		return;
	}

	do{
		switch(pni->devType)
		{
			case NODE_INFO_TYPE_LIGHT_RGB:
				printf("deal echo output for RGB\r\n");
				if(strcmp(cmd,"turnOn") == 0)
				{
					userFillUartCmdForOnOff(pni->shortAddr,ep,1);
				}else if(strcmp(cmd,"turnOff") == 0){
					userFillUartCmdForOnOff(pni->shortAddr,ep,0);
				}else  if(strcmp(cmd,"setPercentage") == 0){
					pni->epList[0].devData.colorStateApp.level = 255 * value / 100;
					level = 255 * value / 100;
					userFillUartCmdForLevel(pni->shortAddr,ep,MOVE_TO_LEVEL_WTONOFF,level,0);
				}else  if(strcmp(cmd,"incrementPercentage") == 0){
					level = 255 * value / 100;
					if(level + pni->epList[0].devData.colorStateApp.level > 255)
					{
						pni->epList[0].devData.colorStateApp.level = 255;
					}else{
						pni->epList[0].devData.colorStateApp.level += level;
					}
					userFillUartCmdForLevel(pni->shortAddr,ep,MOVE_TO_LEVEL_WTONOFF,pni->epList[0].devData.colorStateApp.level,0);
				}else  if(strcmp(cmd,"decrementPercentage") == 0){
					level = 255 * value / 100;
					if(level > pni->epList[0].devData.colorStateApp.level )
					{
						pni->epList[0].devData.colorStateApp.level = 0;
					}else{
						pni->epList[0].devData.colorStateApp.level -= level;
					}
					userFillUartCmdForLevel(pni->shortAddr,ep,MOVE_TO_LEVEL_WTONOFF,pni->epList[0].devData.colorStateApp.level,0);
				}else{
					printf("the cmd was error\r\n");
					break;
				}
				
				break;
			case NODE_INFO_TYPE_AI1SW:
			case NODE_INFO_TYPE_AI2SW:
			case NODE_INFO_TYPE_AI3SW:
				printf("deal echo output for switch\r\n");
				for(j = 0; j < pni->epNum; j ++)
				{
					if(pni->epList[j].ep == ep)
					{
						tmEp = j;
					}
				}
				if(tmEp != 0xff)
				{
					if(strcmp(cmd,"turnOn") == 0)
					{
						userFillUartCmdForOnOff(pni->shortAddr,ep,1);
					}else if(strcmp(cmd,"turnOff") == 0){
						userFillUartCmdForOnOff(pni->shortAddr,ep,0);
					}else{
						printf("the cmd was error\r\n");
						break;
					}
				}
				
				break;
			case NODE_INFO_TYPE_RELAY_CONTROL:
				printf("echo output for relay\r\n");
				break;
			case NODE_INFO_TYPE_AI_SOCKET_USB:
				printf("echo output for usb plug\r\n");
				for(j = 0; j < pni->epNum; j ++)
				{
					if(pni->epList[j].ep == ep)
					{
						tmEp = j;
					}
				}
				if(tmEp != 0xff)
				{
					if(strcmp(cmd,"turnOn") == 0)
					{
						userFillUartCmdForOnOff(pni->shortAddr,ep,1);
					}else if(strcmp(cmd,"turnOff") == 0){
						userFillUartCmdForOnOff(pni->shortAddr,ep,0);
					}else{
						printf("the cmd was error\r\n");
						break;
					}
				}
				break;
			case NODE_INFO_TYPE_AI_SOCKET_MEASURE:
				printf("echo output for smart plug\r\n");
				if(strcmp(cmd,"turnOn") == 0)
				{
					userFillUartCmdForOnOff(pni->shortAddr,ep,1);
				}else if(strcmp(cmd,"turnOff") == 0){
					userFillUartCmdForOnOff(pni->shortAddr,ep,0);
				}else{
					printf("the cmd was error\r\n");
					break;
				}
				break;
			case NODE_INFO_TYPE_WARNING_DEV: //reserve
				printf("echo output for wd\r\n");
				//if(strcmp(cmd,"turnOn") == 0)
				//{
				//	userAppControlWDFunc(pni->shortAddr,pni->epList[0].ep,1,pni->epList[0].devData.wdStateApp.duration);
				//}else if(strcmp(cmd,"turnOff") == 0){
				//	userAppControlWDFunc(pni->shortAddr,pni->epList[0].ep,0,pni->epList[0].devData.wdStateApp.duration);
				//}else{
				//	printf("the cmd was error\r\n");
				//	break;
				//}

				break;
			case NODE_INFO_TYPE_CUSTOM_RGB_AREA:
				printf("deal echo output for cut rgb\r\n");
				for(j = 0; j < pni->epNum; j ++)
				{
					if(pni->epList[j].ep == ep)
					{
						tmEp = j;
					}
				}
					
				if(tmEp != 0xff)
				{
					if(strcmp(cmd,"turnOn") == 0)
					{
						userFillUartCmdForOnOff(pni->shortAddr,ep,1);
					}else if(strcmp(cmd,"turnOff") == 0){
						userFillUartCmdForOnOff(pni->shortAddr,ep,0);
					}else  if(strcmp(cmd,"setPercentage") == 0){
						pni->epList[tmEp].devData.cstRgbStateApp.brightness = value ;
						userFillUartCmdForBrightness(pni->shortAddr,ep,pni->epList[tmEp].devData.cstRgbStateApp.brightness);
					}else  if(strcmp(cmd,"incrementPercentage") == 0){
						if(value + pni->epList[tmEp].devData.cstRgbStateApp.brightness > 100)
						{
							pni->epList[tmEp].devData.cstRgbStateApp.brightness = 100;
						}else{
							pni->epList[tmEp].devData.cstRgbStateApp.brightness += value;
						}
						userFillUartCmdForBrightness(pni->shortAddr,ep,pni->epList[tmEp].devData.cstRgbStateApp.brightness);
					}else  if(strcmp(cmd,"decrementPercentage") == 0){
						if(value > pni->epList[tmEp].devData.cstRgbStateApp.brightness )
						{
							pni->epList[tmEp].devData.cstRgbStateApp.brightness = 0;
						}else{
							pni->epList[tmEp].devData.cstRgbStateApp.brightness -= value;
						}
						userFillUartCmdForBrightness(pni->shortAddr,ep,pni->epList[tmEp].devData.cstRgbStateApp.brightness);
					}else{
						printf("the cmd was error\r\n");
						break;
					}
				}
				break;
			default:
				printf("the echo output was unknow dev\r\n");
				break;
		}	
	}while(0);
}

void userIotsDeviceDoRecv(uint8_t *data,uint16_t dataLen,uint8_t rcvType,uint32_t appDevId)
{
	json_t *json, *cjson_CID,*cjson_version;
	if(data == NULL)
	{
		printf("the net rcv do something was null\r\n");
		return;
	}

	json = json_parse_with_opts(data,0,0);
	if(json == NULL)
	{
		printf("this is not json data\r\n");
		return;
	}

	cjson_version = json_get_object_item(json, "version");
	if((cjson_version != NULL) && (cjson_version->valueint != 0))//for echo cmd control
	{
		json_t *json_mac,*json_action,*json_value;
		uint8_t devMac[10],i,len;
		char tempString[20],temp[3];
		HM_DEV_RAM_LIST *pni;
		HM_GW_LIST *pgw_info=&gw_info;
		printf("echo com\r\n");
		//DO ECHO

		do{
			json_mac = json_get_object_item(json, "mac");
			json_action = json_get_object_item(json, "action");
			json_value = json_get_object_item(json, "value");
			if((json_mac == NULL) || (json_action == NULL))
			{
				printf("the json was not valid\r\n");
				break;
			}
			len = strlen(json_mac->valuestring);
			memcpy(tempString,json_mac->valuestring,len);
			for(i = 0; i < len/2; i ++)
			{
				temp[0] = tempString[2*i];
				temp[1] = tempString[2*i + 1];
				temp[2] = '\0';
				sscanf_ext(&temp[0],&devMac[i]);
				printf("test 999999:%02x\r\n",devMac[i]);
			}
			if(len == ECHO_CONTROL_DEV_CMD_LEN)
			{
				 printf("echo control zigbee dev\r\n");
				 pni = findInDevListByMac(devMac);
				 if(pni == NULL)
				 {
					printf("the dev mac is not in gw\r\n");
					break;
				 }
				 if(json_value != NULL)
				 {
				 	userDoEchoCmdControlSubDev(pni,pni->epList[0].ep,json_action->valuestring,json_value->valueint);
				 }else{
					userDoEchoCmdControlSubDev(pni,pni->epList[0].ep,json_action->valuestring,0);
				 }
			}else if(len == ECHO_CONTROL_DEV_SUB_CMD_LEN){
				 printf("echo control zigbee dev ep,the ep:%d\r\n",devMac[8]);
				 pni = findInDevListByMac(devMac);
				 if(pni == NULL)
				 {
					printf("the dev mac is not in gw\r\n");
					break;
				 }
				  if(json_value != NULL)
				 {
				 	userDoEchoCmdControlSubDev(pni,devMac[8],json_action->valuestring,json_value->valueint);
				 }else{
					userDoEchoCmdControlSubDev(pni,devMac[8],json_action->valuestring,0);
				 }
			}else if(len == ECHO_CONTROL_GW_CMD_LEN){
				printf("echo control gw scene\r\n");
				if(memcmp(devMac,hmIotsDevInfo.wifiMac,6) == 0)
				{
					if((devMac[6]%10) < MAX_SCENE_NUM)
					{
						if(pgw_info->scene_list[devMac[6]%10].sceneId == devMac[6])
						{
							if(strcmp(json_action->valuestring,"turnOn") == 0)
							{
								printf("fill exe scene ,id:%d\r\n",devMac[6]);
								userFillSceneExeBuf(devMac[6],0);
							}
						}
					}else{
						printf("the scene num was too large\r\n");
						break;
					}
				}else{
					printf("this is not the gw mac\r\n");
					break;
				}
			}else{
				printf("the cmd len was unknow:%d\r\n",len);
				break;
			}
		}while(0);
		
	}else{//fro app cmd control
		printf("app cmd\r\n");
		cjson_CID = json_get_object_item(json, "CID");
		switch(cjson_CID->valueint)
		{
			case MESSAGE_FROM_APP_CHANGE_LIGHT_STATUS:
				{
					userDoNetAppChangeNodeState(json);
				}
				break;
			case MESSAGE_FROM_APP_GETLIGHTLISTINFO:
				{
					userDoNetAppGetNodeState(json,rcvType,appDevId);
				}
				break;
			default:
				break;
		}
	}

	if(json != NULL)
	{
		json_delete(json);
		json = NULL;
	}
}


void userDealNetDataFunc(void)
{
	uint32_t appid;
	uint8_t i;
	uint32_t buflen;
	if(netRcvInfo.netrcvflag)
	{
		
		printf("net rcv data dealing:buf num %d\r\n",netRcvInfo.netrcvnum);
		if(netRcvInfo.netrcvnum <= NET_RCV_PK_NUM)
		{
			for(i = 0; i < netRcvInfo.netrcvnum; i ++)
			{
				userIotsDeviceDoRecv(&netRcvInfo.netrcvbuf[i * NET_RCV_BUF_BASIC],strlen(&netRcvInfo.netrcvbuf[i * NET_RCV_BUF_BASIC]),netRcvInfo.netappinfo[i].dealType,netRcvInfo.netappinfo[i].appDevId);
			}
		}
		memset(netRcvInfo.netrcvbuf,'\0',netRcvInfo.netrcvnum * NET_RCV_BUF_BASIC);
		netRcvInfo.netrcvnum = 0;
		netRcvInfo.netrcvflag = 0;
	}

	if(netSendInfo.netsendflag)
	{
		printf("net start sending data\r\n");
		if(netSendInfo.netsendnum < NET_SEND_PK_NUM)
		{
			
			for(i = 0; i < netSendInfo.netsendnum; i ++)
			{
				appid = netSendInfo.netappinfo[i].appDevId;
				buflen = strlen(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC] );
				printf("send data:%s\r\n",&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC]);
				if(netSendInfo.netappinfo[i].dealType == DATA_SEND_RCV_TYPE_ALL)
				{
					printf("send tcp and udp data\r\n");
					if(tGlobal_net.remote_state == 1)
					{
						if(buflen > 1000)
						{
							//XlinkSendTcpPipe2(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],1000);
							//XlinkSendTcpPipe2(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC + 1000],buflen - 1000);
							
							//XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],1000,NULL);
							//XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC + 1000],buflen - 1000,NULL);
						}else{
							//XlinkSendTcpPipe2(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],buflen);
							//XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],buflen,NULL);
						}
					}else{
						if(buflen > 1000)
						{
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],1000,NULL);
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC + 1000],buflen - 1000,NULL);
						}else{
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],buflen,NULL);
						}
					}
				}else if(netSendInfo.netappinfo[i].dealType == DATA_SEND_RCV_TYPE_TCP){
					if(tGlobal_net.remote_state == 1)
					{
						printf("send tcp data for appid:%d\r\n",appid);
						if(buflen > 1000)
						{
						//	XlinkSendTcpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],1000,appid);
						//	XlinkSendTcpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC + 1000],buflen - 1000,appid);
						}else{
						//	XlinkSendTcpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],buflen,appid);
						}
					}
				}else if(netSendInfo.netappinfo[i].dealType == DATA_SEND_RCV_TYPE_UDP){
						printf("send udp data \r\n");
						if(buflen > 1000)
						{
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],1000,NULL);
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC + 1000],buflen - 1000,NULL);
						}else{
						//	XlinkSendUdpPipe(&netSendInfo.netsendbuf[i * NET_SEND_BUF_BASIC],buflen,NULL);
						}
				}
				//判断是否单点发送还是广播发送，查询的为单点，控制与主动上报为广播
			}
		}
		memset(netSendInfo.netsendbuf,'\0',netSendInfo.netsendnum * NET_SEND_BUF_BASIC);
		netSendInfo.netsendflag = 0;
		netSendInfo.netsendnum = 0;
	}
}


static void hmiots_do_zigbee_event(void)
{
	json_t *jsMain,*jsHttpSendBuf,*jsNewPl;
	char tempData[32];
	char strData[5];
	char *payload;
	uint16_t otaType,clusterId;
	uint8_t i,otaMode;
	uint8_t *tempKey;
	uint16_t keyLen;
	uint32_t cmdSn;
	char key[16];
	uint16_t tempIndex;
	uint8_t versionLen,version[10],status,level;
	HM_DEV_RAM_LIST *pni;
	DEVINFOOPTION devOption1;
	
	for(i = 0; i < MAX_IOTS_BUF_SIZE; i ++)
	{
		if(zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id != HFZ_EVENT_INVALID_EVENT)
		{
			printf("zigbeeFuncToNetBuf[i].zigbee_dev_node_id=%x\n",zigbeeFuncToNetBuf[i].zigbee_dev_node_id);
			pni = findInDevListByNwkadd(zigbeeFuncToNetBuf[i].zigbee_dev_node_id);

			if(pni!=NULL)
			{
				printf("zigbeeFuncToNetBuf[i].zigbee_dev_node_id  have data\n");
			}
			else{
				printf("zigbeeFuncToNetBuf[i].zigbee_dev_node_id  NULL\n");
				}
			if((pni != NULL) || (zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id == HFZ_EVENT_NODE_REMOVED) ||  (zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id == HFZ_EVENT_SOURCE_OTA))
			{
				switch(zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id)
				{
					case HFZ_EVENT_NEW_NODE_ADDED:
						//for app
						printf("new dev add send to app\r\n");
						
						jsMain = json_create_object();
						jsNewPl = userCreateNodeJsPl(pni,CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID);
						json_add_item_to_object(jsMain,GW_FUNC_ACTION_NEW_DEV_UP_OID,jsNewPl);
						//userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);//在主动上报时
						if(jsMain != NULL)
						{	
							json_delete(jsMain);
							jsMain = NULL;
						}
						//hardy add
						
						devOption1.modeByte = ALL_DEV_STATE_ACTION;
						userCreatNodeState_data(pni,CREATE_DEV_STATE_FOR_NEW_NODE_JOIN_OID,devOption1);

						//userCreateNodeState_pointdata();
						
						break;
					case HFZ_EVENT_NODE_REMOVED:
						jsMain = json_create_object();
						memset(tempData,'\0',32);
						memcpy((uint8_t*)&tempIndex,zigbeeFuncToNetBuf[i].para,sizeof(tempIndex));
						sprintf(tempData,"%s%d",GW_APP_COM_DEV_DELETE_OID,tempIndex);
						printf("dev remove send to app,index:%d\r\n",tempIndex);
						//for app
						json_add_item_to_object(jsMain,tempData,json_create_string("{}"));
						userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
						if(jsMain != NULL)
						{
							json_delete(jsMain);
							jsMain = NULL;
						}
						//for https
						//userFillHttpsBufForDev(pni->devType,NULL,HTTPS_DELETE_DEV_CMD);
						break;
					case HFZ_EVENT_NODE_UPDATE_SPECIAL:
						printf("specil state change send to app\r\n");
						//设备状态改变处理，跟HFZ_EVENT_NODE_STATE_CHANGE类似，但是是特殊处理，
						//它没有上报云记录，所以需要app透传
						jsMain = userCreateNodeJsPl(pni,CREATE_DEV_STATE_FOR_UPDATE_STATE_OID);
						//userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
						if(jsMain != NULL)
						{
							json_delete(jsMain);
							jsMain = NULL;
						}
						
						devOption1.modeByte = ALL_DEV_STATE_ACTION;
						userCreatNodeState_data(pni,CREATE_DEV_STATE_FOR_UPDATE_STATE_OID,devOption1);//
						
						break;
					case HFZ_EVENT_NODE_STATE_CHANGE:
						//for app  
						printf("state change send to app\r\n");
						/*jsMain = userCreateNodeJsPl(pni,CREATE_DEV_STATE_FOR_UPDATE_STATE_OID);
						//userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_UDP,0);
						if(jsMain != NULL)
						{
							json_delete(jsMain);
							jsMain = NULL;
						}*/

						devOption1.modeByte = ALL_DEV_STATE_ACTION;
						userCreatNodeState_data(pni,CREATE_DEV_STATE_FOR_UPDATE_STATE_OID,devOption1);//
						break;
					case HFZ_EVENT_NODE_STATE_ALARM:
						//for app
						printf("dev alarm send to app\r\n");
						jsMain = userCreateNodeJsPl(pni,CREATE_dEV_STATE_FOR_ALARM_OID);
						//userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_UDP,0);
						if(jsMain != NULL)
						{
							json_delete(jsMain);
							jsMain = NULL;
						}

						devOption1.modeByte = ALL_DEV_STATE_ACTION;
						userCreatNodeState_data(pni,CREATE_dEV_STATE_FOR_ALARM_OID,devOption1);
						break;
					case HFZ_EVENT_AES_KEY_CHANGE:
						printf("aes key change send to app\r\n");
						tempKey = mem_alloc(64);
						if(tempKey == NULL)
						{
							printf("in do rcv net data:malloc mem was error\r\n");
							break;
						}
						memcpy(key,XLINK_PRODUCT_KEY,16);
						user_aes_encode(hmIotsDevInfo.aesKey,16,&tempKey[1],key);
						keyLen = strlen(&tempKey[1]);
						tempKey[0] = '^';
						tempKey[1 + keyLen] = '&';
						tempKey[2 + keyLen] = '\0';
						printf("encode len:%d,encode data:%s",keyLen,tempKey);
		
						jsNewPl = json_create_object();
						jsMain = json_create_object();
						json_add_item_to_object(jsNewPl,"key",json_create_string(tempKey));
						json_add_item_to_object(jsMain,WIFI_DEV_BASIC_AES_OID,jsNewPl);
						userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
						if(jsMain != NULL)
						{
							json_delete(jsMain);
							jsMain = NULL;
						}
						if(tempKey != NULL)
						{
							mem_free(tempKey);
							tempKey = NULL;
						}
						break;
					case HFZ_EVENT_SOURCE_OTA:
						printf("ota sned to app\r\n");
						otaMode = zigbeeFuncToNetBuf[i].para[0];
						otaType = zigbeeFuncToNetBuf[i].para[1] + ((uint16_t)zigbeeFuncToNetBuf[i].para[2] << 8);
						status = zigbeeFuncToNetBuf[i].para[3];
						printf("ota sned to app mode:%d,ty:%d,sta:%d\r\n",otaMode,otaType,status);
						//jsMain = userCreateOTAJsPl(otaMode,otaType,status);
						//userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_DEV_STATE_UP,NULL,NULL,1,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
						respond_otaack_passdata(&zigbeeFuncToNetBuf[i].para[0],status);
						respond_otaack_pointdata(status);
						break;
					case HFZ_EVENT_WIFI_CMD_WAIT_RSP:
						printf("app wait rsp cmd send to app\r\n");
						if(pni->devType == NODE_INFO_TYPE_VIBRATION)
						{
							
							cmdSn = zigbeeFuncToNetBuf[i].para[0] + (zigbeeFuncToNetBuf[i].para[1] << 8) + (zigbeeFuncToNetBuf[i].para[2] << 16) + (zigbeeFuncToNetBuf[i].para[3] << 24);
							clusterId = zigbeeFuncToNetBuf[i].para[4] + (zigbeeFuncToNetBuf[i].para[5] << 8);
							status = zigbeeFuncToNetBuf[i].para[6];
							printf("smdSn:%d,clusterId:%04x,status:%d\r\n",cmdSn,clusterId,status);
							if(clusterId == ZCL_IAS_ZONE_CLUSTER_ID)
							{
								level = pni->epList[0].devData.zoneStateApp.sensitivityLevel;
								jsMain = json_create_object();
								memset(tempData,'\0',32);
								sprintf(tempData,"%s%d.32",GW_APP_COM_VIBRATION_OID,pni->index);
								json_add_item_to_object(jsMain,tempData,jsNewPl = json_create_object());
								json_add_item_to_object(jsNewPl,"ST",json_create_number(pni->epList[0].devData.zoneStateApp.sensitivityLevel));
								if(status == 1)
								{
									userCreateJsplToBuf(jsMain,APP_CMD_RC_SUCCESS,MESSAGE_RSP_STATUS_CHANGE,"","",cmdSn,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
								}else{
									userCreateJsplToBuf(jsMain,APP_CMD_RC_ERROR_UNKNOW,MESSAGE_RSP_STATUS_CHANGE,"","",cmdSn,SEND_TO_APP_DATA_NEED_ENCODE,DATA_SEND_RCV_TYPE_ALL,0);
								}
							}
						}
						break;
					default:
						break;
				}
			}else{
				printf("in hm iots event:the node id is not find in dev list\r\n ");
			}
			zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id = HFZ_EVENT_INVALID_EVENT;
			zigbeeFuncToNetBuf[i].zigbee_dev_node_id = 0;
			memset(zigbeeFuncToNetBuf[i].para,0,32);
			//if(jsMain != NULL)
			{
				//json_delete(jsMain);
				//jsMain = NULL;
			}
		}
	}
}


int hmSetIotsEvent(uint8_t event_id, uint16_t id, void * param,uint8_t paramLen)
{
	uint8_t i;
	uint8_t tempIndex = 0xff;
	for(i = 0; i < MAX_IOTS_BUF_SIZE; i ++)
	{
		
		if(zigbeeFuncToNetBuf[i].zigbee_fun_to_net_event_id == HFZ_EVENT_INVALID_EVENT)
		{
			if(tempIndex == 0xff)
			{
				tempIndex = i;
			}
		}else{
			if(zigbeeFuncToNetBuf[i].zigbee_dev_node_id == id)
			{
				tempIndex = 0xff;
				break;
			}
		}
	}

	if(tempIndex < MAX_IOTS_BUF_SIZE)
	{
		zigbeeFuncToNetBuf[tempIndex].zigbee_fun_to_net_event_id = event_id;
		zigbeeFuncToNetBuf[tempIndex].zigbee_dev_node_id = id;
		memcpy(zigbeeFuncToNetBuf[tempIndex].para,param,paramLen);
	}
	return 0;
}





void userNetDataDealThread(unsigned int arg)
{
	uint32_t timeus,timeus1;
	char tempAes[40];
	qcom_thread_msleep(3000);
	printf("start the userNetDataDealThread\r\n");
	printf("---21 hmIotsDevInfo.aesKey .....=%s\r\n",hmIotsDevInfo.aesKey);
	qcom_thread_msleep(10);
	if(memcmp(hmIotsDevInfo.aesKey,DEFAULT_AES_128_KEY,16) == 0)//16 32
	{		
		timeus =  qcom_time_us();
		printf("-------init the aes key now\r\n");
		timeus1 =  qcom_time_us();
		memset(tempAes,0,sizeof(tempAes));
		sprintf(tempAes,"%08X%08X",timeus,timeus1);
		//sprintf(tempAes,"%08X%08X%08X%08X",timeus,timeus1,timeus,timeus1);
		tempAes[strlen(tempAes)]=0;
		printf("the aes:%s\r\n",tempAes);
		memset(hmIotsDevInfo.aesKey,0,sizeof(hmIotsDevInfo.aesKey));
		memcpy(hmIotsDevInfo.aesKey,tempAes,strlen(tempAes));
		hmIotsDevInfo.aesKey[strlen(hmIotsDevInfo.aesKey)]=0;
		userFlashWriteSysInfoFunc((uint8_t*)&hmIotsDevInfo);
		
		clear_zigbeenet_flag=1;
	}
	
	while(1)
	{
		while(MQTT_is_connected()==0)//add
		{
			//printf("-------MQTT_is_connected()==0-----\n");
			qcom_thread_msleep(50);
			//return;
		}
		userDealNetDataFunc();
		qcom_thread_msleep(50);
		hmiots_do_zigbee_event();
		qcom_thread_msleep(50);
	}
}


