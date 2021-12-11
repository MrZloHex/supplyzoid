#include "OCPP.h"

#include "messages/boot_notification.h"
#include "messages/remote_start_transaction.h"
#include "messages/start_transaction.h"
#include "messages/remote_stop_transaction.h"
#include "messages/stop_transaction.h"


void
ocpp_init(OCPP *ocpp)
{
	ocpp->id = 1;
	ocpp->waiting_for_resp = false;

	ocpp->now.call.payload = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->now.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->now.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN);
	ocpp->now.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);

	ocpp->last.call.payload = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->last.call_result.payload      = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
	ocpp->last.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN);
	ocpp->last.call_error.error_details = (char *) malloc(sizeof(char)*PAYLOAD_LEN);
}

void
ocpp_free(OCPP *ocpp)
{
	free(ocpp->now.call.payload);
	free(ocpp->now.call_result.payload);
	free(ocpp->now.call_error.error_dscr);
	free(ocpp->now.call_error.error_details);

	free(ocpp->last.call.payload);
	free(ocpp->last.call_result.payload);
	free(ocpp->last.call_error.error_dscr);
	free(ocpp->last.call_error.error_details);
}

void
ocpp_next(OCPP *ocpp)
{
	ocpp->last.type = ocpp->now.type;
	ocpp->last.ID   = ocpp->now.ID;
	ocpp->last.call.action = ocpp->now.call.action;
	strcpyy(ocpp->last.call.payload, ocpp->now.call.payload);
	strcpyy(ocpp->last.call_result.payload, ocpp->now.call_result.payload);
	ocpp->last.call_error.error_code = ocpp->now.call_error.error_code;
	strcpyy(ocpp->last.call_error.error_dscr, ocpp->now.call_error.error_dscr);
	strcpyy(ocpp->last.call_error.error_details, ocpp->now.call_error.error_details);
}	

void
ocpp_update
(
	OCPP *ocpp,
	EVSE *evse
)
{
	#define BUF_LEN 512
	static char buffer[BUF_LEN];
	static size index = 0;

	char ch = (char) getc(stdin);
	if (ch != '\n')
		buffer[index++] = ch;

	if (index >= BUF_LEN)
	{
		memsett(buffer, 0, index);
		index = 0;
		return;
	}
	

	if (mjson(buffer, index, NULL, NULL) > 0)
	{
		buffer[index] = '\0';
		
		// printf("NEW MESSAGE: `%s`\n", buffer);
		ocpp_handle_message(ocpp, evse, buffer, index);

		memsett(buffer, 0, index);
		index = 0;
	}
}

void
ocpp_handle_message
(
 	OCPP *ocpp,
	EVSE *evse,
	const char *str,
	const size length
)
{
	// printf("HANDLE NEW MESSAGE: `%s`\n", str);

	OCPPResult res_parse = ocpp_parse_message(ocpp, str, length);
	if (res_parse == ERROR)
		return;

	if (ocpp->waiting_for_resp)
	{
		if (ocpp->last.call.action == BOOT_NOTIFICATION)
			ocpp_boot_notification_conf(ocpp, evse);
		else if (ocpp->last.call.action == START_TRANSACTION)
			ocpp_start_transaction_conf(ocpp, evse);
		else if (ocpp->last.call.action == STOP_TRANSACTION)
			ocpp_stop_transaction_conf(ocpp, evse);
		else
			printf("NOT IMPLEMETED\n");
	}
	else
	{
		ocpp_next(ocpp);

		if (ocpp->last.call.action == REMOTE_START_TRANSACTION)
			ocpp_remote_start_transaction_req(ocpp, evse);
		else if (ocpp->last.call.action == REMOTE_STOP_TRANSACTION)
			ocpp_remote_stop_transaction_req(ocpp, evse);
		else
			printf("NOT IMPLEMETED\n");
	}
}

