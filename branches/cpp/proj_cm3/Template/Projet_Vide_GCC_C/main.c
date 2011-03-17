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


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

	stm32_Init ();                                  // STM32 setup
 
	/* Code du projet a rajouter ici */
	

	while(1); /* Boucle infinie : un main ne doit jamais rendre la main */

	return 0; 
}
