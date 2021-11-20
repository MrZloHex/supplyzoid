#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"

#include "OCPPParser.h"
#include "OCPPTypes.h"




// typedef unsigned char result;

// OCPP void
// handle_call
// (
//     OCPPConfig *_cfg,
//     const char *_call
// );


// OCPP OCPPCall
// parse_call
// (
//     const char *_call
// );



void
ocpp_init();

void
ocpp_update
(
    EVSE *evse
);


#endif /* OCPP_H_ */