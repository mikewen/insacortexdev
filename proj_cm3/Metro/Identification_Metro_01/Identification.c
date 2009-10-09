#include <stm32f10x_lib.h>
#include "STM32_Init.h"

#include "Metro_Librairie_Version2009d.h"
//#include "comm_matlab_2009a.h"
#include <stdio.h> 
#include "RTL.h"

unsigned char go = 0;
OS_TID tsk_mesure, tsk_consigne;

#define T_SAMP 1 
#define T_WAIT 5
#define T_WAIT_MES 150
#define TENSION 3000  
//#define BUFF ((2*T_WAIT_MES + T_WAIT+20)/T_SAMP)
#define BUFF 1000

u16 stock_courant_a[BUFF];
u16 stock_position_a[BUFF];
u16 stock_vitesse_a[BUFF];

u16 stock_courant_f[BUFF];
u16 stock_position_f[BUFF];
u16 stock_vitesse_f[BUFF];

volatile u32 index_tab;

void TIM1_UP_IRQHandler (void)
{
	TIM1->SR = TIM1->SR & ~TIM_FLAG_Update; 

	if (go==1)
	{			
		stock_position_a[index_tab]=(u16)Lire_Position();
    	stock_courant_a[index_tab]=(u16)Lire_courant();
		stock_vitesse_a[index_tab]=(u16)Lire_Vitesse();
		    
		index_tab++;
   	}
	else if (go==2)
	{
		stock_position_f[index_tab]=(u16)Lire_Position();
    	stock_courant_f[index_tab]=(u16)Lire_courant();
		stock_vitesse_f[index_tab]=(u16)Lire_Vitesse();
		    
		index_tab++;
	}


}

__task void consigne()
{
int i;
unsigned char c;
	
	//tsk_mesure = os_tsk_create (mesure, 1);
	
 	Fixe_Rapport(0);

	while(1)
	{
		//printf ("Tapez  1 pour afficher les mesures, 2 la position actuelle , 3 pour avancer:\r\n*");
		printf ("Tapez  1 pour afficher les mesures, 2 pour acquisition avant, 3 pour acquisition arriere:\r\n");
        c = getchar ();
	    
		switch(c)
		{
			case '1':
				printf("Mesures en acceleration\r\nCourant;Position;Vitesse\r\n");   
		    	for(i=0;i<BUFF;i++)
				{
		        	printf("%d;%d;%d\r\n",stock_courant_a[i] ,stock_position_a[i],stock_vitesse_a[i]);
				}

				printf("\r\n\r\nMesures en deceleration\r\nCourant;Position;Vitesse\r\n");   
		    	for(i=0;i<BUFF;i++)
				{
		        	printf("%d;%d;%d\r\n",stock_courant_f[i] ,stock_position_f[i],stock_vitesse_f[i]);
				}
				break ;
			 
	    	case '2':
				printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(TENSION );				
				
				//os_dly_wait(T_WAIT_MES);

				index_tab=0;
				go=2;

				Fixe_Rapport(0);				
				
				//os_dly_wait(T_WAIT_MES);

				go=0;
				printf (" fait\r\n");
				break;
						
			case '3':
		    	printf ("Acquisition ...");

				index_tab=0;
				go=1;

				Fixe_Rapport(-TENSION );				
				
				//os_dly_wait(T_WAIT_MES);

				index_tab=0;
				go=2;

				Fixe_Rapport(0);				
				
				//os_dly_wait(T_WAIT_MES);

				go=0;
				printf (" fait\r\n");
				break;
					
			default : 
		        printf("Vous avez tapé les mauvais choix, boulet! \r\n");
		        break;
		}
	}
}

int main(void)
{
	go = 0;
	index_tab=0;

	stm32_Init();

	Init_Periphs();

	Fixe_Rapport(0);

	//os_sys_init (consigne); 
 	consigne();

  	while(1)
  	{
		// pour tester le receiver de usart
  		/*unsigned char c;

    	printf ("tapez une lettre 1 pour afficher les mesures\r\n*");
    	c = getchar ();
    	printf ("\r\n*");
    	printf ("vous avez tapez '%c'.\r\n\r\n*", c);	
		   
  		*/
	}
}

int fputc(int ch, FILE *f) 
{
	while (!(USART3->SR & USART_FLAG_TXE));

	USART3->DR = ch;

	return ch;
}

int fgetc(FILE *f) 
{
  	while (!(USART3->SR & USART_FLAG_RXNE));

  	return (USART3->DR);
}
