#include <stm32f10x_lib.h>
#include "gpio.h"

u16 GPIOGetState (u8 port)
{
u16 state =0;

	switch (port)
	{
	case PORT_PA8:
		state = (GPIOA->IDR & GPIO_Pin_8);
		break;
	case PORT_PA12:
		state = (GPIOA->IDR & GPIO_Pin_12);
		break;
	case PORT_PA14:
		state = (GPIOA->IDR & GPIO_Pin_14);
		break;
	}	

	return state;
}
