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
 
#ifndef __TASK_H__
#define __TASK_H__

#include "stm32f10x_type.h"
#include "os_config.h"
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

