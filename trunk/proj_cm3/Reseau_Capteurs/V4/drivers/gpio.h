#ifndef __GPIO_H__
#define __GPIO_H__

#include <stm32f10x_lib.h>

#define PORT_PA8 0
#define PORT_PA12 1
#define PORT_PA14 2

#define BOUTON_TAMP 3
#define BOUTON_WKUP 4

u16 GPIOGetState (u8 port);
int GPIOEtatBouton (int bouton);

#endif /* __GPIO_H__ */
