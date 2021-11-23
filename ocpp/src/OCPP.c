#include "OCPP.h"




void
ocpp_init(OCPP *ocpp)
{
	ocpp->now.call.action  = (char *) malloc(sizeof(char)*ACTION_LEN);
	ocpp->now.call.payload = (char *) malloc(sizeof(char)*JSON_LEN);
	ocpp->now.call_result.payload      = (char *) malloc(sizeof(char)*JSON_LEN);
	ocpp->now.call_error.error_code    = (char *) malloc(sizeof(char)*ERR_CODE_LEN);
	ocpp->now.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN);
	ocpp->now.call_error.error_details = (char *) malloc(sizeof(char)*JSON_LEN);

	ocpp->last.call.action  = (char *) malloc(sizeof(char)*ACTION_LEN);
	ocpp->last.call.payload = (char *) malloc(sizeof(char)*JSON_LEN);
	ocpp->last.call_result.payload      = (char *) malloc(sizeof(char)*JSON_LEN);
	ocpp->last.call_error.error_code    = (char *) malloc(sizeof(char)*ERR_CODE_LEN);
	ocpp->last.call_error.error_dscr    = (char *) malloc(sizeof(char)*DSCR_LEN);
	ocpp->last.call_error.error_details = (char *) malloc(sizeof(char)*JSON_LEN);
}

void
ocpp_free(OCPP *ocpp)
{
	free(ocpp->now.call.action);
	free(ocpp->now.call.payload);
	free(ocpp->now.call_result.payload);
	free(ocpp->now.call_error.error_code);
	free(ocpp->now.call_error.error_dscr);
	free(ocpp->now.call_error.error_details);

	free(ocpp->last.call.action);
	free(ocpp->last.call.payload);
	free(ocpp->last.call_result.payload);
	free(ocpp->last.call_error.error_code);
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
	OCPPMessageType type = ocpp_determine_message_type(ocpp, str, length);
	printf("TYPE: `%d`\n", type);
}




OCPPMessageType
ocpp_determine_message_type
(
	OCPP *ocpp,
	const char *str,
	const size length
)
{
	double type;
	int res = mjson_get_number(str, length, TYPE_POS_MSG, &type);
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
