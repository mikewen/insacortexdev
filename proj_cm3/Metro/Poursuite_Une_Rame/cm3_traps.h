/*
________________________________________________________________________________________
	Cortex M3 Fault Handling
	cm3_traps_config[_TEMMPLATE].h
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
________________________________________________________________________________________
*/

//__________________________________________________
// FAULT handler
// Select IF error is handled by this lib :
//  alternate fast blink on LEDS with
//  4 slow blinks for Hardware Fault
//  3 slow blinks for Memmanage Fault
//  2 slow blinks for Usage Fault
//  1 slow blink for Bus Fault
// OR IF error is hooked to your custom function
// OR IF error is not handled
  
//COMMENT line below to disable this Fault Handling
#define HANDLE_HARDWARE_FAULT
#ifdef 	HANDLE_HARDWARE_FAULT

	#define USER_HARDWARE_FAULT_HANDLER
	#ifdef USER_HARDWARE_FAULT_HANDLER
		extern void Arret_Urgence();
		#define HARDWARE_FAULT_FUNCTION Arret_Urgence();
	#endif

	#ifndef USE_BLINK_LEDS
		#define USE_BLINK_LEDS
	#endif
#endif

//COMMENT line below to disable this Fault Handling
//#define HANDLE_MEMMANAGE_FAULT
#ifdef 	HANDLE_MEMMANAGE_FAULT

	//COMMENT line below to use your own Fault Function
	#define USER_MEMMANAGE_FAULT_HANDLER
	#ifdef USER_MEMMANAGE_FAULT_HANDLER
		extern void Arret_Urgence();
		#define MEMMANAGE_FAULT_FUNCTION Arret_Urgence();
	#endif

	#ifndef USE_BLINK_LEDS
		#define USE_BLINK_LEDS
	#endif
#endif

//COMMENT line below to disable this Fault Handling
//#define HANDLE_USAGE_FAULT
#ifdef 	HANDLE_USAGE_FAULT

	//COMMENT line below to use your own Fault Function
	#define USER_USAGE_FAULT_HANDLER
	#ifdef USER_USAGE_FAULT_HANDLER
		extern void Arret_Urgence();
		#define USAGE_FAULT_FUNCTION Arret_Urgence();
	#endif

	#ifndef USE_BLINK_LEDS
		#define USE_BLINK_LEDS
	#endif
#endif


//COMMENT line below to disable this Fault Handling
//#define HANDLE_BUS_FAULT
#ifdef 	HANDLE_BUS_FAULT

	//COMMENT line below to use your own Fault Function
	#define USER_BUS_FAULT_HANDLER
	#ifdef USER_BUS_FAULT_HANDLER
		extern void Arret_Urgence();
		#define BUS_FAULT_FUNCTION Arret_Urgence();
	#endif

	#ifndef USE_BLINK_LEDS
		#define USE_BLINK_LEDS
	#endif
#endif
