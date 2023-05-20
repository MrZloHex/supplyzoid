#include "task_sequences/remote_stop_sequence/rsts_task_to.h"

#include "serial.h"

Task_Result
rsts_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSTS_TO\r");
#endif
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    return res;
}