#include <stdio.h>
#include <time.h>
#include "hmType.h"
#include "qca_calendar.h"



#define TIMER_DAY_SUNDAY			0
#define TIMER_DAY_MONDAY			1
#define TIMER_DAY_TUESDAY			2
#define TIMER_DAY_WEDNESDAY		3
#define TIMER_DAY_THURDAY			4
#define TIMER_DAY_FRIDAY			5
#define TIMER_DAY_SATURDAT			6

enum{
DATA_EQUAL = 0,
DATA_GREATER = 1,
DATA_LESS = 2,
DATA_GREATER_OR_EQUAL = 3,
DATA_LESS_OR_EQUAL = 4
}DATA_CONDITION;

int timer_real_time_available(void)
{
	T_SYSTEM_TIME cale_time;
	qca_calendar_get_time(&cale_time);
	if((cale_time.year > 2000) && (cale_time.year < 3000))
	{
		return 1;
	}else{
		return 0;
	}
}
char is_today (unsigned char today, unsigned char flag)
{
	if(today == TIMER_DAY_SUNDAY)
	{
		if((flag & 0x40) != 0)
		{
			return 1;
		}else{
			return 0;
		}
	}else{	
		if((flag & (1<< (today-1))) != 0)
		{
			return 1;
		}else{
			return 0;
		}
	}
}


static inline unsigned long hmktime (unsigned int year, unsigned int mon,
    unsigned int day, unsigned int hour,
    unsigned int min, unsigned int sec)
        {
    if (0 >= (int) (mon -= 2)) {    /* 1..12 -> 11,12,1..10 */
         mon += 12;      /* Puts Feb last since it has leap day */
         year -= 1;
    }

    return (((
             (unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
             year*365 - 719499
          )*24 + hour /* now have hours */
       )*60 + min /* now have minutes */
    )*60 + sec; /* finally seconds */
    }

long hm_mktime(HMTM *times)
{
	struct tm tempTime;
	long nowTime;
	tempTime.tm_sec = times->tm_sec;
	tempTime.tm_min = times->tm_min;
	tempTime.tm_hour = times->tm_hour;
	tempTime.tm_mday = times->tm_mday;
	tempTime.tm_mon = times->tm_mon;
	tempTime.tm_year = times->tm_year;
	tempTime.tm_wday = times->tm_wday;
	tempTime.tm_yday = times->tm_yday;
	tempTime.tm_isdst = times->tm_isdst;
	nowTime = hmktime(tempTime.tm_year,tempTime.tm_mon,tempTime.tm_mday,tempTime.tm_hour,tempTime.tm_min,tempTime.tm_sec);
	return nowTime;
}

//(for working lixiong)
//考虑周期执行问题，enable应传入指针，当week到达星期天时，如果flag最高位没有置1，则将enable清零
unsigned char is_time_achieve_hm(unsigned char  enable , unsigned char sh, unsigned char sm, unsigned char eh, unsigned char em, unsigned char wkflag)
{
	T_SYSTEM_TIME cale_time;
	HMTM alm_tm;
	unsigned int now_time,start_time,end_time;
	if(enable==0)
		return 0;

	if((sh+sm+eh+em)==0)//anytime
		return 1;
	
	qca_calendar_get_time(&cale_time);
	if((cale_time.year < 2000) || (cale_time.year > 3000))//时间不对，默认布防有效
	{
		printf("the real time was invalid\r\n");
		return 1;
	}
	

	if(is_today(cale_time.week, wkflag) == 0)//not today
		return 0;
	
	now_time=hmktime(cale_time.year,cale_time.month,cale_time.day,cale_time.hour,cale_time.minute,cale_time.second);
	alm_tm.tm_year = cale_time.year;
	alm_tm.tm_mon= cale_time.month;
	alm_tm.tm_mday= cale_time.day;
	alm_tm.tm_hour= sh;
	alm_tm.tm_min= sm;
	alm_tm.tm_sec= 0;

	start_time=hm_mktime(&alm_tm);

	alm_tm.tm_hour= eh;
	alm_tm.tm_min= em;
	end_time=hm_mktime(&alm_tm);

	if(start_time <  end_time)
	{
		if(now_time<start_time)
			return 0;
		if(now_time>end_time)
			return 0;
	}else{
		if((now_time<start_time) && (now_time>end_time))
			return 0;
	}

	return 1;
}

double Atof(const char *pstr)
{
    double sign = 1.0;
    double num1 = 0.0;
    double num2 = 0.0;
    double point = 0.1;
    while (*pstr == ' ' || *pstr == '/t')
    {
        pstr++;
    }
    if (*pstr == '-')
    {
        sign = -1;
        pstr++;
    }
    while (*pstr)
    {
        if (*pstr == '.')
        {
            pstr++;
            while (*pstr >= '0' && *pstr <= '9')
            {
                num1 += point * (*pstr - '0');
                point *= 0.1;
                pstr++;
            }
        }
        else if (*pstr >= '0' && *pstr <= '9')
        {
            num2 = num2 * 10 + *pstr -'0';
        }
        else
        {
            return (num1 + num2) * (sign);
        }
        pstr++;
    }
    return (num1 + num2) * (sign);
}



int years(int day,int *year)
{  
	if(day<365)
		return day;

	if(*year%4==0&&*year%100!=0||*year%400==0)
	{  	
		if(day>365)
		day-=366;
		(*year)++;
	}else{
		day-=365;
		(*year)++;
	}
	day=years(day,year);
	return day;
}

void month(int *mon,int *day,int year)
{   
	switch(*mon)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			{
				if((*day)>=31)
				{
					(*mon)++;
					*day=*day-31;
				}else return;
			}break;
		case 4:
		case 6:
		case 9:
		case 11:
			{
				if((*day)>=30)
				{
					(*mon)++;
					*day=*day-30;
				}else return;
			}break;
		case 2:
			{
				if(year%4==0&&year%100!=0||year%400==0)
				{
					if((*day)>=29)
					{
						(*mon)++;
						*day=*day-29;
					}else return;
				}else{
					if((*day)>=28)
					{
						(*mon)++;
						*day=*day-28;
					}else return;
				}
			}break;
		default: break;
	}
	month(mon,day,year);
}

