#ifndef __TIMER_H__
#define __TIMER_H__

#include "stdint.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"

typedef struct Timer_S
{
    uint32_t timeout;
    uint32_t target;
    bool start;
    bool restart;
} Timer;

void
timer_set(Timer *t, uint32_t timeout, bool restart)
{
    t->restart = restart;
    t->timeout = timeout;
}

void
timer_start(Timer *t)
{
    if (!t->start)
    {
        t->start = true;
        t->target = HAL_GetTick() + t->timeout;
    }
}

bool
timer_timeout(Timer *t)
{
    if (!(t->start && t->target <= HAL_GetTick()))
        return false;

    if (t->restart)
        t->target += t->timeout;
    else
        t->start = false;

    return true;
}

void
timer_stop(Timer *t)
{
    t->start = false;
}

#endif /* __TIMER_H__ */