#include <stm32f10x_lib.h>
#include "STM32_Reg.h"
#include <stdio.h> 
//#include <stdlib.h>
#include <string.h>
#include "../lib_cm3/Metro_Librairie_Version2009d.h"
#include "stm_clock.h"




#define __RCC_APB1ENR_TIM2EN	  0x00000001
#define __TIM2_EGR_UG             0x0001                
#define __TIM2_PSC                0x0000                
#define __TIM2_ARR                0x0FFF                
#define __TIM2_CR1                0x0004                   
#define __TIM2_CR2                0x0000                  
#define __TIM2_SMCR               0x0000                 
#define __TIM2_CCMR1              0x6800     //TIM2_CH2             
#define __TIM2_CCMR2              0x0068     // TIM2_CH3                  
#define __TIM2_CCER               0x0110              
#define __TIM2_CCR1               0x0000                
#define __TIM2_CCR2               0x0000            
#define __TIM2_CCR3               0x00FA                 
#define __TIM2_CCR4               0x0000               
#define __TIM2_DIER               0x0001 
#define __TIMX_CR1_CEN			  0x0001



 
#define __RCC_APB1ENR_TIM3EN	  0x00000002
#define __TIM3_EGR_UG             0x0001                
#define __TIM3_PSC                0x0000                
#define __TIM3_ARR                0xFFFF                
#define __TIM3_CR1                0x0004                   
#define __TIM3_CR2                0x0000                  
#define __TIM3_SMCR               0x0003                 
#define __TIM3_CCMR1              0x0101               
#define __TIM3_CCMR2              0x0000                     
#define __TIM3_CCER               0x0011              
#define __TIM3_CCR1               0x0000                
#define __TIM3_CCR2               0x0000            
#define __TIM3_CCR3               0x0000                
#define __TIM3_CCR4               0x0000               
#define __TIM3_DIER               0x0000 
#define __TIMX_CR1_CEN			  0x0001



#define __RCC_APB1ENR_TIM4EN	                     0x00000004
#define __TIM4_EGR_UG                                0x0001                
#define __TIM4_PSC                                   0x00FF                
//#define __TIM4_ARR                                   0x009c               
#define __TIM4_CR1                                   0x0004                   
#define __TIM4_CR2                                   0x0000                  
#define __TIM4_SMCR                                  0x0000                 
#define __TIM4_CCMR1                                 0x0000                  
#define __TIM4_CCMR2                                 0x6800                  
#define __TIM4_CCER                                  0x1000              
#define __TIM4_CCR1                                  0x0000                
#define __TIM4_CCR2                                  0x0000            
#define __TIM4_CCR3                                  0x0000               
#define __TIM4_CCR4                                  0x0000               
#define __TIM4_DIER                                  0x0001 



 
#define __ALTERNATE_OUTPUT_PUSH_PULL_AND_OUTPUT_PUSH_PORTA           0x44300BB0
#define __ENABLE_DRIVER_BRIDGE 		                 0x00000020
#define __RCC_APB2ENR_PORTAEN	                     0x00000004
#define __RCC_APB2ENR_PORTCEN	                     0x00000010
#define __ALTERNATE_OUTPUT_PUSH_PULL_PORTB           0x00008BB0	  
#define __RCC_APB2ENR_PORTBEN	                     0x00000008 
#define __CC3_ON									 0x0100
#define __CC3_OFF									 0xFEFF
#define __CC2_OFF									 0xFFEF
#define __CC2_ON									 0x0010
#define __TIMX_CR1_CDISABLE 						 0xFFFE




