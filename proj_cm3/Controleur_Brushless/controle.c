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

volatile unsigned int temp;

int P_MOS[6][3]= 
{
	{1,0,0},{0,1,0},{0,1,0},{0,0,1},{0,0,1},{1,0,0}
};

int N_MOS[6][3]= 
{
	{0,0,1},{0,0,1},{1,0,0},{1,0,0},{0,1,0},{0,1,0}
};

void Callback_capteur_position_avant(void);

void Init_Controle (void)
{
	DEFINE_EVENT(CAPTEUR_POSITION_AVANT, Callback_capteur_position_avant);

  	cycle_moteur=0;
	rapport_pwm=0x0;
	sens_rotation =1;

	position_capteur_avant=0;
	position_capteur_apres=0;

	position_capteur_avant = position_capteur_avant + ((_RESOLUTION_CAPTEUR_/6)*4);
	Regle_Position_Avant(position_capteur_avant);
}

void Init_Moteur(void)
{
	/* Reglage du hacheur */
	rapport_pwm = 3200/2;
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);	
}

void Regle_PWM(int pwm)
{
	temp = (unsigned int)pwm;
	temp = temp*PWM_MAX;
	temp = temp/100;

	rapport_pwm = (int)temp;
}

void Callback_capteur_position_avant(void)
{
	if (sens_rotation==1)
	{
		position_capteur_avant = position_capteur_avant + ((_RESOLUTION_CAPTEUR_/6)*4);
		if (position_capteur_avant>=(_RESOLUTION_CAPTEUR_*4)) position_capteur_avant = position_capteur_avant - (_RESOLUTION_CAPTEUR_*4);

		cycle_moteur ++;
		if (cycle_moteur >=6) cycle_moteur = 0;
	}
	else
	{
		position_capteur_avant = position_capteur_avant - ((_RESOLUTION_CAPTEUR_/6)*4);
		if (position_capteur_avant<0) position_capteur_avant = (_RESOLUTION_CAPTEUR_*4)- position_capteur_avant;

		cycle_moteur --;
		if (cycle_moteur <0) cycle_moteur = 5;
	}

    Regle_Position_Avant(position_capteur_avant);

	/* Reglage du hacheur */
	Regle_Bras_Haut(P_MOS[cycle_moteur][PHASE_A],P_MOS[cycle_moteur][PHASE_B],P_MOS[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS[cycle_moteur][PHASE_A],N_MOS[cycle_moteur][PHASE_B],N_MOS[cycle_moteur][PHASE_C],rapport_pwm);	 
}