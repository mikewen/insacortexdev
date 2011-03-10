#include "stm_regs.h"
#include "stm_timx_config.h"
#include "stm_clock_config.h"
#include "stm_system.h" 

//=======================================================================================
// 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure les PWM possibles sur le uC 
//  Voir PWM.h pour plus d'info
//=======================================================================================


//========================================================================================
// Config des Timerx (2,3,4) General Purpose
//=======================================================================================

//Les registres importants :
// Counter Register (TIMx_CNT)
// Prescaler Register (TIMx_PSC)
// Auto-Reload Register (TIMx_ARR)
// TIMx_CCRx Registre utilisé pour régler la durée	(Compare Capture register)
//
// Organisation physique STM32F103RB
// 64 broches
//  	TM2_CH1_ETR - PA0 	TM3_CH1 - PA6		TIM4_CH1 - PB6	
//    	TM2_CH2 - PA1		TM3_CH2 - PA7		TIM4_CH2 - PB7
// 		TM2_CH3 - PA1		TM3_CH3 - PA8		TIM4_CH3 - PB8
// 		TM2_CH4 - PA3		TM3_CH4 - PA9		TIM4_CH4 - PB9
 



void TIMx_Init( void )
{

#ifdef TIMx_IS_USED

//__________________TIM2____________________
#ifdef TIM2_IS_USED

// validation de l'horloge du periph 
(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM2EN;
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;
//TIMx_CR1=	|xxx... CKD[1:0] ARPE CMS DIR OPM URS UDIS CEN |
// CKD=00 (pour le filtrage de ETR mode compteur, on s'en moque )
// ARPE = 0 (autoreload direct en comparaison, pas de shadow reg
// CMS = 00 (mode edge aligned)
// OPM = 0 (compteur continue à compter même si update event)
// DIR = 0 (comptage up)
// URS (Update request source) = 0 car de toute manière, pas d'IT
// UDIS (Update disable) = 1 car pas d'update sur event
// CEN=1 Counter enable
TIM2->CR1=(TIM_CEN);

//TIMx_CCMR1  = OC2CE OC2M[2:0] OC2PE OC2FE	CC2S[1:0] OC1CE OC1M[2:0] OC1PE OC1FE CC1S[1:0]
// les numéro 1 et 2 précisent les numéros de sortie (1 ou 2)
// IDEM pour TIMx_CCMR2
// CC1S = 00 : la broche OC1 du Timer x est en sortie, 
// OC1M[2:0] = 110 OC1 est drivé PWM edge aligned
// NB les autres bits sont à 0 car ils servent à rendre l'action effective sur event et on s'en  moque
//TIMx_CCER	 TIMx capture/compare Enable reg
// TIMx_CCER= Reserved CC4P CC4E Reserved CC3P CC3E Reserved CC2P CC2E Reserved	CC1P CC1E
// CC1E = 1 : la sortie est active
// CC1P = 0 : logique positive

	// Affectation
#if ((__TIMXCLK/TIM2_FREQ)<65536)
	#define TIM2_PSC 1
	#define TIM2_ARR (__TIMXCLK/TIM2_FREQ)-1
#else
	#define TIM2_PSC (__TIMXCLK/TIM2_FREQ/65536+1)
	#define TIM2_ARR ((__TIMXCLK/(TIM2_PSC))/TIM2_FREQ)-1
#endif	
	TIM2->PSC=(u16) TIM2_PSC - 1;
	TIM2->ARR=(u16) TIM2_ARR;

#ifdef TIM2_CHANNEL_1_IS_PWM 
	TIM2->CCMR1=TIM2->CCMR1&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F : mise à 0 du champ
	TIM2->CCMR1=TIM2->CCMR1|0x06<<4;
	TIM2->CCER = TIM2->CCER &~(0x03);
	TIM2->CCER = TIM2->CCER |(0x01);
	TIM2->CCR1 = (u16) (((float)TIM2_ARR)*((float)TIM2_CHANNEL_1_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(0*4));
	GPIOA->CRL |= (0xB<<(0*4)); // Alternate push pull PA0
#endif
#ifdef TIM2_CHANNEL_2_IS_PWM 
	TIM2->CCMR1=TIM2->CCMR1&~(0x07<<12);
	TIM2->CCMR1=TIM2->CCMR1|0x06<<12;
	TIM2->CCER = TIM2->CCER &~(0x03<<4);
	TIM2->CCER = TIM2->CCER |(0x01<<4);
	TIM2->CCR2 = (u16) (((float)TIM2_ARR)*((float)TIM2_CHANNEL_2_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(1*4));
	GPIOA->CRL |= (0xB<<(1*4)); // Alternate push pull PA1
#endif
#ifdef TIM2_CHANNEL_3_IS_PWM 
	TIM2->CCMR2=TIM2->CCMR2&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F 
	TIM2->CCMR2=TIM2->CCMR2|0x06<<4;
	TIM2->CCER = TIM2->CCER &~(0x03<<8);
	TIM2->CCER = TIM2->CCER |(0x01<<8);
	TIM2->CCR3 = (u16) (((float)TIM2_ARR)*((float)TIM2_CHANNEL_3_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(2*4));
	GPIOA->CRL |= (0xB<<(2*4)); // Alternate push pull PA2
#endif
#ifdef TIM2_CHANNEL_4_IS_PWM 
	TIM2->CCMR2= TIM2->CCMR2&~(0x07<<12);
	TIM2->CCMR2= TIM2->CCMR2|0x06<<12;
	TIM2->CCER = TIM2->CCER &~(0x03<<12);
	TIM2->CCER = TIM2->CCER |(0x01<<12);
	TIM2->CCR4 = (u16) (((float)TIM2_ARR)*((float)TIM2_CHANNEL_4_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(3*4));
	GPIOA->CRL |= (0xB<<(3*4)); // Alternate push pull PA3
#endif

#ifdef TIM2_IT_IS_USED
	 NVIC_ENABLE_PERIPH_IT(TIM2);
	 NVIC_SET_PRIO_PERIPH(TIM2,TIM2_PRIO);
	 TIM2->DIER |= TIM_UIE;
#endif


#endif

//__________________TIM3____________________
#ifdef TIM3_IS_USED
// validation de l'horloge du periph 
(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM3EN;
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;
//TIMx_CR1=	|xxx... CKD[1:0] ARPE CMS DIR OPM URS UDIS CEN |
// CKD=00 (pour le filtrage de ETR mode compteur, on s'en moque )
// ARPE = 0 (autoreload direct en comparaison, pas de shadow reg
// CMS = 00 (mode edge aligned)
// OPM = 0 (compteur continue à compter même si update event)
// DIR = 0 (comptage up)
// URS (Update request source) = 0 car de toute manière, pas d'IT
// UDIS (Update disable) = 1 car pas d'update sur event
// CEN=1 Counter enable
TIM3->CR1=(TIM_CEN);

//TIMx_CCMR1  = OC2CE OC2M[2:0] OC2PE OC2FE	CC2S[1:0] OC1CE OC1M[2:0] OC1PE OC1FE CC1S[1:0]
// les numéro 1 et 2 précisent les numéros de sortie (1 ou 2)
// IDEM pour TIMx_CCMR2
// CC1S = 00 : la broche OC1 du Timer x est en sortie, 
// OC1M[2:0] = 110 OC1 est drivé PWM edge aligned
// NB les autres bits sont à 0 car ils servent à rendre l'action effective sur event et on s'en  moque
//TIMx_CCER	 TIMx capture/compare Enable reg
// TIMx_CCER= Reserved CC4P CC4E Reserved CC3P CC3E Reserved CC2P CC2E Reserved	CC1P CC1E
// CC1E = 1 : la sortie est active
// CC1P = 0 : logique positive

	// Affectation
#if ((__TIMXCLK/TIM3_FREQ)<65536)
	#define TIM3_PSC 1
	#define TIM3_ARR (__TIMXCLK/TIM3_FREQ)-1
#else
	#define TIM3_PSC (__TIMXCLK/TIM3_FREQ/65536+1)
	#define TIM3_ARR ((__TIMXCLK/(TIM3_PSC))/TIM3_FREQ)-1
#endif	
	TIM3->PSC=(u16) TIM3_PSC - 1;
	TIM3->ARR=(u16) TIM3_ARR;

#ifdef TIM3_CHANNEL_1_IS_PWM 
	TIM3->CCMR1=TIM3->CCMR1&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F : mise à 0 du champ
	TIM3->CCMR1=TIM3->CCMR1|0x06<<4;
	TIM3->CCER = TIM3->CCER &~(0x03);
	TIM3->CCER = TIM3->CCER |(0x01);
	TIM3->CCR1 = (u16) (((float)TIM3_ARR)*((float)TIM3_CHANNEL_1_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(6*4));
	GPIOA->CRL |= (0xB<<(6*4)); // Alternate push pull PA6
#endif
#ifdef TIM3_CHANNEL_2_IS_PWM 
	TIM3->CCMR1=TIM3->CCMR1&~(0x07<<12);
	TIM3->CCMR1=TIM3->CCMR1|0x06<<12;
	TIM3->CCER = TIM3->CCER &~(0x03<<4);
	TIM3->CCER = TIM3->CCER |(0x01<<4);
	TIM3->CCR2 = (u16) (((float)TIM3_ARR)*((float)TIM3_CHANNEL_2_DUTY_CYCLE)/100.0);

	GPIOA->CRL &= ~(0xF<<(7*4));
	GPIOA->CRL |= (0xB<<(7*4)); // Alternate push pull PA7
#endif
#ifdef TIM3_CHANNEL_3_IS_PWM 
	TIM3->CCMR2=TIM3->CCMR2&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F 
	TIM3->CCMR2=TIM3->CCMR2|0x06<<4;
	TIM3->CCER = TIM3->CCER &~(0x03<<8);
	TIM3->CCER = TIM3->CCER |(0x01<<8);
	TIM3->CCR3 = (u16) (((float)TIM3_ARR)*((float)TIM3_CHANNEL_3_DUTY_CYCLE)/100.0);

	GPIOB->CRL &= ~(0xF<<(0*4));
	GPIOB->CRL |= (0xB<<(0*4)); // Alternate push pull PB0
#endif
#ifdef TIM3_CHANNEL_4_IS_PWM 
	TIM3->CCMR2= TIM3->CCMR2&~(0x07<<12);
	TIM3->CCMR2= TIM3->CCMR2|0x06<<12;
	TIM3->CCER = TIM3->CCER &~(0x03<<12);
	TIM3->CCER = TIM3->CCER |(0x01<<12);
	TIM3->CCR4 = (u16) (((float)TIM3_ARR)*((float)TIM3_CHANNEL_4_DUTY_CYCLE)/100.0);

	GPIOB->CRL &= ~(0xF<<(1*4));
	GPIOB->CRL |= (0xB<<(1*4)); // Alternate push pull PB1
#endif

#ifdef TIM3_IT_IS_USED
	 NVIC_ENABLE_PERIPH_IT(TIM3)
	 NVIC_SET_PRIO_PERIPH(TIM3,TIM3_PRIO)
	 TIM3->DIER |= TIM_UIE;
#endif

#endif

//__________________TIM4____________________
#ifdef TIM4_IS_USED
// validation de l'horloge du periph 
(RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM4EN;
(RCC->APB2ENR)=(RCC->APB2ENR) | RCC_IOPAEN | RCC_IOPBEN | RCC_IOPCEN;
//TIMx_CR1=	|xxx... CKD[1:0] ARPE CMS DIR OPM URS UDIS CEN |
// CKD=00 (pour le filtrage de ETR mode compteur, on s'en moque )
// ARPE = 0 (autoreload direct en comparaison, pas de shadow reg
// CMS = 00 (mode edge aligned)
// OPM = 0 (compteur continue à compter même si update event)
// DIR = 0 (comptage up)
// URS (Update request source) = 0 car de toute manière, pas d'IT
// UDIS (Update disable) = 1 car pas d'update sur event
// CEN=1 Counter enable
TIM4->CR1=(TIM_CEN);

//TIMx_CCMR1  = OC2CE OC2M[2:0] OC2PE OC2FE	CC2S[1:0] OC1CE OC1M[2:0] OC1PE OC1FE CC1S[1:0]
// les numéro 1 et 2 précisent les numéros de sortie (1 ou 2)
// IDEM pour TIMx_CCMR2
// CC1S = 00 : la broche OC1 du Timer x est en sortie, 
// OC1M[2:0] = 110 OC1 est drivé PWM edge aligned
// NB les autres bits sont à 0 car ils servent à rendre l'action effective sur event et on s'en  moque
//TIMx_CCER	 TIMx capture/compare Enable reg
// TIMx_CCER= Reserved CC4P CC4E Reserved CC3P CC3E Reserved CC2P CC2E Reserved	CC1P CC1E
// CC1E = 1 : la sortie est active
// CC1P = 0 : logique positive

	// Affectation
#if ((__TIMXCLK/TIM4_FREQ)<65536)
	#define TIM4_PSC 1
	#define TIM4_ARR (__TIMXCLK/TIM4_FREQ)-1
#else
	#define TIM4_PSC (__TIMXCLK/TIM4_FREQ/65536+1);
	#define TIM4_ARR ((__TIMXCLK/(TIM4_PSC))/TIM4_FREQ)-1;
#endif	
	TIM4->PSC=(u16) TIM4_PSC - 1;
	TIM4->ARR=(u16) TIM4_ARR;

#ifdef TIM4_CHANNEL_1_IS_PWM 
	TIM4->CCMR1=TIM4->CCMR1&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F : mise à 0 du champ
	TIM4->CCMR1=TIM4->CCMR1|0x06<<4;
	TIM4->CCER = TIM4->CCER &~(0x03);
	TIM4->CCER = TIM4->CCER |(0x01);
	TIM4->CCR1 = (u16) (((float)TIM4_ARR)*((float)TIM4_CHANNEL_1_DUTY_CYCLE)/100.0);

	GPIOB->CRL &= ~(0xF<<(6*4));
	GPIOB->CRL |= (0xB<<(6*4)); // Alternate push pull PB6
#endif
#ifdef TIM4_CHANNEL_2_IS_PWM 
	TIM4->CCMR1=TIM4->CCMR1&~(0x07<<12);
	TIM4->CCMR1=TIM4->CCMR1|0x06<<12;
	TIM4->CCER = TIM4->CCER &~(0x03<<4);
	TIM4->CCER = TIM4->CCER |(0x01<<4);
	TIM4->CCR2 = (u16) (((float)TIM4_ARR)*((float)TIM4_CHANNEL_2_DUTY_CYCLE)/100.0);

	GPIOB->CRL &= ~(0xF<<(7*4));
	GPIOB->CRL |= (0xB<<(7*4)); // Alternate push pull PB7
#endif
#ifdef TIM4_CHANNEL_3_IS_PWM 
	TIM4->CCMR2=TIM4->CCMR2&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F 
	TIM4->CCMR2=TIM4->CCMR2|0x06<<4;
	TIM4->CCER = TIM4->CCER &~(0x03<<8);
	TIM4->CCER = TIM4->CCER |(0x01<<8);
	TIM4->CCR3 = (u16) (((float)TIM4_ARR)*((float)TIM4_CHANNEL_3_DUTY_CYCLE)/100.0);

	GPIOB->CRH &= ~(0xF<<(0*4));
	GPIOB->CRH |= (0xB<<(0*4)); // Alternate push pull PB8
#endif
#ifdef TIM4_CHANNEL_4_IS_PWM 
	TIM4->CCMR2= TIM4->CCMR2&~(0x07<<12);
	TIM4->CCMR2= TIM4->CCMR2|0x06<<12;
	TIM4->CCER = TIM4->CCER &~(0x03<<12);
	TIM4->CCER = TIM4->CCER |(0x01<<12);
	TIM4->CCR4 = (u16) (((float)TIM4_ARR)*((float)TIM4_CHANNEL_4_DUTY_CYCLE)/100.0);

	GPIOB->CRH &= ~(0xF<<(1*4));
	GPIOB->CRH |= (0xB<<(1*4)); // Alternate push pull PB9
#endif

#ifdef TIM4_IT_IS_USED
	 NVIC_ENABLE_PERIPH_IT(TIM4);
	 NVIC_SET_PRIO_PERIPH(TIM4,TIM4_PRIO);
	 TIM4->DIER |= TIM_UIE;
#endif

#endif
#endif
}

#ifdef TIM2_IT_IS_USED
void TIM2_IRQHandler (void)
{
	TIM2_HOOK;
	TIM2->SR &= ~(TIM_UIF);	
}
#endif

#ifdef TIM3_IT_IS_USED
void TIM3_IRQHandler (void)
{
	TIM3_HOOK;
	TIM3->SR &= ~(TIM_UIF);	
}
#endif

#ifdef TIM4_IT_IS_USED
void TIM4_IRQHandler (void)
{
	TIM4_HOOK;
	TIM4->SR &= ~(TIM_UIF);	
}
#endif

