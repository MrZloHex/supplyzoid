#include "task_sequences/remote_start_sequence/rss_task_6.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_6(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_6\r");
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