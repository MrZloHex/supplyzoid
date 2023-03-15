#ifndef __METER_VALUES_SEQ_TASK_1_H__
#define __METER_VALUES_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define MV_TASK_WRAP(__PTR__)   __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = mv_task_1;

Task_Result
mv_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __METER_VALUES_SEQ_TASK_1_H__ */