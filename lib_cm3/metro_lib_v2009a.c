#include <stm32f10x_lib.h>
#include "STM32_Reg.h"

#define __ALTERNATE_OUTPUT_PUSH_PULL  0x003000B0
#define __ENABLE_DRIVER_BRIDGE 		  0x00000020
#define __RCC_APB2ENR_PORTAEN	  0x00000004
#define __RCC_APB2ENR_CLOCK_ALTERNATE	  0x00000001

#define __RCC_CR_HSE_AND_PLL_ON	  0x01010000
#define __RCC_CFGR_SET1	          0x001D0402
#define __RCC_CFGR_SET0	          0xFFDDC40E
#define __RCC_CIR            	  0x00001800

#define __RCC_CR_RESET            0x00000083
#define __RCC_CFGR_RESET          0x00000000
#define __RCC_CIR_RESET           0x00000000


#define __RCC_APB1ENR_TIM2EN	  0x00000001
#define __TIM2_EGR_UG             0x0001                
#define __TIM2_PSC                0x0000                
#define __TIM2_ARR                0x0FFF                
#define __TIM2_CR1                0x0004                   
#define __TIM2_CR2                0x0000                  
#define __TIM2_SMCR               0x0000                 
#define __TIM2_CCMR1              0x7800                  
#define __TIM2_CCMR2              0x0000                  
#define __TIM2_CCER               0x0010              
#define __TIM2_CCR1               0x0000                
#define __TIM2_CCR2               0x00FA            
#define __TIM2_CCR3               0x0000               
#define __TIM2_CCR4               0x0000               
#define __TIM2_DIER               0x0001 
#define __TIMX_CR1_CEN			  0x0001


#define  OUTPUT_PUSH_PULL         0xBBBBBBBB	  //ALTERNATE 
#define  RCC_APB2ENR_PORTBEN	  0x00000008
//-----------------------------------------------------
#define __RCC_APB1ENR_TIM4EN	      0x00000004
#define __TIM4_EGR_UG             0x0001                
#define __TIM4_PSC                0x0000                
#define __TIM4_ARR                0x0FFF                
#define __TIM4_CR1                0x0004                   
#define __TIM4_CR2                0x0000                  
#define __TIM4_SMCR               0x0000                 
#define __TIM4_CCMR1              0x0000                  
#define __TIM4_CCMR2              0x7800                  
#define __TIM4_CCER               0x1000              
#define __TIM4_CCR1               0x0000                
#define __TIM4_CCR2               0x0000            
#define __TIM4_CCR3               0x0000               
#define __TIM4_CCR4               0x00FA               
#define __TIM4_DIER               0x0001 
//-------------------------------------------------------
#define __RCC_APB2ENR_TIM1EN	  0x00000800
#define __TIM1_EGR_UG             0x000F                
#define __TIM1_PSC                0x000A                
#define __TIM1_ARR                0x000A                
#define __TIM1_CR1                0x0004                   
#define __TIM1_CR2                0x0000                  
#define __TIM1_SMCR               0x0000                 
#define __TIM1_CCMR1              0x0000                  
#define __TIM1_CCMR2              0x0000                  
#define __TIM1_CCER               0x0000              
#define __TIM1_CCR1               0x0000                
#define __TIM1_CCR2               0x0000            
#define __TIM1_CCR3               0x0000               
#define __TIM1_CCR4               0x0000               
#define __TIM1_DIER               0x0001 
//-----------------------------------------------------------------













#define __NVIC_SETUP              0
#define __NVIC_USED               0x00000000
#define __NVIC_VTOR_VAL           0x00000000


void Setup_Pll_As_Clock_System()
{ RCC->CR   =__RCC_CR_RESET ;
  RCC->CFGR =__RCC_CFGR_RESET;
  RCC->CIR  =__RCC_CIR_RESET;
  
  RCC->CFGR |=__RCC_CFGR_SET1;
  RCC->CFGR &=__RCC_CFGR_SET0;
  
  RCC->CIR  |=__RCC_CIR;
  RCC->CR   |=__RCC_CR_HSE_AND_PLL_ON;
}

void init_portA()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTAEN;	 // activer l'horloge des registres peripheriques
GPIOA->CRL = __ALTERNATE_OUTPUT_PUSH_PULL; 	  //configurer le port B high comme de sortie push_pull
GPIOA->ODR |=__ENABLE_DRIVER_BRIDGE;
} 
void  init_timer1()
{

      RCC->APB2ENR |= __RCC_APB2ENR_TIM1EN;                     // enable clock for TIM1

  
                                // TIM3 used
     

	  TIM1->EGR= __TIM1_EGR_UG;     //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM1->PSC = __TIM1_PSC;                               // set prescaler
      TIM1->ARR = __TIM1_ARR;                               // set auto-reload
      TIM1->CCR1  = __TIM1_CCR1;                            //
      TIM1->CCR2  = __TIM1_CCR2;                            //
      TIM1->CCR3  = __TIM1_CCR3;                            //
      TIM1->CCR4  = __TIM1_CCR4;                            //
      TIM1->CCMR1 = __TIM1_CCMR1;                           //
      TIM1->CCMR2 = __TIM1_CCMR2;                           //
      TIM1->CCER  = __TIM1_CCER;                            // set capture/compare enable register
      TIM1->SMCR  = __TIM1_SMCR;                            // set slave mode control register
      TIM1->CR1 = __TIM1_CR1;                               // set command register 1
      TIM1->CR2 = __TIM1_CR2;  	
	  TIM1->SR=0x0000;							 // set command register 2

	                              

                           // interrupts used
      TIM1->DIER = __TIM1_DIER;                             // enable interrupt
       //NVIC->ISER[0] |= (1 << (TIM3_IRQChannel & 0x1F));     // enable interrupt
  
	  NVIC->ISER[0] = 0x02000000;

      TIM1->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                 // end TIM1 used
  


}









