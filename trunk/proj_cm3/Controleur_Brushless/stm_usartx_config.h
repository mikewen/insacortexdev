/*
________________________________________________________________________________________
	configuration for usart driver
	usart_config[_TEMMPLATE].h
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
	-	tester en réel
		+ 9600 bauds OK avec zeegbee
		- 19200 bauds marche pas ! 
		   sur l'hyperterm à 9600 passage de zegbee à 19200 avec
		   +++[CR] réponse OK
		   ATBD4[CR] réponse OK
		   ATCN[CR]
		   ouverture de l'hterm à 19200 bauds
		   c'est bien 19200 car  +++[CR] répondes OK ! (je fais ATCN[CR] pour sortir)
		   compil du projet avec la lib à 19200 bauds + tests => CARACTERES BIZARRES
		   sans doute un problème de data rate...
	- 	voir la compatibilité avec le noyau TR
	-   clarifier l'histoire du //#pragma import(__use_no_semihosting_swi) 
_____________________________________________________________________________________
*/
#ifndef __USARTx_CONFIG__
#define __USARTx_CONFIG__

#include "stm_clock.h"

//CONFIGURE
#define __USART1_REMAP 0x0
// 0 ->    USART1 TX/RX on PA9/PA10
// 0x04 -> USART1  TX/RX on PB6/PB7

//CONFIGURE
#define __USART2_REMAP 0x0
// 0 ->    USART2 TX/RX on PA2/PA3
// 0x08 -> USART2 TX/RX on PD5/PD6

//CONFIGURE
#define __USART3_REMAP 0
// 0 ->      USART3 TX/RX on PB10/PB11
// 0x10 ->   USART3 TX/RX on PC10/PC11
// others -> USART1 TX/RX on PD8/PD9

// Select the USART to use
//CONFIGURE
#define USART_NUM	1   // choose to use USART1 USART2 USART3


//CONFIGURE baudrate among following values
#define BAUDRATEx   9600      // OK avec zigbee
//#define BAUDRATEx   14400 
//#define BAUDRATEx   19200 
//#define BAUDRATEx   28800 
//#define BAUDRATEx   38400 
//#define BAUDRATEx   56000 
//#define BAUDRATEx   57600 
//#define BAUDRATEx   115200 

//choose USART mode to use and configure it
//CONFIGURE
//#define USART_POL
#define USART_DMA
//#define USART_IRQ

//what should we do while wainting ?
//#define USART_WAIT  os_wait();

//CONFIGURE if you use GCC with MiniLib
// or comment to use standard ARMcc microlib
#define __MINILIB__

#endif /* __USARTX_CONFIG__ */

