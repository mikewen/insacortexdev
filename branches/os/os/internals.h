#ifndef __INTERNALS_H__
#define __INTERNALS_H__

#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"

extern u32 Registers[17];
extern u32 Saved_LR;
extern u32 Params[4];

extern u32 OS_Stack[OS_STACK_SIZE];

extern TaskType CurrentTask;
extern TaskType NextTask;
extern u8 InterruptPending;
extern u32 LR_Return;

void Internals_Init(void);

#endif /* __INTERNALS_H__ */


