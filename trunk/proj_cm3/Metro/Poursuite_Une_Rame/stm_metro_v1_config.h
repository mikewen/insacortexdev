#ifndef __STM_METRO_CONF
#define __STM_METRO_CONF
#include "stm_clock.h"

//COMMENT line below to remove ADC code and current measure functions
//#define USE_ADC

//COMMENT line below to remove T3 code and position measure functions
#define USE_POSITION

//COMMENT line below to remove T2 code and speeed measure functions
#define USE_SPEED
#define SPEED_TIC_FREQ  (2000UL*100UL)//(2000UL*100UL)

//COMMENT line below to remove PWM code and functions
#define USE_PWM 
#ifdef USE_PWM 
	#define PWM_FREQ (9765UL)//(4882UL)
#endif

//COMMENT line below to disable Leds Binking after periph are initialized
//#define BLINK_ON_START
#ifdef BLINK_ON_START
	#define USE_BLINK_LEDS
	#define BLINK_NBR (2)
#endif

//__________________________________________________
// FAULT handler
// Select if 
//error is handled by this lib :
//  stop PWM
//  then alternate fast blink on LEDS
//  4 slow blinks for Hardware Fault
//  3 slow blinks for Memmanage Fault
//  2 slow blinks for Usage Fault
//  1 slow blink for Bus Fault
//error is hooked to your custom function
//error is not handled
  
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

#endif //STM_METRO_CONF
