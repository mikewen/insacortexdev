#ifndef __STM_CLOCK
#define __STM_CLOCK
	
#define __RCC_CR_HSE_AND_PLL_ON	  0x01010000
#define __RCC_CFGR_SET1	          0x000D0402	  
#define __RCC_CFGR_SET0	          0xFFDDC40E
#define __RCC_CIR            	  0x00001800
#define __RCC_CR_RESET            0x00000083
#define __RCC_CFGR_RESET          0x00000000
#define __RCC_CIR_RESET           0x00000000


void Setup_Pll_As_Clock_System() ;

#endif				