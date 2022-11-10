#ifndef AUTHORIZE_H_
#define AUTHORIZE_H_

#include "OCPP.h"

void
ocpp_authorize_req
(
	OCPP *ocpp,
	OCPP_IdTag id_tag
);

void
ocpp_authorize_conf
(
	OCPP *ocpp
);

#endif /* AUTHORIZE_H_ */