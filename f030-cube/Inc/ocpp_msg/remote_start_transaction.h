#ifndef REMOTE_START_TRANSACTION_H_
#define REMOTE_START_TRANSACTION_H_

#include "OCPP.h"

#define P_ID_TAG "$.idTag"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi
);

void
ocpp_remote_start_transaction_conf
(
	OCPP *ocpp,
	unsigned char status
);

#endif /* REMOTE_START_TRANSACTION_H_ */
