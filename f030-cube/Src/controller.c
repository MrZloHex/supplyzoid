#include "controller.h"
#include "serial.h"

Controller_Result
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *ocpp_uart,
	UART_HandleTypeDef *rapi_uart,
	TIM_HandleTypeDef *ocpp_tim,
	TIM_HandleTypeDef *rapi_tim,
	RTC_HandleTypeDef *rtc
)
{
	Controller_Queue_Result res = _controller_queue_initialize(&(controller->queue), MAX_QUEUE_CAPACITY);
	if (res != CTRL_QUE_OK)
	{
		CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, res);
	}

	res = controller_set_simpletask(controller, TASK_OCPP_GET_MSG);
	if (res != CTRL_QUE_OK)
	{
		CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, res);
	}
	res = controller_set_simpletask(controller, TASK_RAPI_GET_MSG);
	if (res != CTRL_QUE_OK)
	{
		CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, res);
	}


	_controller_ocpp_initialize(&(controller->ocpp), ocpp_uart, ocpp_tim, rtc);
	_controller_rapi_initialize(&(controller->rapi), rapi_uart, rapi_tim);


	CONTROLLER_OKAY;
}

Controller_Result
controller_update(Controller *controller)
{
	Controller_Task task = { .type = NO_TASK };
	Controller_Queue_Result q_res = controller_get_task(controller, &task);
	if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
	
#ifdef DEBUG
	if (task.type != NO_TASK)
	{
		uprintf(controller->ocpp.uart, 1000, 100, "TASK %u\n", task.type);
	}
#endif

	Controller_Protocol_Result res;
	Controller_TaskResult t_res;
	switch (task.type)
	{
		case NO_TASK:
			break;

		case TASK_OCPP_GET_MSG:
			res = _controller_ocpp_start_recv(&(controller->ocpp));
			if (res != CTRL_PTCL_OK) { CONTROLLER_OCPP_ERROR(res); }
			break;

		case TASK_OCPP_TRANSFER_MSG:
			res = _controller_ocpp_transfer(&(controller->ocpp));
			if (res != CTRL_PTCL_OK) { CONTROLLER_OCPP_ERROR(res); }
			q_res = controller_set_simpletask(controller, TASK_OCPP_GET_MSG);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			q_res = controller_set_simpletask(controller, TASK_OCPP_PROC_MSG);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			break;

		case TASK_OCPP_PROC_MSG:
			_controller_ocpp_process(&(controller->ocpp));
			break;

		case TASK_OCPP_MAKE_REQ:
			t_res = _controller_ocpp_make_req(&(controller->ocpp), task.data.ocpp_make_req);
			if (t_res.type != CTRL_OK) { CONTROLLER_RAPI_ERROR(t_res.errors.rapi_err) }
			q_res = controller_set_task(controller, t_res.task);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			break;

		case TASK_OCPP_SEND_REQ:
			_controller_ocpp_send_req(&(controller->ocpp), task.data.ocpp_send_req);
			break;

		case TASK_RAPI_GET_MSG:
			res = _controller_rapi_start_recv(&(controller->rapi));
			if (res != CTRL_PTCL_OK) { CONTROLLER_RAPI_ERROR(res); }
			break;

		case TASK_RAPI_TRANSFER_MSG:
			res = _controller_rapi_transfer(&(controller->rapi));
			if (res != CTRL_PTCL_OK) { CONTROLLER_RAPI_ERROR(res); }
			q_res = controller_set_simpletask(controller, TASK_RAPI_GET_MSG);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			q_res = controller_set_simpletask(controller, TASK_RAPI_PROC_MSG);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			break;

		case TASK_RAPI_PROC_MSG:
			t_res = _controller_rapi_process(&(controller->rapi));
			if (t_res.type != CTRL_OK) { CONTROLLER_RAPI_ERROR(t_res.errors.rapi_err) }
			q_res = controller_set_task(controller, t_res.task);
			if (q_res != CTRL_QUE_OK) { CONTROLLER_ERROR(CTRL_QUEUE_ERR, queue_err, q_res); }
			break;

		
	}

	CONTROLLER_OKAY;
}





Controller_Queue_Result
controller_get_task(Controller *controller, Controller_Task *task)
{
	return _controller_queue_dequeue(&(controller->queue), task);
}

Controller_Queue_Result
controller_set_simpletask(Controller *controller, Controller_TaskType type)
{
	Controller_Task task = { .type = type };
	return controller_set_task(controller, task);
}

Controller_Queue_Result
controller_set_task(Controller *controller, Controller_Task task)
{
	return _controller_queue_enqueue(&(controller->queue), task);
}