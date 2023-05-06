#include "task_sequences/stop_sequence/sts_task_1.h"
#include "task_sequences/stop_sequence/sts_task_2.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
sts_task_1(Controller *ctrl, OCPP_MessageID t_id)
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
    uprintf(DBUG_UART, 1000, 10, "STS_1\r");
#endif

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_LOCKED);
    _rapi_send_req(&(ctrl->rapi));
        
    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.func = sts_task_2;
    res.task.task.usart = RAPI_USART;
    res.task.task.genesis_time = HAL_GetTick();

    return res;
}


