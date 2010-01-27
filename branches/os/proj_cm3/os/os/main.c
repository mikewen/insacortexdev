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

TaskType Tache_1_ID;

st_TaskInfo Tache_1_info;
const char Tache_1_nom[] = "Tache_1";

/* Prototype des taches */
TASK(Tache_1);

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
	Tache_1_info.type = 0;

	Tache_1_ID = DeclareTask(&Tache_1_info);

	StartOS(OSDEFAULTAPPMODE);

	while(1); /* Boucle infinie : un main ne doit jamais rendre la main */

	return 0; 
}

TASK(Tache_1)
{
volatile int i;

	for (i=1; i<5; i++);
}
