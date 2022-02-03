#ifndef STOP_TRANSACTION_H_
#define STOP_TRANSACTION_H_

#include "OCPP.h"

void
ocpp_stop_transaction_req
(
    OCPP *ocpp,
    RAPI *rapi,
    STM32RTC *rtc
);

void
ocpp_stop_transaction_conf
(
    OCPP *ocpp
);

#endif /* STOP_TRANSACTION_H_ */
