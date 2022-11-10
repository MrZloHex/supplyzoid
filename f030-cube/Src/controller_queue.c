#include "controller_queue.h"

Controller_Queue_Result
_controller_queue_initialize(Controller_Queue *queue, size_t capacity)
{
	queue->capacity = capacity;
	queue->size = queue->front = 0;
	queue->rear = capacity -1;

	queue->queue = (Controller_Task *)malloc(sizeof(Controller_Task)*capacity);
	if (queue->queue == NULL)
	{
		return CTRL_QUE_ALLOC_ERR;
	}

	return CTRL_QUE_OK;
}

Controller_Queue_Result
_controller_queue_enqueue(Controller_Queue *queue, Controller_Task task)
{
	if (queue->size == queue->capacity)
	{
		return CTRL_QUE_OVERFLOW;
	}

	queue->rear = (queue->rear +1) % queue->capacity;
	queue->queue[queue->rear] = task;
	queue->size += 1;

	return CTRL_QUE_OK;
}

Controller_Queue_Result
_controller_queue_dequeue(Controller_Queue *queue, Controller_Task *task)
{
	if (task == NULL)
	{
		return CTRL_QUE_NULLPTR;
	}

	if (queue->size == 0)
	{
		*task = NO_TASK;
		return CTRL_QUE_EMPTY;
	}

	*task = queue->queue[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size -= 1;

	return CTRL_QUE_OK;
}