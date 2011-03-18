/*
________________________________________________________________________________________
	ADC driver files for stm32f103
	Configuration is located in file stm_adc_config.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. stm_adc
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
			
________________________________________________________________________________________
TODO

________________________________________________________________________________________
*/

#include "stm_adc_config.h" 
#include "stm_regs.h" 
#include "stm_system.h" 

#ifdef ADC1_USE_DMA
#define ADC_BUFF_LENGTH (SEQ_L*N_SAMPLES)	 
short int Adc_Buff[ADC_BUFF_LENGTH];
const short int * Adc_Buff_End = &(Adc_Buff[ADC_BUFF_LENGTH-1]);
const short int * Adc_Buff_Start = &(Adc_Buff[0]);


#endif
void Init_Adc(void)
{
	
	#ifdef ADC1_IS_USED
	(RCC->APB2ENR) |= RCC_ADC1EN; // clock enable

	// Switch on ADC1
	ADC1->CR2 |= ADC_ADON;

	ADC1->CR1 |= __ADC_MODE ;
	ADC1->CR2 |= CONV_TYPE ; // CONT bit;

	#ifdef SCAN_MODE
	ADC1->CR1 |= ADC_SCAN ;
	#endif 

	
	#ifdef EXT_TRIG
	ADC1->CR2 |= ADC_EXTTRIG ; // CONT bit;
	ADC1->CR2 |= EXT_TRIG ; // CONT bit;
	#else
	ADC1->CR2 |= ADC_EXTSEL_IS_SWSTART ;
	#endif // External trigger conf

    //regular groupe sequence config
	ADC1->SQR3 = (S1C<<0)|(S2C<<5)|(S3C<<10)|(S4C<<15)|(S5C<<20)|(S6C<<25);
	ADC1->SQR2 = (S7C<<0)|(S8C<<5)|(S9C<<10)|(S10C<<15)|(S11C<<20)|(S12C<<25);
	ADC1->SQR1 = (S13C<<0)|(S14C<<5)|(S15C<<10)|(S16C<<15)|((SEQ_L-1)<<20);

	ADC1->SMPR2 = (C0T<<0) |(C1T<<3) |(C2T<<6) |(C3T<<9) |(C4T<<12) |(C5T<<15) |(C6T<<18) |(C7T<<21) |(C8T<<24)|(C9T<<27);
	ADC1->SMPR1 = (C10T<<0)|(C11T<<3)|(C12T<<6)|(C13T<<9)|(C14T<<12)|(C15T<<15)|(C16T<<18)|(C17T<<21)|(C18T<<24);

	// ADC interrupt enables
	#ifdef ADC_EOCIT
	ADC1->CR1 |= ADC_EOCIE ;
	#endif
 	#ifdef ADC_JEOCIT
	ADC1->CR1 |= ADC_JEOCIE ;
	#endif
 	#ifdef ADC_AWDIT
	ADC1->CR1 |= ADC_AWDIE ;
	#endif

 	// ADC interrupt prio 
	#if (defined(ADC_EOCIT) || defined(ADC_JEOCIT) || defined(ADC_AWDIT))
	#define ADC_IT
	NVIC_ENABLE_PERIPH_IT(ADC1_2);
	NVIC_SET_PRIO_PERIPH(ADC1_2,ADC_PRIO);
	#endif

	// DMA configuration
	#ifdef ADC1_USE_DMA
	ADC1->CR1 |= ADC_EOCIE ;
	ADC1->CR2 |= ADC_DMA ;

	RCC->AHBENR |=RCC_DMA1EN;// DMA1 CLOCK ENABLE
	DMA1_Channel1->CPAR =(u32)&(ADC1->DR); // @ source 
 	DMA1_Channel1->CCR |= (DMA_PSIZE_IS_16BITS | DMA_MSIZE_IS_16BITS);   //ADC1-CR is 16 bits 
  	DMA1_Channel1->CMAR =(u32) Adc_Buff;   // @destination
	DMA1_Channel1->CCR &= ~(DMA_DIR); //from periph to memory		
  	DMA1_Channel1->CCR |=DMA_CIRC;   //Circular mode 
 	DMA1_Channel1->CNDTR=ADC_BUFF_LENGTH;
  	DMA1_Channel1->CCR |=DMA_MINC;   //Increment 
  	DMA1_Channel1->CCR |=ADC_DMA_PL;	// priority level

	// DMA interrupt enables
	#ifdef ADC_DMA_TEIT
  	DMA1_Channel1->CCR |=DMA_TEIE;
	#endif
 	#ifdef ADC_DMA_HTIT
  	DMA1_Channel1->CCR |=DMA_HTIE;
	#endif
 	#ifdef ADC_DMA_TCIT
  	DMA1_Channel1->CCR |=DMA_TCIE;
	#endif

 	// DMA interrupt prio 
	#if (defined(ADC_DMA_HTIT) || defined(ADC_DMA_TCIT) || defined(ADC_DMA_TEIT))
	#define ADC_DMA_IT
	NVIC_ENABLE_PERIPH_IT(DMA1_CHANNEL1);
	NVIC_SET_PRIO_PERIPH(DMA1_CHANNEL1,ADC_DMA_PRIO);
	#endif //Dma prio

  	DMA1_Channel1->CCR |= DMA_EN;	// l'DMA bazar est en marche
	#endif	//DMA config


	#endif //ADC1
	
	//____________ADC2 config______________________________________
	#ifdef ADC_2_IS_USED
	(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_ADC2EN;
	// NO Mode conf for ADC2 (its done on ADC1)

	// Switch on ADC2
	ADC2->CR2 |= ADC_ADON; 
	#endif //ADC2

}

#ifdef ADC_DMA_IT
void DMAChannel1_IRQHandler (void)
{
	#ifdef ADC_DMA_TEIT
	#if (!defined(ADC_DMA_HTIT) && !defined(ADC_DMA_TCIT))
	ADC_DMA_TE_HOOK;
	#else 
	if ( (DMA1->ISR & DMA_TEIF1 ) ) ADC_DMA_TE_HOOK;
	#endif
	#endif

	#ifdef ADC_DMA_HTIT
	#if (!defined(ADC_DMA_TEIT) && !defined(ADC_DMA_TCIT))
	ADC_DMA_HT_HOOK;
	#else 
	if ( (DMA1->ISR & DMA_HTIF1 ) ) ADC_DMA_HT_HOOK;
	#endif
	#endif

	#ifdef ADC_DMA_TCIT
	#if (!defined(ADC_DMA_HTIT) && !defined(ADC_DMA_TEIT))
	ADC_DMA_TC_HOOK;
	#else 
	if ( (DMA1->ISR & DMA_TCIF1 ) ) ADC_DMA_TC_HOOK;
	#endif
	#endif


	DMA1->IFCR |= (DMA_CTEIF1 | DMA_CHTIF1 | DMA_CTCIF1 | DMA_CGIF1); //reset interrupt flag

} 
#endif //ADC1 DMA

#ifdef ADC_IT
void ADC_IRQHandler(void)
{
	#ifdef ADC_EOCIT
	#if (!defined(ADC_JEOCIT) && !defined(ADC_AWDIT))
	ADC_EOC_HOOK;
	#else 
	if (ADC1->SR & ADC_EOC ) ADC_EOC_HOOK;
	#endif
	#endif

	#ifdef ADC_JEOCIT
	#if (!defined(ADC_EOCIT) && !defined(ADC_AWDIT))
	ADC_JEOC_HOOK;
	#else 
	if (ADC1->SR & ADC_JEOC ) ADC_JEOC_HOOK;
	#endif
	#endif

	#ifdef ADC_AWDIT
	#if (!defined(ADC_JEOCIT) && !defined(ADC_EOCIT))
	ADC_AWD_HOOK;
	#else 
	if (ADC1->SR & ADC_AWD ) ADC_AWD_HOOK;
	#endif
	#endif

	ADC1->SR &= ~(ADC_EOC | ADC_JEOC | ADC_AWD);
	
}
#endif //ADC IT
