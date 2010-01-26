#ifndef __TASK_H__
#define __TASK_H__

#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"

/* lock source definition */
#define LOCK_SOURCE_NONE		0
#define LOCK_SOURCE_RESOURCE	1
#define LOCK_SOURCE_EVENT		2
#define LOCK_SOURCE_ALARM		3

/* registers definition */
#define R0_Reg		0
#define R1_Reg		1
#define R2_Reg		2
#define R3_Reg		3
#define R4_Reg		4
#define R5_Reg		5
#define R6_Reg		6
#define R7_Reg		7
#define R8_Reg		8
#define R9_Reg		9
#define R10_Reg		10
#define R11_Reg		11
#define R12_Reg		12
#define SP_Reg		13
#define LR_Reg		14
#define PC_Reg		15
#define PSR_Reg		16

struct st_Task
{
	st_TaskInfo *taskinfo;
	TaskStateType state;
	u8 locksource;
	u8 locksourceid;
	u32 registers[17];
	u32 LR;
};

extern struct st_Task Task_List[MAX_TASK_NBR];
extern u32 TaskStackPointer[MAX_TASK_NBR+1];
void Task_Init(void);

#endif /* __TASK_H__ */


