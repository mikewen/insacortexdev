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

#ifndef __PORT_H__
#define __PORT_H__

#include "os_config.h"

#define	ActivateTask_Fct_Id			1
#define TerminateTask_Fct_Id		2
#define ChaineTask_Fct_Id			3
#define Schedule_Fct_Id				4
#define GetResource_Fct_Id			5
#define	ReleaseResource_Fct_Id		6
#define SetRelAlarm_Fct_Id			7
#define SetAbsAlarm_Fct_Id			8
#define CancelAlarm_Fct_Id			9
#define SetEvent_Fct_Id				10
#define ClearEvent_Fct_Id			11
#define WaitEvent_Fct_Id			12

extern const u32 StartupStack[];

#define STARTUP_STACK_SIZE			18 // Exprim� en mots de 32 bits */
#define SET_ENTRY_POINT(tid,ep)		Task_List[tid].taskinfo->stack[STACK_SIZE-2] = ep	

void FastCopy (u32 *dest, u32 *src, u32 len);

#endif /* __PORT_H__ */

