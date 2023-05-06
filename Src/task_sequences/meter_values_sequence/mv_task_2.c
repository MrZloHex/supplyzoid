#include "task_sequences/meter_values_sequence/mv_task_2.h"
#include "task_sequences/meter_values_sequence/mv_task_3.h"

#include "serial.h"
#include "controller_ocpp_msg.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "MV_2\r");
#endif

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .func = mv_task_3,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    
    uint32_t ws;
	_rapi_get_energy_usage_resp(&(ctrl->rapi), &ws, NULL);
	uint32_t wh = ws / 3600;
	ctrl->ocpp.wh = wh;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_METER_VALUES, &wh, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_METER_VALUES);

    res.task.task.id = ctrl->ocpp.id_msg -1;
    return res;
}


