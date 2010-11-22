/*
 * Reseau de capteur version 4
 *
 *   Carte A transmet B en simplex (sans acquitement) l'etat de deux capteurs (Boutons TAMP => PA0; bouton WKUP => PC13) Toute les 0.5 secondes
 *   Carte B fait suivre à C en utilisant XBE, avec acquitement et retransmission.
 *   Carte C envoi un SMS contenant le numero du capteur
 *
 *       [A]---(RT606)--->[B]---(XBEE)--->[C]---(GSM)--->[Telephone]
 *                           <--(XBEE)----            
 *
 * Les clefs de compilation suivantes sont utilisées:
 *         _LCD_DEBUG_: Permet de desactiver l'ecran LCD, lors des simulations
 */

#include "uart.h"
#include "time.h"
#include "gpio.h"
#include "lcd.h"
#include "rs606.h"

#include "STM32_Init.h"
#include <stdio.h>
#include <string.h>
#include "stm_system.h"

/*
 * Affectation des moyens de communication 5GSM/XBEE/RS606) sur les UARTs 
 */
#define GSM		&UART_1
#define XBEE	&UART_2
#define RS606	&UART_3

#define LCD		&LCD_FILE

/*
 * Periode des taches periodiques (exprimé en dizaine de ms) 
 */
#define PERIODE_SYSTICK 10 /* exprimé en dizaine de ms (10ms) */

/*
 * Taille des buffers pour stocker les messages recus (en octets)
 */
#define TAILLE_BUFFER 200

/*
 * Variable globale pour la recuperation des messages des differents moyens de communication 
 */
char c;

char buffer_RS606[TAILLE_BUFFER];
char buffer_XBEE[TAILLE_BUFFER];
int index_buffer_RS606;
int index_buffer_XBEE;
int buffer_RS606_plein;
int buffer_XBEE_plein;

int Counter100ms;

int Buttonstate;
#define TEMPO_RS_MAX 10
#define TEMPO_XBEE_MAX 10
int tempo_envoi_RS;
int tempo_envoi_XBEE;
char caractere_tx_RS;
char caractere_tx_XBEE;
char relache_bouton_RS;
char relache_bouton_XBEE;

/* 
 * Variables et definition pour les machines a état de la RS606 et du XBEE 
 */
enum
{
	RS606_OFF,
	RS606_TRANSMIT,
	RS606_WAIT_END_TRANSMITION,
	RS606_RECEIVE
}

enum
{
	XBEE_OFF,
	XBEE_TRANSMIT,
	XBEE_RECEIVE
}

int RS606MachineState;
int XBEEMachineState;

/*
 * Prototypes des fonctions utilisées par le main
 */
void HarvestMessages(void);
void RS606PeriodicTask(void);
void XBEEPeriodicTask(void);

/*
 * int main (void)
 *
 * Point d'entrée du programme
 *
 * Parametres:
 *     Entrée: 
 *            aucun	
 *     Sortie:
 *            aucun
 *
 * Limitations:
 *     La fonction ne doit pas se terminer -> Boucle infinie
 */
