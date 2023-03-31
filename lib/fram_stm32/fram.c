#include "fram.h"

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
)
{
	fram->hi2c     = hi2c;
	fram->i2c_addr = address;
	fram->wp_gpio  = wp_gpio;
	fram->wp_pin   = wp_pin;
	fram->timeout  = timeout;
	fram->type     = type;
	_fram_address_size(fram);
	HAL_GPIO_WritePin(wp_gpio, wp_pin, 0);
}

HAL_StatusTypeDef
fram_read_array(FRAM *fram, const size_t address, uint8_t *arr, const size_t size)
{
	return HAL_I2C_Mem_Read
	(
		fram->hi2c, _fram_tweak_address(fram, address),
		address, fram->addr_size, arr,
		size, fram->timeout
	);
}

HAL_StatusTypeDef
fram_read_byte(FRAM *fram, const size_t address, uint8_t *r)
{
	return fram_read_array(fram, address, r, 1);
}

HAL_StatusTypeDef
fram_write_array(FRAM *fram, const size_t address, uint8_t *arr, const size_t size)
{
	return HAL_I2C_Mem_Write
	(
		fram->hi2c, _fram_tweak_address(fram, address),
		address, fram->addr_size, arr,
		size, fram->timeout
	);
}

HAL_StatusTypeDef
fram_write_byte(FRAM *fram, const size_t address, uint8_t r)
{
	return fram_write_array(fram, address, &r, 1);
}

void
fram_protect(FRAM *fram, uint8_t protect)
{
	HAL_GPIO_WritePin(fram->wp_gpio, fram->wp_pin, protect);
}


size_t
_fram_tweak_address(FRAM *fram, size_t address)
{
	if (fram->type == MB85RC16)
		return fram->i2c_addr | (address >> 8);
	else
		return fram->i2c_addr;
}

void
_fram_address_size(FRAM *fram)
{
	if (fram->type == MB85RC16)
		fram->addr_size = I2C_MEMADD_SIZE_8BIT;
	else
		fram->addr_size = I2C_MEMADD_SIZE_8BIT;
}
