#include "task_sequences/get_state_sequence/gs_task_1.h"
#include "task_sequences/get_state_sequence/gs_task_2.h"
#include "task_sequences/get_state_sequence/gs_task_to_1.h"

#include "serial.h"
#include "controller_ocpp.h"
#include "controller_rapi.h"
#include "controller_rapi_msg.h"

Task_Result
gs_task_1(Controller *ctrl, OCPP_MessageID _id)
{
    #ifdef DEBUG
        uprintf(DBUG_UART, 100, 10, "GS_1\r");
    #endif

    _rapi_get_state_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = RAPI_USART,
                .func = gs_task_2,
                .func_timeout = gs_task_to_1,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    return res;
}