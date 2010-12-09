/*
________________________________________________________________________________________
 Application de démonstration pour les ressources sous trampoline
________________________________________________________________________________________
USAGE
	exécutez le script got_goiled.bat et choisissez Projet_no_can.oil
		=> génération de fichier pour le noyau temps réel OSEK/trampoline
	compilez et testez Simulateur ou LocoLymex
________________________________________________________________________________________
REVS
	
________________________________________________________________________________________

*/

#include "standard_types.h"

// DEBUG avec printf sur USART ou LCD


#define ON_LCD (0x1CD)
#define ON_USART (1)
// COMMENT this line to remove printf debug
#define DEBUG ON_LCD


#if (DEBUG == ON_LCD)

	#include "../../lib_cm3/lcd.h"

#endif 

//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"

//PERIPH includes
#include "../../lib_cm3/stm_clock.h"





//_________________________________________________________________________
TASK(Task_1)
// Tâche 0
// Alarm 0 (2ms)
//_________________________________________________________________________
{

	TerminateTask();

}

//_________________________________________________________________________
TASK(Task_2)
// Tâche 1
// Alarm 1 (10ms)
//_________________________________________________________________________
{ 

	TerminateTask();
}

//_________________________________________________________________________
TASK(Task_3)
// Tache 2
//	sans alarme
{

	TerminateTask();

}

//_________________________________________________________________________
void InitApp(void)
{
	#if (DEBUG == ON_LCD)
		lcd_init();
	#endif
}

int main (void)
{
	InitApp();
	StartOS(OSDEFAULTAPPMODE);
	return 0;	
}