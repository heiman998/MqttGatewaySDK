#include "qcom_common.h"
#include "qca_calendar.h"

void test_calendar()
{
    T_SYSTEM_TIME cale_time;

    cale_time.year		=  2017;	// year
    cale_time.month		=  4;	// month
    cale_time.day		=  19;	// day
    cale_time.hour		=  1;	// hour
    cale_time.minute	=  2;	// minute
    cale_time.second	=  3;	// second

    qca_calendar_init_time(&cale_time);

    qca_calendar_get_time(&cale_time);
    printf("%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
    printf("%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);

    qcom_thread_msleep(1000);

    qca_calendar_get_time(&cale_time);
    printf("%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
    printf("%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);

    qcom_thread_msleep(6000);

    qca_calendar_get_time(&cale_time);
    printf("%d-%d-%d ", cale_time.year, cale_time.month, cale_time.day);
    printf("%d:%d:%d\r\n", cale_time.hour, cale_time.minute, cale_time.second);

    
} 

