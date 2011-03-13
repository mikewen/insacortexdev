/******************************************************************************
* File Name          : filtre.c
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 01/09/2010
* Description        : Mainfile for filter
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "stm_regs.h"
#include "adc.h"
#include "pwm.h"
#include "stm_clock.h"

#define INPUT_BUFFER_SIZE 10
#define OUTPUT_BUFFER_SIZE 10

short int Input_Buffer[INPUT_BUFFER_SIZE];
short int Output_Buffer[OUTPUT_BUFFER_SIZE];

void PWM_Callback(void);

volatile int tmp;

int main (void)
{
volatile int i;

    /* Init des periphs */
	Init_Clock_System();
	Init_ADC();
	Init_PWM(PWM_Callback);

	/* Activation des IT TIM4 */
	NVIC_SET_PRIO_PERIPH(TIM4, 10);
	NVIC_CLEAR_PENDING_PERIPH_IT(TIM4);
	NVIC_ENABLE_PERIPH_IT(TIM4);

	/* Et pour le debug, on active aussi la prise en compte des vecteurs de fautes */
	NVIC_ENABLE_SYSTEM_IT(MEM_FAULT);
	NVIC_ENABLE_SYSTEM_IT(BUS_FAULT);
	NVIC_ENABLE_SYSTEM_IT(USAGE_FAULT);

	while (1)
	{
		i =i+1;
	}

	return 0;
}

void PWM_Callback(void)
{
	tmp =Lire_ADC();
	tmp = tmp >>2;

	Regle_PWM(tmp);
}
