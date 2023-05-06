#include "task_sequences/boot_sequence/bs_task_to.h"

#include "serial.h"

Task_Result
bs_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "BS_TO\r");
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
    
    return res;
}