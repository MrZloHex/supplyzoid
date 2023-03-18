#include "task_sequences/stop_sequence/sts_task_2.h"
#include "task_sequences/stop_sequence/sts_task_3.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
sts_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "STS_2\r");
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
                .func = sts_task_3
            }
        }
    };
    

    ctrl->ocpp.in_transaction = false;
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));
        
    return res;
}



