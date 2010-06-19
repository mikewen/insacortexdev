/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of brushless DC motor controller.
 *
 * Brushless DC motor controller is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Brushless DC motor controller is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Brushless DC motor controller; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"

#include "capteur.h"
#include "hacheur.h"
#include "adc.h"
#include "controle.h"

#include "config.h"
#include "callback.h"

#include "stm_clock.h"
#include <stdlib.h>

volatile int commande_courante;
int calage;
int frein;
int gaz;
int avance;
volatile int Phase_A_tab, Phase_B_tab, Phase_C_tab;

const s16 Tableau_Commande[480]=
{

        0, 2998, 5971, 8894, 11742, 14492, 17120, 19605,
        21925, 24061, 25995, 27711, 29195, 30434, 31417, 32137,
        32587, 32764, 32665, 32293, 31650, 30741, 29575, 28160,
        26509, 24635, 22555, 20285, 17846, 15256, 12539, 9716,
        6812, 3851, 857, -2143, -5125, -8065, -10937, -13718,
        -16383, -18911, -21280, -23471, -25464, -27244, -28796, -30106,
        -31163, -31959, -32486, -32741, -32722, -32427, -31861, -31028,
        -29934, -28589, -27004, -25192, -23169, -20952, -18559, -16010,
        -13327, -10532, -7649, -4701, -1714, 1286, 4276, 7231,
        10125, 12934, 15635, 18204, 20620, 22864, 24916, 26758,
        28377, 29757, 30887, 31758, 32363, 32696, 32755, 32539,
        32050, 31293, 30272, 28998, 27480, 25732, 23768, 21604,
        19259, 16753, 14106, 11341, 8480, 5549, 2570, -428,
        -3425, -6392, -9306, -12142, -14875, -17484, -19947, -22242,
        -24350, -26254, -27938, -29387, -30590, -31536, -32218, -32629,
        -32767, -32629, -32218, -31536, -30590, -29387, -27938, -26254,
        -24350, -22242, -19947, -17484, -14875, -12142, -9306, -6392,
        -3425, -428, 2570, 5549, 8480, 11341, 14106, 16753,
        19259, 21604, 23768, 25732, 27480, 28998, 30272, 31293,
        32050, 32539, 32755, 32696, 32363, 31758, 30887, 29757,
        28377, 26758, 24916, 22864, 20620, 18204, 15635, 12934,
        10125, 7231, 4276, 1286, -1714, -4701, -7649, -10532,
        -13327, -16010, -18559, -20952, -23169, -25192, -27004, -28589,
        -29934, -31028, -31861, -32427, -32722, -32741, -32486, -31959,
        -31163, -30106, -28796, -27244, -25464, -23471, -21280, -18911,
        -16383, -13718, -10937, -8065, -5125, -2143, 857, 3851,
        6812, 9716, 12539, 15256, 17846, 20285, 22555, 24635,
        26509, 28160, 29575, 30741, 31650, 32293, 32665, 32764,
        32587, 32137, 31417, 30434, 29195, 27711, 25995, 24061,
        21925, 19605, 17120, 14492, 11742, 8894, 5971, 2998,
        0, -2998, -5971, -8894, -11742, -14492, -17120, -19605,
        -21925, -24061, -25995, -27711, -29195, -30434, -31417, -32137,
        -32587, -32764, -32665, -32293, -31650, -30741, -29575, -28160,
        -26509, -24635, -22555, -20285, -17846, -15256, -12539, -9716,
        -6812, -3851, -857, 2143, 5125, 8065, 10937, 13718,
        16383, 18911, 21280, 23471, 25464, 27244, 28796, 30106,
        31163, 31959, 32486, 32741, 32722, 32427, 31861, 31028,
        29934, 28589, 27004, 25192, 23169, 20952, 18559, 16010,
        13327, 10532, 7649, 4701, 1714, -1286, -4276, -7231,
        -10125, -12934, -15635, -18204, -20620, -22864, -24916, -26758,
        -28377, -29757, -30887, -31758, -32363, -32696, -32755, -32539,
        -32050, -31293, -30272, -28998, -27480, -25732, -23768, -21604,
        -19259, -16753, -14106, -11341, -8480, -5549, -2570, 428,
        3425, 6392, 9306, 12142, 14875, 17484, 19947, 22242,
        24350, 26254, 27938, 29387, 30590, 31536, 32218, 32629,
        32767, 32629, 32218, 31536, 30590, 29387, 27938, 26254,
        24350, 22242, 19947, 17484, 14875, 12142, 9306, 6392,
        3425, 428, -2570, -5549, -8480, -11341, -14106, -16753,
        -19259, -21604, -23768, -25732, -27480, -28998, -30272, -31293,
        -32050, -32539, -32755, -32696, -32363, -31758, -30887, -29757,
        -28377, -26758, -24916, -22864, -20620, -18204, -15635, -12934,
        -10125, -7231, -4276, -1286, 1714, 4701, 7649, 10532,
        13327, 16010, 18559, 20952, 23169, 25192, 27004, 28589,
        29934, 31028, 31861, 32427, 32722, 32741, 32486, 31959,
        31163, 30106, 28796, 27244, 25464, 23471, 21280, 18911,
        16383, 13718, 10937, 8065, 5125, 2143, -857, -3851,
        -6812, -9716, -12539, -15256, -17846, -20285, -22555, -24635,
        -26509, -28160, -29575, -30741, -31650, -32293, -32665, -32764,
        -32587, -32137, -31417, -30434, -29195, -27711, -25995, -24061,
        -21925, -19605, -17120, -14492, -11742, -8894, -5971, -2998,
};
#define _AVANCE_120_DEGRES_ 22
#define _AVANCE_240_DEGRES_ 45
#define _SINUS_90_DEGRES_ 17

