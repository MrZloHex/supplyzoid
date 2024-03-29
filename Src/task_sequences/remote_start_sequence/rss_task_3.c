#include "task_sequences/remote_start_sequence/rss_task_3.h"
#include "task_sequences/remote_start_sequence/rss_task_4.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_3\r");
#endif
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
    
    uint8_t evse_state;
    uint8_t pilot_state;
    _rapi_get_state_resp(&(ctrl->rapi), &evse_state, NULL, &pilot_state, NULL);
    
    bool accept = (evse_state == EVSE_STATE_B && pilot_state == EVSE_STATE_B && !ctrl->memory.available && !ctrl->memory.in_transaction);
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_START_TRANSACTION, &accept, NULL, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
    if (!accept)
        return res;

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.func = rss_task_4;
    res.task.task.usart = RAPI_USART;
    res.task.task.func_timeout = rss_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_3;
    }

    return res;
}