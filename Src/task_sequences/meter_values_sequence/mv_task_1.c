#include "task_sequences/meter_values_sequence/mv_task_1.h"
#include "task_sequences/meter_values_sequence/mv_task_2.h"
#include "task_sequences/meter_values_sequence/mv_task_to.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task MV 1");

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
                .func = mv_task_2,
                .func_timeout = mv_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = mv_task_1;
    }

    UNUSED(t_id);

    return res;
}

