#include "rapi_msg/evse_state_transition.h"
#include "ocpp_msg/status_notification.h"

void
rapi_evse_state_transition_req
(
    RAPI *rapi,
    OCPP *ocpp
)
{
    if (rapi->token_index < 5)
        return;

    uint8_t evsestate;
    uint8_t pilotstate;
    hex_str_to_uint8(&evsestate, rapi->tokens[1]);
    hex_str_to_uint8(&pilotstate, rapi->tokens[2]);

    OCPPChargePointStatus state;
    OCPPChargePointErrorCode error;
    if (evsestate == EVSE_STATE_UNKNOWN)
    {
        state = Unavailable;
        error = NoError;
    }
    else if (evsestate < EVSE_FAULT_STATE_BEGIN)
    {
        state = evsestate -1;
        // TODO Implement indicate all errors
        error = NoError;
    }
    else
    {
        error = OtherError;
        state = Faulted;
    }

    ocpp_status_notification_req(ocpp, rapi, state, error);
    ocpp_send_req(ocpp, ACT_STATUS_NOTIFICATION);
    // usart_rapi_print_str("Evse State Transition\r");
}

void
rapi_evse_state_transition_resp(RAPI *rapi)
{

}
