#include <stm32f10x_lib.h>
#include "STM32_Reg.h"

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
#define __TIM2_CCMR1              0x6800   //TIM2_CH2             
#define __TIM2_CCMR2              0x0068   // TIM2_CH3                  
#define __TIM2_CCER               0x0110              
#define __TIM2_CCR1               0x0000                
#define __TIM2_CCR2               0x0000            
#define __TIM2_CCR3               0x00FA                 
#define __TIM2_CCR4               0x0000               
#define __TIM2_DIER               0x0001 
#define __TIMX_CR1_CEN			  0x0001


#define __RCC_APB1ENR_TIM4EN	                     0x00000004
#define __TIM4_EGR_UG                                0x0001                
#define __TIM4_PSC                                   0x0000                
#define __TIM4_ARR                                   0x0FFF                
#define __TIM4_CR1                                   0x0004                   
#define __TIM4_CR2                                   0x0000                  
#define __TIM4_SMCR                                  0x0000                 
#define __TIM4_CCMR1                                 0x0000                  
#define __TIM4_CCMR2                                 0x6800                  
#define __TIM4_CCER                                  0x1000              
#define __TIM4_CCR1                                  0x0000                
#define __TIM4_CCR2                                  0x0000            
#define __TIM4_CCR3                                  0x0000               
#define __TIM4_CCR4                                  0x00FA               
#define __TIM4_DIER                                  0x0001 


 
#define __ALTERNATE_OUTPUT_PUSH_PULL_AND_OUTPUT_PUSH_PORTA           0x00300BB0
#define __ENABLE_DRIVER_BRIDGE 		                 0x00000020
#define __RCC_APB2ENR_PORTAEN	                     0x00000004
#define __ALTERNATE_OUTPUT_PUSH_PULL_PORTB           0x000000B0	  
#define __RCC_APB2ENR_PORTBEN	                     0x00000008 
#define __CC3_ON									 0x0100
#define __CC3_OFF									 0xFEFF
#define __CC2_OFF									 0xFFEF
#define __CC2_ON									 0x0010
#define __TIMX_CR1_CDISABLE 						 0xFFFE
							   

 void Init_PortA()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTAEN;	        	 				  // enable clock for port A
GPIOA->CRL    =__ALTERNATE_OUTPUT_PUSH_PULL_AND_OUTPUT_PUSH_PORTA ; 	              // port pA2 config as alternate function because it's output pwm(TIM2_CH2), and pA5 config output push pull
GPIOA->ODR   |=__ENABLE_DRIVER_BRIDGE;								  // driver motor enable
}


void Init_PortB()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTBEN;	 				             // enable clock for port B
GPIOB->CRH = __ALTERNATE_OUTPUT_PUSH_PULL_PORTB; 	                 // port pB9 config as alternate function because it's output pwm(TIM4_CH4)
} 
void  Init_Timer2()	  // PWM1
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM2EN;                          // enable clock for TIM2
   	  TIM2->EGR= __TIM2_EGR_UG;                                     //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM2->PSC = __TIM2_PSC;                               // set prescaler
      TIM2->ARR = __TIM2_ARR;                               // set auto-reload
      TIM2->CCR1  = __TIM2_CCR1;                            //
      TIM2->CCR2  = __TIM2_CCR2;                            //
      TIM2->CCR3  = __TIM2_CCR3;                            //
      TIM2->CCR4  = __TIM2_CCR4;                            //
      TIM2->CCMR1 = __TIM2_CCMR1;                           //
      TIM2->CCMR2 = __TIM2_CCMR2;                           //output compare 3 mode( here it's pwm mode 1) and output compare 3 preload enable
      TIM2->CCER  = __TIM2_CCER;                            // set capture/compare enable register
      TIM2->SMCR  = __TIM2_SMCR;                            //  slave mode control register
      TIM2->CR1   = __TIM2_CR1;                               //  command register 1
      TIM2->CR2   = __TIM2_CR2;  	
	  TIM2->SR=0x0000;							            // reset status register
                           // interrupts 
      TIM2->DIER = __TIM2_DIER;                             // enable interrupt
      NVIC->ISER[0] = 0x10000000;							// enable  nested vector interrupt controler

      //TIM2->CR1 |= __TIMX_CR1_CEN;                     // enable timer
                                 
}


void  Init_Timer4()		      //PWM2
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM4EN;                 // enable clock for TIM4
	  TIM4->EGR= __TIM4_EGR_UG;                             //Re -initialize the counter and generates an update of the registers

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
      TIM4->SMCR  = __TIM4_SMCR;                            //  slave mode control register
      TIM4->CR1   = __TIM4_CR1;                             //  command register 1
      TIM4->CR2   = __TIM4_CR2;  	
	  TIM4->SR=0x0000;							 			// reset status register

	                              

                           // interrupts used
      TIM4->DIER = __TIM4_DIER;                             // enable interrupt
	  NVIC->ISER[0] = 0x40000000;  							// enable  nested vector interrupt controler

     // TIM4->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                
}

 void Pwm1()
{ 
  TIM2->CR1 &=__TIMX_CR1_CDISABLE;
  TIM2->CCER &=__CC3_OFF;
  TIM2->CCER |=__CC2_ON;
  TIM2->CR1  |= __TIMX_CR1_CEN;
}



void Pwm2()
{ 
  TIM2->CR1 &=__TIMX_CR1_CDISABLE;
  TIM2->CCER &=__CC2_OFF;
  TIM2->CCER |=__CC3_ON; 		                                // output control oc2 signal is output on the corresponding output pin
  TIM2->CR1 |= __TIMX_CR1_CEN;
}


void Setup_Pll_As_Clock_System()
{ RCC->CR   =__RCC_CR_RESET ;
  RCC->CFGR =__RCC_CFGR_RESET;
  RCC->CIR  =__RCC_CIR_RESET;
  RCC->CFGR |=__RCC_CFGR_SET1;
  RCC->CFGR &=__RCC_CFGR_SET0;
  RCC->CIR  |=__RCC_CIR;
  RCC->CR   |=__RCC_CR_HSE_AND_PLL_ON;
}

void Init_Periphs()			  
{
 Setup_Pll_As_Clock_System();
 Init_PortA();
 Init_PortB();
 Init_Timer2();
 //Init_Timer4();
}

void Fixe_Rapport(int duty_cycle)				 // frequency is fixed for 12 bits, so the value of duty cycle is max for 12 bits 4095 
{
 if(duty_cycle > 0)
      { 
	   if(duty_cycle > 4095)
	      TIM2->CCR2 =0x0FFF;
	   else 
	      TIM2->CCR2 =duty_cycle; 	  	  
	   Pwm1();
	  }
  else if(duty_cycle < 0)
      {
	   if(duty_cycle < -4095)
	     TIM2->CCR3 =0x0FFF;
	   else 
	   TIM2->CCR3 = -(duty_cycle); 	   
	   Pwm2();
	  }
	  
 if(duty_cycle==0)
   {
    TIM2->CR1 &=__TIMX_CR1_CDISABLE;	
	//TIM4->CR1 &=__TIMX_CR1_CDISABLE;
	TIM4->CCER &=__CC2_OFF;
	TIM2->CCER &=__CC3_OFF;
   }	  
	  
}
