#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/remote_start_sequence/rss_task_2.h"

#include "serial.h"

Task_Result
rss_task_1(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_1\r");
    uprintf(ctrl->rapi.uart, 1000, 100,  "`%s`\r", ctrl->ocpp.message.data.call.payload);

    // _controller_ocpp_make_req(&(ctrl->ocpp), ACT_AUTHORIZE);
    // _controller_ocpp_send_req(&(ctrl->ocpp), ACT_AUTHORIZE);

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
                .id = ctrl->ocpp.id_msg /*-1*/,
                .func = rss_task_2 
            }
        }
    };
    return res;
}

