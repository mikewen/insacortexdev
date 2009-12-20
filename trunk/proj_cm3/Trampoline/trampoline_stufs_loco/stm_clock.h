#ifndef __STM_CLOCK
#define __STM_CLOCK

//#include "STM32_Reg.h"


#define __PLLMULL 5
#define __HSI 8000000UL
#define __HSE 8000000UL
#define __SYSCLK 40000000UL
#define __HCLK	(__SYSCLK / 1)
#define __PCLK1 (__HCLK / 2)
#define __PCLK2 (__HCLK / 1)



// RCC_CR configure register
#define __RCC_CR_RESET    0x00000083
#define PLL3RDY (1<<29)
#define PLL3ON  (1<<28)
#define PLL2RDY (1<<27)
#define PLL2ON 	(1<<26)
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

//RCC_CFGR
#define __RCC_CFGR_RESET          0x00000000
#define MCO		(1<<24)
#define USBPRE	(1<<22)
#define PLLMUL	(1<<18)		 // PLL multiplication factor from 2 to 16
							 // caution max frequency output pll at 72MHz
							//TODO warning automaique de > 72MHz

#define PLLMUL_VAL(multiplier) (((__PLLMULL-2) & 0xF )<<18)
#define PLLXTPRE (1<<17)
#define PLLSRC	(1<<16)	     // PLL entry clock source
#define PLLSRC_HSI (0<<16)
#define PLLSRC_HSE (1<<16)
#define ADCPRE	(1<<14)
#define PPRE2	(1<<11)		 // APB2 high speed prescaler=> PCLK2
#define PPRE2_DIV_2 (4<<11)
#define PPRE2_DIV_4 (5<<11)
#define PPRE2_DIV_8 (6<<11)
#define PPRE2_DIV_16 (7<<11)
#define PPRE1	(1<<8)		 // APB1 low speed prescaler=> PCLK1
#define PPRE1_NOT_DIVIDED (0<<8)
#define PPRE1_DIV_2 (4<<8)
#define PPRE1_DIV_4 (5<<8)
#define PPRE1_DIV_8 (6<<8)
#define PPRE1_DIV_16 (7<<8)

#define HPRE	(1<<4)
#define SWS		(1<<2)
#define SW		(1<<0)	//System clock switch
#define SW_HSI  (0)	    // 8MHz  internal RC 
#define SW_HSE  (1)		// External crystal
#define SW_PLL  (2)		// PLL (on HSI or HSE)


//RCC_CIR
#define __RCC_CIR_RESET           0x00000000
#define CSSC	(1<<23) // clock security system interrupt clear
#define PLLRDYC	(1<<20)	//						 interrupt clear
#define HSERDYC	(1<<19)
#define HSIRDYC	(1<<18)
#define LSERDYC	(1<<17)		
#define LSIRDYC	(1<<16)
#define PLLRDYIE (1<<12)	//interrupt enable
#define HSERDYIE (1<<11)
#define HSIRDYIE (1<<10)
#define LSERDYIE (1<<9)		
#define LSIRDYIE (1<<8)
#define CSSF	(1<<7)  //clock security system interrupt flag
#define PLLRDYF	(1<<4)  //								  flag
#define HSERDYF	(1<<3)
#define HSIRDYF	(1<<2)
#define LSERDYF	(1<<1)		
#define LSIRDYF	(1<<0)



#define __RCC_CIR_VALUE       (HSERDYIE | PLLRDYIE)  //TODO DEBUG car polling ! 0x00001800
#define __RCC_CFGR_VALUE 	(SW_PLL | PPRE1_DIV_2 | PLLSRC_HSE | PLLMUL_VAL(__PLLMULL) )
#define __RCC_CR_VALUE	  	(HSEON|PLLON)



void Setup_Pll_As_Clock_System() ;
#endif

/*----------------------------------------------------------------------------
 Define SYSCLK
 *----------------------------------------------------------------------------*/
			