int main (void)
{
	/* Init du micro et des peripheriques */
	stm32_Init ();
	UARTInit();

	lcd_init();	
	lcd_clear();

	/* Reglage du niveau de priorité des interruptions */
#pragma diag_remark	61, 68	/* suppression des messages d'erreur penibles */
	NVIC_SET_PRIO_PERIPH(USART1,10);
	NVIC_SET_PRIO_PERIPH(USART2,10);
	NVIC_SET_PRIO_PERIPH(USART3,10);
	NVIC_SET_PRIO_PERIPH(EXTI4,9);
	NVIC_SET_PRIO_SYSTEM(SYSTICK,14);
#pragma diag_default 61, 68

	GPIOB->ODR = GPIOB->ODR & ~(1<<15);
	RS606SetMode(RS606_RX);

	/* Preparation du service Timer */
	Counter100ms = 0 ; 
	TIMEInit(NULL);

	/* Initialisation des variables globales utilisées par les taches periodiques */
	index_buffer_RS606 = 0;
	index_buffer_XBEE = 0;
	buffer_RS606_plein = 0;
	buffer_XBEE_plein =0;

	tempo_envoi_RS = 0;
	tempo_envoi_XBEE = 0;
	caractere_tx_RS ='A';
	caractere_tx_XBEE = 'A';

	relache_bouton_RS =0;
	relache_bouton_XBEE=0;

	/* Initialisation des machines à etat */
	RS606MachineState = RS606_OFF;
	XBEEMachineState = XBEE_OFF;

	/* Affichage d'un message d'accueil */
	set_cursor(0,0);
	fprintf(LCD,"Res. de capteurs");
	set_cursor(0,1);
	fprintf(LCD,"Version 4");

	/* Attente de 3 secondes */
	TIMEWaitxms(3000);	

	/* Demarrage du systick */
	TIMEEnabled=1;

	/* Effacement de l'ecran et affichage de l'ecran de base */
	set_cursor(0,0);
	fprintf(LCD,"RS-R:                ");
	set_cursor(0,1);
	fprintf(LCD,"XB-R:                ");

	/* Boucle principale, infinie */
	while (1)
	{
		/* Recuperation et mise en forme des messages sur les liaisons series */
		HarvestMessages(); 

		/*
		 * Activation des actions pariodiques
		 * Remarque 1: La periode de base (minimale) est de 10 ms
		 *             Pour obtenir des periodes multiples de cette periode de base, utilisez des compteurs
		 * Remarque 2: Les actions réalisées ici sont dites "collaboratives": si vous faites une operation
		 *             bloquante, vous bloquez toutes les autres actions, y compris la recuperation des messages
		 */

		if (TIME10ms()==TRUE) /* Si 10 ms se sont ecoulées, execution des actions periodiques */
		{
			Counter100ms++;
			
			/* 
			 * Si 100 ms se sont ecoulées, execution des traitements periodiques à recurrence de 100ms 
			 */ 

			if (Counter100ms==PERIODE_SYSTICK)	
			{
				/* Remise a zero du compteur de periode 100ms */
				Counter100ms = 0;
			
			   	/* 
				 * Tache periodique pour le module FM RS606 (433 MHZ)
				 * Voir la description de la fonction pour plus d'infos 
				 */
				RS606PeriodicTask();
			
			   	/* 
				 * Tache periodique pour le module 802.15.4 XBEE (2,4 GHZ)
				 * Voir la description de la fonction pour plus d'infos 
				 */
				XBEEPeriodicTask();
			}	
		}
	}
}

void HarvestMessages(void)
{
		/* Remplissage du buffer pour la liaison RS606 (FM) */
		if (UARTBufferState(RS606) != EMPTY)
		{
			c=(char)fgetc(RS606);
			
			if ((c=='\n') || (c=='\r'))	/* Si l'on recoit un newline (\n) ou un retour chariot (\r), le message est terminé */
			{
				buffer_RS606[index_buffer_RS606]=0;
				buffer_RS606_plein=1;
			}
			else
			{
				buffer_RS606[index_buffer_RS606] = c;
				index_buffer_RS606++;

				if (index_buffer_RS606>=TAILLE_BUFFER) index_buffer_RS606 = TAILLE_BUFFER-1;
			}	
		}

		/* Remplissage du buffer pour la liaison XBEE (2.4GHz) */
		if (UARTBufferState(XBEE) != EMPTY)
		{
			c=(char)fgetc(XBEE);
			
			if ((c=='\n') || (c=='\r'))	/* Si l'on recoit un newline (\n) ou un retour chariot (\r), le message est terminé */
			{
				buffer_XBEE[index_buffer_XBEE]=0;
				buffer_XBEE_plein=1;
			}
			else
			{
				buffer_XBEE[index_buffer_XBEE] = (char)c;
				index_buffer_XBEE++;

				if (index_buffer_XBEE>=TAILLE_BUFFER) index_buffer_XBEE = TAILLE_BUFFER-1;
			}	
		}

		/* Remplissage du buffer pour la liaison GSM (900 Mhz) */
		/* A faire !!! */
}							
 
