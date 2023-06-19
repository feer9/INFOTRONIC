#ifndef DRIVERS_RTC_H_
#define DRIVERS_RTC_H_

#include "chip.h"
#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "Drivers/GPIO.h"

typedef struct {
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t dayOfMonth;
	uint32_t dayOfWeek;
	uint32_t dayOfYear;
	uint32_t month;
	uint32_t year;
} rtc_t;

void RTC_init		( void );
void RTC_setTime	(rtc_t*);
void RTC_getTime	(rtc_t*);
void RTC_resetTime	( void );
void RTC_setTime_fromString(char*);
void RTC_setGPREG_fromTime(void);
void RTC_setAlarmInMinutes(uint32_t minutes);
void RTC_setAlarmInSeconds(uint32_t seconds);
#endif /* DRIVERS_RTC_H_ */
