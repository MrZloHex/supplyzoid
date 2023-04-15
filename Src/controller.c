#include "controller.h"
#include "controller_lcd.h"
#include "serial.h"
#include "timer.h"

#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/meter_values_sequence/mv_task_1.h"
#include "task_sequences/stop_sequence/sts_task_1.h"

Controller_Result
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *ocpp_uart,
	UART_HandleTypeDef *rapi_uart,
	TIM_HandleTypeDef *ocpp_tim,
	TIM_HandleTypeDef *rapi_tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 		  wp_pin
)
{
	Controller_TaskSet_Result res = _controller_taskset_initialize(&(controller->task_set), MAX_TASKSET_CAPACITY);
	if (res != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
	}

	_controller_ocpp_initialize(&(controller->ocpp), ocpp_uart, ocpp_tim, rtc, i2c, wp_gpio, wp_pin);
	_controller_rapi_initialize(&(controller->rapi), rapi_uart, rapi_tim);

	_controller_lcd_init(controller, i2c);
	_controller_memory_init(&(controller->memory), i2c);

	if (controller->memory.in_transaction)
	{
		Controller_TaskWrap wr;	
		STS_TASK_WRAP((&wr));
		Controller_TaskSet_Result tres = _controller_taskset_push(&(controller->task_set), wr);
		if (tres != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}

	CONTROLLER_OKAY;
}

Controller_Result
controller_update(Controller *controller)
{
	_controller_lcd_update(controller);

	Controller_TaskSet_Result tres;
	Controller_TaskWrap wrap;
	
	// UPDATE MESSAGES ON OCPP UART
	if (controller->ocpp.msg_received)
	{
		if (_controller_ocpp_transfer(&(controller->ocpp)) == CTRL_PTCL_OK)
		{
			HAL_UART_Receive_IT(controller->ocpp.uart, (uint8_t *)&(controller->ocpp.accumulative_buffer[0]), 1);
			Controller_Protocol_Result res = _controller_ocpp_process_income(&(controller->ocpp), &wrap);
			if (res == CTRL_PTCL_RESPONSE) { ; }
			else if (res != CTRL_PTCL_OK)
			{
				CONTROLLER_OCPP_ERROR(res);
			}
			else
			{
				tres = _controller_taskset_push(&(controller->task_set), wrap);
				if (tres != CTRL_SET_OK)
				{
					CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
				}
			}
		}
	}

	// UPDATE MESSAGES ON RAPI UART
	if (controller->rapi.msg_received)
	{
	uprintf(DBUG_UART, 1000, 100, "GOT `%s`\n", controller->rapi.accumulative_buffer);
		if (_controller_rapi_transfer(&(controller->rapi)) == CTRL_PTCL_OK)
		{
			HAL_UART_Receive_IT(controller->rapi.uart, (uint8_t *)&(controller->rapi.accumulative_buffer[0]), 1);
			Controller_Protocol_Result res = _controller_rapi_process_income(&(controller->rapi), &wrap);
			if (res == CTRL_PTCL_RESPONSE) { ; }
			else if (res != CTRL_PTCL_OK)
			{
				CONTROLLER_RAPI_ERROR(res);
			}
			else
			{
				tres = _controller_taskset_push(&(controller->task_set), wrap);
				if (tres != CTRL_SET_OK)
				{
					CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
				}
			}
		}
	}

	#if 1
	#define METER_VALUES_TIMEOUT 10000
	static Timer mv_timer;
	timer_set(&mv_timer, METER_VALUES_TIMEOUT, true);

	if (controller->memory.in_transaction) { timer_start(&mv_timer); }
	else 								 { timer_stop (&mv_timer); }

	if (timer_timeout(&mv_timer))
	{
		Controller_TaskWrap mv_wrap;
		MV_TASK_WRAP((&mv_wrap));
		_controller_taskset_push(&(controller->task_set), mv_wrap);
	}
	#endif

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

	for (size_t i = 0; i < controller->task_set.size; ++i)
	{
		tres = _controller_taskset_next(&(controller->task_set), &task_wrap);
		if (tres != CTRL_SET_OK)
		{
			_controller_taskset_esc_iter(&(controller->task_set));
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}

		if (task_wrap.type == WRAP_EMPTY)
		{
			continue;
		}

		if (task_wrap.task.type == TASK_PROCESS)
		{
			if (task_wrap.task.usart == OCPP_USART)
			{
				if (!controller->ocpp.is_response)							 { continue; }
				if (!_ocpp_get_resp(&(controller->ocpp), task_wrap.task.id)) { continue; }
			}
			else if (task_wrap.task.usart == RAPI_USART)
			{
				if (controller->rapi.pending) { continue; }
			}
		}
		// __debug_taskset_print(&(controller->task_set));
		if (task_wrap.task.func == NULL)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, CTRL_SET_NULLPTR);
		}

		// EXECUTE TASK AND SET NEXT TASK
		Task_Result task_res = task_wrap.task.func(controller, task_wrap.task.trigger_id);
		if (task_res.type == TRES_NEXT)
		{
			tres = _controller_taskset_update_task(&(controller->task_set), task_res.task);
			if (tres != CTRL_SET_OK)
			{
				_controller_taskset_esc_iter(&(controller->task_set));
				CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
			}
			// __debug_taskset_print(&(controller->task_set));
			break;
		}
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
