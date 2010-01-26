#include "stm32f10x_type.h"
#include "stm32f10x_lib.h"
#include "config.h"
#include "kernel.h"

#include "task.h"
#include "alarm.h"
#include "interrupts.h"
#include "resource.h"
#include "internals.h"

#ifdef __WITH_EVENTS__
#include "events.h"
#endif /* __WITH_EVENTS__ */

u32	volatile CPUID;

void Stack_Swap(void);

AppModeType	GetActiveApplicationMode(void)
{
	return OSDEFAULTAPPMODE;
}

void StartOS(AppModeType Mode)
{
	CPUID = SCB->CPUID;

	/* Mode n'est pas utilisé pour l'instant */
	/* Initialisation des taches */
	Task_Init();

	/* Initialisation des resources */
	Resource_Init();

	/* Initialisation des alarms */
	Alarm_Init();

	/* Initialisation des interruptions */
	Interrupt_Init();

#ifdef __WITH_EVENTS__
	/* Initialisation des events */
	Events_Init();
#endif /* __WITH_EVENTS__ */

	/* Initialisation des ressources internes */
	Internals_Init();

	/* Change la stack 
	   Avant: SP = MSP, PSP = 0
	   Apres: SP = PSP = ancienne MSP, MSP = OS_Stack
	*/

	Stack_Swap();
}

void ShutdownOS(StatusType Error)
{
}


