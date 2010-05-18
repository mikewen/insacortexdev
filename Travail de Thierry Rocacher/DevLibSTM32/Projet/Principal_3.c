#include "../../../Bibli_INSA_STM32/Lib_STM32_TR_V2_1/stm_regs.h"  
#include "../../../Bibli_INSA_STM32/Lib_STM32_TR_V2_1/Port_IO.h" 
#include "../../../Bibli_INSA_STM32/Lib_STM32_TR_V2_1/NVIC_SysTick.h" 
#include "../../../Bibli_INSA_STM32/Lib_STM32_TR_V2_1/PWM.h"

// variation intensité LED.
// 2 secondes pour monter, 2 secondes pour descendre
// On monte en 100 intensités réparties. Donc chaque éclairage
// dure 2/100 = 20ms. Ce sera la période du systick

unsigned int Puissance;
char UpDown;

void SysTick_Handler (void)
{
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
}

//=======================================================================================
int main(void)
{


Puissance=0;
UpDown=0;
PWM_Init(4, 4, 20.0, 8000.0 );
Port_IO_Init('B', 9, 'o', 2);

Port_IO_Init('B', 8, 'o', 0);
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

