#ifndef __REMOTE_START_SEQ_TASK_1_H__
#define __REMOTE_START_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define RSS_TASK_WRAP(__PTR__, __TRIGGER_ID__)  __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;                  \
                                __PTR__->task.func = rss_task_1;                    \
                                strcpy(__PTR__->task.trigger_id, __TRIGGER_ID__)

#define P_ID_TAG "$.idTag"

Task_Result
rss_task_1(Controller *ctrl, OCPP_MessageID t_id);

#endif /* ___REMOTE_START_SEQ_TASK_1_H_*/