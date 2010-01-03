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

void Init_IO_ASC (void);
