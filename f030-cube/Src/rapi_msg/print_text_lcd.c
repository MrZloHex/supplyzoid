#include "rapi_msg/print_text_lcd.h"

#include "mjson.h"

void
rapi_print_text_lcd_req
(
	RAPI *rapi,
	uint8_t x,
	uint8_t y,
	char *text
)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %u %u %s^",
		"FP",
		x, y,
		text
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_print_text_lcd(RAPI *rapi)
{

}

