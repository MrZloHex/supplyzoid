#include "task_sequences/remote_stop_sequence/rsts_task_4.h"
#include "task_sequences/remote_stop_sequence/rsts_task_5.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSTS_4\r");
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .func = rsts_task_5
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STOP_TRANSACTION, &wh, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STOP_TRANSACTION);

    res.task.task.id = ctrl->ocpp.id_msg -1;
        
    return res;
}



