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

int rapport_pwm;
int cycle_moteur;
int frein;

int avance, delta_avance;
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

#define _DISTANCE_ROUE_ 213.6283
#define PHASE_A	0
#define PHASE_B 1
#define PHASE_C 2

#define COEFF_KP	0.08
#define COEFF_KI	0.01
#define COEFF_KD	0.04

#define	V_MAX 		25000.0

int P_MOS_NORMAL[6][3]= 
{
	{1,0,0},{0,1,0},{0,1,0},{0,0,1},{0,0,1},{1,0,0}
};

int N_MOS_NORMAL[6][3]= 
{
	{0,0,1},{0,0,1},{1,0,0},{1,0,0},{0,1,0},{0,1,0}
};

int P_MOS_COMPLET[6][3]= 
{
	{1,0,0},{1,1,0},{0,1,0},{0,1,1},{0,0,1},{1,0,1}
};

int N_MOS_COMPLET[6][3]= 
{
	{0,1,1},{0,0,1},{1,0,1},{1,0,0},{1,1,0},{0,1,0}
};

int P_MOS_FREIN[6][3]= 
{
	{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1}
};

int N_MOS_FREIN[6][3]= 
{
	{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
};

void Callback_capteur_position_avant(void);
void Callback_capteur_position_arriere(void);

void Init_Controle (void)
{
	DEFINE_EVENT(CAPTEUR_POSITION_AVANT, Callback_capteur_position_avant);
	DEFINE_EVENT(CAPTEUR_POSITION_ARRIERE, Callback_capteur_position_arriere);

  	cycle_moteur=0;
	rapport_pwm=0x0;
	sens_rotation =CONTROLE_MODE_AVANT;
	frein = 0;

	Regle_Position_Avant((_PAS_60_DEGRES_));
	Regle_Position_Arriere((_RESOLUTION_ENCODEUR_-1));	

	/*REgle le timer 1 pour essai */
	RCC->APB2ENR |= RCC_TIM1EN; /* Mise en route de l'horloge du timer1 */
	TIM1->PSC = 800;
	TIM1->ARR = 10000; /* -> a 40 Mhz, ca donne une frequence de 5 Hz */

	TIM1->DIER = TIM_UIE;
}

void Cale_Moteur(void)
{
	/* Reglage du hacheur */
	Regle_Controle(10, CONTROLE_MODE_AVANT); /* PWM à 10% */
	cycle_moteur=5;

	Regle_Bras_Haut(P_MOS_NORMAL[cycle_moteur][PHASE_A],P_MOS_NORMAL[cycle_moteur][PHASE_B],P_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
	Regle_Bras_Bas(N_MOS_NORMAL[cycle_moteur][PHASE_A],N_MOS_NORMAL[cycle_moteur][PHASE_B],N_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);	
}

void Init_Moteur(void)
{
	cycle_moteur=0;
	avance =-15;

	tour =0;
	vitesse= 0.0;
	periode=0;
	timer_precedent=0;

	Kp_v = COEFF_KP;
	Ki_v = COEFF_KI;
	Kd_v = COEFF_KD;

	derniere_commande=0;
	derniere_erreur=0;
	vitesse_consigne=0;

	Regle_Position_Avant((_PAS_60_DEGRES_)-avance);
	Regle_Position_Arriere((_RESOLUTION_ENCODEUR_+avance));	
	Ecrire_Capteur(0);

	SysTick->LOAD = 50000;
	SYSTICK_CLOCK_AHB_8();
	SYSTICK_ENABLE_COUNTER();
	SYSTICK_ENABLE_IT();
	SYSTICK_ENABLE_COUNTER();

	/* Reglage du hacheur */
	Regle_Controle(0, CONTROLE_MODE_AVANT); /* PWM à 0% */
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
	*v = (int)vitesse;
	*t = tour;
	*av = avance;
	*p = cycle_moteur;	
}

void Fourni_coeffs (int *kp, int *ki, int *kd)
{
	*kp= (int)(Kp_v*1000.0);
	*ki= (int)(Ki_v*1000.0);
	*kd= (int)(Kd_v*1000.0);
}

void Regle_PWM(int pwm)
{
unsigned int temp;

	temp = (unsigned int)pwm;
	temp = temp*PWM_MAX;
	temp = temp/100;

	rapport_pwm = (int)temp;
}

void Regle_Avance(int av)
{
	delta_avance= avance - av;
	avance = av;
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
	
	Regle_PWM(derniere_commande);
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
 
void Regle_Controle(int pourcent, int mode)
{
	frein = 0;

	vitesse_consigne=(((float)pourcent)*V_MAX)/100.0;
	
	asservissement_vitesse();

	if (mode == CONTROLE_MODE_FREIN)
	{
		frein =1;
	}
	else 
	{	
		frein = 0;
			
		if (sens_rotation != mode)
		{
			sens_rotation = mode;
		
			if (mode == CONTROLE_MODE_AVANT)
			{
		 		cycle_moteur+=3;
			}
			else 
			{
				cycle_moteur-=3;
			}
		}
	}

	if (cycle_moteur >=6) cycle_moteur=0;
	if (cycle_moteur<0) cycle_moteur = 5;

	/* Reglage du hacheur */
	if (frein)
	{
		Regle_Bras_Haut(P_MOS_FREIN[cycle_moteur][PHASE_A],P_MOS_FREIN[cycle_moteur][PHASE_B],P_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_FREIN[cycle_moteur][PHASE_A],N_MOS_FREIN[cycle_moteur][PHASE_B],N_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
	}
	else
	{
		Regle_Bras_Haut(P_MOS_NORMAL[cycle_moteur][PHASE_A],P_MOS_NORMAL[cycle_moteur][PHASE_B],P_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_NORMAL[cycle_moteur][PHASE_A],N_MOS_NORMAL[cycle_moteur][PHASE_B],N_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
	}
}

void Callback_capteur_position_avant(void)
{
int position_capteur_avant;
int position_capteur_arriere;
int val;
int nouvelle_periode;

	val = SysTick->VAL;
	 
	if (val>= timer_precedent) 
	{
		nouvelle_periode = (timer_precedent+0x1000000) - val ;
	}
	else 
	{
		nouvelle_periode = timer_precedent-val;
	}
	
	timer_precedent = val;
	
	if (nouvelle_periode <80000 ) periode = nouvelle_periode;

	cycle_moteur ++;
	if (cycle_moteur >=6) 
	{	
		cycle_moteur = 0;
		tour++;
	}
	
	position_capteur_avant = Lire_Capteur_Avant();
	position_capteur_arriere = Lire_Capteur_Arriere();
	position_capteur_avant = (position_capteur_avant + _PAS_60_DEGRES_);
	position_capteur_arriere = (position_capteur_arriere + _PAS_60_DEGRES_);

	if (delta_avance != 0) 
	{
		position_capteur_avant += delta_avance;
		position_capteur_arriere += delta_avance;
		delta_avance =0;	
	}

	if (position_capteur_avant>=_RESOLUTION_ENCODEUR_) position_capteur_avant = position_capteur_avant - _RESOLUTION_ENCODEUR_;
	if (position_capteur_arriere>=_RESOLUTION_ENCODEUR_) position_capteur_arriere = position_capteur_arriere - _RESOLUTION_ENCODEUR_;
	if (position_capteur_avant<0) position_capteur_avant = _RESOLUTION_ENCODEUR_ + position_capteur_avant;
	if (position_capteur_arriere<0) position_capteur_arriere = _RESOLUTION_ENCODEUR_ + position_capteur_arriere;

    Regle_Position_Avant(position_capteur_avant);
	Regle_Position_Arriere(position_capteur_arriere);

#ifndef _DEBUG_CAPTEUR_
	/* Reglage du hacheur */
	if (frein)
	{
		Regle_Bras_Haut(P_MOS_FREIN[cycle_moteur][PHASE_A],P_MOS_FREIN[cycle_moteur][PHASE_B],P_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_FREIN[cycle_moteur][PHASE_A],N_MOS_FREIN[cycle_moteur][PHASE_B],N_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
	}
	else
	{
		Regle_Bras_Haut(P_MOS_NORMAL[cycle_moteur][PHASE_A],P_MOS_NORMAL[cycle_moteur][PHASE_B],P_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_NORMAL[cycle_moteur][PHASE_A],N_MOS_NORMAL[cycle_moteur][PHASE_B],N_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
	}
#else
	Regle_Bras_Haut(0,0,0,0);

	if (cycle_moteur==0) 
	{
		Regle_Bras_Bas(0,1,0,0);
	}
	else
	{
		Regle_Bras_Bas((cycle_moteur&0x01),0,0,0);
	}
#endif 

	Calcul_stats();
}

void Callback_capteur_position_arriere(void)
{
int position_capteur_avant;
int position_capteur_arriere;
int val;
int nouvelle_periode;

	val = SysTick->VAL;
	 
	if (val>= timer_precedent) 
	{
		nouvelle_periode = (timer_precedent+0x1000000) - val ;
	}
	else 
	{
		nouvelle_periode = timer_precedent-val;
	}
	
	timer_precedent = val;

	if (nouvelle_periode <80000 ) periode = nouvelle_periode;

	cycle_moteur --;
	if (cycle_moteur <0) 
	{
		cycle_moteur = 5;
		tour--;
	}

	position_capteur_avant = Lire_Capteur_Avant();
	position_capteur_arriere = Lire_Capteur_Arriere();
	position_capteur_avant = (position_capteur_avant - _PAS_60_DEGRES_);
	position_capteur_arriere = (position_capteur_arriere - _PAS_60_DEGRES_);

	if (delta_avance != 0) 
	{
		position_capteur_avant += delta_avance;
		position_capteur_arriere += delta_avance;
		delta_avance =0;	
	}

	if (position_capteur_avant>=_RESOLUTION_ENCODEUR_) position_capteur_avant = position_capteur_avant - _RESOLUTION_ENCODEUR_;
	if (position_capteur_arriere>=_RESOLUTION_ENCODEUR_) position_capteur_arriere = position_capteur_arriere - _RESOLUTION_ENCODEUR_;
	if (position_capteur_avant<0) position_capteur_avant = _RESOLUTION_ENCODEUR_ + position_capteur_avant;
	if (position_capteur_arriere<0) position_capteur_arriere = _RESOLUTION_ENCODEUR_ + position_capteur_arriere;

    Regle_Position_Avant(position_capteur_avant);
	Regle_Position_Arriere(position_capteur_arriere);

	/* Reglage du hacheur */
	if (frein)
	{
		Regle_Bras_Haut(P_MOS_FREIN[cycle_moteur][PHASE_A],P_MOS_FREIN[cycle_moteur][PHASE_B],P_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_FREIN[cycle_moteur][PHASE_A],N_MOS_FREIN[cycle_moteur][PHASE_B],N_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
	}
	else
	{
		Regle_Bras_Haut(P_MOS_NORMAL[cycle_moteur][PHASE_A],P_MOS_NORMAL[cycle_moteur][PHASE_B],P_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_NORMAL[cycle_moteur][PHASE_A],N_MOS_NORMAL[cycle_moteur][PHASE_B],N_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
	}

	Calcul_stats();
}

void TIM1_UP_IRQHandler (void)
{
	TIM1->SR &= ~(TIM_UIF);

	cycle_moteur ++;
	if (cycle_moteur >=6) cycle_moteur = 0;

	/* Reglage du hacheur */
	if (frein)
	{
		Regle_Bras_Haut(P_MOS_FREIN[cycle_moteur][PHASE_A],P_MOS_FREIN[cycle_moteur][PHASE_B],P_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_FREIN[cycle_moteur][PHASE_A],N_MOS_FREIN[cycle_moteur][PHASE_B],N_MOS_FREIN[cycle_moteur][PHASE_C],rapport_pwm);
	}
	else
	{
		Regle_Bras_Haut(P_MOS_NORMAL[cycle_moteur][PHASE_A],P_MOS_NORMAL[cycle_moteur][PHASE_B],P_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
		Regle_Bras_Bas(N_MOS_NORMAL[cycle_moteur][PHASE_A],N_MOS_NORMAL[cycle_moteur][PHASE_B],N_MOS_NORMAL[cycle_moteur][PHASE_C],rapport_pwm);
	} 
}

void SysTick_Handler(void)
{
	asservissement_vitesse();	
}