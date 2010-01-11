/*----------------------------------------------------------------------------
 * Name:    getc.c
 * Purpose: fonction getc sur liaison serie
 * Version: V1.00
 *----------------------------------------------------------------------------
 * SDM [28 Juillet 2009] création et test en simulé sur USART 2 
 *     testé sous GCC (aucun interet sur ARMCC)
 *----------------------------------------------------------------------------*/

#include "stdio.h"				// import FILE type
#include <stdarg.h>
FILE __stdout;
FILE __stdin;

#if defined __MINILIB__
#error this file is obsolete: use minilib instead
#endif /* __MINILIB__ */

/* Remarque: le fichier lib_cm3.h fait une redefinition de getchar en lgetchar */
int getc(FILE *f)
{
	return (fgetc(__stdin));
} 