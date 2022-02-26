#include "rapi_msg/print_text_lcd.h"

void
rapi_print_text_lcd_req
(
	RAPI *rapi,
	u8 x,
	u8 y,
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

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_print_text_lcd(RAPI *rapi)
{

}

