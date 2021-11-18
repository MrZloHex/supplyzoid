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

    uint8_t field = 0;

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
                        printf("%c\n", ch);
        switch (ch)
        {
            case '[':
                data_in++;
                field = MESSAGE_TYPE_ID;
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
                message_type_id = buffer[0] - '0';
                printf("%d\n", message_type_id);
                if (message_type_id != 2)
                {
                    fprintf(stderr, "INVALID MESSAGE TYPE ");
                    exit(1);
                }
                field = MESSAGE_ID;
            }
        }
    }

    return call;
}