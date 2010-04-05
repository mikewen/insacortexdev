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
void Default_Callback(void);

void Init_Capteur (void)
{
	/* Reglage des callback par defaut */
	DEFINE_EVENT(CAPTEUR_POSITION_AVANT, Default_Callback);
	DEFINE_EVENT(CAPTEUR_POSITION_APRES, Default_Callback);
	DEFINE_EVENT(CAPTEUR_SEUIL_HAUT, Default_Callback);
	DEFINE_EVENT(CAPTEUR_SEUIL_BAS, Default_Callback);

	/* Reglage du port B */
	RCC->APB2ENR |= RCC_IOPBEN; /* Mise en route de l'horloge du port B */	
	GPIOB->CRL &= ~((3<<GPIO_MODE_6_SHIFT) + (3<<GPIO_CNF_6_SHIFT) +
	               (3<<GPIO_MODE_7_SHIFT) + (3<<GPIO_CNF_7_SHIFT));
	GPIOB->CRL |= ((GPIO_MODE_INPUT<<GPIO_MODE_6_SHIFT) + (GPIO_CNF_DIGITAL_INPUT<<GPIO_CNF_6_SHIFT) +
	               (GPIO_MODE_INPUT<<GPIO_MODE_7_SHIFT) + (GPIO_CNF_DIGITAL_INPUT<<GPIO_CNF_7_SHIFT));

	/* Reglage du timer 4 -> Encodeur */
	RCC->APB1ENR |= RCC_TIM4EN; /* Mise en route de l'horloge du timer 4 */

	TIM4->ARR = _RESOLUTION_CAPTEUR_*4; /* Amplitude du timer = Resolution du capteur*4 */
	TIM4->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */ 
	TIM4->PSC = 300;
//	TIM4->PSC = 0;

//	TIM4->SMCR |= TIM_SMS_IS_ENCODER_3;	/* Reglage en mode encodeur */
	TIM4->SMCR |= TIM_SMS_IS_DISABLED;	/* Reglage en mode encodeur */

//	TIM4->CCMR1 |= TIM_CC2S_IS_INPUT_TI2 + TIM_OC2M_VAL(TIM_OCxM_ACTIVE_LEVEL_ON_MATCH) + 
//	               TIM_CC1S_IS_INPUT_TI1 + TIM_OC1M_VAL(TIM_OCxM_ACTIVE_LEVEL_ON_MATCH); /* IC1FP1 sur TI1, IC2FP2, sur TI2 */
	//TIM3->CCER &= ~((CC1P) | (CC2P)); /* Capture sur les fronts montants */
	
	//TIM4->CCMR1 = TIM_OC2M_VAL(TIM_OCxM_TOGGLE) + TIM_OC1M_VAL(TIM_OCxM_TOGGLE);

	//TIM4->CR1 |= TIM_CEN; 
	
	TIM4->DIER |= TIM_CC2IE + TIM_CC1IE;
}

void Demarre_Capteur(void)
{
	Ecrire_Capteur(0);
	TIM4->CR1 |= TIM_CEN; 
}

int Lire_Capteur(void)
{
	return TIM4->CNT;
}

void Ecrire_Capteur(int val)
{
	TIM4->CNT = val;
}

void Regle_Position_Avant(int val)
{
	TIM4->CCR1=val;
	TIM4->CCMR1 |= TIM_OC1M_VAL(TIM_OCxM_TOGGLE);
}

void Regle_Position_Apres(int val)
{
	TIM4->CCR2=val;
	TIM4->CCMR1 |= TIM_OC1M_VAL(TIM_OCxM_TOGGLE);
}

void Regle_Seuil_Vitesse_Haut(int val)
{
}

void Regle_Seuil_Vitesse_Bas(int val)
{
}

void TIM4_IRQHandler(void)
{
int SR_TMP;

	SR_TMP=TIM4->SR;

		if (SR_TMP&TIM_CC1IF)
		{
			TIM4->SR = TIM4->SR & ~(TIM_CC1IF);	
			SEND_EVENT(CAPTEUR_POSITION_AVANT_EVENT);
		}
		
		if (SR_TMP&TIM_CC2IF)
		{
			TIM4->SR = TIM4->SR & ~(TIM_CC2IF);	
			SEND_EVENT(CAPTEUR_POSITION_APRES_EVENT);
		}	
	
}

void Default_Callback(void)
{
	while (1);
}
