#include <stm32f10x_lib.h>
//#include <stm32Init.h>
#include "Metro_Librairie_Version2009c.h"
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

short int stock_courant[BUFF];
short int stock_position[BUFF];


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
		       i++;
   			}
	}
   	
 }


  __task void consigne()
 {	  int i;
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
		
		  for(i=0;i<BUFF;i++)
		     printf("%d %d \r\n*",stock_courant[i] ,stock_position[i]);
  			  
		//EcrireMatInt ("Pos",BUFF,stock_position);
		while(1);
 }











int main(void)
{

  Init_Periphs();
  printf("Start with position %d *",Lire_Position());
  os_sys_init (consigne); 
   
  while(1)
  {
  }


}
