/*************************************************************************************
** Titre:STRUCT_DONNEES.H				     		                   				**
** Auteurs: Claire DUBOIS, Denis DESMAELE, Sébastien DARDONVILLE, Katia ROULLET	    **
** Date:31/10/2007									                   				**
** Sujet: défini une structure de données                              				**
**************************************************************************************/

#ifndef __STRUCT_DONNEES_H__
#define __STRUCT_DONNEES_H__

/*****************************************************************************
**				Structure données de l'état trottinette    					**
** La structure comprend:													**
** 	- Position de la rame par rapport à sa position initiale				**
**	- Vitesse de la rame													**
******************************************************************************/

typedef struct {
	int position;
	float vitesse;
	}Etat_Trottinette;

#endif