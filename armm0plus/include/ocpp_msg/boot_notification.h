#ifndef BOOT_NOTIFICATION_H_

#define BOOT_NOTIFICATION_H_

#include "RTC.h"
#include "usart_ocpp.h"

#include "stdtypes.h"
#include "OCPP.h"

#define P_INTERVAL     "$.interval"
#define P_STATUS       "$.status"
#define P_CURRENT_TIME "$.currentTime"

void
ocpp_boot_notification_req
(
	OCPP *ocpp
);

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc
);

#endif /* BOOT_NOTIFICATION_H_ */
