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
#include "adc.h"

#include "config.h"

u16 Buffer_ADC[6];

void Init_ADC (void)
{
  	RCC->APB2ENR |= RCC_ADC1EN;            // enable peripheral clock for ADC1
	// switch on adc module
	ADC1->CR2 |= ADC_ADON ;

 	// continuous scan of channels 1,14,15
  	ADC1->CR1  |= ADC_SCAN;                      
  	ADC1->CR2  |= (ADC_EXTSEL_ON_SWSTART | ADC_CONT | ADC_DMA);  // use data align right,continuous conversion
                                               // EXTSEL = SWSTART 
                                               // enable ADC, no external Trigger
											   // send DMA request
    // A revoir
  	ADC1->SQR1 |= (4-1)<<ADC_L_SHIFT;        // sequence of 3 conversion
  	//ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  	
	// A revoir: channel 1 then 2 then 3 and 4
	ADC1->SQR3  |= (1<<ADC_SQ1_SHIFT) | (2<<ADC_SQ2_SHIFT) | (3<<ADC_SQ3_SHIFT)| (4<<ADC_SQ3_SHIFT); // set channel1 as 1st conversion

	// A revoir: setup ADC guard on channel 1
	ADC1->CR1 |= 1;
		
	// select single regular channel for guard
	ADC1->CR1 |= (ADC_AWDSGL | ADC_AWDEN);
	ADC1->CR1 &= ~(ADC_JAWDEN);
	
	// valid guard interrupt flag
	//ADC1->CR1 |= (AWDIE);

	//DMA configuration
	RCC->AHBENR |= RCC_DMA1EN;// DMA1 CLOCK ENABLE
	DMA1_Channel1->CPAR =(u32)&(ADC1->DR); // @ source 
 	DMA1_Channel1->CCR |= (DMA_PSIZE_IS_16BITS | DMA_MSIZE_IS_16BITS);   //ADC1-CR is 16 bits 
  	DMA1_Channel1->CMAR =(u32) Buffer_ADC;   // @destination
	DMA1_Channel1->CCR &= ~(DMA_DIR); //from periph to memory		
  	DMA1_Channel1->CCR |=DMA_CIRC;   //Circular mode 
 	DMA1_Channel1->CNDTR=4;
  	DMA1_Channel1->CCR |=DMA_MINC;   //Circular mode 
  	DMA1_Channel1->CCR |=DMA_PL_IS_MEDIUM;	// priority level
 	
  	DMA1_Channel1->CCR |= DMA_EN;	// l'DMA bazar est en marche
 
 	ADC1->CR2  |=  (ADC_ADON);				   // start SW conversion	
}

int Lire_ADC(int voie)
{
	if ((voie <=0) || (voie>5)) return 0;
	else return Buffer_ADC[voie];
}

void Regle_Garde_ADC(int voie, int valeur)
{
}

void ADC_IRQHandler(void)
{
}