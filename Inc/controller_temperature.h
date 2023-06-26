/**
  ******************************************************************************
  * @file    controller_temperatur.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TEMPERATURE_H__
#define __CONTROLLER_TEMPERATURE_H__

#include "i2c.h"

void
_controller_temp_init(I2C_HandleTypeDef *i2c);

float
_controller_temp_get_temp();

#endif /* __CONTROLLER_TEMPERATURE_H__ */

