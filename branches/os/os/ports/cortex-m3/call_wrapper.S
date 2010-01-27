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
					SVC		1 				/* Call OS service (RQ: the value is meaningless)*/
					BX		LR				/* get back to main appli */
                
					.end
