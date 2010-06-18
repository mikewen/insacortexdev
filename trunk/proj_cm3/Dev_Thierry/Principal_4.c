#include "stm_regs.h"  
#include "Port_IO.h" 
#include "NVIC_SysTick.h" 
#include "PWM.h"
#include "ADC.h"

// variation intensité LED.
// 2 secondes pour monter, 2 secondes pour descendre
// On monte en 100 intensités réparties. Donc chaque éclairage
// dure 2/100 = 20ms. Ce sera la période du systick

vu16 Puissance;
char UpDown;

void SysTick_Handler (void)
{
vu16 ValeurADC;
vu16  Valeur_PWM;

// "LED qui ronfle"
PWM_Valeur_Timer_4_Voie_4=Puissance;
if (UpDown==0)
	{
	Puissance=Puissance+4;
	if (Puissance==400) 
		{
		UpDown=1;
		}
	}
else
   {
	Puissance=Puissance-4;
	if (Puissance==0) 
		{
		UpDown=0;
		}
	}
// Renvoie ADC vers LED
ValeurADC=ADC_Lire(0);
Valeur_PWM=(vu16)(400*ValeurADC/4096);
PWM_Valeur_Timer_4_Voie_3=Valeur_PWM;
}

//=======================================================================================
int main(void)
{
char Sequencement_ADC[6];  // table à remplir avant l'init de l'ADC. Doit contenir
						   // les ordres de scan (0 à 15) 0 en premier, 15 en dernier



// initialise voie 1 en ADC	(PA.1)
Sequencement_ADC[0]=1;
ADC_Init(1, Sequencement_ADC, 5.0, 4000.0);
Port_IO_Init('A', 1, 'i', 0); // In analogique

Puissance=0;
UpDown=0;
// init de la PWM avec timer4, voie 4 (PB9)
PWM_Init(4, 4, 20.0, 8000.0 );
Port_IO_Init('B', 9, 'o', 2);
// init de la PWM avec timer4, voie 3 (PB9)
PWM_Init(4, 3, 20.0, 8000.0 );
Port_IO_Init('B', 8, 'o', 2);
// Config en entrée pull up / down
Port_IO_Init('B', 10, 'i', 2);
Port_IO_Init('B', 11, 'i', 2);
Port_IO_Init('B', 12, 'i', 2);
Port_IO_Init('B', 13, 'i', 2);
Port_IO_Init('B', 14, 'i', 2);
Port_IO_Init('B', 15, 'i', 2);



// Gestion du Systick
Systick_Prio_IT(2);
Systick_Period(8.0,20000.0); // 20ms seconde
SysTick_CkSource_ProcessorClk;
SysTick_Enable_IT;
SysTick_On;


while(1)
{

}
	
}