/*
 * Tache periodique du module RS606
 *
 * Rappel concernant le transmetteur:
 * Le transmetteur utilise une porteuse à 433 Mhz. Le module ne possede aucune intelligence embarqué.
 * En plus des lignes RX et TX directement connectées à l'UART 3, le module possede 3 autres lignes:
 * TX_CMD (PB8): à 0, le module emet une porteuse et transmet les données presentes sur la ligne TX.
 * RX_CMD (PB9): à 0, le module recupere les données presentes dasn la bande 433 Mhz et les recopie sur la ligne RX.
 * CD (PC4): lorsque le module est en reception (RX_CMD à 0), cette ligne indique si une porteuse est en cours d'emission
 *         0: une porteuse est emise
 *         1: Aucune porteuse detectée
 *
 * Le module peut donc fonctionner dans 4 modes:
 *         Off: (TX_CMD et RX_CMD à 1) Le module est desactivé.
 *         Reception: (TX_CMD à 1 et RX_CMD à 0) Le module est en reception.
 *         Transmission: (TX_CMD à 0 et RX_CMD à 1) Le module est en transmission.
 *         LoopBack: (TX_CMD et RX_CMD à 0) Le module est en transmission et recopie sur RX les données envoyées.
 *
 * Si plusieurs modules emettent une porteuse en meme temps, les données transmises seront brouillées. Il est donc
 * important de verifier si le media (la FM) est disponible en verifiant la presence de porteuse (ligne CD) lorsque le module 
 * est en reception.
 *
 * A cela s'ajoutent les limitations suivantes:
 *        A: Lorsque le module est basculé du mode transmission au mode reception, le premier caractere recu est verrolé.
 *        B: Le module etant fondamentalement simplex (half duplex), lorsque l'on passe le module de transmission à reception, 
 *           il faut s'assurer que la precedente transmission soit completement terminée, sinon les données restantes seront perdues.
 *	         Pour info, l'UART est "bufferisé" en transmission et reception. Ainsi, les fonctions printf, puts, etc ne sont pas bloquants.
 *           Autrement dit, ces fonctions se terminent avant que la transmission se soit effectivement terminée !
 *        C: Lorsque le module RS606 est en reception et qu'aucune porteuse n'est emise, le module decode du bruit electronique.
 *           Il en resulte la reception de caracteres aleatoires.
 *        
 * On va resoudre ces problemes en:
 *        A: Ajoutant un caractere supplementaire ($) en debut de message à transmettre. Lors de la reception d'un message
 *           le premier caractere sera ignoré et supprimé.
 *        B: Utilisant une fonction special de l'UART (UARTTransmissionComplete) pour savoir si la transmission est completement
 *           terminée.
 *        C: Surveillant la ligne CD et desactiver la reception de l'UART 3 lorsque la ligne est à 1 (pas de porteuse). Ceci est
 *           réalisé par le fichier RS606.c et par UART.c
 *
 * La machine à état va etre celui-ci:
 *
 *									 
 *			     B(TAMP) appuyé	  ----------------  B(TAMP) relaché	  ----------------------------  
 *			  ------------------> | Transmission |------------------> | Attente fin transmission |-----
 *			  | 				  ----------------					  ----------------------------	  |
 * 		   -------					 ^																  |
 * Init -> | OFF | 					 |																  |
 *		   -------                   --------------------------										  |
 *			  |					  -------------				  |										  |
 *			  ------------------> | Reception |----------------										  |
 *				 B(TAMP) relaché  -------------	B(TAMP) appuyé										  |
 *										^															  |
 *										|															  |
 *										---------------------------------------------------------------
 *												  UARTTransmissionComplete() == 1
 * 
 */        
