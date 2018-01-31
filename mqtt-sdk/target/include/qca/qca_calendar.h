#ifndef _QCA_CALENDAR_H_
#define _QCA_CALENDAR_H_

typedef struct {
	int		week; // SUNDAY=0, MONDAY=1 ...
	int		year;
	int		month;
	int		day;
	int		hour;
	int		minute;
	int		second;
} T_SYSTEM_TIME;

/**
* @fn void qca_calendar_init_time(T_SYSTEM_TIME *time)
* @brief init system time
* @param time setting time
* @return none
*/
void qca_calendar_init_time(T_SYSTEM_TIME *time);

/**
* @fn void qca_calendar_get_time(T_SYSTEM_TIME *time)
* @brief get current time
* @param[out] time store current time info
* @return none
*/
void qca_calendar_get_time(T_SYSTEM_TIME *time);


#endif



