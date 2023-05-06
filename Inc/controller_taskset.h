/**
	******************************************************************************
	* @file    controller_taskset.h 
	* @author  MrZloHex
	******************************************************************************
	*/

#ifndef __CONTROLLER_TASKSET_H__
#define __CONTROLLER_TASKSET_H__

#include "stdlib.h"
#include "stdbool.h"
#include "controller_task.h"
#include "controller_types.h"

typedef struct Controller_TaskSet_S
{
	Controller_TaskWrap *set;
	size_t			capacity;
	size_t			size;

	bool			iterated;
	size_t		   *tasks;
	size_t    		index;
} Controller_TaskSet;

Controller_TaskSet_Result
_controller_taskset_initialize(Controller_TaskSet *set, size_t capacity);

Controller_TaskSet_Result
_controller_taskset_push(Controller_TaskSet *set, Controller_TaskWrap task);

Controller_TaskSet_Result
_controller_taskset_pop(Controller_TaskSet *set, size_t *last_id);

Controller_TaskSet_Result
_controller_taskset_iterate(Controller_TaskSet *set);

Controller_TaskSet_Result
_controller_taskset_next(Controller_TaskSet *set, Controller_TaskWrap *task);

Controller_TaskSet_Result
_controller_taskset_update_task(Controller_TaskSet *set, Controller_TaskWrap task);

Controller_TaskSet_Result
_controller_taskset_esc_iter(Controller_TaskSet *set);

void
__debug_taskset_print(Controller_TaskSet *set);

#endif /* __CONTROLLER_TASKSET_H__ */