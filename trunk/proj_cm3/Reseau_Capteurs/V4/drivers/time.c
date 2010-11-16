#include <stm32f10x_lib.h>
#include "time.h"

TIME_CALLBACK TIMECallback = 0x00;
u32 TIMECounter;

void TIMEInit (TIME_CALLBACK callback)
{
	TIMECallback = callback;
	TIMECounter = 0;
}

u32 TIMEGetCounter(void)
{
	return TIMECounter;
}

void TIMEResetCounter(void)
{
	 TIMECounter = 0;
}

void SysTick_Handler(void)
{
  	TIMECounter++;

	if (TIMECallback != 0x0)
	{
		TIMECallback();
	}
}
