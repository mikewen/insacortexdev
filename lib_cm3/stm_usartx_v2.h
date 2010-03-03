/*----------------------------------------------------------------------------
 * Name:    lib_usartx
 * Purpose: USART usage for STM32 with stdio.h
 *  
 * Version: V1.00
 *----------------------------------------------------------------------------
 * ACCO [28 Juillet 2009] création et test en simulé sur USART 1,2 et 3 
 *     c'est une dérive du projet boards/keil/stm32/USART_pol
 *  utilise seulement des includes de la microlib ou minilib.
 * ACCO 
 *
 * SDM
 * Si les fichiers lib_usartx.*  sont compilé avec le switch __MINILIB__ defini,
 * le driver exporte les fonctions usart_write et usart_read qui peuvent etre utilisées directement ou appelées
 * dans les fonctions _read et_write de syscalls.c (minilib) -> Le projet est compilé pour la minilib
 * Si le switch n'est pas actif, le driver exporte les focntions _fgetc et _fputc, utilisé par la
 *  microlib de keil.
 *----------------------------------------------------------------------------
  USAGE :
  	1\ include the lib_usartx.c in project
	2\ include the lib_usartx.h in main program
	3\ copy usartx_config_template.h in your project, rename is usartx_config.h
	4\ edit usartx_config.h to config USART to use and BAUDRATE
	5\ call setup_usart()	 before  any use of stdlib functions !

 	6\ when running connect terminal with 
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
		   sur l'hyperterm à 9600 passage de zigbee à 19200 avec
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

#include "stm_clock.h"
#include "stm_usartx_v2_config.h"

#include <stdio.h>
/*----------------------------------------------------------------------------
  external functions
 *----------------------------------------------------------------------------*/

#if !defined __MINILIB__
	struct __FILE {
	  int handle;                 // Add whatever you need here 
	};

	int fputc(int ch, FILE *f);
	int fgetc(FILE *f);

#else
	#include "missing_defs.h"

	int usart_write(int ch);
	int usart_read(void);
#endif /* !defined __MINILIB__ */


#ifdef USART_POL
#endif

#ifdef USART_IRQ
	#ifdef USART_POL
		#warning "USART_POL and USART_IRQ mode used in the same time !! choose IRQ one"
		#undef USART_POL
	#endif
	#define RBUF_SIZE 	(16)
	#define TBUF_SIZE  (16)

#endif

#ifdef USART_DMA
	#define RBUF_SIZE 	(5)
	#define TBUF_SIZE  (500)

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

void setup_usart(void);

#endif /* __LIB_USARTx */
