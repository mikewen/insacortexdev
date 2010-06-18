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

#define _SYSTICK_MAX_ (unsigned int)0xFFFFFF

volatile unsigned int periode_capteur;
int dernier_systick;

unsigned int vitesse_moteur;
int tour;

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
	SysTick->LOAD = _SYSTICK_MAX_; /* 2^24 -1 */
	SYSTICK_ENABLE_IT();

	TIM4->DIER |= TIM_CC3IE + TIM_CC4IE;

	periode_capteur = _PERIODE_VITESSE_NULLE_;
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

int Lire_Capteur_Avant(void)
{
  	return (TIM4->CCR3);
}

int Lire_Capteur_Arriere(void)
{
	return (TIM4->CCR4);
}

void Ecrire_Capteur(int val)
{
	TIM4->CNT = val;

	Regle_Position_Avant(val+10);
	Regle_Position_Arriere(val-10);
}

int Donne_Vitesse(void)
{
int temp;

	if (periode_capteur == _PERIODE_VITESSE_NULLE_) return 0;

	temp = periode_capteur * 10;
	temp = 40000000/temp;
	vitesse_moteur = 60*temp;

	return -vitesse_moteur;
}

int Donne_nb_tour(void)
{
	return tour;
}

void Regle_Position_Avant(int val)
{
	if (val<_RESOLUTION_ENCODEUR_ - 1)
	{
		TIM4->CCR3=val;
	}
	else
	{
		TIM4->CCR3=val - _RESOLUTION_ENCODEUR_;
	}

	TIM4->CCMR2 |= TIM_OC3M_VAL(TIM_OCxM_TOGGLE);
}

void Regle_Position_Arriere(int val)
{
	if (val<_RESOLUTION_ENCODEUR_ - 1)
	{
		TIM4->CCR4=val;
	}
	else
	{
		TIM4->CCR4=val - _RESOLUTION_ENCODEUR_;
	}

	TIM4->CCMR2 |= TIM_OC4M_VAL(TIM_OCxM_TOGGLE);
}

void TIM4_IRQHandler(void)
{
int SR_TMP;
int Valeur_SYSTICK = SysTick->VAL;
int Compteur = TIM4->CNT;

	SR_TMP=TIM4->SR;

		if (SR_TMP&TIM_CC3IF) /* Capteur_avant */
		{
			TIM4->SR = TIM4->SR & ~(TIM_CC3IF);	
			Regle_Position_Avant(Compteur+10);
			Regle_Position_Arriere(Compteur-10);

			tour++;
						
			if (dernier_systick<=Valeur_SYSTICK)
			{
				periode_capteur= _SYSTICK_MAX_ + dernier_systick - Valeur_SYSTICK;
			}
			else
			{
				periode_capteur= dernier_systick - Valeur_SYSTICK;
			}
		}
		
		if (SR_TMP&TIM_CC4IF) /* Capteur_Arriere */
		{
			TIM4->SR = TIM4->SR & ~(TIM_CC4IF);	
			Regle_Position_Avant(Compteur+10);
			Regle_Position_Arriere(Compteur-10);

			tour --;

			if (dernier_systick<=Valeur_SYSTICK)
			{
				periode_capteur= _SYSTICK_MAX_ + dernier_systick - Valeur_SYSTICK;
			}
			else
			{
				periode_capteur= dernier_systick - Valeur_SYSTICK;
			}

			periode_capteur = -periode_capteur;
		}
		
		dernier_systick = Valeur_SYSTICK;	
}

void SysTick_Handler()
{
static int overflow =0;

	overflow++;

	if (overflow>=2)
	{
		overflow =2;
		periode_capteur = _PERIODE_VITESSE_NULLE_;
	}
}
