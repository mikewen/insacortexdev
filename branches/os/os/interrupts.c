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
#include "kernel.h"
#include "interrupts.h"

u8 InterruptCounter;

/* Interrupt services declaration */
void EnableAllInterrupts(void)
{
}

void DisableAllInterrupts(void)
{
}

void ResumeAllInterrupts(void)
{
}

void SuspendAllInterrupts(void)
{
}

void ResumeOSInterrupts(void)
{
}

void SuspendOSInterrupts(void)
{
}

void Interrupt_Init(void)
{
	DisableAllInterrupts();

	InterruptCounter=1;
}
