#include "../os/tpl_os.h"
#include "../../lib_user/uart.h"
#include "../../lib_user/lcd.h"
#include "../../lib_user/lib_trajectoire_2009a.h"
#include "tpl_os_generated_configuration.h"
#include "../../system/STM32_Init.h"
#include "../../lib_user/led.h"

/*Test selon le mail de peh

  Un système avec deux tâches, l''une T1 de période 10 ms et de durée 7 ms (à ajuster avec une boucle),
 la seconde T2 de période 2 ms et de durée 1 ms avec démarrage synchrone (même valeur de ALARMTIME).
  Les deux tâches doivent avoir une valeur pour ACTIVATION de 1 (pas de mémorisation des activations). 
  La période de T2 supérieure à T1.

 Première version les deux tâches FULL pour SCHEDULE (voir fichier archi_appli_1.oil).
  Normalement T1 devrait rater une activation sur 2. 
  De plus tu devrais observer plusieurs préemptions de T1 par T2 (tu verras si le changement de contexte se fait bien).
*/

int compteur = 0;

#define TMP_CNT ((int)(7000.0/4.0/(0.219099-0.00028363)*0.1) )



void InitApp(void)
{

	stm32_Init ();

	//lcd_init();
	   initGenerateur(20, 800, 1000);
}

int main (void)
{
	InitApp();
	StartOS(OSDEFAULTAPPMODE);
	return 1;
		
}



TASK(Tache1)
{

			if (getPhase())
	{
		initTrajectoire(5600);
	}
	else
	{
		calculConsigneSuivante();
	}

    TerminateTask();
}

TASK(Tache2)
{

	Etat cons;

	Toggle_Led(LED_1);
	
	cons= lireConsigne();
	if ((cons.Vit & 0x0001) == 1)
	{
		 Toggle_Led(LED_2);
	}
	if ((cons.Pos & 0x0001) == 1)
	{
		Toggle_Led(LED_3);
	}

    TerminateTask();
}
