#include "../os/tpl_os.h"
#include "../../lib_user/uart.h"
#include "../../lib_user/lcd.h"
#include "../../lib_user/lib_trajectoire_2009a.h"
#include "tpl_os_generated_configuration.h"
#include "../../system/STM32_Init.h"
#include "../../lib_user/led.h"
#include "../../lib_cm3/Metro_Librairie_Version2009d.h"

//#include <stm32f10x_lib.h> 
 
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
Etat cons;

#define TMP_CNT ((int)(7000.0/4.0/(0.219099-0.00028363)*0.1) )
#define TENSION 3000 

void InitApp(void)
{
	stm32_Init ();
	Init_Periphs();

	//lcd_init();
	initGenerateur(20, 800, 1000);

	cons.Vit = 0;
	//UART3->BRR = 0x823;
}

int main (void)
{
	InitApp();
	
	printf (XBEE,"Tapez  1 pour lancer le test de roulage en avant, 2 pour arreter:\r\n");

	StartOS(OSDEFAULTAPPMODE);

	return 1;		
}



TASK(Tache1)
{
u8 c;
bool test;

/*	if (getPhase())
	{
		initTrajectoire(5600);
	}
	else
	{
		calculConsigneSuivante();
	}
*/

		
        test = getchar_NB (XBEE,&c);
	    
		if (test)
		{
			switch(c)
			{
				case '1':
					printf(XBEE,"Lancement du test ...\r\n");
					printf(XBEE,"Mesures du Courant;Position;Vitesse\r\n"); 
					
					/* A fond en avant */
					cons.Vit = TENSION;
	
					break ;
				 
		    	case '2':
					printf (XBEE,"Arret\r\n");
	
					/*Stoppez les machines !*/
					cons.Vit = 0;				
					
					break;
						
				default : 
			        printf(XBEE,"Vous avez tapé les mauvais choix, boulet! \r\n");
			        break;
			}	// Fin switch (c)
		}		

    TerminateTask();
}

TASK(Tache2)
{
	/*Etat cons;
	cons= lireConsigne(); */
	
	Fixe_Rapport(cons.Vit);

    TerminateTask();
}
