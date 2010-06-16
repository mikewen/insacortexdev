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
#include "hacheur.h"

#include "config.h"
#include "callback.h"

#define _DEAD_TIME_ 4;
//#define _DEAD_TIME_ DeadTime;
int DeadTime;

eventptr PWM_OVERFLOW_EVENT;

void Default_Callback_Hacheur (void)
{
}

/* 
 * Init_Hacheur
 *
 * Initialisation des bras haut et bas du hacheur
 * Bras haut (PMOS) connecté au timer 2 (canaux 1,3 et 4)
 * Bras bas (NMOS) connecté aux GPIO (pas de timer)
 */
void Init_Hacheur (void)
{
	DeadTime = 500;

	/* Init du callback sur une fonction vide */
	DEFINE_EVENT(PWM_OVERFLOW, Default_Callback_Hacheur);

	/* Reglage du timer 2 -> PWM pour bras haut*/
	RCC->APB1ENR |= RCC_TIM2EN; /* Mise en route de l'horloge du timer 2 */

	TIM2->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM2->PSC = 0;
	TIM2->ARR = _PERIODE_PWM_TIM2_; /* Periode de PWM -> 20Khz */

	TIM2->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM2->CCER = 0x3303;
	TIM2->CCMR1 = TIM_OC1M_VAL(TIM_OCxM_PWM_1) + TIM_CC1S_IS_OUTPUT + TIM_OC1PE ;
	TIM2->CCMR2 = TIM_OC3M_VAL(TIM_OCxM_PWM_1) + TIM_CC3S_IS_OUTPUT + TIM_OC3PE +
				  TIM_OC4M_VAL(TIM_OCxM_PWM_1) + TIM_CC4S_IS_OUTPUT + TIM_OC4PE ;

	TIM2->CCR1 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;	
	
	/* Reglage du timer 3 -> PWM pour bras bas*/
	RCC->APB1ENR |= RCC_TIM3EN; /* Mise en route de l'horloge du timer 3 */

	TIM3->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM3->PSC = 0;
	TIM3->ARR = _PERIODE_PWM_TIM2_; /* Periode de PWM -> 20Khz */

	TIM3->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM3->CCER = 0x3330;
	TIM3->CCMR1 = TIM_OC2M_VAL(TIM_OCxM_PWM_1) + TIM_CC2S_IS_OUTPUT + TIM_OC2PE ;
	TIM3->CCMR2 = TIM_OC3M_VAL(TIM_OCxM_PWM_1) + TIM_CC3S_IS_OUTPUT + TIM_OC3PE +
				  TIM_OC4M_VAL(TIM_OCxM_PWM_1) + TIM_CC4S_IS_OUTPUT + TIM_OC4PE ;

	TIM3->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;

	/* Regle les bras du hacheur en sortie */
	/* Reglage du port A */
	RCC->APB2ENR |= RCC_IOPAEN; /* Mise en route de l'horloge du port A */
	
	GPIOA->ODR |= GPIO_PIN_0 + GPIO_PIN_2 + GPIO_PIN_3;	
	
	GPIOA->CRL &= ~((3<<GPIO_MODE_0_SHIFT) + (3<<GPIO_CNF_0_SHIFT) +
	               (3<<GPIO_MODE_2_SHIFT) + (3<<GPIO_CNF_2_SHIFT) +
	               (3<<GPIO_MODE_3_SHIFT) + (3<<GPIO_CNF_3_SHIFT));
	
	GPIOA->CRL |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_0_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_0_SHIFT) +
	              (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_2_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_2_SHIFT) +
	              (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_3_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_3_SHIFT);
			 
	/* Reglage du port C */
	RCC->APB2ENR |= RCC_IOPCEN; /* Mise en route de l'horloge du port C */

	GPIOC->ODR &= ~(GPIO_PIN_7 + GPIO_PIN_8 + GPIO_PIN_9); 
		
	GPIOC->CRL &= ~((3<<GPIO_MODE_7_SHIFT) + (3<<GPIO_CNF_7_SHIFT));
	GPIOC->CRH &= ~((3<<GPIO_MODE_8_SHIFT) + (3<<GPIO_CNF_8_SHIFT) +
	               (3<<GPIO_MODE_9_SHIFT) + (3<<GPIO_CNF_9_SHIFT));

	GPIOC->CRL |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_7_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_7_SHIFT);
	GPIOC->CRH |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_8_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_8_SHIFT) +
	              (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_9_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_9_SHIFT); 

	/* Remap du timer 3 sur PC7, 8 et 9 */
	RCC->APB2ENR |= RCC_AFIOEN; /* Mise en route de l'horloge de l'AFIO */

	/* Remap entierement le timer 3 */
	AFIO->MAPR = AFIO_TIM3_FULL_REMAP<<AFIO_TIM3_REMAP_SHIFT;

	TIM2->CR1 |= TIM_CEN; 
	TIM3->CR1 |= TIM_CEN; 

	/* Synchronisation des deux timers */
	TIM2->CNT=0;
	TIM3->CNT=0xD4-0xC9;
}

