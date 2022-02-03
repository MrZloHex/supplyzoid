#ifndef START_TRANSACTION_H_
#define START_TRANSACTION_H_

#include "OCPP.h"

#define P_TRANSACTION_ID "$.transactionId"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc,
	char id_tag[21]
);

void
ocpp_start_transaction_conf
(
	OCPP *ocpp,
	RAPI *rapi
);

#endif /* START_TRANSACTION_H_ */
