#include <stdio.h>
#include "stm_clock.h"
#include "lib_usartx.h"

struct __FILE {
  int handle;                 // Add whatever you need here 
};
FILE __stdout;
FILE __stdin;

int main (void)
{
char c;

	Setup_Clock_System();
	setup_usart();

	printf ("toto\n");
	
	c= fgetc(&__stdin);

	printf ("%c\n", c);
	for (;;);
}
