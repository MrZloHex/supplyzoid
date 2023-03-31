/**
  ******************************************************************************
  * @file           : lcd_i2c.c
  * @author         : MrZloHex
  * @brief          : Library for liquid crystal displays with IIC expnder
  ******************************************************************************
  */

#include "lcd_i2c.h"
#include "stdio.h"
#include "stdarg.h"

void
lcd_i2c_init
(
	LCD_I2C *lcd,
	I2C_HandleTypeDef *i2c,
	uint8_t  address,
	LCD_Size size,
	uint32_t timeout
)
{
	lcd->hi2c    = i2c;
	lcd->address = address;
	lcd->timeout = timeout;
	lcd->size    = size;

	lcd->function_set = FUNC_SET | FUNC_SET_DLEN_8B;
	lcd->blacklight = I2CLCD_BL;
	lcd->entry_mode_set = MODE_SET ;
	lcd->cursor_display_shift = CD_SHIFT;

	uint8_t ret;
	ret = 0;

	// As per HD44780, if reset timing cannot be generated, initilization should be a sequence
	// of 0x3 writes with specific delays afterwards

	ret |= _lcd_i2c_cmd_4bit(lcd, lcd->function_set);
	HAL_Delay(4);
	ret |= _lcd_i2c_cmd_4bit(lcd, lcd->function_set);
	ret |= _lcd_i2c_cmd_4bit(lcd, lcd->function_set);

	lcd->function_set = FUNC_SET | FUNC_SET_DLEN_4B;
	ret |= _lcd_i2c_cmd_4bit(lcd, lcd->function_set);

	lcd->function_set = FUNC_SET | FUNC_SET_DLEN_4B | FUNC_SET_LINES_2 | FUNC_SET_FO_5X8;
	ret |= _lcd_i2c_cmd(lcd, lcd->function_set);

	lcd->diplay_ctrl = DISP_CTRL | DISP_CTRL_CURSOR_OFF | DISP_CTRL_BLINK_OFF | DISP_CTRL_DISPLAY_ON;
	ret |= _lcd_i2c_cmd(lcd, lcd->diplay_ctrl);

	ret |= _lcd_i2c_cmd(lcd, lcd->entry_mode_set);

	lcd_i2c_clear(lcd);

	HAL_Delay(5);
}

void
lcd_i2c_set_cursor(LCD_I2C *lcd, uint8_t x, uint8_t y)
{
	const static uint8_t k_lcd_16_y[4] = {0x00, 0x40, 0x10, 0x50};
	const static uint8_t k_lcd_20_y[4] = {0x00, 0x40, 0x14, 0x54};

	if (y >= 4)
		return;

	uint8_t address = 0x80;
	if (lcd->size >= LCD_16x2 && lcd->size <= LCD_16x4)
	{
		address |= k_lcd_16_y[y] + x;
	}
	else
	{
		address |= k_lcd_20_y[y] + x;
	}

	_lcd_i2c_cmd(lcd, address);
}

void
lcd_i2c_print(LCD_I2C *lcd, const char *str)
{
	while (*str) lcd_i2c_char(lcd, (uint8_t) *str++);
}

void
lcd_i2c_clear(LCD_I2C *lcd)
{
	_lcd_i2c_cmd(lcd, 0x01);
	HAL_Delay(2);
}

HAL_StatusTypeDef
lcd_i2c_blink(LCD_I2C* lcd, uint8_t blink_en)
{
	if (blink_en) lcd->diplay_ctrl |= DISP_CTRL_BLINK_ON;
	else lcd->diplay_ctrl &= ~(DISP_CTRL_BLINK_ON);
	return _lcd_i2c_cmd(lcd, lcd->diplay_ctrl);
}

HAL_StatusTypeDef
lcd_i2c_cursor(LCD_I2C *lcd, uint8_t cursor_en)
{
	if (cursor_en) lcd->diplay_ctrl |= (DISP_CTRL_CURSOR_ON);
	else lcd->diplay_ctrl &= ~(DISP_CTRL_CURSOR_ON);
	return _lcd_i2c_cmd(lcd, lcd->diplay_ctrl);
}

HAL_StatusTypeDef
lcd_i2c_home(LCD_I2C *lcd)
{
	return _lcd_i2c_cmd(lcd, RET_HOME);
}

HAL_StatusTypeDef
lcd_i2c_set_abs_cursor(LCD_I2C *lcd, uint8_t pos)
{
	return _lcd_i2c_data(lcd, DDRAM_ADDR | pos, I2CLCD_OPTS_NOINIT);
}

HAL_StatusTypeDef
lcd_i2c_get_abs_cursor(LCD_I2C *lcd, uint8_t *pos)
{
	HAL_StatusTypeDef res = _lcd_i2c_get_data(lcd, pos);
	*pos = *pos & 0x7F;
	return res;
}

HAL_StatusTypeDef
lcd_i2c_char(LCD_I2C *lcd, uint8_t chr)
{
	return _lcd_i2c_data(lcd, chr, I2CLCD_OPTS_DATA);
}

void
lcd_i2c_printf
(
	LCD_I2C *lcd,
	const char * restrict fmt,
	...
)
{
	char str[41];
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, 41, fmt, args);
	va_end(args);
	lcd_i2c_print(lcd, str);
}


