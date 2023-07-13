#include "task_sequences/meter_values_sequence/mv_task_4.h"

#include "serial.h"
#include "controller_ocpp_msg.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task MV 4");

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

    UNUSED(ctrl);
    UNUSED(t_id);
    
    return res;
}



