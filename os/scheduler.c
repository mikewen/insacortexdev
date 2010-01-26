#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "scheduler.h"
#include "task.h"
#include "internals.h"

void Reschedule(void)
{
TaskType TaskID;
u8 End;

	/* On parcours la table des taches à la recherche d'une tache RUNNING ou READY, 
	   non bloqués par une source (locksource) */

	/* On commence par la priorité la plus forte (MAX_TASK_NBR-1)*/   
	TaskID = MAX_TASK_NBR-1;
	End = 0;

	while (End != 1)
	{
		if (Task_List[TaskID].state != SUSPENDED)
		{
			if (Task_List[TaskID].locksource == LOCK_SOURCE_NONE)
			{
				NextTask = TaskID;
				End = 1;

				if (NextTask != CurrentTask)
				{
					Task_List[NextTask].state = RUNNING;
				
					if (Task_List[CurrentTask].state == RUNNING)
					{
						Task_List[CurrentTask].state = READY;
					}
				}
			}
		}

		/* La tache 0, la moins prioritaire, correspond à la "background task" d'autres OS
		   En ce sens, elle ne peut pas etre bloquée sur semaphore et doit tjs exister */
		
		if (End == 0)
		{ 
			if (TaskID == 0) 
			{
				NextTask = TaskID;
				Task_List[TaskID].locksource = LOCK_SOURCE_NONE;
				End = 1;

				if (NextTask != CurrentTask)
				{
					Task_List[NextTask].state = RUNNING;
				}
			}
			else
			{
				TaskID--;
			}
		}
	}
}

void SwapTask(void)
{
u8 i;

	if (InterruptPending == 0)
	{
		/* Echange des contextes de la tache actuelle avec la nouvelle tache */

		for (i=0; i<4; i++)
		{
			Task_List[CurrentTask].registers[i] = Params[i];
			Registers[i] = Task_List[NextTask].registers[i];
		}

		for (i=4; i<17; i++)
		{
			Task_List[CurrentTask].registers[i] = Registers[i];
			Registers[i] = Task_List[NextTask].registers[i];
		}

		Task_List[CurrentTask].LR=Saved_LR;
		Saved_LR = Task_List[NextTask].LR;

	 	CurrentTask = NextTask;
	}
}