void
lcd_i2c_pos_printf
(
	LCD_I2C *lcd,
	uint8_t x,
	uint8_t y,
	const char * restrict fmt,
	...
)
{
	lcd_i2c_set_cursor(lcd, x, y);
	char str[41];
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, 41, fmt, args);
	va_end(args);;
	lcd_i2c_print(lcd, str);
}

HAL_StatusTypeDef
lcd_i2c_make_custom_char(LCD_I2C *lcd, uint8_t cgaddr, uint8_t *charmap)
{
	HAL_StatusTypeDef hal_stat;
	uint8_t _addr;

	cgaddr = (cgaddr << 3);

	// get the current DDRAM address
	hal_stat = lcd_i2c_get_abs_cursor(lcd, &_addr);

	for (uint8_t i = 0; i < 8; i++)
	{
		lcd->cgram_addr = CGRAM_ADDR | (cgaddr | i);
		hal_stat |= _lcd_i2c_cmd(lcd, lcd->cgram_addr);

		hal_stat |= lcd_i2c_char(lcd, charmap[i]);
	}

	// restore DDRAM address such that further writes are in DDRAM section
	hal_stat |= lcd_i2c_set_abs_cursor(lcd, _addr);

	return hal_stat;
}



HAL_StatusTypeDef
_lcd_i2c_data(LCD_I2C *lcd, uint8_t data, uint8_t opts)
{
	// Opts[0] - R/S select
	// Opts[1] - initialization

	HAL_StatusTypeDef hal_stat;
	uint8_t i2c_frame_data[5];
	uint8_t cmd;
	uint8_t wait_bf;
	uint8_t i2c_frame_size;
	uint8_t n;
	uint8_t lcd_opts;

	// Select between command/data frame
	cmd = opts & I2CLCD_OPTS_RS;

	// Toggle between waiting 1ms or poll Busy Flag
	wait_bf = opts & I2CLCD_OPTS_WAIT_BF;

	// Frame size for I2C communication.
	if (opts & I2CLCD_OPTS_4B)
	{
		i2c_frame_size = 2;
	}
	else
	{
		i2c_frame_size = 4;
	}

	// Add final all 1s on the 4 data bits to be able to read BusyFlag after a transaction
	i2c_frame_size += wait_bf;

	lcd_opts = (I2CLCD_RS & cmd) | (I2CLCD_BL & lcd->blacklight) | (I2CLCD_E);

	i2c_frame_data[0] = (data & 0xF0) | lcd_opts;
	i2c_frame_data[1] = i2c_frame_data[0] & (~I2CLCD_E);

	i2c_frame_data[2] = ((data << 4) & 0xF0) | lcd_opts;
	i2c_frame_data[3] = i2c_frame_data[2] & (~I2CLCD_E);

	// Preferably remove this
	if (wait_bf)
	{
		i2c_frame_data[i2c_frame_size-1] = i2c_frame_data[i2c_frame_size-2] | 0x80;
	}

	// HAL transmits i2c_frame_data[0],[1], ... , i2c_frame_data[i2c_frame_size-1]
	hal_stat = HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, i2c_frame_data, i2c_frame_size, lcd->timeout);

	//i2cLcd_WaitBusyFlag();
	if (wait_bf)
	{
		n = 0;
		do
		{
			hal_stat |= HAL_I2C_Master_Receive(lcd->hi2c, lcd->address, i2c_frame_data, 1, lcd->timeout);
			n++;
		} while ( (n < I2CLCD_MAX_BF_POLLS) && (i2c_frame_data[0] & 0x80) );
	}
	else
	{
		HAL_Delay(1);
	}

	return hal_stat;
}

HAL_StatusTypeDef
_lcd_i2c_get_data(LCD_I2C *lcd, uint8_t *data)
{
	HAL_StatusTypeDef hal_stat;
	uint8_t i2c_frame_data[5];

	uint8_t lcd_opts;

	lcd_opts = (I2CLCD_BL & lcd->blacklight) /*| (I2CLCD_E)*/ | (I2CLCD_RW);

	// first need to write 0xF to data bits of PCF, E and R
	i2c_frame_data[0] = (0xF0) | lcd_opts;
	i2c_frame_data[1] = i2c_frame_data[0] | (I2CLCD_E);

	i2c_frame_data[2] = 0;
	i2c_frame_data[3] = 0;

	// HAL receives i2c_frame_data[0],[1], ... , i2c_frame_data[i2c_frame_size-1]
	hal_stat = HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, i2c_frame_data, 2, lcd->timeout);
	hal_stat |= HAL_I2C_Master_Receive(lcd->hi2c, lcd->address, &i2c_frame_data[2], 1, lcd->timeout);

	hal_stat |= HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, i2c_frame_data, 2, lcd->timeout);
	hal_stat |= HAL_I2C_Master_Receive(lcd->hi2c, lcd->address, &i2c_frame_data[3], 1, lcd->timeout);

	*data = (i2c_frame_data[2] & 0xF0) | ( (i2c_frame_data[3] >> 4) & 0x0F);

	return hal_stat;
}



HAL_StatusTypeDef
_lcd_i2c_cmd(LCD_I2C *lcd, uint8_t cmd)
{
	return _lcd_i2c_data(lcd, cmd, I2CLCD_OPTS_COMMAND | I2CLCD_OPTS_NOINIT);
}

HAL_StatusTypeDef
_lcd_i2c_cmd_4bit(LCD_I2C *lcd, uint8_t cmd)
{
	return _lcd_i2c_data(lcd, cmd, I2CLCD_OPTS_INIT);
}

