#ifndef EVSE_H_

#define EVSE_H_

#include "stdtypes.h"
#include "OCPPTypes.h"

#include <time.h>
#include <stdio.h>

#define S_AVAILABLE      1
#define S_PREPARING      2
#define S_CHARGING       3
#define S_SUSPENDED_EV   4
#define S_SUSPENDED_EVSE 5
#define S_FINISHING      6
#define S_RESERVED       7
#define S_UNAVAILABLE    8
#define S_FAULTED        9
typedef unsigned char EVSEState;

typedef struct
{
	// NEEDFUL STATES FOR `CORE` IMPLEMENTATION OF OCPPM 1.6
	// const   bool AuthorizeRemoteTxRequests;
	//         int  ClockAlignedDataInterval;
	//         int  ConnectionTimeOut;
	//              ConnectorPhaseRotation;
	// const   int  GetConfigurationMaxKeys;
	//         int  HeartbeatInterval;
	//         bool LocalAuthorizeOffline;
	//         bool LocalPreAuthorize;
	//              MeterValuesAlignedData;
	//              MeterValuesSampledData;
	//         int  MeterValueSampleInterval;
	// const   int  NumberOfConnectors;
	//         int  ResetRetries;
	//         bool StopTransactionOnEVSideDisconnect;
	//         bool StopTransactionOnInvalidId;
	//              StopTxnAlignedData;
	//              StopTxnSampledData;
	// const        SupportedFeatureProfiles;
	// const   int  SupportedFeatureProfilesMaxLength;
	//         int  TransactionMessageAttempts;
	//         int  TransactionMessageRetryInterval;
	//         bool UnlockConnectorOnEVSideDisconnect;

	EVSEState state;


	unsigned long heartbeat_time;
	time_t last_ping;
	bool booted;
	
} EVSE;


void
evse_init
(
	EVSE *evse
);

void
evse_update
(
	EVSE *evse,
	OCPP *ocpp
);

#endif /* EVSE_H_ */