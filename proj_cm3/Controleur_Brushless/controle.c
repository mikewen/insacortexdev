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

        0, 0, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, 0, 0, 0, 0,
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
	avance =7;

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

	if (commande_courante >=0)
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

	Phase_A = (Phase_A*gaz);
	Phase_B = (Phase_B*gaz);
	Phase_C = (Phase_C*gaz);

	Commande_Hacheur(Phase_A, Phase_B, Phase_C); 
}

void Callback_hacheur(void)
{
	Commande_moteur(commande_courante);
}
