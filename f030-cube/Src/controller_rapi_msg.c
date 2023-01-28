#include "controller_rapi_msg.h"

#include "mjson.h"
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
_rapi_get_energy_usage_req(Controller_RAPI *rapi)
{
    // usart_rapi_println_str("Get energy state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GU"
	);

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}

void
_rapi_get_energy_usage_resp
(
    Controller_RAPI *rapi,
    uint32_t *ws,
    uint32_t *whacc
)
{
    if (rapi->token_index < 3)
		return;

	if (ws != NULL)
		charset_to_uint32(ws, rapi->tokens[1]);
	if (whacc != NULL)
		charset_to_uint32(whacc, rapi->tokens[2]);
}

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

void
_rapi_reset_req(Controller_RAPI *rapi)
{
	// usart_rapi_println_str("Reset");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"FR"
	);

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}

void
_rapi_set_auth_lock_req
(
	Controller_RAPI *rapi,
	uint8_t lock_state
)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %u^",
		"S4",
		lock_state
	);

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}