#ifndef __TIME_H__
#define __TIME_H__

#include "rtc.h"

void
adjust_rtc_time
(
	RTC_HandleTypeDef *rtc,
    char time[25]
);

void
get_rtc_time
(
	RTC_HandleTypeDef *rtc,
    char time[25]
);

#endif /* __TIME_H__ */