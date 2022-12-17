#include "task_sequences/boot_sequence/bs_task_1.h"
#include "task_sequences/boot_sequence/bs_task_2.h"

#include "serial.h"

Task_Result
bs_task_1(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "BS_1\r");
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .func = bs_task_2
            }
        }
    };
    return res;
}
