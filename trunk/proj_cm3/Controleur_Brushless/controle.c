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

int P_MOS[6]= {'A','B','B','C','C','A'};
int N_MOS[6]= {'C','C','A','A','B','B'};

void Callback_capteur_position_avant(void);

void Init_Controle (void)
{
	DEFINE_EVENT(CAPTEUR_POSITION_AVANT, Callback_capteur_position_avant);
	
	cycle_moteur=0;
	rapport_pwm=0xFFFF;
}

void Callback_capteur_position_avant(void)
{
	if (sens_rotation==1)
	{
		cycle_moteur ++;
		if (cycle_moteur >=6) cycle_moteur = 0;
	}
	else
	{
		cycle_moteur --;
		if (cycle_moteur <0) cycle_moteur = 5;
	}
}