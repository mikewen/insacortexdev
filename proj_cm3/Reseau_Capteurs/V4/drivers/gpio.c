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
	case BOUTON_TAMP:
		state = (GPIOC->IDR & GPIO_Pin_13);
		break;
	case BOUTON_WKUP:
		state = (GPIOA->IDR & GPIO_Pin_0);
		break;
	}	

	return state;
}

int GPIOEtatBouton (int bouton)
{
int etat;

	return etat;
}
