#ifndef RAPI_H_
#define RAPI_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"
#include "OCPPTypes.h"

#define RAPI_BUF_LEN 32
#define RAPI_SOC '$'
#define RAPI_EOC 0xD
#define RAPI_SOSI ':'

#define RAPI_MAX_TOKENS 10

// EVSE states for m_EvseState
#define EVSE_STATE_UNKNOWN 0x00
#define EVSE_STATE_A 0x01 // vehicle state A 12V - not connected
#define EVSE_STATE_B 0x02 // vehicle state B 9V - connected, ready
#define EVSE_STATE_C 0x03 // vehicle state C 6V - charging
#define EVSE_STATE_D 0x04 // vehicle state D 3V - vent required
#define EVSE_FAULT_STATE_BEGIN EVSE_STATE_D
#define EVSE_STATE_DIODE_CHK_FAILED 0x05 // diode check failed
#define EVSE_STATE_GFCI_FAULT 0x06 // GFCI fault
#define EVSE_STATE_NO_GROUND 0x07 //bad ground
#define EVSE_STATE_STUCK_RELAY 0x08 //stuck relay
#define EVSE_STATE_GFI_TEST_FAILED 0x09 // GFI self-test failure
#define EVSE_STATE_OVER_TEMPERATURE 0x0A // over temperature error shutdown
#define EVSE_STATE_OVER_CURRENT 0x0B // over current error shutdown
#define EVSE_FAULT_STATE_END EVSE_STATE_OVER_CURRENT


typedef struct
{
	char buf_cmd[RAPI_BUF_LEN];
	size buf_index;

	char *tokens[RAPI_MAX_TOKENS];
	size token_index;
} RAPI;

void
rapi_reset(RAPI *rapi);

bool
rapi_update
(
	RAPI *rapi,
	OCPP *ocpp
);

bool
rapi_analyze(RAPI *rapi);

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


void
rapi_app_chksum(RAPI *rapi);



#endif /* RAPI_H_ */
