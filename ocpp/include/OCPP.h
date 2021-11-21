#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"

#include "OCPPTypes.h"


typedef struct
{
    OCPPMessage now;
    OCPPMessage last;
} OCPP;


void
ocpp_init(OCPP *ocpp);

void
ocpp_free(OCPP *ocpp);

void
ocpp_update
(
    OCPP *ocpp,
    EVSE *evse
);


#endif /* OCPP_H_ */