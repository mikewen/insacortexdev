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

#include "interface_uart.h"
#include "config.h"

#include "stm_usartx.h"

#include <stdio.h>

#include "controle.h"
#include "asservissement.h"
#include "hacheur.h"

char buffer[50];
char *commande;

int pourcent_pwm;
int param_avance;
int coeff_kv;
int val_dead_time;

FILE Port_COM;

void Init_Interface_uart(void)
{
	printf ("Controleur Brushless\nRealisation: SDM\nVer: %d.%02d\n\n",_VERSION_MAJEUR_, _VERSION_MINEUR_);

#ifdef _AVEC_CAPTEUR_
	printf ("Mode capteur: AVEC\nResolution capteur: %d\n\n",_RESOLUTION_CAPTEUR_);
#else
	printf ("Mode capteur: SANS\n");
#endif /* _AVEC_CAPTEUR_ */ 

	commande=buffer;
	Port_COM._file=1;	
}

void Interface_uart(void)
{
char c;
int commande_prete;

	printf ("\n> ");
	commande_prete = 0;

	while (commande_prete==0)
	{
		if (uart_buffer_full())
		{
			c=fgetc(&Port_COM);
			fputc(c, &Port_COM);

			if (c=='\r')
			{
				*commande= 0;
				commande=buffer;
				commande_prete++;
			}
			else
			{
				*commande = c;
				commande++;
			}
		}
	}

	if (commande_prete)
	{
		switch (commande[0])
		{
			case 'a':
			case 'A':
				sscanf (commande, "%d", &pourcent_pwm);

				if (pourcent_pwm<-100) pourcent_pwm=-100;
				if (pourcent_pwm>100) pourcent_pwm=100;

				printf ("\nEn marche a %d%% de la vitesse max\n", pourcent_pwm);
				Regle_Controle(pourcent_pwm);
				break;
			case 's':
			case 'S':
				printf ("\nArret du moteur (roue libre)\n");
				Regle_Controle(0);
				break;
			case 'f':
			case 'F':
				sscanf (commande, "%d", &pourcent_pwm);

				if (pourcent_pwm<0) pourcent_pwm=0;
				if (pourcent_pwm>100) pourcent_pwm=100;

				printf ("\nFrein a %d%%\n", pourcent_pwm);
				//Regle_Controle(pourcent_pwm, CONTROLE_MODE_FREIN);
				break;
			case 'd':
			case 'D':
				Init_Moteur();
				printf ("\nCapteur calle a zero\n");
				break;
			case 'p':
			case 'P':
				switch (commande[1])
				{
					case '0':
						sscanf (&commande[2], "%d", &param_avance);

						if (param_avance >= _PAS_60_DEGRES_) param_avance=_PAS_60_DEGRES_-1;
						if (param_avance<=-(_PAS_60_DEGRES_)) param_avance=-(_PAS_60_DEGRES_) +1;
						
						Regle_Avance(param_avance);
						printf ("\nAvance regle a %d\n",param_avance);  
						break;
					case '1':
						sscanf (&commande[2], "%d", &coeff_kv);

						if (coeff_kv >= 2000) coeff_kv=1999;
						if (coeff_kv< 0) coeff_kv=0;
						
						Regle_Coeff_Kv(coeff_kv);
						printf ("\nKp regle a %d\n",coeff_kv);  
						break;
					case '2':
						sscanf (&commande[2], "%d", &coeff_kv);

						if (coeff_kv >= 2000) coeff_kv=1999;
						if (coeff_kv< 0) coeff_kv=0;
						
						Regle_Coeff_Ki(coeff_kv);
						printf ("\nKi regle a %d\n",coeff_kv);  
						break;
					case '3':
						sscanf (&commande[2], "%d", &coeff_kv);

						if (coeff_kv >= 2000) coeff_kv=1999;
						if (coeff_kv< 0) coeff_kv=0;
						
						Regle_Coeff_Kd(coeff_kv);
						printf ("\nKd regle a %d\n",coeff_kv);  
						break;
					default:
						printf ("\nParametre non supporte\n");
				}
				break;
			case 'e':
			case 'E':
				
				break;
			case 'r':
			case 'R':
				sscanf (commande, "%d", &val_dead_time);

				if (val_dead_time<0) val_dead_time=0;
				if (val_dead_time>PWM_MAX-1) val_dead_time=PWM_MAX-1;

				RegleTempsMort(val_dead_time);
				printf ("\nDead Time regle a %d%%\n", val_dead_time);

				break;
			case 'i':
			case 'I':
				Interface_Info();
				break;
			case '?':
				Interface_Aide();
				break; 
			default:
				printf ("\nCommande inconnue\n");
		}

		commande=buffer;
		commande_prete=0;
	}
}

void Interface_Aide(void)
{
	printf ("\nAide commandes:\n\n");

	printf ("A=xxx -> Vitesse en marche avant [xxx entre -100 et 100 %%]\n");
	printf ("S     -> Stop le moteur [en roue libre]\n\n");
	printf ("F=xxx -> Frein actif [xxx entre 0 et 100 %%]. xxx=0 correspond à S\n");
	printf ("I     -> Infos sur l'etat du systeme [vitesse, avance, etc]\n\n");
	printf ("D     -> Calle le 0 du capteur et reinit le systeme\n\n");
	printf ("Py=xx -> Regle certains parametres du systeme [y=parametre, xx=valeur]\n\n");
	printf ("      y=0 -> Regle l'avance [entre 0 et %d]\n", (int)(_PAS_60_DEGRES_));
	printf ("E     -> Affiche l'etat du systeme\n");
	printf ("R     -> Regle le temps mort de 0 a %d\n", PWM_MAX-1);
	printf ("?     -> Affiche ce menu\n\n");  
}

void Interface_Info(void)
{
int av, p, t, v;
int kp, ki, kd;

	Fourni_stats(&v, &t, &av, &p);

	printf ("\nInfo systeme:\n\n");

	printf ("Version: %d.%d\n\n", _VERSION_MAJEUR_, _VERSION_MINEUR_);
	printf ("Vitesse: %d\n", v);
	printf ("nbr tour moteur: %d\n", t);
	printf ("avance: %d\n", av);
	printf ("Phase moteur: %d\n",p);
	printf ("\n");

	Fourni_coeffs (&kp, &ki, &kd);
	printf ("kp: %d\n", kp);
	printf ("ki: %d\n", ki);
	printf ("kd: %d\n", kd);
	  
	printf ("\n");
}