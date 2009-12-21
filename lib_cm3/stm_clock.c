#include "stm_clock.h"
#include <stm32f10x_lib.h>



/* Peripheral memory map 
#define PERIPH_BASE           ((u32)0x40000000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        	 (PERIPH_BASE + 0x20000)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
*/

// RCC_CR  Control Register definitions
#define RCC_CR  *((unsigned int *)0x40021000)
#define PLLRDY 	(1<<25)
#define PLLON 	(1<<24)
#define CSSON 	(1<<19)
#define HSEBYP 	(1<<18)
#define HSERDY 	(1<<17)
#define HSEON 	(1<<16)
#define HSICAL 	(1<<8)
#define HSITRIM (1<<3)
#define HSIRDY 	(1<<1)
#define HSION 	(1<<0)
#define __RCC_CR_RESET    0x00000083


//RCC_CFGR Clock ConFiG Register  definition
#define RCC_CFGR  *((unsigned int *)0x40021004)
#define MCO		(1<<24)
#define USBPRE	(1<<22)
#define PLLMUL	(1<<18)		 // PLL multiplication factor from 2 to 16
#define PLLMUL_VAL(mult) (((mult-2) & 0xF )<<18)
#define PLLXTPRE (1<<17)
#define PLLSRC	(1<<16)	     // PLL entry clock source
#define PLLSRC_HSI (0<<16)
#define PLLSRC_HSE (1<<16)
#define ADCPRE	(1<<14)
#define ADCPRE_VAL(pres) (((pres/2) & 0x3 )<<14)
#define PPRE2	(1<<11)		 // APB2 high speed prescaler=> PCLK2
#define PPRE2_DIV_2 (4<<11)
#define PPRE2_DIV_4 (5<<11)
#define PPRE2_DIV_8 (6<<11)
#define PPRE2_DIV_16 (7<<11)
#define PPRE2_VAL(pre) ((pre==1) ? 0 : ((pre/2)-(pre/8)-2*(pre/16)+3)<<11)
#define PPRE1	(1<<8)		 // APB1 low speed prescaler=> PCLK1
#define PPRE1_NOT_DIVIDED (0<<8)
#define PPRE1_DIV_2 (4<<8)
#define PPRE1_DIV_4 (5<<8)
#define PPRE1_DIV_8 (6<<8)
#define PPRE1_DIV_16 (7<<8)
#define PPRE1_VAL(pre) ((pre==1) ? 0 : ((pre/2)-(pre/8)-2*(pre/16)+3)<<8)
#define HPRE	(1<<4)		 //AHB prescaler just before APB1 and APB2 prescaler
#define HPRE_NOT_DIVIDED (0<<4)
#define HPRE_DIV_2 	(8<<4)
#define HPRE_DIV_4 	(9<<4)
#define HPRE_DIV_8 	(10<<4)
#define HPRE_DIV_16 (11<<4)
#define HPRE_DIV_64 (12<<4)
#define HPRE_DIV_128 (13<<4)
#define HPRE_DIV_256 (14<<4)
#define HPRE_DIV_512 (15<<4)
#define HPRE_VAL(pre) ( (7+(pre>256)+(pre>128)+(pre>64)+(pre>16)+(pre>8)+(pre>4)+(pre>2)+(pre>1))<<4 )
#define SWS		(1<<2)
#define SW		(1<<0)	//System clock switch
#define SW_HSI  (0)	    // 8MHz  internal RC 
#define SW_HSE  (1)		// External crystal
#define SW_PLL  (2)		// PLL (on HSI or HSE)

#define __RCC_CFGR_RESET          0x00000000



#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSE) )
//	#define __RCC_CR_VALUE	  	(HSEON|PLLON)
	#define __RCC_CFGR_VALUE 	(PLLMUL_VAL(__PLLMULL)|(_PLLXTPRE*PLLXTPRE)|PLLSRC_HSE|ADCPRE_VAL(_ADCPRE)|SW_PLL|PPRE1_VAL(_PPRE1)|PPRE2_VAL(_PPRE2)|HPRE_VAL(_HPRE) )
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
	RCC_CR =__RCC_CR_RESET ;
	RCC_CFGR =__RCC_CFGR_RESET;
	RCC->CIR  =__RCC_CIR_RESET;

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
	  
	  //wait PLL OK
	  while((RCC_CR & PLLRDY) ==0);
	
	 
}

 /*------------------------------------end config clock system 40Mhz ------------------------------------------------*/
