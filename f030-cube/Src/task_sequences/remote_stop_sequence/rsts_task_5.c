#include "task_sequences/remote_stop_sequence/rsts_task_5.h"

#include "rapi_msg/get_energy_usage.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"

Task_Result
rsts_task_5(Controller *ctrl, OCPP_MessageID t_id)
{
    uprintf(ctrl->rapi.uart, 1000, 10, "RSTS_5\r");
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

	char status[10];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return res;

	if (strcmp("Accepted", status) == 0)
	{
		uprintf(ctrl->ocpp.uart, 1000, 64, "TRANSACTION FINISHED\n");
	}
	else if (strcmp("Rejected", status) == 0)
	{
		uprintf(ctrl->ocpp.uart, 1000, 64, "TRANSACTION ABORTED\n");
	}


    return res;
}




