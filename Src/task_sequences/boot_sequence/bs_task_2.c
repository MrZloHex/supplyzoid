#include "task_sequences/boot_sequence/bs_task_2.h"
#include "task_sequences/boot_sequence/bs_task_1.h"

#include "serial.h"
#include "string.h"
#include "mjson.h"
#include "time.h"

Task_Result
bs_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
    LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task BS 2");

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_TRIGGER,
                .func = bs_task_1
            }
        }
    };

    size_t pay_len = strlen(ctrl->ocpp.message.data.call_result.payload);

	double interval;
	int res_int = mjson_get_number(ctrl->ocpp.message.data.call_result.payload, pay_len, P_INTERVAL, &interval);
	if (res_int == 0)
		return res; 
	

	char status[9];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_STATUS, status, 9);
	if (res_st == -1)
		return res;

	if (strcmp(status, "Accepted") == 0)
	{}
	else if (strcmp(status, "Pending") == 0 || strcmp(status, "Rejected") == 0)
	{
        return res;
    }

	char time[25];
	int res_time = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, pay_len, P_CURRENT_TIME, time, 25);
	if (res_time == -1)
		return res;

    LOGGER_LOG(&(ctrl->logger), LT_DEBUG, "BOOTED");
	adjust_rtc_time(ctrl->ocpp.rtc, time);

    ctrl->ocpp._started = true;

    res.type = TRES_NEXT;
    res.task.type = WRAP_FINISHED;
    res.task.task.func = NULL;

    UNUSED(t_id);

    return res;
}