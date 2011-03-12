/*
________________________________________________________________________________________
	configuration for usart driver
	usart_config[_TEMMPLATE].h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. cm3_traps
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
	[Acco 12/03/2011] finalisation et commentaires de la première version
_______________________________________________________________________________________
TODO
  Remaps des ports
_____________________________________________________________________________________
*/
#ifndef __ADC_CONFIG__
#define __ADC_CONFIG__


/*	TODO	AFIO_MAPR
Bits 20 ADC2_ETRGREG_REMAP: ADC 2 external trigger regular conversion remapping
Bits 19 ADC2_ETRGINJ_REMAP: ADC 2 external trigger injected conversion remapping
Bits 18 ADC1_ETRGREG_REMAP: ADC 1 external trigger regular conversion remapping
Bits 17 ADC1_ETRGINJ_REMAP: ADC 1 External trigger injected conversion remapping
*/
//CONFIGURE TODO
#define __ADC1_ETRGREG_REMAP 0x0
// 0 ->  ?
// 1 -> ?

// CONFIGURE go to stm_clock_config to configure ADCprescaler

/* Modes you can choose or not
OK		NO_ADC_USED
DEVEL	ADC_DUALMOD_IS_INDEPENDANT 	
TODO	ADC_DUALMOD_IS_COMBINED_1 
TODO	ADC_DUALMOD_IS_COMBINED_2 	
TODO	ADC_DUALMOD_IS_COMBINED_3 
TODO	ADC_DUALMOD_IS_COMBINED_4 
TODO	ADC_DUALMOD_IS_INJECTED 	
TODO	ADC_DUALMOD_IS_REGULAR 	
TODO	ADC_DUALMOD_IS_FAST 		
TODO	ADC_DUALMOD_IS_SLOW 	
TODO	ADC_DUALMOD_IS_ALTERNATE 	
*/
// CONFIGURE your mode
#define __ADC_MODE ADC_DUALMOD_IS_INDEPENDANT

#if (__ADC_MODE==ADC_DUALMOD_IS_INDEPENDANT)
	#define ADC1_IS_USED

	//CONFIGURE in independant if you use ADC2
	#define ADC_2_IS_USED

#else
   //NO ADC USED

#endif //ADC_MODE


//__________________________________ADC1___________________________________
#ifdef ADC1_IS_USED

	
	//CONFIGURE scan mode for multiple channel conversion
	#define SCAN_MODE //TODO et si on mets pas scan ça fait quoi ?

	// CONFIGURE sequence length from 1 to 16
	#define SEQ_L 1
	// CONFIGURE sequence of channels to scan
	#define S1C	10  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S2C	11  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S3C	12  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S4C	13  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S5C	14  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S6C	15  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S7C	16  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S8C	17  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S9C	10  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S10C 0  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S11C 1  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S12C 2  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S13C 3  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S14C 4  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S15C 5  // channel from 0 to  15, or 16=temp or 17=Vref
	#define S16C 6  // channel from 0 to  15, or 16=temp or 17=Vref

	//CONFIGURE each ADC1 channel sampling time
	// CxT  =        |0  |1  |2   |3   |4   |5   |6   |7    |
	// correspond to |1.5|7.5|13.5|28.5|41.5|55.5|71.5|239.5| cycles
	#define C0T	 0  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C1T	 1  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C2T	 2  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C3T	 3  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C4T	 4  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C5T	 5  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C6T	 6  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C7T	 7  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C8T	 0  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C9T	 1  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C10T 2  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C11T 3  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C12T 4  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C13T 5  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C14T 6  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C15T 7  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C16T 0  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C17T 1  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5
	#define C18T 2  // 0=1.5 1=7.5 2=13.5 3=28.5 4=41.5 5=55.5 6=71.5 7=239.5

	#define IS_CONTINUOUS (ADC_CONT)
	#define IS_SINGLE (0)

	// CONFIGURE single or continuous conversion mode
	//#define CONV_TYPE IS_CONTINUOUS 
	#define CONV_TYPE IS_SINGLE 

	#if (CONV_TYPE==IS_SINGLE)
	//CONFIGURE trigger event when single conversion
	// if no event selected then trigger is software
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM1_CC1
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM1_CC2
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM1_CC3
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM2_CC2
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM3_TRGO
	//#define EXT_TRIG ADC_EXTSEL_IS_TIM4_CC4
	//#define EXT_TRIG ADC_EXTSEL_IS_EXTI
	//#define EXT_TRIG ADC_EXTSEL_IS_SWSTART
	#endif //External event selection
	
	// CONFIGURE use of DMA	 (circular incremental mode)
	#define ADC1_USE_DMA
	#ifdef ADC1_USE_DMA
		//CONFIGURE number of group conversion to be buffered
		#define N_SAMPLES (256) 

		//CONFIGURE DMA priority level
		//#define ADC_DMA_PL	DMA_PL_IS_LOW   	
		//#define ADC_DMA_PL	DMA_PL_IS_MEDIUM 
		//#define ADC_DMA_PL	DMA_PL_IS_HIGH  
		#define ADC_DMA_PL	DMA_PL_IS_VERYHIGH

		//CONFIGURE if Transfert Error Interrupt is needed
		#define ADC_DMA_TEIT
		//CONFIGURE protoype and name of hook to execute on TEIE
		extern void Adc_Dma_Pannic(void);
		#define ADC_DMA_TE_HOOK Adc_Dma_Pannic()

		//CONFIGURE if Hamf Transfert Complete Interrupt is needed
		#define ADC_DMA_HTIT
		//CONFIGURE protoype and name of hook to execute on HTIE
		extern void Adc_Dma_Half(void);
		#define ADC_DMA_HT_HOOK Adc_Dma_Half()

		//CONFIGURE if Transfert Complete Interrupt is needed
		#define ADC_DMA_TCIT
		//CONFIGURE protoype and name of hook to execute on TCIE
		extern void Adc_Dma_Finished(void);
		#define ADC_DMA_TC_HOOK Adc_Dma_Finished()
		
		//CONFIGURE dma interrupt priority level
		#define ADC_DMA_PRIO 4

	#endif //USE_DMA

	//CONFIGURE if EndOfConversion Interrupt is needed
	#define ADC_EOCIT
	//CONFIGURE protoype and name of hook to execute on EOCIT
	extern void Adc_Eoc(void);
	#define ADC_EOC_HOOK Adc_Eoc();

	//CONFIGURE if EndOf Injected Conversion Interrupt is needed
	#define ADC_JEOCIT
	//CONFIGURE protoype and name of hook to execute on JEOCIT
	extern void Adc_Jeoc(void);
	#define ADC_JEOC_HOOK Adc_Jeoc();

	//CONFIGURE if Analog Watch Dog Interrupt is needed
	#define ADC_AWDIT
	//CONFIGURE protoype and name of hook to execute on AWDIT
	extern void Adc_Awd(void);
	#define ADC_AWD_HOOK Adc_Awd();
	
	//CONFIGURE adc interrupt priority level
	#define ADC_PRIO 7
#endif //ADC1_IS_USED



//__________________________________________________________________________
//  ADC1 master configuration
//__________________________________________________________________________



#endif /* __STM_ADC_CONFIG__ */

