#include "task_sequences/meter_values_sequence/mv_task_4.h"

#include "serial.h"
#include "controller_ocpp_msg.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_4(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "MV_4\r");
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



