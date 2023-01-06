#ifndef __REMOTE_START_SEQ_TASK_1_H__
#define __REMOTE_START_SEQ_TASK_1_H__

#include "controller_task.h"
#include "controller.h"

#define RSS_TASK_WRAP(__PTR__)  __PTR__->type = WRAP_IN_PROGRESS;   \
                                __PTR__->task.type = TASK_TRIGGER;  \
                                __PTR__->task.func = rss_task_1;

#define P_ID_TAG "$.idTag"

Task_Result
rss_task_1(Controller *ctrl);

#endif /* ___REMOTE_START_SEQ_TASK_1_H_*/