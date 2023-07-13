#include "task_sequences/stop_sequence/sts_task_to.h"
#include "task_sequences/stop_sequence/sts_task_1.h"

#include "serial.h"

Task_Result
sts_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task ST TO");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .func = sts_task_1,
                .func_timeout = sts_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    UNUSED(ctrl);
    UNUSED(t_id);

    return res;
}



