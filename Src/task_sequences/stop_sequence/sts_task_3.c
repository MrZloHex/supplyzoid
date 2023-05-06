#include "task_sequences/stop_sequence/sts_task_3.h"
#include "task_sequences/stop_sequence/sts_task_4.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
sts_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "STS_3\r");
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
                .usart = OCPP_USART,
                .func = sts_task_4,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STOP_TRANSACTION, &wh, &(ctrl->memory.transaction_id));
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STOP_TRANSACTION);

    res.task.task.id = ctrl->ocpp.id_msg -1;
        
    return res;
}



