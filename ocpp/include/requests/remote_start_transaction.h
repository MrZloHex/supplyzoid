#ifndef REMOTE_START_TRANSACTION_H_
#define REMOTE_START_TRANSACTION_H_

#include "stdtypes.h"
#include "EVSE.h"
#include "OCPP.h"

#include "requests/start_transaction.h"

#include <stdio.h>

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
);

void
ocpp_remote_start_transaction_conf
(
	OCPP *ocpp
);

#endif /* REMOTE_START_TRANSACTION_H_ */