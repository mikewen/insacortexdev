/*
________________________________________________________________________________________
	Clock configuration files for stm32f103
	stm_clock_config.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. stm_clock
	RELPATH is the relative path from your projetc directory to lib_cm3 directory	
	
   	include RELPATH/lib_cm3/???.c file in your makefile
	copy    RELPATH/lib_cm3/???_config_TEMPLATE.h in your project directory	(noted ./)
	rename  ./???_config_TEMPLATE.h as ./???_config.h in your project directory
	edit    ./???_config.h file for your project (lines with //CONF tags)  
	ensure that ./ path is visible in CC compiler options 
	add "#include "RELPATH/lib_cm3/???.h" in app. code 
	add a call to "Init_???();" at initialisation step of your APP
________________________________________________________________________________________
REVS
	[Acco 06/01/2010] finalisation et commentaires de la première version
		Testée en réel et simulé (utilisation de PWM et USART et CAN)		
________________________________________________________________________________________
TODO
	test _SYSCLK_SOURCE IS_HSI and IS_HSE
	+ Peripheral clock enable  functions
	+ USBCLK, MCO, CSS fonctionalities
	+ RTCCLK IWDGCLK functionalities
	+ TIM1 and TIM2  
 	+ stm32_GetPCLK2() et autres fonctions pour assurer 
	  la compatibilité avec stm32f10x lib
________________________________________________________________________________________
*/

#include "stm_clock.h"

//________________________________
// Clock registers descriptions

// RCC_CR  Control Register definitions
#define RCC_CR  *((volatile unsigned int *)0x40021000)
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

//__________________________________________
//RCC_CFGR Clock ConFiG Register  definition
#define RCC_CFGR  *((volatile unsigned int *)0x40021004)
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

#define __RCC_CIR_VALUE       (HSERDYIE | PLLRDYIE)  //TODO DEBUG car polling ! 0x00001800


//_______________________________________
//RCC_CIR Clock Interrupt Register
#define RCC_CIR  *((volatile unsigned int *)0x40021008)
#define CSSC    (1<<23) // clock security system interrupt clear
#define PLLRDYC (1<<20) //                                               interrupt clear
#define HSERDYC (1<<19)
#define HSIRDYC (1<<18)
#define LSERDYC (1<<17)         
#define LSIRDYC (1<<16)
#define PLLRDYIE (1<<12)        //interrupt enable
#define HSERDYIE (1<<11)
#define HSIRDYIE (1<<10)
#define LSERDYIE (1<<9)         
#define LSIRDYIE (1<<8)
#define CSSF    (1<<7)  //clock security system interrupt flag
#define PLLRDYF (1<<4)  //                                                                flag
#define HSERDYF (1<<3)
#define HSIRDYF (1<<2)
#define LSERDYF (1<<1)          
#define LSIRDYF (1<<0)
#define __RCC_CIR_RESET           0x00000000

//_______________________________
//determine what should be used
#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSE) )
	#define __RCC_CFGR_VALUE 	(PLLMUL_VAL(__PLLMULL)|(_PLLXTPRE&PLLXTPRE)|PLLSRC_HSE|ADCPRE_VAL(_ADCPRE)|SW_PLL|PPRE1_VAL(_PPRE1)|PPRE2_VAL(_PPRE2)|HPRE_VAL(_HPRE) )
	#define HSE_IS_USED
	#define PLL_IS_USED
#elif ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSI) )
	#define HSI_IS_USED
	#define PLL_IS_USED
#elif (_SYSCLK_SOURCE  == IS_HSI)
	#define HSI_IS_USED
#elif (_SYSCLK_SOURCE  == IS_HSE)
	#define HSE_IS_USED
#endif

//____________________________
void Init_Clock_System()
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

//__________________________________________
//ensure compatibility with STM32f10x lib
unsigned int stm32_GetPCLK1 (void) __attribute__ ((weak));
unsigned int stm32_GetPCLK1 (void) 
{
	return (unsigned int) (__PCLK1) ;
}

