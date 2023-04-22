#ifndef __GET_STATE_SEQ_TASK_1_H__
#define __GET_STATE_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define GS_TASK_WRAP(__PTR__)   __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = gs_task_1;

Task_Result
gs_task_1(Controller *ctrl, OCPP_MessageID _id);

#endif /* __GET_STATE_SEQ_TASK_1_H__ */