#define __AWDEN_MASQUE_OR      0x00800000 // analog watchdog enabled on regular channel
#define __EOCIE_MASQUE_OR      0x00000020 // interrupt  enable for EOC end of conversion
#define __AWDCH_MASQUE_OR      0x0000000C // analog watchdog selects bits -channel 12-
#define __ADON__MASQUE_OR      0x00000001 // A/D converter on /off
#define __CONT__MASQUE_AND     0xFFFFFFFD // Single conversion mode 
#define __CAL__MASQUE_OR       0x00000004 // enable calibration 
#define __SWSTART__MASQUE_OR   0x00400000 // start conversion  of regular  channel
#define __SMP12                ((1<<8)||(1<<7)||(1<<6)) // sample time 239.5 cycles for channel 12 
#define __SQR1                 0x00000000  // only conversion 
#define __SQR3__MASQUE_OR      0x0000000C  // channel 12 in the first conversion in regular sequence 
#define __EXTSEL__MASQUE_OR	   0x000E0000  //these bits select the external event used  to trigger the start of conversion (SWSTART)of a regular group
#define __EXTTRIG__MASQUE_OR   0x00100000  // this bit to enable /disable the external  trigger used to start 
#define __ENABLE_CLOCK_ADC     0x00000200	
#define __ADC_GLOBAL_INTERRUPT 0x00040000 
#define __RESET_EOC_AND_FLAG_START_CONVERSION  	 0xFFFFFFED
#define __TEST_END_OF_CONVERSION				 0x00000002
#define DT_VITESSE  ( DT/0.0000064)
							   





 void Init_PortA()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTAEN;	        	 				  // enable clock for port A
GPIOA->CRL    =__ALTERNATE_OUTPUT_PUSH_PULL_AND_OUTPUT_PUSH_PORTA ; 	              // port pA1, pA2 config as alternate function because it's output pwm(TIM2_CH2, TIM2_CH3), and pA5 config output push pull , PA6,PA7 AS INPUTS FOR CAPTEUR DE POSITION (TIM3_CH1 ,TIM3_CH2)
GPIOA->ODR   |=__ENABLE_DRIVER_BRIDGE;								  // driver motor enable
}


void Init_PortB()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTBEN;	 				             // enable clock for port B
GPIOB->CRH = __ALTERNATE_OUTPUT_PUSH_PULL_PORTB; 	                 // port pB9 config as alternate function because it's output pwm(TIM4_CH4)
} 

  void Init_PortC()
{
RCC->APB2ENR |=__RCC_APB2ENR_PORTCEN;	        	 				  // enable clock for port C
                                            // port pC2 config as ANALG INPUT
GPIOC->CRL   =0x34444044;                   //port pc7 config as output
GPIOC->CRH   =0x44444434;					 //port pc9 config as output

GPIOC->ODR	 |=0x00000280;                   //two leds off 
}

 /*-----------------------------------------config timerx-------------------------------------------------*/

#ifdef USE_PWM
//_________________________________________PWM_________________________________________________
//
// pr�cision sur 12 bits
// TIMER 2 config for PWM outputs
// pas de prescaler sur APB1 (40M)
// reload sur 12 bits => 17,5KHz environ, 
// mode  PWM mode-1
// pas d'interruption
void  Init_Timer2()	  
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
      TIM2->CCMR1 = __TIM2_CCMR1;                           // output compare 3 mode( here it's pwm mode 1) and output compare 3 preload enable
      TIM2->CCMR2 = __TIM2_CCMR2;                           //output compare 3 mode( here it's pwm mode 2) and output compare 3 preload enable
      TIM2->CCER  = __TIM2_CCER;                            // set capture/compare enable register
      TIM2->SMCR  = __TIM2_SMCR;                            //  slave mode control register
      TIM2->CR1   = __TIM2_CR1;                               //  command register 1
      TIM2->CR2   = __TIM2_CR2;  	
	  TIM2->SR=0x0000;							            // reset status register
                           // interrupts 
      TIM2->DIER = __TIM2_DIER;                             // enable interrupt
    //  NVIC->ISER[0] = 0x10000000;							// enable  nested vector interrupt controler
    // TIM2->CR1 |= __TIMX_CR1_CEN;                     // enable timer
                                 
}
 
 
 void Pwm1()	//�a roule en arri�re
{
  
  // bloque TIMER 2 pour changer de canal PWM actif
  // �vite les impulsions parasites
  TIM2->CR1 &=__TIMX_CR1_CDISABLE;
   
  TIM2->CCER &=__CC3_OFF;
  TIM2->CCER |=__CC2_ON;

  // channel 2 actif on r�active le timer
  TIM2->CR1  |= __TIMX_CR1_CEN;
 
  GPIOC->ODR &=0xFFFFFDFF;			  //LED ARRIERE ON
  GPIOC->ODR  |=0x00000080; 		  //LED AVANCE OFF
}



