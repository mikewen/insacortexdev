/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of INSA OS.
 *
 * INSA OS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * INSA OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with INSA OS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */
 
#include "stm32f10x_type.h"
#include "os_config.h"
#include "kernel.h"
#include "task.h"
#include "scheduler.h"
#include "common.h"

#include "port.h"

struct st_Task Task_List[MAX_TASK_NBR];
u32 TaskStackPointer[MAX_TASK_NBR+1]; 	// Stock les SP en cours des taches non actives 

/* Declaration de la background task */
void BackgroundTask(void);
TaskType BackgroundTask_ID;
st_TaskInfo BackgroundTask_info;
const char BackgroundTask_nom[] = "Background Task";

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
			Task_List[TaskID].taskinfo= 	TaskInfo;
			Task_List[TaskID].locksource=	LOCK_SOURCE_NONE;
			Task_List[TaskID].state=		READY;

			/* On lui recopie la stack initiale */
			FastCopy(Task_List[TaskID].taskinfo->stack + (STACK_SIZE*4) - (STARTUP_STACK_SIZE*4), 
		     	    (u32*)StartupStack, STARTUP_STACK_SIZE);

			/* on met a jour le point d'entrée */
			SET_ENTRY_POINT(TaskID,(u32)(Task_List[TaskID].taskinfo->entrypoint));
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
	if (TaskID >= MAX_TASK_NBR)
	{
		return E_OS_INVALID_TASK;
	} 
	
	*State = Task_List[TaskID].state;

	return E_OK;
}

StatusType	ActivateTask_Int(TaskType TaskID)
{
u8 Status;

	if (TaskID >= MAX_TASK_NBR)
	{
		return E_OS_INVALID_TASK;
	} 

	Status = E_OK;

	if (Task_List[TaskID].state == SUSPENDED)
	{
		/* Elle passe a READY et rentre dans le pool des taches activables*/
		Task_List[TaskID].state = 		READY;
		Task_List[TaskID].locksource=	LOCK_SOURCE_NONE;

		/* On lui recopie la stack initiale */
		FastCopy(Task_List[TaskID].taskinfo->stack + (STACK_SIZE*4) - (STARTUP_STACK_SIZE*4), 
		         (u32*)StartupStack, STARTUP_STACK_SIZE);

		/* on met a jour le point d'entrée */
		SET_ENTRY_POINT(TaskID,(u32)(Task_List[TaskID].taskinfo->entrypoint));
		
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
//		Task_List[CurrentTask].taskinfo = 0x0;

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
	Reschedule();

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

	/* Ajout de la background task */
	BackgroundTask_info.taskname = (char*)BackgroundTask_nom;
	BackgroundTask_info.entrypoint = BackgroundTask;
	BackgroundTask_info.priority = 0;  	// plus faible priorité
	BackgroundTask_info.type = 0;

	BackgroundTask_ID = DeclareTask(&BackgroundTask_info);
}

void BackgroundTask(void)
{
	while (1);
}
