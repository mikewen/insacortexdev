/* mon_stdio ****************************************************************
 *  permet d'utiliser stdio.h (printf,scanf etc.) avec différentes solutions
 *  hardwares définies par STDIO_MODE:
 *    STDIO_ASC_SIM => simule les transferts par la liason série asynchrone ASC0
 *         visualisation en simulé sur la sérial window UART0
 *    STDIO_ASC_REEL => transfert par la liason série ASC0 (programmée par le
 *         moniteur de Keil). Visualition
 *
 * En mode réel, UTILISEZ la clé de compilation et d'assemblage MCB167NET qui 
 *    va forcer STDIO_MODE dans le mode STDIO_ASC_REEL	                      *
 * En mode  simulé, EFFACEZ cette clé MCB167NET. (Onglets C166 et A166 du projet)
 *    qui va forcer STDIO_MODE dans le mode STDIO_ASC_SIM  		  *
 *
 * Dans tousq les cas Visualisation sur la fenêtre SERIAL #1 de KEIL  		  *
 ******************************************************************************
 * void Init_Mon_Stdio(void) : il faut lancer cette fonction en premier afin de  *
 *			pouvoir bénéficier de la bibliothèque stdio et effectuer des E/S. *
 *																			  *
 ******************************************************************************/
#include <stdio.h>
#define STDIO_ASC_SIM 1
#define STDIO_ASC_REEL 2

#ifdef MCB167NET
#define STDIO_MODE STDIO_ASC_REEL
#else
#define STDIO_MODE STDIO_ASC_SIM
#endif

void Init_Mon_Stdio (void);
