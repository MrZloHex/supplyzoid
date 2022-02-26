#ifndef RAPI_H_
#define RAPI_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"
#include "OCPPTypes.h"

#include "mjson.h"
#include "usart_rapi.h"

#include "RAPI_add.h"

// #define NULL ((void *)0)

void
rapi_reset(RAPI *rapi);

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


#endif /* RAPI_H_ */