void init_portB()
{
RCC->APB2ENR |=RCC_APB2ENR_PORTBEN;	 // activer l'horloge des registres peripheriques
GPIOB->CRH = OUTPUT_PUSH_PULL; 	  //configurer le port B high comme de sortie push_pull
} 

void init_portC()
{
RCC->APB2ENR |=0x00000010;	 // activer l'horloge des registres peripheriques
GPIOC->CRL =0x00000000; 	  //configurer le port B high comme de sortie push_pull
} 
void  init_timer2()
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM2EN;                     // enable clock for TIM2

  
                                // TIM2 used
     

	  TIM2->EGR= __TIM2_EGR_UG;     //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM2->PSC = __TIM2_PSC;                               // set prescaler
      TIM2->ARR = __TIM2_ARR;                               // set auto-reload
      TIM2->CCR1  = __TIM2_CCR1;                            //
      TIM2->CCR2  = __TIM2_CCR2;                            //
      TIM2->CCR3  = __TIM2_CCR3;                            //
      TIM2->CCR4  = __TIM2_CCR4;                            //
      TIM2->CCMR1 = __TIM2_CCMR1;                           //
      TIM2->CCMR2 = __TIM2_CCMR2;                           //
      TIM2->CCER  = __TIM2_CCER;                            // set capture/compare enable register
      TIM2->SMCR  = __TIM2_SMCR;                            // set slave mode control register
      TIM2->CR1 = __TIM2_CR1;                               // set command register 1
      TIM2->CR2 = __TIM2_CR2;  	
	  TIM2->SR=0x0000;							 // set command register 2

	                              

                           // interrupts used
      TIM2->DIER = __TIM2_DIER;                             // enable interrupt
       //NVIC->ISER[0] |= (1 << (TIM3_IRQChannel & 0x1F));     // enable interrupt
  
	  NVIC->ISER[0] = 0x10000000;

      //---TIM2->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                 // end TIM2 used
  


}

void  init_timer4()
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM4EN;                     // enable clock for TIM4

  
                                // TIM4 used
     

	  TIM4->EGR= __TIM4_EGR_UG;  //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM4->PSC = __TIM4_PSC;                               // set prescaler
      TIM4->ARR = __TIM4_ARR;                               // set auto-reload
      TIM4->CCR1  = __TIM4_CCR1;                            //
      TIM4->CCR2  = __TIM4_CCR2;                            //
      TIM4->CCR3  = __TIM4_CCR3;                            //
      TIM4->CCR4  = __TIM4_CCR4;                            //
      TIM4->CCMR1 = __TIM4_CCMR1;                           //
      TIM4->CCMR2 = __TIM4_CCMR2;                           //
      TIM4->CCER  = __TIM4_CCER;                            // set capture/compare enable register
      TIM4->SMCR  = __TIM4_SMCR;                            // set slave mode control register
      TIM4->CR1   = __TIM4_CR1;                               // set command register 1
      TIM4->CR2   = __TIM4_CR2;  	
	  TIM4->SR=0x0000;							 // set command register 2

	                              

                           // interrupts used
      TIM4->DIER = __TIM4_DIER;                             // enable interrupt
       //NVIC->ISER[0] |= (1 << (TIM3_IRQChannel & 0x1F));     // enable interrupt
  
	  NVIC->ISER[0] = 0x40000000;  

     //--- TIM4->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                 // end TIM4 used
  



}



  void stm32_NvicSetup (void) {

  if (__NVIC_USED & (1 << 0)) {                              // Vector Table Offset Register
    SCB->VTOR = (__NVIC_VTOR_VAL & (u32)0x1FFFFF80);         // set register
  }

}
//---------Configuration ADC-------------------------------------------------------------

