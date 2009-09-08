/*----------------------------------------------------------------------------
 * Name:    lib_usartx_pol
 * Purpose: USART usage for STM32 with stdio.h
 *  
 * Version: V1.00
 *----------------------------------------------------------------------------
 * ACCO [28 Juillet 2009] création et test en simulé sur USART 1,2 et 3 
 *     c'est une dérive du projet boards/keil/stm32/USART_pol
 *  utilise seulement des includes de la microlib.
 * ACCO 
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
 */
#ifndef __LIB_USARTx
#define __LIB_USARTx

#define __USART1_REMAP 0x04
// 0 ->    USART1 TX/RX on PA9/PA10
// 0x04 -> USART1  TX/RX on PB6/PB7
#define __USART2_REMAP 0
// 0 ->    USART2 TX/RX on PA2/PA3
// 0x08 -> USART2 TX/RX on PD5/PD6
#define __USART3_REMAP 0
// 0 ->      USART3 TX/RX on PB10/PB11
// 0x10 ->   USART3 TX/RX on PC10/PC11
// others -> USART1 TX/RX on PD8/PD9

// Select the USART to use
#define USART_NUM	1   // choose to use USART1
//#define USART_NUM	2   // choose to use USART2
//#define USART_NUM	3   // choose to use USART3

//choose baudrate among following values
#define BAUDRATEx   9600      // OK avec zigbee
//#define BAUDRATEx   14400 
//#define BAUDRATEx   19200 
//#define BAUDRATEx   28800 
//#define BAUDRATEx   38400 
//#define BAUDRATEx   56000 
//#define BAUDRATEx   57600 
//#define BAUDRATEx   115200 

//choose USART mode to use and configure it
#define USART_POL
#ifdef USART_POL
#endif

//#define USART_IRQ
#ifdef USART_IRQ
	#ifdef USART_POL
		#warning "USART_POL and USART_IRQ mode used in the same time !! choose IRQ one"
		#undef USART_POL
	#endif
	#define RBUF_SIZE 	(16)
	#define TBUF_SIZE  (16)

#endif

//#define USART_DMA
#ifdef USART_DMA
	#define RBUF_SIZE 	(16)
	#define TBUF_SIZE  (16)

	#ifdef USART_POL
		#warning "USART_POL and USART_DMA mode used in the same time !! choose DMA one"
		#undef USART_POL
	#endif
	#ifdef USART_IRQ
		#warning "USART_IRQ and USART_DMA mode used in the same time !! choose DMA one"
		#undef USART_IRQ
	#endif

#endif
//what should we do while wainting ?
//#define USART_WAIT  os_wait();

#endif

void setup_usart(void);
