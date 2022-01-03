#ifndef STOP_TRANSACTION_H_
#define STOP_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_stop_transaction_req
(
    OCPP *ocpp,
    EVSE *evse
);

void
ocpp_stop_transaction_conf
(
    OCPP *ocpp,
    EVSE *evse
);

#endif /* STOP_TRANSACTION_H_ */
