#include "stm_clock.h"
#include <stm32f10x_lib.h>
//#include "STM32_Reg.h"
/*--------------------------------------config clock system 40Mhz -------------------------------------------------*/
//
// HSE clock en entrée (quartz 8M)
// PLL x5 => 40M
// AHB 40M
// APB1 APB2 input à 40M 
// APB1 output à 20M (PORTA et TIM2-6,...) 
// APB2 output à 40M (PORTA et TIM2-6,...)
//   tout périph APB2 à 40 sauf l'ADC à 20M
// Pas d'interruption de clock

void Setup_Pll_As_Clock_System()
{ 
	  RCC->CR   =__RCC_CR_RESET ;
	  // AHB sur la clock interne HSI
	  RCC->CFGR =__RCC_CFGR_RESET;
	  RCC->CIR  =__RCC_CIR_RESET;
	  
	  RCC->CFGR |=__RCC_CFGR_SET1;
	  RCC->CFGR &=__RCC_CFGR_SET0;
	  
	  RCC->CIR  |=__RCC_CIR;
	 
	  //AHB sur PLL qui est sur HSE
	  RCC->CR |=0x00010000;
	 
	  // wait for HSE OK
	  while((RCC->CR & 0x00020000) ==0);
	  
	  //active la PLL
	  RCC->CR |=0x01000000;
	  
	  //wait PLL OK
	  while((RCC->CR & 0x02000000) ==0);
	
	  //RCC->CR   |=__RCC_CR_HSE_AND_PLL_ON;
}

 /*------------------------------------end config clock system 40Mhz ------------------------------------------------*/
