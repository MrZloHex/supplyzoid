#include "OCPP.h"

result
handle_call
(
    OCPPConfig *_cfg,
    const char *_call
)
{
    printf("NEW CALL REQUEST: `%s`\n", _call);
    OCPPCall call = parse_call(_call);

    return 0;
}


OCPPCall
parse_call
(
    const char *_call
)
{
    OCPPCall call;

    uint8_t field = MESSAGE_TYPE_ID;

    uint8_t message_type_id;
    char    message_id[36];
    char    action[100];
    char    *json = (char*) malloc(sizeof(char)*1024);

    char    *buffer = (char *) malloc(sizeof(char)*1024);
    size_t  index_buf = 0;


    uint8_t data_in = 0;


    for (size_t i = 0; i < strlen(_call); ++i)
    {
        char ch = _call[i];
        switch (ch)
        {
            case '[':
                data_in++;
                break;
            case ']':
                data_in--;
                break;
            default:
                buffer[index_buf++] = ch;
                break;
        }
        if (data_in < 0)
        {
            fprintf(stderr, "INVALID REQUEST\n");
            exit(1);
        }
        else if (data_in == 1)
        {
            if (field == MESSAGE_TYPE_ID)
            {
                
            }
        }
    }

    return call;
}