#define __AWDEN_MASQUE_OR      0x00800000 // analog watchdog enabled on regular channel
#define __EOCIE_MASQUE_OR      0x00000020 // interrupt  enable for EOC end of conversion
#define __AWDCH_MASQUE_OR      0x0000000C // analog watchdog selects bits -channel 12-
#define __ADON__MASQUE_OR      0x00000001 // A/D converter on /off
#define __CONT__MASQUE_AND     0xFFFFFFFD // Single conversion mode 
#define __CAL__MASQUE_OR       0x00000004 // enable calibration 
#define __SWSTART__MASQUE_OR   0x00400000 // start conversion  of regular  channel
#define __SMP12                0x00000000  // sample time 1.5 cycles for channel 12 
#define __SQR1                 0x00000000  // only conversion 
#define __SQR3__MASQUE_OR      0x0000000C  // channel 12 in the first conversion in regular sequence 
#define __EXTSEL__MASQUE_OR	   0x000E0000  //these bits select the external event used  to trigger the start of conversion (SWSTART)of a regular group
#define __EXTTRIG__MASQUE_OR   0x00100000  // this bit to enable /disable the external  trigger used to start 
#define __ENABLE_CLOCK_ADC     0x00000200	
#define __ADC_GLOBAL_INTERRUPT 0x00040000 




void Setup_Adc()
{
 RCC->APB2ENR |=__ENABLE_CLOCK_ADC;
 ADC1->CR1    |=__AWDEN_MASQUE_OR;
 ADC1->CR1    |=__EOCIE_MASQUE_OR;
 ADC1->CR1    |=__AWDCH_MASQUE_OR;
 ADC1->CR2    &=__CONT__MASQUE_AND;
 ADC1->CR2    |= __EXTSEL__MASQUE_OR;
 ADC1->CR2    |=__EXTTRIG__MASQUE_OR;
 ADC1->SMPR1  =__SMP12;
 ADC1->SQR1   =__SQR1;
 ADC1->SQR3   |=__SQR3__MASQUE_OR;
  NVIC->ISER[0] |=__ADC_GLOBAL_INTERRUPT;
}

void Calibration_Adc() // appel une fois au froid mais au mois deux cycles AVANT d'activer l'ADC 
{
 ADC1->CR2   |=__CAL__MASQUE_OR ;    // set calibration but it's  cleared by hardware 
}

void Adc_On()
{
ADC1->CR2   |=__ADON__MASQUE_OR;
}

void Start_Conversion()
{
ADC1->CR2   |= __SWSTART__MASQUE_OR; // set conversion but it's  cleared by hardware 
}

u32 Get_Courant_Moteur()
{ u32  __DATA_ADC;
  __DATA_ADC =(ADC1->DR)&(0x0000FFFF);
 return __DATA_ADC; 

}
//---------------------------------End configuration ADC-------------------------------------------


 //------------------------------------configuration SysTik	---------------------------------

 void Setup_Systick_Timer()
 {
/* STK->CTRL &=0xFFFFFFFB; // clock source is AHB/8
 STK->CTRL |=0x00000002;  // counting  down to zero to asserts the systick exception request
 STK->LOAD =0x00FFFFFF;	  // (1/(9MHZ))*(STK->LOAD)	==> EXCEPTION CHAQUE 1.8 SECONDE
 STK->CTRL |=0x00000001; //  COUNTER  ENABLE 
 
 */}








#define __TIM3_EGR_UG             0x0001                
#define __TIM3_PSC                0xFFFF                
#define __TIM3_ARR                0x0FFF               
#define __TIM3_CR1                0x0000                   
#define __TIM3_CR2                0x0000                  
#define __TIM3_SMCR               0x0000                 
#define __TIM3_CCMR1              0x0000                  
#define __TIM3_CCMR2              0x0000                  
#define __TIM3_CCER               0x0000              
#define __TIM3_CCR1               0x0000                
#define __TIM3_CCR2               0x0000            
#define __TIM3_CCR3               0x0000               
#define __TIM3_CCR4               0x0000               
#define __TIM3_DIER               0x0001 

 void  init_timer3()
{

      RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;                     // enable clock for TIM3

  
                                // TIM3 used
     

	  TIM3->EGR= __TIM3_EGR_UG;  //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM3->PSC = __TIM3_PSC;                               // set prescaler
      TIM3->ARR = __TIM3_ARR;                               // set auto-reload
      TIM3->CCR1  = __TIM3_CCR1;                            //
      TIM3->CCR2  = __TIM3_CCR2;                            //
      TIM3->CCR3  = __TIM3_CCR3;                            //
      TIM3->CCR4  = __TIM3_CCR4;                            //
      TIM3->CCMR1 = __TIM3_CCMR1;                           //
      TIM3->CCMR2 = __TIM3_CCMR2;                           //
      TIM3->CCER  = __TIM3_CCER;                            // set capture/compare enable register
      TIM3->SMCR  = __TIM3_SMCR;                            // set slave mode control register
      TIM3->CR1 = __TIM3_CR1;                               // set command register 1
      TIM3->CR2 = __TIM3_CR2;  	
	  TIM3->SR=0x0000;							 // set command register 2

	                              

                           // interrupts used
      TIM3->DIER = __TIM3_DIER;                             // enable interrupt
       //NVIC->ISER[0] |= (1 << (TIM3_IRQChannel & 0x1F));     // enable interrupt
  
	  NVIC->ISER[0] = 0x20000000;

      TIM3->CR1 |= TIMX_CR1_CEN;                              // enable timer
                                 // end TIM3 used
  


}
