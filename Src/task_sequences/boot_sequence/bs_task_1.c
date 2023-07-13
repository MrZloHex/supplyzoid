#include "task_sequences/boot_sequence/bs_task_1.h"
#include "task_sequences/boot_sequence/bs_task_2.h"
#include "task_sequences/boot_sequence/bs_task_to.h"

#include "serial.h"
#include "controller_ocpp.h"

Task_Result
bs_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task BS 1");

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_BOOT_NOTIFICATION, NULL, NULL, NULL);
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
                .func = bs_task_2,
                .func_timeout = bs_task_to,
		        .genesis_time = HAL_GetTick()
            }
        }
    };

    UNUSED(t_id);

    return res;
}
