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
#include "resource.h"
#include "common.h"
#include "scheduler.h"
#include "task.h"

struct st_Resource
{
	st_ResourceInfo *resourceinfo;
	u8 state;
} Resource_List[MAX_RESOURCE_NBR];

ResourceType	DeclareResource(st_ResourceInfo *ResourceInfo)
{
ResourceType ResID;
u8 End;

	if (ResourceInfo->resourcetype != RES_BINARY) return INVALID_RESOURCE;

	End = 0;
	ResID = 0;

	while (End != 1)
	{
		if (Resource_List[ResID].resourceinfo == 0x0)
		{
			End =1;

			Resource_List[ResID].resourceinfo = ResourceInfo;
			Resource_List[ResID].state = Resource_List[ResID].resourceinfo->initstate;
			if (Resource_List[ResID].state >1) Resource_List[ResID].state=1; 				
		}
		else
		{
			ResID++;

			if (ResID == MAX_RESOURCE_NBR)
			{
				End = 1;	
			}
		}		
	}
		
	return ResID;
}

StatusType		GetResource_Int(ResourceType ResID)
{
	if (Resource_List[ResID].resourceinfo == 0x0) return E_OS_ID;

	if (Resource_List[ResID].state>0)
	{
		/* La resource est deja prise */
		Task_List[CurrentTask].locksource = LOCK_SOURCE_RESOURCE;
		Task_List[CurrentTask].locksourceid = ResID;
		Task_List[CurrentTask].state = READY;

		Reschedule();
	}
	else
	{
		/* On prend la resource */
		Resource_List[ResID].state=1;
	}

	return E_OK;
}

StatusType		ReleaseResource_Int(ResourceType ResID)
{
u8 i;
u8 End;

	if (Resource_List[ResID].resourceinfo == 0x0) return E_OS_ID;

	if (Resource_List[ResID].state>0)
	{
		/* Recherche si un tache plus prioritaire etait bloqué sur ce semaphore */
		i = MAX_TASK_NBR-1;
		End = 0;

		while (End != 1)
		{
			if (Task_List[i].locksource == LOCK_SOURCE_RESOURCE)
			{
				if (Task_List[i].locksourceid == ResID)
				{
					Task_List[i].locksource = LOCK_SOURCE_NONE;
					End = 1;
				}
			}
			else
			{
				i = i-1;

				if (i==0)
				{
					End = 1;

					/* Liberation de la ressource */
					Resource_List[ResID].state=0;
				}
			}
		}
	}
	else
	{
		/* La ressource est deja libre: rien a faire */
	}

	return E_OK;
}

void Resource_Init(void)
{
u8 i;

	for(i=0; i<	MAX_RESOURCE_NBR; i++)
	{
		Resource_List[i].resourceinfo = 0x0;
	}
}
