#include "task_sequences/remote_start_sequence/rss_task_5.h"
#include "task_sequences/remote_start_sequence/rss_task_4.h"
#include "task_sequences/remote_start_sequence/rss_task_6.h"
#include "task_sequences/remote_start_sequence/rss_task_8.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_5\r");
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
                .func = rss_task_6,
                .genesis_time = HAL_GetTick(),
                .func_timeout = rss_task_to
            }
        }
    };
    
    uint8_t evse_state;
    uint8_t pilot_state;
    _rapi_get_state_resp(&(ctrl->rapi), &evse_state, NULL, &pilot_state, NULL);
    
    if (evse_state != EVSE_STATE_C || pilot_state != EVSE_STATE_C)
    {
        #define k_B2_C2_MAX_TRANSITION_TIME 3000
        if (ctrl->seq_timer_var + k_B2_C2_MAX_TRANSITION_TIME > HAL_GetTick())
        {
            res.task.task.func = rss_task_4;
        }
        else
        {
            res.task.task.func = rss_task_8;
        }
        res.task.task.type = TASK_TRIGGER;
        return res;
    }


    ctrl->memory.in_transaction = true;
    _controller_memory_store(&(ctrl->memory));

    _rapi_get_energy_usage_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_5;
    }

    return res;
}