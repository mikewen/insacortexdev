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
#include "stm_clock.h"
#include "stm_usartx.h"
#include "lcd.h"
#include "adc.h"

#include "interface.h"
#include "capteur.h"
#include "hacheur.h"
#include "controle.h"

#include "config.h"

volatile int Drapeau_Init_Moteur;

int main (void)
{
	/* Demarrage de l'horloge */
	Init_Clock_System();

	/* Demarrage des peripheriques */
	setup_usart();
	Init_ADC();

	/* Interface */
	Init_Interface();
	Init_Capteur();
	Init_Hacheur();
	Init_Controle();

 	/* Reglage des IT */
	NVIC_SET_PRIO_PERIPH(USART1, 15); 	/* La liaison serie est la moins prio */
	NVIC_SET_PRIO_PERIPH(DMA1_CHANNEL4, 15); 	/* La liaison serie est la moins prio */
	NVIC_SET_PRIO_SYSTEM(SYSTICK, 14);	/* Le timer systeme n'est pas tr�s prioritaire */
	NVIC_SET_PRIO_PERIPH(TIM1_UP, 13);
	NVIC_SET_PRIO_PERIPH(TIM2, 12);
	NVIC_SET_PRIO_PERIPH(TIM3, 11);
	NVIC_SET_PRIO_PERIPH(TIM4, 10);
	NVIC_SET_PRIO_PERIPH(ADC1_2, 9);

	/* Efface les etats "PENDING" des vecteurs */
	NVIC_CLEAR_PENDING_PERIPH_IT(USART1);
	NVIC_CLEAR_PENDING_PERIPH_IT(DMA1_CHANNEL4);
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM1_UP);
	NVIC_CLEAR_PENDING_PERIPH_IT(ADC1_2);
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM2);
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM3);
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM4);
	/* RQ: il n'existe pas d'etat "PENDING" pour les IT SYSTICK */

	/* Active la prise en compte des IT */
	NVIC_ENABLE_PERIPH_IT(TIM4);
//	NVIC_ENABLE_PERIPH_IT(ADC1_2);
//	NVIC_ENABLE_PERIPH_IT(DMA1_CHANNEL4);
//	NVIC_ENABLE_PERIPH_IT(ADC1_2);
//	NVIC_ENABLE_PERIPH_IT(TIM1_UP);
//	NVIC_ENABLE_PERIPH_IT(ADC1_2);
//	NVIC_ENABLE_PERIPH_IT(TIM1_UP);
//	NVIC_ENABLE_PERIPH_IT(ADC1_2);
	/* RQ: La prise en compte des IT SYSTICK est actif par defaut */

	SysTick->LOAD = 900000*10;
//    SysTick->LOAD = 9;
	SYSTICK_CLOCK_AHB_8();

	/* Et pour le debug, on active aussi la prise en compte des vecteurs de fautes */
	NVIC_ENABLE_SYSTEM_IT(MEM_FAULT);
	NVIC_ENABLE_SYSTEM_IT(BUS_FAULT);
	NVIC_ENABLE_SYSTEM_IT(USAGE_FAULT);

	/* Calage du capteur */
	Cale_Moteur();
	Drapeau_Init_Moteur=0;

	SYSTICK_ENABLE_IT();
	SYSTICK_ENABLE_COUNTER();

	while (!Drapeau_Init_Moteur);

	SYSTICK_DISABLE_COUNTER();
	SYSTICK_DISABLE_IT();
	
	Demarre_Capteur();
	Init_Moteur();

	for (;;)
	{
		Interface();
	}
}

void SysTick_Handler(void)
{
	Drapeau_Init_Moteur=1;	
}