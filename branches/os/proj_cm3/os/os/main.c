/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: fichier vide pour projet
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32_Init.h"                           // STM32 Initialization
#include "kernel.h"

/* Description des taches */
// Tache 1
TaskType Tache_1_ID;
st_TaskInfo Tache_1_info;
const char Tache_1_nom[] = "Tache_1";
TASK(Tache_1);

// Tache 2
TaskType Tache_2_ID;
st_TaskInfo Tache_2_info;
const char Tache_2_nom[] = "Tache_2";
TASK(Tache_2);

// Event 1 -> declanche la tache 2
st_EventInfo Event1_Info;
EventMaskType Event1_ID;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)
{
	stm32_Init ();                                  // STM32 setup
 
	/* Code du projet a rajouter ici */
	InitOS();

	Tache_1_info.taskname = (char*)Tache_1_nom;
	Tache_1_info.entrypoint = Tache_1;
	Tache_1_info.priority = 1;

	Tache_1_ID = DeclareTask(&Tache_1_info);

	Tache_2_info.taskname = (char*)Tache_2_nom;
	Tache_2_info.entrypoint = Tache_2;
	Tache_2_info.priority = 2;

	Tache_2_ID = DeclareTask(&Tache_2_info);

	Event1_Info.TaskID = Tache_2_ID;
	Event1_ID = DeclareEvent(&Event1_Info);

	StartOS(OSDEFAULTAPPMODE);

	while(1); /* Boucle infinie : un main ne doit jamais rendre la main */

	return 0; 
}

TASK(Tache_1)
{
volatile int i;

	for (i=1; i<5; i++);
}

TASK(Tache_2)
{
volatile int i;
volatile int j;

	for (i=1; i<5; i++)
	{
		WaitEvent(Event1_ID);
		ClearEvent(Event1_ID);

		j=j+i;
	}
}

void BackgroundTask(void)
{
int i, j;

	i=0;
	j=0;
	while (1)
	{
		i = i + (1<<j);
		j++;

		if (j>31) 
		{
			j=0;
			ActivateTask(Tache_1_ID);
		}

		if (i == 0)
		{	
			//ActivateTask(Tache_2_ID);
			SetEvent(Tache_2_ID, Event1_ID); 	
		}
	}
}

