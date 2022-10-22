#include "ocpp_msg/start_transaction.h"
#include "rapi_msg/get_energy_usage.h"

#include "mjson.h"
#include "serial.h"
#include "time.h"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi,
	char id_tag[21]
)
{
	rapi_get_energy_usage_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	uint32_t ws;
	rapi_get_energy_usage_resp(rapi, &ws, NULL);
	uint32_t wh = ws / 3600;

	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%u,%Q:%Q,%Q:%lu,%Q:%Q}",
		"connectorId",
		1,
		"idTag",
		id_tag,
		"meterStart",
		wh,
		"timestamp",
		time
	);

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = ACT_START_TRANSACTION;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_start_transaction_conf
(
	OCPP *ocpp
)
{
	if (strcmp(ocpp->pres_msg.ID, ocpp->last_msg.ID) != 0)
		return;

	ocpp->_wait_resp = false;

	if (ocpp->pres_msg.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size_t pay_len = strlen(ocpp->pres_msg.call_result.payload);

	double transaction_id;
	int res_int = mjson_get_number(ocpp->pres_msg.call_result.payload, pay_len, P_TRANSACTION_ID, &transaction_id);
	if (res_int == 0)
		return;

	char status[10];
	int res_st = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return;

	if (strcmp("Accepted", status) == 0)
	{
		ocpp->_transaction_id = (int)transaction_id;
		// uprintf(ocpp->uart, 1000, 64, "TRANSACTION CONFIRMED\n");
	}
	else if (strcmp("Rejected", status) == 0)
	{
		// uprintf(ocpp->uart, 1000, 64, "TRANSACTION REJECTED\n");
	}
	else
	{
		return;
	}

		
}
