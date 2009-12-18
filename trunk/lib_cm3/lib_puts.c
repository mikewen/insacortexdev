/*----------------------------------------------------------------------------
 * Name:    puts.c
 * Purpose: fonction puts sur liaison serie
 * Version: V1.00
 *----------------------------------------------------------------------------
 * SDM [28 Juillet 2009] création et test en simulé sur USART 2 
 *     testé sous GCC (aucun interet sur ARMCC)
 *----------------------------------------------------------------------------*/

#include "stdio.h"				// import FILE type
FILE __stdout ;

int	puts(const char *__str)
{
int i;

	i=0;
	
	while (__str[i]!=0)
	{
		fputc((int)__str[i],& __stdout);
		i++;
	}
	
	return i;
}