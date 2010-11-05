/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * lib_cm3 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * lib_cm3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with lib_cm3; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"
#include "stm_clock.h"
#include "lib_flash.h"

u16 write_buffer[32];
int i;

int main (void)
{
	/* Reglage des horloges systeme (40Mhz)*/
	//Init_Clock_System();

	for (i=0; i<32; i++)
	{
		write_buffer[i]=i;
	}

	FLASH_ErasePage(0x801FC00);
	FLASH_Program(0x801FC00, write_buffer, 32);

	for(;;)
	{
	}

	return 0;
}