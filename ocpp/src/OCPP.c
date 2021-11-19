#include "OCPP.h"


void
make_call
(
    OCPPCall call_cfg,
    char    *_dest
)
{
    char buf[512] = "[";
    buf[1] = CALL + '0';
    strcpyy(buf+2, ",\"");
    strlenn(buf);

    char m_id[36];
    int size_id = int_to_charset(call_cfg.messageID, m_id, 0);
    strcpyy(buf+4, m_id);
    strcpyy(buf+4+size_id, "\",\"");

    int size_act = strlenn(call_cfg.action);
    strcpyy(buf+7+size_id, call_cfg.action);
    strcpyy(buf+7+size_id+size_act, "\",");

    int size_payload = strlenn(call_cfg.payload);
    strcpyy(buf+9+size_id+size_act, call_cfg.payload);
    strcpyy(buf+9+size_id+size_act+size_payload, "]\0");


    strcpyy(_dest, buf);
}


int
handle_response
(
    const char   *_resp,
    OCPPResponse *response
)
{
    int result;

    expected_data field = MESSAGE_TYPE_ID;

    OCPPType message_type_id;
    char     message_id[36];
    char     error_code[30];
    char     error_dscr[255];
    char    *json   = (char*) malloc(sizeof(char)*1024);

    char    *buffer = (char*) malloc(sizeof(char)*1024);
    unsigned int index_buf = 0;

    short ready = 0;

    int data_in = 0;
    int block   = 0;
    int json_started = 0;

    for (unsigned int i = 0; i < strlenn(_resp); ++i)
    {
        char ch = _resp[i];

        if (ch == ',')
            next_data_field(&field, message_type_id);


        if (ready == 1)
            break;

        
        if (data_in > 0)
        {   
            if (field != C_PAYLOAD && ch == ',')
                {}
            else
                buffer[index_buf++] = ch;
            
            if (message_type_id == CALLRESULT)
            {
                if(field == R_PAYLOAD)
                {
                    if      (ch == '{')
                    {
                        ++block;
                        json_started = 1;
                    }
                    else if (ch == '}')
                    {
                        --block;
                    }

                    if (block == 0 && json_started)
                    {
                        buffer[index_buf] = '\0';
                        strcpyy(json, buffer);
                        index_buf = 0;
                        memsett(buffer, 0, 1024);
                        block = 0;
                        ready = 1;
                    }
                }
            }
            else if (message_type_id == CALLERROR)
            {
                if (field == ERROR_CODE)
                {
                    if (ch == '"')
                        ++block;
    
                    if (block == 2)
                    {
                        buffer[index_buf-1] = '\0';
                        strcpyy(message_id, buffer+1);
                        index_buf = 0;
                        memsett(buffer, 0, 1024);
                        block = 0;
                    }
                }
                else if (field == ERROR_DSCR)
                {
                    if (ch == '"')
                        ++block;
    
                    if (block == 2)
                    {
                        buffer[index_buf-1] = '\0';
                        strcpyy(message_id, buffer+1);
                        index_buf = 0;
                        memsett(buffer, 0, 1024);
                        block = 0;
                    }
                }
                else if(field == ERROR_DETAILS)
                {
                    if      (ch == '{')
                    {
                        ++block;
                        json_started = 1;
                    }
                    else if (ch == '}')
                    {
                        --block;
                    }

                    if (block == 0 && json_started)
                    {
                        buffer[index_buf] = '\0';
                        strcpyy(json, buffer);
                        index_buf = 0;
                        memsett(buffer, 0, 1024);
                        block = 0;
                        ready = 1;
                    }
                }
            }

            if (field == MESSAGE_TYPE_ID)
            {
                message_type_id = buffer[index_buf-1] - '0';

                if (message_type_id != CALLRESULT && message_type_id != CALLERROR)
                {
                    fprintf(stderr, "INVALID MESSAGE TYPE %c\n", message_type_id + '0');
                    exit(1);
                }

                result = message_type_id;
                index_buf = 0;
                memsett(buffer, 0, 1024);
            }
            else if (field == MESSAGE_ID)
            {
                if (ch == '"')
                    ++block;

                if (block == 2)
                {
                    buffer[index_buf-1] = '\0';
                    strcpyy(message_id, buffer+1);
                    index_buf = 0;
                    memsett(buffer, 0, 1024);
                    block = 0;
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
        if (data_in < 0)
        {
            fprintf(stderr, "INVALID REQUEST\n");
            exit(1);
        }
        
    }

    free(json);
    free(buffer);

    return result;
}



void
next_data_field
(
    expected_data *data,
    OCPPType type
)
{
    if (type == CALL)
    {
        if (*data != C_PAYLOAD)
            *data += 1;
        else
            *data = C_PAYLOAD;
    }
    else if (type == CALLRESULT)
    {
        if (*data != R_PAYLOAD)
            *data += 1;
        else
            *data = R_PAYLOAD;
    }
    else if (type == CALLERROR)
    {
        if (*data != ERROR_DETAILS)
            *data += 1;
        else
            *data = ERROR_DETAILS;
    }
}
