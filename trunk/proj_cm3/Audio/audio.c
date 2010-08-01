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

void Init_Periphs(void)
{
	/* Reglage des horloges systeme (40Mhz)*/
	Init_Clock_System();

	/* Liaison serie 19400/8/N/1 */
	setup_usart();

	/* Timer 2, ch 1 = sortie audio, 40 Khz */
	RCC->APB1ENR |= RCC_TIM2EN; /* Mise en route de l'horloge du timer 2 */

	TIM2->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM2->PSC = 0;
	TIM2->ARR = 1000; /* Periode de PWM -> 40Khz */

	TIM2->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM2->CCER = 0x3303;
	TIM2->CCMR1 = TIM_OC1M_VAL(TIM_OCxM_PWM_1) + TIM_CC1S_IS_OUTPUT + TIM_OC1PE ;

	TIM2->CCR1 = 0;

	TIM2->CR1 |= TIM_CEN;

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

