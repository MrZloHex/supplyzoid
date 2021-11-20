#include "OCPP.h"




void
ocpp_init()
{

}


void
ocpp_update
(
    EVSE *evse
)
{
    static char buffer[256];
    static size index;

    char ch = (char) getc(stdin);
    if (ch != '\n')
        buffer[index++] = ch;
    
    if (check_json(buffer, index))
    {
        buffer[index] = '\0';
        printf("NEW MESSAGE: `%s`\n", buffer);

        OCPPMessage *mes = (OCPPMessage *) malloc(sizeof(OCPPMessage));
        determine_message(buffer, index, mes);

        memsett(buffer, 0, index);
        index = 0;
        

        
    }

}

