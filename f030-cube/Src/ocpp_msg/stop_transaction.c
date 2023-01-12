#include "ocpp_msg/stop_transaction.h"

#include "mjson.h"
#include "serial.h"
#include "time.h"

void
ocpp_stop_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
)
{
	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%ld,%Q:%Q,%Q:%d}",
		"meterStop",
		*wh,
		"timestamp",
		time,
		"transactionId",
		ocpp->transaction_id
	);

	
	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_STOP_TRANSACTION;
	strcpy(ocpp->message.data.call.payload, payload);
}

// void
// ocpp_stop_transaction_conf
// (
// 	OCPP *ocpp
// )
// {
// 	if (strcmp(ocpp->last_msg.ID, ocpp->pres_msg.ID) != 0)
// 		return;

// 	ocpp->_wait_resp = false;

// 	if (ocpp->pres_msg.type == CALLERROR)
// 	{
// 		return;  // TODO: add handling CALLERRROR
// 	}

// 	// AVAILABLE
// 	// evse_change_state(evse, ocpp, S_AVAILABLE);
// 	size_t pay_len = strlen(ocpp->pres_msg.call_result.payload);

// 	char status[10];
// 	int res_st = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
// 	if (res_st < 1)
// 		return;

// 	if (strcmp("Accepted", status) == 0)
// 		uprintf(ocpp->uart, 1000, 64, "TRANSACTION FINISHED\n");
// 	else if (strcmp("Rejected", status) == 0)
// 		uprintf(ocpp->uart, 1000, 64, "FAILED TO FINISH TRANSACTION\n");
// 	else
// 		return;

// }

