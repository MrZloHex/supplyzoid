#ifndef RAPI_EVSE_STATE_TRANSITION_H_
#define RAPI_EVSE_STATE_TRANSITION_H_

#include "RAPI.h"

void
rapi_evse_state_transition_req
(
    RAPI *rapi,
    OCPP *ocpp
);

void
rapi_evse_state_transition_resp(RAPI *rapi);

#endif /* RAPI_EVSE_STATE_TRANSITION_H_ */