#include "task_sequences/data_transfer_sequence/dt_task_1.h"

#include "mjson.h"
#include "controller_ocpp.h"
#include "controller_temperature.h"
#include "stdio.h"
#include "serial.h"

void
_dt_err_resp(Controller *ctrl, OCPP_MessageID t_id)
{ 
	OCPPDataTransferStatus st = DTS_Rejected;
	char buf[] = "";
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
	_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
}

Task_Result
dt_task_1(Controller *ctrl, OCPP_MessageID _id)
{
#ifdef DEBUG
	uprintf(DBUG_UART, 1000, 10, "DT_1\n");
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

	char data[MAX_DATA_TRANSFER_DATA_LEN];
	int res_id = mjson_get_string(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_DATA, data, MAX_DATA_TRANSFER_DATA_LEN);
	if (res_id == -1)
	{
		_dt_err_resp(ctrl, _id);
		return res;
	}

	uprintf(DBUG_UART, 1000, 50, "%s\n", data);
// #define MAX_METHOD_LEN 16
// 	char method[MAX_METHOD_LEN];
// 	res_id = mjson_get_string(data, strlen(data), "$.method", method, MAX_METHOD_LEN);
// 	if (res_id == -1)
// 	{
// 		_dt_err_resp(ctrl, _id);
// 		return res;
// 	}

	OCPPDataTransferStatus st = DTS_Accepted;
	if (strcmp(data, "get_alt_measurements") == 0)
	{
		char buf[14];
		sprintf(buf, "%f", _controller_temp_get_hum());
    	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
		_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	}
	else if (strcmp(data, "get_error") == 0)
	{
		char buf[14];
		sprintf(buf, "%f", _controller_temp_get_hum());
    	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
		_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	}
	else
	{
		_dt_err_resp(ctrl, _id);
	}


	return res;
}
