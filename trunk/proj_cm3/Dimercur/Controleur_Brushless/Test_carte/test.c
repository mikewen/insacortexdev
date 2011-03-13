#include <stm32f10x_lib.h>
#include "STM32_Init.h"
#include <stdio.h>

int main (void)
{
	stm32_Init();

	GPIOA->ODR=(1<<0)+(1<<2)+(1<<3);
	GPIOC->ODR=0;

	for (;;);
}
