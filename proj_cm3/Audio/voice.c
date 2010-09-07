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

const u16 note_array[5][12]=
{
	/* Do, Do#,  Ré,   Ré#,  Mi,   Fa,   Fa#,  Sol,  Sol#, La,   La#,  Si */
	{8600, 8117, 7662, 7232, 6826, 6443, 6081, 5740, 5418, 5114, 4827, 4556},	/* octave 2 */
	{4300, 4059, 3831, 3616, 3413, 3221, 3041, 2870, 2709, 2557, 2413, 2278},	/* octave 3 */
	{2150, 2029, 1915, 1808, 1706, 1611, 1520, 1435, 1354, 1278, 1207, 1139},	/* octave 4 */
	{1075, 1015, 958,  903,  853,  805,  760,  717,  677,  639,  603,  569 },	/* octave 5 */
	{538,  507,  479,  452,  427,  403,  380,  359,  339,  320,  302,  285 }	/* octave 6 */
};

volatile u16 voice_buffer[4];
u8 voice_note[4];
u8 voice_octave[4];
u16 voice_reload[4];
u16 voice_counter[4];

#define _PERIODE_PWM_TIM3_ 	72000000U
#define PWM_MAX 			65535

void Init_Voice (void)
{
	/* timer 3 : oscillateurs */
	/* timer 2 : ADSR */
	/* timer 1 : modulation fm */

	/* Reglage du timer 3 -> PWM pour bras haut*/
	RCC->APB1ENR |= RCC_TIM3EN; /* Mise en route de l'horloge du timer 3 */

	TIM3->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM3->PSC = 0;
	TIM3->ARR = PWM_MAX; 

	TIM3->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM3->CCER = 0x0;
	TIM3->CCMR1 = TIM_OC1M_VAL(TIM_OCxM_FROZEN) + TIM_OC1PE + TIM_OC2M_VAL(TIM_OCxM_FROZEN) + TIM_OC2PE;
	TIM3->CCMR2 = TIM_OC3M_VAL(TIM_OCxM_FROZEN) + TIM_OC3PE + TIM_OC4M_VAL(TIM_OCxM_FROZEN) + TIM_OC4PE;

	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;	

	TIM3->DIER |= TIM_CC1IE + TIM_CC2IE + TIM_CC3IE + TIM_CC4IE; /* Active les IT overflow */
	 
	TIM3->CR1 |= TIM_CEN; 
}

void Regle_Canal(int canal, int note, int octave)
{
	if (note==_VOICE_OFF_)
	{
		voice_buffer[canal]=0;
		//TIM3->CCER = TIM3->CCER & ~(0x01<<4*canal);
		switch (canal)
		{
		case 0:
			TIM3->CCMR1 = TIM3->CCMR1 & TIM_OC1M_VAL(TIM_OCxM_FROZEN);
			break;
		case 1:
			TIM3->CCMR1 = TIM3->CCMR1 & TIM_OC2M_VAL(TIM_OCxM_FROZEN);
			break;
		case 2:
			TIM3->CCMR2 = TIM3->CCMR2 & TIM_OC3M_VAL(TIM_OCxM_FROZEN);
			break;
		case 3:
			TIM3->CCMR2 = TIM3->CCMR2 & TIM_OC4M_VAL(TIM_OCxM_FROZEN);
		}
	}
	else
	{
		voice_reload[canal]=note_array[octave][note];

		//TIM3->CCER = TIM3->CCER | (0x01<<4*canal);

		switch (canal)
		{
		case 0:
			TIM3->CCR1 = TIM3->CNT + voice_reload[canal];
			TIM3->CCMR1 = TIM3->CCMR1 | TIM_OC1M_VAL(TIM_OCxM_TOGGLE);
			break;
		case 1:
			TIM3->CCR2 = TIM3->CNT + voice_reload[canal];
			TIM3->CCMR1 = TIM3->CCMR1 | TIM_OC2M_VAL(TIM_OCxM_TOGGLE);
			break;
		case 2:
			TIM3->CCR3 = TIM3->CNT + voice_reload[canal];
			TIM3->CCMR2 = TIM3->CCMR2 | TIM_OC3M_VAL(TIM_OCxM_TOGGLE);
			break;
		case 3:
			TIM3->CCR4 = TIM3->CNT + voice_reload[canal];
			TIM3->CCMR2 = TIM3->CCMR2 | TIM_OC4M_VAL(TIM_OCxM_TOGGLE);
		}
	}	
}

void TIM3_IRQHandler (void)
{
u32 SR;
u16 CNT;

	CNT = TIM3->CNT;
	SR= TIM3->SR;

	if (SR & TIM_CC1IF)
	{
		TIM3->CCR1 = CNT + voice_reload[0];
		
		TIM3->SR = TIM3->SR & ~(TIM_CC1IF);	

		voice_buffer[0]=Waveforms[0][voice_counter[0]];
		voice_counter[0] = (voice_counter[0]+1) & 0x3F;
	}

	if (SR & TIM_CC2IF)
	{
		TIM3->CCR2 = CNT + voice_reload[1];
		
		TIM3->SR = TIM3->SR & ~(TIM_CC2IF);	
	}

	if (SR & TIM_CC3IF)
	{
		TIM3->CCR3 = CNT + voice_reload[2];
		
		TIM3->SR = TIM3->SR & ~(TIM_CC3IF);	
	}

	if (SR & TIM_CC4IF)
	{
		TIM3->CCR4 = CNT + voice_reload[3];
		
		TIM3->SR = TIM3->SR & ~(TIM_CC4IF);	
	}
}