#ifndef _HM_HALBEEP_H
#define _HM_HALBEEP_H

#define BEEP_LOCAL_INIT_LEVEL	9

typedef enum{
BEEP_ALARM		= 1,
BEEP_ARM		= 2,
BEEP_HOME_ARM	= 3,
BEEP_DISARM	= 4,
BEEP_SOUND_REGULA = 5,
BEEP_RETURN_ALARM = 6,
BEEP_LANGUAGE_SET	 = 7,
BEEP_ALARM_TIMER_CHANGE = 8,
BEEP_ALARM_SILENCE = 9,
BEE_SOUND_LOCAL_REGULA = 10
}BEEP_WORK_TYPE;

typedef enum{
LIGHT_STATE_NONE = 0,
LIGHT_STATE_ON = 1,
LIGHT_STATE_OFF = 2,
LIGHT_STATE_DEV_JOINED,
LIGHT_STATE_DEV_JOINING,
LIGHT_STATE_ALARM,
LIGHT_STATE_WIFI_OTA,
LIGHT_STATE_ZB_OTA,
LIGHT_STATE_DISCONNECT_WIFI,
LIGHT_STATE_NIGHT_LIGHT
}LIGHT_WORK_TYPE;


typedef struct{
unsigned char work_flag;
unsigned char work_type;
unsigned char value;
}BEEP_INFO;

void adc_check_power_on_or_off(void);
void adc_lux_read_and_check(void);

 void lightNight_timerS_handle(unsigned int alarm, void *arg);
 void set_mqttserver_connected(void);
#endif

