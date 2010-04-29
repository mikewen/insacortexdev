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

#ifndef _CONTROLE_H_
#define _CONTROLE_H_

#include "stm_regs.h"

#include "config.h"

#define CONTROLE_MODE_AVANT 1
#define CONTROLE_MODE_ARRIERE 2
#define CONTROLE_MODE_FREIN 0

void Cale_Moteur(void);
void Init_Moteur(void);
void Init_Controle (void);
void Regle_Controle(int pwm, int mode);
void Regle_Avance(int av);
void Fourni_stats (int *vitesse, int* tour, int* avance, int* phase);
void Fourni_coeffs (int *kp, int *ki, int *kd);
void Regle_Coeff_Kv(int kv);
void Regle_Coeff_Ki(int ki);
void Regle_Coeff_Kd(int kd);

#endif /* _CONTROLE_H_ */

