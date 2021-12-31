#ifndef RTC_H_
#define RTC_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"
#include "STM32RTC.h"


void
adjust_rtc_time
(
    STM32RTC *rtc,
    char time[25]
);

#endif /* RTC_H_ */