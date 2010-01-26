#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"

void Reschedule(void);
void SwapTask(void);

#endif /* __SCHEDULER_H__ */


