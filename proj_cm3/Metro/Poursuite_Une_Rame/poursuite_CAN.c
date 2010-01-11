/*
________________________________________________________________________________________
 Application d'exemple pour le BE Système Embarqué Distribué des 5TRS
 poursuite_CAN.c
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
					Vérifier s'il y a PANIQUE. Dans ce cas stoppe toute les tache
					et appèle Arret_Urgence qui ne termine jamais.
	
	Tache à 2ms : Régulation PID de la voiture sur la consigne générée

	Tache éternelle : fait rien

________________
               /  Hardware Fault apparaît uniquement en réel ??!!!
			  /
			 /______________\   Hardware fault récupérée par la fonction
			 				/       Arret_Urgence

	 Arret_Urgence() :
	 	 envoie sur le bus CAN le message 66=0xAB pour indiquer
	 	qu'il y a PANIQUE. La fonction stoppe la commande 
		et fait clignotter les leds avant et arrière
		La fonction ne termine jamais

*/

#include "standard_types.h"

// DEBUG avec printf sur USART ou LCD
// pour le moment ça déconne !

#define ON_LCD (0x1CD)
#define ON_USART (1)
// COMMENBT this line to remove printf debug
#define DEBUG ON_USART

// how to control start stop of trajectories ?
#define WITH_USART	(1)
#define EN_BOUCLE  (0x100F)
#define CONTROL EN_BOUCLE 

#if (DEBUG == ON_USART)
	#include "../../lib_cm3/stm_usartx.h"
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

//CAN stufs
#include "../../lib_cm3/CAN.h"                    // STM32 CAN adaption layer







/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {
  int i;

  CAN_setup ();                                   // setup CAN interface
 
  CAN_wrFilter (33, STANDARD_FORMAT);             // Enable reception of messages
 
  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
  //CAN_testmode(CAN_BTR_LBKM);      // Loopback, Silent Mode (self-test)

  CAN_start ();                                   // leave init mode

  CAN_waitReady ();                               // wait til mbx is empty

  CAN_TxMsg.id = 33;                              // initialise message to send
  for (i = 0; i < 8; i++) CAN_TxMsg.data[i] = 0xAB;
  CAN_TxMsg.len = 1;
  CAN_TxMsg.format = STANDARD_FORMAT;
  CAN_TxMsg.type = DATA_FRAME;

}




//____________________________________________________________
//    Arret_Urgence en cas de Hardware fault provoqué par cette
//         voiture
//    OU en cas de réception du message de panique sur le bus CAN
//____________________________________________________________

#ifdef USER_HARDWARE_FAULT_HANDLER
void Arret_Urgence(void)
{

	Fixe_Rapport(0); //stopee la machine
	CAN_wrMsg (&CAN_TxMsg); // transmit message	Pannic 66= 0xBA
   
	while (1)
	{
		CAN_wrMsg (&CAN_TxMsg);
  		Fixe_Rapport(0);
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
		
//#define NB_PAS_BOUCLE 277
int Pannic;

void InitApp(void)
{
	Init_Periphs();

	#if ((CONTROL==WITH_USART)||(DEBUG==ON_USART) )
		setup_usart();
		printf("Usart OK\n ");
	#endif

	#if (DEBUG==ON_LCD)
		lcd_init();
	#endif

	initGenerateur(10, 2000, 1800);
	//             Périod(ms) , Rising time (ms) , Vitmax (pas/s)

	Set_Position(NB_PAS_BOUCLE); //comme si on terminait un cycle 
	
	can_Init ();                                    // initialise CAN interface	
	Pannic=0;
}
TASK(Generer_Trajectoire)
{ 
   if (CAN_RxRdy) {
      CAN_RxRdy = 1;

      Pannic = CAN_RxMsg.data[0];
    }
  	if (Pannic)
	{
		CancelAlarm(0);
		CancelAlarm(1);
		Arret_Urgence();
	}
	if (TRAJECTOIRE_TERMINEE)
	{
		SetEvent ( 2, Evt_arrivee);
		CancelAlarm(1);
		SetRelAlarm (1, 1000, 10) ;
		#if (DEBUG == ON_USART)
		printf("%i\n ",(int) Lire_Position());
		//printf("Stop\n ");
		#endif
		reinitEtat(NB_PAS_BOUCLE);
		initTrajectoire(NB_PAS_BOUCLE);
	   	//while (~CAN_TxRdy);
		//CAN_waitReady ();
		//CAN_wrMsg (&CAN_TxMsg);                     // transmit message

	}
	else
	{
		calculConsigneSuivante();
	}
	Cons= lireConsigne();
/*	if (CAN_TxRdy) {
      CAN_TxRdy = 0;
	  CAN_wrMsg (&CAN_TxMsg);                     // transmit message
   
    }
*/
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

		//printf("Position = %d",(int) etatRame.Pos);

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