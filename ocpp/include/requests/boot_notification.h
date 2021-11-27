#ifndef BOOT_NOTIFICATION_H_

#define BOOT_NOTIFICATION_H_

#include "stdtypes.h"
#include "EVSE.h"
#include "OCPP.h"

#include <stdio.h>

void
ocpp_boot_notification_req
(
	OCPP *ocpp,
	char req[REQ_LEN]
);

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	EVSE *evse
);

#endif /* BOOT_NOTIFICATION_H_ */