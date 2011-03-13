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

#ifndef _HACHEUR_H_
#define _HACHEUR_H_

#include "stm_regs.h"
#include "callback.h"
#include "config.h"

#define MOS_OUVERT 0xFFFFFFFF
#define MOS_FERME 0xFFFFFFFE

#define _FREQ_HACHAGE_		(80000U)
#define _PERIODE_PWM_TIM2_ 	(40000000U/_FREQ_HACHAGE_)
#define _PERIODE_PWM_TIM3_ 	_PERIODE_PWM_TIM2_
#define PWM_MAX 			_PERIODE_PWM_TIM2_

void Init_Hacheur (void);
void Commande_Hacheur(int Phase_A, int Phase_B, int Phase_C);
void ActiveITOverflow(eventptr cbk);
void RegleTempsMort(int deadtime);
#endif /* _HACHEUR_H_ */

