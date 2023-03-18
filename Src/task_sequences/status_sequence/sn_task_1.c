#include "task_sequences/status_sequence/sn_task_1.h"
#include "task_sequences/status_sequence/sn_task_2.h"

#include "serial.h"
#include "controller_ocpp.h"

#include "controller_rapi_msg.h"

Task_Result
sn_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
#ifdef DEBUG
    uprintf(ctrl->rapi.uart, 1000, 10, "SN_1\r");
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
    _rapi_evse_state_transition_req(&(ctrl->rapi), &evse_state, &pilot_state);
    if (evse_state ==  EVSE_STATE_UNKNOWN)
        return rres;

    ctrl->e_state = evse_state;
    ctrl->p_state = pilot_state;

    OCPP_ChargePointStatus status;
    OCPP_ChargePointErrorCode error = CPEC_NoError;
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

    ctrl->ocpp.status = status;

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
                .func = sn_task_2
            }
        }
    };
    return res;
}

