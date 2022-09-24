#include "OCPP.h"

#include "stdlib.h"
#include "mjson.h"
#include "serial.h"
#include "convert.h"


#include "ocpp_msg/boot_notification.h"
#include "ocpp_msg/remote_start_transaction.h"
#include "ocpp_msg/start_transaction.h"
#include "ocpp_msg/remote_stop_transaction.h"
#include "ocpp_msg/stop_transaction.h"
#include "ocpp_msg/meter_values.h"
#include "ocpp_msg/heartbeat.h"
#include "ocpp_msg/data_transfer.h"
#include "ocpp_msg/reset.h"


void
ocpp_init
(
	OCPP *ocpp,
	UART_HandleTypeDef *uart,
	RTC_HandleTypeDef *rtc
)
{
	ocpp->uart = uart;
	ocpp->rtc = rtc;

	ocpp->_transaction_id = 0;
	ocpp->_id = 1;
	ocpp->_wait_resp = false;
	ocpp->_booted = false;

	ocpp->pres_msg.call.payload 			= (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->pres_msg.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->pres_msg.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN   );
	ocpp->pres_msg.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->pres_msg.ID					    = (char *) malloc(sizeof(char)*ID_LEN     );

	ocpp->last_msg.call.payload			    = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->last_msg.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->last_msg.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN   );
	ocpp->last_msg.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->last_msg.ID					    = (char *) malloc(sizeof(char)*ID_LEN     );
}

void
ocpp_deinit(OCPP *ocpp)
{
	free(ocpp->pres_msg.call.payload);
	free(ocpp->pres_msg.call_result.payload);
	free(ocpp->pres_msg.call_error.error_dscr);
	free(ocpp->pres_msg.call_error.error_details);
	free(ocpp->pres_msg.ID);

	free(ocpp->last_msg.call.payload);
	free(ocpp->last_msg.call_result.payload);
	free(ocpp->last_msg.call_error.error_dscr);
	free(ocpp->last_msg.call_error.error_details);
	free(ocpp->last_msg.ID);
}

void
ocpp_next(OCPP *ocpp)
{
	ocpp->last_msg.type = ocpp->pres_msg.type;
	strcpy(ocpp->last_msg.ID, ocpp->pres_msg.ID);
	ocpp->last_msg.call.action = ocpp->pres_msg.call.action;
	strcpy(ocpp->last_msg.call.payload, ocpp->pres_msg.call.payload);
	strcpy(ocpp->last_msg.call_result.payload, ocpp->pres_msg.call_result.payload);
	ocpp->last_msg.call_error.error_code = ocpp->pres_msg.call_error.error_code;
	strcpy(ocpp->last_msg.call_error.error_dscr, ocpp->pres_msg.call_error.error_dscr);
	strcpy(ocpp->last_msg.call_error.error_details, ocpp->pres_msg.call_error.error_details);
}	

void
ocpp_update
(
	OCPP *ocpp,
	RAPI *rapi
)
{
		char buffer[OCPP_BUF_LEN] = {0};

	USART_Result state = uread(ocpp->uart, 100, buffer, OCPP_BUF_LEN, '\n');
	
	// if (state != USART_IO_TIMEOUT) uprintf(ocpp->uart, 100, 6, "S %d\n", state);

	if (state == USART_IO_OK) uprintf(ocpp->uart, 1000, OCPP_BUF_LEN+10, "`%s`\n", buffer);

	if (state == USART_IO_ERROR) Error_Handler();
	// uint8_t ch = 0;
	// HAL_StatusTypeDef state = HAL_UART_Receive(ocpp->uart, &ch, 1, 100);
	// if (state == HAL_OK)
	// {
	// // 	if (ch != '\n')
	// // 	{
	// 		// ocpp->buffer[(ocpp->buf_i)++] = ch;
	// uprintf(ocpp->uart, 1000, 6, "`%c`\n", ch);
	// // 	}

	// // 	if (ocpp->buf_i >= OCPP_BUF_LEN)
	// // 	{
	// // 		memset(ocpp->buffer, 0, ocpp->buf_i);
	// // 		ocpp->buf_i = 0;
	// // 		return;
	// // 	}

	// // 	if (mjson(ocpp->buffer, ocpp->buf_i, NULL, NULL) > 0)
	// // 	{
	// // 		ocpp->buffer[ocpp->buf_i] = '\0';

	// // 		ocpp_handle_message(ocpp, rapi);

	// // 		memset(ocpp->buffer, 0, ocpp->buf_i);
	// // 		ocpp->buf_i = 0;
	// // 	}
	// }
	// else if (state != HAL_TIMEOUT)
	// {
	// 	uprintf(ocpp->uart, 100, 6, "S %d\n", state);
	// }
}

