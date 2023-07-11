#include "task_sequences/get_state_sequence/gs_task_to_2.h"

#include "serial.h"

Task_Result
gs_task_to_2(Controller *ctrl, OCPP_MessageID t_id)
{

#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "GS_TO_2\r");
#endif
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    ctrl->ocpp._started = false;
    
    UNUSED(t_id);

    return res;
}