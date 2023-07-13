#include "task_sequences/remote_stop_sequence/rsts_task_5.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task RSTS 5");

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


    UNUSED(ctrl);
    UNUSED(t_id);

    return res;
}




