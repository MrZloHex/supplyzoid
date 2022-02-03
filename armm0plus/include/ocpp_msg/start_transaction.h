#ifndef START_TRANSACTION_H_
#define START_TRANSACTION_H_

#include "OCPP.h"

#define P_TRANSACTION_ID "$.transactionId"
#define P_ID_INFO_STATUS "$.idTagInfo.status"

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
	OCPP *ocpp
);

#endif /* START_TRANSACTION_H_ */
