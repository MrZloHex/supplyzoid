#include "controller_temperature.h"

#include "sht2x_for_stm32_hal.h"

void
_controller_temp_init(I2C_HandleTypeDef *i2c)
{
    SHT2x_Init(i2c);
    SHT2x_SetResolution(RES_14_12);
}

float
_controller_temp_get_temp()
{
    return SHT2x_GetTemperature(1);
}