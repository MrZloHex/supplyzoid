#ifndef RAPI_GET_ENERGY_USAGE_H_
#define RAPI_GET_ENERGY_USAGE_H_

#include "controller_rapi.h"

void
_rapi_get_energy_usage_req(Controller_RAPI *rapi);

void
_rapi_get_energy_usage_resp
(
    Controller_RAPI *rapi,
    uint32_t *ws,
    uint32_t *whacc
);

#endif /* RAPI_GET_ENERGY_USAGE_H_ */