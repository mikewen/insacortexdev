#include "stm_clock.h"
#include <stm32f10x_lib.h>

#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSE) )
//	#define __RCC_CR_VALUE	  	(HSEON|PLLON)
	#define __RCC_CFGR_VALUE 	(PLLMUL_VAL(__PLLMULL)|(_PLLXTPRE&PLLXTPRE)|PLLSRC_HSE|ADCPRE_VAL(_ADCPRE)|SW_PLL|PPRE1_VAL(_PPRE1)|PPRE2_VAL(_PPRE2)|HPRE_VAL(_HPRE) )
	#define HSE_IS_USED
	#define PLL_IS_USED
#elif ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSI) )
//	#define __RCC_CR_VALUE	  	(HSION|PLLON)
	#define HSI_IS_USED
	#define PLL_IS_USED

#elif (_SYSCLK_SOURCE  == IS_HSI)
//	#define __RCC_CR_VALUE	  	(HSION)
	#define HSI_IS_USED

#elif (_SYSCLK_SOURCE  == IS_HSE)
//	#define __RCC_CR_VALUE	  	(HSEON)
	#define HSE_IS_USED

#endif


void Setup_Clock_System()
{ 
	RCC_CR =__RCC_CR_RESET ;
	RCC_CFGR =__RCC_CFGR_RESET;
	RCC_CIR  =__RCC_CIR_RESET;

	RCC_CFGR =__RCC_CFGR_VALUE;
	 	 
	#ifdef HSE_IS_USED
		//activate HSE
		RCC_CR |= HSEON;
		//wait for HSE to be ready
	  	while((RCC_CR & HSERDY) == 0);
	#endif  

	#ifdef HSI_IS_USED
		//activate HSI
		RCC_CR |= HSION;
		//wait for HSI to be ready
	  	while((RCC_CR & HSIRDY) == 0);
	#endif  

	#ifdef PLL_IS_USED
		//activate PLL
		RCC_CR |= PLLON;
		//wait for PLL to be ready
		while((RCC_CR & PLLRDY) == 0);
	#endif  
	 	 
}

unsigned int stm32_GetPCLK1 (void)
{
	return (unsigned int) (__PCLK1) ;
}