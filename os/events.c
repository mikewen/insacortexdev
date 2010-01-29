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
#include "events.h"

#include "port.h"

/* Event services declaration */

/*
 * void Task_Init(void)
 * 
 * Initialise la partie du noyau responsable de la gestion des taches
 */
EventMaskType	DeclareEvent(st_EventInfo *EventInfo)
{
EventMaskType event;

	event = SearchFreeLsb(Task_List[EventInfo->TaskID]->events_list);

	if (event == MAX_EVENT_NBR) return INVALID_EVENT;
	else 
	{
		Task_List[EventInfo->TaskID]->events_list += (1<<event);
		return (1<<event);
	}
}

StatusType		SetEvent_Int(TaskType TaskID, EventMaskType Mask)
{
	return E_OK;
}

StatusType		ClearEvent_Int(EventMaskType Mask)
{
	return E_OK;
}

StatusType		GetEvent(TaskType TaskID, EventMaskType Mask)
{
	return E_OK;
}

StatusType		WaitEvent_Int(EventMaskType Mask)
{
	return E_OK;
}

void Events_Init(void)
{
}

