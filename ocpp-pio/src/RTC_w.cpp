#include "STM32RTC.h"

extern STM32RTC& rtc;

extern "C"
void
rtc_begin()
{
    rtc.begin();
}


extern "C"
void
rtc_set_time
(
    uint8_t  hours,
    uint8_t  minutes,
    uint8_t  seconds,
    uint32_t subSeconds = 1000
)
{
    rtc.setTime
    (
        hours,
        minutes,
        seconds,
        subSeconds
    );
}


extern "C"
void
rtc_set_date
(
    uint8_t day,
    uint8_t month,
    uint8_t year
)
{
    rtc.setDate
    (
        day,
        month,
        year
    );
}