#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "internals.h"
#include "task.h"
#include "scheduler.h"

u32 Registers[17];
u32	Saved_LR;
u32 LR_Return;

u32	OS_Stack[OS_STACK_SIZE];

u32 Params[4];

TaskType CurrentTask;
TaskType NextTask;
u8 InterruptPending;

void Internals_Init(void)
{
	CurrentTask = E_OS_INVALID_TASK; 
	NextTask = E_OS_INVALID_TASK;

	LR_Return = 0xFFFFFFFD;
	InterruptPending = 0;
}

void TaskReturn(void)
{
	Task_List[CurrentTask].state = SUSPENDED;
	Task_List[CurrentTask].taskinfo = 0x0;

	Reschedule();
}

