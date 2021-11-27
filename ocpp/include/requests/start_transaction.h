#ifndef START_TRANSACTION_H_
#define START_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse,
	char req[REQ_LEN]
);

#endif /* START_TRANSACTION_H_ */