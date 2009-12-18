#ifndef __STM_CLOCK
#define __STM_CLOCK

#include "STM32_Reg.h"
	
#define __RCC_CR_HSE_AND_PLL_ON	  0x01010000
#define __RCC_CFGR_SET1	          0x000D0402	  
#define __RCC_CFGR_SET0	          0xFFDDC40E
#define __RCC_CIR            	  0x00001800
#define __RCC_CR_RESET            0x00000083
#define __RCC_CFGR_RESET          0x00000000
#define __RCC_CIR_RESET           0x00000000


void Setup_Pll_As_Clock_System() ;

/*----------------------------------------------------------------------------
 Define SYSCLK
 *----------------------------------------------------------------------------*/
#define __HSI 8000000UL
//#define __HSE 8000000UL
#define __PLLMULL (((__RCC_CFGR_VAL & CFGR_PLLMULL_MASK) >> 18) + 2)

#if   ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x00) 
  #define __SYSCLK   __HSI                        // HSI is used as system clock
#elif ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x01)
  #define __SYSCLK   __HSE                        // HSE is used as system clock
#elif ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x02)
  #if (__RCC_CFGR_VAL & CFGR_PLLSRC_MASK)         // HSE is PLL clock source
    #if (__RCC_CFGR_VAL & CFGR_PLLXTPRE_MASK)     // HSE/2 is used
      #define __SYSCLK  ((__HSE >> 1) * __PLLMULL)// SYSCLK = HSE/2 * pllmull
    #else                                         // HSE is used
      #define __SYSCLK  ((__HSE >> 0) * __PLLMULL)// SYSCLK = HSE   * pllmul
    #endif  
  #else                                           // HSI/2 is PLL clock source
    #define __SYSCLK  ((__HSI >> 1) * __PLLMULL)  // SYSCLK = HSI/2 * pllmul
  #endif
#else
   #error "ask for help"
#endif

/*----------------------------------------------------------------------------
 Define  HCLK
 *----------------------------------------------------------------------------*/
#define __HCLKPRESC  ((__RCC_CFGR_VAL & CFGR_HPRE_MASK) >> 4)
#if (__HCLKPRESC & 0x08)
  #define __HCLK        (__SYSCLK >> ((__HCLKPRESC & 0x07)+1))
#else
  #define __HCLK        (__SYSCLK)
#endif

/*----------------------------------------------------------------------------
 Define  PCLK1
 *----------------------------------------------------------------------------*/
#define __PCLK1PRESC  ((__RCC_CFGR_VAL & CFGR_PRE1_MASK) >> 8)
#if (__PCLK1PRESC & 0x04)
  #define __PCLK1       (__HCLK >> ((__PCLK1PRESC & 0x03)+1))
#else
  #define __PCLK1       (__HCLK)
#endif

/*----------------------------------------------------------------------------
 Define  PCLK2
 *----------------------------------------------------------------------------*/
#define __PCLK2PRESC  ((__RCC_CFGR_VAL & CFGR_PRE2_MASK) >> 11)
#if (__PCLK2PRESC & 0x04)
  #define __PCLK2       (__HCLK >> ((__PCLK2PRESC & 0x03)+1))
#else
  #define __PCLK2       (__HCLK)
#endif

#endif				