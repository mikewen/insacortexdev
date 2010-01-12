#include <stm32f10x_lib.h>
#include "led.h"

void Toggle_Led (int led)
{
	GPIOB->ODR = GPIOB->ODR ^ (1<<led);
}

