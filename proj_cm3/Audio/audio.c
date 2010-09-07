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
#include "voice.h"

void Callback_pwm(void);

int compteur;
int note;
int octave;

void Init_Periphs(void)
{
	/* Reglage des horloges systeme (40Mhz)*/
	Init_Clock_System();

	/* Liaison serie 19400/8/N/1 */
	setup_usart();

	/* Activation de la sortie audio (timer 4)*/
	Init_PWM ();

	/* Activation des oscillateur de voix */
	Init_Voice ();

	/* Activation du systick à 1 Khz */
	SysTick->LOAD = 72000;
	SYSTICK_ENABLE_IT();
	SYSTICK_CLOCK_AHB();
	SYSTICK_ENABLE_COUNTER();

	/* Activation des IT */
	NVIC_SET_PRIO_PERIPH(TIM3, 5);
	NVIC_SET_PRIO_PERIPH(TIM4, 10);
	NVIC_SET_PRIO_SYSTEM(SYSTICK,14);

	NVIC_ENABLE_PERIPH_IT(TIM3);
	NVIC_ENABLE_PERIPH_IT(TIM4);
}

int main (void)
{
	compteur=0;
	note=_DO_;
	octave=0;

	Init_Periphs();

	while (1);
	return 0;
}

void SysTick_Handler(void)
{
	compteur++;

	if (compteur >=200)
	{
		compteur =0;

		Regle_Canal(0,note, octave);
		note++;

		if (note>_SI_) 
		{
			note = _DO_;
			octave++;
		}

		if (octave>5) octave=0;
	}
}
