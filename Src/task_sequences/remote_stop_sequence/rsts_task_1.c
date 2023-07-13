#include "task_sequences/remote_stop_sequence/rsts_task_1.h"
#include "task_sequences/remote_stop_sequence/rsts_task_2.h"
#include "task_sequences/remote_stop_sequence/rsts_task_to.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSTS 1");

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

	double transaction_id_d;
	int res_id = mjson_get_number(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_TRANSACTION_ID, &transaction_id_d);
	if (res_id == 0 || (uint32_t)transaction_id_d != ctrl->memory.transaction_id)
    {
        bool r = false;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_STOP_TRANSACTION, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
		return res;
    }

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    strcpy(res.task.task.trigger_id, t_id);
    res.task.task.func = rsts_task_2;
    res.task.task.usart = RAPI_USART;
    res.task.task.func_timeout = rsts_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    _rapi_get_state_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rsts_task_1;
    }

    return res;
}


