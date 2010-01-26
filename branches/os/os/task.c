#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "task.h"
#include "scheduler.h"
#include "internals.h"

struct st_Task Task_List[MAX_TASK_NBR];
u32 TaskStackPointer[MAX_TASK_NBR+1];

TaskType DeclareTask(st_TaskInfo *TaskInfo)
{
TaskType TaskID;

	TaskID = TaskInfo->priority;

	if (TaskID >= MAX_TASK_NBR)
	{
		TaskID = E_OS_INVALID_TASK;
	} 
	else
	{
		if (Task_List[TaskID].taskinfo != 0x0)
		{
			 TaskID = E_OS_INVALID_TASK;
		}
		else
		{
			Task_List[TaskID].taskinfo= TaskInfo;
			Task_List[TaskID].locksource=LOCK_SOURCE_NONE;
		}
	}
	
	return TaskID;
}

StatusType GetTaskID(TaskType *TaskID)
{
	*TaskID = CurrentTask;
	 
	return E_OK;
}

StatusType	GetTaskState(TaskType TaskID, TaskStateType *State)
{
	*State = Task_List[TaskID].state;

	return E_OK;
}

StatusType	ActivateTask_Int(TaskType TaskID)
{
u8 Status;
u8 i;

	Status = E_OK;

	if (Task_List[TaskID].state == SUSPENDED)
	{
		/* Elle passe a READY */
		Task_List[TaskID].state = READY;
		Task_List[TaskID].locksource = LOCK_SOURCE_NONE;

		for (i=0; i<13; i++)
		{
			Task_List[TaskID].registers[i]=0;
		}
		
		Task_List[TaskID].registers[SP_Reg] = (u32)((&Task_List[TaskID].taskinfo->stack)+ STACK_SIZE - (8*4));
		Task_List[TaskID].registers[LR_Reg] = 0;
		Task_List[TaskID].registers[PC_Reg] = (u32)(&Task_List[TaskID].taskinfo->entrypoint);
		Task_List[TaskID].registers[PSR_Reg] = 0;
		Task_List[TaskID].LR = 0;
		
		Reschedule();	
	}
	else
	{
		Status = E_OS_LIMIT; // la tache a deja été activée
	}
	return Status;
}

StatusType	TerminateTask_Int(void)
{
u8 Status;

	Status = E_OK;

	if (Task_List[CurrentTask].state != SUSPENDED)
	{
		/* Elle passe a SUSPENDED */
		Task_List[CurrentTask].state = SUSPENDED;
		Task_List[CurrentTask].taskinfo = 0x0;

		Reschedule();	
	}
	else
	{
		Status = E_OS_LIMIT; // la tache a deja été activée
	}
	return Status;
}

StatusType	ChaineTask_Int(TaskType TaskID)
{
	return E_OS_LIMIT;
}

StatusType	Schedule_Int(void)
{
	return E_OK;
}

void Task_Init(void)
{
u8 i;

	for(i=0; i<	MAX_TASK_NBR; i++)
	{
		Task_List[i].taskinfo = 0x0;
		Task_List[i].state = SUSPENDED;
	}
}
