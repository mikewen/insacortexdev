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
#include "common.h"
#include "task.h"
#include "scheduler.h"

u32 Registers[17];
u32	Saved_LR;
u32 LR_Return;

u32	OS_Stack[OS_STACK_SIZE];

u32 Params[4];

TaskType CurrentTask;
TaskType NextTask;
u8 InterruptPending;

void Common_Init(void)
{
	CurrentTask = E_OS_INVALID_TASK; 
	NextTask = E_OS_INVALID_TASK;

	LR_Return = 0xFFFFFFFD;
	InterruptPending = 0;
}

void TaskReturn(void)
{
	Task_List[CurrentTask].state = SUSPENDED;
	Task_List[CurrentTask].taskinfo = 0x0;

	Reschedule();
}

