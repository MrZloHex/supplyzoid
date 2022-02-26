#ifndef RAPI_RAPI_ADD_ON_H_
#define RAPI_RAPI_ADD_ON_H_

#include "RAPI_Types.h"

bool
rapi_is_msg_correct(RAPI *rapi);

void
rapi_append_chksum(RAPI *rapi);

#endif /* RAPI_RAPI_ADD_ON_H_ */