OCPPResult
ocpp_parse_message
(
	OCPP *ocpp,
	const char *str,
	const size length
)
{
	OCPPMessageType msg_type = ocpp_determine_message_type(str, length);
	if (msg_type == ERROR)
		return ERROR;

	ocpp->now.type = msg_type;

	OCPPMessageID msg_id = ocpp_get_message_id(str, length);
	if (msg_id == 0)
		return ERROR;
	ocpp->now.ID = msg_id;

	if (msg_type == CALL)
	{
		OCPPCallAction action = ocpp_get_action(str, length);
		if (action == 0)
			return ERROR;

		ocpp->now.call.action = action;

		char payload[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(CALL, str, length, payload);
		if (res == ERROR)
			return ERROR;

		strcpyy(ocpp->now.call.payload, payload);

		// printf("NEW CALL REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->now.ID);
		// printf("\tACTION: `%d`\n", ocpp->now.call.action);
		// printf("\tPAYLOAD: `%s`\n", ocpp->now.call.payload);
	}
	else if (msg_type == CALLRESULT)
	{
		char payload[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(CALLRESULT, str, length, payload);
		if (res == ERROR)
			return ERROR;

		strcpyy(ocpp->now.call_result.payload, payload);

		// printf("NEW CALL RESULT REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->now.ID);
		// printf("\tPAYLOAD: `%s`\n", ocpp->now.call_result.payload);
	}
	else if (msg_type == CALLERROR)
	{
		OCPPCallErrorCode err_code = ocpp_get_call_error_code(str, length);
		ocpp->now.call_error.error_code = err_code;

		char description[DSCR_LEN];
		OCPPResult res_d = ocpp_get_call_error_descr(str, length, description);
		if (res_d == ERROR)
			return ERROR;

		strcpyy(ocpp->now.call_error.error_dscr, description);

		char details[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(CALLERROR, str, length, details);
		if (res == ERROR)
			return ERROR;
		strcpyy(ocpp->now.call_error.error_details, details);

		// printf("NEW CALL ERROR REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->now.ID);
		// printf("\tERROR CODE: `%d`\n", ocpp->now.call_error.error_code);
		// printf("\tERROR DESCRIPTION: `%s`\n", ocpp->now.call_error.error_dscr);
		// printf("\tERROR DETAILS: `%s`\n", ocpp->now.call_error.error_details);
	}
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
		strcpyy(action_str, "BootNotification");
	else if (action == START_TRANSACTION)
		strcpyy(action_str, "StartTransaction");
	else if (action == STOP_TRANSACTION)
		strcpyy(action_str, "StopTransaction");
	else if (action == STATUS_NOTIFICATION)
		strcpyy(action_str, "StatusNotification");
	else
	{
		printf("NO SUCH REQUEST AVAILABLE\n");
		return;
	}

	char id[37];
	int_to_charset(ocpp->id, id, 1);

	char req[REQ_LEN];

	mjson_snprintf
	(
		req, REQ_LEN,
		"[%u,%Q,%Q,%s]",
		CALL,
		id,
		action_str,
		ocpp->now.call.payload
	);

	// SENDING
	printf("SENDING REQUEST: `%s`\n", req);
	// SENDING

	ocpp->id++;
	if (action != STATUS_NOTIFICATION)
		ocpp->waiting_for_resp = true;

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
		char id[37];
		int_to_charset(ocpp->now.ID, id, 1);
		char req[REQ_LEN];
		mjson_snprintf
		(
			req, REQ_LEN,
			"[%u,%Q,%s]",
			ocpp->now.type,
			id,
			ocpp->now.call_result.payload
		);


		// SENDING
		printf("SENDING RESPONSE: `%s`\n", req);
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







/*
*/






OCPPMessageType
ocpp_determine_message_type
(
	const char *str,
	const size length
)
{
	double type;
	int res = mjson_get_number(str, length, POS_MSG_TYPE, &type);
	if (res <= 0)
		return ERROR;
	switch ((int)type) {
        case CALL:
            return CALL;
        case CALLRESULT:
            return CALLRESULT;
        case CALLERROR:
			return CALLERROR;
        default:
            return ERROR;
    }
}


OCPPMessageID
ocpp_get_message_id
(
	const char *str,
	const size length
)
{
	char buf[50];
	int res = mjson_get_string(str, length, POS_MSG_ID, buf, 50);
	if (res <= 0)
		return ERROR;
	OCPPMessageID id;
	charset_to_ulong(&id , buf);
	return id;
}

OCPPCallAction
ocpp_get_action
(
	const char *str,
	const size length
)
{
	char buf[ACTION_LEN];
	int res = mjson_get_string(str, length, POS_CL_ACT, buf, ACTION_LEN);
	if (res <= 0)
		return ERROR;
	
	if (strcmpp(buf, "RemoteStartTransaction"))
		return REMOTE_START_TRANSACTION;
	else if (strcmpp(buf, "RemoteStopTransaction"))
		return REMOTE_STOP_TRANSACTION;
	else
		return ERROR;
}

OCPPResult
ocpp_get_payload
(
	OCPPMessageType type,
	const char *str,
	const size length,
	char *dst
)
{
	char path[5];
	if (type == CALL)
		strcpyy(path, POS_CL_PAYLOAD);
	else if (type == CALLRESULT)
		strcpyy(path, POS_CR_PAYLOAD);
	else if (type == CALLERROR)
		strcpyy(path, POS_CE_ERR_DETL);
	else
		return ERROR;


	const char *p;
    int n;
    if (mjson_find(str, length, path, &p, &n) != MJSON_TOK_OBJECT)
        return ERROR;

	strncpyy(dst, p, n);
	return 1;
}

OCPPCallErrorCode
ocpp_get_call_error_code
(
	const char *str,
	const size length
)
{
	char buf[ERR_CODE_LEN];
	int res = mjson_get_string(str, length, POS_CE_ERR_CODE, buf, ERR_CODE_LEN);
	if (res <= 0)
		return ERROR;
	
	if (strcmpp(buf, "GenericError"))
		return GENERIC_ERROR;
	else
		return ERROR;
}

OCPPResult
ocpp_get_call_error_descr
(
	const char *str,
	const size length,
	char *dscr
)
{
	char buf[DSCR_LEN];
	int res = mjson_get_string(str, length, POS_CE_ERR_DSCR, buf, DSCR_LEN);
	if (res <= 0)
		return ERROR;

	strcpyy(dscr, buf);
}
