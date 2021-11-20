#ifndef EVSE_H_

#define EVSE_H_

#include <stdbool.h>

#define S_AVAILABLE      0
#define S_PREPARING      1
#define S_CHARGING       2
#define S_SUSPENDED_EV   3
#define S_SUSPENDED_EVSE 4
#define S_FINISHING      5
#define S_RESERVED       6
#define S_UNAVAILABLE    7
#define S_FAULTED        8
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
} EVSE;


void
evse_init
(
    EVSE *evse
);

#endif /* EVSE_H_ */