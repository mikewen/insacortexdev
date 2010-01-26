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

struct st_Task
{
	TaskStateType state;
	u8 locksource;
	u8 locksourceid;
	st_TaskInfo *taskinfo;
};

extern struct st_Task Task_List[MAX_TASK_NBR];
extern u32 TaskStackPointer[MAX_TASK_NBR+1];
void Task_Init(void);

#endif /* __TASK_H__ */


