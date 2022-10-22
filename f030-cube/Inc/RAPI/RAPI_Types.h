#ifndef RAPI_RAPI_TYPES_H_
#define RAPI_RAPI_TYPES_H_

#include "stddef.h"
#include "stdbool.h"
#include "usart.h"

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
	UART_HandleTypeDef *uart;
	bool got_msg;
	bool proc_msg;
	char *buffer;
	size_t buf_i;

	char  *buf_cmd;
	size_t buf_index;

	char **tokens;
	size_t token_index;
} RAPI;

#endif /* RAPI_RAPI_TYPES_H_ */