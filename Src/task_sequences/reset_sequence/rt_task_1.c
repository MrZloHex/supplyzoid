#include "task_sequences/reset_sequence/rt_task_1.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

#include "controller_rapi_msg.h"

Task_Result
rt_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RT 1");
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

	char status[8];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, strlen(ctrl->ocpp.message.data.call_result.payload), P_RESET_TYPE, status, 8);
    bool r = res_st == -1 ? false : true;
    
    bool type_reset = false;
    if      (strcmp(status, "Hard") == 0) { type_reset = false; }
    else if (strcmp(status, "Soft") == 0) { type_reset = true;  }
    else    { r = false; }

    #ifdef DEBUG
    uprintf(DBUG_UART, 1000, 100, "%u %s\r", r, status);
    #endif
    
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_RESET, &r, NULL, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
    if (!r)
        return res;

    if (type_reset && ctrl->memory.in_transaction)
    {
        _rapi_set_auth_lock_req(&(ctrl->rapi), 0);
        _rapi_send_req(&(ctrl->rapi));

        // ocpp_start_transaction_req(&(ctrl->ocpp), )
    }

    _rapi_reset_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));

    return res;
}


