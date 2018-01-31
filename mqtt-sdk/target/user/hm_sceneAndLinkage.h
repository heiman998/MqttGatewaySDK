#ifndef _HM_SCENE_AND_LINKAGE_H
#define _HM_SCENE_AND_LINKAGE_H

#define MAX_ALLOW_OUT_DEV_NUM		20
#define OUT_DEV_LIST_INVALID			0
#define OUT_DEV_LIST_EXECUTE_NOW		1
#define OUT_DEV_LIST_EXECUTE_TIMER	2




typedef struct{
uint8_t needOutType;
OUTPUTINFO devOutput;
}NEEDOUTLIST;

#define MAX_SCENE_EXE_BUF_NUM			3
typedef struct{
uint8_t valid;
uint8_t sceneId;
uint8_t exeMode;//0 普通执行，需要考虑delay时间，1 直接执行，不用考虑delay时间
}SCENEEXEBUF;

int userAddSceneFunc(json_t *scenePl,uint8_t scId);
int userFillSceneExeBuf(uint8_t sceneId,uint8_t exeMode);
int userCheckSceneIdWasList(uint8_t sceneId);
int userDeleteSceneFromId(uint8_t sceneId);
void userReadSceneListFunc(uint32_t sn,uint8_t rcvType,uint32_t appDevId);
int userAddLinkageFunc(json_t *linkagePl,uint8_t lkId);
void userReadLinkageListFunc(uint32_t sn,uint8_t rcvType,uint32_t appDevId);
int userDeleteLinkageFromId(uint8_t linkageId);
int userSetEnableLinkageFunc(uint8_t linkageId,uint8_t state);
void userLinkageInputEventTriggerFunc(SCENE_LIST_TYPE type,HM_DEV_RAM_LIST *pni,DEVINFOOPTION devAction);
void userCheckDevLinkWdWasTrigger(uint16_t devIndex,uint32_t alarmSation);
int userReadDevLinkWdFunc(uint16_t index,uint32_t sn,uint8_t rcvType,uint32_t appDevId);
int userCreatDevLinkWdFunc(json_t *scenePl,HM_DEV_RAM_LIST *pni);
int userReadDevLinkGWLightNightFunc(uint32_t sn,uint8_t rcvType,uint32_t appDevId);

#endif
