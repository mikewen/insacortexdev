#include <stm32f10x_lib.h>
#include "adc.h"

void ADCInit (void) {

//  GPIOA->CRL &= ~0x0000000F;                    // set PIN1 as analog input (see stm32_Init.c)
//  PA0,PA1 et PA4 sont initialisées en entrées analogiques au niveau de STM32_Init.c

  	RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1

  	ADC1->SQR3  = ADC_Channel_0;					  // set channel0 as 1st conversion                       
  	ADC1->SQR1  = 0x00000000;                       // only one conversion
  	ADC1->SMPR2 = ADC_SampleTime_55Cycles5 + 
                  (ADC_SampleTime_55Cycles5<<3) +
				  (ADC_SampleTime_55Cycles5<<12);    // set sample time channel0, 1 and 4 (55,5 cycles)
  

  	ADC1->CR1   = 0x00000100;                       // use independant mode, SCAN mode
  	ADC1->CR2 = ADC_DataAlign_Right+
                ADC_ExternalTrigConv_None+ 1;		  // use data align right,
                                                  // EXTSEL = SWSTART, enable ADC

  	//ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

u16 ADCConvert (u8 channel)
{
	if ((channel == ADC_Channel_1) ||
	    (channel == ADC_Channel_5) ||
		(channel == ADC_Channel_7))
	{
		ADC1->SQR3 = channel;

		ADC1->CR2  |=  0x00500000; 	// start SW conversion

		while (!(ADC1->SR & ADC_FLAG_EOC));
	}

	return (u16)ADC1->DR;
}
