/*****************************************************************************/
/* STM32F10x.s: Startup file for ST STM32F10x device series                  */
/*****************************************************************************/
/* <<< Use Configuration Wizard in Context Menu >>>                          */
/*****************************************************************************/
/* This file is part of the uVision/ARM development tools.                   */
/* Copyright (c) 2005-2007 Keil Software. All rights reserved.               */
/* This software may only be used under the terms of a valid, current,       */
/* end user licence from KEIL for a compatible version of KEIL software      */
/* development tools. Nothing else gives you the right to use this software. */
/*****************************************************************************/
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

OS_Jump_Table:		.long	OS_Timer_Tick
					.long	OS_Create_Task
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
 * Remember: 
 * When this handler is called, R0 hold function id, R1, R2 and R3 parameters
 * At end, R0 hold return info (status most of time) 
 * This handler should have same priority as timertick (should not be preempted by timertick interrupt)
 */
					.global	SVC_Handler
					.thumb
					.thumb_func	
					.type	SVC_Handler, %function
SVC_Handler:                  
					PUSH 	{R4-R11} 		/* Save remaining registers onto system stack -> All registers are saved */

					CMP		R0, #((OS_Jump_Table_End - OS_Jump_Table)/4) /* Check if R0 is greater than jump table size */
					BCC		SVC_OS_Call	   	/* If R0 is inside jump table, continue to the call of requested function */

SVC_Invalid_Fct:	MOV		R0, #-1			/* Otherwise load R0 with OS_INVALID_FCT */
					B		SVC_Exit		/* And exit	 							 */

SVC_OS_Call:		LDR		R4, =OS_Jump_Table	/* Retrieved function @ in jump table */
					LSL		R0,	#2				/* Compute correct offset from start of table */
					LDR		R4, [R4, R0]		/* And load R4 with @ of requested function */
					PUSH	{LR}
					BLX		R4 	 			/* Call requested function of OS	*/
					POP		{LR}

					LDR		R4, =OS_Task_Switch_Request /* Get task switching info: */
					LDR		R4, [R4]
					CBZ		R4, SVC_Exit 				/* if zero, no switch needed, pop the stack and exit */

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
					MOV		R0, #0			/* 0 is the function ID of the RTC 				  */
                	B       SVC_Handler	 	/* Branch to SVC_Handler for continuing treatment */
                
					.end