/* 
 * Commande_Hacheur
 *
 * Programme le hacheur en mode push pull
 * Phase_A, Phase_B, Phase_C: valeur entre 0.0 et 65535.0
 *      0.0: le bras est tout le temps collé à la masse
 *      65535.0: le bras est tout le temps collé à VCC
 *      Entre ces deux valeurs, c'est proportionnel
 */
void Commande_Hacheur(int Phase_A, int Phase_B, int Phase_C)
{
int Temp_Phase_A_Bras_Haut;
int Temp_Phase_B_Bras_Haut;
int Temp_Phase_C_Bras_Haut;

int Temp_Phase_A_Bras_Bas;
int Temp_Phase_B_Bras_Bas;
int Temp_Phase_C_Bras_Bas;

	if ((Phase_A==0) && (Phase_B== 0) &&(Phase_C==0))
	{
		Temp_Phase_A_Bras_Haut = 0;
		Temp_Phase_B_Bras_Haut = 0;
		Temp_Phase_C_Bras_Haut = 0;
	
		Temp_Phase_A_Bras_Bas = 0;
		Temp_Phase_B_Bras_Bas = 0;
		Temp_Phase_C_Bras_Bas = 0;
	}
	else
	{	
		/* Rajout de l'offset */
		Temp_Phase_A_Bras_Haut = Phase_A+32768;
		Temp_Phase_B_Bras_Haut = Phase_B+32768;
		Temp_Phase_C_Bras_Haut = Phase_C+32768;

		/* Remise a l'echelle */
		Temp_Phase_A_Bras_Haut = (Temp_Phase_A_Bras_Haut*PWM_MAX)>>16;
		Temp_Phase_B_Bras_Haut = (Temp_Phase_B_Bras_Haut*PWM_MAX)>>16;
		Temp_Phase_C_Bras_Haut = (Temp_Phase_C_Bras_Haut*PWM_MAX)>>16;
	
		Temp_Phase_A_Bras_Bas = Temp_Phase_A_Bras_Haut;
		Temp_Phase_B_Bras_Bas = Temp_Phase_B_Bras_Haut;
		Temp_Phase_C_Bras_Bas = Temp_Phase_C_Bras_Haut;

		/* Suppression du temps mort + saturation */
		Temp_Phase_A_Bras_Haut = Temp_Phase_A_Bras_Haut - _DEAD_TIME_;
		if (Temp_Phase_A_Bras_Haut <0) Temp_Phase_A_Bras_Haut = 0;
		Temp_Phase_B_Bras_Haut = Temp_Phase_B_Bras_Haut - _DEAD_TIME_;
		if (Temp_Phase_B_Bras_Haut <0) Temp_Phase_B_Bras_Haut = 0;
		Temp_Phase_C_Bras_Haut = Temp_Phase_C_Bras_Haut - _DEAD_TIME_;
		if (Temp_Phase_C_Bras_Haut <0) Temp_Phase_C_Bras_Haut = 0;
	
		Temp_Phase_A_Bras_Bas = Temp_Phase_A_Bras_Bas + _DEAD_TIME_;
		if (Temp_Phase_A_Bras_Bas >PWM_MAX) Temp_Phase_A_Bras_Bas = PWM_MAX;
		Temp_Phase_B_Bras_Bas = Temp_Phase_B_Bras_Bas + _DEAD_TIME_;
		if (Temp_Phase_B_Bras_Bas >PWM_MAX) Temp_Phase_B_Bras_Bas = PWM_MAX;
		Temp_Phase_C_Bras_Bas = Temp_Phase_C_Bras_Bas + _DEAD_TIME_;
		if (Temp_Phase_C_Bras_Bas >PWM_MAX) Temp_Phase_C_Bras_Bas = PWM_MAX;
	}

/* Valeur pour branche haute (Tim 2)*/
	TIM2->CCR1 = Temp_Phase_A_Bras_Haut;
	TIM2->CCR3 = Temp_Phase_B_Bras_Haut;
	TIM2->CCR4 = Temp_Phase_C_Bras_Haut;
		
/* Valeur pour branche haute (Tim 3)*/
	TIM3->CCR2 = Temp_Phase_A_Bras_Bas;
	TIM3->CCR3 = Temp_Phase_B_Bras_Bas;
	TIM3->CCR4 = Temp_Phase_C_Bras_Bas;
}

/* 
 * RegleTempsMort
 *
 * Enregistre un callback sur l'IT timer 2
 * Active automatiquement les IT sur overflow
 */
void RegleTempsMort(int deadtime)
{
	DeadTime = deadtime;
}

/* 
 * ActiveITOverflow
 *
 * Enregistre un callback sur l'IT timer 2
 * Active automatiquement les IT sur overflow
 */
void ActiveITOverflow(eventptr cbk)
{
	DEFINE_EVENT(PWM_OVERFLOW, cbk);

	/* Activer l'IT Timer2 overflow */
	TIM2->DIER |= TIM_UIE; 
}

void TIM2_IRQHandler(void)
{
int SR_TMP;

	SR_TMP=TIM2->SR;

		if (SR_TMP&TIM_UIF)
		{
			TIM2->SR = TIM2->SR & ~(TIM_UIF);	
			SEND_EVENT(PWM_OVERFLOW_EVENT);
		}	
}
