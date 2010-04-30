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

/* 
 * Init_Hacheur
 *
 * Initialisation des bras haut et bas du hacheur
 * Bras haut (PMOS) connecté au timer 2 (canaux 1,3 et 4)
 * Bras bas (NMOS) connecté aux GPIO (pas de timer)
 */
void Init_Hacheur (void)
{
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

	TIM2->CR1 |= TIM_CEN; 	
	
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

	GPIOC->CRL |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_7_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_7_SHIFT);
	GPIOC->CRH |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_8_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_8_SHIFT) +
	              (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_9_SHIFT) + (GPIO_CNF_OUTPUT_PUSH_PULL<<GPIO_CNF_9_SHIFT); 
}

/* 
 * Regle_Bras_Haut
 *
 * Programme le bras haut
 * MOS_A, MOS_B, MOS_C: valeur 0 ou 1
 *      0: le PMOS est ouvert
 *      1: Le PMOS est "haché" au rapport indiqué dans le parametre PWM
 */
void Regle_Bras_Haut (int MOS_A, int MOS_B, int MOS_C, int pwm)
{
	TIM2->CCR1 = MOS_A*pwm;
	TIM2->CCR3 = MOS_B*pwm;
	TIM2->CCR4 = MOS_C*pwm;
}

/* 
 * Regle_Bras_Bas
 *
 * Programme le bras bas
 * MOS_A, MOS_B, MOS_C: valeur 0 ou 1
 *      0: le NMOS est ouvert
 *      1: Le NMOS est fermé
 *
 * Le parametre pwm ne sert pas.
 */
void Regle_Bras_Bas (int MOS_A, int MOS_B, int MOS_C, int pwm)
{
	GPIOC->ODR &= (MOS_A<<7) + (MOS_B<<8) + (MOS_C<<9);
	GPIOC->ODR |= (MOS_A<<7) + (MOS_B<<8) + (MOS_C<<9);
}
