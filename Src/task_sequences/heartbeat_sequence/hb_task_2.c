#include "task_sequences/heartbeat_sequence/hb_task_2.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"
#include "time.h"

Task_Result
hb_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "HB_2\r");
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

	char time[25];
	int res_time = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, strlen(ctrl->ocpp.message.data.call_result.payload), "$.currentTime", time, 25);
	if (res_time == -1)
		return res;


	adjust_rtc_time(ctrl->ocpp.rtc, time);

    UNUSED(t_id);

    return res;
}