void Pwm2()		//�a roule en avance 
{
  TIM2->CR1 &=__TIMX_CR1_CDISABLE;
  TIM2->CCER &=__CC2_OFF;
  TIM2->CCER |=__CC3_ON; 		                                // output control oc2 signal is output on the corresponding output pin
  TIM2->CR1 |= __TIMX_CR1_CEN;
  GPIOC->ODR &=0xFFFFFF7F;	       //LED AVANCE ON
  GPIOC->ODR |=0x00000200; 		   //LED ARRIERE OFF
}

void Fixe_Rapport( short int duty_cycle)				 // frequency is fixed for 12 bits, so the value of duty cycle is max for 12 bits 4095 
{
 if(duty_cycle > 0)
      { 
	   if(duty_cycle > __TIM2_ARR)
	       TIM2->CCR3 =__TIM2_ARR;
	   else 
		  TIM2->CCR3 = (duty_cycle); 	  	  
	  
	   Pwm2();    
	  }
  else if(duty_cycle < 0)
      {
	   if(duty_cycle < -__TIM2_ARR)
		 TIM2->CCR2 =__TIM2_ARR;
	   else
	     TIM2->CCR2 =-duty_cycle;	   
	    
		Pwm1();
	 
	  }
	  
 if(duty_cycle==0)
   {
    TIM2->CR1 &=__TIMX_CR1_CDISABLE;	
	TIM2->CCER &=__CC2_OFF;
	TIM2->CCER &=__CC3_OFF;
	GPIOC->ODR |=0x00000200;    // LED ARRIERE OFF
	GPIOC->ODR  |=0x00000080;	//LED AVANCE OFF
   }	  
	  
}
 #endif
//_________________________________________FIN PWM_________________________________________________



//_____________________________________________________CODEURS INCREMENTAUX__________________________
#ifdef USE_POSITION
	#define USE_T3
#endif
#ifdef USE_SPEED
	#define USE_T3
#endif
#ifdef USE_T3
void  Init_Timer3()	  
// mode encod incremental-3 (compte et d�compte)
// pas d'interrupt
// r�sultat dans TIM3->CNT (16 bits sign�s) -32535 � 32536 max !!!
// TO DEBUG
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM3EN;                          // enable clock for TIM3
   	  TIM3->EGR= __TIM3_EGR_UG;                                     //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM3->PSC = __TIM3_PSC;                               // set prescaler
      TIM3->ARR = __TIM3_ARR;                               // set auto-reload
      TIM3->CCR1  = __TIM3_CCR1;                            //
      TIM3->CCR2  = __TIM3_CCR2;                            //
      TIM3->CCR3  = __TIM3_CCR3;                            //
      TIM3->CCR4  = __TIM3_CCR4;                            //
      TIM3->CCMR1 = __TIM3_CCMR1;                           //
      TIM3->CCMR2 = __TIM3_CCMR2;                           
      TIM3->CCER  = __TIM3_CCER;                            // set capture/compare enable register
      TIM3->SMCR  = __TIM3_SMCR;                            //  slave mode control register
      TIM3->CR1   = __TIM3_CR1;                               //  command register 1
      TIM3->CR2   = __TIM3_CR2;  	
	  TIM3->SR=0x0000;							            // reset status register
                           // interrupts 
     
	 
	  TIM3->DIER = __TIM3_DIER;                             // enable interrupt
    //  NVIC->ISER[0] = 0x20000000;							// enable  nested vector interrupt controler

      
	  TIM3->CR1 |= __TIMX_CR1_CEN;                     // enable timer
                                 
}
#endif  
#ifdef USE_POSITION
  int Lire_Position()
{
   return (int)	(TIM3->CNT);
}
#endif  
//_____________________________________________FIN CODEURS INCREMENTAUX__________________________

