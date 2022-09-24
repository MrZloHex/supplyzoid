#ifndef RAPI_PRINT_TEXT_LCD_H_
#define RAPI_PRINT_TEXT_LCD_H_

#include "RAPI.h"

void
rapi_print_text_lcd_req
(
	RAPI *rapi,
	uint8_t x,
	uint8_t y,
	char *text
);

void
rapi_print_text_lcd_resp(RAPI *rapi);

#endif /* RAPI_PRINT_TEXT_LCD_H_ */
