#include "OCPP.h"

void
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

    expected_data field = MESSAGE_TYPE_ID;

    uint8_t message_type_id;
    char    message_id[36];
    char    action[100];
    char    *json = (char*) malloc(sizeof(char)*1024);

    char    *buffer = (char *) malloc(sizeof(char)*1024);
    size_t  index_buf = 0;


    uint8_t data_in = 0;
    uint8_t block = 0;
    bool  json_started = false;


    for (size_t i = 0; i < strlen(_call); ++i)
    {
        char ch = _call[i];

        if (ch == ',')
        {
            next_data_field(&field);
            continue;
        }

        if (data_in < 0)
        {
            fprintf(stderr, "INVALID REQUEST\n");
            exit(1);
        }
        else if (data_in > 0)
        {
            buffer[index_buf++] = ch;
            if (field == MESSAGE_TYPE_ID)
            {
                message_type_id = buffer[index_buf-1] - '0';
                if (message_type_id != CALL)
                {
                    fprintf(stderr, "INVALID MESSAGE TYPE %c\n", message_type_id + '0');
                    exit(1);
                }
                index_buf = 0;
                memset(buffer, 0, 1024);

                printf("MESSAGE TYPE ID: `%d`\n", message_type_id);
            }
            else if (field == MESSAGE_ID)
            {
                if (ch == '"')
                    ++block;

                if (block == 2)
                {
                    buffer[index_buf-1] = '\0';
                    strcpy(message_id, buffer+1);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    printf("MESSAGE ID: `%s`\n", message_id);
                }
            }
            else if (field == ACTION)
            {
                if (ch == '"')
                    ++block;

                if (block == 2)
                {
                    buffer[index_buf-1] = '\0';
                    strcpy(action, buffer+1);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    printf("ACTION: `%s`\n", action);
                }
            }
            else if(field == PAYLOAD)
            {
                if      (ch == '{')
                {
                    ++block;
                    json_started = true;
                }
                else if (ch == '}')
                {
                    --block;
                }

                if (block == 0 && json_started)
                {
                    buffer[index_buf-1] = '\0';
                    strcpy(action, buffer);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    printf("PAYLOAD: `%s`\n", action);
                }
            }
        }

        switch (ch)
        {
            case '[':
                data_in++;
                break;
            case ']':
                data_in--;
                break;
            default:
                
                break;
        }
        
    }

    return call;
}






void
next_data_field(expected_data *data)
{
    if (*data != PAYLOAD)
        *data += 1;
    else
        *data = PAYLOAD;
}