//______________________________________________MESURE VITESSE__________________________
// mode timer
// APB1 � 40MHz
// prescaler � 256
// et reload selon #define de dt en haut (float en s) 0 � TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// interruption valid�e priot� 0
#ifdef USE_SPEED
 void  Init_Timer4()		    
{

      RCC->APB1ENR |= __RCC_APB1ENR_TIM4EN;                 // enable clock for TIM4
	  TIM4->EGR= __TIM4_EGR_UG;                             //Re -initialize the counter and generates an update of the registers

	                          // detailed settings used
      TIM4->PSC = __TIM4_PSC;                               // set prescaler
      TIM4->ARR = DT_VITESSE;                               // set auto-reload
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
	  // priorit� dans NVIC->IP[0]   TODO !!! priorit�es selon defines

     TIM4->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                
}


static int old_position ;
static int new_position =0;

 
void TIM4_IRQHandler(void)
 {
 
 	if((TIM4->SR & 0x0001)) // est-ce un overflow ?(d�passement)
      {
	  	TIM4->SR &= ~(0x0001);		 // reset interrupt flag
		old_position = new_position;
		new_position=Lire_Position();
 	  }		 
  
 }

 int Lire_Vitesse()
{   	
   return  (int) ((float)(new_position-old_position)/(DT));	 
}

#endif
/*-----------------------------------------end config timerx-------------------------------------------------*/



 /*-------------------------------------------config ADC---------------------------------------------------------*/
#ifdef USE_ADC
// APB2 40MHz
// ADC prescaler � 2 : ADC � 20MHz
// mode continuous mode sur channel 12
// end of conversion interrupt valid�e
// temps de conversion 239.5+12.5 cycles = 252 cycles = 12,5 us (de t�te)
// data align�e � droite
 void Setup_Adc()
{
 RCC->APB2ENR |=__ENABLE_CLOCK_ADC;
 ADC1->CR1    |=__AWDEN_MASQUE_OR;
 ADC1->CR1    |=__EOCIE_MASQUE_OR;
 ADC1->CR1    |=__AWDCH_MASQUE_OR;
 ADC1->CR2    |=~__CONT__MASQUE_AND;
 ADC1->CR2    |= __EXTSEL__MASQUE_OR;
 ADC1->CR2    |=__EXTTRIG__MASQUE_OR;
 ADC1->SMPR1  =__SMP12;
 ADC1->SQR1   =__SQR1;
 ADC1->SQR3   |=__SQR3__MASQUE_OR;
// NVIC->ISER[0] |=__ADC_GLOBAL_INTERRUPT;
}

 void Adc_On()
{
ADC1->CR2   |=__ADON__MASQUE_OR;
}

void Start_Conversion()
{
ADC1->CR2   |= __SWSTART__MASQUE_OR; // set conversion but it's  cleared by hardware 
}

 void ADC_IRQHandler(void)						 //THIS BIT IS SET BY HARDWARE AT THE END OF  A GROUP CHANNEL CONVERSION IT4S CLEARED BY SOFTWARE OR BY READING THE ADC_DR
{
   if(ADC1->SR &=__TEST_END_OF_CONVERSION)
        { ADC1->SR &=__RESET_EOC_AND_FLAG_START_CONVERSION;
//         __COURANT = Get_Courant_Moteur();
	    }

}

short int Lire_courant()
{ 
	short int  conv;
  	conv =(ADC1->DR)&(0x0000FFF);
 	return conv; 

}
#endif 
/*---------------------------------------end config ADC----------------------------------------------------*/
 



void Init_Periphs()			  
{
 Setup_Pll_As_Clock_System();
 Init_PortA();
 Init_PortB();
 Init_PortC();

 
 #ifdef USE_ADC
  Setup_Adc();
  Adc_On();
  Start_Conversion();
 #endif

 #ifdef USE_PWM
 Init_Timer2();
 #endif

 #ifdef USE_POSITION
 Init_Timer3();
 #endif

 #ifdef USE_SPEED
 Init_Timer4();
 #endif


}
