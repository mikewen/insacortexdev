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

#ifndef _CAPTEUR_H_
#define _CAPTEUR_H_

#include "stm_regs.h"
#include "callback.h"
#include "config.h"

#define _PERIODE_VITESSE_NULLE_	(unsigned int)0xFFFFFFFF

void Demarre_Capteur(void);
void Init_Capteur (void);
int Lire_Capteur(void);
int Lire_Capteur_Avant(void);
int Lire_Capteur_Arriere(void);
void Ecrire_Capteur(int val);
void Regle_Position_Avant(int val);
void Regle_Position_Arriere(int val);
void Regle_Seuil_Vitesse_Haut(int val);
void Regle_Seuil_Vitesse_Bas(int val);
int Donne_Vitesse(void);
int Donne_nb_tour(void);

#endif /* _CAPTEUR_H_ */