void
ocpp_handle_message
(
 	OCPP *ocpp,
	RAPI *rapi
)
{
	uprintf(ocpp->uart, 1000, 556, "HANDLE NEW MESSAGE: %s\n", ocpp->buffer);
	

	OCPPResult res_parse = ocpp_parse_message(ocpp);
	if (res_parse == ERROR_P)
		return;


	if (!ocpp->_booted)
	{
		if (ocpp->_wait_resp)
			ocpp_boot_notification_conf(ocpp, rapi);
	}
	else
	{
		if (ocpp->_wait_resp)
		{
			if (ocpp->last_msg.call.action == START_TRANSACTION)
				ocpp_start_transaction_conf(ocpp);
			else if (ocpp->last_msg.call.action == STOP_TRANSACTION)
				ocpp_stop_transaction_conf(ocpp);
			else if (ocpp->last_msg.call.action == METER_VALUES)
				ocpp_meter_values_conf(ocpp);
			else if (ocpp->last_msg.call.action == HEARTBEAT)
				ocpp_heartbeat_conf(ocpp);
			else if (ocpp->last_msg.call.action == DATA_TRANSFER)
				ocpp_data_transfer_conf(ocpp);
			// else
				// printf("NOT IMPLEMETED\n");
		}
		else
		{
			ocpp_next(ocpp);
			if (ocpp->last_msg.call.action == REMOTE_START_TRANSACTION)
				ocpp_remote_start_transaction_req(ocpp, rapi);
			else if (ocpp->last_msg.call.action == REMOTE_STOP_TRANSACTION)
				ocpp_remote_stop_transaction_req(ocpp, rapi);
			else if (ocpp->last_msg.call.action == RESET)
				ocpp_reset_req(ocpp, rapi);
			// else
				// printf("NOT IMPLEMETED\n");
		}
	}
}

