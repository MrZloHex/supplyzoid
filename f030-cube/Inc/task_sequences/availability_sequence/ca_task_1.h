#ifndef __AVAILABILITY_SEQ_TASK_1_H__
#define __AVAILABILITY_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define CA_TASK_WRAP(__PTR__, __TRIGGER_ID__)   __PTR__->type = WRAP_IN_PROGRESS;   \
                                                __PTR__->task.type = TASK_TRIGGER;  \
                                                __PTR__->task.func = ca_task_1;     \
                                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__)

#define P_CONTROLLER_ID "$.connectorId"
#define P_TYPE          "$.type"

Task_Result
ca_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __AVAILABILITY_SEQ_TASK_1_H__ */