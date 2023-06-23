#include "task_sequences/send_local_sequence/sll_task_1.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"


Task_Result
sll_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
            .task = 
            {
                .func = NULL
            }
        }
    };
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "SLL_1\r");
#endif


	char type[16];
	if (mjson_get_string(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_UPDATE_TYPE, type, 16) == -1)
        return res;

    if (strcmp(type, "Differential") == 0)
    {
        OCPP_UpdateStatus s = US_NotSupported;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_SEND_LOCAL_LIST, &s, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
        return res;
    }
    else if (strcmp(type, "Full") == 0) {}
    else
    {
        return res;
    }

    double list_version = 0;
	if (mjson_get_number(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_LIST_VERSION, &list_version) == 0)
        return res;


    int list_len = 0;
    char list[OCPP_BUF_LEN];
    if (mjson_find(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_LIST, (const char **)&list, &list_len) != MJSON_TOK_ARRAY)
        return res;

    // uprintf(ctrl->ocpp.uart, 1000, 1024, "`%.*s`\n", list_len, list);
    #warning EMPTY LIST????

    int koff, klen, voff, vlen, vtype, off;
    for (off = 0; (off = mjson_next(list, list_len, off, &koff, &klen, &voff, &vlen, &vtype));)
    {
        char s_status[14];
	    if (mjson_get_string(list+voff, vlen, P_TOKEN_STATUS, s_status, 14) == -1)
		    return res;

        OCPP_IdTagInfoStatus status = ocpp_get_idtaginfostatus(s_status);
        if (status == ITIS_NONE)
            return res;

        OCPP_IdToken token = { .idTagInfo.status = status };

	    if (mjson_get_string(list+voff, vlen, P_TOKEN_TAG, token.idTag, OCPP_IdTag_Len-1) == -1)
		    return res;

        ocpp_authlist_add_token(&(ctrl->ocpp.list), token);
    }


    ocpp_authlist_set_version(&(ctrl->ocpp.list), (size_t)list_version);


    OCPP_UpdateStatus s = US_Accepted;
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_SEND_LOCAL_LIST, &s, NULL, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);

    return res;
}



