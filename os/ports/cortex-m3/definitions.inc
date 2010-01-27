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

		.equ 	E_INVALID_FCT,				-1	  		/* Invalid function */

/* Function ID definition */

		.equ	TimerTick_Fct_Id,			0
		.equ 	ActivateTask_Fct_Id,		1
		.equ	TerminateTask_Fct_Id,		2
		.equ	ChaineTask_Fct_Id,			3
		.equ	Schedule_Fct_Id,			4

