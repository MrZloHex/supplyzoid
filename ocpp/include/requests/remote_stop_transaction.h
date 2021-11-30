#ifndef REMOTE_STOP_TRANSACTION_H_
#define REMOTE_STOP_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_remote_stop_transaction_req
(
    OCPP *ocpp
);

void
ocpp_remote_stop_transaction_conf
(
    OCPP *ocpp
);

#endif /* REMOTE_STOP_TRANSACTION_H_ */