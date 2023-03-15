#include "task_sequences/remote_start_sequence/rss_task_4.h"
#include "task_sequences/remote_start_sequence/rss_task_5.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_4\r");
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
                .func = rss_task_5
            }
        }
    };
    
    ctrl->ocpp.in_transaction = true;
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));
        
    return res;
}