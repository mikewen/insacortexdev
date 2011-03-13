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

#include "appli.h"
#include "interface.h"
#include "capteur.h"
#include "controle.h"

#include "config.h"
#include "callback.h"

#include "stm_clock.h"

volatile int consigne_vitesse;

float Kp_v, Ki_v, Kd_v;
volatile float derniere_commande;
volatile float partiel_prop;
volatile float partiel_int;
volatile float partiel_der;

volatile int erreur;
volatile int derniere_erreur;
volatile int integral;
volatile int derivee;

int asservissement;

void Init_Application (void)
{
	consigne_vitesse = 0;

	Kp_v = COEFF_KP;
	Ki_v = COEFF_KI;
	Kd_v = COEFF_KD;

	erreur=0;
	derniere_erreur=0;
	integral=0;
	derivee=0;

	derniere_commande = 0.0;
	partiel_prop =0.0;
	partiel_int =0.0;
	partiel_der =0.0;

	asservissement = 0;
}

void Application (void)
{
	/* Boucle infinie sur la scrutation de l'interface serie, pour prise en compte des commandes */
	for (;;)
	{
		Interface();
	}
}

void Regle_Controle(int consigne_demandee)
{
int temp;

	temp=(consigne_demandee*_VITESSE_MAX_)/100;	  

	consigne_vitesse=temp;
	if (asservissement == 0)
	{
		commande_courante = consigne_demandee;	
	}
}

void Gere_Asservissement(int tours, int vitesse)
{
/*
	previous_error = 0
	integral = 0 
	start:
  		error = setpoint - actual_position
  		integral = integral + (error*dt)
  		derivative = (error - previous_error)/dt
  		output = (Kp*error) + (Ki*integral) + (Kd*derivative)
  		previous_error = error
*/

	if (consigne_vitesse == 0)
	{
		erreur=0;
		derniere_erreur=0;
		integral=0;
		derivee=0;

		derniere_commande = 0.0;
		partiel_prop =0.0;
		partiel_int =0.0;
		partiel_der =0.0;
	}
	else
	{
		erreur =  (float)(consigne_vitesse - vitesse);
		integral = integral + erreur;
		derivee = erreur - derniere_erreur;
		
		partiel_prop=(Kp_v*(float)erreur);
		partiel_int=(Ki_v*(float)integral);
		partiel_der=(Kd_v*(float)derivee);
		derniere_commande =  partiel_prop+ partiel_int + partiel_der;
		derniere_erreur = erreur;
		 
		if (derniere_commande <-100.0) derniere_commande =-100.0;
		if (derniere_commande >100.0) derniere_commande = 100.0;
	}

	Commande_moteur((int) derniere_commande);
}

 
