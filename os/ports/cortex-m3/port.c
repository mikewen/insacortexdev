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
#include "port.h"

void Task_End_Fallback(void);

#define INITIAL_REG		0xDEADC0DE
#define INITIAL_PSR		0x01000000
#define INITIAL_LR		0xFFFFFFF9

const u32 StartupStack[] = 
{
	INITIAL_REG,											// R0 (en tant que code retour de la fonction, ici sans signification)
	INITIAL_LR,												// LR (celui pushé avant d'appeler OS_switch)
	INITIAL_REG, INITIAL_REG, INITIAL_REG, INITIAL_REG,		// R4, R5, R6, R7
	INITIAL_REG, INITIAL_REG, INITIAL_REG, INITIAL_REG,		// R8, R9, R10, R11
	0, INITIAL_REG, INITIAL_REG, INITIAL_REG,		// R0, R1, R2, R3
	INITIAL_REG,											// R12
	(u32)Task_End_Fallback,				// Link @ vers Task_End_Fallback
	0,									// Initial PC: a completer avec le point d'entrée de la tache
	INITIAL_PSR							// Initial PSR, seul le bit T (Thumb) est positionné
};

void FastCopy (u32 *dest, u32 *src, u32 len)
{
	while (len!=0)
	{
		*dest= *src;
		dest++;
		src++;
		len--; 
	}
}
