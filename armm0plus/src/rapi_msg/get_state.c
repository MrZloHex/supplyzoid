#include "rapi_msg/get_state.h"

void
rapi_get_state_req(RAPI *rapi)
{
    usart_rapi_println_str("Get state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GS"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_state_resp(RAPI *rapi)
{

}