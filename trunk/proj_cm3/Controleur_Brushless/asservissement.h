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

#ifndef _ASSERVISSEMENT_H_
#define _ASSERVISSEMENT_H_

#include "stm_regs.h"

#define _DISTANCE_ROUE_ 213.6283

#define COEFF_KP	0.08
#define COEFF_KI	0.01
#define COEFF_KD	0.04

#define	V_MAX 		25000.0

void Init_Asservissement (void);
void Fourni_stats (int *v, int* t, int* av, int* p);
void Fourni_coeffs (int *kp, int *ki, int *kd);
int asservissement(int consigne);
void Regle_Coeff_Kv(int kv);
void Regle_Coeff_Ki(int ki);
void Regle_Coeff_Kd(int kd);
 
#endif /* _ASSERVISSEMENT_H_ */
