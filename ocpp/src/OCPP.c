#include "OCPP.h"




void
ocpp_init(OCPP *ocpp)
{
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
	printf("HANDLE NEW MESSAGE: `%s`\n", str);

	OCPPResult res_parse = ocpp_parse_message(ocpp, str, length);
	if (res_parse == ERROR)
		return;

	// HANDLING
	
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

		printf("NEW CALL REQ:\n");
		printf("\tID: `%ld`\n", ocpp->now.ID);
		printf("\tACTION: `%d`\n", ocpp->now.call.action);
		printf("\tPAYLOAD: `%s`\n", ocpp->now.call.payload);
	}
	else if (msg_type == CALLRESULT)
	{
		char payload[PAYLOAD_LEN];
		OCPPResult res = ocpp_get_payload(CALLRESULT, str, length, payload);
		if (res == ERROR)
			return ERROR;

		strcpyy(ocpp->now.call_result.payload, payload);

		printf("NEW CALL RESULT REQ:\n");
		printf("\tID: `%ld`\n", ocpp->now.ID);
		printf("\tPAYLOAD: `%s`\n", ocpp->now.call_result.payload);
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

		printf("NEW CALL ERROR REQ:\n");
		printf("\tID: `%ld`\n", ocpp->now.ID);
		printf("\tERROR CODE: `%d`\n", ocpp->now.call_error.error_code);
		printf("\tERROR DESCRIPTION: `%s`\n", ocpp->now.call_error.error_dscr);
		printf("\tERROR DETAILS: `%s`\n", ocpp->now.call_error.error_details);
	}
}



















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
	
	if (strcmpp(buf, "BootNotification"))
		return BOOT_NOTIFICATION;
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
