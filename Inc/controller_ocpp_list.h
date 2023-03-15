#ifndef __CONTROLLER_OCPP_LIST_H__
#define __CONTROLLER_OCPP_LIST_H__

#include "stdlib.h"
#include "fram.h"

#define FRAM_BASE_ADDR_LEN 0
#define FRAM_BASE_ADDR_TOK 2
#define FRAM_TOK_SIZE sizeof(OCPP_IdToken)
#define FRAM_MAX_TOK  (2048-FRAM_BASE_ADDR_TOK)/FRAM_TOK_SIZE // FOR 2K FRAM 
#define FRAM_TOK_TAGADDR(__INDEX__) FRAM_BASE_ADDR_TOK+sizeof(OCPP_IdTagInfoStatus) + FRAM_TOK_SIZE*__INDEX__
#define FRAM_TOK_INFOADDR(__INDEX__) FRAM_BASE_ADDR_TOK + FRAM_TOK_SIZE*__INDEX__

typedef enum OCPP_IdTagInfoStatus_E
{
    ITIS_Accepted       = 0x0U,
    ITIS_Blocked        = 0x1U,
    ITIS_Expired        = 0x2U,
    ITIS_Invalid        = 0x3U,
    ITIS_ConcurrentTx   = 0x4U,
    ITIS_NONE           = 0x5U
} OCPP_IdTagInfoStatus;

typedef struct OCPP_IdTagInfo_S
{
    OCPP_IdTagInfoStatus status;
} OCPP_IdTagInfo;

typedef struct OCPP_IdToken_S
{
    char idTag[21];
    OCPP_IdTagInfo idTagInfo;
} OCPP_IdToken;

HAL_StatusTypeDef
_store_id_token(FRAM *fram, OCPP_IdToken token, size_t shift);

typedef struct AuthorizationList_S
{
    size_t version;
    FRAM    fram;
} OCPP_AuthorizationList;

void
ocpp_authlist_init
(
    OCPP_AuthorizationList *list,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 		  wp_pin
);

void
ocpp_authlist_add_token
(
    OCPP_AuthorizationList *list,
    OCPP_IdToken            token
);

void
ocpp_authlist_set_version
(
    OCPP_AuthorizationList *list,
    size_t                  version
);

OCPP_IdTagInfoStatus
ocpp_get_idtaginfostatus(const char *s);

#endif /* __CONTROLLER_OCPP_LIST_H__ */