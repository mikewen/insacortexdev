#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "../../lib_cm3/metro_lib_v2009a.h"  
 
#define __RESET_EOC_AND_FLAG_START_CONVERSION  	 0xFFFFFFED
#define __TEST_END_OF_CONVERSION				 0x00000002


u32 __COURANT; 


 void Avance_train(void)
{ TIM2->CR1 &=0xFFFE;
  TIM2->CCER &=0xFFEF;
  TIM4->CCER |=0x1000;
  TIM4->CR1 |=0x0001;
}



void Recule_train_Arriere(void)
{ TIM4->CR1 &=0xFFFE;
  TIM4->CCER &=0xEFFF;
  TIM2->CCER |=0x0010; 
  TIM2->CR1 |=0x0001;
}


void SysTick_Handler(void)
{


}

void TIM3_IRQHandler(void)
{

}

void TIM1_UP_IRQHandler(void)
{ static int i=0; 
  		
if((TIM1->SR & 0x0001))
      {TIM1->SR &=0xFFFE;		 // reset interrupt flag
	   Start_Conversion();

	  }	
/*if(i==100)
{
 Avance_train();
}*/		 
  i++;
} 




void ADC_IRQHandler(void)
{
   if(ADC1->SR &=__TEST_END_OF_CONVERSION)
        { ADC1->SR &=__RESET_EOC_AND_FLAG_START_CONVERSION;
         __COURANT = Get_Courant_Moteur();
	    }
} 

int main()
{
 stm32_NvicSetup ();
 Setup_Pll_As_Clock_System();
 init_portA();
 init_portB();
 init_portC();
 init_timer2();
 init_timer4();
 init_timer1();
 Setup_Adc();
 Adc_On();
 init_timer3();			
				// STM32 setup
 //GPIOA->ODR |=0x0020; 
 Recule_train_Arriere();
                        
while(1)
   { 
   }
}

