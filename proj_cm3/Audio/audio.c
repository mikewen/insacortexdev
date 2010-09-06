/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * MinOS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * MinOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with MinOS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"
#include "stm_clock.h"
#include "stm_usartx.h"
#include "pwm.h"

void Callback_pwm(void);

void Init_Periphs(void)
{
	/* Reglage des horloges systeme (40Mhz)*/
	Init_Clock_System();

	/* Liaison serie 19400/8/N/1 */
	setup_usart();

	/* Activation de la sortie audio (timer 4)*/
	Init_PWM (Callback_pwm);

	/* Activation des oscillateur de voix */


	/* Activation du systick à 80 Khz */

}

int main (void)
{
	Init_Periphs();

	return 0;
}

void Systick_Handler(void)
{
	
}

void Callback_pwm(void)
{
} 