#include <stm32f10x_lib.h>
#include "STM32_Reg.h"
#include <stdio.h> 
//#include <stdlib.h>
#include <string.h>
#include "../lib_cm3/Metro_Librairie_Version2009d.h"



#define __RCC_CR_HSE_AND_PLL_ON	  0x01010000
#define __RCC_CFGR_SET1	          0x000D0402	  
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
							   


/*--------------------------------------config clock system 40Mhz -------------------------------------------------*/
//
// HSE clock en entrée (quartz 8M)
// PLL x5 => 40M
// AHB 40M
// APB1 APB2 input à 40M 
// APB1 output à 20M (PORTA et TIM2-6,...) 
// APB2 output à 40M (PORTA et TIM2-6,...)
//   tout périph APB2 à 40 sauf l'ADC à 20M
// Pas d'interruption de clock

void Setup_Pll_As_Clock_System()
{ RCC->CR   =__RCC_CR_RESET ;
 // AHB sur la clock interne HSI
  RCC->CFGR =__RCC_CFGR_RESET;
  RCC->CIR  =__RCC_CIR_RESET;
  
  RCC->CFGR |=__RCC_CFGR_SET1;
  RCC->CFGR &=__RCC_CFGR_SET0;
  
  RCC->CIR  |=__RCC_CIR;
 
  //AHB sur PLL qui est sur HSE
  RCC->CR |=0x00010000;
 
  // wait for HSE OK
  while((RCC->CR & 0x00020000) ==0);
  
  //active la PLL
  RCC->CR |=0x01000000;
  
  //wait PLL OK
  while((RCC->CR & 0x02000000) ==0);

  //RCC->CR   |=__RCC_CR_HSE_AND_PLL_ON;
}

 /*------------------------------------end config clock system 40Mhz ------------------------------------------------*/



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
// précision sur 12 bits
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
 
 
 void Pwm1()	//ça roule en arriére
{
  
  // bloque TIMER 2 pour changer de canal PWM actif
  // évite les impulsions parasites
  TIM2->CR1 &=__TIMX_CR1_CDISABLE;
   
  TIM2->CCER &=__CC3_OFF;
  TIM2->CCER |=__CC2_ON;

  // channel 2 actif on réactive le timer
  TIM2->CR1  |= __TIMX_CR1_CEN;
 
  GPIOC->ODR &=0xFFFFFDFF;			  //LED ARRIERE ON
  GPIOC->ODR  |=0x00000080; 		  //LED AVANCE OFF
}



void Pwm2()		//ça roule en avance 
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
// mode encod incremental-3 (compte et décompte)
// pas d'interrupt
// résultat dans TIM3->CNT (16 bits signés) -32535 à 32536 max !!!
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
// APB1 à 40MHz
// prescaler à 256
// et reload selon #define de dt en haut (float en s) 0 à TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// interruption validée prioté 0
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
	  // priorité dans NVIC->IP[0]   TODO !!! prioritées selon defines

     TIM4->CR1 |= __TIMX_CR1_CEN;                              // enable timer
                                
}


