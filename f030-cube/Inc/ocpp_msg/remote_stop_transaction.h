#ifndef REMOTE_STOP_TRANSACTION_H_
#define REMOTE_STOP_TRANSACTION_H_

#include "controller_ocpp.h"


// void
// ocpp_remote_stop_transaction_req
// (
// 	OCPP *ocpp,
// 	RAPI *rapi
// );

void
ocpp_remote_stop_transaction_conf
(
	Controller_OCPP *ocpp,
	bool *status
);

#endif /* REMOTE_STOP_TRANSACTION_H_ */
