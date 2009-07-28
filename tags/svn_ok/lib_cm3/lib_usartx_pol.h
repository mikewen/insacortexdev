/*----------------------------------------------------------------------------
 * Name:    lib_usartx_pol
 * Purpose: USART usage for STM32 with stdio.h
 *  
 * Version: V1.00
 *----------------------------------------------------------------------------
 * ACCO [28 Juillet 2009] création et test en simulé sur USART 1,2 et 3 
 *     c'est une dérive du projet boards/keil/stm32/USART_pol
 *  utilise seulement des includes de la microlib.
 * 
 *----------------------------------------------------------------------------
  USAGE :
  	1\ include the lib_usartx_pol.c in project
	2\ include the lib_usartx_pol.h in main program
	3\ edit lib_usartx_pol.h to config USART to use and BAUDRATE
	4\ call usartx_setup()	 before  any use of stdlib functions !

 	5\ when running connect terminal with 
		matching BAUDRATE
	 	8bits DATA
		1bit Stop
		NO PARITY check
		NO flow control
	______________________________________________________________________
	TODO :
	-	tester en réel
	- 	voir la compatibilité avec le noyau TR
	-   clarifier l'histoire du //#pragma import(__use_no_semihosting_swi) 
 */



// USART config
//#define NUM_USART	1   // choose to use USART1
#define NUM_USART	2   // choose to use USART2
//#define NUM_USART	3   // choose to use USART3

//choose baudrate among following values
//#define BAUDRATEx   9600 
//#define BAUDRATEx   14400 
//#define BAUDRATEx   19200 
//#define BAUDRATEx   28800 
//#define BAUDRATEx   38400 
//#define BAUDRATEx   56000 
//#define BAUDRATEx   57600 
#define BAUDRATEx   115200 

void usartx_setup(void);
