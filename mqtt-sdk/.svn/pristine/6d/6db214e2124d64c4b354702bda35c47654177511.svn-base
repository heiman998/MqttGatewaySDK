#ifndef _HM_TIEMR_EVENT_H_
#define _HM_TIMER_EVENT_H_

#define USER_TIMER_SAVE_ALL_FLASH_TIMR		7200//600//1200=10min //14000    //µ•Œª1/2 √Î

typedef enum{
TIMER_EVENT_DEV_CONTROL,
TIMER_EVNET_LINKAGE_TRIGGER,
TIMER_EVNET_SCENE_EXECUTE
}TIMEREVENT;

typedef enum{
TIMER_EVENT_ACTION_NONE = 0,
TIMER_EVENT_ACTION_TIMERS = 1,
TIMER_EVENT_ACTION_DEAL_NOW,
}TIMERACTION;



#define MAX_SYS_TIMER_EVENT_NUM 		50
typedef struct{
uint8_t tmonth;
uint8_t tday;
uint8_t thour;
uint8_t tmin;
uint8_t tsec;
uint8_t twf;
uint8_t dealFlag;
}T_TIMER;

typedef enum{
TIMER_CHECK_STATE_NOT_VALID_EVENT = -2,
TIMER_CHECK_STATE_OUT_MODEDE = -1,
TIMER_CHECK_STATE_NOT_TRIGGER = 0,
TIMER_CHECK_STATE_TRIGGER_NOW = 1
}TIMERCHECKSTATE;


typedef struct{
uint8_t timerValid;
OUTPUTINFO output;
T_TIMER t_timer;
}SYSTIMER;

void userFillTimerEvent(T_TIMER timer,OUTPUTINFO output );
void userUpdateTimerEvent(T_TIMER timer,OUTPUTINFO output );
void userAddDelayToTimerEvent(uint16_t delays,OUTPUTINFO output);
#endif

