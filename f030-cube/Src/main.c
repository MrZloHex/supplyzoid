/**
	******************************************************************************
	* @file           : main.c
	* @brief          : Main program body
	******************************************************************************
	* @attention
	*
	* Copyright (c) 2022 STMicroelectronics.
	* All rights reserved.
	*
	* This software is licensed under terms that can be found in the LICENSE file
	* in the root directory of this software component.
	* If no LICENSE file comes with this software, it is provided AS-IS.
	*
	******************************************************************************
	*/

#include "main.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "string.h"
#include "controller.h"
#include "serial.h"

void SystemClock_Config(void);

static Controller controller = {0};

void
HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// OCPP
	if (huart->Instance == controller.ocpp.uart->Instance)
	{
		__HAL_TIM_SET_COUNTER(controller.ocpp.tim, 0);
		if (controller.ocpp.accumulative_buffer[controller.ocpp.acc_buf_index] == '\n')
		{
			if (controller.ocpp.acc_buf_index > 0)
			{
				controller.ocpp.accumulative_buffer[controller.ocpp.acc_buf_index] = 0;
				controller.ocpp.msg_received = true;
			}
			else
			{
				// EMPTY MSG
				HAL_UART_Receive_IT(controller.ocpp.uart, (uint8_t *)&controller.ocpp.accumulative_buffer[0], 1);
			}
		}
		else
		{
			if (controller.ocpp.acc_buf_index == OCPP_BUF_LEN -1)
			{
				// SET GLOBAL FAULT OF BUFFER OVERFLOW
				HAL_UART_Receive_IT(controller.ocpp.uart, (uint8_t *)&controller.ocpp.accumulative_buffer[0], 1);
			}
			else
			{
				HAL_UART_Receive_IT(controller.ocpp.uart, (uint8_t *)&controller.ocpp.accumulative_buffer[++controller.ocpp.acc_buf_index], 1);
			}
		}
	}
	// RAPI
	else if (huart->Instance == controller.rapi.uart->Instance)
	{
		__HAL_TIM_SET_COUNTER(controller.rapi.tim, 0);
		if (controller.rapi.accumulative_buffer[controller.rapi.acc_buf_index] == '\r')
		{
			if (controller.rapi.acc_buf_index > 0)
			{
				controller.rapi.accumulative_buffer[controller.rapi.acc_buf_index] = 0;
				controller.rapi.msg_received = true;
			}
			else
			{
				HAL_UART_Receive_IT(controller.rapi.uart, (uint8_t *)&controller.rapi.accumulative_buffer[0], 1);
			}
		}
		else
		{
			if (controller.rapi.acc_buf_index == RAPI_BUF_LEN -1)
			{
				HAL_UART_Receive_IT(controller.rapi.uart, (uint8_t *)&controller.rapi.accumulative_buffer[0], 1);
			}
			else
			{
				HAL_UART_Receive_IT(controller.rapi.uart, (uint8_t *)&controller.rapi.accumulative_buffer[++controller.rapi.acc_buf_index], 1);
			}
		}
	}

}

void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == controller.ocpp.tim->Instance)
	{
		if (controller.ocpp.acc_buf_index > 0)
		{
			__HAL_TIM_SET_COUNTER(controller.ocpp.tim, 0);
			controller.ocpp.acc_buf_index = 0;
			HAL_UART_Receive_IT(controller.ocpp.uart, (uint8_t *)&controller.ocpp.accumulative_buffer[0], 1);
		}
	}
	else if (htim->Instance == htim14.Instance)
	{
		if (controller.rapi.acc_buf_index > 0)
		{
			__HAL_TIM_SET_COUNTER(controller.rapi.tim, 0);
			controller.rapi.acc_buf_index = 0;
			HAL_UART_Receive_IT(controller.rapi.uart, (uint8_t *)&controller.rapi.accumulative_buffer[0], 1);
		}
	}
}


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_IWDG_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_TIM6_Init();
	MX_TIM14_Init();

	uprintf(&huart2, 100, 10, "hello\r");
	uprintf(&huart1, 100, 10, "hello\n");
	HAL_IWDG_Refresh(&hiwdg);

	// uprintf(&huart1, 100, 100, "[2,\"1\",\"BootNotification\",{\"chargePointVendor\":\"EV Solutions\",\"chargePointModel\":\"PROTOTYPE\"}]\n");

	Controller_Result res = controller_initialize
							(
								&controller,
								&huart1, &huart2,
								&htim6, &htim14,
								&hrtc
							);
	if (res.type != CTRL_OK)
	{
		Error_Handler_with_err("FAILED ON INITIALIZATION");
	}

	HAL_UART_Receive_IT(&huart1, (uint8_t *)&(controller.ocpp.accumulative_buffer[0]), 1);
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&(controller.rapi.accumulative_buffer[0]), 1);

	while (1)
	{
		HAL_IWDG_Refresh(&hiwdg);
		Controller_Result res = controller_update(&controller);
		if (res.type != CTRL_OK)
		{	
			uprintf(&huart1, 1000, 100, "ERR: %u\n", res.type);
			uprintf(&huart1, 1000, 100, "OCPP ERR: %u\n", res.errors.ocpp_err);
			uprintf(&huart1, 1000, 100, "TSET ERR: %u\n", res.errors.tset_err);
			Error_Handler_with_err("FAILED IN LOOP");
		}
		// YOU SHOULD HANDLE IT!!
 
	}
}

/**
	* @brief System Clock Configuration
	* @retval None
	*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
															|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
															|RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
	* @brief  This function is executed in case of error occurrence.
	* @retval None
	*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	// rapi_deinit(&rapi);
	// ocpp_deinit(&ocpp);
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

void
Error_Handler_with_err(const char * err)
{
    uprintf(&huart1, 1000, 256, "ERROR: %s\n", err);
    Error_Handler();
}

#ifdef  USE_FULL_ASSERT
/**
	* @brief  Reports the name of the source file and the source line number
	*         where the assert_param error has occurred.
	* @param  file: pointer to the source file name
	* @param  line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
