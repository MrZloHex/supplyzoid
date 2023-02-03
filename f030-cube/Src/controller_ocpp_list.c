#include "controller_ocpp_list.h"

#include "string.h"


void
ocpp_authlist_init(OCPP_AuthorizationList *list)
{
    list->capacity = 10;
    list->curr_tok = 0;
        #warning UB: not checking ERROR MALLOC
    list->tokens = (OCPP_IdToken *)malloc(sizeof(OCPP_IdToken) * list->capacity);
}

void
ocpp_authlist_add_token
(
    OCPP_AuthorizationList *list,
    OCPP_IdToken            token
)
{
    if (list->curr_tok == list->capacity)
    {
        list->capacity += 1;
        #warning UB: not checking ERROR REALLOC
        list->tokens = (OCPP_IdToken *)realloc(list->tokens, sizeof(OCPP_IdToken) * list->capacity);
    }

    list->tokens[list->curr_tok++] = token;
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