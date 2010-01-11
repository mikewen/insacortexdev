/*
________________________________________________________________________________________
	STM32 peripherals lib for use with INSA subway project
	stm_metro_v1.h
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. stm_metro_v1
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
	+ Mesure courant ADC avec DMA 
	+ config fréaquence mesure de courant
	Améliorer la fréquence de mesure de la vitesse pour les basse vitesses
______________________________________________________________________________________
*/

#ifndef __STM_METRO_CONF
#define __STM_METRO_CONF
#include "stm_clock.h"

//COMMENT line below to remove ADC code and current measure functions
#define USE_ADC

//COMMENT line below to remove T3 code and position measure functions
#define USE_POSITION

//COMMENT line below to remove T2 code and speeed measure functions
#define USE_SPEED
#define SPEED_TIC_FREQ  (2000UL*100UL)//(2000UL*100UL)

//COMMENT line below to remove PWM code and functions
#define USE_PWM 
#ifdef USE_PWM 
	//DO NOT EDIT LED_ARRIERE and LED_AVANT
	#define LED_ARRIERE (1<<7)  //Led sur le PortC.7 (arrière de la loco)							   
	#define LED_AVANT (1<<9)	//Led sur le PortC.9 (avant de la loco)	

	//CONFIGURE  PWM Frequency should divide HXCLK to be precise
	#define PWM_FREQ (9765UL)//(4882UL)

	//CONFIGURE  which Led to light while going forward or backward
	#define LED_AVANCE (LED_ARRIERE) //Led allumée pendant l'avance							   
	#define LED_RECULE (LED_AVANT)	 //Led allumée pendant une marche arrière
#endif

//COMMENT line below to disable Leds Binking after periph are initialized
//#define BLINK_ON_START
#ifdef BLINK_ON_START
	#define USE_BLINK_LEDS
	#define BLINK_NBR (2)
#endif


#endif //STM_METRO_CONF
