#include "task_sequences/remote_start_sequence/rss_task_8.h"
#include "task_sequences/remote_start_sequence/rss_task_9.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_8(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_8\r");
#endif
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .usart = RAPI_USART,
                .func = rss_task_9,
                .func_timeout = rss_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    
    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_LOCKED);
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_8;
    }

    return res;
}