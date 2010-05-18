#include "stm_regs.h"
#include "stm_clock.h"
#include "stm_usartx_v2.h"

volatile int i;

int main (void)
{
	Init_Clock_System();

	i=0;

	while (1)
	{
		i = i+1;
	}
}