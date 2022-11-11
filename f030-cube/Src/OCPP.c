// #include "OCPP.h"
// 
// #include "stdlib.h"
// #include "mjson.h"
// #include "serial.h"
// #include "string.h"
// #include "convert.h"
// 
// 
// #include "ocpp_msg/boot_notification.h"
// #include "ocpp_msg/remote_start_transaction.h"
// #include "ocpp_msg/start_transaction.h"
// #include "ocpp_msg/remote_stop_transaction.h"
// #include "ocpp_msg/stop_transaction.h"
// #include "ocpp_msg/meter_values.h"
// #include "ocpp_msg/heartbeat.h"
// #include "ocpp_msg/data_transfer.h"
// #include "ocpp_msg/reset.h"
// #include "ocpp_msg/change_availability.h"
// 
// 
// void
// ocpp_init
// (
// 	OCPP *ocpp,
// 	UART_HandleTypeDef *uart,
// 	RTC_HandleTypeDef *rtc
// )
// {
//     memset(ocpp->buffer, 0, OCPP_BUF_LEN);
// 	memset(ocpp->cmd_buf, 0, OCPP_BUF_LEN);
// 	ocpp->buf_i = 0;
// 	ocpp->got_msg = false;
// 	ocpp->proc_msg = true;
// 	ocpp->uart = uart;
// 	ocpp->rtc = rtc;
// 
// 	ocpp->_transaction_id = 0;
// 	ocpp->_id = 1;
// 	ocpp->_wait_resp = false;
// 	ocpp->_is_transaction = false;
// 	ocpp->millis = HAL_GetTick();
// 	ocpp->_booted = false;
// 
// 	ocpp->pres_msg.call.payload 			= (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->pres_msg.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->pres_msg.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN   );
// 	ocpp->pres_msg.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->pres_msg.ID					    = (char *) malloc(sizeof(char)*ID_LEN     );
// 
// 	ocpp->last_msg.call.payload			    = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->last_msg.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->last_msg.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN   );
// 	ocpp->last_msg.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
// 	ocpp->last_msg.ID					    = (char *) malloc(sizeof(char)*ID_LEN     );
// }
// 
// void
// ocpp_deinit(OCPP *ocpp)
// {
// 	free(ocpp->pres_msg.call.payload);
// 	free(ocpp->pres_msg.call_result.payload);
// 	free(ocpp->pres_msg.call_error.error_dscr);
// 	free(ocpp->pres_msg.call_error.error_details);
// 	free(ocpp->pres_msg.ID);
// 
// 	free(ocpp->last_msg.call.payload);
// 	free(ocpp->last_msg.call_result.payload);
// 	free(ocpp->last_msg.call_error.error_dscr);
// 	free(ocpp->last_msg.call_error.error_details);
// 	free(ocpp->last_msg.ID);
// }
// 
// void
// ocpp_next(OCPP *ocpp)
// {
// 	ocpp->last_msg.type = ocpp->pres_msg.type;
// 	strcpy(ocpp->last_msg.ID, ocpp->pres_msg.ID);
// 	ocpp->last_msg.call.action = ocpp->pres_msg.call.action;
// 	strcpy(ocpp->last_msg.call.payload, ocpp->pres_msg.call.payload);
// 	strcpy(ocpp->last_msg.call_result.payload, ocpp->pres_msg.call_result.payload);
// 	ocpp->last_msg.call_error.error_code = ocpp->pres_msg.call_error.error_code;
// 	strcpy(ocpp->last_msg.call_error.error_dscr, ocpp->pres_msg.call_error.error_dscr);
// 	strcpy(ocpp->last_msg.call_error.error_details, ocpp->pres_msg.call_error.error_details);
// }	
// 
// void
// ocpp_update
// (
// 	OCPP *ocpp,
// 	RAPI *rapi
// )
// {
// 	if (ocpp->got_msg && ocpp->proc_msg)
// 	{
// 		strcpy(ocpp->cmd_buf, ocpp->buffer);
// 		ocpp->got_msg = false;
// 		ocpp->proc_msg = false;
// 	}
// 
// 	if (!ocpp->proc_msg)
// 	{
// 		// uprintf(rapi->uart, 10, 1024, "goT `%s`\r", ocpp->cmd_buf);
// 		ocpp_handle_message(ocpp, rapi);
// 		ocpp->proc_msg = true;
// 	}
// 
// 	if (!ocpp->got_msg && HAL_UART_GetState(ocpp->uart) == HAL_UART_STATE_READY)
// 	{
// 		HAL_UART_Receive_IT(ocpp->uart, (uint8_t *)&ocpp->buffer[0], 1);
// 		ocpp->buf_i = 0;
// 	}
// 
// 
// 	// if (ocpp->_is_transaction)
// 	// {
// 	// 	if ((ocpp->millis + 5000) <= HAL_GetTick())
// 	// 	{
// 	// 		ocpp->millis = HAL_GetTick();
// 	// 		ocpp_meter_values_req(ocpp, rapi);
// 	// 		ocpp_send_req(ocpp, ACT_METER_VALUES);
// 	// 	}
// 	// }
// }
// 
// void
// ocpp_handle_message
// (
//  	OCPP *ocpp,
// 	RAPI *rapi
// )
// {
// 	// uprintf(ocpp->uart, 1000, 556, "HANDLE NEW MESSAGE: %s\n", ocpp->buffer);
// 	if (ocpp_parse_message(ocpp) == RES_ERROR)
// 		return;
// 
// 
// 	if (!ocpp->_booted)
// 	{
// 		if (ocpp->_wait_resp)
// 			ocpp_boot_notification_conf(ocpp, rapi);
// 	}
// 	else
// 	{
// 		if (ocpp->_wait_resp)
// 		{
// 			if (ocpp->last_msg.call.action == ACT_START_TRANSACTION)
// 				ocpp_start_transaction_conf(ocpp);
// 			else if (ocpp->last_msg.call.action == ACT_STOP_TRANSACTION)
// 				ocpp_stop_transaction_conf(ocpp);
// 			else if (ocpp->last_msg.call.action == ACT_METER_VALUES)
// 				ocpp_meter_values_conf(ocpp);
// 			else if (ocpp->last_msg.call.action == ACT_HEARTBEAT)
// 				ocpp_heartbeat_conf(ocpp);
// 			else if (ocpp->last_msg.call.action == ACT_DATA_TRANSFER)
// 				ocpp_data_transfer_conf(ocpp);
// 			// else
// 				// printf("NOT IMPLEMETED\n");
// 		}
// 		else
// 		{
// 			ocpp_next(ocpp);
// 			if (ocpp->last_msg.call.action == ACT_REMOTE_START_TRANSACTION)
// 				ocpp_remote_start_transaction_req(ocpp, rapi);
// 			else if (ocpp->last_msg.call.action == ACT_REMOTE_STOP_TRANSACTION)
// 				ocpp_remote_stop_transaction_req(ocpp, rapi);
// 			else if (ocpp->last_msg.call.action == ACT_RESET)
// 				ocpp_reset_req(ocpp, rapi);
// 			else if (ocpp->last_msg.call.action == ACT_CHANGE_AVAILABILITY)
// 				ocpp_change_availability_req(ocpp);
// 			// else
// 				// printf("NOT IMPLEMETED\n");
// 		}
// 	}
// }
// 
// OCPPResult
// ocpp_parse_message(OCPP *ocpp)
// {
// 	if (ocpp_determine_message_type(ocpp) == RES_ERROR) { return RES_ERROR; }
// 	if (ocpp_get_message_id(ocpp) == RES_ERROR) { return RES_ERROR; }
// 
// 	if (ocpp->pres_msg.type == CALL)
// 	{
// 		if (ocpp_get_action(ocpp) == RES_ERROR) { return RES_ERROR; }
// 		if (ocpp_get_payload(ocpp, CALL) == RES_ERROR) { return RES_ERROR; }
// 	}
// 	else if (ocpp->pres_msg.type == CALLRESULT)
// 	{
// 		if (ocpp_get_payload(ocpp, CALLRESULT) == RES_ERROR) { return RES_ERROR; }
// 		// printf("NEW CALL RESULT REQ:\n");
// 		// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
// 		// printf("\tPAYLOAD: `%s`\n", ocpp->pres_msg.call_result.payload);
// 	}
// 	else if (ocpp->pres_msg.type == CALLERROR)
// 	{
// 		if (ocpp_get_call_error_code(ocpp) == RES_ERROR) { return RES_ERROR; }
// 		if (ocpp_get_call_error_descr(ocpp) == RES_ERROR) {return RES_ERROR; }
// 		if (ocpp_get_payload(ocpp, CALLERROR) == RES_ERROR) { return RES_ERROR; }
// 		// printf("NEW CALL ERROR REQ:\n");
// 		// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
// 		// printf("\tERROR CODE: `%d`\n", ocpp->pres_msg.call_error.error_code);
// 		// printf("\tERROR DESCRIPTION: `%s`\n", ocpp->pres_msg.call_error.error_dscr);
// 		// printf("\tERROR DETAILS: `%s`\n", ocpp->pres_msg.call_error.error_details);
// 	}
// 
// 	return RES_OK;
// }
// 
// void
// ocpp_send_req
// (
// 	OCPP *ocpp,
// 	OCPPCallAction action
// )
// {
// 	char action_str[ACTION_LEN];
// 	if (action == ACT_BOOT_NOTIFICATION)
// 		strcpy(action_str, "BootNotification");
// 	else if (action == ACT_START_TRANSACTION)
// 		strcpy(action_str, "StartTransaction");
// 	else if (action == ACT_STOP_TRANSACTION)
// 		strcpy(action_str, "StopTransaction");
// 	else if (action == ACT_STATUS_NOTIFICATION)
// 		strcpy(action_str, "StatusNotification");
// 	else if (action == ACT_METER_VALUES)
// 		strcpy(action_str, "MeterValues");
// 	else if (action == ACT_HEARTBEAT)
// 		strcpy(action_str, "HeartBeat");
// 	else if (action == ACT_DATA_TRANSFER)
// 		strcpy(action_str, "DataTransfer");
// 	else if (action == ACT_RESET)
// 		strcpy(action_str, "Reset");
// 	else if (action == ACT_AUTHORIZE)
// 		strcpy(action_str, "Authorize");
// 	else
// 	{
// 		// printf("NO SUCH REQUEST AVAILABLE\n");
// 		return;
// 	}
// 
// 	ocpp_set_msg_id(ocpp);
// 
// 	char req[REQ_LEN];
// 
// 	mjson_snprintf
// 	(
// 		req, REQ_LEN,
// 		"[%u,%Q,%Q,%s]",
// 		CALL,
// 		ocpp->pres_msg.ID,
// 		action_str,
// 		ocpp->pres_msg.call.payload
// 	);
// 
// 	// SENDING
// 	uprintf(ocpp->uart, 1000, REQ_LEN, "%s\n", req);
// 	// SENDING
// 
// 	ocpp->_id++;
// 	ocpp->_wait_resp = true;
// 
// 	ocpp_next(ocpp);
// }
// 
// void
// ocpp_send_resp
// (
// 	OCPP *ocpp,
// 	OCPPMessageType type
// )
// {
// 	if (type == CALLRESULT)
// 	{
// 		char req[REQ_LEN];
// 		mjson_snprintf
// 		(
// 			req, REQ_LEN,
// 			"[%u,%Q,%s]",
// 			ocpp->pres_msg.type,
// 			ocpp->pres_msg.ID,
// 			ocpp->pres_msg.call_result.payload
// 		);
// 
// 
// 		// SENDING
// 		uprintf(ocpp->uart, 1000, REQ_LEN, "%s\n", req);
// 		// SENDING
// 	}
// 	else if (type == CALLERROR)
// 	{
// 
// 	}
// 	else
// 	{
// 		return;
// 	}
// }
// 
// 
// 
// 
// 
// 
// 
// // /*
// // */
// 
// 
// 
// 
// 
// 
// OCPPResult
// ocpp_determine_message_type(OCPP *ocpp)
// {
// 	double type = 0;
// 	int res = mjson_get_number(ocpp->cmd_buf, strlen(ocpp->cmd_buf), POS_MSG_TYPE, &type);
// 	if (res == 0 || !IS_MSG_TYPE((uint32_t)type))
// 		return RES_ERROR;
// 
// 	ocpp->pres_msg.type = (OCPPMessageType)type;
// 	// uprintf(ocpp->uart, 1000, 10, "type: %u\n", (OCPPMessageType)type);
// 	return RES_OK;
// }
// 
// 
// OCPPResult
// ocpp_get_message_id(OCPP *ocpp)
// {
// 	char buf[ID_LEN] = {0};
// 	int res = mjson_get_string(ocpp->cmd_buf, strlen(ocpp->cmd_buf), POS_MSG_ID, buf, ID_LEN);
// 	if (res <= 0)
// 		return RES_ERROR;
// 
// 	strcpy(ocpp->pres_msg.ID, buf);
// 	// uprintf(ocpp->uart, 1000, 20+ID_LEN, "id: %s\n", buf);
// 	return RES_OK;
// }
// 
// OCPPResult
// ocpp_get_action(OCPP *ocpp)
// {
// 	char buf[ACTION_LEN];
// 	int res = mjson_get_string(ocpp->cmd_buf, strlen(ocpp->cmd_buf), POS_CL_ACT, buf, ACTION_LEN);
// 	if (res <= 0)
// 		return RES_ERROR;
// 	
// 	if (strcmp(buf, "RemoteStartTransaction") == 0)
// 		ocpp->pres_msg.call.action = ACT_REMOTE_START_TRANSACTION;
// 	else if (strcmp(buf, "RemoteStopTransaction") == 0)
// 		ocpp->pres_msg.call.action = ACT_REMOTE_STOP_TRANSACTION;
// 	else if (strcmp(buf, "ChangeAvailability") == 0)
// 		ocpp->pres_msg.call.action = ACT_CHANGE_AVAILABILITY;
// 	else if (strcmp(buf, "Reset") == 0)
// 		ocpp->pres_msg.call.action = ACT_RESET;
// 	else
// 		return RES_ERROR;
// 	
// 
// 	return RES_OK;
// }
// 
// OCPPResult
// ocpp_get_payload
// (
// 	OCPP *ocpp,
// 	OCPPMessageType type
// )
// {
// 	char path[5] = {0};
// 	if (type == CALL)
// 		strcpy(path, POS_CL_PAYLOAD);
// 	else if (type == CALLRESULT)
// 		strcpy(path, POS_CR_PAYLOAD);
// 	else if (type == CALLERROR)
// 		strcpy(path, POS_CE_ERR_DETL);
// 	else
// 		return RES_ERROR;
// 
// 
// 	const char *p;
//     int n;
// 	if (mjson_find(ocpp->cmd_buf, strlen(ocpp->cmd_buf), path, &p, &n) != MJSON_TOK_OBJECT)
// 		return RES_ERROR;
// 
// 	if (type == CALL)
// 		strncpy(ocpp->pres_msg.call.payload, p, n);
// 	else if (type == CALLRESULT)
// 		strncpy(ocpp->pres_msg.call_result.payload, p, n);
// 	else if (type == CALLERROR)
// 		strncpy(ocpp->pres_msg.call_error.error_details, p, n);
// 
// 	return RES_OK;
// }
// 
// OCPPResult
// ocpp_get_call_error_code(OCPP *ocpp)
// {
// 	char buf[ERR_CODE_LEN];
// 	int res = mjson_get_string(ocpp->cmd_buf, strlen(ocpp->cmd_buf), POS_CE_ERR_CODE, buf, ERR_CODE_LEN);
// 	if (res <= 0)
// 		return RES_ERROR;
// 	
// 	if (strcmp(buf, "GenericError") == 0)
// 		ocpp->pres_msg.call_error.error_code = GENERIC_ERROR;
// 	else
// 		return RES_ERROR;
// 
// 	return RES_OK;
// }
// 
// OCPPResult
// ocpp_get_call_error_descr(OCPP *ocpp)
// {
// 	char buf[DSCR_LEN];
// 	int res = mjson_get_string(ocpp->cmd_buf, strlen(ocpp->cmd_buf), POS_CE_ERR_DSCR, buf, DSCR_LEN);
// 	if (res <= 0)
// 		return RES_ERROR;
// 
// 	strcpy(ocpp->pres_msg.call_error.error_dscr, buf);
// 
// 	return RES_OK;
// }
// 
// 
// void
// ocpp_set_msg_id(OCPP *ocpp)
// {
// 	char id[ID_LEN];
// 	int_to_charset(ocpp->_id, id, 1);
// 	ocpp->_id++;
// 	strcpy(ocpp->pres_msg.ID, id);
// }
// 