#include "RTC.h"
#include "mjson.h"

#include "usart_ocpp.h"

static int
zero_2_int
(
	mjson_print_fn_t fn,
	void *fndata,
	va_list *ap
)
{
	int value = va_arg(*ap, int);
	if (value < 10)
		return mjson_printf(fn, fndata, "0%u", value);
	else
		return mjson_printf(fn, fndata, "%u", value);
}

static int
zero_3_int
(
	mjson_print_fn_t fn,
	void *fndata,
	va_list *ap
)
{
	int value = va_arg(*ap, int);
	if (value < 10)
		return mjson_printf(fn, fndata, "00%u", value);
	else if (value < 100)
		return mjson_printf(fn, fndata, "0%u", value);
	else
		return mjson_printf(fn, fndata, "%u", value);
}


void
adjust_rtc_time
(
	STM32RTC *rtc,
	char time[25]
)
{
	char year_str[5];
	strncpyy(year_str, time+2, 2);
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
	ul32 milliseconds;
	STR_TO_NUM(milliseconds, milliseconds_str);

	
	stm32rtc_set_time
	(
		rtc,
		hours,
		minutes,
		seconds,
		milliseconds
	);

	stm32rtc_set_date
	(
		rtc,
		day,
		month,
		year
	);
}

void
get_rtc_time
(
	STM32RTC *rtc,
	char time[25]
)
{
	u8 day;
	u8 month;
	u8 year;
	stm32rtc_get_date
	(
		rtc,
		NULL,
		&day,
		&month,
		&year
	);

	u8 hours;
	u8 minutes;
	u8 seconds;
	ul32 sub_seconds;
	stm32rtc_get_time
	(
		rtc,
		&hours,
		&minutes,
		&seconds,
		&sub_seconds,
		NULL
	);

	mjson_snprintf
	(
		time, 25,
		"20%u-%M-%MT%M:%M:%M.%MZ",
		year,
		zero_2_int, month,
		zero_2_int, day,
		zero_2_int, hours,
		zero_2_int, minutes,
		zero_2_int, seconds,
		zero_3_int, sub_seconds
	);
}
