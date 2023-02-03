#ifndef __SEND_LOCAL_SEQ_TASK_1_H__
#define __SEND_LOCAL_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define SLL_TASK_WRAP(__PTR__, __TRIGGER_ID__)  __PTR__->type = WRAP_IN_PROGRESS;                   \
                                                __PTR__->task.type = TASK_TRIGGER;                  \
                                                __PTR__->task.func = sll_task_1;                    \
                                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__)

#define P_LIST_VERSION "$.listVersion"
#define P_UPDATE_TYPE  "$.updateType"
#define P_LIST         "$.localAuthorizationList"
#define P_TOKEN_TAG    "$.idTag"
#define P_TOKEN_STATUS "$.idTagInfo.status"

Task_Result
sll_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __SEND_LOCAL_SEQ_TASK_1_H__ */