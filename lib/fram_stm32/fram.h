/**
  ******************************************************************************
  * @file           : fram.h
  * @author         : MrZloHex
  * @brief          : Library for FRAM chips
  ******************************************************************************
  */

#ifndef __FRAM_H__
#define __FRAM_H__

#if   defined(STM32F0)
	#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
	#include "stm32f1xx_hal.h"
#elif defined(STM32F3)
	#include "stm32f3xx_hal.h"
#elif defined(STM32F4)
	#include "stm32f4xx_hal.h"
#elif defined(STM32G0)
	#include "stm32g0xx_hal.h"
#elif defined(STM32G1)
	#include "stm32g1xx_hal.h"
#endif

typedef enum FRAM_Type_E
{
	MB85RC16,
	NOT_SUPPORTED
} FRAM_Type;

typedef struct FRAM_S
{
	I2C_HandleTypeDef *hi2c;
	uint8_t 	i2c_addr;
	GPIO_TypeDef   *wp_gpio;
	uint16_t 	wp_pin;
	uint32_t 	timeout;
	uint16_t 	addr_size;
	FRAM_Type 	type;
} FRAM;


/**
 * @brief Initilize FRAM for an IIC bus (e.g. fram_init(&fram, &hi2c1, 0xA0, GPIOB, GPIO_PIN_1, MB85RC16, 10);)
 * @attention For chips with variable IIC address, use base address
 *
 * @param fram		Pointer to FRAM struct
 * @param i2c		IIC handler
 * @param address	Address of FRAM on IIC bus (e.g. 0xA0 for MB85RC16)
 * @param wp_gpio	GPIO for pin WP of FRAM chip
 * @param wp_pin	PIN Number of pin WP
 * @param type		Type of FRAM chip (see FRAM_Type enumeration)
 * @param timeout	Timeout value of blocking function of IIC transmit
 */
void
fram_init
(
	FRAM 		  *fram,
	I2C_HandleTypeDef *hi2c,
	uint8_t 	address,
	GPIO_TypeDef   *wp_gpio,
	uint16_t 	wp_pin,
	FRAM_Type 	type,
	uint32_t 	timeout
);

/**
 * @brief Read array from FRAM chip
 * 
 * @param fram 		Pointer to FRAM struct
 * @param address	Address of memory
 * @param arr		Array where to store data
 * @param size		Size of array 
 */
HAL_StatusTypeDef
fram_read_array(FRAM *fram, const size_t address, uint8_t *arr, const size_t size);

/**
 * @brief Read byte from FRAM chip
 * 
 * @param fram 		Pointer to FRAM struct
 * @param address	Address of memory
 * @param r			Byte where to store data
 */
HAL_StatusTypeDef
fram_read_byte(FRAM *fram, const size_t address, uint8_t *r);

/**
 * @brief Write array in FRAM chip
 * 
 * @param fram 		Pointer to FRAM struct
 * @param address	Address of memory
 * @param arr		Array of data
 * @param size		Size of array 
 */
HAL_StatusTypeDef
fram_write_array(FRAM *fram, const size_t address, uint8_t *arr, const size_t size);

/**
 * @brief Write byte in FRAM chip
 * 
 * @param fram 		Pointer to FRAM struct
 * @param address	Address of memory
 * @param r			Byte to store
 */
HAL_StatusTypeDef
fram_write_byte(FRAM *fram, const size_t address, uint8_t r);

/**
 * @brief Protect FRAM from being rewritten 
 * 
 * @param fram 		Pointer to FRAM struct
 */
void
fram_protect(FRAM *fram, uint8_t protect);

size_t
_fram_tweak_address(FRAM *fram, size_t address);

void
_fram_address_size(FRAM *fram);

#endif /* __FRAM_H__ */
