/******************************************************************************
* File Name          : ADC.c
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 01/09/2010
* Description        : adc driver
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

#define ADC_L_MASK (0x000F)
#define ADC_SQX_MASK (0x001F)

short int ADC_Array[ADC_ARRAY_SIZE];
int ADC_index;

/* 
 * Fonction: 	Init_ADC
 * Role: 		Initialisation de l'ADC
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_ADC (void) 
{
	ADC_index =0;

  	RCC->APB2ENR |= RCC_ADC1EN;            // enable peripheral clock for ADC1
	// switch on adc module
	ADC1->CR2 |= ADC_ADON ;

 	// continuous scan of channels 1,14,15
  	ADC1->CR1  |= (ADC_SCAN);                      
  	//ADC1->CR1  |= EOCIE;                       // use independant mode, EOCIE mode
  	ADC1->CR2  |= (ADC_EXTSEL_ON_SWSTART+ ADC_EXTTRIG);  // use data align right,continuous conversion
                                               // EXTSEL = SWSTART 
 
  	ADC1->SQR1 |= ((1-1) & ADC_L_MASK)<<ADC_L_SHIFT;        // sequence of 1 conversion
  	//	ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  	
	// channel 14 
	ADC1->SQR3  |= ((14 & ADC_SQX_MASK)<<ADC_SQ1_SHIFT);                       // set channel1 as 1st conversion

	//DMA configuration
	/*RCC->AHBENR |=RCC_DMA1EN;// DMA1 CLOCK ENABLE
	DMA1_Channel1->CPAR =(u32)&(ADC1->DR); // @ source 
 	DMA1_Channel1->CCR |= (DMA_PSIZE_IS_16BITS | DMA_MSIZE_IS_16BITS);   //ADC1-CR is 16 bits 
  	DMA1_Channel1->CMAR =(u32) ADC_Array;   // @destination
	DMA1_Channel1->CCR &= ~(DMA_DIR); //from periph to memory		
  	DMA1_Channel1->CCR |=DMA_CIRC;   //Circular mode 
 	DMA1_Channel1->CNDTR=ADC_ARRAY_SIZE;
  	DMA1_Channel1->CCR |=DMA_MINC;   //Circular mode 
  	DMA1_Channel1->CCR |=DMA_PL_IS_MEDIUM;	// priority level
 	
  	DMA1_Channel1->CCR |= DMA_EN;	// l'DMA bazar est en marche*/
 
 	//ADC1->CR2  |=  (ADC_ADON);				   // start SW conversion
}

/* 
 * Fonction: 	Lire_ADC
 * Role: 		Lecture de la derniere valeur de l'ADC
 *
 * Sortie: 
 *		Dernier valeur de l'ADC ou zero si le buffer est vide
 */
int Lire_ADC(void)
{
int tmp;
    tmp =0;

	ADC1->CR2  |=  (ADC_SWSTART);				   // start SW conversion

	while (!(ADC1->SR & ADC_EOC));
	
	tmp = ADC1->DR;

	return tmp;
}