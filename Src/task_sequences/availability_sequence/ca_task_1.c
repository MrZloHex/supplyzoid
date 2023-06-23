#include "task_sequences/availability_sequence/ca_task_1.h"
#include "task_sequences/availability_sequence/ca_task_2.h"
#include "task_sequences/availability_sequence/ca_task_3.h"

#include "serial.h"
#include "controller_ocpp.h"
#include "string.h"
#include "mjson.h"


Task_Result
ca_task_1(Controller *ctrl, OCPP_MessageID t_id)
{
    // there is  a problem with svcheduling and overflowing the task buffer
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "CA_1\r");
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

	double  connector_id;
    char    status[12];
	int res_id = mjson_get_number(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_CONTROLLER_ID, &connector_id);
	int res_st = mjson_get_string(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_TYPE, status, 12);
	if (res_id == 0 || res_st == -1 || (uint32_t)connector_id > 1)
    {
        OCPP_AvailabilityStatus r = AS_Rejected;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_CHANGE_AVAILABILITY, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
		return res;
    }

    bool operative = false;
    if (strcmp(status, "Inoperative") == 0)     { operative = false; }
    else if (strcmp(status, "Operative") == 0)  { operative = true;  }
    else
    {
        OCPP_AvailabilityStatus r = AS_Rejected;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_CHANGE_AVAILABILITY, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
		return res;
    }

    if (ctrl->memory.in_transaction)
    {
        OCPP_AvailabilityStatus r = AS_Scheduled;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_CHANGE_AVAILABILITY, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
        // res.task.type = WRAP_IN_PROGRESS;
        // res.task.task.func = ca_task_3;
        // res.task.task.trigger_id[0] = operative;
        // res.task.task.type = TASK_TRIGGER;
        // res.task.task.genesis_time = HAL_GetTick();
		return res;
    }
    else
    {
        if (operative)
        {
            ctrl->memory.available = false;
            ctrl->memory.status = CPS_Available;
        }
        else
        {
            ctrl->memory.available = true;
            ctrl->memory.status = CPS_Unavailable;
        }

        _controller_memory_store(&(ctrl->memory));
        OCPP_AvailabilityStatus r = AS_Accepted;
        _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_CHANGE_AVAILABILITY, &r, NULL, NULL);
        _controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
    }

    res.task.type = WRAP_IN_PROGRESS;
    res.task.task.func = ca_task_2;
    res.task.task.trigger_id[0] = operative;
    res.task.task.type = TASK_TRIGGER;
    res.task.task.genesis_time = HAL_GetTick();
		
    return res;
}


