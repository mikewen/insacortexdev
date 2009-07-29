/*----------------------------------------------------------------------------
 * Name:    printf.c
 * Purpose: fonction printf sur liaison serie
 * Version: V1.00
 *----------------------------------------------------------------------------
 * SDM [28 Juillet 2009] création et test en simulé sur USART 2 
 *     testé sous GCC (aucun interet sur ARMCC)
 *----------------------------------------------------------------------------*/


#include "stdio.h"				// import FILE type
#include <stdarg.h>

void printint(int i)
{
int digit;
	
	digit = i % 10;
	digit = digit + '0';
	i = i / 10;
	fputc(i, NULL);
}

void printstring(char *s)
{
int i;

	i=0;
	while (s[i]!= 0x0)
	{
		fputc((int)s[i], NULL);
		i++;
	}
}

void printhex(int i)
{
}

int	printf(const char *fmt, ...)
{
char *p;
va_list argp;
int i;
char *s;

	va_start(argp, fmt);

	for(p = (char*)fmt; *p != '\0'; p++)
	{
		if(*p != '%')
		{
			fputc((int)*p, NULL);
			continue;
		}

		switch(*++p)
		{
		case 'c':
			i = va_arg(argp, int);
			fputc(i, NULL);
			break;

		case 'd':
			i = va_arg(argp, int);
			printint(i);
			break;

		case 's':
			s = va_arg(argp, char *);
			printstring(s);
			break;

		case 'x':
			i = va_arg(argp, int);
			printhex(i);
		break;

		case '%':
			fputc('%', NULL);
			break;
		}
	}

	va_end(argp);

	return 0;
}



