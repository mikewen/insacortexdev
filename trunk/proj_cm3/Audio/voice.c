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
#include "waveform.h"

struct VOICE_ST voices[_NB_VOICES_];

u16 note_array[5][12]=
{
	/* Do, Do#,  Ré,   Ré#,  Mi,   Fa,   Fa#,  Sol,  Sol#, La,   La#,  Si */
	{8600, 8117, 7662, 7232, 6826, 6443, 6081, 5740, 5418, 5114, 4827, 4556},	/* octave 2 */
	{4300, 4059, 3831, 3616, 3413, 3221, 3041, 2870, 2709, 2557, 2413, 2278},	/* octave 3 */
	{2150, 2029, 1915, 1808, 1706, 1611, 1520, 1435, 1354, 1278, 1207, 1139},	/* octave 4 */
	{1075, 1015, 958,  903,  853,  805,  760,  717,  677,  639,  603,  569 },	/* octave 5 */
	{538,  507,  479,  452,  427,  403,  380,  359,  339,  320,  302,  284 }	/* octave 6 */
};

volatile u16 voice_buffer[4];
u8 voice_note[4];
u8 voice_octave[4];
u16 voice_reload[4];
u16 voice_counter[4];

volatile int TIM3_cnt;

volatile int SR;
volatile int CCR1;
volatile int CCMR1;
volatile int CCMR2;
volatile int CCER;
volatile int CNT;

int waveform_nbr;

#define _PERIODE_PWM_TIM3_ 	72000000U
#define PWM_MAX 			65535

void Init_Voice (void)
{
	/* timer 3 : oscillateurs */
	/* timer 2 : ADSR */
	/* timer 1 : modulation fm */

	/* Reglage du timer 1 -> Canal 0*/
	RCC->APB2ENR |= RCC_TIM1EN; /* Mise en route de l'horloge du timer 1 */

	TIM1->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM1->PSC = 0;
	TIM1->ARR = PWM_MAX; 

	TIM1->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	

	TIM1->DIER |= TIM_UIE; /* Active les IT overflow */

	/* Reglage du timer 2 -> Canal 1*/
	RCC->APB1ENR |= RCC_TIM2EN; /* Mise en route de l'horloge du timer 3 */

	TIM2->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM2->PSC = 0;
	TIM2->ARR = PWM_MAX; 

	TIM2->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	

	TIM2->DIER |= TIM_UIE; /* Active les IT overflow */

	/* Reglage du timer 3 -> Canal 2*/
	RCC->APB1ENR |= RCC_TIM3EN; /* Mise en route de l'horloge du timer 3 */

	TIM3->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM3->PSC = 0;
	TIM3->ARR = PWM_MAX; 

	TIM3->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	

	TIM3->DIER |= TIM_UIE; /* Active les IT overflow */ 

	waveform_nbr = 2;
}

void Regle_Canal(int canal, int note, int octave)
{
	if (note==_VOICE_OFF_)
	{
		voice_buffer[canal]=0;
		voice_reload[canal]=0;

		switch (canal)
		{
		case 0:
			TIM1->CR1 &= ~TIM_CEN;
			break;
		case 1:
			TIM2->CR1 &= ~TIM_CEN;
			break;
		case 2:
			TIM3->CR1 &= ~TIM_CEN;
			break;
		}
	}
	else
	{
		voice_reload[canal]=note_array[octave][note];

		switch (canal)
		{
		case 0:
			TIM1->ARR = voice_reload[canal];
			TIM1->CR1 |= TIM_CEN;
			break;
		case 1:
			TIM2->ARR = voice_reload[canal];
			TIM2->CR1 |= TIM_CEN;
			break;
		case 2:
			TIM3->ARR = voice_reload[canal];
			TIM3->CR1 |= TIM_CEN;
			break;
		}
	}	
}

void TIM1_UP_IRQHandler (void)
{
	if (TIM1->SR & TIM_UIF)
	{
		TIM1->SR = TIM1->SR & ~(TIM_UIF);		

		voice_buffer[0]=Waveforms[waveform_nbr][voice_counter[0]];
		voice_counter[0]++;
		if (voice_counter[0]>=64) voice_counter[0]=0;
	}
}

void TIM2_IRQHandler (void)
{
	if (TIM2->SR & TIM_UIF)
	{
		TIM2->SR = TIM2->SR & ~(TIM_UIF);		

		voice_buffer[1]=Waveforms[waveform_nbr][voice_counter[1]];
		voice_counter[1]++;
		if (voice_counter[1]>=64) voice_counter[1]=0;
	}
}

void TIM3_IRQHandler (void)
{
	if (TIM3->SR & TIM_UIF)
	{
		TIM3->SR = TIM3->SR & ~(TIM_UIF);		

		voice_buffer[2]=Waveforms[waveform_nbr][voice_counter[2]];
		voice_counter[2]++;
		if (voice_counter[2]>=64) voice_counter[2]=0;
	}
}