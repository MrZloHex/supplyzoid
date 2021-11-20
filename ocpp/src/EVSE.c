#include "EVSE.h"


void
evse_init
(
    EVSE *evse
)
{
    evse->state = S_AVAILABLE;
}
