#include "task_sequences/remote_start_sequence/rss_task_3.h"

#include "serial.h"

Task_Result
rss_task_3(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_3\r");
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