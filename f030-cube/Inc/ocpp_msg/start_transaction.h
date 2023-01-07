#ifndef START_TRANSACTION_H_
#define START_TRANSACTION_H_

#include "controller_ocpp.h"

#define P_TRANSACTION_ID "$.transactionId"
#define P_ID_INFO_STATUS "$.idTagInfo.status"

void
ocpp_start_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
);

void
ocpp_start_transaction_conf
(
	Controller_OCPP *ocpp
);

#endif /* START_TRANSACTION_H_ */
