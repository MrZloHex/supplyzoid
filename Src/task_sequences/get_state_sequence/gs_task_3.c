#include "task_sequences/get_state_sequence/gs_task_3.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
gs_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task GS 3");

    ctrl->ocpp._started = true;

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

    UNUSED(t_id);

    return res;
}



