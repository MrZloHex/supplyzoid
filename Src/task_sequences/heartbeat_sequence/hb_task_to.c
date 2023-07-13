#include "task_sequences/heartbeat_sequence/hb_task_to.h"

#include "serial.h"

Task_Result
hb_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task HB TO");

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

    UNUSED(ctrl);
    UNUSED(t_id);

    return res;
}



