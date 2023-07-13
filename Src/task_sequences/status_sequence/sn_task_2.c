#include "task_sequences/status_sequence/sn_task_2.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
sn_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task SN 2");

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

    ctrl->ocpp._started = true;

    UNUSED(t_id);

    return res;
}


