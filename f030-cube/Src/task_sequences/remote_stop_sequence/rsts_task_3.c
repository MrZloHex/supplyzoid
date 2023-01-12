#include "task_sequences/remote_stop_sequence/rsts_task_3.h"
#include "task_sequences/remote_stop_sequence/rsts_task_4.h"

#include "rapi_msg/get_energy_usage.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSTS_3\r");
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
                .func = rsts_task_4
            }
        }
    };
    

    _rapi_get_energy_usage_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));
        
    return res;
}




