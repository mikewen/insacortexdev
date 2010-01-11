/*
 * This file is part of minilib c project
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 ****************************************************
 *
 * File: test_stdio.c
 * Use : test bench for stdio functions
 *
 ****************************************************
 *
 * S. DI MERCURIO: 01-10-10 : first release
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

volatile int res;
char *buffer[10];
char *ptr;

int test_stdio (void)
{
/* standard ISO C 99 */

	res = fgetc(stdin);
	ptr = fgets(buffer, 1, stdin);
	res = fputc ('c', stdout);
	res = fputs (buffer, stdout);
	res = getchar();
	res = printf ("Hello world !");
	res = putchar ('c');
	res = sscanf ("", "");
/* minilib specific */
	
	
	return res;
}
