#include "task_sequences/remote_start_sequence/rss_task_9.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
rss_task_9(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_9\r");
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



