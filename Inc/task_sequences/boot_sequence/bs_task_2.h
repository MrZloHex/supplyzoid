#ifndef __BOOT_SEQ_TASK_2_H__
#define __BOOT_SEQ_TASK_2_H__

#include "controller_task.h"
#include "controller.h"

#define P_INTERVAL     "$.interval"
#define P_STATUS       "$.status"
#define P_CURRENT_TIME "$.currentTime"

Task_Result
bs_task_2(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __BOOT_SEQ_TASK_2_H__ */