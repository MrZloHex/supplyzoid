#include "rapi_msg/evse_state_transition.h"

#include "convert.h"

void
_rapi_evse_state_transition_req
(
	Controller_RAPI *rapi,
	uint8_t *evse_state,
	uint8_t *pilot_state
)
{
	if (rapi->token_index < 5)
		return;

	if (evse_state != NULL)
	   hex_str_to_uint8(evse_state, rapi->tokens[1]);
	if (pilot_state != NULL)
		hex_str_to_uint8(pilot_state, rapi->tokens[2]);
}

void
_rapi_evse_state_transition_resp(Controller_RAPI *rapi)
{

}
