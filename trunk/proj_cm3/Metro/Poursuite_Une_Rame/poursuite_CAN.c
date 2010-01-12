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
	- Il y a une petite impulsion de commande à l'init ! Le controler démarre
	   avant le générateur de trajectoire... à débugguer
	- L'événement ne fonctionne pas . Problème de trampoline ?
	- Faire le BE :-)

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


#define ON_LCD (0x1CD)
#define ON_USART (1)
// COMMENT this line to remove printf debug
#define DEBUG ON_USART

// how to control start stop of trajectories ?
#define WITH_USART	(1)
#define EN_BOUCLE  (0x100F)
#define CONTROL EN_BOUCLE 

#if (DEBUG == ON_USART)
	#include "../../lib_cm3/stm_usartx.h"
	#include <stdio.h>
	
#endif
#if (DEBUG == ON_LCD)

	#include "../../lib_cm3/lcd.h"

#endif 

//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"
#define EVENT_ARRIVEE	2	   //TODO débuguer trampoline

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

#ifdef USER_HARDWARE_FAULT_HANDLER   //voir ../.../lib_cm3/cm3_traps.h et ./cm3_traps_config.h
void Arret_Urgence(void)
{

	Fixe_Rapport(0); //stopee la machine
	CAN_wrMsg (&CAN_TxMsg); // send message	Pannic 66= 0xBA
   
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
Etat etatRame;
float com;
float gainsCommande[3] = {100.0 , 46.0 , 8.4} ;

int Pannic;

//_________________________________________________________________________
TASK(Controler_Rame)
// Tâche 0
// Alarm 0 (2ms)
//_________________________________________________________________________
{

	// Consigne mise à jour par "Generer_Trajectoire"
	etatRame = calculerEtatRelatif(Cons.Pos,Cons.Vit);

  	if (TRAJECTOIRE_TERMINEE)  // voir lib_trajectoire_2009.h
	{   //Arrêt en station
		Fixe_Rapport(0); 
	}
	else
	{
		com=calculerCommande(etatRame,gainsCommande) ;
		Fixe_Rapport((u16) com);
	}
	TerminateTask();

}

//_________________________________________________________________________
TASK(Generer_Trajectoire)
// Tâche 1
// Alarm 1 (10ms)
//_________________________________________________________________________
{ 
	// Vérifie si un message passe sur le CAN
   if (CAN_RxRdy) 
   {
       CAN_RxRdy = 1;
	   Pannic = CAN_RxMsg.data[0];
    }

	// Tire l'alarme si panique
  	if (Pannic)
	{
		CancelAlarm(0);
		CancelAlarm(1);
		Arret_Urgence(); 
		//fonction bloquante
	}

	// Gére la trajectoire
	if (TRAJECTOIRE_TERMINEE)  // voir lib_trajectoire_2009.h
	{
		SetEvent (2, EVENT_ARRIVEE); //déclenche la tâche d'affichage
		
		// On bloque le générateur pour 1 seconde (attente en station)
		CancelAlarm(1);
		SetRelAlarm (1, 1000, 10) ;

		//affiche la position en fin de station
		#if (DEBUG == ON_USART)
		printf("Pos %i\n ",(int) Lire_Position());
		#endif

		// recale le compteur de position en relatif
		// pour éviter les overflow 
		reinitEtat(NB_PAS_BOUCLE);
		initTrajectoire(NB_PAS_BOUCLE);

	}
	else // Trajectoire en cours de réalisation
	{
		calculConsigneSuivante();
	}

	//Mets à jour la consigneutilisée par "Controler_Rame"
	Cons= lireConsigne();

	TerminateTask();
}

//_________________________________________________________________________
TASK(Afficher)
// Tache 2
//	sans alarme
{
	while (1) 
	{
	 	WaitEvent(EVENT_ARRIVEE);
		ClearEvent(EVENT_ARRIVEE);
		
		//affiche la position en fin de station
		#if (DEBUG == ON_USART)
		printf("Position = %i\n ",(int) Lire_Position());
		#endif

	}
	TerminateTask();

}

//_________________________________________________________________________
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

	// COnfigure le profil de trajectoire
	initGenerateur(10, 2000, 1800);
	//      Périod(ms) , Rising time (ms) , Vitmax (pas/s)

	Set_Position(NB_PAS_BOUCLE); //comme si on terminait un cycle 
	
	can_Init ();                                    // initialise CAN interface	
	Pannic=0;
}

int main (void)
{
	InitApp();
	StartOS(OSDEFAULTAPPMODE);
	return 0;	
}