void RS606PeriodicTask(void)
{
	ButtonState = GPIOButton(BUTTON_TAMP);

	switch (RS606MachineState)
	{
	case RS606_OFF:
		if ((ButtonState == BUTTON_PRESSED) || (ButtonState == BUTTON_JUST_PRESSED))
		{
			/* Bouton appuyé -> En transmission */
			RS606MachineState = RS606_TRANSMIT;

			RS606SetMode(RS606_TX);
		}
		else
		{
		}
		break;
	case RS606_TRANSMIT:
		break;
	case RS606_WAIT_END_TRANSMITION:
		break;
	case RS606_RECEIVE:
		break;
	default:
		RS606MachineState = RS606_OFF; 
	}

	if (!GPIOGetState(BUTTON_TAMP))	
	{
		/* Passe le module RS606 en transmission */
		RS606SetMode(RS606_TX);
		relache_bouton_RS = 1;
	
		/* Envoi de caractere sur RS606 */
		tempo_envoi_RS++;
	
		if (tempo_envoi_RS>=TEMPO_RS_MAX)
		{
			tempo_envoi_RS =0;
			caractere_tx_RS++;
			
			if (caractere_tx_RS>'Z') caractere_tx_RS = 'A';
	
			set_cursor(0,0);
			fprintf(LCD,"RS-T: MSG %c   ",caractere_tx_RS);
	
			fprintf (RS606,"$MSG %c\r",caractere_tx_RS);  /* le dollar sert de caractere perdu */
		}
	}
	else
	{
		if (relache_bouton_RS==1)
		{
			/*  passe le RS606 en mode reception*/
			RS606SetMode(RS606_RX);
	
			relache_bouton_RS=0;
	
			set_cursor(0,0);
			fprintf(LCD,"RS-R:                ");
		}
	
		/* si une trame à été recue */
		if (buffer_RS606_plein == 1)
		{
			buffer_RS606_plein =0;
			index_buffer_RS606 = 0;
	
			if (strlen (buffer_RS606) >1)
			{
				set_cursor(0,0);
				fprintf(LCD,"RS-R: %s           ",buffer_RS606+1); 	/* suppression du premier caractere */
			}
			else
			{
				set_cursor(0,0);
				fprintf(LCD,"RS-R:              ");
			}
		}
	}
}

/*
 * Tache periodique du module XBEE
 *
 * Rappel concernant le module XBEE:
 * A la difference du module RS606, le XBEE embarque de l'intelligence (en fait il implemente une stack PHY 
 * conforme à la norme 802.15.4). Du coup, la surveillance du media, la verification de collision, le filtrage 
 * d'adresse et de pan ID sont directement integrés au module.
 *
 * Le module apparait donc basiquement comme une liaison serie bi-directionnelle. 
 *
 * La machine à état ressemble donc à ceci:
 *
 *									 
 *			     B(WKUP) appuyé	  ----------------  B(WKUP) relaché	   
 *			  ------------------> | Transmission |-----------------------
 *			  | 				  ----------------					    |
 * 		   -------					     ^								|
 * Init -> | OFF | 					     |								|
 *		   -------                       ----------------------  		|
 *			  |					  -------------				  |			|
 *			  ------------------> | Reception |----------------			|
 *				 B(WKUP) relaché  -------------	B(WKUP) appuyé		    |
 *										^								|
 *										|								|
 *										---------------------------------
 *												  
 * 
 */  
void XBEEPeriodicTask(void)
{
	if (!GPIOGetState(BOUTON_WKUP))	
	{
		/* Envoi de caractere sur XBEE */
		tempo_envoi_XBEE++;
		relache_bouton_XBEE = 1;
	
		if (tempo_envoi_XBEE>=TEMPO_XBEE_MAX)
		{
			tempo_envoi_XBEE =0;
			caractere_tx_XBEE++;
	
			if (caractere_tx_XBEE>'Z') caractere_tx_XBEE = 'A';
		
			set_cursor(0,1);
			fprintf(LCD,"XB-T: MSG %c   ",caractere_tx_XBEE);
	
			fprintf (XBEE,"MSG %c\r",caractere_tx_XBEE);
		}
	}
	else
	{
		if (relache_bouton_XBEE==1)
		{
			relache_bouton_XBEE=0;
	
			set_cursor(0,1);
			fprintf(LCD,"XB-R:                ");
		}
	
		/* Si une trame à été recue */
		if (buffer_XBEE_plein == 1)
		{
			buffer_XBEE_plein =0;
			index_buffer_XBEE = 0;
	
			set_cursor(0,1);
			fprintf(LCD,"XB-R: %s         ",buffer_XBEE); 
		}
	}
}


