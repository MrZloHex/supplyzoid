/**
  ******************************************************************************
  * @file    STM32RTC.cpp
  * @author  Zlobin Alexey
  * @brief   Provides a RTC interface for Arduino
  *
  */


#include "STM32RTC.h"

// CONTROL FUNCTION

void
stm32rtc_init
(
	STM32RTC *rtc
)
{
	rtc->_configured = false;
	rtc->_reset = false;
	rtc->source_clock = LSI_CLOCK;
}

void
stm32rtc_begin
(
	STM32RTC *rtc,
	hourFormat_t h_fmt
)
{
	if (!rtc->_configured)
	{
		rtc->format = h_fmt;
		RTC_init
		(
			h_fmt,
			rtc->source_clock,
			rtc->_reset
		);
		rtc->_configured = true;
		stm32rtc_sync_time(rtc);
		stm32rtc_sync_date(rtc);
		rtc->alarm_day = rtc->day;
		rtc->alarm_hours = rtc->hours;
		rtc->alarm_minutes = rtc->minutes;
		rtc->alarm_seconds = rtc->seconds;
		rtc->alarm_sub_seconds = rtc->sub_seconds;
		rtc->alarm_period = rtc->period;
	}
	else
	{
		stm32rtc_sync_time(rtc);
		stm32rtc_sync_date(rtc);
		stm32rtc_sync_alarm_time(rtc);
	}
}

void
stm32rtc_reset
(
	STM32RTC *rtc,
	hourFormat_t hour_fmt
)
{
	rtc->_reset = true;
	rtc->_configured = false;
	rtc->alarm_enabled = false;
	stm32rtc_begin(rtc, hour_fmt);
}

void
stm32rtc_end(STM32RTC *rtc)
{
	if (rtc->_configured)
	{
		RTC_DeInit();
		rtc->_configured = false;
		rtc->alarm_enabled = false;
	}
}

// GET FUNCTIONS

ul32
stm32rtc_get_sub_seconds(STM32RTC *rtc)
{
	stm32rtc_sync_time(rtc);
	return rtc->sub_seconds;
}

u8
stm32rtc_get_seconds(STM32RTC *rtc)
{
	stm32rtc_sync_time(rtc);
	return rtc->seconds;
}

u8
stm32rtc_get_minutes(STM32RTC *rtc)
{
	stm32rtc_sync_time(rtc);
	return rtc->minutes;
}

u8
stm32rtc_get_hsours
(
	STM32RTC *rtc,
	hourAM_PM_t *period
)
{
	stm32rtc_sync_time(rtc);
	if (period != NULL)
		*period = rtc->period;
	return rtc->hours;
}

void
stm32rtc_get_time
(
	STM32RTC *rtc,
	u8 *hours,
	u8 *minutes,
	u8 *seconds,
	ul32 *sub_seconds,
	hourAM_PM_t *period
)
{
	stm32rtc_sync_time(rtc);
	if (hours != NULL)
		*hours = rtc->hours;
	if (minutes != NULL)
		*minutes = rtc->minutes;
	if (seconds != NULL)
		*seconds = rtc->seconds;
	if (sub_seconds != NULL)
		*sub_seconds = rtc->sub_seconds;
	if (period != NULL)
		*period = rtc->period;
}

u8
stm32rtc_get_week_day(STM32RTC *rtc)
{
	stm32rtc_sync_date(rtc);
	return rtc->wday;
}

u8
stm32rtc_get_day(STM32RTC *rtc)
{
	stm32rtc_sync_date(rtc);
	return rtc->day;
}

u8
stm32rtc_get_month(STM32RTC *rtc)
{
	stm32rtc_sync_date(rtc);
	return rtc->month;
}

u8
stm32rtc_get_year(STM32RTC *rtc)
{
	stm32rtc_sync_date(rtc);
	return rtc->year;
}

void
stm32rtc_get_date
(
	STM32RTC *rtc,
	u8 *wday,
	u8 *day,
	u8 *month,
	u8 *year
)
{
	stm32rtc_sync_date(rtc);
	if (wday != NULL)
		*wday = rtc->wday;
	if (day != NULL)
		*day = rtc->day;
	if (month != NULL)
		*month = rtc->month;
	if (year != NULL)
		*year = rtc->year;
}




// SET FUNCTIONS

