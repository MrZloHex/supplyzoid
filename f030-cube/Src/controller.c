#include "controller.h"
#include "serial.h"

#include "task_sequences/remote_start_sequence/rss_task_1.h"

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
	Controller_TaskSet_Result res = _controller_taskset_initialize(&(controller->task_set), MAX_TASKSET_CAPACITY);
	if (res != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
	}

	_controller_ocpp_initialize(&(controller->ocpp), ocpp_uart, ocpp_tim, rtc);
	_controller_rapi_initialize(&(controller->rapi), rapi_uart, rapi_tim);
	CONTROLLER_OKAY;
}

Controller_Result
controller_update(Controller *controller)
{
	Controller_TaskSet_Result tres;
	Controller_TaskWrap wrap;

	// UPDATE MESSAGES ON OCPP UART
	if (controller->ocpp.msg_received)
	{
		if (_controller_ocpp_transfer(&(controller->ocpp)) == CTRL_PTCL_OK)
		{
			HAL_UART_Receive_IT(controller->ocpp.uart, (uint8_t *)&(controller->ocpp.accumulative_buffer[0]), 1);
			Controller_Protocol_Result res = _controller_ocpp_process_income(&(controller->ocpp), &wrap);
			if (res != CTRL_PTCL_OK)
			{
				CONTROLLER_OCPP_ERROR(res);
			}
			tres = _controller_taskset_push(&(controller->task_set), wrap);
			if (tres != CTRL_SET_OK)
			{
				CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
			}
		}
	}

	// UPDATE MESSAGES ON RAPI UART
	if (controller->rapi.msg_received)
	{
		if (_controller_rapi_transfer(&(controller->rapi)) == CTRL_PTCL_OK)
		{
			HAL_UART_Receive_IT(controller->rapi.uart, (uint8_t *)&(controller->rapi.accumulative_buffer[0]), 1);
			Controller_Protocol_Result res = _controller_rapi_process_income(&(controller->rapi), &wrap);
			if (res != CTRL_PTCL_OK)
			{
				CONTROLLER_OCPP_ERROR(res);
			}
			tres = _controller_taskset_push(&(controller->task_set), wrap);
			if (tres != CTRL_SET_OK)
			{
				CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
			}
		}
	}

	// IF THERE ARE NO FACES (TASKS SORRY) - RETURN
	if (controller->task_set.size == 0)
	{
		CONTROLLER_OKAY;
	}

	tres = _controller_taskset_iterate(&(controller->task_set));
	if (tres != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
	}

	Controller_TaskWrap task_wrap;

	for (size_t i = 0; i < controller->task_set.capacity; ++i)
	{
		tres = _controller_taskset_next(&(controller->task_set), &task_wrap);
		if (tres != CTRL_SET_OK)
		{
			_controller_taskset_esc_iter(&(controller->task_set));
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
		uprintf(controller->ocpp.uart, 1000, 100, "wrap %u, %p\n", task_wrap.type, task_wrap.task.func);

		if (task_wrap.type == WRAP_EMPTY)
		{
			continue;
		}

		if (task_wrap.task.type == TASK_PROCESS)
		{

		}

		if (task_wrap.task.func == NULL)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, CTRL_SET_NULLPTR);
		}

		// EXECUTE TASK AND SET NEXT TASK
		Task_Result task_res = task_wrap.task.func(controller);
		if (task_res.type == TRES_NEXT)
		{
			tres = _controller_taskset_update_task(&(controller->task_set), task_res.task);
			if (tres != CTRL_SET_OK)
			{
				_controller_taskset_esc_iter(&(controller->task_set));
				CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
			}
		}

		break;
	}

	_controller_taskset_esc_iter(&(controller->task_set));

	// CHECK FOR FINISHED TASKS AND DELETE THEM
	tres = _controller_taskset_pop(&(controller->task_set));
	if (tres != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
	}

	CONTROLLER_OKAY;
}
