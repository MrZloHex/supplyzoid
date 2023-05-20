#ifndef __REMOTE_STOP_SEQ_TASK_TO_H__
#define __REMOTE_STOP_SEQ_TASK_TO_H__

#include "controller_task.h"
#include "controller.h"

Task_Result
rsts_task_to(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___REMOTE_STOP_SEQ_TASK_TO_H__*/