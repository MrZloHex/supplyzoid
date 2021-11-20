#include "OCPPParser.h"


void
make_call
(
    OCPPCall call,
    char    *dest
)
{
    char buf[512] = "[";
    buf[1] = CALL + '0';
    strcpyy(buf+2, ",\"");
    strlenn(buf);

    char m_id[36];
    size size_id = int_to_charset(call.messageID, m_id, 0);
    strcpyy(buf+4, m_id);
    strcpyy(buf+4+size_id, "\",\"");

    size size_act = strlenn(call.action);
    strcpyy(buf+7+size_id, call.action);
    strcpyy(buf+7+size_id+size_act, "\",");

    size size_payload = strlenn(call.payload);
    strcpyy(buf+9+size_id+size_act, call.payload);
    strcpyy(buf+9+size_id+size_act+size_payload, "]\0");

    strcpyy(dest, buf);
}

void
make_call_result
(
    OCPPCallResult call_res,
    char          *dest
)
{
    char buf[512] = "[";
    buf[1] = CALLRESULT + '0';
    strcpyy(buf+2, ",\"");
    strlenn(buf);

    char m_id[36];
    size size_id = int_to_charset(call_res.messageID, m_id, 0);
    strcpyy(buf+4, m_id);
    strcpyy(buf+4+size_id, "\",\"");

    size size_payload = strlenn(call_res.payload);
    strcpyy(buf+7+size_id, call_res.payload);
    strcpyy(buf+7+size_id+size_payload, "]\0");

    strcpyy(dest, buf);
}

void
make_call_error
(
    OCPPCallError call_err,
    char         *dest
)
{
    char buf[512] = "[";
    buf[1] = CALLRESULT + '0';
    strcpyy(buf+2, ",\"");
    strlenn(buf);

    char m_id[36];
    size size_id = int_to_charset(call_err.messageID, m_id, 0);
    strcpyy(buf+4, m_id);
    strcpyy(buf+4+size_id, "\",\"");

    size size_err_code = strlenn(call_err.error_code);
    strcpyy(buf+7+size_id, call_err.error_code);
    strcpyy(buf+7+size_id+size_err_code, "\",\"");

    size size_err_dscr = strlenn(call_err.error_dscr);
    strcpyy(buf+10+size_id, call_err.error_dscr);
    strcpyy(buf+10+size_id+size_err_code+size_err_dscr, "\",");

    size size_err_det = strlenn(call_err.error_details);
    strcpyy(buf+12+size_id, call_err.error_details);
    strcpyy(buf+12+size_id+size_err_code+size_err_dscr+size_err_det, "]\0");

    strcpyy(dest, buf);
}

OCPPType
determine_message
(
    const char  *_resp,
    const size   length,
    OCPPMessage *message
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

    for (unsigned int i = 0; i < length; ++i)
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
