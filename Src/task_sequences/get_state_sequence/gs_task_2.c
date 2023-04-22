#include "task_sequences/get_state_sequence/gs_task_2.h"
#include "task_sequences/get_state_sequence/gs_task_3.h"

#include "serial.h"
#include "controller_ocpp.h"
#include "controller_rapi.h"
#include "controller_rapi_msg.h"

Task_Result
gs_task_2(Controller *ctrl, OCPP_MessageID _id)
{
    #ifdef DEBUG
        uprintf(DBUG_UART, 100, 10, "GS_2\r");
    #endif
    Task_Result rres =
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

    uint8_t evse_state, pilot_state;
    _rapi_get_state_resp(&(ctrl->rapi), &evse_state, NULL, &pilot_state, NULL);
    if (ctrl->memory.available)
        return rres;

    OCPP_ChargePointStatus status;
    OCPP_ChargePointErrorCode error = CPEC_NoError;
    // TODO: MinimumStatusDuration
    if (evse_state > EVSE_STATE_UNKNOWN && evse_state <= EVSE_FAULT_STATE_BEGIN)
    {
        status = (OCPP_ChargePointStatus)evse_state;
        if (pilot_state == EVSE_STATE_C && evse_state == EVSE_STATE_B)
            status = CPS_SuspendedEVSE; // TODO: verify it OCPP
    }
    else
    {
        status = CPS_Faulted;
        if (evse_state == EVSE_STATE_NO_GROUND)
            error = CPEC_GroundFailure;
        else if (evse_state == EVSE_STATE_OVER_CURRENT)
            error = CPEC_OverCurrentFailure;
        else
            error = CPEC_OtherError;
    }

    ctrl->memory.status = status;
    _controller_memory_store(&(ctrl->memory));

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION, &status, &error);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION);

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
                .id = ctrl->ocpp.id_msg -1,
                .func = gs_task_3 
            }
        }
    };

    return res;
}