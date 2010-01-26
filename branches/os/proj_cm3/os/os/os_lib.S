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
					.file	"os_lib.S"

					.section ".text"
					.align	2

/* 
 * Main entry point for OS
 * Remember: 
 * When this handler is called, R0 hold function id, R1, R2 and R3 parameters
 * At end, R0 hold return info (status most of time) 
 * This handler should have same priority as timertick (should not be preempted by timertick interrupt)
 */
					.global	OS_Func_1
					.thumb
					.thumb_func	
					.type	OS_Func_1, %function
OS_Func_1:                  
					SVC		1 				/* Call OS */
					BX		LR				/* get back to main appli */
                
					.end
