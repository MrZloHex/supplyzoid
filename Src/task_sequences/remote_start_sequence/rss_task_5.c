#include "task_sequences/remote_start_sequence/rss_task_5.h"
#include "task_sequences/remote_start_sequence/rss_task_6.h"
#include "task_sequences/remote_start_sequence/rss_task_to.h"
#include "task_sequences/stop_sequence/sts_task_1.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "controller_rapi_msg.h"

Task_Result
rss_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RSS_5\r");
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

	size_t pay_len = strlen(ctrl->ocpp.message.data.call_result.payload);

    uprintf(DBUG_UART, 1000, 100, "`%s`\r", ctrl->ocpp.message.data.call_result.payload);
	double transaction_id;
	int res_int = mjson_get_number(ctrl->ocpp.message.data.call_result.payload, pay_len, P_TRANSACTION_ID, &transaction_id);
	if (res_int == 0)
		return res;
	

	char status[16];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 16);
	if (res_st == -1)
		return res;

	uprintf(DBUG_UART, 1000, 100, "`%s`\r", status);

	ctrl->memory.transaction_id = (uint32_t)transaction_id;

	if (strcmp("Accepted", status) == 0)
	{
		_controller_memory_store(&(ctrl->memory));
		#ifdef DEBUG
		uprintf(DBUG_UART, 1000, 64, "TRANSACTION CONFIRMED\r");
		#endif
	}
	else
	{
		#ifdef DEBUG
		uprintf(DBUG_UART, 1000, 64, "TRANSACTION REJECT\r");
		#endif
		res.task.type = WRAP_IN_PROGRESS;
		res.task.task.func = sts_task_1;
		res.task.task.genesis_time = HAL_GetTick();
		res.task.task.type = TASK_TRIGGER;
	}


	res.task.type = WRAP_IN_PROGRESS;
    res.task.task.type = TASK_PROCESS;
    res.task.task.usart = RAPI_USART;
    res.task.task.func = rss_task_6;
    res.task.task.genesis_time = HAL_GetTick();
    res.task.task.func_timeout = rss_task_to;

    _rapi_set_auth_lock_req(&(ctrl->rapi), AUTH_UNLOCKED);
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rss_task_5;
    }
    

    return res;
}