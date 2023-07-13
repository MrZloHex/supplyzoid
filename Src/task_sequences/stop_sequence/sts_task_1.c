#include "task_sequences/stop_sequence/sts_task_1.h"
#include "task_sequences/stop_sequence/sts_task_2.h"
#include "task_sequences/stop_sequence/sts_task_to.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
sts_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task ST 1");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .func = sts_task_2,
                .usart = RAPI_USART,
                .func_timeout = sts_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_LOCKED);
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = sts_task_1;
    }
        
    UNUSED(t_id);

    return res;
}


