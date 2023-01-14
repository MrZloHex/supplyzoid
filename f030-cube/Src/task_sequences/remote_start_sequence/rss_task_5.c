#include "task_sequences/remote_start_sequence/rss_task_5.h"
#include "task_sequences/remote_start_sequence/rss_task_6.h"

#include "serial.h"
#include "rapi_msg/get_energy_usage.h"

Task_Result
rss_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_5\r");
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
                .func = rss_task_6
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_START_TRANSACTION, &wh, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_START_TRANSACTION);

    res.task.task.id = ctrl->ocpp.id_msg -1;
        
    return res;
}