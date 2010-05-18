#include "../Sys/stm_regs.h" 

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
 



unsigned int PWM_Init(char Timer, char Voie, float Frequence_PWM_Khz, float Frequence_Ck_Timer_Khz )
//Renvoie la résolution optimale

{
char Erreur;
vu16 TIMx_CR1;	 // Control reg
vu16 TIMx_CCMR1;
vu16 TIMx_CCMR2;
vu16 TIMx_CCER;	 // TIMx capture/compare Enable reg
vu16 TIMx_PSC;
vu16 TIMx_ARR;
vu16 TIMx_CNT;

float N_Reload, N_prescaler;
vu16 N_prescaler_vu16;
unsigned int Resolution;



// Chargement des variables en fonction du numéro de Timer :
switch(Timer)
{
case 2 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIMx_CR1=TIM2->CR1;	 
	 TIMx_CCMR1=TIM2->CCMR1;
	 TIMx_CCMR2=TIM2->CCMR2;
	 TIMx_CCER=TIM2->CCER;	 
	 TIMx_PSC=TIM2->PSC;
 	 TIMx_ARR=TIM2->ARR;
	 TIMx_CNT=TIM2->CNT;
	break;
case 3 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIMx_CR1=TIM3->CR1;	 
	 TIMx_CCMR1=TIM3->CCMR1;
	 TIMx_CCMR2=TIM3->CCMR2;
	 TIMx_CCER=TIM3->CCER;	 
	 TIMx_PSC=TIM3->PSC;
 	 TIMx_ARR=TIM3->ARR;
	 TIMx_CNT=TIM3->CNT;
	break;
case 4 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIMx_CR1=TIM4->CR1;	 
	 TIMx_CCMR1=TIM4->CCMR1;
	 TIMx_CCMR2=TIM4->CCMR2;
	 TIMx_CCER=TIM4->CCER;	 
	 TIMx_PSC=TIM4->PSC;
 	 TIMx_ARR=TIM4->ARR;
	 TIMx_CNT=TIM4->CNT;
	break;
default  : 	Erreur = 1; // champs Timer ne rentre pas ds l'intervalle [2:4]
}

Erreur=0; // A priori pas d'érreur. Rem erreur prévue pour Timer, Voie



//TIMx_CR1=	|xxx... CKD[1:0] ARPE CMS DIR OPM URS UDIS CEN |
// CKD=00 (pour le filtrage de ETR mode compteur, on s'en moque )
// ARPE = 0 (autoreload direct en comparaison, pas de shadow reg
// CMS = 00 (mode edge aligned)
// OPM = 0 (compteur continue à compter même si update event)
// DIR = 0 (comptage up)
// URS (Update request source) = 0 car de toute manière, pas d'IT
// UDIS (Update disable) = 1 car pas d'update sur event
// CEN=1 Counter enable
TIMx_CR1=0x03;


//TIMx_CCMR1  = OC2CE OC2M[2:0] OC2PE OC2FE	CC2S[1:0] OC1CE OC1M[2:0] OC1PE OC1FE CC1S[1:0]
// les numéro 1 et 2 précisent les numéros de sortie (1 ou 2)
// IDEM pour TIMx_CCMR2
// CC1S = 00 : la broche OC1 du Timer x est en sortie, 
// OC1M[2:0] = 110 OC1 est drivé PWM edge aligned
// NB les autres bits sont à 0 car ils servent à rendre l'action effective sur event et on s'en  moque

if (Voie==1)
	{
	TIMx_CCMR1=TIMx_CCMR1&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F : mise à 0 du champ
	TIMx_CCMR1=TIMx_CCMR1|0x06<<4;
	}	
else if	(Voie==2)
	{
	TIMx_CCMR1=TIMx_CCMR1&~(0x07<<12);
	TIMx_CCMR1=TIMx_CCMR1|0x06<<12;
	}
else if (Voie==3)
	{
	TIMx_CCMR2=TIMx_CCMR2&~(0x07<<4); // masque = ~0000 0000 0111 0000=FF8F 
	TIMx_CCMR2=TIMx_CCMR2|0x06<<4;
	}	
else if	(Voie==4)
	{
	TIMx_CCMR2=TIMx_CCMR2&~(0x07<<12);
	TIMx_CCMR2=TIMx_CCMR2|0x06<<12;
	}
else
	{
	Erreur = 1; // champs Voie ne rentre pas ds l'intervalle [1:4]
	}

//TIMx_CCER	 TIMx capture/compare Enable reg
// TIMx_CCER= Reserved CC4P CC4E Reserved CC3P CC3E Reserved CC2P CC2E Reserved	CC1P CC1E
// CC1E = 1 : la sortie est active
// CC1P = 0 : logique positive
switch(Voie)
{
case 1 :
	TIMx_CCER = TIMx_CCER &~(0x03);
	TIMx_CCER = TIMx_CCER |(0x01);
	break;
case 2 :
	TIMx_CCER = TIMx_CCER &~(0x03<<4);
	TIMx_CCER = TIMx_CCER |(0x01<<4);
	break;
case 3 :
	TIMx_CCER = TIMx_CCER &~(0x03<<8);
	TIMx_CCER = TIMx_CCER |(0x01<<8);
	break;
case 4 :
	TIMx_CCER = TIMx_CCER &~(0x03<<12);
	TIMx_CCER = TIMx_CCER |(0x01<<12);
	break;
default  : 	Erreur = 1; // champs Voie ne rentre pas ds l'intervalle [1:4]

}

// Paramétrage du prescaler 16bit et du Reload 16 bits
//
// Periode totale = N_prescaler*N_Reload*T_ck

N_Reload=Frequence_Ck_Timer_Khz/Frequence_PWM_Khz;
if (N_Reload<65535.0)  // Tient sur 16 bits
	{ 
	// Pas de prescaler
	TIMx_ARR=(vu16)N_Reload;
	TIMx_PSC=0;
	}
else 	  // Tient sur 32 bits
	{
	//N_Reload tient sur 32 bits
	N_prescaler=Frequence_Ck_Timer_Khz/(65536.0*Frequence_PWM_Khz)+1.0;
	N_prescaler_vu16=(vu16)N_prescaler;
	N_Reload=Frequence_Ck_Timer_Khz/(Frequence_PWM_Khz*(float)N_prescaler_vu16);
	// Affectation
	TIMx_PSC=N_prescaler_vu16-1;
	TIMx_ARR=(vu16)N_Reload;

	}


if (Erreur==1)
	{
	N_Reload=0.0;
	TIMx_ARR=0;
	TIMx_PSC=0;
	}


// Affectation des registres + Activer horloge Timer 
switch(Timer)
{
case 2 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIM2->CR1=TIMx_CR1; 
	 TIM2->CCMR1=TIMx_CCMR1;
	 TIM2->CCMR2=TIMx_CCMR2;
	 TIM2->CCER=TIMx_CCER;	 
	 TIM2->PSC=TIMx_PSC;
 	 TIM2->ARR=TIMx_ARR;
	 TIM2->CNT=TIMx_CNT;
	 (RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM2EN;
	break;
case 3 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIM2->CR1=TIMx_CR1; 
	 TIM3->CCMR1=TIMx_CCMR1;
	 TIM3->CCMR2=TIMx_CCMR2;
	 TIM3->CCER=TIMx_CCER;	 
	 TIM3->PSC=TIMx_PSC;
 	 TIM3->ARR=TIMx_ARR;
	 TIM3->CNT=TIMx_CNT;
	 (RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM3EN;
	break;
case 4 :
// TIMx->CNT etc, x allant de 2 à 4.	
	 TIM4->CR1=TIMx_CR1; 
	 TIM4->CCMR1=TIMx_CCMR1;
	 TIM4->CCMR2=TIMx_CCMR2;
	 TIM4->CCER=TIMx_CCER;	 
	 TIM4->PSC=TIMx_PSC;
 	 TIM4->ARR=TIMx_ARR;
	 TIM4->CNT=TIMx_CNT;
	 (RCC->APB1ENR)=(RCC->APB1ENR) | RCC_TIM4EN;
	break;
}




Resolution=(vu16)N_Reload;
return Resolution;


}


