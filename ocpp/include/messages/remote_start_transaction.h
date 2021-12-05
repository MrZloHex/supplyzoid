#ifndef REMOTE_START_TRANSACTION_H_
#define REMOTE_START_TRANSACTION_H_

#include "stdtypes.h"
#include "stringg.h"
#include "EVSE.h"
#include "OCPP.h"

#include <stdio.h>

#define P_ID_TAG "$.idTag"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
);

void
ocpp_remote_start_transaction_conf
(
	OCPP *ocpp,
	unsigned char status
);

#endif /* REMOTE_START_TRANSACTION_H_ */
