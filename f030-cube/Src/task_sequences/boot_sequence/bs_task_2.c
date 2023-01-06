#include "task_sequences/boot_sequence/bs_task_2.h"

#include "serial.h"

Task_Result
bs_task_2(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "BS_2\r");
    Task_Result res =
    {
        .type = TRES_WAIT,
        .task =
        {
            .type = WRAP_FINISHED,
            .task = 
            {
                .func = NULL
            }
        }
    };

    uprintf(ctrl->rapi.uart, 1000, 266, "`%s`\r", ctrl->ocpp.message.data.call_result.payload);
    res.type = TRES_NEXT;
    return res;
}