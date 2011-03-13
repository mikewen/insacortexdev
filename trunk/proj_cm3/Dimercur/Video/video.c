/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * MinOS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * MinOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with MinOS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"
#include "stm_clock.h"
#include "stm_usartx.h"

#define _TAILLE_LIGNE_	80
#define _NB_LIGNE_	25
#define _TAILLE_VIDEO_ _TAILLE_LIGNE_*_NB_LIGNE_

#define _ATTRIB_CLIGNOTANT 0x80
#define _ATTRIB_VIDEO_INVERSE 0x40
#define _ATTRIB_COULEUR_NOIR 0x0
#define _ATTRIB_COULEUR_GRIS 0x1
#define _ATTRIB_COULEUR_ARGENT 0x2
#define _ATTRIB_COULEUR_BLANC 0x3
	
struct ST_VIDEO
{
	unsigned char caractere;
	unsigned char attributs;
} video_array[_TAILLE_VIDEO_];

void Init_Periphs(void)
{
	Init_Clock_System();

	setup_usart();
}

int main (void)
{
int i;

	Init_Periphs();

	/* Remplissage de la table */
	for (i=0; i<_TAILLE_VIDEO_; i++)
	{
		video_array[i].caractere=' ';
		video_array[i].attributs=_ATTRIB_COULEUR_NOIR;
	}

	return 0;
}