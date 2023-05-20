#ifndef __STOP_SEQ_TASK_1_H__
#define __STOP_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define STS_TASK_WRAP(__PTR__)  __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = sts_task_1;    \
                                __PTR__->task.genesis_time = HAL_GetTick()

Task_Result
sts_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___STOP_SEQ_TASK_1_H_*/