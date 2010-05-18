
//=======================================================================================
 // 	Auteur T.R.
//  12 Mai 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure le timer système Systick
//=======================================================================================

// Exemple de code :
// Gestion du Systick
/*
Systick_Prio_IT(2);
Systick_Period(8,500000); // 0.5 seconde
SysTick_CkSource_ProcessorClk;
SysTick_Enable_IT;
SysTick_On;
*/

#ifndef _NVIC_SysTick_H__
#define _NVIC_SysTick_H__

//========================================================================================
// Validation des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Enable_IT(char NumeroIT);


//========================================================================================
// Invalidation des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Disable_IT(char NumeroIT);

//========================================================================================
// Fixer la priorité des lignes d'interruption (hors coeur), 0 à 63
//=======================================================================================

void NVIC_Prio_IT(char NumeroIT, char Prio);

//========================================================================================
// Gestion du Timer SysTick
//=======================================================================================

#define  SysTick_On ((SysTick->CTRL)=(SysTick->CTRL)|1<<0)
#define  SysTick_Off ((SysTick->CTRL)=(SysTick->CTRL)& ~(1<<0))
#define  SysTick_Enable_IT ((SysTick->CTRL)=(SysTick->CTRL)|1<<1)
#define  SysTick_Disable_IT ((SysTick->CTRL)=(SysTick->CTRL)& ~(1<<1))
#define  SysTick_CkSource_AHB_Div8 	((SysTick->CTRL)=(SysTick->CTRL)& ~(1<<2))
#define  SysTick_CkSource_ProcessorClk 	((SysTick->CTRL)=(SysTick->CTRL)|1<<2)

//========================================================================================
// Fixer la priorité de l'IT coeur Systick, plus elle est basse, plus la prio est importante
//=======================================================================================

void Systick_Prio_IT(char Prio);

//========================================================================================
// Fixer la periode du Systick et revoie de la valeur effective
//=======================================================================================

float Systick_Period(float Freq_In_MHz, float Duree_us);


#endif
