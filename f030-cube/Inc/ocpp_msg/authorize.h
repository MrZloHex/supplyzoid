#ifndef AUTHORIZE_H_
#define AUTHORIZE_H_

#include "OCPP.h"

void
ocpp_aithorize_req
(
	OCPP *ocpp
);

void
ocpp_authorize_conf
(
	OCPP *ocpp
);

#endif /* AUTHORIZE_H_ */