void
stm32rtc_set_sub_seconds
(
	STM32RTC *rtc,
	ul32 sub_seconds
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (sub_seconds < 1000)
			rtc->sub_seconds = sub_seconds;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_seconds
(
	STM32RTC *rtc,
	u8 seconds
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (seconds < 60)
			rtc->seconds = seconds;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_minutes
(
	STM32RTC *rtc,
	u8 minutes
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (minutes < 60)
			rtc->minutes = minutes;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_hours
(
	STM32RTC *rtc,
	u8 hours
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (hours < 24)
			rtc->hours = hours;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_hours_12
(
	STM32RTC *rtc,
	u8 hours,
	hourAM_PM_t period
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (hours < 12)
			rtc->hours = hours;
		if (rtc->format == HOUR_FORMAT_12)
			rtc->period = period;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_time
(
	STM32RTC *rtc,
	u8 hours,
	u8 minutes,
	u8 seconds,
	ul32 sub_seconds
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (hours < 24)
			rtc->hours = hours;
		if (minutes < 60)
			rtc->minutes = minutes;
		if (seconds < 60)
			rtc->seconds = seconds;
		if (sub_seconds < 1000)
			rtc->sub_seconds = sub_seconds;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}
}

void
stm32rtc_set_time_12
(
	STM32RTC *rtc,
	u8 hours,
	u8 minutes,
	u8 seconds,
	ul32 sub_seconds,
	hourAM_PM_t period
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_time(rtc);
		if (hours < 24)
			rtc->hours = hours;
		if (minutes < 60)
			rtc->minutes = minutes;
		if (seconds < 60)
			rtc->seconds = seconds;
		if (sub_seconds < 1000)
			rtc->sub_seconds = sub_seconds;
		if (rtc->format == HOUR_FORMAT_12)
			rtc->period = period;
		RTC_SetTime
		(
			rtc->hours,
			rtc->minutes,
			rtc->seconds,
			rtc->sub_seconds,
			rtc->period
		);
	}

}

void
stm32rtc_set_week_day
(
	STM32RTC *rtc,
	u8 wday
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (wday >= 1 && wday <= 7)
			rtc->wday = wday;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}

void
stm32rtc_set_day
(
	STM32RTC *rtc,
	u8 day
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (day >= 1 && day <= 31)
			rtc->day = day;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}

void
stm32rtc_set_month
(
	STM32RTC *rtc,
	u8 month
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (month >= 1 && month <= 12)
			rtc->month = month;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}

void
stm32rtc_set_year
(
	STM32RTC *rtc,
	u8 year
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (year < 100)
			rtc->year = year;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}

void
stm32rtc_set_date
(
	STM32RTC *rtc,
	u8 day,
	u8 month,
	u8 year
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (day >= 1 && day <= 31)
			rtc->day = day;
		if (month >= 1 && month <= 12)
			rtc->month = month;
		if (year < 100)
			rtc->year = year;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}

void
stm32rtc_set_wdate
(
	STM32RTC *rtc,
	u8 day,
	u8 month,
	u8 year,
	u8 wday
)
{
	if (rtc->_configured)
	{
		stm32rtc_sync_date(rtc);
		if (wday >= 1 && wday <= 7)
			rtc->wday = wday;
		if (day >= 1 && day <= 31)
			rtc->day = day;
		if (month >= 1 && month <= 12)
			rtc->month = month;
		if (year < 100)
			rtc->year = year;
		RTC_SetDate
		(
			rtc->year,
			rtc->month,
			rtc->day,
			rtc->wday
		);
	}
}


















void
stm32rtc_sync_time(STM32RTC *rtc)
{
	if (rtc->_configured)
	{
		RTC_GetTime
		(
			&(rtc->hours),
			&(rtc->minutes),
			&(rtc->seconds),
			&(rtc->sub_seconds),
			&(rtc->period)
		);
	}
}

void
stm32rtc_sync_date(STM32RTC *rtc)
{
	if (rtc->_configured)
	{
		RTC_GetDate
		(
			&(rtc->year),
			&(rtc->month),
			&(rtc->day),
			&(rtc->wday)
		);
	}
}

void
stm32rtc_sync_alarm_time(STM32RTC *rtc)
{
	if (rtc->_configured)
	{
		u8 mask;
		RTC_GetAlarm
		(
			&(rtc->alarm_day),
			&(rtc->alarm_hours),
			&(rtc->alarm_minutes),
			&(rtc->alarm_seconds),
			&(rtc->alarm_sub_seconds),
			&(rtc->alarm_period),
			&(rtc->alarm_mask)
		);
	}
}
