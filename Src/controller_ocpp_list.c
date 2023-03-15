#include "controller_ocpp_list.h"

#include "string.h"


void
ocpp_authlist_init
(
    OCPP_AuthorizationList *list,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 		  wp_pin
)
{
    fram_init(&(list->fram), i2c, 0xA0, wp_gpio, wp_pin, MB85RC16, 10);
    fram_write_byte(&(list->fram), FRAM_BASE_ADDR_LEN, 0);
}

void
ocpp_authlist_add_token
(
    OCPP_AuthorizationList *list,
    OCPP_IdToken            token
)
{
    uint8_t len;
    fram_read_byte(&(list->fram), FRAM_BASE_ADDR_LEN, &len);
    if (len == FRAM_MAX_TOK)
        return;

    _store_id_token(&(list->fram), token, len);
    fram_write_byte(&(list->fram), FRAM_BASE_ADDR_LEN, len+1);
}

void
ocpp_authlist_set_version
(
    OCPP_AuthorizationList *list,
    size_t                  version
)
{
    list->version = version;
}


OCPP_IdTagInfoStatus
ocpp_get_idtaginfostatus(const char *s)
{
    if      (strcmp(s, "Accepted"    ) == 0)    { return ITIS_Accepted;     }
    else if (strcmp(s, "Blocked"     ) == 0)    { return ITIS_Blocked;      }
    else if (strcmp(s, "Expired"     ) == 0)    { return ITIS_Expired;      }
    else if (strcmp(s, "Invalid"     ) == 0)    { return ITIS_Invalid;      }
    else if (strcmp(s, "ConcurrentTx") == 0)    { return ITIS_ConcurrentTx; }
    else                                        { return ITIS_NONE;         }
}

HAL_StatusTypeDef
_store_id_token(FRAM *fram, OCPP_IdToken token, size_t shift)
{
    HAL_StatusTypeDef r = fram_write_array(fram, FRAM_TOK_TAGADDR(shift), (uint8_t *)token.idTag, 21);
    if (r != HAL_OK)
        return r;

    return fram_write_byte(fram, FRAM_TOK_INFOADDR(shift), token.idTagInfo.status);
}

HAL_StatusTypeDef
_load_id_token(FRAM *fram, OCPP_IdToken *token, size_t shift)
{
    HAL_StatusTypeDef r = fram_read_array(fram, FRAM_TOK_TAGADDR(shift), (uint8_t *)token->idTag, 21);
    if (r != HAL_OK)
        return r;

    return fram_read_byte(fram, FRAM_TOK_INFOADDR(shift), &(token->idTagInfo.status));
}