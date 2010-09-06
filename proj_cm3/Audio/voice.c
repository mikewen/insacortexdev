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
#include "voice.h"


struct VOICE_ST voices[_NB_VOICES_];

extern u16 note_array[12][5]=
{
	/* Do, Do#,  Ré,   Ré#,  Mi,   Fa,   Fa#,  Sol,  Sol#, La,   La#,  Si */
	{8600, 8117, 7662, 7232, 6826, 6443, 6081, 5740, 5418, 5114, 4827, 4556},	/* octave 2 */
	{4300, 4059, 3831, 3616, 3413, 3221, 3041, 2870, 2709, },	/* octave 3 */
	{},	/* octave 4 */
	{},	/* octave 5 */
	{}	/* octave 6 */
};

#define _PERIODE_PWM_TIM3_ 	72000000U
#define PWM_MAX 			65535

void Init_Voice (void)
{
	/* timer 3 : oscillateurs */
	/* timer 2 : ADSR */
	/* timer 1 : modulation fm */

	/* Reglage du timer 3 -> PWM pour bras haut*/
	RCC->APB1ENR |= RCC_TIM3EN; /* Mise en route de l'horloge du timer 3 */

	TIM4->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM4->PSC = 0;
	TIM4->ARR = PWM_MAX; 

	TIM4->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM4->CCER = 0x1111;
	TIM4->CCMR2 = TIM_OC3M_VAL(TIM_OCxM_PWM_1) + TIM_CC3S_IS_OUTPUT + TIM_OC3PE ;

	TIM4->CCR3 = 0;	

	TIM4->DIER |= TIM_UIE; /* Active les IT overflow */
	 
	/* Regle les bras du hacheur en sortie */
	/* Reglage du port B */
	RCC->APB2ENR |= RCC_IOPBEN; /* Mise en route de l'horloge du port A */
	
	GPIOB->ODR |= GPIO_PIN_8; 	
	
	GPIOB->CRH &= ~((3<<GPIO_MODE_8_SHIFT) + (3<<GPIO_CNF_8_SHIFT));
	
	GPIOB->CRH |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_8_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_8_SHIFT);		 

	TIM4->CR1 |= TIM_CEN; 
}

void Regle_PWM(int val)
{
int tmp;

	tmp = val;

	if (tmp > 0x3FF) tmp = 0x3FF;
	if (tmp < 0) tmp =0;
	 
	TIM4->CCR3 = tmp;	
}

void TIM2_IRQHandler (void)
{
	TIM4->SR = 0;

	SEND_EVENT(PWM_OVERFLOW_EVENT);
}