static int old_position ;
static int new_position =0;

 
void TIM4_IRQHandler(void)
 {
 
 	if((TIM4->SR & 0x0001)) // est-ce un overflow ?(dépassement)
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
// ADC prescaler à 2 : ADC à 20MHz
// mode continuous mode sur channel 12
// end of conversion interrupt validée
// temps de conversion 239.5+12.5 cycles = 252 cycles = 12,5 us (de tête)
// data alignée à droite
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
 
/*-------------------------------------------config usart with DMA---------------------------------------------------------*/ 
#ifdef USE_STAR_USART 

char DATA_EMIS[100];
char DATA_RECU;
int ok=1;
int rok=0;
short int vitesse;

  void setup_usart()							//ATTENTION CONFIG  PIN RX AS FLOTING INPUT AND TX AS ALTERNATE FUNCTION
 {
  RCC->AHBENR |=0x00000001;// DMA1 CLOCK ENABLE
  RCC->APB1ENR|=0x00040000;// USART 3 CLOCK ENABLE
  TX_DMA_CHANNEL->CPAR =(u32)&(USART3->DR); // ADRESSE  USART_DR
  TX_DMA_CHANNEL->CMAR =(u32)(&DATA_EMIS[0]);   //ADRESSE DATA_EMIS
  TX_DMA_CHANNEL->CNDTR=10;	// transeiver 10 byte  by default
  TX_DMA_CHANNEL->CCR|=0x00000100;	// PRIORITY LEVEL MEDIUM
  TX_DMA_CHANNEL->CCR|=0x00000010;   //READ FROM MEMORY
  TX_DMA_CHANNEL->CCR|=0x00000080;   //	POINTER INCREMENTALE MEMORY 
  
  //TX_DMA_CHANNEL->CCR|=0x00000020;	//CIRCULAR MODE

  TX_DMA_CHANNEL->CCR|=0x00000002; // tc enable interrupt
  TX_DMA_CHANNEL->CCR|=0x00000001;  //ENABLE CHANNEL	 2	 
 
  RX_DMA_CHANNEL->CPAR =	(u32)&(USART3->DR); // ADRESSE  USART_DR
  RX_DMA_CHANNEL->CMAR =(u32)&DATA_RECU;   //ADRESSE DATA_RECU
  RX_DMA_CHANNEL->CNDTR=1;	// receiver 1 byte	by default
  RX_DMA_CHANNEL->CCR|=0x00000100;	// PRIORITY LEVEL MEDIUM
  RX_DMA_CHANNEL->CCR&=0xFFFFFFEF;   //READ FROM PERIPHERAL
  RX_DMA_CHANNEL->CCR|=0x00000080;   //	POINTER INCREMENTALE MEMORY 
  RX_DMA_CHANNEL->CCR|=0x00000020;	//CIRCULAR MODE
  
  RX_DMA_CHANNEL->CCR|=0x00000002;   //tc enable interrupt
  RX_DMA_CHANNEL->CCR|=0x00000001;  //ENABLE CHANNEL	 3	  and tc enable    
  NVIC->ISER[0] = 0x00003000;
  

  USART3->CR1 |=0x00002000;	 // UE ENABLE
  USART3->CR1 &=0xFFFFEFFF;	// M=0 , DATA 8BITS
  USART3->CR2 &=0xFFFFCFFF;	 // 1 STOP BIT
  USART3->CR1 |=0x0000000C;	  // ENABLE RECEIVER AND TRANSEIVER TE,RE
  USART3->CR3 |=0x000000C0;// DMAT AND DMAR ENABLE
  USART3->BRR  =0x00000823;	// BAUDE RATE 9600 ,DIV_FRA =0x6 ,DIV_MANT=0xEA
  USART3->CR3 |=0x00000700;	// CTS ENABLE ,RTS ENABLE ,CTSIE
  //USART3->CR1 |=0x00000080;	// TXE ENABLE INTERRUPT  
  //NVIC->ISER[1] = 0x0000080;
 }

 struct __FILE 
  {
  int handle;                 
  };
FILE __stdout;
FILE __stdin;

/*---------------------------------------------fputc---------------------------------------------------------*/
int fputc(int ch, FILE *f) {
static int compteur=0;

char data[100]; 
  if(ch!='*')
    {
  	  data[compteur]=(char)ch;
      compteur++; 
           }
		   
		   else 
   			   { while(!ok)
			        {
					}
			      
				TX_DMA_CHANNEL->CCR &=0xfffffffe;    //disABLE CHANNEL	 2	 
				TX_DMA_CHANNEL->CNDTR=compteur;	   // transeiver X byte
			    strncpy(DATA_EMIS,data,compteur);
			    compteur=0 ;
				ok=0;
		    	TX_DMA_CHANNEL->CCR |=0x00000001;  
			   }

	 

	  
      return (ch);
 
 
 }
 
 /*--------------------------------------------end fputc---------------------------------------------------------*/

/*---------------------------------------------fgetc---------------------------------------------------------*/
int fgetc(FILE *f) {
  int ch;
        while(!rok)
	        {
		    }
   ch =DATA_RECU;
   rok=0;
  return (ch);
}
/*------------------------------------------end fgetc---------------------------------------------------------*/ 

 
 
 
 
 
void DMAChannel2_IRQHandler (void)
{	if(DMA1->ISR & 0x00000020)
          {
		  ok=1;
	       DMA1->IFCR |=0x0000000F0;
		  }
 

} 

void DMAChannel3_IRQHandler (void)
{	if(DMA1->ISR & 0x00000200)
          {
		  rok=1;
	       DMA1->IFCR |=0x000000F00;
		  }
 

}


 #endif
//------------------------------------------------------------------------

//-------------------------------------------------------------------------
 
/*-------------------------------------------end config usart with DMA---------------------------------------------------------*/ 
 
 

#ifdef USE_DMA_USART
#define ALTERNATE_PUSH_PULL_2MHZ  ((1<<3)|(1<<1)) 
#define FLOATING_INPUT  		  (1<<2)

#if ( USART_NUM == 1 )
	#define USARTx USART1
	#define TX_DMA_CHANNEL DMA1_Channel4 
	#define TX_DMA_HANDLER DMAChannel4_IRQHandler
	#define DMA_CHANNEL_NVIC_POS 14
	#define DMAx_ISR_MASK (1<<13)
	#define DMAx_RESET_MASK (0xF<<(4*3))
	#define RX_DMA_CHANNEL DMA1_Channel5 
	#define SET_APBxENR RCC->APB2ENR|=(1<<14)
	//USART 1 has TX/RX on PA9/10 or PB6/7
	#ifdef  PA_9
	#warning "PA_9 is already configured : reconfiguring anyway"
	#endif
	#ifdef  PA_10
	#warning "PA_10 is already configured : reconfiguring anyway"
	#endif
	#define PA_9
	#define PA_10
	#define GPIOx_CRx GPIOA->CRH
	#define TX_PIN 9
	#define RX_PIN 10
	#define SHIFT_FOR_CRH_L 8

#endif
#if ( USART_NUM == 2 )
	#define USARTx USART2
	#define TX_DMA_CHANNEL DMA1_Channel7 
	#define TX_DMA_HANDLER DMAChannel7_IRQHandler
	#define DMA_CHANNEL_NVIC_POS 17
	#define DMAx_ISR_MASK (1<<25)
	#define DMAx_RESET_MASK (0xF<<(4*6))
	#define RX_DMA_CHANNEL DMA1_Channel6 
	#define SET_APBxENR RCC->APB1ENR|=(1<<17)
	//USART 2 has TX/RX on PA2/3 or PD5/6
	#ifdef  PA_2
	#warning "PA_9 is already configured : reconfiguring anyway"
	#endif
	#ifdef  PA_3
	#warning "PA_10 is already configured : reconfiguring anyway"
	#endif
	#define PA_2
	#define PA_3
	#define GPIOx_CRx GPIOA->CRL
	#define TX_PIN 2
	#define RX_PIN 3
	#define SHIFT_FOR_CRH_L 0
#endif
#if ( USART_NUM == 3 )
	#define USARTx USART3
	#define TX_DMA_CHANNEL DMA1_Channel2 
	#define TX_DMA_HANDLER DMAChannel2_IRQHandler
	#define DMA_CHANNEL_NVIC_POS 12
	#define DMAx_ISR_MASK (1<<5)
	#define DMAx_RESET_MASK (0xF<<(4*1))
	#define RX_DMA_CHANNEL DMA1_Channel3 
	#define SET_APBxENR RCC->APB1ENR|=(1<<18)
	//USART 3 has TX/RX on PB10/11 or PC10/11 or PD8/9
	#ifdef  PB_10
	#warning "PA_9 is already configured : reconfiguring anyway"
	#endif
	#ifdef  PB_11
	#warning "PA_10 is already configured : reconfiguring anyway"
	#endif
	#define PB_10
	#define PB_11
	#define GPIOx_CRx GPIOB->CRH
	#define TX_PIN 10
	#define RX_PIN 11
	#define SHIFT_FOR_CRH_L 8

#endif


char in_buff[IN_BUFF_SIZE];
#define END_IN_BUFF  (&(in_buff[IN_BUFF_SIZE-1]))
char * in_ptr = in_buff;

void flush_serial_input(void)
{ 
	int i;
	for (i=0;i<IN_BUFF_SIZE;i++) in_buff[i] = 0;
};

 void setup_usart()							//ATTENTION CONFIG  PIN RX AS FLOTING INPUT AND TX AS ALTERNATE FUNCTION
 {
 
  RCC->AHBENR |=0x00000001;// DMA1 CLOCK ENABLE
  SET_APBxENR ;
  //RCC->APB1ENR|=0x00040000;// USART 3 CLOCK ENABLE
  TX_DMA_CHANNEL->CPAR =(u32)&(USARTx->DR); // ADRESSE  USART_DR
  // TX_DMA_CHANNEL->CMAR =(u32)(&DATA_EMIS[0]);  
  TX_DMA_CHANNEL->CNDTR=1;	
  TX_DMA_CHANNEL->CCR|=0x00000100;	// PRIORITY LEVEL MEDIUM
  TX_DMA_CHANNEL->CCR|=0x00000010;   //READ FROM MEMORY
  TX_DMA_CHANNEL->CCR|=0x00000080;   //	POINTER INCREMENTALE MEMORY 
  
  //TX_DMA_CHANNEL->CCR|=0x00000020;	//CIRCULAR MODE

  //TX_DMA_CHANNEL->CCR|=0x00000001;  //ENABLE CHANNEL	 2	 
 

  RX_DMA_CHANNEL->CPAR =	(u32)&(USARTx->DR); // ADRESSE  USART_DR
  RX_DMA_CHANNEL->CMAR =(u32)in_buff;   //ADRESSE DATA_RECU
  RX_DMA_CHANNEL->CNDTR=IN_BUFF_SIZE;
  RX_DMA_CHANNEL->CCR|=0x00000100;	// PRIORITY LEVEL MEDIUM
  RX_DMA_CHANNEL->CCR&=0xFFFFFFEF;   //READ FROM PERIPHERAL
  RX_DMA_CHANNEL->CCR|=0x00000080;   //	POINTER INCREMENTALE MEMORY 
  RX_DMA_CHANNEL->CCR|=0x00000020;	//CIRCULAR MODE
  
  //NVIC->ISER[0] = 0x00003000;
  NVIC->ISER[0] = 1<<DMA_CHANNEL_NVIC_POS;
  

  USARTx->CR1 |=0x00002000;	 // UE ENABLE
  USARTx->CR1 &=0xFFFFEFFF;	// M=0 , DATA 8BITS
  USARTx->CR2 &=0xFFFFCFFF;	 // 1 STOP BIT
  USARTx->CR1 |=0x0000000C;	  // ENABLE RECEIVER AND TRANSEIVER TE,RE
  USARTx->CR3 |=0x000000C0;// DMAT AND DMAR ENABLE
  USARTx->BRR  =0x00000823;	// BAUDE RATE 9600 ,DIV_FRA =0x6 ,DIV_MANT=0xEA
  USARTx->CR3 |=0x00000700;	// CTS ENABLE ,RTS ENABLE ,CTSIE
  //USART3->CR1 |=0x00000080;	// TXE ENABLE INTERRUPT  
  //NVIC->ISER[1] = 0x0000080;

  // Config TX/RX pins
  GPIOx_CRx &= ~(0xF                      << ((TX_PIN-SHIFT_FOR_CRH_L)*4));
  GPIOx_CRx |=  (ALTERNATE_PUSH_PULL_2MHZ << ((TX_PIN-SHIFT_FOR_CRH_L)*4)) ;
  GPIOx_CRx &= ~(0xF                      << ((RX_PIN-SHIFT_FOR_CRH_L)*4));
  GPIOx_CRx |=  (FLOATING_INPUT           << ((RX_PIN-SHIFT_FOR_CRH_L)*4)) ;

   TX_DMA_CHANNEL->CCR|=0x00000002; // tc enable interrupt

   flush_serial_input();
   	   
   //RX_DMA_CHANNEL->CCR|=0x00000002;   //tc enable interrupt
   RX_DMA_CHANNEL->CCR|=0x00000001;  //ENABLE CHANNEL	 3	  and tc enable    
 
 }


char send_buff[SEND_BUFF_SIZE];
#define END_SEND_BUFF  (&(send_buff[SEND_BUFF_SIZE-1]))
 
 char *send_ptr=send_buff;
 char *pdma;
 char send_buffer_empty=1;
 short int dma_size;


int fputc(int ch, FILE *f) 
 { 
   if (!send_buffer_empty)
 	{
		while (send_ptr == (char *) (TX_DMA_CHANNEL->CMAR)) ;
 
 		*send_ptr=ch;
   
   		if(send_ptr>=END_SEND_BUFF)
      	{
	  		send_ptr=send_buff;
	  	}
	  	else  
	  		send_ptr++;
   	 }
	 else
	 {
	 	 *send_ptr=ch;
   
 		// send single char to DMA
		TX_DMA_CHANNEL->CCR &=0xfffffffe;    //disABLE CHANNEL	 2	 
		TX_DMA_CHANNEL->CNDTR=1;
		dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
		TX_DMA_CHANNEL->CMAR=(unsigned int) send_ptr;
	    send_buffer_empty=0;

   		if(send_ptr>=END_SEND_BUFF)
      	{
	  		send_ptr=send_buff;
	  	}
	  	else  
	  		send_ptr++;

		TX_DMA_CHANNEL->CCR |=0x00000001;  


	 }
   
   return ch;
  }


 void TX_DMA_HANDLER (void)
{	
	char * pDMA ;
   	  

	if ( (DMA1->ISR & DMAx_ISR_MASK) && !send_buffer_empty ) 	  //end of conversion interrupt
    {

			pDMA = (char *) (TX_DMA_CHANNEL->CMAR + dma_size);
   	
			if(pDMA>END_SEND_BUFF)
    	  	{
		  		pDMA=send_buff;
		  	}
	
		   if ( pDMA == send_ptr) 
		   {
		   	send_buffer_empty = 1;
		   }
		   else
		   {
		   		if (send_ptr>pDMA)
				{
				    // DMA up to send_ptr
					TX_DMA_CHANNEL->CCR &=0xfffffffe;    //disABLE CHANNEL	 2	 
					TX_DMA_CHANNEL->CNDTR=(send_ptr-pDMA);
					dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
					TX_DMA_CHANNEL->CMAR= (unsigned int) pDMA;
					TX_DMA_CHANNEL->CCR |=0x00000001;  
				}
				else
				{	// DMA up to end of buffer
					TX_DMA_CHANNEL->CCR &=0xfffffffe;    //disABLE CHANNEL	 2	 
					TX_DMA_CHANNEL->CNDTR=(END_SEND_BUFF-pDMA+1);
					dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
					TX_DMA_CHANNEL->CMAR=(unsigned int) pDMA;
					TX_DMA_CHANNEL->CCR |=0x00000001;  
				
				}
			}
		   		
		   		
	}
	DMA1->IFCR |= (DMAx_RESET_MASK); //reset interrupt flag
}



/*---------------------------------------------fgetc---------------------------------------------------------*/
int fgetc(FILE *f) {
  int ch;
  
  while(*in_ptr == 0) ;
  
  ch = *in_ptr;
  
  *(in_ptr++) = 0;
  if (in_ptr > END_IN_BUFF) in_ptr=in_buff ;
  	
  return (ch);
}


/*------------------------------------------end fgetc---------------------------------------------------------*/ 
#endif
 


void Init_Periphs()			  
{
 //Setup_Pll_As_Clock_System();
 Init_PortA();
 Init_PortB();
 Init_PortC();

 #ifdef USE_STAR_USART
 setup_usart();
 #endif
 #ifdef USE_DMA_USART
 setup_usart();
 #endif

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
