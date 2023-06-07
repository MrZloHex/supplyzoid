#include "task_sequences/heartbeat_sequence/hb_task_to.h"

#include "serial.h"

Task_Result
hb_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "HB_TO\r");
#endif

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
            .task = 
            {
                .func = NULL
            }
        }
    };
    return res;
}



