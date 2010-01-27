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
OS_Jump_Table_End:

/*					
 * Switch routine
 */
					.global	OS_Switch_Tasks
					.thumb
					.thumb_func
					.type	OS_Switch_Tasks, %function
OS_Switch_Tasks:
                	B       . 				/* TODO: Write the switching function  */


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
SVC_Handler:        PUSH 	{R4-R11} 		/* Save remaining registers onto system stack -> All registers are saved */

					LDR		R6, =CurrentTask
					LDR		R6, [R6]		/* Store in R6 the task id when entering system call */
											/* R6 = TaskId before system call */

					CMP		R0, #((OS_Jump_Table_End - OS_Jump_Table)/4) /* Check if R0 is greater than jump table size */
					BCC		SVC_OS_Call	   	/* If R0 is inside jump table, continue to the call of requested function */

SVC_Invalid_Fct:	MOV		R0, #E_INVALID_FCT	/* Otherwise load R0 with E_INVALID_FCT */
					B		SVC_Exit		/* And exit	 							 */

SVC_OS_Call:		LDR		R4, =OS_Jump_Table	/* Retrieved function @ in jump table */
					LSL		R0,	#2				/* Compute correct offset from start of table */
					LDR		R4, [R4, R0]		/* And load R4 with @ of requested function */
					PUSH	{LR}
					BLX		R4 	 			/* Call requested function of OS	*/
					POP		{LR}

					LDR		R7, =CurrentTask
					LDR		R7, [R7]	   	/* Store in R7 the task id after system call */
											/* R7 = TaskId after system call */
					EOR		R6, R7			/* R6 = R6 XOR R7 => If Task id before and after system call are different (R6 != 0), task switch is needed */
					CBZ		R6, SVC_Exit 	/* If zero, no switch needed, pop the stack and exit */

					PUSH 	{LR}
					BL		OS_Switch_Tasks	/* Otherwise, swap previous task stack and selected task stack */
					POP		{LR}

SVC_Exit:			STR		R0, [SP, #4*8]
					POP		{R4-R11} 		/* Restore register from stack and exit	*/
					BX		LR				/* Leave SVC handler */		

/* 
 * RTC Timer entry point for OS
 */
					.global	SysTick_Handler
					.thumb
					.thumb_func	
					.type	SysTick_Handler, %function
SysTick_Handler: 
					MOV		R0, #TimerTick_Fct_Id			/* 0 is the function ID of the RTC 				  */
                	B       SVC_Handler	 	/* Branch to SVC_Handler for continuing treatment */

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
					MOV		R0, #TerminateTask_Fct_Id			/* Call TerminateTask service */
                	B       SVC_Handler	 	/* Branch to SVC_Handler for continuing treatment */
                
					.end
