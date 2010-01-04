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
#define DEBUG ON_USART

// how to control start stop of trajectories ?
#define WITH_USART	(1)
#define EN_BOUCLE  (0x100F)
#define CONTROL EN_BOUCLE 

//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"
#define Evt_arrivee	1
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
#include "../../lib_user/lib_autom_2009a.h"
// GLOBALS for appli
Etat cons;

float Com,VCom,Pos,CPos,Ep,Kp;
//#define DISTANCE 27792			
#define DISTANCE 277

void InitApp(void)
{
	Init_Periphs();

	#if ((CONTROL==WITH_USART)||(DEBUG==ON_USART) )
		setup_usart();
		printf("Usart OK\n");
	#endif

	#if (DEBUG==ON_LCD)
		lcd_init();
	#endif

	initGenerateur(10, 2000, 1800);
	//             Périod(ms) , Rising time (ms) , Vitmax (pas/s)

	Set_Position(DISTANCE); //comme si on terminait un cycle 
		
}
TASK(Generer_Trajectoire)
{ 
	if (getPhase())
	{
		SetEvent ( 2, Evt_arrivee);
		CancelAlarm(1);
		SetRelAlarm (1, 1000, 10) ;
		printf("%d",(int) Lire_Position());
		reinitEtat(DISTANCE);
		initTrajectoire(DISTANCE);

	}
	else
	{
		calculConsigneSuivante();
	}

	TerminateTask();
}


Etat etatRame;
Une_Consigne Cons;
float com;
float gainsCommande[3] = {100.0 , 46.0 , 8.4} ;
TASK(Controler_Rame)
{


	Cons= lireConsigne();
	etatRame = calculerEtatRelatif(Cons.Pos,Cons.Vit);
	
	com=calculerCommande(etatRame,gainsCommande) ;
	Fixe_Rapport((u16) com);

	TerminateTask();

}

TASK(Afficher)
{
	while (1) {
	 	WaitEvent(Evt_arrivee);
		ClearEvent(Evt_arrivee);

		//printf("ab");

	}
	TerminateTask();

}

int main (void)
{
	InitApp();
//	printf("Salut les musclés\n");
	StartOS(OSDEFAULTAPPMODE);
	return 0;	
}