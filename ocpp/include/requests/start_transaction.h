#ifndef START_TRANSACTION_H_
#define START_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse,
	char id_tag[21]
);

void
ocpp_start_transaction_conf
(
	OCPP *ocpp
);

#endif /* START_TRANSACTION_H_ */
