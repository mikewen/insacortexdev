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
					.file	"call_wrapper.S"

					.section ".text"
					.align	2

/* 
 * Call wrapper
 */
					.global	OSCallWrapper_0
					.thumb
					.thumb_func	
					.type	OSCallWrapper_0, %function
OSCallWrapper_0: 

					.global	OSCallWrapper_1
					.thumb
					.thumb_func	
					.type	OSCallWrapper_1, %function
OSCallWrapper_1:

					.global	OSCallWrapper_2
					.thumb
					.thumb_func	
					.type	OSCallWrapper_2, %function
OSCallWrapper_2:

					.global	OSCallWrapper_3
					.thumb
					.thumb_func	
					.type	OSCallWrapper_3, %function
OSCallWrapper_3:                 
					PUSH	{R1-R3}
					MRS		R1, IPSR  		/* R1 contains IPSR register, Reserved bits read as 0 */
					CMP		R1, #0
					BEQ		Wrapper_NotIT	/* If (IPSR & 0x1FF) == 0, we are not under IT */
					
					PUSH 	{LR}
					BL		OS_Call			/* If under IT, call directly OS service */
					POP		{LR}		

					/* And then, record a pending PendSV interrupt */
					MOVW	R1, #0xED04		/* R1 contains address of SCB_ICSR register, used */
					MOVT	R1,	#0xE000		/* for pending PendSV interrupt */
					                           
					MOVW	R2, #0x0000		/* R2 contains mask used to set PENDSVSET bit	*/
					MOVT	R2, #0x1000		/* in SCB_ICSR register */

					LDR		R3, [R1]
					AND		R3, R2
					STR		R3, [R1]

					POP		{R1-R3}
					BX		LR

Wrapper_NotIT:		POP 	{R1-R3}
					SVC		1 				/* Call OS service (RQ: the immediate value is meaningless)*/
Wrapper_End:		BX		LR				/* get back to main appli */
                
					.end
