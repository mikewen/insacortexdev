#include <stm32f10x_lib.h>
//#include <stm32Init.h>
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
#define BUFF ((4*T_WAIT_MES + T_WAIT+20)/T_SAMP)

 int stock_courant[BUFF];
 int stock_position[BUFF];
 int stock_vitesse[BUFF];

 __task void mesure()
 {	static int i=0;
 			
	os_itv_set (T_SAMP);
	while(1)
	{	
		i=0;
		while	(go)
			 {
				os_itv_wait();
				stock_position[i]=Lire_Position();
    			stock_courant[i]=Lire_courant();
				stock_vitesse[i]=Lire_Vitesse();
		       i++;
   			}
	}
   	
 }


  __task void consigne()
 {	  int i;
      unsigned char c;
	tsk_mesure = os_tsk_create (mesure, 1);
	    Fixe_Rapport(0);
	    os_dly_wait(3*T_WAIT);
 	
 		// set on
  		//os_mut_init (go);
		go=1;
	    os_dly_wait(T_WAIT);
 	 	Fixe_Rapport(TENSION );
		os_dly_wait(T_WAIT_MES);

		
		//set off
   	 	Fixe_Rapport(0);
		os_dly_wait(T_WAIT_MES);

		//set reverse
   	 	Fixe_Rapport(-TENSION );
		os_dly_wait(T_WAIT_MES);

		//send values
		//os_mut_release (go);
			
		Fixe_Rapport(0);
		os_dly_wait(T_WAIT_MES);
			go=0;
	
	
		//EcrireMatInt ("Pos",BUFF,stock_position);
		while(1){
		
		
		printf ("tapez  1 pour afficher les mesures, 2 la position actuelle , 3 pour avancer:\r\n*");
        c = getchar ();
	    switch(c)
		       {
	
	  
		case'1' :  { 
		            for(i=0;i<BUFF;i++)
		            printf("courant:%d position:%d vitesse:%d \r\n*",stock_courant[i] ,stock_position[i],stock_vitesse[i]);
					break ;
             
			 		}
			 
	    case '2': {
		           	 printf("%d\r\n*",Lire_Position());
				     break;
				   }
		
		case '3': {
		           	 Fixe_Rapport(TENSION );
				     break;
				   }
	
		
		case '5': {
		           	 Fixe_Rapport(-TENSION );
				     break;
				   }
		  
		 
		 case '4': {
		           	 Fixe_Rapport(0);
				     break;
				   } 
		 
		 default : { 
		             printf("vous avez tapez les mauvais choix\r\n*");
		             break;
					 } 
		
              }

     	}
 }











int main(void)
{
  
  Init_Periphs();
  os_sys_init (consigne); 
 
 
  while(1)
  {// pour tester le receiver de usart
  /*unsigned char c;

    printf ("tapez une lettre 1 pour afficher les mesures\r\n*");
    c = getchar ();
    printf ("\r\n*");
    printf ("vous avez tapez '%c'.\r\n\r\n*", c);	
		   
  	*/
  

  }


}
