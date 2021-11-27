#ifndef OCPP_STATES_H_

#define OCPP_STATES_H_

// ACTIONS
#define BOOT_NOTIFICATION        1
#define REMOTE_START_TRANSACTION 2
#define REMOTE_STOP_TRANSACTION  3
#define START_TRANSACTION        4
#define STOP_TRANSACTION         5
#define STATUS_NOTIFICATION      6


// ERROR CODES
#define NOT_IMPLEMENTED                1
#define NOT_SUPPORTED                  2
#define INTERNAL_ERROR                 3
#define PROTOCOL_ERROR                 4
#define SECURITY_ERROR                 5
#define FORMATION_VIOLATION            6
#define PROPERTY_CONSTRAINT_VIOLATION  7
#define OCCURENCE_CONSTRAINT_VIOLATION 8
#define TYPE_CONSTRAINT_VIOLATION      9
#define GENERIC_ERROR                  10


// REGISTRATION STATUS
#define ACCEPTED 1
#define PENDING  2
#define REJECTED 3

#endif /* OCPP_STATES_H_ */