void Callback_hacheur(void);

void Init_Controle (void)
{
  	gaz=0;
	calage = _SINUS_90_DEGRES_;
}

void Init_Moteur(void)
{
	Ecrire_Capteur(_RESOLUTION_ENCODEUR_-15);

	/* Reglage du hacheur */
	Regle_Controle(0); /* gaz à 0% */
	avance =10;

	ActiveITOverflow(Callback_hacheur);
}

void Regle_Avance(int av)
{
	avance = av;
}

int Lire_Avance(void)
{
	return avance;
}
 
void Commande_moteur(int commande)
{
int Phase_A, Phase_B, Phase_C;
int capteur;
int index_tab;
int av;

	commande_courante=commande;
	gaz = abs(commande);

	if (commande >=0)
	{
		calage = _SINUS_90_DEGRES_;
		av = avance;
	}
	else
	{
		calage = -_SINUS_90_DEGRES_;
		av = -avance;
	} 

	capteur = Lire_Capteur();
	index_tab = capteur+calage+avance;
	
	if (index_tab>=_TAILLE_COMMANDE_)
	{
		Phase_A = (int)Tableau_Commande[index_tab-_TAILLE_COMMANDE_];
	}
	else
	{
		Phase_A = (int)Tableau_Commande[index_tab];
	}

	if (index_tab + _AVANCE_120_DEGRES_>=_TAILLE_COMMANDE_)
	{
		Phase_B = (int)Tableau_Commande[(index_tab+_AVANCE_120_DEGRES_)-_TAILLE_COMMANDE_];
	}
	else
	{
		Phase_B = (int)Tableau_Commande[(index_tab+_AVANCE_120_DEGRES_)];
	}

	if (index_tab + _AVANCE_240_DEGRES_>=_TAILLE_COMMANDE_)
	{
		Phase_C = (int)Tableau_Commande[(index_tab+_AVANCE_240_DEGRES_)-_TAILLE_COMMANDE_];
	}
	else
	{
		Phase_C = (int)Tableau_Commande[(index_tab+_AVANCE_240_DEGRES_)];
	}

	Phase_A_tab = Phase_A;
	Phase_B_tab = Phase_B;
	Phase_C_tab = Phase_C;

	Phase_A = (Phase_A*gaz)/100;
	Phase_B = (Phase_B*gaz)/100;
	Phase_C = (Phase_C*gaz)/100;

	Commande_Hacheur(Phase_A, Phase_B, Phase_C); 
}

void Callback_hacheur(void)
{
	Commande_moteur(commande_courante);
}
