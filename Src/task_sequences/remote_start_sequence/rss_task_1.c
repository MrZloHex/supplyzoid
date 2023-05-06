#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/remote_start_sequence/rss_task_2.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rss_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
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
    uprintf(DBUG_UART, 1000, 10, "RSS_1\r");
    uprintf(DBUG_UART, 1000, 100,  "`%s`\r", ctrl->ocpp.message.data.call.payload);
#endif
    OCPP_IdTag id_tag;
	int res_id = mjson_get_string(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_ID_TAG, id_tag, OCPP_IdTag_Len-1);
	if (res_id == -1)
		return res;

    strcpy(ctrl->ocpp.idtag, id_tag);
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_AUTHORIZE, &id_tag, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_AUTHORIZE);

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.usart = OCPP_USART;
    strcpy(res.task.task.trigger_id, t_id);
    res.task.task.id = ctrl->ocpp.id_msg -1;
    res.task.task.func = rss_task_2;
    res.task.task.func_timeout = rss_task_to;
    res.task.task.genesis_time = HAL_GetTick();

    return res;
}

