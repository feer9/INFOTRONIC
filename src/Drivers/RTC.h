#ifndef DRIVERS_RTC_H_
#define DRIVERS_RTC_H_

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

typedef struct {
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t dom;
	uint32_t dow;
	uint32_t doy;
	uint32_t month;
	uint32_t year;
} rtc_t;

void RTC_init	( void );
void RTC_setTime(rtc_t*);
void RTC_getTime(rtc_t*);

#endif /* DRIVERS_RTC_H_ */
