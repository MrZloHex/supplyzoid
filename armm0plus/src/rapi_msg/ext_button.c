#include "rapi_msg/ext_button.h"

void
rapi_ext_button_req(RAPI *rapi)
{
	usart_rapi_println_str("Extrenal button was pressed");
}

void
rapi_ext_button_resp(RAPI *rapi){

}

