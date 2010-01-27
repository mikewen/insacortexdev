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
#include "scheduler.h"
#include "task.h"
#include "common.h"

void Reschedule(void)
{
TaskType TaskID;
u8 End;

	/* On parcours la table des taches à la recherche d'une tache RUNNING ou READY, 
	 * non bloqués par une source (locksource)
	 * A l'issue de cette fonction, la tache a executer sera indiquée dans CurrentTask 
	 */

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

