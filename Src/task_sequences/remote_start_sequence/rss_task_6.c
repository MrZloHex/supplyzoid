#include "task_sequences/remote_start_sequence/rss_task_6.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"

Task_Result
rss_task_6(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_6\r");
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

	double transaction_id;
	int res_int = mjson_get_number(ctrl->ocpp.message.data.call_result.payload, pay_len, P_TRANSACTION_ID, &transaction_id);
	if (res_int == 0)
		return res;

	char status[10];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return res;

	if (strcmp("Accepted", status) == 0)
	{
		ctrl->memory.transaction_id = (uint32_t)transaction_id;
		_controller_memory_store(&(ctrl->memory));
		#ifdef DEBUG
		uprintf(ctrl->rapi.uart, 1000, 64, "TRANSACTION CONFIRMED\r");
		#endif
	}
	else if (strcmp("Rejected", status) == 0)
	{
		#ifdef DEBUG
		uprintf(ctrl->rapi.uart, 1000, 64, "TRANSACTION REJECTED\r");
		#endif
	}


    return res;
}