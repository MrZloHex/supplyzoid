#ifndef RAPI_H_
#define RAPI_H_

// #include "stdtypes.h"
#include "string.h"
#include "convert.h"

#include "usart.h"

#include <OCPP/OCPP_Types.h>
#include "RAPI/RAPI_Types.h"

// #define NULL ((void *)0)

void
rapi_init
(
	RAPI *rapi,
	UART_HandleTypeDef *uart
);

void
rapi_reset(RAPI *rapi);

void
rapi_deinit(RAPI *rapi);


bool
rapi_update
(
	RAPI *rapi,
	OCPP *ocpp
);

void
rapi_process_cmd
(
	RAPI *rapi,
	OCPP *ocpp
);

void
rapi_send_req(RAPI *rapi);

bool
rapi_get_resp
(
	RAPI *rapi,
	OCPP *ocpp
);

bool
rapi_is_msg_correct(RAPI *rapi);

void
rapi_append_chksum(RAPI *rapi);

#endif /* RAPI_H_ */
