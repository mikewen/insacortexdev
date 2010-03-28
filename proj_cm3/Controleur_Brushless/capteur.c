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
#include "callback.h"

eventptr CAPTEUR_POSITION_AVANT_EVENT;
eventptr CAPTEUR_POSITION_APRES_EVENT;
eventptr CAPTEUR_SEUIL_HAUT_EVENT;
eventptr CAPTEUR_SEUIL_BAS_EVENT;

int periode_capteur;
float vitesse_moteur;
int sens_rotation;

void Init_Capteur (void)
{
	/* Reglage du timer 3 -> Encodeur */
	RCC->APB1ENR |= RCC_TIM3EN; /* Mise en route de l'horloge du timer 4 */

	TIM3->SMCR |= TIM_SMS_IS_ENCODER_3;	/* Reglage en mode encodeur */
	TIM3->CCMR1 |= TIM_CC2S_IS_INPUT_TI2 | TIM_CC1S_IS_INPUT_TI1; /* IC1FP1 sur TI1, IC2FP2, sur TI2 */
	//TIM3->CCER &= ~((CC1P) | (CC2P)); /* Capture sur les fronts montants */
	
	TIM3->ARR = _RESOLUTION_CAPTEUR_*4; /* Amplitude du timer = Resolution du capteur*4 */
	TIM3->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */ 
	
	TIM3->CR1 |= TIM_CEN; 
	
	TIM3->DIER |= TIM_CC2IE + TIM_CC1IE + TIM_CC4IE + TIM_CC3IE;

	/* Reglage du port B */
	RCC->APB2ENR |= RCC_IOPBEN; /* Mise en route de l'horloge du portB */	
//	PORTB->CRL |= 

}

int Lire_Capteur(void)
{
	return TIM3->CNT;
}

void Ecrire_Capteur(int val)
{
	TIM3->CNT = val;
}

void Regle_Position_Avant(int val)
{
	TIM3->CCR1=val;
}

void Regle_Position_Apres(int val)
{
	TIM3->CCR2=val;
}

void Regle_Seuil_Vitesse_Haut(int val)
{
}

void Regle_Seuil_Vitesse_Bas(int val)
{
}

void TIM3_IRQHandler(void)
{
int SR_TMP;

	SR_TMP=TIM3->SR;

		if (SR_TMP&TIM_CC1IF)
		{
			TIM3->SR = TIM3->SR & ~(TIM_CC1IF);	
		}
		
		if (SR_TMP&TIM_CC2IF)
		{
			TIM3->SR = TIM3->SR & ~(TIM_CC2IF);	
		}	

		if (SR_TMP&TIM_CC3IF)
		{
			TIM3->SR = TIM3->SR & ~(TIM_CC3IF);	
		}
		
		if (SR_TMP&TIM_CC4IF)
		{
			TIM3->SR = TIM3->SR & ~(TIM_CC4IF);	
		}	
}
