#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                            // missing bit definitions

volatile u8 i;

void TIM1_UP_IRQHandler (void)
{
	i = i + 1;
}

int main (void)
{
	stm32_Init ();

	while (1);
}
