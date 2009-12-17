/*----------------------------------------------------------------------------
 * Name:    test_usart.c
 * Purpose: USART usage for STM32 with stdio.h
 * Version: V1.00
 *----------------------------------------------------------------------------
 * ACCO [28 Juillet 2009] création et test en simulé sur USART 1,2 et 3 
 *     c'est une dérive du projet boards/keil/stm32/USART_pol
 *  utilise seulement des includes de la microlib.
 * 
 *----------------------------------------------------------------------------*/


#include <stdio.h>

#include "../../lib_cm3/lib_usartx.h"

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

                                 
  setup_usart();

  printf ("Polling mode Serial I/O Example\r\n\r\n");

  while (1) {                                     // Loop forever
    unsigned char c;

    printf ("Press a key. ");
    c = getchar ();
    printf ("\r\n");
    printf ("You pressed '%c'.\r\n\r\n", c);
  } // end while
} // end main
