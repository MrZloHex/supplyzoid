#include "task_sequences/heartbeat_sequence/hb_task_2.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
hb_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "HB_2\r");
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




