

#include <stm32f10x_lib.h>
#include "stm_nvic.h"

void NVIC_INIT(void)
{
	NVIC_SET_PRIO(WWDG,5);
	NVIC_SET_PRIO(USBWAKEUP,6);
}