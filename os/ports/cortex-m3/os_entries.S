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

.include "definitions.inc"

					.syntax unified		/* Required in order to be able to use Thumb-2 instructions
										   Otherwise, produce only Thumb-1 code (16 bits instructions) */			
					.cpu cortex-m3			 
					.fpu softvfp
					.thumb
					.file	"os_entries.S"

					.section ".text"
					.align	2
/*
 * Jump table for OS entry points
 */

OS_Jump_Table:		.long	AlarmTimerTick
					.long	ActivateTask_Int
					.long	TerminateTask_Int
					.long	ChaineTask_Int
					.long	Schedule_Int
					.long	GetResource_Int		
					.long	ReleaseResource_Int	
					.long	SetRelAlarm_Int		
					.long	SetAbsAlarm_Int	
					.long	CancelAlarm_Int		
					.long	SetEvent_Int			
					.long	ClearEvent_Int		
					.long	WaitEvent_Int	
OS_Jump_Table_End:

/* 
 * OS startup
 * This function start the first task
 */
 					.global	OS_Start_Switch
					.thumb
					.thumb_func
					.type	OS_Start_Switch, %function
OS_Start_Switch:
					BL		Reschedule		

					LDR		R0, =CurrentTask
					LDR		R0, [R0]				/* R0 contains the task to switch to */

					LDR		R1, =TaskStackPointer	/* R1 points to the beginning of TaskStackPointer Table */
					
					LSL		R0, #2
					LDR		R0, [R1, R0]  			/* R1 contains Stack Pointer of CurrentTask */

					MSR		PSP, R0					/* PSP is loaded with CurrentTask Stack pointer */
					MOVW	R0, #0x5000				/* R0 contains new MSP value */
					MOVT	R0, #0x2000
					MSR		MSP, R0

					MRS		R0, CONTROL
					MOV		R0, #2
					MSR		CONTROL, R0	  			/* Now the thread mode use PSP stack pointer */
					ISB 							/* Instruction Barrier to ensure next instruction will use PSP, not MSP */

					/* 
					 * Now, SP points on top of CurrentTask stack: use of PUSH and POP instructions
					 * will make use of CurrentTask stack.
					 */

					POP 	{R4-R11}				/* Restore Initial data for this task */
					POP		{R0-R3, R12, LR}
					POP		{PC}					/* and jump at entry point */

					/*
					 * Sadly the very first task to be started will have an offset of one word in its stack
					 * That's because initial PSR is never popped. I don't know how to do that except with 
					 * a return from exception
					 */

/*					
 * Switch routine:
 *
 * Enter: 	R0 = return code of OS fucntion
 *			R6 = Active Task before OS call
 *			R7 = Active task after OS call
 *
 * Exit:	R0 and SP stored in previous task infos
 *			R0 and SP restored from newly activated task infos
 */
					.global	OS_Switch_Tasks
					.thumb
					.thumb_func
					.type	OS_Switch_Tasks, %function
OS_Switch_Tasks:	
					LDR		R1, =TaskStackPointer	/* R0 pointe au debut de la table TaskStackPointer */
 
					LSL		R6, #2					/* R6 = offset from start of table TaskStackPointer (previous task index) */
					STR		R4, [R1, R6]			/* Store previous task SP (contains in R4) in TaskStackPointer table */
					STR		R0, [R4, #4*8]		    /* Store R0 (return status) in previous task stack*/ 

					LSL		R7, #2					/* R7 = offset from start of table TaskStackPointer (new task index) */			
					LDR		R4, [R1, R7]		    /* Get back SP for the new task */
					LDR		R0, [R4, #4*8]			/* Restore R0 (status code) */

                	BX      LR 						/* End of function */

/* 
 * Main entry point for OS
 * This handler should have same priority as timertick (should not be preempted by timertick interrupt)
 *
 * Enter: 	R0 = Function call number
 *        	R1 = Parameter 1
 *		  	R2 = Parameter 2
 *        	R3 = Parameter 3
 *
 * Exit:	R0 = Function exit code
 */
					.global	SVC_Handler
					.thumb
					.thumb_func	
					.type	SVC_Handler, %function
SVC_Handler:        
					MRS		R12, PSP		/* R12 contains now the current stack pointer of the task */
					STMDB	R12!, {R4-R11}	/* Save remaining registers onto task stack -> All registers are saved */
					MOV		R4, R12		    /* And now, R4 contains the stack pointer of this task */

					LDR		R6, =CurrentTask
					LDR		R6, [R6]		/* Store in R6 the task id when entering system call */
											/* R6 = TaskId before system call */

					CMP		R0, #((OS_Jump_Table_End - OS_Jump_Table)/4) /* Check if R0 is greater than jump table size */
					BCC		SVC_OS_Call	   		/* If R0 is inside jump table, continue to the call of requested function */

SVC_Invalid_Fct:	MOV		R0, #E_INVALID_FCT	/* Otherwise load R0 with E_INVALID_FCT */
					B		SVC_Exit			/* And exit	 							 */

SVC_OS_Call:		LDR		R5, =OS_Jump_Table	/* Retrieved function @ in jump table */
					LSL		R0,	#2				/* Compute correct offset from start of table */
					LDR		R5, [R5, R0]		/* And load R5 with @ of requested function */
					
					PUSH	{LR}
					BLX		R5 	 				/* Call requested function of OS	*/
					POP		{LR}

					LDR		R7, =CurrentTask
					LDR		R7, [R7]	   	/* Store in R7 the task id after system call */
											/* R7 = TaskId after system call */
					CMP		R6, R7			/* If Task id before and after system call are different, task switch is needed */
					BEQ		SVC_Exit	 	/* If zero, no switch needed, pop the stack and exit */

					PUSH 	{LR}
					BL		OS_Switch_Tasks	/* Otherwise, swap previous task stack and selected task stack */
					POP		{LR}

SVC_Exit:			STR		R0, [R4, #4*8]  /* R4 contains SP for current task */		
					MOV		R12, R4		    /* Move back R4 into R12 */
					LDMFD	R12!,{R4-R11}   /* Restore register from stack and exit	*/
					MSR		PSP, R12 
					BX		LR				/* Leave SVC handler */	
					
/* 
 * RTC Timer entry point for OS
 */
					.global	SysTick_Handler
					.thumb
					.thumb_func	
					.type	SysTick_Handler, %function
SysTick_Handler: 
					MOV		R0, #TimerTick_Fct_Id			/* 0 is the function ID of the RTC */
                	B       SVC_Handler	 	/* Branch to SVC_Handler for continuing treatment  */

/* 
 * NMI entry point for OS (for call under IT)
 */
					.global	SysTick_Handler
					.thumb
					.thumb_func	
					.type	SysTick_Handler, %function
NMI_Handler: 
					/* R0  contains the function ID to be called */
                	B       SVC_Handler	 	/* Branch to SVC_Handler for continuing treatment  */

/* 
 * End of task fallback
 * This function @ is inserted at end of every task stack.
 * This way, if a task reach its end, and have not previously called TerminateTask,
 * the task end correctly.
 */
					.global	Task_End_Fallback
					.thumb
					.thumb_func	
					.type	Task_End_Fallback, %function
Task_End_Fallback: 
					MOV		R0, #TerminateTask_Fct_Id	/* Call TerminateTask service */
                	SVC     1	 	/* Branch to SVC_Handler for continuing treatment */
               
					.end
