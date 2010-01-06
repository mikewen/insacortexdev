/*--------------------------Blink leds function --------------------*/

#ifdef USE_BLINK_LEDS


void Blink_Leds(int repet, int duree)
{
	volatile unsigned int i,j;
	volatile int dummy;

	for(j=repet;j>0;j--)
 	{ 
	   	GPIOC->ODR &=~(LED_AVANCE);	       //LED AVANCE ON
	  	GPIOC->ODR &=~(LED_RECULE);			  //LED ARRIERE ON

	  	for (i=duree;i>0;i--)
	 	{
			dummy++;
		}

	  	GPIOC->ODR |= (LED_AVANCE); 		   //LED ARRIERE OFF
	  	GPIOC->ODR |= (LED_RECULE); 		  //LED AVANCE OFF	 

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
		Fixe_Rapport(0);
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
		Fixe_Rapport(0);
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
		Fixe_Rapport(0);
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
		Fixe_Rapport(0);
		while(1)
		{
	    	Blink_Leds(10,DUREE_RAPIDE);
    		Blink_Leds(1,DUREE_LENTE);

		}
	#endif 
 } 
 #endif
