#include "task_sequences/boot_sequence/bs_task_1.h"
#include "task_sequences/boot_sequence/bs_task_2.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
bs_task_1(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "BS_1\r");

    _controller_ocpp_make_req(&(ctrl->ocpp), ACT_BOOT_NOTIFICATION);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_BOOT_NOTIFICATION);

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .id = ctrl->ocpp.id_msg -1,
                .func = bs_task_2
            }
        }
    };
    return res;
}
