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

int rapport_pwm;
int cycle_moteur;
int position_capteur_avant;
int position_capteur_apres;

#define PHASE_A	0
#define PHASE_B 1
#define PHASE_C 2

int P_MOS[6][3]= 
{
	{1,0,0},{0,1,0},{0,1,0},{0,0,1},{0,0,1},{1,0,0}
};

int N_MOS[6][3]= 
{
	{0,0,1},{0,0,1},{1,0,0},{1,0,0},{0,1,0},{0,1,0}
};

void Callback_capteur_position_avant(void);
void Callback_capteur_position_apres(void);

void Init_Controle (void)
{
	DEFINE_EVENT(CAPTEUR_POSITION_AVANT, Callback_capteur_position_avant);
	DEFINE_EVENT(CAPTEUR_POSITION_APRES, Callback_capteur_position_apres);

  	cycle_moteur=0;
	rapport_pwm=0x0;
	sens_rotation =CONTROLE_MODE_AVANT;

	position_capteur_avant = ((_RESOLUTION_CAPTEUR_/6)*4);
	position_capteur_apres = ((_RESOLUTION_CAPTEUR_)*4)-1;

	Regle_Position_Avant(position_capteur_avant);
	Regle_Position_Apres(position_capteur_apres);
}

void Cale_Moteur(void)
{
	/* Reglage du hacheur */
	rapport_pwm = 3200/2;
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);	
}

void Init_Moteur(void)
{
	Regle_Controle(0, CONTROLE_MODE_AVANT); /* PWM à 0% */

	Regle_Position_Avant((_RESOLUTION_CAPTEUR_/6)*4);
	Regle_Position_Apres(_RESOLUTION_CAPTEUR_-1);

	/* Reglage du hacheur */
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);
}

void Regle_Controle(int pwm, int mode)
{
unsigned int temp;

	temp = (unsigned int)pwm;
	temp = temp*PWM_MAX;
	temp = temp/100;

	rapport_pwm = (int)temp;
	
	if (sens_rotation != mode)
	{
		if (mode == CONTROLE_MODE_AVANT) cycle_moteur++;
		else cycle_moteur--;
	}
	
	if (cycle_moteur >=6) cycle_moteur=0;
	if (cycle_moteur<0) cycle_moteur = 5;

	sens_rotation = mode;

	/* Reglage du hacheur */
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);
}

void Callback_capteur_position_avant(void)
{
	position_capteur_avant = position_capteur_avant + ((_RESOLUTION_CAPTEUR_/6)*4);
	if (position_capteur_avant>=(_RESOLUTION_CAPTEUR_*4)) position_capteur_avant = position_capteur_avant - (_RESOLUTION_CAPTEUR_*4);

	position_capteur_apres = position_capteur_apres + ((_RESOLUTION_CAPTEUR_/6)*4);
	if (position_capteur_apres>=(_RESOLUTION_CAPTEUR_*4)) position_capteur_apres = position_capteur_apres - (_RESOLUTION_CAPTEUR_*4);

	cycle_moteur ++;
	if (cycle_moteur >=6) cycle_moteur = 0;

    Regle_Position_Avant(position_capteur_avant);
	Regle_Position_Apres(position_capteur_apres);

	/* Reglage du hacheur */
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);	 
}

void Callback_capteur_position_apres(void)
{
	position_capteur_avant = position_capteur_avant - ((_RESOLUTION_CAPTEUR_/6)*4);
	if (position_capteur_avant<0) position_capteur_avant = (_RESOLUTION_CAPTEUR_*4) - position_capteur_avant;

	position_capteur_apres = position_capteur_apres - ((_RESOLUTION_CAPTEUR_/6)*4);
	if (position_capteur_apres<0) position_capteur_apres = (_RESOLUTION_CAPTEUR_*4) - position_capteur_apres;

	cycle_moteur --;
	if (cycle_moteur <0) cycle_moteur = 5;

    Regle_Position_Avant(position_capteur_avant);
	Regle_Position_Apres(position_capteur_apres);

	/* Reglage du hacheur */
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);	 
}