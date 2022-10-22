#ifndef __OCPP_STATES_H__
#define __OCPP_STATES_H__

typedef enum OCPPCallAction_E
{
    ACT_BOOT_NOTIFICATION        = 1U,
    ACT_REMOTE_START_TRANSACTION = 2U,
    ACT_REMOTE_STOP_TRANSACTION  = 3U,
    ACT_START_TRANSACTION        = 4U,
    ACT_STOP_TRANSACTION         = 5U,
    ACT_STATUS_NOTIFICATION      = 6U,
    ACT_METER_VALUES             = 7U,
    ACT_HEARTBEAT                = 8U,
    ACT_DATA_TRANSFER        	 = 9U,
    ACT_RESET                    = 10U
} OCPPCallAction;


typedef enum OCPPCallErrorCode_E
{
    NOT_IMPLEMENTED                = 1U,
    NOT_SUPPORTED                  = 2U,
    INTERNAL_ERROR                 = 3U,
    PROTOCOL_ERROR                 = 4U,
    SECURITY_ERROR                 = 5U,
    FORMATION_VIOLATION            = 6U,
    PROPERTY_CONSTRAINT_VIOLATION  = 7U,
    OCCURENCE_CONSTRAINT_VIOLATION = 8U,
    TYPE_CONSTRAINT_VIOLATION      = 9U,
    GENERIC_ERROR                  = 10U,
} OCPPCallErrorCode;


// REGISTRATION STATUS
#define REG_ACCEPTED 1
#define REG_PENDING  2
#define REG_REJECTED 3

// REMOTE START STOP STATUS
#define RSS_ACCEPTED 1
#define RSS_REJECTED 2

#endif /* __OCPP_STATES_H__ */