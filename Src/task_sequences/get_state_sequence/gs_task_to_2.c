#include "task_sequences/get_state_sequence/gs_task_to_2.h"

#include "serial.h"

Task_Result
gs_task_to_2(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_INFO, "Task GS TO 2");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    ctrl->ocpp._started = false;
    
    UNUSED(t_id);

    return res;
}