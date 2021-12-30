#include "RTC.h"


void
adjust_rtc_time(char time[25])
{
    char year_str[5];
    strncpyy(year_str, time, 4);
    u8 year;
    STR_TO_NUM(year, year_str);

    char month_str[3];
    strncpyy(month_str, &time[5], 2);
    u8 month;
    STR_TO_NUM(month, month_str);

    char day_str[3];
    strncpyy(day_str, &time[8], 2);
    u8 day;
    STR_TO_NUM(day, day_str);

    char hours_str[3];
    strncpyy(hours_str, &time[11], 2);
    u8 hours;
    STR_TO_NUM(hours, hours_str);

    char minutes_str[3];
    strncpyy(minutes_str, &time[14], 2);
    u8 minutes;
    STR_TO_NUM(minutes, minutes_str);

    char seconds_str[3];
    strncpyy(seconds_str, &time[17], 2);
    u8 seconds;
    STR_TO_NUM(seconds, seconds_str);

    char milliseconds_str[4];
    strncpyy(milliseconds_str, &time[20], 3);
    u32 milliseconds;
    STR_TO_NUM(milliseconds, milliseconds_str);

    
    rtc_set_time
    (
        hours,
        minutes,
        seconds,
        milliseconds
    );

    rtc_set_date
    (
        day,
        month,
        year
    );
}