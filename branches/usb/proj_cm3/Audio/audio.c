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
#include "pwm.h"
#include "voice.h"
#include "adc.h"

void Callback_pwm(void);

int compteur;
int note;
int octave;

#define BPM 144
#define COMPTEUR_MAX 104
int index_musique;
#define MUSIQUE_MAX 16*24

typedef struct
{
	u8 note;
	u8 octave;
} ST_NOTE;

ST_NOTE partition[MUSIQUE_MAX][4]=
{
	{{_SI_,1},			{_MI_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 1 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_SI_,1},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_SOL_,1},			{_SI_,1},			{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	

	{{_MI_,1},			{_LA_,1},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 2 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_DO_,2},			{_MI_,2},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_SOL_D_,1},		{_SI_,1},			{_SOL_D_,0}, 		{_VOICE_EMPTY_,0}}, /* Temps 3 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SOL_D_,1},		{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_SOL_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_SOL_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_MI_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 4 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_RE_,1}, 			{_VOICE_EMPTY_,0}}, /* Temps 5 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_FA_,1},			{_RE_,2},			{_RE_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_FA_,2},			{_RE_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_DO_,2},			{_LA_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_RE_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_RE_,0}, 			{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_SOL_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_FA_,2},			{_SOL_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_SOL_,1},			{_MI_,2},			{_DO_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 6 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_MI_,1},			{_DO_,2},			{_DO_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,2},			{_MI_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_D_,0}, 		{_VOICE_EMPTY_,0}},

	{{_FA_,1},			{_RE_,2},			{_RE_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_MI_,1},			{_DO_,2},			{_RE_D_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_SI_,1},			{_VOICE_OFF_,0}, 	{_VOICE_EMPTY_,0}}, /* Temps 7 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SI_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,1},			{_SI_,1},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_SI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_MI_,2},			{_VOICE_OFF_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SOL_D_,0},		{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 8 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

/* Reprise */
	{{_SI_,1},			{_MI_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 9 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_SI_,1},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_SOL_,1},			{_SI_,1},			{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	

	{{_MI_,1},			{_LA_,1},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 10 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_DO_,2},			{_MI_,2},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_SOL_D_,1},		{_SI_,1},			{_SOL_D_,0}, 		{_VOICE_EMPTY_,0}}, /* Temps 11 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SOL_D_,1},		{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_SOL_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_SOL_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_MI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_MI_,2},			{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 12 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_RE_,1}, 			{_VOICE_EMPTY_,0}}, /* Temps 13 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_FA_,1},			{_RE_,2},			{_RE_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_FA_,2},			{_RE_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_DO_,2},			{_LA_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_RE_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_RE_,0}, 			{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_SOL_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_FA_,2},			{_SOL_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},


	{{_SOL_,1},			{_MI_,2},			{_DO_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 14 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_MI_,1},			{_DO_,2},			{_DO_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,2},			{_MI_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_DO_D_,0}, 		{_VOICE_EMPTY_,0}},

	{{_FA_,1},			{_RE_,2},			{_RE_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_MI_,1},			{_DO_,2},			{_RE_D_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_SI_,1},			{_VOICE_OFF_,0}, 	{_VOICE_EMPTY_,0}}, /* Temps 15 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SI_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,1},			{_SI_,1},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_LA_,1},			{_DO_,2},			{_SI_,0},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_RE_,2},			{_VOICE_OFF_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SI_,1},			{_MI_,2},			{_VOICE_OFF_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_SOL_D_,0},		{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 16 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_OFF_,1},	{_VOICE_OFF_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0},	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

/* FIN */
	{{_LA_,1},			{_MI_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 17 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_RE_,2},			{_LA_D_,0}, 		{_VOICE_EMPTY_,0}}, /* Temps 18 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,1},			{_SI_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 19 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_LA_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_MI_,1},			{_SOL_D_,1},		{_LA_D_,0},			{_VOICE_EMPTY_,0}}, /* Temps 20 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

/*Reprise de fin */
	{{_LA_,1},			{_MI_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 21 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_D_,1},		{_RE_,2},			{_LA_D_,0}, 		{_VOICE_EMPTY_,0}}, /* Temps 22 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_SOL_,1},			{_SI_,1},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_DO_,2},			{_LA_,0}, 			{_VOICE_EMPTY_,0}}, /* Temps 23 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_MI_,2},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_LA_,1},			{_LA_,2},			{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_RE_,2},			{_SOL_D_,2},		{_LA_D_,0},			{_VOICE_EMPTY_,0}}, /* Temps 24 */
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},		 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,1},	{_LA_,0},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},

	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_LA_,0}, 			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_MI_,1},			{_VOICE_EMPTY_,0}},
	{{_VOICE_EMPTY_,1},	{_VOICE_EMPTY_,2},	{_VOICE_EMPTY_,0}, 	{_VOICE_EMPTY_,0}}
};

void Init_Periphs(void)
{
	/* Reglage des horloges systeme (40Mhz)*/
	Init_Clock_System();

	/* Liaison serie 19400/8/N/1 */
	setup_usart();

	/* Activation de la sortie audio (timer 4)*/
	Init_PWM ();

	/* Activation des oscillateur de voix */
	Init_Voice ();

	/* Activation de l'ADC */
	Init_ADC();

	/* Activation du systick à 1 Khz */
	SysTick->LOAD = 72000;
	SYSTICK_ENABLE_IT();
	SYSTICK_CLOCK_AHB();
	SYSTICK_ENABLE_COUNTER();

	/* Activation des IT */
	NVIC_SET_PRIO_PERIPH(TIM1_UP, 10);
	NVIC_SET_PRIO_PERIPH(TIM2, 10);
	NVIC_SET_PRIO_PERIPH(TIM3, 10);
	NVIC_SET_PRIO_PERIPH(TIM4, 11);
	NVIC_SET_PRIO_SYSTEM(SYSTICK,14);

	NVIC_ENABLE_PERIPH_IT(TIM1_UP);
	NVIC_ENABLE_PERIPH_IT(TIM2);
	NVIC_ENABLE_PERIPH_IT(TIM3);
	NVIC_ENABLE_PERIPH_IT(TIM4);
}

int main (void)
{
int temp;

	compteur=0;
	note=_DO_;
	octave=0;

	index_musique=0;

	Init_Periphs();

	while (1)
	{
		temp = Lire_ADC();

		temp = temp/ (0x400/8);

		if (temp <0) temp = 0;
		if (temp>6) temp =6;

		waveform_nbr = temp;
	}

	return 0;
}

void SysTick_Handler(void)
{
int i;

	compteur++;

	if (compteur>=COMPTEUR_MAX)
	{
		compteur=0;

		for (i=0; i<3; i++)
		{
			if (partition[index_musique][i].note != _VOICE_EMPTY_)
			{
				Regle_Canal(i,
				            partition[index_musique][i].note, 
							partition[index_musique][i].octave);	
			}
		} 

		/*Regle_Canal(0,
				            partition[index_musique][0].note, 
							partition[index_musique][0].octave);   */

		index_musique++;
		if (index_musique>=MUSIQUE_MAX) index_musique=0;
	}
}
