#ifndef __STM_CLOCK
#define __STM_CLOCK

#include "stm_clock_config.h"


void Setup_Clock_System(void) ;
extern unsigned int stm32_GetPCLK1 (void);
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



#endif
