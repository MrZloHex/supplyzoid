#include "task_sequences/remote_start_sequence/rss_task_2.h"
#include "task_sequences/remote_start_sequence/rss_task_3.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_2\r");
#endif
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


    size_t pay_len = strlen(ctrl->ocpp.message.data.call_result.payload);
	
	char status[16];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_ID_TAG_INFO_STATUS, status, 16);
	if (res_st == -1 || strcmp(status, "Accepted") != 0 || ctrl->memory.available || ctrl->memory.in_transaction) 
	{
        bool r = false;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_START_TRANSACTION, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
        return res;
    }

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    strcpy(res.task.task.trigger_id, t_id);
    res.task.task.func = rss_task_3;
    res.task.task.usart = RAPI_USART;
    res.task.task.func_timeout = rss_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    _rapi_get_state_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_2;
    }


    return res;
}


