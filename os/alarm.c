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
#include "alarm.h"

struct st_Alarm
{
	st_AlarmInfo *alarminfo;
	u32 counter;
} Alarm_List[MAX_ALARM_NBR];

/* Alarm services declaration */

/*
 * AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo)
 * 
 * Rajoute une alarme à la liste
 */
AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo)
{
u32 index;

	index =0;

	while (index< MAX_ALARM_NBR)
	{
		if (Alarm_List[index].alarminfo == 0)
		{
			Alarm_List[index].alarminfo = AlarmInfo;
			Alarm_List[index].alarminfo->state = ALARM_OFF;
		}
		else index++;
	}

	if (index == MAX_ALARM_NBR) return E_OS_LIMIT;
	else return E_OK;
}

StatusType	GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[index].alarminfo == 0))
	{
		return E_OS_ID;
	}

	Info = &(Alarm_List[index].alarminfo->basetype);

	return E_OK;
}

StatusType	GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[index].alarminfo == 0))
	{
		return E_OS_ID;
	}

	Tick = &(Alarm_List[index].counter);

	return E_OK;
}

StatusType	SetRelAlarm_Int(AlarmType AlarmID, TickType Increment, TickType Cycle)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[index].alarminfo == 0))
	{
		return E_OS_ID;
	}

	return E_OK;
}

StatusType	SetAbsAlarm_Int(AlarmType AlarmID, TickType Start, TickType Cycle)
{
	return E_OK;
}

StatusType	CancelAlarm_Int(AlarmType AlarmID)
{
	return E_OK;
}

void Alarm_Init(void)
{
u8 i;

	for(i=0; i<	MAX_ALARM_NBR; i++)
	{
		Alarm_List[i].alarminfo = 0x0;
	}
}

/* It systeme du timer  */
void AlarmTimerTick(register int func_index)
{
}

