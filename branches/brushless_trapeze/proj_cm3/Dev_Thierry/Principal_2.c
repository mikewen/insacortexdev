#include "stm_regs.h"  
#include "Port_IO.h" 
#include "NVIC_SysTick.h" 
#include "PWM.h"

void SysTick_Handler (void)
{

if (Port_IO_Lire('B', 8)==0)
	{
	Port_IO_Set('B', 8); 
	Port_IO_Set('B', 9);
	}
else
	{
	Port_IO_Clr('B', 8);
	Port_IO_Clr('B', 9);
	}
}

//=======================================================================================
int main(void)
{
unsigned int Reso;
// Configuration des ports de sortie (RB8, RB9).
// Les autres lignes en entrée pull down
Port_IO_Init('B', 8, 'o', 0);
Port_IO_Init('B', 9, 'o', 0);

Port_IO_Init('B', 10, 'i', 2);
Port_IO_Clr('B', 10);
Port_IO_Init('B', 11, 'i', 2);
Port_IO_Clr('B', 11);
Port_IO_Init('B', 12, 'i', 2);
Port_IO_Clr('B', 12);
Port_IO_Init('B', 13, 'i', 2);
Port_IO_Clr('B', 13);
Port_IO_Init('B', 14, 'i', 2);
Port_IO_Clr('B', 14);
Port_IO_Init('B', 15, 'i', 2);
Port_IO_Clr('B', 15);



// Gestion du Systick
Systick_Prio_IT(2);
Systick_Period(8.0,500000.0); // 0.5 seconde
SysTick_CkSource_ProcessorClk;
SysTick_Enable_IT;
SysTick_On;

//unsigned int PWM_Init(char Timer, char Voie, float Frequence_PWM_Khz, float Frequence_Ck_Timer_Khz );
Reso= PWM_Init(2, 4, 20.0, 8000.0 );
Port_IO_Init('A', 3, 'o', 2);
PWM_Valeur_Timer_2_Voie_4=Reso/4;

while(1)
{

}
	
}

