#include "task_sequences/remote_stop_sequence/rsts_task_5.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSTS_5\r");
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
		uprintf(DBUG_UART, 1000, 64, "TRANSACTION FINISHED\n");
        #endif

    return res;
}




