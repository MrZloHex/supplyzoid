#include "task_sequences/meter_values_sequence/mv_task_1.h"
#include "task_sequences/meter_values_sequence/mv_task_2.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "MV_1\r");
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
                .func = mv_task_2
            }
        }
    };
    
    _rapi_get_energy_usage_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));

    return res;
}

