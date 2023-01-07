#include "task_sequences/remote_start_sequence/rss_task_6.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"

Task_Result
rss_task_6(Controller *ctrl)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSS_6\r");
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
		ctrl->ocpp.transaction_id = (uint32_t)transaction_id;
		uprintf(ctrl->ocpp.uart, 1000, 64, "TRANSACTION CONFIRMED\n");
	}
	else if (strcmp("Rejected", status) == 0)
	{
		uprintf(ctrl->ocpp.uart, 1000, 64, "TRANSACTION REJECTED\n");
	}


    return res;
}