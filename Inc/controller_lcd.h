/**
  ******************************************************************************
  * @file    controller_lcd.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_LCD_H__
#define __CONTROLLER_LCD_H__

#include "controller.h"

const static char *k_ChargePointStatus[] = {
	"",
	"Available  ",
	"Connected  ",
	"Charging   ",
	"Waiting    ",
	"Waiting    ",
	"Finishing  ",
	"Reserved   ",
	"Unavailable",
	"Faulted    "
};

const static char *k_station_status[] = {
	"Offline", "Online "
};


void
_controller_lcd_init(Controller *ctrl, I2C_HandleTypeDef *i2c)
{
	lcd_i2c_init(&(ctrl->lcd), i2c, 0x4E, LCD_20x4, 10);
}

void
_controller_lcd_update(Controller *ctrl)
{
	static bool online = true;
	static OCPP_ChargePointStatus port1 = 0;
	lcd_i2c_pos_printf(&(ctrl->lcd), 0, 1, "%X %X %X", ctrl->memory.status, ctrl->memory.in_transaction, ctrl->memory.available);
	// static OCPP_ChargePointStatus port2 = CPS_Unavailable;

	if (online != ctrl->ocpp._started)
	{
		online = ctrl->ocpp._started;
		lcd_i2c_pos_printf(&(ctrl->lcd), 0, 0, "Station: %s", k_station_status[online]);
	}

	if (ctrl->rapi._started && port1 != ctrl->memory.status)
	{
		port1 = ctrl->memory.status;
		lcd_i2c_pos_printf(&(ctrl->lcd), 0, 2, "Port 1: %s", k_ChargePointStatus[port1]);
	}
	// lcd_i2c_pos_printf(&(ctrl->lcd), 0, 3, "Port 2: %s", k_ChargePointStatus[port2]);
}


#endif /* __CONTROLLER_LCD_H__ */

