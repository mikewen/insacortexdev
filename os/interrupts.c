#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "interrupts.h"

u8 InterruptCounter;

/* Interrupt services declaration */
void EnableAllInterrupts(void)
{
}

void DisableAllInterrupts(void)
{
}

void ResumeAllInterrupts(void)
{
}

void SuspendAllInterrupts(void)
{
}

void ResumeOSInterrupts(void)
{
}

void SuspendOSInterrupts(void)
{
}

void Interrupt_Init(void)
{
	DisableAllInterrupts();

	InterruptCounter=1;
}
