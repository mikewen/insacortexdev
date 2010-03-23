/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of brushless DC motor controller.
 *
 * Brushless DC motor controller is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Brushless DC motor controller is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Brushless DC motor controller; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"
#include "capteur.h"

#include "config.h"

void Init_Capteur (void)
{
	/* Reglage du timer 4 -> Encodeur */
	RCC->APB1ENR |= RCC_TIM4EN; /* Mise en route de l'horloge du timer 4 */

	TIM4->SMCR |= TIM_SMS_IS_ENCODER_3;	/* Reglage en mode encodeur */
	TIM4->CCMR1 |= TIM_CC2S_IS_INPUT_TI2 | TIM_CC1S_IS_INPUT_TI1; /* IC1FP1 sur TI1, IC2FP2, sur TI2 */
	//TIM4->CCER &= ~((CC1P) | (CC2P)); /* Capture sur les fronts montants */
	
	TIM4->ARR = _RESOLUTION_CAPTEUR_; /* Amplitude du timer = Resolution du capteur */
	TIM4->CNT = _RESOLUTION_CAPTEUR_+1; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */ 
	
	TIM4->CR1 |= TIM_CEN; 	
}

int Lire_Capteur(void)
{
	return 0;
}

void Ecrire_Capteur(int val)
{
}

void Regle_Capteur_Haut(int val)
{
}

void Regle_Capteur_Bas(int val)
{
}


