//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"

//PERIPH includes
#include "../../lib_cm3/stm_clock.h"
#include "../../lib_cm3/cm3_traps.h"
#include "../../lib_cm3/stm_metro_v1.h"

#include "../../lib_cm3/stm_usartx.h"
#include <stdio.h>



unsigned char go = 0;

#define T_SAMP 2 // ms échantillonage mesure
#define T_ORDO 10 //ms échnatillonage consigne os_wait etc. 
#define T_WAIT 100
#define TENSION (0x0FFF)
#define BUFF 1000
#define T_WAIT_MES (BUFF*T_SAMP)


u16 stock_courant_a[BUFF];
u16 stock_position_a[BUFF];
u16 stock_vitesse_a[BUFF];

u16 stock_courant_f[BUFF];
u16 stock_position_f[BUFF];
u16 stock_vitesse_f[BUFF];

volatile u32 index_tab;

void tempo(int ms)
{
		CancelAlarm(0);
		SetRelAlarm (0, ms, T_ORDO) ;
}
TASK(Mesurer)
{

	if (go==1)
	{//mesure accélération			
		stock_position_a[index_tab]=Lire_Position();
    	stock_vitesse_a[index_tab]=Lire_Vitesse();
		stock_courant_a[index_tab]=Lire_courant();

		index_tab++;
   	}
	else if (go==2)
	{ //mesure décélération
		stock_position_f[index_tab]=Lire_Position();
		stock_vitesse_f[index_tab]=Lire_Vitesse();
		stock_courant_f[index_tab]=Lire_courant();
		   
		index_tab++;
	}

	TerminateTask();
}
//FILE __stdin;
TASK(Consigner)
{
int i;
unsigned char c;
	
	if (go==0)	
	{
 		Fixe_Rapport(0);

		printf ("Tapez  1 pour afficher les mesures, 2 pour acquisition avant, 3 pour acquisition arriere\n ");
        c = fgetc(stdin);
		switch(c)
		{
			case '1':
				printf("Mesures en accel/decel\r\nAcc_I;Acc_Pos;Acc_Vit;Dec_I;Dec_Pos;Dec_Vit\n ");   
		    	for(i=0;i<BUFF;i++)
				{
		        	printf("%d;%d;%d;%d;%d;%d\r\n ",stock_courant_a[i] ,stock_position_a[i],stock_vitesse_a[i],stock_courant_f[i] ,stock_position_f[i],stock_vitesse_f[i]);
				}
			
				break ;
			 
	    	case '2':
				printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(TENSION );				
				
				tempo(T_WAIT_MES);
				
		
				break;
						
			case '3':
		    	printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(-TENSION );				
				
				tempo(T_WAIT_MES);

				break;
					
			default : 
		        printf("Vous avez tapé les mauvais choix, boulet!\n ");
		        break;
		}
	}
	else if (go==1)
	{
				index_tab=0;
				go=2;

				Fixe_Rapport(0);				
				
				tempo(T_WAIT_MES);
	}
	else
	{
				go=0;
				printf (" fait\n ");
				
	
	}
	TerminateTask();
}

void InitApp(void)
{
	Init_Periphs();
	setup_usart();
	go = 0;
	index_tab=0;

	Fixe_Rapport(0);
}

int main (void)
{
	InitApp();
	StartOS(OSDEFAULTAPPMODE);
	return 0;	
}

