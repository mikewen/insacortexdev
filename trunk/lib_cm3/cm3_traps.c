/*
________________________________________________________________________________________
	Cortex M3 Fault Handling
	cm3_traps.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. cm3_traps
	RELPATH is the relative path from your projetc directory to lib_cm3 directory	
	
   	include RELPATH/lib_cm3/???.c file in your makefile
	copy    RELPATH/lib_cm3/???_config_TEMPLATE.h in your project directory	(noted ./)
	rename  ./???_config_TEMPLATE.h as ./???_config.h in your project directory
	edit    ./???_config.h file for your project (lines with //CONF tags)  
	ensure that ./ path is visible in CC compiler options 
	add "#include "RELPATH/lib_cm3/???.h" in app. code 
	add a call to "Init_???();" at initialisation step of your APP
________________________________________________________________________________________
REVS
	[Acco 06/01/2010] finalisation et commentaires de la première version
_______________________________________________________________________________________
TODO
	declare proper OLYMEX MASK
	check LED_PORT conf with warnings
	add Init_Traps to ensure Exception is enable 
	And GPIO LED clock is enabled
________________________________________________________________________________________
*/

#include <stm32f10x_lib.h>
#include "cm3_traps.h"


/*--------------------------Blink leds function --------------------*/

#ifdef USE_BLINK_LEDS

void Blink_Leds(int repet, int duree)
{
	volatile unsigned int i,j;
	volatile int dummy;

	for(j=repet;j>0;j--)
 	{ 
	   	LED_PORT->ODR &=~(LED_MASK);	  //LED ON

	  	for (i=duree;i>0;i--)
	 	{
			dummy++;
		}

	  	LED_PORT->ODR |= (LED_MASK); 		   //LED  OFF

	  	for (i=duree;i>0;i--)
	 	{
			dummy--;
		}
 	}
}
#endif 

/*---------------------- Hardware Fault Handle ------------------*/

 #ifdef HANDLE_HARDWARE_FAULT

 void HardFault_Handler(void)
 {
 	#ifdef USER_HARDWARE_FAULT_HANDLER
		 HARDWARE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(4,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 /*---------------------- Memmanage Fault Handle ------------------*/

 #ifdef HANDLE_MEMMANAGE_FAULT

 void MemManage_Handler(void)
 {
 	#ifdef USER_MEMMANAGE_FAULT_HANDLER
		 MEMMANAGE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(3,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 /*---------------------- Usage Fault Handle ------------------*/

 #ifdef HANDLE_USAGE_FAULT

 void UsageFault_Handler(void)
 {
 	#ifdef USER_USAGE_FAULT_HANDLER
		 USAGE_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(2,DUREE_LENTE);

		}
	#endif 
 } 
 #endif

 
 /*---------------------- BUS Fault Handle ------------------*/

 #ifdef HANDLE_BUS_FAULT

 void BusFault_Handler(void)
 {
 	#ifdef USER_BUS_FAULT_HANDLER
		 BUS_FAULT_FUNCTION ;		
	#else
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(1,DUREE_LENTE);

		}
	#endif 
 } 
 #endif
