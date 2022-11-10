/**
  ******************************************************************************
  * @file    controller_queue.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_QUEUE_H__
#define __CONTROLLER_QUEUE_H__

#include "stdlib.h"
#include "controller_types.h"

typedef struct Controller_Queue_S
{
	Controller_Task *queue;
	size_t 	 		    capacity;
	size_t			    size;
	ptrdiff_t			front, rear;
} Controller_Queue;

Controller_Queue_Result
_controller_queue_initialize(Controller_Queue *queue, size_t capacity);

Controller_Queue_Result
_controller_queue_enqueue(Controller_Queue *queue, Controller_Task task);

Controller_Queue_Result
_controller_queue_dequeue(Controller_Queue *queue, Controller_Task *task);

#endif /* __CONTROLLER_QUEUE_H__ */