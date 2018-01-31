#include "hm_app.h"
#include "qcom_common.h"
#include "qcom_time.h"
#include "qcom_pwm_api.h"
#include "hm_halBeep.h"
#include "qcom/qcom_internal.h"
#include "hm_keyAndSmartLink.h"
//#include "qcom_adc_api.h"

extern tGlobal_net_type tGlobal_net;
extern DEV_MYSELF_INFO hmIotsDevInfo;
BEEP_INFO beep_info;
GWSTATFORTIME gwStateForRealTime;
static qcom_timer_t beepWork_timer;
static qcom_timer_t beepLevel_timer;
static qcom_timer_t lightWork_timerL;
static qcom_timer_t lightWork_timerS;
static qcom_timer_t lightWhiteOnOffTimer;
qcom_timer_t gwNightLightTimer;

TX_SEMAPHORE beep_semaphore_ptr;
TX_SEMAPHORE beep_level_semaphore_ptr;
static uint8_t beep_local_level;
extern int wifi_connect_flag;

void gpio_init_func(void)
{
	qca_gpio_set_pin_dir(BEEP_BUSY,1);
	qca_gpio_set_pin_dir(BEEP_SDA,0);
	qca_gpio_set_pin_status(BEEP_SDA,1);

	qca_gpio_set_pin_dir(ZIGBEE_RESET,0);
	qca_gpio_set_pin_status(ZIGBEE_RESET,0);

	qca_gpio_set_pin_dir(RGB_G,0);
	qca_gpio_set_pin_status(RGB_G,0);

	qca_gpio_set_pin_dir(RGB_R,0);
	qca_gpio_set_pin_status(RGB_R,0);

	qca_gpio_set_pin_dir(RGB_B,0);
	qca_gpio_set_pin_status(RGB_B,0);


	//qcom_pwm_port_set(100000, 1000, 0, PWM_5, 2);
	//qcom_pwm_control(0, 1, PWM_PORT_5);
}


