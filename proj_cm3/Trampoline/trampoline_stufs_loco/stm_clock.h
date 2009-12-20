#ifndef __STM_CLOCK
#define __STM_CLOCK

#define IS_HSI 0
#define IS_HSE 1
#define IS_PLL 2
#define IS_DIRECT 0
#define IS_DIVIDED_BY_2 1

#define __HSI 8000000UL
#define __HSE 8000000UL

//_________________________________________________________________________________________
// SYSTEM CLOCK CONFIGURATION

// CONFIGURE SYSCLK source : it can be
//  IS_HSI : for High Speed Internal clock  (the 8MHz local RC clock)
//  IS_HSE : for High Speed External Clock  (the external crystal) 8MHz on MCBSTM32 boards
//  IS_PLL : when using the Phase Locked Loop circuit to multiply frequency 
#define _SYSCLK_SOURCE IS_PLL


#if (_SYSCLK_SOURCE  == IS_HSI)
	#error "line 14 : IS_HSI conf not yet implemented"

#elif (_SYSCLK_SOURCE  == IS_HSE)
	#error "line 14 : IS_HSE conf not yet implemented"


#elif (_SYSCLK_SOURCE  == IS_PLL)


	// CONFIGURE PLL source ONLY if you set _SYSCLK_SOURCE IS_PLL
	//  IS_HSI : for High Speed Internal clock  (the 8MHz local RC clock)
	//  IS_HSE : for High Speed External Clock  (the external crystal) 8MHz on MCBSTM32 boards
	//  IS_PLL : when using the Phase Locked Loop circuit to multiply frequency 
	#define _PLL_SOURCE IS_HSE
   	
	#if  (_PLL_SOURCE  == IS_HSI)
		
	#elif  (_PLL_SOURCE  == IS_HSE)
		//CONFIGURE pll extern prescaler ONLY IF you set _SYSCLK_SOURCE IS_PLL and _PLL_SOURCE IS_HSE
		// 								   OR IF you will set _USBCLK IS_USED
		// IS_DIRECT : when PLL input is directly drive by HSE external crystal
		// IS_DIVIDED_BY_2 : when PLL input is drived by /2 prescaler of HSE external crystal 
		#define _PLLXTPRE IS_DIVIDED_BY_2
	#else
		#error "Line 28 : invalid PLL_SOURCE should be IS_HSI, IS_HSE or IS_PLL"
	#endif
	
	// CONFIGURE pll multiplication factor  ONLY IF you set _SYSCLK_SOURCE IS_PLL
	// 								  		 OR IF you will set _USBCLK IS_USED
	// [2 to 16] choose any multiplication factor in this fork 
	#define __PLLMULL 5

	#if (__PLLMULL >= 16)
	#error "Line 52 : __PLLMULL value too high! Should be from 2 to 16"
	#elif (__PLLMULL <= 2)
	#error "Line 52 : __PLLMULL value too low! Should be from 2 to 16"
	#endif
#else
#error "Line 14 : invalid HCLK_SOURCE should be IS_HSI, IS_HSE or IS_PLL"
#endif

// end of system clock cnfiguration
//_________________________________________________________________________________________

//_________________________________________________________________________________________
// AUXILIARY PERIPHERALS CLOCK configuration


// CONFIGURE AHB prescaler (presacler that gives clock to APB1,APB2,
//                          SDIO,FSMC,AHB,Cortex system timer and FCLK
// {1,2,4,8,16,64,128,256,512} choose any division factor in this list
//                              (32 is not on the list!)
#define _HPRE	16 

// CONFIGURE PPRE1 APB low-speed prescaler (presacler that gives clock to 
// {1,2,4,8,16} choose any division factor in this list
#define _PPRE1	4 

// CONFIGURE PPRE2 APB high-speed prescaler (presacler that gives clock to 
// {1,2,4,8,16} choose any division factor in this list
#define _PPRE2	1 

// CONFIGURE ADCPRE ADC prescaler 
// {1,2,4,6,8} choose any division factor in this list
#define _ADCPRE	1 

#if ((_HPRE!=1)&&(_HPRE!=2)&&(_HPRE != 4)&&(_HPRE!=8)&&(_HPRE!=16)&&(_HPRE!=64)&&(_HPRE!=128)&&(_HPRE!=256)&&(_HPRE!=512) )
#error "Line 75 : _HPRE value should be 1,2,4,8,16,64,128,256 or 512 (32 is not on the list!)"
#endif

#if ((_PPRE1!=1)&&(_PPRE1!=2)&&(_PPRE1!=4)&&(_PPRE1!=8)&&(_PPRE1!=16) )
#error "Line 80 : _PPRE1 value should be 1,2,4,8 or 16"
#endif

#if ((_PPRE2!=1)&&(_PPRE2!=2)&&(_PPRE2!=4)&&(_PPRE2!=8)&&(_PPRE2!=16) )
#error "Line 84 : _PPRE2 value should be 1,2,4,8 or 16"
#endif

#if ((_ADCPRE!=1)&&(_ADCPRE!=2)&&(_ADCPRE!=4)&&(_ADCPRE!=6)&&(_ADCPRE!=8) )
#error "Line 88 : _ADCPRE value should be 1,2,4,6 or 8"
#endif

// end of auxiliary peripherals clock configuration
//_________________________________________________________________________________________




		// LA SUITE DOIT ETRE ENLEVEE ET MODIFIEE


#define __HSI 8000000UL
#define __HSE 8000000UL
#define __SYSCLK 40000000UL
#define __HCLK	(__SYSCLK / 1)
#define __PCLK1 (__HCLK / 2)
#define __PCLK2 (__HCLK / 1)




#define __RCC_CIR_VALUE       (HSERDYIE | PLLRDYIE)  //TODO DEBUG car polling ! 0x00001800



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



void Setup_Pll_As_Clock_System() ;
#endif

/*----------------------------------------------------------------------------
 Define SYSCLK
 *----------------------------------------------------------------------------*/
			