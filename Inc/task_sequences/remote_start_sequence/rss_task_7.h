#ifndef __REMOTE_START_SEQ_TASK_7_H__
#define __REMOTE_START_SEQ_TASK_7_H__

#include "controller_task.h"
#include "controller.h"

#define P_TRANSACTION_ID "$.transactionId"
#define P_ID_INFO_STATUS "$.idTagInfo.status"

Task_Result
rss_task_7(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___REMOTE_START_SEQ_TASK_7_H_*/