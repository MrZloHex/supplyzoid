#include "task_sequences/send_local_sequence/sll_task_1.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"


Task_Result
sll_task_1(Controller *ctrl, OCPP_MessageID t_id)
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
    uprintf(ctrl->rapi.uart, 1000, 10, "SLL_1\r");
#endif


	char type[16];
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call_result.payload, strlen(ctrl->ocpp.message.data.call_result.payload), P_UPDATE_TYPE, type, 16);
    bool r = res_st == -1 ? false : true;

    return res;
}



