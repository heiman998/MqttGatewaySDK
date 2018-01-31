#ifndef _HM_ZIGBEE_INTERFACE_H_
#define _HM_ZIGBEE_INTERFACE_H_

typedef struct{
	uint8_t s_month;
	uint8_t s_day;
	uint8_t s_hour;
	uint8_t s_min;
	uint8_t e_month;
	uint8_t e_day;
	uint8_t e_hour;
	uint8_t e_min;
	uint8_t wk_flag;
}FUNC_SETTIMER;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
	
	uint8_t AlarmMode_se;
}ZB_DOOR_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_WATER_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
	
	uint8_t AlarmMode_se;
}ZB_PIR_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_SMOKE_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OPState;
	uint32_t AlarmTime;
}ZB_GAS_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_CO_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_SOS_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_REMOTE_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
	int16_t Temperature;
	int16_t Humidity;

	uint16_t HumidityEnable_se;
	int16_t HumidityUp_se;
	int16_t HumidityLower_se;
	int16_t TemperatureEnable_se;
	int16_t TemperatureUp_se;
	uint16_t TemperatureLower_se;
}ZB_HUMITURE_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OFState;
	uint8_t Brightness;
	uint8_t RedVaule;
	uint16_t GreeVaule;
	uint16_t BlueVaule;
	uint32_t Time;

	uint8_t OFState_se;
	uint8_t Brightness_se;
	uint8_t Red_se;
	uint16_t Gree_se;
	uint16_t Blue_se;
	FUNC_SETTIMER Timer_se;
}ZB_RGB_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t RealyState;
	uint8_t UsbState;
	uint32_t Time;

	uint8_t Realy_se;
	uint8_t Usb_se;
	FUNC_SETTIMER RTimer_se;
	FUNC_SETTIMER UTimer_se;
}ZB_UPLUG_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t RealyState;
	uint32_t PowerValue;
	uint64_t ElectricityValue;
	uint32_t Time;

	uint8_t Realy_se;
	FUNC_SETTIMER Timer_se;
}ZB_SPLUG_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OFState;
	uint8_t Time;

	uint8_t OF_se;
	FUNC_SETTIMER Timer_se;
}ZB_Slight1_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OF1State;
	uint8_t OF2State;
	uint32_t Time;

	uint8_t OF1_se;
	FUNC_SETTIMER Timer1_se;
	uint8_t OF2_se;
	FUNC_SETTIMER Timer2_se;
}ZB_Slight2_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OF1State;
	uint8_t OF2State;
	uint8_t OF3State;
	uint32_t Time;

	uint8_t OF1_se;
	FUNC_SETTIMER Timer1_se;
	uint8_t OF2_se;
	FUNC_SETTIMER Timer2_se;
	uint8_t OF3_se;
	FUNC_SETTIMER Timer3_se;
}ZB_Slight3_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OPState;
	uint8_t BatteryAlarm;
	uint8_t BatteryRemain;	
	uint32_t Time;
	uint32_t AlarmTime;

	uint8_t OP_se;
	uint16_t AlarmTime_se;
}ZB_SAlARM_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t OF1State;
	uint8_t OF2State;
	uint8_t OF3State;
	uint8_t OF4State;
	uint8_t Brightness1Value;
	uint8_t Brightness2Value;
	uint8_t Brightness3Value;
	uint8_t Brightness4Value;
	uint16_t CWL1Value;
	uint16_t CWL2Value;
	uint16_t CWL3Value;
	uint16_t CWL4Value;
	uint32_t Time;

	uint8_t OF1_se;
	uint8_t OF2_se;
	uint8_t OF3_se;
	uint8_t OF4_se;
	uint8_t Brightness1_se;
	uint8_t Brightness2_se;
	uint8_t Brightness3_se;
	uint8_t Brightness4_se;
	uint16_t CWL1_se;
	uint16_t CWL2_se;
	uint16_t CWL3_se;
	uint16_t CWL4_se;
	FUNC_SETTIMER Timer1_se;
	FUNC_SETTIMER Timer2_se;
	FUNC_SETTIMER Timer3_se;
	FUNC_SETTIMER Timer4_se;
}ZB_FOURLAMP_PARAM;

typedef struct{
	uint16_t IDIndex;
	uint8_t Online;
	uint8_t BatteryAlarm;
	uint8_t OPState;
	uint8_t BatteryRemain;
	uint32_t AlarmTime;
}ZB_VIBRATION_PARAM;  


