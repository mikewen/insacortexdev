/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of INSA OS.
 *
 * INSA OS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * INSA OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with INSA OS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */
 
#include "stm32f10x_type.h"
#include "stm32f10x_lib.h"
#include "os_config.h"
#include "kernel.h"

#include "task.h"
#include "alarm.h"
#include "interrupts.h"
#include "resource.h"
#include "common.h"

#include "scheduler.h"

#ifdef __WITH_EVENTS__
#include "events.h"
#endif /* __WITH_EVENTS__ */

AppModeType	GetActiveApplicationMode(void)
{
	return OSDEFAULTAPPMODE;
}

void InitOS(void)
{
	/* Initialisation des ressources communes */
	Common_Init();

	/* Initialisation des taches */
	Task_Init();

	/* Initialisation des resources */
	//Resource_Init();

	/* Initialisation des alarms */
	//Alarm_Init();

	/* Initialisation des interruptions */
	//Interrupt_Init();

#ifdef __WITH_EVENTS__
	/* Initialisation des events */
	Events_Init();
#endif /* __WITH_EVENTS__ */
}

void StartOS(AppModeType Mode)
{
	/* Mode n'est pas utilis� pour l'instant */

	Schedule(); /* Appel de la routine asm qui va lancer la premiere tache */
}

void ShutdownOS(StatusType Error)
{
}

