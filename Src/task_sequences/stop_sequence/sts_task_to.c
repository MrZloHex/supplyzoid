#include "task_sequences/stop_sequence/sts_task_to.h"
#include "task_sequences/stop_sequence/sts_task_1.h"

#include "serial.h"

Task_Result
sts_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "STS_TO\r");
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
                .func = sts_task_1,
                .func_timeout = sts_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    return res;
}



