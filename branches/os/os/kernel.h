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
 
#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "stm32f10x_type.h"
#include "os_config.h"

/************* General **************/
/* Invalid function call */
#define E_INVALID_FCT	-1

u32 OSCallWrapper_0(u32 Function_Id);
u32 OSCallWrapper_1(u32 Function_Id, u32 Param_1);
u32 OSCallWrapper_2(u32 Function_Id, u32 Param_1, u32 Param_2);
u32 OSCallWrapper_3(u32 Function_Id, u32 Param_1, u32 Param_2, u32 Param_3);

#define	ActivateTask_Fct_Id		1
#define TerminateTask_Fct_Id	2
#define ChaineTask_Fct_Id		3
#define Schedule_Fct_Id			4

/********** Task handling ***********/
/* StatusType definition */
#define E_OK			0
#define E_OS_ACCESS		1
#define E_OS_CALLEVEL	2
#define E_OS_ID			3
#define E_OS_LIMIT		4
#define E_OS_NOFUNC		5
#define E_OS_RESOURCE	6
#define E_OS_STATE		7
#define E_OS_VALUE		8

#define E_OS_INVALID_TASK	MAX_TASK_NBR

/* Type definition */
typedef u32 			TaskType;
typedef u8 				*TaskRefType;
typedef char 			TaskStateType;
typedef char			*TaskStateRefType;
typedef u8				StatusType;

typedef struct 
{
	char *taskname;
	void (*entrypoint)(void);
	u32	priority;
	u32 current_stack_ptr;
	u32 stack[STACK_SIZE];
} st_TaskInfo;

/* Constants definition */
#define RUNNING 	'R'
#define WAITING 	'W'
#define READY		'r'
#define SUSPENDED	'S'

#define INVALID_TASK	0

/* Task Macro Definition */
#define TASK(name) void name(void)

/* Services declaration */
TaskType	DeclareTask(st_TaskInfo *TaskInfo);
StatusType	GetTaskID(TaskType *TaskID);
StatusType	GetTaskState(TaskType TaskID,  TaskStateType *State);

// StatusType	ActivateTask(TaskType TaskID);
	#define ActivateTask(TaskID) 	OSCallWrapper_1(ActivateTask_Fct_Id, (u32)TaskID)

// StatusType	TerminateTask(void);
	#define TerminateTask(void) 	OSCallWrapper_0(TerminateTask_Fct_Id)

// StatusType	ChaineTask(TaskType TaskID);
	#define ChaineTask(TaskID) 		OSCallWrapper_1(ChaineTask_Fct_Id, (u32)TaskID)

// StatusType	Schedule(void);
	#define Schedule(void) 			OSCallWrapper_0(Schedule_Fct_Id)

/*********** Interrupt handling *********/
/* ISR Macro Definition */
#define ISR(name) void name(void)

/* Services declaration */
void EnableAllInterrupts(void);
void DisableAllInterrupts(void);
void ResumeAllInterrupts(void);
void SuspendAllInterrupts(void);
void ResumeOSInterrupts(void);
void SuspendOSInterrupts(void);

/*********** Resource management *********/
/* Type definition */
typedef u8 ResourceType;

typedef struct 
{
	u8 initstate;
	u8 resourcetype;
} st_ResourceInfo;

/* Services declaration */
ResourceType	DeclareResource(st_ResourceInfo *ResourceInfo);
StatusType		GetResource(ResourceType ResID);
StatusType		ReleaseResource(ResourceType ResID);

/* Constants declaration */
#define RES_SCHEDULER		0xFF

#define INVALID_RESOURCE 	MAX_RESOURCE_NBR

#define RES_MUTEX			0
#define RES_BINARY			1
#define	RES_COUNTER			2

/*********** Alarms management *********/
/* Type definition */
typedef u32 	TickType;
typedef u32 	*TickRefType;
typedef struct 
{
	TickType maxallowedvalue;
	TickType tickperbase;
	TickType mincycle;
} AlarmBaseType, *AlarmBaseRefType;
typedef u8 AlarmType;

typedef struct 
{
	u8 type;
	u32 period;
	void (*callbackfunc)(void);
} st_AlarmInfo;

/* Alarm Macro Definition */
#define ALARMCALLBACK(name) void name(void)

/* Services declaration */
AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo);
StatusType	GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);
StatusType	GetAlarm(AlarmType AlarmID, TickRefType Tick);
StatusType	SetRelAlarm(AlarmType AlarmID, TickType Increment, TickType Cycle);
StatusType	SetAbsAlarm(AlarmType AlarmID, TickType Start, TickType Cycle);
StatusType	CancelAlarm(AlarmType AlarmID);

/* Constants declaration */
#define OSMAXALLOWEDVALUE	100000000
#define OSTICKPERBASE		1
#define OSMINCYCLE			1
#define OSTICKDURATION		1000000

#define INVALID_ALARM		MAX_ALARM_NBR

#ifdef __WITH_EVENTS__
/*********** Event management *********/
/* Type definition */
typedef u8 	EventMaskType;
typedef u8  *EventMaskRefType;

typedef struct 
{
	u8 eventtype;
} st_EventInfo;

/* Services declaration */
EventMaskType	DeclareEvent(st_EventInfo *EventInfo);
StatusType		SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType		ClearEvent(EventMaskType Mask);
StatusType		GetEvent(TaskType TaskID, EventMaskType Mask);
StatusType		WaitEvent(EventMaskType Mask);

#define INVALID_EVENT	MAX_EVENT_NBR

#endif /* __WITH_EVENTS__ */

/*********** OS management *********/
/* Type definition */
typedef u8 AppModeType;
	
/* Services declaration */
AppModeType	GetActiveApplicationMode(void);
void		InitOS(void);
void		StartOS(AppModeType Mode);
void 		ShutdownOS(StatusType Error);

/* Constants declaration */
#define OSDEFAULTAPPMODE	0

#endif /* __KERNEL_H__ */
