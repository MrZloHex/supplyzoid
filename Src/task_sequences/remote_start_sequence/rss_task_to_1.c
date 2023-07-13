#include "task_sequences/remote_start_sequence/rss_task_to_1.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_to_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSS TO 1");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_LOCKED);
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_to_1;
    }

    ctrl->memory.status = CPS_Faulted;
    _controller_memory_store(&(ctrl->memory));

    UNUSED(t_id);

    return res;
}