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
	"Available",
	"Plugged",
	"Charging",
	"SuspendEVSE",
	"SuspendEV",
	"Finishing",
	"Reserved",
	"Unavailable",
	"Faulted"
};

uint8_t plug[8] ={
  0x0A,
  0x0A,
  0x1F,
  0x11,
  0x11,
  0x0E,
  0x04,
  0x04
};

uint8_t spark[8] ={
  0x03,
  0x06,
  0x0C,
  0x1C,
  0x07,
  0x06,
  0x0C,
  0x18
};

uint8_t car1[8] ={
  0x00,
  0x01,
  0x02,
  0x0F,
  0x10,
  0x16,
  0x06,
  0x00
};
uint8_t car2[8] ={
  0x1F,
  0x00,
  0x00,
  0x1F,
  0x00,
  0x1F,
  0x00,
  0x00
};
uint8_t car3[8] ={
  0x18,
  0x04,
  0x04,
  0x1E,
  0x01,
  0x0D,
  0x0C,
  0x00
};

uint8_t avail[8] =
{
	0x10,
	0x18,
	0x18,
	0x18,
	0x15,
	0x12,
	0x10,
	0x00
};

uint8_t wait[8] =
{
	0x1F,
	0x11,
	0x0A,
	0x04,
	0x0A,
	0x11,
	0x1F,
	0x00
};

void
_controller_lcd_init(Controller *ctrl, I2C_HandleTypeDef *i2c)
{
	lcd_i2c_init(&(ctrl->lcd), i2c, 0x4E, LCD_20x4, 10);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 0, car1);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 1, car2);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 2, car3);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 3, plug);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 4, spark);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 5, wait);
	lcd_i2c_make_custom_char(&(ctrl->lcd), 6, avail);
}

void
_controller_lcd_update(Controller *ctrl)
{
	static OCPP_ChargePointStatus state = 0;
	static uint32_t wh = 100;

	if (wh != ctrl->ocpp.wh && ctrl->ocpp.in_transaction)
	{
		wh = ctrl->ocpp.wh;
		lcd_i2c_pos_printf(&(ctrl->lcd), 2, 1, "%uWh", wh);
	}

	if (state == ctrl->ocpp.status)
		return;
	    
	state = ctrl->ocpp.status;
	lcd_i2c_clear(&(ctrl->lcd));
	lcd_i2c_pos_printf(&(ctrl->lcd), 0, 0, "#1 %s", k_ChargePointStatus[state]);

	lcd_i2c_set_cursor(&(ctrl->lcd), 20-3, 0);
	lcd_i2c_char(&(ctrl->lcd), 0);
	lcd_i2c_char(&(ctrl->lcd), 1);
	lcd_i2c_char(&(ctrl->lcd), 2);
	lcd_i2c_set_cursor(&(ctrl->lcd), 20-4, 0);
	if (state == CPS_Available)
	{
		lcd_i2c_char(&(ctrl->lcd), 6);
		lcd_i2c_printf(&(ctrl->lcd), "   ");
	}
	else if (state == CPS_Preparing)
		lcd_i2c_char(&(ctrl->lcd), 3);
	else if (state == CPS_Charging)
		lcd_i2c_char(&(ctrl->lcd), 4);
	else if (state == CPS_SuspendedEV)
	{
		lcd_i2c_char(&(ctrl->lcd), 5);
		lcd_i2c_printf(&(ctrl->lcd), "   ");
	}
	else if (state == CPS_SuspendedEVSE)
		lcd_i2c_char(&(ctrl->lcd), 5);
	else
		lcd_i2c_char(&(ctrl->lcd), 'x');
}


#endif /* __CONTROLLER_LCD_H__ */

