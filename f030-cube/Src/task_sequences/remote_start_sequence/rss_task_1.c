#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/remote_start_sequence/rss_task_2.h"

#include "serial.h"

Task_Result
rss_task_1(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_1\r");
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .func = rss_task_2 
            }
        }
    };
    return res;
}

