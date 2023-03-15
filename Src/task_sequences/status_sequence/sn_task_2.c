#include "task_sequences/status_sequence/sn_task_2.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
sn_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "SN_2\r");
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


