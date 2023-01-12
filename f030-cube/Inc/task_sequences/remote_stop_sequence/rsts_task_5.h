#ifndef __REMOTE_STOP_SEQ_TASK_5_H__
#define __REMOTE_STOP_SEQ_TASK_5_H__

#include "controller_task.h"
#include "controller.h"

#define P_ID_INFO_STATUS "$.idTagInfo.status"

Task_Result
rsts_task_5(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___REMOTE_STOP_SEQ_TASK_5_H_*/