#include "task_sequences/get_verlist_sequence/gvl_task_1.h"

#include "serial.h"
#include "mjson.h"
#include "string.h"


Task_Result
gvl_task_1(Controller *ctrl, OCPP_MessageID t_id)
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
    uprintf(DBUG_UART, 1000, 10, "GVL_1\r");
#endif


    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_GET_LOCAL_LIST_VERSION, NULL, NULL);
    _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);


    return res;
}