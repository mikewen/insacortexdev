/*
________________________________________________________________________________________
 Application d'exemple pour le BE Système Embarqué Distribué des 5TRS
 poursuite_no_CAN.c
________________________________________________________________________________________
USAGE
	exécutez le script got_goiled.bat et choisissez Projet_no_can.oil
		=> génération de fichier pour le noyau temps réel OSEK/trampoline
	compilez et testez Simulateur ou LocoLymex
________________________________________________________________________________________
REVS
	[Acco PEH DiMERCURIO 06/01/2010] finalisation et commentaires de la première version
		Testée en réel et simulé
________________________________________________________________________________________
TODO 
	Corriger le BOGUE qui arrive dès fois en réel
	Faire le BE :-)

__________________________________________________________________________________
  
  	Tache à 10 ms : Générateur de trajectoire pour faire un tour de piste 
	avec la locomotive.
					
	
	Tache à 2ms : Régulation PID de la voiture sur la consigne générée

	Tache éternelle : fait rien

________________
               /  Hardware Fault apparaît uniquement en réel ??!!!
			  /
			 /______________\   Hardware fault récupérée par la fonction
			 				/       Arret_Urgence

	 Arret_Urgence() :
		La fonction stoppe la commande 
		et fait clignotter les leds avant et arrière
		La fonction ne termine jamais

*/

#include "standard_types.h"

// DEBUG avec printf sur USART ou LCD
// pour le moment ça déconne !

#define ON_LCD (0x1CD)
#define ON_USART (1)
// COMMENBT this line to remove printf debug
//#define DEBUG ON_USART

// how to control start stop of trajectories ?
#define WITH_USART	(1)
#define EN_BOUCLE  (0x100F)
#define CONTROL EN_BOUCLE 

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

//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"
#define Evt_arrivee	1	   //TODO débuguer trampoline

//PERIPH includes
#include "../../lib_cm3/stm_clock.h"
#include "../../lib_cm3/cm3_traps.h"
#include "../../lib_cm3/stm_metro_v1.h"



//____________________________________________________________
//    Arret_Urgence en cas de Hardware fault provoqué par cette
//         voiture
//    OU en cas de réception du message de panique sur le bus CAN
//____________________________________________________________

#ifdef USER_HARDWARE_FAULT_HANDLER
void Arret_Urgence(void)
{

	Fixe_Rapport(0); //stopee la machine
   
	while (1)
	{
		//fait clignotter les leds avant et arriere
    	Blink_Leds(10,DUREE_RAPIDE);
    	Blink_Leds(4,DUREE_LENTE);
	};
}
#endif




//APPLI includes
#include "../../lib_user/lib_trajectoire_2009a.h"
#include "./lib_autom_2009a.h"






// GLOBALS for appli
Une_Consigne Cons; 

float Com,VCom,Pos,CPos,Ep,Kp;
		

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

	Set_Position(NB_PAS_BOUCLE); //comme si on terminait un cycle 
	
}
TASK(Generer_Trajectoire)
{ 
	if (TRAJECTOIRE_TERMINEE)
	{
		SetEvent ( 2, Evt_arrivee);
		CancelAlarm(1);
		SetRelAlarm (1, 1000, 10) ;
		#if (DEBUG == ON_USART)
			printf("%d",(int) Lire_Position());
		#endif
		reinitEtat(NB_PAS_BOUCLE);
		initTrajectoire(NB_PAS_BOUCLE);

	}
	else
	{
		calculConsigneSuivante();
	}
	Cons= lireConsigne();

	TerminateTask();
}


Etat etatRame;

float com;
float gainsCommande[3] = {100.0 , 46.0 , 8.4} ;
TASK(Controler_Rame)
{

	// Cons mise à jour par generer

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