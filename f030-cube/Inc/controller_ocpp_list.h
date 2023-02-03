#ifndef __CONTROLLER_OCPP_LIST_H__
#define __CONTROLLER_OCPP_LIST_H__

#include "stdlib.h"

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

typedef struct AuthorizationList_S
{
    size_t version;
    OCPP_IdToken *tokens;
    size_t curr_tok;
    size_t  capacity;
} OCPP_AuthorizationList;

void
ocpp_authlist_init(OCPP_AuthorizationList *list);

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