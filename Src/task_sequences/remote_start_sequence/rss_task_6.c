#include "task_sequences/remote_start_sequence/rss_task_6.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_6(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSS 6");

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