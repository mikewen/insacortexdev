/* STDIO_SSC.c ****************************************************************
 *	Entrées-Sorties série via le port synchrone SSC et le circuit d'interface *
 *		MAX3110E de MAXIM. Ce module met donc en place une seconde liaison	  *
 *		série RS232, 9600 Bauds, 8 bits sans parité, sans contrôle de flux.   *
 *	Le 9 septembre 2002														  *
 *	par	F. Pompignac														  *
 ******************************************************************************
 **	ATTENTION ! Ce module utilise des interruptions pour la détection		 **
 **		d'erreurs. Il autorise donc les IT (IEN = 1).						 **
 ******************************************************************************
 * En mode réel, UTILISEZ la clé de compilation et d'assemblage MCB167NET.	  *
 * En mode  simulé, EFFACEZ cette clé.										  *
 ******************************************************************************
 * void Init_IO_SSC(void) : il faut lancer cette fonction en premier afin de  *
 *			pouvoir bénéficier de la bibliothèque stdio et effectuer des E/S. *
 *			(Si des LEDs clignotent sur la carte, voir la rubrique PROBLEMES  *
 *			un peu plus loin;)												  *
 *				- Mode réel : les E/S passent par la seconde ligne RS232,	  *
 *						(COM2 et terminal virtuel).							  *
 *				- Mode simulé : les E/S passent par la première ligne RS232,  *
 *						(COM1 et fenêtre SERIAL #1 de KEIL).				  *
 *																			  *
 * void putchar (char)    : PRIMITIVE d'envoi d'un caractère sur la liaison   *
 *			série 2. Elle est employée par toutes les fonctions d'affichage : *
 *			puts(), printf(), vprintf().									  *
 *				- Mode réel : elle remplace automatiquement la primitive	  *
 *						putchar() fournie par KEIL.							  *
 *				- Mode simulé : la présente fonction n'est pas utilisée.	  *
 *																			  *
 * char _getkey (void)	  : PRIMITIVE de réception d'un caractère depuis la   *
 *			liaison série 2. Elle est appelée par toutes les fonctions de	  *
 *			saisie : getchar(), scanf(), gets()...							  *
 *				- Mode réel : elle remplace automatiquement la primitive	  *
 *						_getkey() fournie par KEIL.							  *
 *				- Mode simulé : la présente fonction n'est pas utilisée.	  *
 ******************************************************************************/
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PROBLEMES :															       %
%      - Trois LED's clignotent sur la carte MCB167NET : impossible d'initia-  %
% liser la liaison série. Le circuit n'est sans doute pas correctement branché,%
% ou n'est pas alimenté.												       %
%      - Une LED clignote : reportez-vous aux lignes 372-382 [IT_Erreur_SSC()] %
% du présent listing.													       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include <reg167.h>
#include <intrins.h>

#define Trappe_Logicielle 0x50

/*                        *****************************						  */
/**************************>>>>> PARTIE PUBLIQUE <<<<<*************************/ 
/*						  *****************************						  */


/******************************************************************************
 *  void Init_IO_SSC (void)													  *	
 **************************													  *	
 *	Si Mode réel (clé MCB167NET présente)									  *	
 *		alors	Initialiser le port série synchrone SSC,					  *	
 *				Configurer MAXIM3110E										  *	
 *		sinon (Mode simulé)													  *	
 *				Initialiser le port 3										  *	
 *				Initialiser S0												  *	
 ******************************************************************************
 */ 
void Init_IO_SSC (void)
	{
#ifndef MCB167NET
//# MODE SIMULE # MODE SIMULE # MODE SIMULE # MODE SIMULE # MODE SIMULE # MODE
	P3 = P3 | 0x0400;		/* La bascule 10 de sortie du port P3 doit être à '1'
				car elle est combinée par un ET avec la fonction alternative.
						 							(User's Manual, p. 7-7) */
	DP3 = DP3 | 0x0400;		// ligne P3.10 en sortie - TxD
	_nop_();				// pipe-line !   (User's Manual, p. 4-8)
	DP3 = DP3 & 0xF7FF;		// ligne P3.11 en entrée - RxD
	_nop_();				// pipe-line !
	S0TIC = 0x0080;			// prêt à émettre, sans IT.
	S0RIC = 0;				// rien reçu, sans IT itou.
	S0BG  = 0x0040;			// vitesse : 9600 bauds.

/*  S0CON : voir User's Manual, pp. 11-2 et 11-3.
	  S0R	1   Générateur de bauds en marche			S0CON.15
	  S0LB	0   Usage normal, sans rebouclage			S0CON.14
	  S0BRS	0   Vitesse définie par S0BG.				S0CON.13
	  S0ODD	0   Parité.									S0CON.12
	   ---  0	--										S0CON.11
	  S0OE	0   RAZ du drapeau d'erreur de débordement	S0CON.10
	  S0FE	0   RAZ du drapeau d'erreur de trame		S0CON.9
	  S0PE	0   RAZ du drapeau d'erreur de parité		S0CON.8
	  S0OEN	0   Ignore les débordements					S0CON.7
	  S0FEN	0   Ignore les erreurs de trame				S0CON.6
	  S0PEN	0   Ignore la parité						S0CON.5
	  S0REN	1   RECEPTEUR EN MARCHE !					S0CON.4
	  S0STP	0   Trame avec 1 bit de stop				S0CON.3
	  S0M	001 Trame avec 8 bits de données, async.	S0CON.2-0
	---------------
	c'est-à-dire  S0CON <-- 1000 0000 0001 0001b = 8011h
 */
 	S0CON = 0x8011;
#else
	_nop_ ();
#endif
//# ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # END
	}


//# ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # END

