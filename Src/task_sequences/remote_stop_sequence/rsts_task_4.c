#include "task_sequences/remote_stop_sequence/rsts_task_4.h"
#include "task_sequences/remote_stop_sequence/rsts_task_5.h"
#include "task_sequences/remote_stop_sequence/rsts_task_to.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSTS 4");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .func = rsts_task_5
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STOP_TRANSACTION, &wh, &(ctrl->memory.transaction_id), NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STOP_TRANSACTION);

    res.task.task.id = ctrl->ocpp.id_msg -1;
    res.task.task.func_timeout = rsts_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    UNUSED(t_id);
        
    return res;
}



