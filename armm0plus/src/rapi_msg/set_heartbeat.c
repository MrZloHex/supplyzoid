#include "rapi_msg/set_heartbeat.h"

void
rapi_set_heartbeat_req
(
    RAPI *rapi,
    my_time time
)
{
    char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %lu",
		"SY",
        time
	);

	strcpyy(rapi->buf_cmd, payload);
    rapi_app_chksum(rapi);
}

void
rapi_set_heartbeat_resp(RAPI *rapi)
{

}