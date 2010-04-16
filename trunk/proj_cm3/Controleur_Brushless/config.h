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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define _AVEC_CAPTEUR_
//define _SANS_CAPTEUR_

#define _RESOLUTION_CAPTEUR_	120
#define _RESOLUTION_ENCODEUR_	(_RESOLUTION_CAPTEUR_*2)
#define _PAS_60_DEGRES_			(_RESOLUTION_ENCODEUR_/6)

#define _VERSION_MAJEUR_		1
#define _VERSION_MINEUR_		0

/* Ne rien modifier après cette ligne */
/*___________________________________________________________________________________________________________*/

#ifdef _AVEC_CAPTEUR_
	#ifdef _SANS_CAPTEUR_
		#error "Impossible de definir _SANS_CAPTEUR_ et _AVEC_CAPTEUR_ en meme temps"
		#error "Faites un choix !"
	#endif /* _SANS_CAPTEUR_ */

	#ifndef _RESOLUTION_CAPTEUR_
		#error "_RESOLUTION_CAPTEUR_ n'est pas defini"
	#endif /* _RESOLUTION_CAPTEUR_ */
#endif /* _AVEC_CAPTEUR_ */

#endif /* _CONFIG_H_ */
