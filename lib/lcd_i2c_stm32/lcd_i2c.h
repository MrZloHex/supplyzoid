/**
  ******************************************************************************
  * @file           : lcd_i2c.h
  * @author         : MrZloHex
  * @brief          : Library for liquid crystal displays with IIC expnder
  ******************************************************************************
  */

#ifndef __LCD_I2C_H__
#define __LCD_I2C_H__

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


#define I2CLCD_RS_POS			(0)
#define I2CLCD_RW_POS			(1)
#define I2CLCD_E_POS			(2)
#define I2CLCD_BL_POS			(3)


#define I2CLCD_RS				(1 << I2CLCD_RS_POS)
#define I2CLCD_RW				(1 << I2CLCD_RW_POS)
#define I2CLCD_E				(1 << I2CLCD_E_POS)
#define I2CLCD_BL				(1 << I2CLCD_BL_POS)

// Defines for command access
#define CLR_DISPLAY				(1<<0)
#define RET_HOME				(1<<1)
#define MODE_SET				(1<<2)
#define DISP_CTRL				(1<<3)
#define CD_SHIFT				(1<<4)
#define FUNC_SET				(1<<5)
#define CGRAM_ADDR				(1<<6)
#define DDRAM_ADDR				(1<<7)

#define MODE_SET_INCR			(1<<1)
#define MODE_SET_DECR			(0)
#define MODE_SET_DISP_SHIFT_ON	(1<<0)
#define MODE_SET_DISP_SHIFT_OFF	(0)

#define SHIFT					(1<<0)
#define DIR_INCR_DECR			(1<<1)

#define DISP_CTRL_BLINK_ON		(1<<0)
#define DISP_CTRL_BLINK_OFF		(0)

#define DISP_CTRL_CURSOR_ON		(1<<1)
#define DISP_CTRL_CURSOR_OFF	(0)

#define DISP_CTRL_DISPLAY_ON	(1<<2)
#define DISP_CTRL_DISPLAY_OFF	(0)

#define CD_SHIFT_DSIPLAY		(1<<3)
#define CD_SHIFT_CURSOR			(0)

#define CD_SHIFT_RIGHT			(1<<2)
#define CD_SHIFT_LEFT			(1<<2)

#define FUNC_SET_FO_5X10		(1<<2)
#define FUNC_SET_FO_5X8			0
#define FUNC_SET_LINES_2		(1<<3)
#define FUNC_SET_LINES_1		0
#define FUNC_SET_DLEN_8B		(1<<4)
#define FUNC_SET_DLEN_4B		0


// SendByte options for physical access
// opts[0]	0: Send Command (RS = 0)
//			1: Send Data (RS = 1)
// opts[1]	0: Send 8 bits (2 by 4bits)
// opts[1]	1: Send 4 bits
// opts[2]	0: Backlight On
// opts[2]	1: Backlight Off

#define I2CLCD_OPTS_RS			(1<<0)
#define I2CLCD_OPTS_DATA		I2CLCD_OPTS_RS
#define I2CLCD_OPTS_COMMAND		(0)

#define I2CLCD_OPTS_INIT		(1<<1)
#define I2CLCD_OPTS_NOINIT		(0)
#define I2CLCD_OPTS_4B			I2CLCD_OPTS_INIT
#define I2CLCD_OPTS_8B			I2CLCD_OPTS_NOINIT

#define I2CLCD_OPTS_WAIT_BF		(1<<3)

#define I2CLCD_MAX_BF_POLLS		127

#define DIR_INCR DIR_INCR_DECR & 0xFF

#define I2CLCD_CGRAM_ADDR_POS	(3)


typedef enum LCD_Size_E
{
	LCD_16x2   = 0x2U,
	LCD_16x4   = 0x3U,
	LCD_20x2   = 0x4U,
	LCD_20x4   = 0x5U,
	LCD_40x2   = 0x6U
} LCD_Size;

typedef struct LCD_I2C_S
{
	I2C_HandleTypeDef *hi2c;
	uint8_t 	address;
	uint32_t 	timeout;
	LCD_Size 	size;

	uint8_t entry_mode_set;
	uint8_t diplay_ctrl;
	uint8_t cursor_display_shift;
	uint8_t function_set;
	uint8_t	cgram_addr;
	uint8_t ddram_addr;
	uint8_t blacklight;
} LCD_I2C;

/**
 * @brief Initilize LCD for an IIC bus (e.g. lcd_i2c_init(&lcd, &hi2c1, 0x4E, LCD_16x2, 10);)
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param i2c		IIC handler
 * @param address	Address of IIC expander connected to LCD (e.g. 0x4E for PCF8574)
 * @param size		Size of LCD (see LCD_Size enumeration)
 * @param timeout	Timeout value of blocking function of IIC transmit
 */
void
lcd_i2c_init
(
	LCD_I2C *lcd,
	I2C_HandleTypeDef *i2c,
	uint8_t  address,
	LCD_Size size,
	uint32_t timeout
);

/**
 * @brief Set cursor to position
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param x			Column
 * @param y			Row
 */
void
lcd_i2c_set_cursor(LCD_I2C *lcd, uint8_t x, uint8_t y);


HAL_StatusTypeDef
lcd_i2c_char(LCD_I2C *lcd, uint8_t chr);

/**
 * @brief Print string to set position
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param str		String to print
 */
void
lcd_i2c_print(LCD_I2C *lcd, const char *str);

/**
 * @brief Formatted print LCD
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param fmt		Format string
 * @param ...		va_list of formats
 */
void
lcd_i2c_printf
(
	LCD_I2C *lcd,
	const char * restrict fmt,
	...
);

/**
 * @brief Formattedd print to postition
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param x			Column
 * @param y			Row
 * @param fmt		Format string
 * @param ...		va_list of formats
 */
void
lcd_i2c_pos_printf
(
	LCD_I2C *lcd,
	uint8_t x,
	uint8_t y,
	const char * restrict fmt,
	...
);

/**
 * @brief Clear LCD
 *
 * @param lcd		Pointer to LCD_I2C struct
 */
void
lcd_i2c_clear(LCD_I2C *lcd);

HAL_StatusTypeDef
lcd_i2c_blink(LCD_I2C* lcd, uint8_t blink_en);

HAL_StatusTypeDef
lcd_i2c_cursor(LCD_I2C *lcd, uint8_t cursor_en);

HAL_StatusTypeDef
lcd_i2c_home(LCD_I2C *lcd);

HAL_StatusTypeDef
lcd_i2c_set_abs_cursor(LCD_I2C *lcd, uint8_t pos);

HAL_StatusTypeDef
lcd_i2c_get_abs_cursor(LCD_I2C *lcd, uint8_t *pos);

HAL_StatusTypeDef
lcd_i2c_make_custom_char(LCD_I2C *lcd, uint8_t cgaddr, uint8_t *charmap);


HAL_StatusTypeDef
_lcd_i2c_cmd(LCD_I2C *lcd, uint8_t cmd);

HAL_StatusTypeDef
_lcd_i2c_cmd_4bit(LCD_I2C *lcd, uint8_t cmd);

HAL_StatusTypeDef
_lcd_i2c_data(LCD_I2C *lcd, uint8_t data, uint8_t opts);

HAL_StatusTypeDef
_lcd_i2c_get_data(LCD_I2C *lcd, uint8_t *data);

#endif /* __LCD_I2C_H__ */

