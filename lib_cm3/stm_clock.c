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
	RCC->CFGR =__RCC_CFGR_RESET;
	RCC->CR   =__RCC_CR_RESET ;
	RCC->CIR  =__RCC_CIR_RESET;

	  RCC->CFGR =__RCC_CFGR_VALUE;

	  RCC->CR   =__RCC_CR_VALUE ;
	  //RCC->CIR  =__RCC_CIR_VALUE;
	 
	  //AHB sur PLL qui est sur HSE
	  RCC->CR |=  HSEON ; //0x00010000;
	 
	  // wait for HSE OK
	  while((RCC->CR & HSERDY) ==0);
	  
	  //active la PLL
	  RCC->CR |= PLLON;
	  
	  //wait PLL OK
	  while((RCC->CR & PLLRDY) ==0);
	
	 
}

 /*------------------------------------end config clock system 40Mhz ------------------------------------------------*/
