#ifndef __DATA_TRANSFER_SEQ_TASK_1_H__ 
#define __DATA_TRANSFER_SEQ_TASK_1_H__ 

#include "controller_task.h"
#include "controller.h"



#define DT_TASK_WRAP(__PTR__, __TRIGGER_ID__)  __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;                  \
                                __PTR__->task.func = dt_task_1;                    \
                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__);   \
                                __PTR__->task.genesis_time = HAL_GetTick()

#define P_DATA "$.data"
#define MAX_DATA_TRANSFER_DATA_LEN 256

Task_Result
dt_task_1(Controller *ctrl, OCPP_MessageID _id);

#endif /* __DATA_TRANSFER_SEQ_TASK_1_H__ */