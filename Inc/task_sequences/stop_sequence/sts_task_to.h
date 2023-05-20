#ifndef __STOP_SEQ_TASK_TO_H__
#define __STOP_SEQ_TASK_TO_H__

#include "controller_task.h"
#include "controller.h"

Task_Result
sts_task_to(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __STOP_SEQ_TASK_TO_H__*/