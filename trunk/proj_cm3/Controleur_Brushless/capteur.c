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
#include "appli.h"

volatile int nb_tours;
volatile int position_actuelle;
volatile int ancienne_position; 

volatile int vitesse_moteur;

void Init_Capteur (void)
{
	/* Reglage du port B */
	RCC->APB2ENR |= RCC_IOPBEN; /* Mise en route de l'horloge du port B */	
	GPIOB->CRL &= ~((3<<GPIO_MODE_6_SHIFT) + (3<<GPIO_CNF_6_SHIFT) +
	               (3<<GPIO_MODE_7_SHIFT) + (3<<GPIO_CNF_7_SHIFT));
	GPIOB->CRL |= ((GPIO_MODE_INPUT<<GPIO_MODE_6_SHIFT) + (GPIO_CNF_DIGITAL_INPUT<<GPIO_CNF_6_SHIFT) +
	               (GPIO_MODE_INPUT<<GPIO_MODE_7_SHIFT) + (GPIO_CNF_DIGITAL_INPUT<<GPIO_CNF_7_SHIFT));

	/* Reglage du timer 4 -> Encodeur */
	RCC->APB1ENR |= RCC_TIM4EN; /* Mise en route de l'horloge du timer 4 */

	TIM4->ARR = _RESOLUTION_ENCODEUR_ - 1; /* Amplitude du timer = Resolution du capteur*2 -1 */
	TIM4->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */ 
	TIM4->PSC = 0;

	TIM4->SMCR |= TIM_SMS_IS_ENCODER_3;	/* Reglage en mode encodeur */

	TIM4->CCMR1 |= TIM_CC2S_IS_INPUT_TI2 +  
	               TIM_CC1S_IS_INPUT_TI1; /* IC1FP1 sur TI1, IC2FP2, sur TI2 */
	
    /*Regle le SYSTICK pour mesurer les durées du capteur */
	SYSTICK_ENABLE_COUNTER(); /* Mise en route de l'horloge du timer1 */
	SYSTICK_CLOCK_AHB();
	SysTick->LOAD = 2000000; /* 50ms*/
	SYSTICK_ENABLE_IT();

	TIM4->DIER |= TIM_UIE;

	nb_tours=0;
	position_actuelle=0;
	ancienne_position=0;
	vitesse_moteur=0;
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

int Donne_Vitesse(void)
{
	return vitesse_moteur;
}

int Donne_nb_tour(void)
{
	return nb_tours;
}

void TIM4_IRQHandler (void)
{
	if (TIM4->SR & TIM_UIF)
	{
		TIM4->SR = TIM4->SR & ~TIM_UIF;

		if (TIM4->CR1 & TIM_DIR) /* le compteur decompte */
		{
			nb_tours--;
		}
		else
		{
			nb_tours++;
		}
	}
}

void SysTick_Handler()
{
int temp;
	temp = TIM4->CNT;

	ancienne_position = position_actuelle;
	position_actuelle = (nb_tours*_RESOLUTION_ENCODEUR_) + temp;

	vitesse_moteur = position_actuelle - ancienne_position;
	vitesse_moteur = vitesse_moteur * 25; /* Vitesse moteur exprimé en dixieme de tour/minute */
	
	Gere_Asservissement(nb_tours, vitesse_moteur);	  
}
