#include "task_sequences/boot_sequence/bs_task_to.h"
#include "task_sequences/boot_sequence/bs_task_1.h"

#include "serial.h"

Task_Result
bs_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_INFO, "Task BS TO");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .func = bs_task_1,
                .func_timeout = bs_task_to,
                .genesis_time = HAL_GetTick(),
                .type = TASK_TRIGGER
            }
        }
    };

    ctrl->ocpp._started = false;

    UNUSED(t_id);
    
    return res;
}