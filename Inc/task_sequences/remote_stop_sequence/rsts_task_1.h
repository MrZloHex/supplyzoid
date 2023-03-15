#ifndef __REMOTE_STOP_SEQ_TASK_1_H__
#define __REMOTE_STOP_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define RSTS_TASK_WRAP(__PTR__, __TRIGGER_ID__) __PTR__->type = WRAP_IN_PROGRESS;   \
                                                __PTR__->task.type = TASK_TRIGGER;  \
                                                __PTR__->task.func = rsts_task_1;   \
                                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__)
                                        
#define P_TRANSACTION_ID "$.transactionId"

Task_Result
rsts_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___REMOTE_STOP_SEQ_TASK_1_H_*/