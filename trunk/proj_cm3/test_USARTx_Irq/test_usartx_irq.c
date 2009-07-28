/*----------------------------------------------------------------------------
 * Name:    test_usartx_irq.c
 * Purpose: USART usage for STM32 with stdio.h (interruptive)
 *----------------------------------------------------------------------------
  *----------------------------------------------------------------------------*/


#include <stdio.h>
                       // STM32 Initialization
#include "../../lib_cm3/lib_usartx_irq.h"


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

 
  usartx_setup();                                  // init RX / TX buffers

  printf ("Interrupt driven Serial I/O Example\r\n\r\n");

  while (1) {                                     // Loop forever
    unsigned char c;

    printf ("Press a key. ");
    c = getchar ();
    printf ("\r\n");
    printf ("You pressed '%c'.\r\n\r\n", c);
  } // end while
} // end main
