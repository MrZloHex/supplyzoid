#ifndef __RESET_SEQ_TASK_1_H__
#define __RESET_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define RT_TASK_WRAP(__PTR__, __TRIGGER_ID__) __PTR__->type = WRAP_IN_PROGRESS;   \
                                                __PTR__->task.type = TASK_TRIGGER;  \
                                                __PTR__->task.func = rt_task_1;   \
                                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__); \
                                                __PTR__->task.genesis_time = HAL_GetTick()

#define P_RESET_TYPE "$.type"

Task_Result
rt_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __RESET_SEQ_TASK_1_H__ */