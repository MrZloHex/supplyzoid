#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"

Task_Result
rss_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSS TO");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    UNUSED(ctrl);
    UNUSED(t_id);

    return res;
}