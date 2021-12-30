#ifndef RTC_H_
#define RTC_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"

#include "Serial.h"

void
rtc_begin();

void
rtc_set_time
(
    u8  hours,
    u8  minutes,
    u8  seconds,
    u32 subSeconds
);

void
rtc_set_date
(
    u8 day,
    u8 month,
    u8 year
);


void
adjust_rtc_time(char time[25]);

#endif /* RTC_H_ */