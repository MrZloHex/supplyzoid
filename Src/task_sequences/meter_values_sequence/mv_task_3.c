#include "task_sequences/meter_values_sequence/mv_task_3.h"
#include "task_sequences/meter_values_sequence/mv_task_4.h"
#include "task_sequences/meter_values_sequence/mv_task_to.h"

#include "serial.h"
#include "controller_ocpp_msg.h"
#include "controller_rapi_msg.h"

Task_Result
mv_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "MV_3\r");
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
                .func = mv_task_4,
                .func_timeout = mv_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
    

    uint32_t mamps, mvolts;
    _rapi_get_charging_current_voltage_resp(&(ctrl->rapi), &mvolts, &mamps);
    uint32_t amps = mamps / 1000, volts = mvolts / 1000;

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_METER_VALUES, &(ctrl->ocpp.wh), &amps, &volts);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_METER_VALUES);

    res.task.task.id = ctrl->ocpp.id_msg -1;
    return res;
}