OCPPResult
ocpp_parse_message(OCPP *ocpp)
{
	OCPPMessageType msg_type = ocpp_determine_message_type(ocpp);
	if (msg_type == ERROR_P)
		return ERROR_P;

	ocpp->pres_msg.type = msg_type;

	OCPPMessageID msg_id = ocpp_get_message_id(ocpp);
	if (msg_id == ERROR_P)
		return ERROR_P;
	strcpy(ocpp->pres_msg.ID, msg_id);
	free(msg_id);

	if (msg_type == CALL)
	{
		OCPPCallAction action = ocpp_get_action(ocpp);
		if (action == 0)
			return ERROR_P;

		ocpp->pres_msg.call.action = action;

		char payload[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(ocpp, CALL, payload);
		if (res == ERROR_P)
			return ERROR_P;

		strcpy(ocpp->pres_msg.call.payload, payload);

	}
	else if (msg_type == CALLRESULT)
	{
		char payload[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(ocpp, CALLRESULT, payload);
		if (res == ERROR_P)
			return ERROR_P;

		strcpy(ocpp->pres_msg.call_result.payload, payload);

		// printf("NEW CALL RESULT REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
		// printf("\tPAYLOAD: `%s`\n", ocpp->pres_msg.call_result.payload);
	}
	else if (msg_type == CALLERROR)
	{
		OCPPCallErrorCode err_code = ocpp_get_call_error_code(ocpp);
		ocpp->pres_msg.call_error.error_code = err_code;

		char description[DSCR_LEN];
		OCPPResult res_d = ocpp_get_call_error_descr(ocpp, description);
		if (res_d == ERROR_P)
			return ERROR_P;

		strcpy(ocpp->pres_msg.call_error.error_dscr, description);

		char details[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(ocpp, CALLERROR, details);
		if (res == ERROR_P)
			return ERROR_P;
		strcpy(ocpp->pres_msg.call_error.error_details, details);

		// printf("NEW CALL ERROR REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
		// printf("\tERROR CODE: `%d`\n", ocpp->pres_msg.call_error.error_code);
		// printf("\tERROR DESCRIPTION: `%s`\n", ocpp->pres_msg.call_error.error_dscr);
		// printf("\tERROR DETAILS: `%s`\n", ocpp->pres_msg.call_error.error_details);
	}

	return !ERROR_P;
}

void
ocpp_send_req
(
	OCPP *ocpp,
	OCPPCallAction action
)
{
	char action_str[ACTION_LEN];
	if (action == BOOT_NOTIFICATION)
		strcpy(action_str, "BootNotification");
	else if (action == START_TRANSACTION)
		strcpy(action_str, "StartTransaction");
	else if (action == STOP_TRANSACTION)
		strcpy(action_str, "StopTransaction");
	else if (action == STATUS_NOTIFICATION)
		strcpy(action_str, "StatusNotification");
	else if (action == METER_VALUES)
		strcpy(action_str, "MeterValues");
	else if (action == HEARTBEAT)
		strcpy(action_str, "HeartBeat");
	else if (action == DATA_TRANSFER)
		strcpy(action_str, "DataTransfer");
	else if (action == RESET)
		strcpy(action_str, "Reset");
	else
	{
		// printf("NO SUCH REQUEST AVAILABLE\n");
		return;
	}

	ocpp_set_msg_id(ocpp);

	char req[REQ_LEN];

	mjson_snprintf
	(
		req, REQ_LEN,
		"[%u,%Q,%Q,%s]",
		CALL,
		ocpp->pres_msg.ID,
		action_str,
		ocpp->pres_msg.call.payload
	);

	// SENDING
	uprintf(ocpp->uart, 1000, REQ_LEN, "%s\n", req);
	// SENDING

	ocpp->_id++;
	if (action != STATUS_NOTIFICATION)
		ocpp->_wait_resp = true;

	ocpp_next(ocpp);
}

void
ocpp_send_resp
(
	OCPP *ocpp,
	OCPPMessageType type
)
{
	if (type == CALLRESULT)
	{
		char req[REQ_LEN];
		mjson_snprintf
		(
			req, REQ_LEN,
			"[%u,%Q,%s]",
			ocpp->pres_msg.type,
			ocpp->pres_msg.ID,
			ocpp->pres_msg.call_result.payload
		);


		// SENDING
		uprintf(ocpp->uart, 1000, REQ_LEN, "%s\n", req);
		// SENDING
	}
	else if (type == CALLERROR)
	{

	}
	else
	{
		return;
	}
}







// /*
// */






OCPPMessageType
ocpp_determine_message_type(OCPP *ocpp)
{
	double type;
	int res = mjson_get_number(ocpp->buffer, ocpp->buf_i, POS_MSG_TYPE, &type);
	if (res <= 0)
		return ERROR_P;
	switch ((int)type) {
        case CALL:
            return CALL;
        case CALLRESULT:
            return CALLRESULT;
        case CALLERROR:
			return CALLERROR;
        default:
            return ERROR_P;
    }
}


OCPPMessageID
ocpp_get_message_id(OCPP *ocpp)
{
	char buf[50];
	int res = mjson_get_string(ocpp->buffer, ocpp->buf_i, POS_MSG_ID, buf, 50);
	if (res <= 0)
		return ERROR_P;
	size_t id_len = strlen(buf);
	OCPPMessageID id = (char *)malloc(id_len+1);
	strcpy(id, buf);
	id[id_len] = 0;
	return id;
}

OCPPCallAction
ocpp_get_action(OCPP *ocpp)
{
	char buf[ACTION_LEN];
	int res = mjson_get_string(ocpp->buffer, ocpp->buf_i, POS_CL_ACT, buf, ACTION_LEN);
	if (res <= 0)
		return ERROR_P;
	
	if (strcmp(buf, "RemoteStartTransaction"))
		return REMOTE_START_TRANSACTION;
	else if (strcmp(buf, "RemoteStopTransaction"))
		return REMOTE_STOP_TRANSACTION;
	else if (strcmp(buf, "Reset"))
		return RESET;
	else
		return ERROR_P;
}

OCPPResult
ocpp_get_payload
(
	OCPP *ocpp,
	OCPPMessageType type,
	char *dst
)
{
	char path[5];
	if (type == CALL)
		strcpy(path, POS_CL_PAYLOAD);
	else if (type == CALLRESULT)
		strcpy(path, POS_CR_PAYLOAD);
	else if (type == CALLERROR)
		strcpy(path, POS_CE_ERR_DETL);
	else
		return ERROR_P;


	const char *p;
    int n;
	if (mjson_find(ocpp->buffer, ocpp->buf_i, path, &p, &n) != MJSON_TOK_OBJECT)
		return ERROR_P;

	strncpy(dst, p, n);
	return 1;
}

OCPPCallErrorCode
ocpp_get_call_error_code(OCPP *ocpp)
{
	char buf[ERR_CODE_LEN];
	int res = mjson_get_string(ocpp->buffer, ocpp->buf_i, POS_CE_ERR_CODE, buf, ERR_CODE_LEN);
	if (res <= 0)
		return ERROR_P;
	
	if (strcmp(buf, "GenericError"))
		return GENERIC_ERROR;
	else
		return ERROR_P;
}

OCPPResult
ocpp_get_call_error_descr
(
	OCPP *ocpp,
	char *dscr
)
{
	char buf[DSCR_LEN];
	int res = mjson_get_string(ocpp->buffer, ocpp->buf_i, POS_CE_ERR_DSCR, buf, DSCR_LEN);
	if (res <= 0)
		return ERROR_P;

	strcpy(dscr, buf);

	return !ERROR_P;
}


void
ocpp_set_msg_id(OCPP *ocpp)
{
	char id[50];
	int_to_charset(ocpp->_id, id, 1);
	ocpp->_id++;
	strcpy(ocpp->pres_msg.ID, id);
}
