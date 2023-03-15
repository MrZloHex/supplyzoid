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
	"Preparing",
	"Charging",
	"SuspendedEVSE",
	"SuspendedEV",
	"Finishing",
	"Reserved",
	"Unavailable",
	"Faulted"
};

void
_controller_lcd_update(Controller *ctrl)
{
    static OCPP_ChargePointStatus state = 0;
    if (state == ctrl->ocpp.status)
        return;
        
    state = ctrl->ocpp.status;
    lcd_i2c_clear(&(ctrl->lcd));
    lcd_i2c_pos_printf(&(ctrl->lcd), 0, 0, "%s", k_ChargePointStatus[state]);
    lcd_i2c_pos_printf(&(ctrl->lcd), 0, 1, "TOK SIZE: %u", FRAM_TOK_SIZE);
    lcd_i2c_pos_printf(&(ctrl->lcd), 0, 2, "MAX TOK: %u %u", FRAM_MAX_TOK, FRAM_TOK_TAGADDR(1));
    lcd_i2c_pos_printf(&(ctrl->lcd), 0, 3, "E %u P %u", ctrl->e_state, ctrl->p_state);
}


#endif /* __CONTROLLER_LCD_H__ */

