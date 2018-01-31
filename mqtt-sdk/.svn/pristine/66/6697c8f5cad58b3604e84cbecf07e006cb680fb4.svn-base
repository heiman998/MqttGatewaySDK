/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __QCOM_SYSTEM_H__
#define __QCOM_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WLAN_NUM_OF_DEVICES 2

enum WDT_CMDS{
    APP_WDT_DISABLE,
    APP_WDT_ENABLE,
    APP_WDT_USER_DEF,
    APP_WDT_USER_DBG
};

/**
 *This function is used to do the initial configuration before ROM continues to load board data and other content from NVRAM. 
 *It must be called by the first executable image in NVRAM, typically sdk_proxy.out. It takes a 32-bit parameter. Bit[31-8] 
 *is the user application start address passed to ROM (typically 0xa02800). You can take a look at 
 *demos/sdk_shell/sdk_shell.target.ld and sdk_flash/1_0_sdk_flash.sh and see how the parameter is calculated and passed.
 *
 * @param param 32-bit parameters. Bit[31-8] is start address of user application (<<8). Bit[7-0] is reserved for future use.
 */
void qcom_sys_init(unsigned int param);

/**
 *This function is to start WLAN operation. It installs a lot of patches then calls wlan_main() to start the WLAN operation.
 *After wlan_main(), it also does some post-processing to setup WMI and HTC correctly. This function must be called firstly 
 *before other user functions.
 *
 * @param fn		user main function. RTOS will schedule it after init
 */

typedef struct{
	unsigned char isMccEnabled;
	unsigned char numOfVdev;
}QCOM_SYS_START_PARA_t;

void qcom_sys_start(void(*fn)(void), QCOM_SYS_START_PARA_t *pSysStartPara);

/**
 *This function is to reset to chip. It is only a warm reset by pointing PC to reset vector. We may change it later to support 
 watch dog reset or cold reset.
 */
void qcom_sys_reset(void);
void qcom_watchdog_feed(void);
int qcom_watchdog(int enable, int timeout);
void qcom_enable_print(int enable);
void qcom_sys_set_current_devid(int devid);

#define RTC_SECS_MASK           0x0000007F
#define RTC_MIN_MASK            0x00007F00
#define RTC_HOUR_MASK           0x003F0000
#define RTC_WDAY_MASK           0x07000000
#define RTC_YDAY_MASK           0x0000003F
#define RTC_MON_MASK            0x00001F00
#define RTC_YEAR_MASK           0x00FF0000

typedef struct rtc_time
{
    int  Sec;
    int  min;
    int  hour;
    int  mon;
    int  year;
    int  wday;
    int  yday;
} tRtcTime;

void qcom_set_time(tRtcTime time);
void qcom_set_scan_timeout(unsigned char sec);
/* API for semaphore put/get, adding proctect for these 2 functions
** we provide real-time timer for customer and when user want to
** use semaphore in such case, these new APIs are suggested to replace 
** orginial tx_semaphore_xx.
*/
unsigned int qcom_tx_semaphore_put(void *semaphore_ptr);
unsigned int qcom_tx_semaphore_get(void *semaphore_ptr, unsigned int wait_option);

#ifdef __cplusplus
}
#endif

#endif

