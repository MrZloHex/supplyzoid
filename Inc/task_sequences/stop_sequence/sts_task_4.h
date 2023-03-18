#ifndef __STOP_SEQ_TASK_4_H__
#define __STOP_SEQ_TASK_4_H__

#include "controller_task.h"
#include "controller.h"

#define P_ID_INFO_STATUS "$.idTagInfo.status"

Task_Result
sts_task_4(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __STOP_SEQ_TASK_4_H_*/