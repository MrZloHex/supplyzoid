#include "requests/remote_start_transaction.h"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	printf("REMOTE START TRANSACTION\n");
	if (evse->is_transaction)
	{
		// REJECTED
		ocpp_send_resp(ocpp, );
	}	
	else
	{
		// ACCEPTED
	}
}

void
ocpp_remote_start_transaction_conf
(
	OCPP *ocpp
)
{
	
}
