#include "task_sequences/remote_start_sequence/rss_task_3.h"
#include "task_sequences/remote_start_sequence/rss_task_4.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_3\r");
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
    
    bool accept = (evse_state == EVSE_STATE_B && pilot_state == EVSE_STATE_C);
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_START_TRANSACTION, &accept, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
    if (!accept)
        return res;

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_UNLOCKED);
    _rapi_send_req(&(ctrl->rapi));
        
    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.func = rss_task_4;
    res.task.task.usart = RAPI_USART;

    return res;
}