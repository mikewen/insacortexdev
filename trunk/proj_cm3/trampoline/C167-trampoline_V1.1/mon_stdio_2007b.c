/* STDIO_ASC.c ****************************************************************
 *	Entrées-Sorties série via le port asynchrone ASC en simu et en réel       *
 *  stdio.h est donc utilisable (printf etc.) et visualisables sur la         *
 *  "serial windows #1" de Keil.
 *   Septembre 2007 pascal Acco												  *
 ******************************************************************************
 ******************************************************************************
 * En mode réel, UTILISEZ la clé de compilation et d'assemblage MCB167NET.	  *
 * En mode  simulé, EFFACEZ cette clé. (Onglets C166 et A166 du projet)		  *
 ******************************************************************************
 * void Init_IO_ASC(void) : il faut lancer cette fonction en premier afin de  *
 *			pouvoir bénéficier de la bibliothèque stdio et effectuer des E/S. *
 *				- Mode réel : les E/S passent par la ligne ASC configurée par *
 *                 le moniteur: On fait rien donc !                           *
 *				- Mode simulé : les E/S passent par la liasion série ASC      *
 *						(visu sur fenêtre SERIAL #1 de KEIL).        		  *
 *																			  *
 ******************************************************************************/

#include <c167cs.h>
#include <intrins.h>
#include "mon_stdio_2007b.h"

#define Trappe_Logicielle 0x50

  
void Init_Mon_Stdio (void)
	{
#if(STDIO_MODE==STDIO_ASC_SIM)
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
	// Tout est configuré par le moniteur !!!
#endif
//# ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # ENDIF # END
}
