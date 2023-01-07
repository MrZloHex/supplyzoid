#include "rapi_msg/get_state.h"

#include "mjson.h"
#include "convert.h"

void
_rapi_get_state_req(Controller_RAPI *rapi)
{
	// usart_rapi_println_str("Get state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GS"
	);

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}

void
_rapi_get_state_resp
(
	Controller_RAPI *rapi,
	uint8_t *evse_state,
	uint32_t *elapsed_time,
	uint8_t *pilot_state,
	uint8_t *vflags
)
{
	if (rapi->token_index < 5)
		return;

	if (evse_state != NULL)
		hex_str_to_uint8(evse_state, rapi->tokens[1]);
	if (elapsed_time != NULL)
		charset_to_uint32(elapsed_time, rapi->tokens[2]);
	if (pilot_state != NULL)
		hex_str_to_uint8(pilot_state, rapi->tokens[3]);
	if (vflags != NULL)
		hex_str_to_uint8(vflags, rapi->tokens[4]);
}
