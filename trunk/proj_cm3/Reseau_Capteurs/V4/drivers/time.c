#include <stm32f10x_lib.h>
#include "time.h"

TIME_CALLBACK TIMECallback = 0x00;
u32 TIMECounter;
int TIMEEnabled;

void TIMEInit (TIME_CALLBACK callback)
{
	TIMECallback = callback;
	TIMECounter = 0;
	TIMEEnabled =0;
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
	if (TIMEEnabled)
	{
	  	TIMECounter++;
	
		if (TIMECallback != 0x0)
		{
			TIMECallback();
		}
	}
}

void TIMEWait1ms(void)
{
#define _MAX_1MS_ 9000L
volatile unsigned int i;

	for (i=_MAX_1MS_; i!=0; i--); 
}

void TIMEWaitxms(int ms)
{
volatile int cpt;

	for (cpt=0; cpt<ms; cpt++)
	{
		TIMEWait1ms();	
	}
}

