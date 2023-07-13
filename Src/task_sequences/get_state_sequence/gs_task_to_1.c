#include "task_sequences/get_state_sequence/gs_task_to_1.h"

#include "serial.h"

Task_Result
gs_task_to_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_INFO, "Task GS TO 1");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    ctrl->rapi._started = false;

    UNUSED(t_id);
    
    return res;
}