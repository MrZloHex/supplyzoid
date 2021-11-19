#include "OCPP.h"

// void
// handle_call
// (
//     OCPPConfig *_cfg,
//     const char *_call
// )
// {
//     // printf("NEW CALL REQUEST: `%s`\n", _call);
//     OCPPCall call = parse_call(_call);

//     return 0;
// }


// OCPPCall
// parse_call
// (
//     const char *_call
// )
// {
//     OCPPCall call;

//     expected_data field = MESSAGE_TYPE_ID;

//     int      message_type_id;
//     char    message_id[36];
//     char    action[100];
//     char    *json = (char*) malloc(sizeof(char)*1024);

//     char    *buffer = (char *) malloc(sizeof(char)*1024);
//     unsigned int index_buf = 0;


//     int data_in = 0;
//     int block = 0;
//     bool  json_started = false;


//     for (unsigned int i = 0; i < strlen(_call); ++i)
//     {
//         char ch = _call[i];

//         if (ch == ',')
//             next_data_field(&field);


//         if (data_in < 0)
//         {
//             // fprintf(stderr, "INVALID REQUEST\n");
//             // exit(1);
//         }
//         else if (data_in > 0)
//         {   
//             if (field != PAYLOAD && ch == ',')
//                 {}
//             else
//                 buffer[index_buf++] = ch;
            
//             if (field == MESSAGE_TYPE_ID)
//             {
//                 message_type_id = buffer[index_buf-1] - '0';
//                 if (message_type_id != CALL)
//                 {
//                     // fprintf(stderr, "INVALID MESSAGE TYPE %c\n", message_type_id + '0');
//                     exit(1);
//                 }
//                 index_buf = 0;
//                 memset(buffer, 0, 1024);

//                 // printf("MESSAGE TYPE ID: `%d`\n", message_type_id);
//             }
//             else if (field == MESSAGE_ID)
//             {
//                 if (ch == '"')
//                     ++block;

//                 if (block == 2)
//                 {
//                     buffer[index_buf-1] = '\0';
//                     // strcpy(message_id, buffer+1);
//                     index_buf = 0;
//                     memset(buffer, 0, 1024);
//                     block = 0;

//                     // printf("MESSAGE ID: `%s`\n", message_id);
//                 }
//             }
//             else if (field == ACTION)
//             {
//                 if (ch == '"')
//                     ++block;

//                 if (block == 2)
//                 {
//                     buffer[index_buf-1] = '\0';
//                     // strcpy(action, buffer+1);
//                     index_buf = 0;
//                     memset(buffer, 0, 1024);
//                     block = 0;

//                     // printf("ACTION: `%s`\n", action);
//                 }
//             }
//             else if(field == PAYLOAD)
//             {
//                 if      (ch == '{')
//                 {
//                     ++block;
//                     json_started = true;
//                 }
//                 else if (ch == '}')
//                 {
//                     --block;
//                 }

//                 if (block == 0 && json_started)
//                 {
//                     buffer[index_buf-1] = '\0';
//                     // strcpy(action, buffer);
//                     index_buf = 0;
//                     // memset(buffer, 0, 1024);
//                     block = 0;

//                     // printf("PAYLOAD: `%s`\n", action);
//                 }
//             }
//         }

//         switch (ch)
//         {
//             case '[':
//                 data_in++;
//                 break;
//             case ']':
//                 data_in--;
//                 break;
//             default:
                
//                 break;
//         }
        
//     }

//     return call;
// }




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


    printf("NEW RESPONSE: `%s`\n", _resp);


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
                        memset(buffer, 0, 1024);
                        block = 0;
                        ready = 1;

                        printf("RESULT PAYLOAD: `%s`\n", json);
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
                        memset(buffer, 0, 1024);
                        block = 0;
    
                        printf("ERROR CODE: `%s`\n", message_id);
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
                        memset(buffer, 0, 1024);
                        block = 0;
    
                        printf("ERROR DESCRIPTION: `%s`\n", message_id);
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
                        memset(buffer, 0, 1024);
                        block = 0;
                        ready = 1;

                        printf("ERROR DETAILS: `%s`\n", json);
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
                    strcpyy(message_id, buffer+1);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    printf("MESSAGE ID: `%s`\n", message_id);
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