#ifndef __BOOT_SEQ_TASK_1_H__
#define __BOOT_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define BS_TASK_WRAP(__PTR__)   __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = bs_task_1;

Task_Result
bs_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __BOOT_SEQ_TASK_1_H__ */