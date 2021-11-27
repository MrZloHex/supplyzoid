#include "requests/remote_start_transaction.h"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	evse->state = S_PREPARING;
}
