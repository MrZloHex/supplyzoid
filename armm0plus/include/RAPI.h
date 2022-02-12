#ifndef RAPI_H_
#define RAPI_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"
#include "OCPPTypes.h"

#include "mjson.h"
#include "usart_rapi.h"

// #define NULL ((void *)0)

#define RAPI_BUF_LEN 32
#define RAPI_SOC '$'
#define RAPI_EOC 0xD
#define RAPI_SOSI ':'

#define RAPI_MAX_TOKENS 10

// EVSE states for m_EvseState
#define EVSE_STATE_UNKNOWN 0x00
#define EVSE_STATE_A       0x01 // vehicle state A 12V - not connected
#define EVSE_STATE_B       0x02 // vehicle state B 9V - connected, ready
#define EVSE_STATE_C       0x03 // vehicle state C 6V - charging
#define EVSE_STATE_D       0x04 // vehicle state D 3V - vent required
#define EVSE_FAULT_STATE_BEGIN EVSE_STATE_D
#define EVSE_STATE_DIODE_CHK_FAILED 0x05 // diode check failed
#define EVSE_STATE_GFCI_FAULT 0x06       // GFCI fault
#define EVSE_STATE_NO_GROUND 0x07 //bad ground
#define EVSE_STATE_STUCK_RELAY 0x08 //stuck relay
#define EVSE_STATE_GFI_TEST_FAILED 0x09 // GFI self-test failure
#define EVSE_STATE_OVER_TEMPERATURE 0x0A // over temperature error shutdown
#define EVSE_STATE_OVER_CURRENT 0x0B // over current error shutdown
//reserved #define EVSE_STATE_PILOT_ERROR 0x0C // pilot self test error
//reserved #define EVSE_STATE_TEMP_SENSOR_FAULT 0x0D // temp sensor dead
#define EVSE_STATE_RELAY_CLOSURE_FAULT 0x0E
#define EVSE_FAULT_STATE_END EVSE_STATE_RELAY_CLOSURE_FAULT
           
#define EVSE_STATE_SLEEPING 0xfe // waiting for timer
#define EVSE_STATE_DISABLED 0xff // disabled

// Auth lock state
#define AUTH_UNLOCKED 0
#define AUTH_LOCKED 1

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
