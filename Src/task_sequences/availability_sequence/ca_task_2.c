#include "task_sequences/availability_sequence/ca_task_2.h"
#include "task_sequences/status_sequence/sn_task_2.h"
#include "task_sequences/status_sequence/sn_task_to.h"

#include "serial.h"

Task_Result
ca_task_2(Controller *ctrl, OCPP_MessageID t_id)
{
    // there is  a problem with svcheduling and overflowing the task buffer
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "CA_2\r");
#endif
    Task_Result res =
    {
        .type = TRES_WAIT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task = 
            {
                .func = NULL
            }
        }
    };

    OCPP_ChargePointErrorCode error = CPEC_NoError;
    OCPP_ChargePointStatus status = t_id[0] ? CPS_Available : CPS_Unavailable;
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION, &status, &error, NULL);
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
                .func_timeout = sn_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };
 
	return r;
}


