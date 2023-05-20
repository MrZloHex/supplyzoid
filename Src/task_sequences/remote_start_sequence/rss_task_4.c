#include "task_sequences/remote_start_sequence/rss_task_4.h"
#include "task_sequences/remote_start_sequence/rss_task_5.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_4\r");
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
                .func = rss_task_5,
                .func_timeout = rss_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    
    _rapi_get_state_req(&(ctrl->rapi)); 
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_4;
    }

    if (ctrl->seq_timer_var == 0)
    {
        ctrl->seq_timer_var = HAL_GetTick();
    }

    return res;
}