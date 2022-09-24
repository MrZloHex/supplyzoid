#ifndef STOP_TRANSACTION_H_
#define STOP_TRANSACTION_H_

#define P_ID_INFO_STATUS "$.idTagInfo.status"

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
    OCPP *ocpp
);

#endif /* STOP_TRANSACTION_H_ */
