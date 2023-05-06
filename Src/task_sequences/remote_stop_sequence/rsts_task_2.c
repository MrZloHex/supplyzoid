#include "task_sequences/remote_stop_sequence/rsts_task_2.h"
#include "task_sequences/remote_stop_sequence/rsts_task_3.h"
#include "task_sequences/remote_stop_sequence/rsts_task_to.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
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
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSTS_2\r");
#endif

	uint8_t evse_state;
	_rapi_get_state_resp(&(ctrl->rapi), &evse_state, NULL, NULL, NULL);

    bool accept = (evse_state == EVSE_STATE_C);
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_STOP_TRANSACTION, &accept, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
    if (!accept)
        return res;

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.func = rsts_task_3;
    res.task.task.usart = RAPI_USART;
    res.task.task.func_timeout = rsts_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_LOCKED);
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rsts_task_2;
    }
        
    return res;
}



