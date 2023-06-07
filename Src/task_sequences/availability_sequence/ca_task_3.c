#include "task_sequences/availability_sequence/ca_task_3.h"
#include "task_sequences/status_sequence/sn_task_2.h"
#include "task_sequences/status_sequence/sn_task_to.h"

#include "serial.h"

Task_Result
ca_task_3(Controller *ctrl, OCPP_MessageID t_id)
{
    // there is  a problem with svcheduling and overflowing the task buffer
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "CA_3\r");
#endif
    Task_Result res =
    {
        .type = TRES_WAIT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .func = ca_task_3,
                .genesis_time = HAL_GetTick(),
                .func_timeout = ca_task_3
            }
        }
    };

    if (ctrl->memory.in_transaction)
        return res;

    OCPP_ChargePointErrorCode error = CPEC_NoError;
    OCPP_ChargePointStatus status = t_id[0] ? CPS_Available : CPS_Unavailable;
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION, &status, &error);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION);

    Task_Result r =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .id = ctrl->ocpp.id_msg -1,
                .func = sn_task_2,
                .genesis_time = HAL_GetTick(),
                .func_timeout = sn_task_to
            }
        }
    };
 
	return r;
}


