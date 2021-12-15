#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include "OCPP.h"

void
ocpp_heartbeat_req
(
	OCPP *ocpp
);

void
ocpp_heartbeat_conf
(
	OCPP *ocpp
);

#endif /* HEARTBEAT_H_ */
