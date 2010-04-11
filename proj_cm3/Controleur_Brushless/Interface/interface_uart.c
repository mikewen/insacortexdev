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

char buffer[50];
char *commande;

volatile int pourcent_pwm;

FILE Port_COM;

void Init_Interface_uart(void)
{
	printf ("Controleur Brushless\nRéalisation: SDM\nVer: %d.%02d\n\n",_VERSION_MAJEUR_, _VERSION_MINEUR_);

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

				if (pourcent_pwm<0) pourcent_pwm=0;
				if (pourcent_pwm>100) pourcent_pwm=100;

				printf ("En avant à %d%%\n", pourcent_pwm);
				Regle_Controle(pourcent_pwm, CONTROLE_MODE_AVANT);
				break;
			case 'r':
			case 'R':
				sscanf (commande, "%d", &pourcent_pwm);

				if (pourcent_pwm<0) pourcent_pwm=0;
				if (pourcent_pwm>100) pourcent_pwm=100;

				printf ("En arriere à %d%%\n", pourcent_pwm);
				Regle_Controle(pourcent_pwm, CONTROLE_MODE_ARRIERE);
				break;
			case 's':
			case 'S':
				printf ("Arret du moteur (roue libre)\n");
				Regle_Controle(0, CONTROLE_MODE_AVANT);
				break;
			case 'f':
			case 'F':
				break;
			case 'd':
			case 'D':
				break;
			case 'p':
			case 'P':
				break;
			case 'i':
			case 'I':
				Interface_Info();
				break;
			case '?':
				Interface_Aide();
				break; 
			default:
				printf ("Commande inconnue\n");
		}

		commande=buffer;
		commande_prete=0;
	}
}

void Interface_Aide(void)
{
	printf ("\nAide commandes:\n\n");

	printf ("A=xxx -> Vitesse en marche avant [xxx entre 0 et 100 %%]\n");
	printf ("R=xxx -> Vitesse en marche arriere [xxx entre 0 et 100 %%]\n");
	printf ("S     -> Stop le moteur [en roue libre]\n\n");
	printf ("F=xxx -> Frein actif [xxx entre 0 et 100 %%]. xxx=0 correspond à S\n");
	printf ("I     -> Infos sur l'etat du systeme [vitesse, avance, etc]\n\n");
	printf ("D     -> Calle le 0 du capteur et reinit le systeme\n\n");
	printf ("Py=xx -> Regle certains parametres du systeme [y=parametre, xx=valeur]\n\n");
	printf ("      y=0 -> Regle l'avance [entre 0 et %d]\n", (int)((_RESOLUTION_CAPTEUR_*4)/6));
	printf ("?     -> Affiche ce menu\n\n");  
}

void Interface_Info(void)
{
	printf ("\nInfo systeme:\n\n");

	printf ("Version: %d.%d\n", _VERSION_MAJEUR_, _VERSION_MINEUR_);
}