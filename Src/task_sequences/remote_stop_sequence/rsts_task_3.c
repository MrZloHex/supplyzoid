#include "task_sequences/remote_stop_sequence/rsts_task_3.h"
#include "task_sequences/remote_stop_sequence/rsts_task_4.h"
#include "task_sequences/remote_stop_sequence/rsts_task_to.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSTS_3\r");
#endif
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .usart = RAPI_USART,
                .func = rsts_task_4,
                .func_timeout = rsts_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    

    ctrl->memory.in_transaction = false;
    ctrl->memory.status = CPS_SuspendedEVSE;
    _controller_memory_store(&(ctrl->memory));
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rsts_task_3;
    }

    UNUSED(t_id);

    return res;
}




