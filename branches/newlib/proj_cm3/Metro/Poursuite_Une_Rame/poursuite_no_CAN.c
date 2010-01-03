// Pierre-Emmanuel Hladik et Pascal Acco
// 12/12/2009
/**********************************************************
Fichier squelette pour le TP d'introduction 
à OSEK  du porjet système embarqué distribué
***********************************************************/

//COMPILATION KEYS
//#define CONTROL_WITH_USART

#define ON_LCD (0x1CD)
#define ON_USART (1)
// wich output to print out debug infos
//#define DEBUG ON_USART

// how to control start stop of trajectories ?
#define WITH_USART	(1)
#define EN_BOUCLE  (0x100F)
#define CONTROL EN_BOUCLE 

//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"

//PERIPH includes
#include "../../lib_cm3/stm_clock.h"
#include "../../lib_cm3/stm_metro_v1.h"
#include "../../lib_user/led.h"

#if (DEBUG == ON_USART)

	#include "../../lib_cm3/lib_usartx.h"
	#include <stdio.h>
	#ifdef CONTROL_WITH_USART
	
		#define bool int
		FILE __stdin;
	#endif

#elif (DEBUG == ON_LCD)

	#include "../../lib_user/lcd.h"

#endif 


//APPLI includes
#include "../../lib_user/lib_trajectoire_2009a.h"

// GLOBALS for appli
Etat cons;
 float Com,VCom,Pos,CPos,Ep,Kp;

void InitApp(void)
{
	Init_Periphs();
	#if ((CONTROL==WITH_USART)||(DEBUG==ON_USART) )
		setup_usart();
	#endif

	#if (DEBUG==ON_LCD)
		lcd_init();
	#endif

	initGenerateur(10, 1000, 2000);
	//             Périod(ms) , Rising time (ms) , Vitmax (pas/s)
}

 int cpt=0;
TASK(Generer_Trajectoire)
{ 
	cpt++;
	if (getPhase())
	{
		Set_Position(0);
		initTrajectoire(32000);
	}
	else
	{
		if (cpt>4)
		{
		//	printf("%d %d\n",(unsigned int) cons.Pos,(unsigned int) Pos);
			cpt=0;
		}
		calculConsigneSuivante();
	}

	TerminateTask();
}

TASK(Clignoter_Led)
{

	Kp=RAPPORT_MAX/40.0;
/*	Toggle_Led(1);
	
	cons= lireConsigne();
	if ((cons.Vit & 0x0001) == 1)
	{
		 Toggle_Led(2);
	}
	if ((cons.Pos & 0x0001) == 1)
	{
		Toggle_Led(3);
	}
	*/

	cons= lireConsigne();
	Pos= (float)(Lire_Position());
	CPos = (float)(cons.Pos);
	Ep = CPos-Pos;
	Com = Kp * Ep ;// + Ki * Ip;
	#define VCOMMAX 10
	if ((Com-VCom)>VCOMMAX) Com = VCom + VCOMMAX;
	if ((Com-VCom)<-VCOMMAX) Com = VCom - VCOMMAX;
	VCom = Com;
	Fixe_Rapport((u16) Com);

	TerminateTask();

}

int main (void)
{
	InitApp();
//	printf("Salut les musclés\n");
	StartOS(OSDEFAULTAPPMODE);
	return 0;	
}