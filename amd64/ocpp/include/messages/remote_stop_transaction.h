#ifndef REMOTE_STOP_TRANSACTION_H_
#define REMOTE_STOP_TRANSACTION_H_

#include "OCPP.h"

#include <stdio.h>

#define P_TRANSACTION_ID "$.transactionId"

void
ocpp_remote_stop_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
);

void
ocpp_remote_stop_transaction_conf
(
	OCPP *ocpp,
	unsigned char status
);

#endif /* REMOTE_STOP_TRANSACTION_H_ */
