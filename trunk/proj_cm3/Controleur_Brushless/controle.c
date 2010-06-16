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
#include "asservissement.h"

#include "config.h"
#include "callback.h"

#include "stm_clock.h"
#include <stdlib.h>

int commande_courante;
int calage;
int frein;
int gaz;
int avance;
volatile int Phase_A_tab, Phase_B_tab, Phase_C_tab;

const s16 Tableau_Commande[480]=
{

        0, 857, 1714, 2570, 3425, 4276, 5125, 5971,
        6812, 7649, 8480, 9306, 10125, 10937, 11742, 12539,
        13327, 14106, 14875, 15635, 16383, 17120, 17846, 18559,
        19259, 19947, 20620, 21280, 21925, 22555, 23169, 23768,
        24350, 24916, 25464, 25995, 26509, 27004, 27480, 27938,
        28377, 28796, 29195, 29575, 29934, 30272, 30590, 30887,
        31163, 31417, 31650, 31861, 32050, 32218, 32363, 32486,
        32587, 32665, 32722, 32755, 32767, 32755, 32722, 32665,
        32587, 32486, 32363, 32218, 32050, 31861, 31650, 31417,
        31163, 30887, 30590, 30272, 29934, 29575, 29195, 28796,
        28377, 27938, 27480, 27004, 26509, 25995, 25464, 24916,
        24350, 23768, 23169, 22555, 21925, 21280, 20620, 19947,
        19259, 18559, 17846, 17120, 16383, 15635, 14875, 14106,
        13327, 12539, 11742, 10937, 10125, 9306, 8480, 7649,
        6812, 5971, 5125, 4276, 3425, 2570, 1714, 857,
        0, -857, -1714, -2570, -3425, -4276, -5125, -5971,
        -6812, -7649, -8480, -9306, -10125, -10937, -11742, -12539,
        -13327, -14106, -14875, -15635, -16383, -17120, -17846, -18559,
        -19259, -19947, -20620, -21280, -21925, -22555, -23169, -23768,
        -24350, -24916, -25464, -25995, -26509, -27004, -27480, -27938,
        -28377, -28796, -29195, -29575, -29934, -30272, -30590, -30887,
        -31163, -31417, -31650, -31861, -32050, -32218, -32363, -32486,
        -32587, -32665, -32722, -32755, -32767, -32755, -32722, -32665,
        -32587, -32486, -32363, -32218, -32050, -31861, -31650, -31417,
        -31163, -30887, -30590, -30272, -29934, -29575, -29195, -28796,
        -28377, -27938, -27480, -27004, -26509, -25995, -25464, -24916,
        -24350, -23768, -23169, -22555, -21925, -21280, -20620, -19947,
        -19259, -18559, -17846, -17120, -16383, -15635, -14875, -14106,
        -13327, -12539, -11742, -10937, -10125, -9306, -8480, -7649,
        -6812, -5971, -5125, -4276, -3425, -2570, -1714, -857,
        0, 857, 1714, 2570, 3425, 4276, 5125, 5971,
        6812, 7649, 8480, 9306, 10125, 10937, 11742, 12539,
        13327, 14106, 14875, 15635, 16383, 17120, 17846, 18559,
        19259, 19947, 20620, 21280, 21925, 22555, 23169, 23768,
        24350, 24916, 25464, 25995, 26509, 27004, 27480, 27938,
        28377, 28796, 29195, 29575, 29934, 30272, 30590, 30887,
        31163, 31417, 31650, 31861, 32050, 32218, 32363, 32486,
        32587, 32665, 32722, 32755, 32767, 32755, 32722, 32665,
        32587, 32486, 32363, 32218, 32050, 31861, 31650, 31417,
        31163, 30887, 30590, 30272, 29934, 29575, 29195, 28796,
        28377, 27938, 27480, 27004, 26509, 25995, 25464, 24916,
        24350, 23768, 23169, 22555, 21925, 21280, 20620, 19947,
        19259, 18559, 17846, 17120, 16383, 15635, 14875, 14106,
        13327, 12539, 11742, 10937, 10125, 9306, 8480, 7649,
        6812, 5971, 5125, 4276, 3425, 2570, 1714, 857,
        0, -857, -1714, -2570, -3425, -4276, -5125, -5971,
        -6812, -7649, -8480, -9306, -10125, -10937, -11742, -12539,
        -13327, -14106, -14875, -15635, -16383, -17120, -17846, -18559,
        -19259, -19947, -20620, -21280, -21925, -22555, -23169, -23768,
        -24350, -24916, -25464, -25995, -26509, -27004, -27480, -27938,
        -28377, -28796, -29195, -29575, -29934, -30272, -30590, -30887,
        -31163, -31417, -31650, -31861, -32050, -32218, -32363, -32486,
        -32587, -32665, -32722, -32755, -32767, -32755, -32722, -32665,
        -32587, -32486, -32363, -32218, -32050, -31861, -31650, -31417,
        -31163, -30887, -30590, -30272, -29934, -29575, -29195, -28796,
        -28377, -27938, -27480, -27004, -26509, -25995, -25464, -24916,
        -24350, -23768, -23169, -22555, -21925, -21280, -20620, -19947,
        -19259, -18559, -17846, -17120, -16383, -15635, -14875, -14106,
        -13327, -12539, -11742, -10937, -10125, -9306, -8480, -7649,
        -6812, -5971, -5125, -4276, -3425, -2570, -1714, -857,
};
#define _AVANCE_120_DEGRES_ 80
#define _AVANCE_240_DEGRES_ 160
#define _SINUS_90_DEGRES_ 60

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
 
void Regle_Controle(int commande)
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
	Regle_Controle(commande_courante);
}
