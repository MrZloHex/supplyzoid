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


OCPPResponse
handle_response
(
    const char *_resp
)
{
    OCPPResponse response;

    expected_data field = MESSAGE_TYPE_ID;

    int      message_type_id;
    char    message_id[36];
    char    action[100];
    char    *json = (char*) malloc(sizeof(char)*1024);

    char    *buffer = (char *) malloc(sizeof(char)*1024);
    unsigned int index_buf = 0;


    int data_in = 0;
    int block = 0;
    bool  json_started = false;


    for (unsigned int i = 0; i < strlen(_resp); ++i)
    {
        char ch = _resp[i];

        if (ch == ',')
            next_data_field(&field);


        if (data_in < 0)
        {
            // fprintf(stderr, "INVALID REQUEST\n");
            // exit(1);
        }
        else if (data_in > 0)
        {   
            if (field != PAYLOAD && ch == ',')
                {}
            else
                buffer[index_buf++] = ch;
            
            if (field == MESSAGE_TYPE_ID)
            {
                message_type_id = buffer[index_buf-1] - '0';
                if (message_type_id != CALL)
                {
                    // fprintf(stderr, "INVALID MESSAGE TYPE %c\n", message_type_id + '0');
                    exit(1);
                }
                index_buf = 0;
                memset(buffer, 0, 1024);

                // printf("MESSAGE TYPE ID: `%d`\n", message_type_id);
            }
            else if (field == MESSAGE_ID)
            {
                if (ch == '"')
                    ++block;

                if (block == 2)
                {
                    buffer[index_buf-1] = '\0';
                    // strcpy(message_id, buffer+1);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    // printf("MESSAGE ID: `%s`\n", message_id);
                }
            }
            else if (field == ACTION)
            {
                if (ch == '"')
                    ++block;

                if (block == 2)
                {
                    buffer[index_buf-1] = '\0';
                    // strcpy(action, buffer+1);
                    index_buf = 0;
                    memset(buffer, 0, 1024);
                    block = 0;

                    // printf("ACTION: `%s`\n", action);
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
                    // strcpy(action, buffer);
                    index_buf = 0;
                    // memset(buffer, 0, 1024);
                    block = 0;

                    // printf("PAYLOAD: `%s`\n", action);
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

    return response;
}



void
next_data_field(expected_data *data)
{
    if (*data != PAYLOAD)
        *data += 1;
    else
        *data = PAYLOAD;
}