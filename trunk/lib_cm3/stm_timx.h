//=======================================================================================

// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les PWM possibles sur le uC 
//
//  ________ Rappel sur les ressources __________________________________________________
//
//  3 Timer "general Purpose", TIM2, TIM3 et TIM4
//  Chacun d'entre eux dispose de 4 voies de sorties numérotées de 1 à 4
//  Mapping des IO:
//  	TM2_CH1_ETR - PA0 	TM3_CH1 - PA6		TIM4_CH1 - PB6	
//    	TM2_CH2 - PA1		TM3_CH2 - PA7		TIM4_CH2 - PB7
// 		TM2_CH3 - PA1		TM3_CH3 - PA8		TIM4_CH3 - PB8
// 		TM2_CH4 - PA3		TM3_CH4 - PA9		TIM4_CH4 - PB9
//
//   Chaque sortie physique, par exemple PA1, partage l'IO et l'alternate function (PWM ici)
//=======================================================================================



#ifndef _TIMx_H__
#define _TIMx_H__
#include "stm_regs.h"



void TIMx_Init(void);

// Set the duty cycle to a specific channel
#define SET_DUTY_CYCLE(timer,channel,duty) 	timer->CCR##channel = (u16) (((float)timer->ARR) * (duty/100.0));
// timer can be TIM2, TIM3 or TIM4
// channel the number from 1 to 4
//duty cycle in % : any float from 0.0 to 100.0
// Exemple :
// SET_DUTY_CYCLE(TIM2,1,50.0)   channel 1 of TIM2 is at 50% 



#endif
