#include "task_sequences/stop_sequence/sts_task_4.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
sts_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "STS_4\r");
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

        #ifdef DEBUG
		uprintf(DBUG_UART, 1000, 64, "TRANSACTION STOPPED\n");
        #endif


    UNUSED(ctrl);
    UNUSED(t_id);

    return res;
}