/***************************************************************
白色RGB比例,最亮R:G:B = 100:96:92
***************************************************************/
void userLightWitheOnOffControl(uint8_t onoff)
{
	uint16_t rLevel ;
	uint16_t gLevel ;
	uint16_t bLevel ;
	uint8_t tempLevel = hmIotsDevInfo.light_level;
	if(tempLevel < 5)
	{
		tempLevel = 5;
	}else if(tempLevel > 100){
		tempLevel = 100;
		hmIotsDevInfo.light_level = 100;
	}
	rLevel = (uint16_t)tempLevel * 100;
	gLevel = (uint16_t)tempLevel * 96;
	bLevel = (uint16_t)tempLevel * 92;
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		if(onoff)
		{
			qcom_pwm_port_set(100000, rLevel, 0, PWM_R, 2);
			qcom_pwm_port_set(100000, gLevel, 0, PWM_G, 2);
			qcom_pwm_port_set(100000, bLevel, 0, PWM_B, 2);
			qcom_pwm_control(0, 1, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}


/**********************************************
白色闪50%亮度， 
R:50 * 100 = 5000
G:50 * 96 = 4800
B:50 * 92 =  4600
**********************************************/
void userLightWitheBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		qcom_pwm_port_set(200, 5000, 0, PWM_B, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_R, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		if(start)
		{
			qcom_pwm_port_set(200, 5000, 0, PWM_R, 2);
			qcom_pwm_port_set(200, 4800, 0, PWM_G, 2);
			qcom_pwm_port_set(200, 4600, 0, PWM_B, 2);
			qcom_pwm_control(0, 1, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}

void userLightBluBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		qcom_pwm_port_set(200, 5000, 0, PWM_B, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_R, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		if(start)
		{
			qcom_pwm_port_set(200, 5000, 0, PWM_B, 2);
			qcom_pwm_control(0, 1,PWM_PORT_B);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}

void userLightGreenBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		qcom_pwm_port_set(500, 5000, 0, PWM_B, 2);
		qcom_pwm_port_set(500, 5000, 0, PWM_R, 2);
		qcom_pwm_port_set(500, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		if(start)
		{
			qcom_pwm_port_set(500, 5000, 0, PWM_G, 2);
			qcom_pwm_control(0, 1,PWM_PORT_G);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_B | PWM_PORT_G);
		}
	}
}

// 1秒两次，250ms亮，250ms灭
void userLightRedBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 1)
	{
		qcom_pwm_port_set(200, 5000, 0, PWM_B, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_R, 2);
		qcom_pwm_port_set(200, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		if(start)
		{
			qcom_pwm_port_set(200, 5000, 0, PWM_R, 2);
			qcom_pwm_control(0, 1,PWM_PORT_R);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}

//四秒一次，实际测试大概3s，1.5s灭，1.5s亮
void userLightYellowBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		qcom_pwm_port_set(25, 5000, 0, PWM_B, 2);
		qcom_pwm_port_set(25, 5000, 0, PWM_R, 2);
		qcom_pwm_port_set(25, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		if(start)
		{
			qcom_pwm_port_set(25, 3000, 0, PWM_R, 2);
			qcom_pwm_port_set(25, 3000, 0, PWM_G, 2);
			qcom_pwm_control(0, 1,PWM_PORT_R | PWM_PORT_G);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}

void userLightVoiletBlinkControl(uint8_t start)
{
	if(gwStateForRealTime.gwIsAlarning == 0)
	{
		qcom_pwm_port_set(200, 5000, 0, PWM_G, 2);
		qcom_pwm_control(0, 0, PWM_PORT_G);
		if(start)
		{
			qcom_pwm_port_set(200, 3200, 0, PWM_R, 2);
			qcom_pwm_port_set(200, 3200, 0, PWM_B, 2);
			qcom_pwm_control(0, 1,PWM_PORT_R | PWM_PORT_B);
		}else{
			qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
		}
	}
}


static void lightWork_timerL_handle(unsigned int alarm, void *arg)
{
	if(*(uint8_t*)arg == LIGHT_STATE_DEV_JOINING)
	{
		gwStateForRealTime.gwZbIsJoining = 0;
		*(uint8_t*)arg = LIGHT_STATE_NONE; 
	}else if((*(uint8_t*)arg == LIGHT_STATE_WIFI_OTA) || (*(uint8_t*)arg == LIGHT_STATE_ZB_OTA)){
		gwStateForRealTime.gwDevIsOta = 0;
	}
}

static void lightWork_timerS_handle(unsigned int alarm, void *arg)
{
	if(*(uint8_t*)arg == LIGHT_STATE_DEV_JOINED)
	{
		if(gwStateForRealTime.gwZbIsJoining & 0x02)
		{
			gwStateForRealTime.gwZbIsJoining &= ~0x02;
		}
	}
}

static void lightWhite_timerS_handle(unsigned int alarm, void *arg)
{
	hmIotsDevInfo.light_onoff = 0;
}

 void lightNight_timerS_handle(unsigned int alarm, void *arg)
{
	if(hmIotsDevInfo.light_onoff == 2)
	{
		hmIotsDevInfo.light_onoff = 0;
	}
}
void light_work_task(unsigned int arg)
{
	printf("start light work task\r\n");
	uint8_t light_state = LIGHT_STATE_NONE;
	uint8_t staChangeFlag = 1;
	uint8_t tempSleepNum = 0;
	uint8_t last_light_state = LIGHT_STATE_NONE;
	qcom_timer_init(&lightWork_timerL, lightWork_timerL_handle, &light_state, 250*1000, ONESHOT);
	qcom_timer_init(&lightWork_timerS, lightWork_timerS_handle, &light_state, 2*1000, ONESHOT);
	qcom_timer_init(&lightWhiteOnOffTimer, lightWhite_timerS_handle, &light_state, 3*1000, ONESHOT);
	qcom_timer_init(&gwNightLightTimer, lightNight_timerS_handle, &light_state, hmIotsDevInfo.light_on_times * 1000, ONESHOT);
	while(1)
	{
		tempSleepNum ++; 
		if(tempSleepNum >= 5)
		{
			tempSleepNum = 0;
			adc_lux_read_and_check();
		}
		if(staChangeFlag == 1)
		{
			switch(light_state)
			{
				case LIGHT_STATE_NONE:
					last_light_state = LIGHT_STATE_NONE;
					break;
				case LIGHT_STATE_NIGHT_LIGHT:
					printf("led night light\r\n");
					qcom_timer_stop(&gwNightLightTimer);
					userLightWitheOnOffControl(1);
					qcom_timer_start(&gwNightLightTimer);
					last_light_state = LIGHT_STATE_NIGHT_LIGHT;
					break;
				case LIGHT_STATE_ON:
					printf("led on\r\n");
					userLightWitheOnOffControl(1);
					last_light_state = LIGHT_STATE_ON;
					break;
				case LIGHT_STATE_OFF:
					printf("led off\r\n");
					userLightWitheOnOffControl(0);
					last_light_state = LIGHT_STATE_OFF;
					break;
				case LIGHT_STATE_DEV_JOINED:
					printf("led g blink\r\n");
					qcom_timer_stop(&lightWork_timerS);
					userLightGreenBlinkControl(1);
					qcom_timer_start(&lightWork_timerS);
					last_light_state = LIGHT_STATE_DEV_JOINED;
					break;
				case LIGHT_STATE_DEV_JOINING:
					printf("led b blink\r\n");
					if(last_light_state != LIGHT_STATE_DEV_JOINED)
					{
						qcom_timer_stop(&lightWork_timerL);
						userLightBluBlinkControl(1);
						qcom_timer_start(&lightWork_timerL);
					}else{
						userLightBluBlinkControl(1);
					}
					last_light_state = LIGHT_STATE_DEV_JOINING;
					break;
				case LIGHT_STATE_ALARM:
					printf("led r blink\r\n");
					userLightRedBlinkControl(1);
					last_light_state = LIGHT_STATE_ALARM;
					break;
				case LIGHT_STATE_WIFI_OTA:
					printf("wifi led w blink\r\n");
					qcom_timer_stop(&lightWork_timerL);
					userLightWitheBlinkControl(1);
					qcom_timer_start(&lightWork_timerL);
					last_light_state = LIGHT_STATE_WIFI_OTA;
					break;
				case LIGHT_STATE_ZB_OTA:
					printf("zc led V blink\r\n");
					userLightVoiletBlinkControl(1);
					last_light_state = LIGHT_STATE_ZB_OTA;
					break;
				case LIGHT_STATE_DISCONNECT_WIFI:
					printf("led y blink\r\n");
					userLightYellowBlinkControl(1);
					last_light_state = LIGHT_STATE_DISCONNECT_WIFI;
				default:
					break;
			}
			staChangeFlag = 0;
		}

		if(gwStateForRealTime.gwIsAlarning == 1)
		{
			if(light_state != LIGHT_STATE_ALARM)
			{
				printf("alarn led action\r\n");
				light_state = LIGHT_STATE_ALARM;
				staChangeFlag = 1;
			}
		}else {
			if(gwStateForRealTime.gwDevIsOta == 1)//wifi ota
			{
				if(light_state != LIGHT_STATE_WIFI_OTA)
				{
					printf("is wifi ota led action\r\n");
					light_state = LIGHT_STATE_WIFI_OTA;
					staChangeFlag = 1;
				}
			}else if(gwStateForRealTime.gwDevIsOta == 2){//zc ota
				if(light_state != LIGHT_STATE_ZB_OTA)
				{
					printf("is zc ota led action\r\n");
					light_state = LIGHT_STATE_ZB_OTA;
					staChangeFlag = 1;
				}
			}else if(gwStateForRealTime.gwZbIsJoining & 0x02){//has dev joined
				if(light_state != LIGHT_STATE_DEV_JOINED)
				{
					printf("is dev joined led action\r\n");
					light_state = LIGHT_STATE_DEV_JOINED;
					staChangeFlag = 1;
				}
			}else if(gwStateForRealTime.gwZbIsJoining & 0x01){//dev joining
				if(light_state != LIGHT_STATE_DEV_JOINING)
				{
					printf("is dev joining led action\r\n");
					light_state = LIGHT_STATE_DEV_JOINING;
					staChangeFlag = 1;
				}
			}else if(hmIotsDevInfo.light_onoff == 1){
				if(light_state != LIGHT_STATE_ON)
				{
					printf("is led on\r\n");
					qcom_timer_stop(&lightWork_timerL);
					light_state = LIGHT_STATE_ON;
					staChangeFlag = 1;
				}
			}else if(hmIotsDevInfo.light_onoff == 2){//小夜灯效果
				if(light_state != LIGHT_STATE_NIGHT_LIGHT)
				{
					printf("is led night light\r\n");
					light_state = LIGHT_STATE_NIGHT_LIGHT;
					staChangeFlag = 1;
				}
			}else if((tGlobal_net.remote_state != 1) || (gwStateForRealTime.wifiConnect != 1)){
				if(light_state != LIGHT_STATE_DISCONNECT_WIFI)
				{
					printf("is led disconnect server\r\n");
					qcom_timer_stop(&lightWork_timerL);
					light_state = LIGHT_STATE_DISCONNECT_WIFI;
					staChangeFlag = 1;
				}
			}else if(hmIotsDevInfo.light_onoff == 0){
				if(light_state != LIGHT_STATE_OFF)
				{
					printf("is led off\r\n");
					qcom_timer_stop(&lightWork_timerL);
					light_state = LIGHT_STATE_OFF;
					staChangeFlag = 1;
				}
			}
		}
		qcom_thread_msleep(100);
	}
	
}
//tGlobal_net.remote_state != 1) || (gwStateForRealTime.wifiConnect != 1
void set_mqttserver_connected(void)
{
	tGlobal_net.remote_state=1;
	gwStateForRealTime.wifiConnect = 1;
	wifi_connect_flag=1;//add
	//user_start_dealthread();//add
}

void userControlGWLedFunc(uint8_t state,uint16_t timers)
{
	if(state == 1)
	{
		if(hmIotsDevInfo.light_onoff == 0)
		{
			if(timers != 0)
			{
				qcom_timer_stop(&lightWhiteOnOffTimer);
				qcom_timer_init(&lightWhiteOnOffTimer, lightWhite_timerS_handle, NULL, timers*1000, ONESHOT);	
				qcom_timer_start(&lightWhiteOnOffTimer);
			}
			hmIotsDevInfo.light_onoff = 1;
		}
	}else{
		hmIotsDevInfo.light_onoff = 0;
	}
}

static uint8_t beep_wait_release(void)
{
    uint8_t i;
    for(i = 0; i < 200; i ++)
    {
        qcom_thread_msleep(20);
        if( qca_gpio_get_pin_status(BEEP_BUSY ) == 1)
          break;
    }
    if(i == 200)
    {
        printf("the beep was busy");
        return 0;
    }else{
        printf("the beep was not busy");
        return 1;
    }
     
}

static void beep_write_byte(uint8_t data)
{
	uint8_t i;
	qca_gpio_set_pin_status(BEEP_SDA,0);//beep_sda_set(0);
	us_delay(1000);//如果写在一起，程序会被优化，延时时间只有1.2ms，不符合启动条件
	us_delay(1000);
	us_delay(1000);
	for(i = 0; i < 8; i ++)
	{
		qca_gpio_set_pin_status(BEEP_SDA,1);
		if(data & 0x01)
		{
			us_delay(600);
			qca_gpio_set_pin_status(BEEP_SDA,0);
			us_delay(200);
		}else{
			us_delay(200);
			qca_gpio_set_pin_status(BEEP_SDA,0);
			us_delay(600);
		}
		data >>=1;
	}
	qca_gpio_set_pin_status(BEEP_SDA,1);
	us_delay(400);
}



static void beep_mix_mode(uint8_t data1,uint8_t data2)
{
	beep_write_byte(data1);
	if(data2 != 0)
	{
		if(data2 < 0xdf)
		{
			beep_wait_release();
			beep_write_byte(data2);
		}else{
			beep_write_byte(data2);
		}
	}
}
static void beep_action_arm(void)
{
	if(hmIotsDevInfo.app_language == LANGUAGE_ENGLISH)
	{
		//beep_mix_mode(8,11);
		beep_write_byte(8);
	}else{
		//beep_mix_mode(4,7);
		beep_write_byte(4);
	}
}

static void beep_action_home_arm(void)
{
	if(hmIotsDevInfo.app_language == LANGUAGE_ENGLISH)
	{
		//beep_mix_mode(9,11);
		beep_write_byte(9);
	}else{
		//beep_mix_mode(5,7);
		beep_write_byte(5);
	}
}

static void beep_action_disarm(void)
{
	if(hmIotsDevInfo.app_language == LANGUAGE_ENGLISH)
	{
		beep_mix_mode(10,11);
	}else{
		beep_mix_mode(6,7);
	}
}

static void beep_action_return_alarm(void)
{
	beep_mix_mode(2,0xfe);
}

static void beep_action_set_language(void)
{
	if(hmIotsDevInfo.app_language == LANGUAGE_ENGLISH)
	{
		beep_write_byte(11);
	}else{
		beep_write_byte(7);
	}
	
}
static void beep_action_sound_regulation(unsigned char value)
{
	unsigned char sound_value= ((value + 1) * 15) /100;
	if(sound_value > 15 )
		sound_value = 15;
	beep_write_byte(0xe0 + sound_value);
}

static void beep_action_alarm(void)
{
	beep_mix_mode(2,0xf2);
}

static void beep_action_stop_alarm(void)
{
	beep_write_byte(0xfe);
}


void adc_lux_read_and_check(void)
{
	uint16_t tempLux;
	qca_adc_init(ADC_LIGHT_CHANNEL);
	tempLux = qca_adc_read(ADC_LIGHT_CHANNEL);
	//printf("the enviroment lux:%d\r\n",tempLux);
	qca_adc_close();
	if(tempLux >= 2400)
	{
		gwStateForRealTime.environment_lux = ENVIRONMENT_DARK;
	}else{
		gwStateForRealTime.environment_lux = ENVIRONMENT_BRIGHT;
	}
}

void adc_check_power_on_or_off(void)
{
	uint16_t tempAdc;
	qca_adc_init(ADC_5V_CHECK_CHANNEL);
	tempAdc = qca_adc_read(ADC_5V_CHECK_CHANNEL);
	printf("the power adc is:%d\r\n",tempAdc);
	qca_adc_close();
	/******************************
V=3.3*（adc-1000）/(3000-1000)；
分压电阻相等，所以正常电压在2.5V左右，
在此，判断电压小于2V时，就判断外部电源
断电，断电标志置1，adc的值为2200
	******************************/
	if(tempAdc < 1900)
	{
		us_delay(1000);
		us_delay(1000);
		us_delay(1000);
		us_delay(1000);
		us_delay(1000);
		qca_adc_init(ADC_5V_CHECK_CHANNEL);
		tempAdc = qca_adc_read(ADC_5V_CHECK_CHANNEL);
		printf("the power adc is:%d",tempAdc);
		qca_adc_close();
		if(tempAdc < 1900)
		{
			uint16_t testn = 0;
			uint8_t i;
			uint8_t *data;
			gwStateForRealTime.exPowerDown = 1;
			qca_gpio_set_pin_status(RGB_R,0);
			qca_gpio_set_pin_status(RGB_G,0);
			qca_gpio_set_pin_status(RGB_B,0);
			qcom_power_set_mode(DEVICE_ID, REC_POWER);
			qca_gpio_set_pin_status(ZIGBEE_RESET,0);
			printf("the extern power was down,please save flash\r\n");
			data = mem_alloc(4096);
			memset(data,0,4096);
			for(i = 0; i < 5; i ++)
			{
				qca_save_flash_params(FLASH_BASIC_ADDR + i * 4096,data,4096);
			}
				
			while(1)
			{
				testn ++;
				printf("the extern power was down,please save flash:%d\r\n",testn);
				us_delay(100000);
				printf("the extern power was down,please save flash:%d\r\n",testn);
			}
		}
	}else{
		if(gwStateForRealTime.exPowerDown == 1)
		{
			gwStateForRealTime.exPowerDown = 0;
			qcom_power_set_mode(DEVICE_ID, MAX_PERF_POWER);
			qca_gpio_set_pin_status(ZIGBEE_RESET,1);
		}
	}
}




void beep_work_task1(unsigned int arg)
{
    uint8_t i;
    DEV_NUM_INFO readNum;
    printf("enter the beep work thread");
    gpio_init_func();
    while(1)
    {
    	//adc_lux_read_and_check();
    	//readNum = userFlashReadNumFunc();
	//printf("the dev num:%d,linkage num:%d,scene num:%d",readNum.devNum,readNum.linkageNum,readNum.sceneNum);
	//qcom_thread_msleep(10000);

	//adc_lux_read_and_check();
	printf("start light work task 2\r\n");
	qcom_thread_msleep(50);
	printf("start light work task 3\r\n");
	//adc_check_power_on_or_off();
	qcom_thread_msleep(50);
	get_user_times();
	if(is_time_achieve_hm(1,14,40,14,50,0xff))
	{
		printf("it is now time\r\n");
	}else{
		printf("it is not now time\r\n");
	}
        for(i = 1 ;i <= 4; i ++)
	{
		beep_mix_mode(i,0x00);
		qcom_thread_msleep(5000);
	}; 
        beep_action_alarm();
       // qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
      //  qcom_pwm_port_set(100000, 5000, 0, PWM_R, 2);
      //  qcom_pwm_control(0, 1,PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
     //  userLightBluBlinkControl(1);
     userLightVoiletBlinkControl(1);
        qcom_thread_msleep(10000);
       // beep_action_home_arm();
      //  qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
      //  qcom_pwm_port_set(100000, 5000, 0, PWM_G, 2);
      //  qcom_pwm_control(0, 1,PWM_PORT_G);
      // userLightGreenBlinkControl(1);
      //  qcom_thread_msleep(10000);
        //beep_action_disarm();
      //  qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
     //   qcom_pwm_port_set(100000, 5000, 0, PWM_B, 2);
     //   qcom_pwm_control(0, 1,PWM_PORT_B);
      // userLightRedBlinkControl(1);
     //   qcom_thread_msleep(10000);
     //   qcom_pwm_control(0, 0, PWM_PORT_R | PWM_PORT_G | PWM_PORT_B);
     //   qcom_pwm_port_set(100000, 5000, 0, PWM_B, 2);
     //   qcom_pwm_control(0, 1,PWM_PORT_R);
     	userLightYellowBlinkControl(1);
     	qcom_thread_msleep(10000);
     	userLightWitheOnOffControl(1);
     	qcom_thread_msleep(10000);
     	

    }
}

static void beepWork_timer_handle(unsigned int alarm, void *arg)
{
  	if(1)//((beep_info.work_type == BEEP_ALARM) || (beep_info.work_type == BEEP_SOUND_REGULA))//报警的时候如果用户调节了声音，会导致无法退出报警声音
	{
		beep_info.work_type = BEEP_RETURN_ALARM;
		beep_info.work_flag = 1;
		tx_semaphore_put(&beep_semaphore_ptr);
	}
}

static void beepLevel_timer_handle(unsigned int alarm, void *arg)
{

	if((beep_local_level  + 1) < hmIotsDevInfo.sound_level)//beep_local_level +1  和定义的值有关，由于定义的初始化的值为9，而设置值都是10的整数倍
	{
		beep_local_level += 10;
	  	if((beep_info.work_type == BEE_SOUND_LOCAL_REGULA) || (beep_info.work_type == BEEP_ALARM))
		{
			beep_info.work_type = BEE_SOUND_LOCAL_REGULA;
			beep_info.work_flag = 1;
			tx_semaphore_put(&beep_semaphore_ptr);
		}
	}
}

void beep_work_task(unsigned int arg)
{
	gpio_init_func();
	tx_semaphore_create(&beep_semaphore_ptr,"beep_work",0);
	tx_semaphore_create(&beep_level_semaphore_ptr,"beep_level",0);
	beep_local_level = BEEP_LOCAL_INIT_LEVEL;
	beep_info.work_flag = 0;
	beep_info.work_type = 0;
	qcom_timer_init(&beepWork_timer, beepWork_timer_handle, &arg, hmIotsDevInfo.beep_alarm_times * 1000, ONESHOT);
	qcom_timer_init(&beepLevel_timer, beepLevel_timer_handle, &arg, 3 * 1000, PERIODIC);
	beep_action_sound_regulation(hmIotsDevInfo.sound_level);
   	while(1)
	{
		printf("get the beep sem now\r\n");
		if(tx_semaphore_get(&beep_semaphore_ptr,TX_WAIT_FOREVER) == 0)
		{
			printf("get the beep sem successe\r\n");
			if(beep_info.work_flag == 1)
			{
				switch(beep_info.work_type)
				{
					case BEEP_ALARM:
										qcom_timer_stop(&beepWork_timer);
										qcom_timer_stop(&beepLevel_timer);
										beep_action_sound_regulation(beep_local_level);
										beep_action_alarm();
										qcom_timer_start(&beepWork_timer);
										qcom_timer_start(&beepLevel_timer);
										break;
					case BEEP_ARM:
										beep_action_arm();
										break;
					case BEEP_HOME_ARM:
										beep_action_home_arm();
										break;
					case BEEP_DISARM:
										//beepWork_timer 不能关闭，由于其定时器决定了报警时长，也是红灯闪烁的时间
										beep_action_disarm();
										qcom_timer_stop(&beepLevel_timer);//关闭喇叭声音渐变定时器
										qcom_timer_stop(&beepWork_timer);
										gwStateForRealTime.gwIsAlarning = 0;
										beep_local_level = BEEP_LOCAL_INIT_LEVEL;
										beep_action_sound_regulation(hmIotsDevInfo.sound_level);
										
										break;
					case BEEP_SOUND_REGULA:
										beep_action_sound_regulation(beep_info.value);
										break;
					case BEEP_RETURN_ALARM:
										qcom_timer_stop(&beepWork_timer);
										qcom_timer_stop(&beepLevel_timer);
										gwStateForRealTime.gwIsAlarning = 0;
										//设备报警音量不在默认音量值，或者设备本身设置的音量值就等于默认值
										if((beep_local_level != BEEP_LOCAL_INIT_LEVEL) || ((hmIotsDevInfo.sound_level <= (BEEP_LOCAL_INIT_LEVEL + 1)) && (beep_local_level == BEEP_LOCAL_INIT_LEVEL)))
										{
											beep_action_return_alarm();
											beep_local_level = BEEP_LOCAL_INIT_LEVEL;
											beep_action_sound_regulation(hmIotsDevInfo.sound_level);
										}
										
										break;
					case BEEP_LANGUAGE_SET:
										beep_action_set_language();
										break;
					case BEEP_ALARM_TIMER_CHANGE:
										qcom_timer_delete(&beepWork_timer);
										us_delay(3000);
										qcom_timer_init(&beepWork_timer, beepWork_timer_handle, &arg, hmIotsDevInfo.beep_alarm_times * 1000, ONESHOT);
										break;
					case BEEP_ALARM_SILENCE:
										//beepWork_timer 不能关闭，由于其定时器决定了报警时长，也是红灯闪烁的时间
										qcom_timer_stop(&beepLevel_timer);
										beep_action_return_alarm();
										beep_local_level = BEEP_LOCAL_INIT_LEVEL;
										beep_action_sound_regulation(hmIotsDevInfo.sound_level);
										break;
					case BEE_SOUND_LOCAL_REGULA:
										beep_action_sound_regulation(beep_local_level);
										printf("the loacl level:%d\r\n",beep_local_level);
										break;
					default:				break;
				}
				if((beep_info.work_type != BEEP_ALARM) && (beep_info.work_type != BEE_SOUND_LOCAL_REGULA))
					beep_info.work_type = 0;
				//beep_info.value = 0;//
				beep_info.work_flag = 0;
			}
		}else{
			printf("get beep sem was error\r\n");
		}
	}
}

