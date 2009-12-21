/* mon_stdio ****************************************************************
 *  permet d'utiliser stdio.h (printf,scanf etc.) avec différentes solutions
 *  hardwares :
 *    STDIO_ASC_SIM => simule les transferts par la liason série asynchrone ASC0
 *         visualisation en simulé sur la sérial window UART0
 *    STDIO_ASC_REEL => transfert par la liason série ASC0 (programmée par le
 *         moniteur de Keil). Visualition 
 *
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
#include <stdio.h>

void Init_Mon_Stdio (void);
