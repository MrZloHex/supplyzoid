#ifndef RAPI_GET_ENERGY_USAGE_H_
#define RAPI_GET_ENERGY_USAGE_H_

#include "RAPI.h"

void
rapi_get_energy_usage_req(RAPI *rapi);

void
rapi_get_energy_usage_resp
(
    RAPI *rapi,
    uint32_t *ws,
    uint32_t *whacc
);

#endif /* RAPI_GET_ENERGY_USAGE_H_ */