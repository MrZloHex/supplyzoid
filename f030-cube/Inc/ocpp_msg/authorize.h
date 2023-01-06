#ifndef AUTHORIZE_H_
#define AUTHORIZE_H_

#include "controller_ocpp.h"

void
ocpp_authorize_req
(
	Controller_OCPP *ocpp,
	OCPP_IdTag *id_tag
);

void
ocpp_authorize_conf
(
	Controller_OCPP *ocpp
);

#endif /* AUTHORIZE_H_ */