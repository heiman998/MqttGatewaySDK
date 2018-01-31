#ifndef _HM_ZIGBEE_STATE_DEAL_H
#define _HM_ZIGBEE_STATE_DEAL_H

typedef struct{
uint16_t colorIndex;
uint16_t newColor_x;
uint16_t newColor_y;
uint32_t times;
}NEWCOLORREPORT;

#define DEFUALT_ZIGBEE_JOINING_TIMERS		250

void setNodeBrightnessDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint8_t value);
void setNodeColorTempDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint16_t value);
void hmSetAllSensorDisarm(void);
void hmSetAllSensorHomeArm(void);
void hmSetAllSensorArm(void);
void setIasAlarmDetectedEvent(HM_DEV_RAM_LIST *pni, uint16_t alarm);
void setAlarmsAlarmDetectedEvent(HM_DEV_RAM_LIST *pni, uint16_t alarmCluster,uint8_t alarmCode);
void setAceComDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t aceCom,uint8_t *payload,uint8_t len);
void setNodeOnoffDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t ep,uint8_t action);
void updateNodeStateFunc(HM_DEV_RAM_LIST *pni, uint8_t eplistId,uint8_t *payload,uint8_t len);
void setNodeHumilityDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint16_t value);
void setNodeTemperatureDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, int16_t value);
void setNodePowerKWDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint32_t value);
void setNodePowerKWHDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint64_t value);
void setNodeVoltageDetectedEvent(HM_DEV_RAM_LIST *pni,uint8_t ep, uint16_t value);
void setNodeRemoveDetectedEvent(uint16_t index);
void setNodeOnlineUpdateDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t state);
void userNewDevJoiningDetectedEvent(char *newDevData,uint8_t *data,uint8_t len);
void setNodeBatteryDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t value);
void setNodeColorDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t ep,char valueType,uint16_t value);
void setNodeLevelDetectedEvent(HM_DEV_RAM_LIST *pni, uint8_t ep,uint8_t action);
void hmzColorRgbToXY(uint16_t r, uint16_t g, uint16_t b, uint16_t *color_x, uint16_t *color_y, uint8_t *color_level);
void hmzColorXYToRgb(uint16_t color_x, uint16_t color_y, uint8_t color_level, uint16_t *red, uint16_t *green, uint16_t *blue);


void userFillUartCmdForPermitJoin(uint8_t joinTime);

void userFillUartCmdForHeart(void);

void userFillUartCmdForReadDevInfo(uint16_t shortAddr);

void userFillUartCmdForReadZCnfo(void);

void userFillUartCmdForLeaveDev(uint16_t shortAddr);

void userFillUartCmdForZCOta(uint8_t softVersion);

void userFillUartCmdForSetZCToFactory(void);

void userFillUartCmdForSetZCReportRssi(void);

void userFillUartCmdForNodeDescripte(uint16_t shortAddr);

void userFillUartCmdForBrightness(uint16_t shortAddr, uint8_t ep,uint8_t brightness);

int userSetNodeSensitivityLevel(uint16_t shortAddr,uint8_t ep,uint8_t state,uint32_t cmdSn);

void setNodeSensitivityLevelEvent(HM_DEV_RAM_LIST *pni, uint8_t state);

void userFillUartCmdForOnOff(uint16_t shortAddr, uint8_t ep,uint8_t state);

void userDealWriteAttrCmdRspEvent(uint16_t shortAddr,uint16_t clusterId,uint8_t ep,uint8_t status);

void userFillUartCmdForColorTemperature(uint16_t shortAddr, uint8_t ep,uint16_t colorTemp);

void userFillUartCmdForOnOffByGroupsOrBroadcast(uint16_t addr, uint8_t addrType,uint8_t state);

void userFillUartCmdForColorControl(uint16_t shortAddr, uint8_t ep,uint16_t r,uint16_t g,uint16_t b);

void userFillUartCmdForLevel(uint16_t shortAddr, uint8_t ep,uint8_t subcmd,uint8_t level,uint8_t upOrDown);
void userFillUartCmdForWd(uint16_t shortAddr , uint8_t ep,uint8_t subcmd, uint8_t warmMode,uint16_t duration,uint8_t strobeCycle,uint8_t strobeLevel);
void userAppControlWDFunc(uint16_t shortAddr , uint8_t ep,uint8_t action,uint16_t duration);

//目前只支持一个attr的读写，暂时不考虑多个，如果有需要，后续扩展
void userFillUartCmdForWriteAttr(uint16_t shortAddr , uint8_t ep,uint16_t clusterId, uint16_t attrId,uint8_t dataType,uint8_t dataLen,uint8_t *data);


void userFillUartCmdForReadAttr(uint16_t shortAddr , uint8_t ep,uint16_t clusterId, uint16_t attrId);

void userFillUartCmdForConfigReporting(uint16_t shortAddr , uint8_t ep,uint16_t clusterId, uint16_t attrId,
											uint8_t dataType,uint16_t minTime,uint16_t maxTime,uint8_t dataLen,
											uint8_t *data);

void userFillUartCmdForClusterBind(uint16_t shortAddr , uint8_t ep,uint8_t *dstmac ,uint8_t dstep,uint16_t clusterId);
#endif

