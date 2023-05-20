#include "task_sequences/remote_start_sequence/rss_task_6.h"
#include "task_sequences/remote_start_sequence/rss_task_7.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_6(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_6\r");
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
                .func = rss_task_7
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_START_TRANSACTION, &wh, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_START_TRANSACTION);

    res.task.task.id = ctrl->ocpp.id_msg -1;
    res.task.task.func_timeout = rss_task_to;
    res.task.task.genesis_time = HAL_GetTick();
        
    return res;
}