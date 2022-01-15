#ifndef STOP_TRANSACTION_H_
#define STOP_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_stop_transaction_req
(
    OCPP *ocpp,
    RAPI *rapi
);

void
ocpp_stop_transaction_conf
(
    OCPP *ocpp,
    RAPI *rapi
);

#endif /* STOP_TRANSACTION_H_ */
