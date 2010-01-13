#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "led.h"

#define MASK	0xFF00

void led_Init(void)
{
	GPIOB->ODR = GPIOB->ODR & (~MASK); 
}

void led_SetValue(u8 val)
{
	GPIOB->ODR = GPIOB->ODR & (~MASK); // Remet les led à zero
	GPIOB->ODR = GPIOB->ODR | (val<<8); // Ecrit val sur PB8-> PB15
}

