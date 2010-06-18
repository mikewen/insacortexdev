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

#include "asservissement.h"

#include "capteur.h"
#include "controle.h"

#include "config.h"
#include "callback.h"

#include "stm_clock.h"

volatile float vitesse;

float distance;
float derniere_erreur;

float Kp_v, Ki_v, Kd_v;
float derniere_commande;
volatile float vitesse_consigne;

volatile float erreur;
volatile float integral;
volatile float derivee;

void Init_Asservissement (void)
{
	vitesse_consigne = 0.0;

	Kp_v = COEFF_KP;
	Ki_v = COEFF_KI;
	Kd_v = COEFF_KD;

	erreur=0;
	integral=0;
	derivee=0;
}

void Fourni_stats (int *v, int* t, int* av, int* p)
{
	*v = Donne_Vitesse();
	*t = Donne_nb_tour();
	*av = Lire_Avance();
	*p = 0;	
}

void Fourni_coeffs (int *kp, int *ki, int *kd)
{
	*kp= (int)(Kp_v*1000.0);
	*ki= (int)(Ki_v*1000.0);
	*kd= (int)(Kd_v*1000.0);
}

int asservissement(int consigne)
{
	vitesse = (float) Donne_Vitesse();

	vitesse_consigne = (float)consigne*20000.0/100.0;

	erreur =  vitesse_consigne- vitesse;

	derivee = derniere_erreur - erreur;
	derivee = Kd_v*derivee;

	integral = integral + erreur;
	integral = Ki_v*integral; 
	
	derniere_commande = (Kp_v*erreur) + integral + derivee;
	derniere_erreur = erreur;
	 
	if (derniere_commande <-100.0) derniere_commande =-100.0;
	if (derniere_commande >100.0) derniere_commande = 100.0;

	return (int) derniere_commande;
}

void Regle_Coeff_Kv(int kv)
{
	Kp_v = ((float)kv)/1000.0;
}

void Regle_Coeff_Ki(int ki)
{
	Ki_v = ((float)ki)/1000.0;
}

void Regle_Coeff_Kd(int kd)
{
	Kd_v = ((float)kd)/1000.0;
}
 
