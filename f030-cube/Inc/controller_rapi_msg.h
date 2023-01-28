/**
  ******************************************************************************
  * @file    controller_rapi_msg.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_RAPI_MESSAGES_H__
#define __CONTROLLER_RAPI_MESSAGES_H__

#include "controller_rapi.h"

void
_rapi_evse_state_transition_req
(
    Controller_RAPI *rapi,
    uint8_t *evse_state,
    uint8_t *pilot_state
);

void
_rapi_set_auth_lock_req
(
	Controller_RAPI *rapi,
	uint8_t lock_state
);

void
_rapi_reset_req(Controller_RAPI *rapi);

void
_rapi_get_energy_usage_req(Controller_RAPI *rapi);

void
_rapi_get_energy_usage_resp
(
    Controller_RAPI *rapi,
    uint32_t *ws,
    uint32_t *whacc
);

void
_rapi_get_state_req(Controller_RAPI *rapi);

void
_rapi_get_state_resp
(
	Controller_RAPI *rapi,
	uint8_t *evse_state,
	uint32_t *elapsed_time,
	uint8_t *pilot_state,
	uint8_t * vflags
);


#endif /* __CONTROLLER_RAPI_MESSAGES_H__ */
