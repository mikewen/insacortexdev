/******************************************************************************
* File Name          : pwm.c
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 01/09/2010
* Description        : pwm driver
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "stm_regs.h"
#include "pwm.h"
#include "voice.h"

#define DEFINE_EVENT(NOM, event) NOM##_EVENT=event;
#define SEND_EVENT(event) event()

#define _FREQ_HACHAGE_		(40000U)
#define _PERIODE_PWM_TIM4_ 	(72000000U/_FREQ_HACHAGE_)
#define PWM_MAX 			_PERIODE_PWM_TIM4_

eventptr PWM_OVERFLOW_EVENT;

void Init_PWM (void)
{
	/* Reglage du timer 4 -> PWM pour bras haut*/
	RCC->APB1ENR |= RCC_TIM4EN; /* Mise en route de l'horloge du timer 4 */

	TIM4->CNT = 0; /* On cale le timer juste apres (pas de risque de se prendre une IT avant la fin de l'init) */
	TIM4->PSC = 0;
	TIM4->ARR = 0x3FF; /*_PERIODE_PWM_TIM4_;  Periode de PWM -> 40Khz */

	TIM4->SMCR |= TIM_SMS_IS_DISABLED;	/* Desactivation du SMS */	
 
	TIM4->CCER = 0x0100;
	TIM4->CCMR2 = TIM_OC3M_VAL(TIM_OCxM_PWM_1) + TIM_CC3S_IS_OUTPUT + TIM_OC3PE ;

	TIM4->CCR3 = 0;	

	TIM4->DIER |= TIM_UIE; /* Active les IT overflow */
	 
	/* Regle les bras du hacheur en sortie */
	/* Reglage du port B */
	RCC->APB2ENR |= RCC_IOPBEN; /* Mise en route de l'horloge du port A */
	
	GPIOB->ODR |= GPIO_PIN_8; 	
	
	GPIOB->CRH &= ~((3<<GPIO_MODE_8_SHIFT) + (3<<GPIO_CNF_8_SHIFT));
	
	GPIOB->CRH |= (GPIO_MODE_OUTPUT_50_MHZ<<GPIO_MODE_8_SHIFT) + (GPIO_CNF_ALTERNATE_PUSH_PULL<<GPIO_CNF_8_SHIFT);		 

	TIM4->CR1 |= TIM_CEN; 
}

void Regle_PWM(int val)
{
int tmp;

	tmp = val;

	if (tmp > 0x3FF) tmp = 0x3FF;
	if (tmp < 0) tmp =0;
	 
	TIM4->CCR3 = tmp;	
}

void TIM4_IRQHandler (void)
{
u32 tmp;

	TIM4->SR = 0;

	tmp = (voice_buffer[0]+voice_buffer[1]+voice_buffer[2]+voice_buffer[3])>>2;

	Regle_PWM(tmp);
}