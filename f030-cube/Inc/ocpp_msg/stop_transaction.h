#ifndef STOP_TRANSACTION_H_
#define STOP_TRANSACTION_H_

#define P_ID_INFO_STATUS "$.idTagInfo.status"

#include "controller_ocpp.h"

void
ocpp_stop_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
);

void
ocpp_stop_transaction_conf
(
    Controller_OCPP *ocpp
);

#endif /* STOP_TRANSACTION_H_ */
