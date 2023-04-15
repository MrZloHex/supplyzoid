#include "task_sequences/remote_stop_sequence/rsts_task_1.h"
#include "task_sequences/remote_stop_sequence/rsts_task_2.h"

#include "controller_rapi_msg.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_1(Controller *ctrl, OCPP_MessageID t_id)
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
    uprintf(DBUG_UART, 1000, 10, "RSTS_1\r");
#endif
	double transaction_id_d;
	int res_id = mjson_get_number(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_TRANSACTION_ID, &transaction_id_d);
	if (res_id == 0 || (uint32_t)transaction_id_d != ctrl->memory.transaction_id)
    {
        bool r = false;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_REMOTE_STOP_TRANSACTION, &r, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
		return res;
    }

    _rapi_get_state_req(&(ctrl->rapi));
    _rapi_send_req(&(ctrl->rapi));

    res.type = TRES_NEXT;
    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    strcpy(res.task.task.trigger_id, t_id);
    res.task.task.func = rsts_task_2;
    res.task.task.usart = RAPI_USART;

    return res;
}


