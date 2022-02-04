#ifndef OCPP_RESET_H_
#define OCPP_RESET_H_

#include "OCPP.h"

#define P_RESET_TYPE "$.type"

void
ocpp_reset_req
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc
);

void
ocpp_reset_conf
(
	OCPP *ocpp,
	unsigned char status
);

#endif /* OCPP_RESET_H_ */