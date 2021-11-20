#include "OCPP.h"




void
ocpp_init()
{

}


void
ocpp_update
(
    OCPP *ocpp,
    EVSE *evse
)
{
    static char buffer[256];
    static size index;

    char ch = (char) getc(stdin);
    if (ch != '\n')
        buffer[index++] = ch;
    
    

    if (determine_message(buffer, index, &(ocpp->now)) != ERROR)
    {
        buffer[index] = '\0';
        // printf("NEW MESSAGE: `%n", buffer);

        memsett(buffer, 0, index);
        index = 0;
    }
    printf("\n\n");

}

