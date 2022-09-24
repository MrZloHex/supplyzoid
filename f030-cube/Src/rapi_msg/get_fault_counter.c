#include "rapi_msg/get_fault_counter.h"

#include "mjson.h"

void
rapi_get_fault_counter_req(RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GF"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_fault_counter_resp
(
	RAPI *rapi,
	uint8_t *gfitripcnt,
	uint8_t *nogndtripcnt,
	uint8_t *stuckrelaytripcnt
)
{
	if (rapi->token_index < 4)
		return;

	if (gfitripcnt != NULL)
		hex_str_to_uint8(gfitripcnt, rapi->tokens[1]);
	if (nogndtripcnt != NULL)
		hex_str_to_uint8(nogndtripcnt, rapi->tokens[2]);
	if (stuckrelaytripcnt != NULL)
		hex_str_to_uint8(stuckrelaytripcnt, rapi->tokens[3]);
}

