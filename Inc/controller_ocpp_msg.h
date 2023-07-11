/**
  ******************************************************************************
  * @file    controller_ocpp_msg.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_OCPP_MESSAGES_H__
#define __CONTROLLER_OCPP_MESSAGES_H__

#include "controller_ocpp.h"


// CALLS

void
ocpp_authorize_req
(
	Controller_OCPP *ocpp,
	OCPP_IdTag *id_tag
);

void
ocpp_heartbeat_req
(
	Controller_OCPP *ocpp
);

void
ocpp_boot_notification_req(Controller_OCPP *ocpp);

void
ocpp_start_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
);

void
ocpp_status_notification_req
(
    Controller_OCPP *ocpp,
    OCPP_ChargePointStatus *state,
	OCPP_ChargePointErrorCode *error
);

void
ocpp_stop_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh,
	uint32_t *transaction_id
);

void
ocpp_meter_values_req
(
	Controller_OCPP *ocpp,
	double *wh,
	double *amps,
	double *volts
);


// RESPONSES

void
ocpp_remote_start_transaction_conf
(
	Controller_OCPP *ocpp,
	bool *status
);

void
ocpp_remote_stop_transaction_conf
(
	Controller_OCPP *ocpp,
	bool *status
);

void
ocpp_reset_conf
(
	Controller_OCPP *ocpp,
	bool *status
);

void
ocpp_change_availabilty_conf
(
	Controller_OCPP *ocpp,
    OCPP_AvailabilityStatus *status
);

void
ocpp_send_local_list_conf
(
	Controller_OCPP *ocpp,
	OCPP_UpdateStatus *status
);

void
ocpp_get_local_list_version_conf(Controller_OCPP *ocpp);

void
ocpp_data_transfer_conf
(
	Controller_OCPP *ocpp,
	OCPPDataTransferStatus *status,
	char *data
);

#endif /* __CONTROLLER_OCPP_MESSAGES_H__ */