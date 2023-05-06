#include "controller_taskset.h"
#include "serial.h"

Controller_TaskSet_Result
_controller_taskset_initialize(Controller_TaskSet *set, size_t capacity)
{
	set->capacity = capacity;
	set->size = 0;
	set->iterated = false;

	set->set = (Controller_TaskWrap *)malloc(sizeof(Controller_TaskWrap)*capacity);
	if (set->set == NULL)
	{
		return CTRL_SET_ALLOC_ERR;
	}

	for (size_t i = 0; i < set->capacity; ++i)
	{
		set->set[i].type = WRAP_EMPTY;
	}

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_push(Controller_TaskSet *set, Controller_TaskWrap task)
{
	if (set->iterated)
	{
		return CTRL_SET_IN_ITER;
	}

	if (set->size == set->capacity)
	{
		return CTRL_SET_OVERFLOW;
	}

	for (size_t i = 0; i < set->capacity; ++i)
	{
		if (set->set[i].type == WRAP_EMPTY)
		{
			set->set[i] = task;
			set->size++;
			break;
		}
	}

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_pop(Controller_TaskSet *set)
{
	if (set->iterated)
	{
		return CTRL_SET_IN_ITER;
	}
	
	if (set->size == 0)
	{
		return CTRL_SET_OK;
	}

	for (size_t i = 0; i < set->capacity; ++i)
	{
		if (set->set[i].type == WRAP_FINISHED || set->set[i].type == WRAP_TIMEOUT)
		{
			set->set[i].type = WRAP_EMPTY;
			set->size -= 1;
		}
	}

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_iterate(Controller_TaskSet *set)
{
	if (set->iterated)
	{
		return CTRL_SET_IN_ITER;
	}

	set->tasks = (size_t *)malloc(sizeof(size_t)*set->size);
	if (set->tasks == NULL)
	{
		return CTRL_SET_ALLOC_ERR;
	}

	for(size_t i = 0, index = 0; i < set->capacity; ++i)
	{
		if (set->set[i].type == WRAP_IN_PROGRESS)
		{
			set->tasks[index++] = i;
		}
	}

	set->index = 0;
	set->iterated = true;

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_next(Controller_TaskSet *set, Controller_TaskWrap *task)
{
	if (task == NULL)
	{
		return CTRL_SET_NULLPTR;
	}

	if (!set->iterated)
	{
		return CTRL_SET_NOT_ITER;
	}

	*task = set->set[set->tasks[set->index++]];

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_update_task(Controller_TaskSet *set, Controller_TaskWrap task)
{
	if (!set->iterated)
	{
		return CTRL_SET_NOT_ITER;
	}

	set->set[set->tasks[set->index -1]] = task;

	return CTRL_SET_OK;
}

Controller_TaskSet_Result
_controller_taskset_esc_iter(Controller_TaskSet *set)
{
	free(set->tasks);
	set->iterated = false;

	return CTRL_SET_OK;
}


void
__debug_taskset_print(Controller_TaskSet *set)
{
	uprintf(DBUG_UART, 1000, 100, "SIZE %u\t CAP %u\r", set->size, set->capacity);
	uprintf(DBUG_UART, 1000, 100, "TYPE\tPTR\t\tUSART\tTTYPE\tTRIGGER ID\r");
	for (size_t i = 0; i < set->size; ++i)
	{
		uprintf(DBUG_UART, 1000, 100, "%u\t%p\t%u\t%u\t`%s`\r", set->set[i].type, set->set[i].task.func, set->set[i].task.usart, set->set[i].task.type, set->set[i].task.trigger_id);
	}
}
