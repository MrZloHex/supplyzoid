#include "rapi_msg/evse_state_transition.h"

void
rapi_evse_state_transition_req
(
    RAPI *rapi,
    OCPP *ocpp
)
{
    usart_rapi_println_str("Evse State Transition");
}

void
rapi_evse_state_transition_resp(RAPI *rapi)
{

}
