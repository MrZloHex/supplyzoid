#include "task_sequences/status_sequence/sn_task_to.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
sn_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task SN TO");

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



