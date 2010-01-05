// Pierre-Emmanuel Hladik et Pascal Acco
// 12/12/2009
/**********************************************************
Fichier squelette pour le TP d'introduction 
à OSEK  du porjet système embarqué distribué
***********************************************************/

#include "standard_types.h"

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
#define Evt_arrivee	1
//PERIPH includes
#include "../../lib_cm3/stm_clock.h"
#include "../../lib_cm3/stm_metro_v1.h"

//CAN stufs
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Reg.h"                            // STM32 register and bit Definitions
#include "STM32_Init.h"                           // STM32 Initialization
                        						  // STM32 Initialization
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


#ifdef USER_HARDWARE_FAULT_HANDLER
void Arret_Urgence(void)
{

	Fixe_Rapport(0);
//	CAN_waitReady ();
	CAN_wrMsg (&CAN_TxMsg);                     // transmit message
   
	while (1)
	{
//		CAN_waitReady ();
		CAN_wrMsg (&CAN_TxMsg);                     // transmit message
  		Fixe_Rapport(0);
    	Blink_Leds(10,DUREE_RAPIDE);
    	Blink_Leds(4,DUREE_LENTE);
	};
}
#endif

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
Une_Consigne Cons; 

float Com,VCom,Pos,CPos,Ep,Kp;
#define DISTANCE 27792			
//#define DISTANCE 277
int Pannic;
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
	if (getPhase())
	{
		SetEvent ( 2, Evt_arrivee);
		CancelAlarm(1);
		SetRelAlarm (1, 1000, 10) ;
		#if (DEBUG == ON_USART)
			printf("%d",(int) Lire_Position());
		#endif
		reinitEtat(DISTANCE);
		initTrajectoire(DISTANCE);
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