void hm_get_doorsensor_state(HM_DEV_RAM_LIST *pni,ZB_DOOR_PARAM zb_type);
int hm_set_doorsensor_separam(HM_DEV_RAM_LIST *pni,ZB_DOOR_PARAM zb_type);//--
int hm_get_doorsensor_separam(HM_DEV_RAM_LIST *pni,ZB_DOOR_PARAM zb_type);//--


void hm_get_watersensor_state(HM_DEV_RAM_LIST *pni,ZB_WATER_PARAM zb_type);
void hm_get_pirsensor_state(HM_DEV_RAM_LIST *pni,ZB_PIR_PARAM zb_type);
int hm_set_pirsensor_separam(HM_DEV_RAM_LIST *pni,ZB_PIR_PARAM zb_type);//-
int hm_get_pirsensor_separam(HM_DEV_RAM_LIST *pni,ZB_PIR_PARAM zb_type);//-


void hm_get_smokesensor_state(HM_DEV_RAM_LIST *pni,ZB_SMOKE_PARAM zb_type);
void hm_get_gassensor_state(HM_DEV_RAM_LIST *pni,ZB_GAS_PARAM zb_type);
void hm_get_cosensor_state(HM_DEV_RAM_LIST *pni,ZB_CO_PARAM zb_type);
void hm_get_sossensor_state(HM_DEV_RAM_LIST *pni,ZB_SOS_PARAM zb_type);
void hm_get_remotecontrol_state(HM_DEV_RAM_LIST *pni,ZB_REMOTE_PARAM zb_type);
void hm_get_humituresensor_state(HM_DEV_RAM_LIST *pni,ZB_HUMITURE_PARAM zb_type);
void hm_set_humituresensor_separam(HM_DEV_RAM_LIST *pni,ZB_HUMITURE_PARAM zb_type);//-
void hm_get_humituresensor_separam(HM_DEV_RAM_LIST *pni,ZB_HUMITURE_PARAM zb_type);//-


void hm_get_rgblight_state(HM_DEV_RAM_LIST *pni,ZB_RGB_PARAM zb_type);
void hm_set_rgblight_separam(HM_DEV_RAM_LIST *pni,ZB_RGB_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_rgblight_separam(HM_DEV_RAM_LIST *pni,ZB_RGB_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_usbplug_state(HM_DEV_RAM_LIST *pni,ZB_UPLUG_PARAM zb_type);
void hm_set_usbplug_separam(HM_DEV_RAM_LIST *pni,ZB_UPLUG_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_usbplug_separam(HM_DEV_RAM_LIST *pni,ZB_UPLUG_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_measureplug_state(HM_DEV_RAM_LIST *pni,ZB_SPLUG_PARAM zb_type);
void hm_set_measureplug_separam(HM_DEV_RAM_LIST *pni,ZB_SPLUG_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_measureplug_separam(HM_DEV_RAM_LIST *pni,ZB_SPLUG_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_ai1sw_state(HM_DEV_RAM_LIST *pni,ZB_Slight1_PARAM zb_type);
void hm_set_ai1sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight1_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_ai1sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight1_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_ai2sw_state(HM_DEV_RAM_LIST *pni,ZB_Slight2_PARAM zb_type);
void hm_set_ai2sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight2_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_ai2sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight2_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_ai3sw_state(HM_DEV_RAM_LIST *pni,ZB_Slight3_PARAM zb_type);
void hm_set_ai3sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight3_PARAM zb_type,DEVINFOOPTION devOption);//-
void hm_get_ai3sw_separam(HM_DEV_RAM_LIST *pni,ZB_Slight3_PARAM zb_type,DEVINFOOPTION devOption);//-


void hm_get_sensoralarm_state(HM_DEV_RAM_LIST *pni,ZB_SAlARM_PARAM zb_type);
void hm_set_sensoralarm_separam(HM_DEV_RAM_LIST *pni,ZB_SAlARM_PARAM zb_type);
void hm_get_sensoralarm_separam(HM_DEV_RAM_LIST *pni,ZB_SAlARM_PARAM zb_type);


void hm_get_fouredrivelamp_state(HM_DEV_RAM_LIST *pni,ZB_FOURLAMP_PARAM zb_type);
void hm_set_fouredrivelamp_separam(HM_DEV_RAM_LIST *pni,ZB_FOURLAMP_PARAM zb_type,DEVINFOOPTION devOption);
void hm_get_fouredrivelamp_separam(HM_DEV_RAM_LIST *pni,ZB_FOURLAMP_PARAM zb_type,DEVINFOOPTION devOption);
	

void hm_get_vibrationsensor_state(HM_DEV_RAM_LIST *pni,ZB_VIBRATION_PARAM zb_type);












#endif

