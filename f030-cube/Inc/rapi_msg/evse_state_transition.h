#ifndef RAPI_EVSE_STATE_TRANSITION_H_
#define RAPI_EVSE_STATE_TRANSITION_H_

#include "controller_rapi.h"

void
_rapi_evse_state_transition_req
(
    Controller_RAPI *rapi,
    uint8_t *evse_state,
    uint8_t *pilot_state
);

void
_rapi_evse_state_transition_resp(Controller_RAPI *rapi);

#endif /* RAPI_EVSE_STATE_TRANSITION_H_ */