HMTM userChangeSTimeToYears(unsigned int time)
{
	HMTM dstTime;
	unsigned int sec;
	int day,min,hour,mon=1,y;
	int year;
	unsigned int i;
	unsigned int d;
	year=1970;
	sec=time;
	day=(int)(sec/3600/24);
	i=day*24*3600;
	sec-=i;
	d=sec;
	min=d/60%60;
	hour=d/3600;
	i=hour*3600+min*60;
	sec-=i;
	d=sec;
	day=years(day,&year);
	month(&mon,&day,year);
	dstTime.tm_year = year;
	dstTime.tm_mon = mon;
	dstTime.tm_mday = day + 1;//day 为实际过了的时间，如过了21天，实际到22号了
	dstTime.tm_hour = hour;
	dstTime.tm_min = min;
	dstTime.tm_sec = sec;
	printf("the time is\n%d/%02d/%02d  %02d:%02d:%02d\n\n",year,mon,day + 1,hour,min,sec);
	return dstTime;
}


int get_id_from_OID(char *OID, int n)	//Eg."2.1.1.3.1.15.4096.0.1   n==7", return 4096
{
	char *p=NULL;
	int i, last_oid;

	n--;
	for(i=0; i<strlen(OID)-1; i++)
	{
		p=OID +i;
		if(n==0)
		{
			last_oid=atoi(p);
			return last_oid;
		}
		
		if(*p=='.')
			n--;
	}
	return 0;
}

int get_last_id_from_OID(char *OID)	//Eg."2.1.1.3.1.15.4096.0.1", return 1
{
	char *p=NULL;
	int i=0;
	int last_oid;
	
	for(i=strlen(OID)-1; i>=0; i--)
	{
		p=OID +i;
		if(*p=='.')
		{
			last_oid=atoi(p+1);
			return last_oid;
		}
	}

	last_oid=atoi(OID);
	return last_oid;
}

int get_dot_num_from_OID(char *OID)	//Eg."2.1.1.3.1.15.4096.0.1", return 8
{
	int i;
	int num = 0;
	char *p = NULL;
	for (i=0; i<strlen(OID)-1; i++)
	{
		p = OID + i;
		if (*p == '.')
			num++;
	}
	return num;
}

int checkDataAllowConditionToAnother(int sourceData,unsigned char condition,int dstData)
{
	int ret = -1;
	switch(condition)
	{
		case DATA_EQUAL:
			if(sourceData == dstData)
			{
				ret = 0;
			}
			break;
		case DATA_GREATER:
			if(sourceData > dstData)
			{
				ret = 0;
			}
			break;
		case DATA_LESS:
			if(sourceData < dstData)
			{
				ret = 0;
			}
			break;
		case DATA_GREATER_OR_EQUAL:
			if(sourceData >= dstData)
			{
				ret = 0;
			}
			break;
		case DATA_LESS_OR_EQUAL:
			if(sourceData <= dstData)
			{
				ret = 0;
			}
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}


