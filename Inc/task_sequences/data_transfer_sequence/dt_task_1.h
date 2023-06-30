#ifndef __DATA_TRANSFER_SEQ_TASK_1_H__ 
#define __DATA_TRANSFER_SEQ_TASK_1_H__ 

#include "controller_task.h"
#include "controller.h"

#define DT_TASK_WRAP(__PTR__)   __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = dt_task_1;     \
                                __PTR__->task.genesis_time = HAL_GetTick()

Task_Result
dt_task_1(Controller *ctrl, OCPP_MessageID _id);

#endif /* __DATA_TRANSFER_SEQ_TASK_1_H__ */