//OSEK includes
#include "tpl_os.h"
#include "tpl_os_generated_configuration.h"

//PERIPH includes
#include "../../lib_cm3/stm_clock.h"
#include "../../lib_cm3/stm_metro_v1.h"

#include "../../lib_cm3/lib_usartx.h"
#include <stdio.h>



unsigned char go = 0;

#define T_SAMP 10 
#define T_WAIT 50
#define T_WAIT_MES 100
#define TENSION 0x0FFF
//#define BUFF ((2*T_WAIT_MES + T_WAIT+20)/T_SAMP)
#define BUFF 1000

u16 stock_courant_a[BUFF];
u16 stock_position_a[BUFF];
u16 stock_vitesse_a[BUFF];

u16 stock_courant_f[BUFF];
u16 stock_position_f[BUFF];
u16 stock_vitesse_f[BUFF];

volatile u32 index_tab;
#define TMP_CNT ((int)(7000.0/4.0/(0.219099-0.00028363)*0.1*100.0/23.0) )
void tempo(int ms)
{
	int nms,i;
	volatile int j;

	for(nms=0; nms < ms; nms++)
	{
		for(i=0;i<TMP_CNT;i++) j++;
	}
}
TASK(Mesurer)
{

	if (go==1)
	{			
		stock_position_a[index_tab]=Lire_Position();
    	stock_vitesse_a[index_tab]=Lire_Vitesse();
		stock_courant_a[index_tab]=Lire_courant();

		index_tab++;
   	}
	else if (go==2)
	{
		stock_position_f[index_tab]=Lire_Position();
		stock_vitesse_f[index_tab]=Lire_Vitesse();
		stock_courant_f[index_tab]=Lire_courant();
		   
		index_tab++;
	}

	TerminateTask();
}
FILE __stdin;
TASK(Consigner)
{
int i;
unsigned char c;
	
	//tsk_mesure = os_tsk_create (mesure, 1);
	
 	Fixe_Rapport(0);

//	while(1)
	{
		//printf ("Tapez  1 pour afficher les mesures, 2 la position actuelle , 3 pour avancer:\r\n*");
		printf ("Tapez  1 pour afficher les mesures, 2 pour acquisition avant, 3 pour acquisition arriere:\r\n");
    //    c = getchar ();
		c =  fgetc(&__stdin);    
		switch(c)
		{
			case '1':
				printf("Mesures en accel/decel\r\nAcc_I;Acc_Pos;Acc_Vit;Dec_I;Dec_Pos;Dec_Vit\r\n");   
		    	for(i=0;i<BUFF;i++)
				{
		        	printf("%d;%d;%d;%d;%d;%d\r\n",stock_courant_a[i] ,stock_position_a[i],stock_vitesse_a[i],stock_courant_f[i] ,stock_position_f[i],stock_vitesse_f[i]);
				}
			
				break ;
			 
	    	case '2':
				printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(TENSION );				
				
				tempo(T_WAIT_MES);
				
		
				index_tab=0;
				go=2;

				Fixe_Rapport(0);				
				
				tempo(T_WAIT_MES);

				go=0;
				printf (" fait\r\n");
				break;
						
			case '3':
		    	printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(-TENSION );				
				
				tempo(T_WAIT_MES);

				index_tab=0;
				go=2;

				Fixe_Rapport(0);				
				
				tempo(T_WAIT_MES);

				go=0;
				printf (" fait\r\n");
				break;
					
			default : 
		        printf("Vous avez tapé les mauvais choix, boulet! \r\n");
		        break;
		}
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

