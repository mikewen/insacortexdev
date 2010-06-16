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

int periode;
int timer_precedent;
float vitesse;
int tour;
float distance;
float freq_dent;
float distance;
float derniere_erreur;

float Kp_v, Ki_v, Kd_v;
float derniere_commande;
float vitesse_consigne;

void Init_Asservissement (void)
{
  	//cycle_moteur=0;
	//rapport_pwm=0x0;
	//sens_rotation =CONTROLE_MODE_AVANT;
	//frein = 0;

	//Regle_Position_Avant((_PAS_60_DEGRES_));
	//Regle_Position_Arriere((_RESOLUTION_ENCODEUR_-1));	

	/*Regle le timer 1 pour mesurer les durées du capteurs */
	RCC->APB2ENR |= RCC_TIM1EN; /* Mise en route de l'horloge du timer1 */
	TIM1->PSC = 800;
	TIM1->ARR = 10000; /* -> a 40 Mhz, ca donne une frequence de 5 Hz */

	TIM1->DIER = TIM_UIE;
}

void Calcul_stats(void)
{
float temp;
	temp = (float)__HCLK;
	temp = temp/8.0;
	temp = temp/(periode*12.0);
	vitesse = temp*60.0;

	distance = (float)tour;
	distance =  distance* _DISTANCE_ROUE_;

	freq_dent= (float)(periode);
	freq_dent = freq_dent/((float)(_PAS_60_DEGRES_));
	freq_dent = (1.0/freq_dent);
}

void Fourni_stats (int *v, int* t, int* av, int* p)
{
static int avance;

	avance = Lire_Avance();
	*v = (int)vitesse;
	*t = tour;
	*av = avance;
	*p = 0;	
}

void Fourni_coeffs (int *kp, int *ki, int *kd)
{
	*kp= (int)(Kp_v*1000.0);
	*ki= (int)(Ki_v*1000.0);
	*kd= (int)(Kd_v*1000.0);
}

void asservissement_vitesse(void)
{
float erreur;
float integral;
float derivee;

	erreur =  vitesse_consigne- vitesse;

	derivee = derniere_erreur - erreur;
	derivee = Kd_v*derivee;

	integral = derniere_erreur + erreur;
	integral = Ki_v*integral;
	
	derniere_commande = (Kp_v*erreur) + integral + derivee;
	 
	if (derniere_commande <0.0) derniere_commande =0.0;
	if (derniere_commande >100.0) derniere_commande = 100.0;
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
 
